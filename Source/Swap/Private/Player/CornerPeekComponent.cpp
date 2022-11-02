#include "Player/CornerPeekComponent.h"

#include "Camera/CameraComponent.h"
#include "Player/PlayerCharacterBase.h"
#include "Engine/World.h"

static const FName TraceTag(TEXT("CornerPeek"));

UCornerPeekComponent::UCornerPeekComponent()
{
	PrimaryComponentTick.bCanEverTick = PrimaryComponentTick.bStartWithTickEnabled = true;
	SideDistance = 80.0f;
	UpDistance = 60.0f;
	ForwardDistance = 40.0f;
	EmptyCheckDistance = 200.0f;
	Step = 10.0f;
	Side = ECornerPeekSide::None;
}

void UCornerPeekComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UWorld* World = GetWorld();
	if (APlayerCharacterBase* PlayerCharacter = Cast<APlayerCharacterBase>(GetOwner()))
	{
		FVector WallContact, WallNormal;
		const FVector Forward = PlayerCharacter->GetFirstPersonCameraComponent()->GetForwardVector().GetSafeNormal2D();
		const FVector Origin = PlayerCharacter->GetFirstPersonCameraComponent()->GetComponentLocation();
		if (CheckIfWallAhead(World, Origin, Forward, FMath::DegreesToRadians(30.0f), WallContact, WallNormal))
		{
			const FVector Right = FVector::CrossProduct(WallNormal, FVector::UpVector);
			const float Dot = FVector::DotProduct(Forward, Right);
			const bool bLeft = Dot < -0.1f || (Dot >= -0.2f && Dot < 0.2);
			const bool bRight = Dot > 0.1f || (Dot < 0.2f && Dot > -0.2f);
			const float LeftDistance = bLeft ? FindClosestEmptySpaceInDirection(World, Origin, -Right, -WallNormal, SideDistance) : -1;
			const float RightDistance = bRight ? FindClosestEmptySpaceInDirection(World, Origin, Right, -WallNormal, SideDistance) : -1;
			if (LeftDistance > 0 && LeftDistance > RightDistance)
			{
				Side = ECornerPeekSide::Left;
			}
			else if (RightDistance > 0 && RightDistance > LeftDistance)
			{
				Side = ECornerPeekSide::Right;
			}
			else
			{
				Side = ECornerPeekSide::None;
			}

			return;
		}		
	}
	Side = ECornerPeekSide::None;
}

bool UCornerPeekComponent::CheckIfWallAhead(UWorld* World, const FVector& Origin, const FVector& Forward, FVector& OutOrigin, FVector& OutNormal) const
{
	const FVector End = Origin + Forward * ForwardDistance;
	FHitResult Result;
	FCollisionQueryParams Params(TraceTag, true, GetOwner());;
	if (World->LineTraceSingleByChannel(Result, Origin, End, ECC_Visibility, Params))
	{
		OutOrigin = Result.Location;
		OutNormal = Result.Normal;
		return true;
	}

	return false;
}

bool UCornerPeekComponent::CheckIfWallAhead(UWorld* World, const FVector& Origin, const FVector& Forward, float Spread, FVector& OutOrigin, FVector& OutNormal) const
{
	if (!CheckIfWallAhead(World, Origin, Forward, OutOrigin, OutNormal))
	{
		if (!CheckIfWallAhead(World, Origin, FRotator(0, Spread, 0).RotateVector(Forward), OutOrigin, OutNormal))
		{
			if (!CheckIfWallAhead(World, Origin, FRotator(0, -Spread, 0).RotateVector(Forward), OutOrigin, OutNormal))
			{
				return false;
			}
		}
	}
	
	return true;
}


float UCornerPeekComponent::FindClosestEmptySpaceInDirection(UWorld* World, const FVector& Origin, const FVector& Direction, const FVector& Forward, float Distance) const
{
	int CountInRow = 0;
	float BestDistance = -1.0f;
	FCollisionQueryParams Params(TraceTag, true, GetOwner());
	for (float CurrentDistance = Step; CurrentDistance < Distance; CurrentDistance += Step)
	{
		const FVector Start = Origin + Direction * CurrentDistance;
		const FVector End = Start + Forward * EmptyCheckDistance;
		FHitResult Result;
		if (World->LineTraceSingleByChannel(Result, Start, End, ECC_Visibility, Params))
		{
			CountInRow = 0;
		}
		else
		{
			if (CountInRow == 0)
			{
				BestDistance = CurrentDistance;
			}
			CountInRow++;
			if (CountInRow >= 2)
			{
				return BestDistance;
			}
		}
	}

	return -1.0f;
}
