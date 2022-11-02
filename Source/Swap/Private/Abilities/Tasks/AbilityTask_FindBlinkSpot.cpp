// Copyright (c) 2020-2021 Vladimir Kuskov
#include "Abilities/Tasks/AbilityTask_FindBlinkSpot.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Player/CameraInterface.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"

static const FName TraceTag(TEXT("Blink"));

static const float FlatThreshold = 0.76f; // ~40 deg
static const float SafetyOffset = 10.0f;

UAbilityTask_FindBlinkSpot* UAbilityTask_FindBlinkSpot::FindBlinkSpot(UGameplayAbility* OwningAbility, float MaxDistance, float MaxLedgeCheck, float Step)
{
	UAbilityTask_FindBlinkSpot* Task = NewAbilityTask<UAbilityTask_FindBlinkSpot>(OwningAbility);
	ACharacter* Character = Cast<ACharacter>(OwningAbility->GetAvatarActorFromActorInfo());
	check(Character);
	
	Task->bTickingTask = true;
	Task->MaxDistance = MaxDistance;
	Task->MaxLedgeCheck = MaxLedgeCheck;
	Task->Step = Step;
	Task->Shape = Character->GetCapsuleComponent()->GetCollisionShape();

	return Task;
}

void UAbilityTask_FindBlinkSpot::TickTask(float DeltaTime)
{
	if (UWorld* World = GetWorld())
	{
		if (ICameraInterface* CameraInterface = Cast<ICameraInterface>(GetAvatarActor()))
		{
			const FVector Origin = CameraInterface->GetCameraComponent()->GetComponentLocation();
			const FVector Direction = CameraInterface->GetCameraComponent()->GetForwardVector();
			FVector End = Origin + Direction * MaxDistance;
			FVector Location, Normal;
			if (CheckDirectLine(World, Origin, Direction, Location, Normal))
			{
				const FVector AboveGroundLocation = Location + (Shape.GetCapsuleHalfHeight() + SafetyOffset) * FVector::UpVector;

				// Hit the ground.
				if (FVector::DotProduct(Normal, FVector::UpVector) >= FlatThreshold && ValidateSpot(World, AboveGroundLocation, Origin))
				{
					BlinkSpotUpdate.Broadcast(FBlinkLocation(EBlinkSpot::Ground, Location, Normal, AboveGroundLocation));
					return;	
				}

				End = Location + Normal * (Shape.GetCapsuleRadius() + Step);

				// Check for ledge
				// Trace up, then forward, then down. Validate spot. If valid - then we have ledge.
				FVector LedgeLocation, LedgeNormal;
				if (CheckLedge(World, Location, Normal, LedgeLocation, LedgeNormal))
				{
					const FVector AboveLedgeLocation = LedgeLocation + (Shape.GetCapsuleHalfHeight() + SafetyOffset) * LedgeNormal;					
					// Must be kind of flat and valid
					if (FVector::DotProduct(LedgeNormal, FVector::UpVector) >= FlatThreshold && ValidateSpot(World, AboveLedgeLocation, Origin))
					{
						BlinkSpotUpdate.Broadcast(FBlinkLocation(EBlinkSpot::Ledge, End, Normal.GetSafeNormal2D(), AboveLedgeLocation));
						return;
					}
				}
			}

			// So we must teleport in air
			if (ValidateSpot(World, End, Origin))
			{
				//BlinkSpotUpdate.Broadcast(EBlinkSpot::Air, End, End);
				BlinkSpotUpdate.Broadcast(FBlinkLocation(EBlinkSpot::Air, End, -Direction.GetSafeNormal2D(), End));
			}
			else
			{
				BlinkSpotUpdate.Broadcast(FBlinkLocation());
			}
			
			return;		
		}	
	}

	EndTask();
}

bool UAbilityTask_FindBlinkSpot::CheckDirectLine(UWorld* World, const FVector& Origin, const FVector& Direction, FVector& OutLocation, FVector& OutNormal) const
{
	check(World);
	
	const FCollisionQueryParams Params(TraceTag, false, GetAvatarActor());
	const FVector End = Origin + Direction * MaxDistance;
	FHitResult Result;
	if (World->LineTraceSingleByChannel(Result, Origin, End, ECC_Visibility, Params))
	{
		OutLocation = Result.Location;
		OutNormal = Result.Normal;
		return true;
	}

	return false;
}

bool UAbilityTask_FindBlinkSpot::ValidateSpot(UWorld* World, const FVector& Spot, const FVector& CameraLocation) const
{
	check(World);
	
	FCollisionQueryParams Params(TraceTag, false, GetAvatarActor());
	FHitResult Result;
	return !World->LineTraceSingleByChannel(Result, Spot, CameraLocation, ECC_Visibility, Params) && !World->SweepSingleByChannel(Result, Spot, Spot, FQuat::Identity, ECC_Visibility, Shape, Params);
}

bool UAbilityTask_FindBlinkSpot::CheckLedge(UWorld* World, const FVector& HitPlace, const FVector& Normal, FVector& OutLocation, FVector& OutNormal) const
{
	check(World);

	FHitResult ResultUp;
	const FVector StartUp = HitPlace + Normal * Step * 1.5f;
	FVector Right = FVector::CrossProduct(Normal, FVector::UpVector);
	FVector RealUp = FVector::CrossProduct(Right, Normal);
	const FVector EndUp = StartUp + RealUp * MaxLedgeCheck;
	const FCollisionQueryParams Params(TraceTag, false, GetAvatarActor());
	FVector UpPosition = EndUp;
	const float DesiredHeight = Shape.GetCapsuleHalfHeight() * 2.0f;
	if (World->LineTraceSingleByChannel(ResultUp, StartUp, EndUp, ECC_Visibility, Params))
	{
		if (ResultUp.Distance < DesiredHeight)
		{
			return false;
		}
		UpPosition = ResultUp.Location + FVector::DownVector * Step;
	}
	const FVector LimitDown = UpPosition - Normal * (Shape.GetCapsuleRadius() * 2.0f);
	FHitResult ForwardResult;
	if (World->LineTraceSingleByChannel(ForwardResult, UpPosition, LimitDown, ECC_Visibility, Params))
	{
		return false;
	}
	for (float Distance = Step; Distance < Shape.GetCapsuleRadius() * 2.0f; Distance += Step)
	{
		const FVector StartDown = UpPosition - Normal * Distance;
		const FVector EndDown = StartDown + FVector::DownVector * MaxLedgeCheck;
		FHitResult DownResult;
		if (World->LineTraceSingleByChannel(DownResult, StartDown, EndDown, ECC_Visibility, Params))
		{
			OutLocation = DownResult.Location;
			OutNormal = DownResult.Normal;

			return true;
		}	
	}	

	return false;
}
