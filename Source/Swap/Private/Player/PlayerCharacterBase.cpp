#include "Player/PlayerCharacterBase.h"
#include "Player/PlayerCharacterBase.h"
#include "Player/PlayerAttributeSet.h"
#include "Player/CornerPeekComponent.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "Camera/CameraComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Hearing.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NiagaraComponent.h"
#include "Components/CapsuleComponent.h"

static const FName TurnAxis(TEXT("Turn"));
static const FName TurnRateAxis(TEXT("TurnRate"));
static const FName LookUpAxis(TEXT("LookUp"));
static const FName LookUpRateAxis(TEXT("LookUpRate"));
static const FName MoveForwardAxis(TEXT("MoveForward"));
static const FName MoveRightAxis(TEXT("MoveRight"));
static const FName CrouchAction(TEXT("Crouch"));
static const FName SprintAction(TEXT("Sprint"));
static const FName PeekAction(TEXT("Peek"));

const FName NoiseFootstepTag = "Footstep";
const FName NoiseLandingTag = "Landing";

APlayerCharacterBase::APlayerCharacterBase()
{
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(GetRootComponent());
	FirstPersonCamera->bUsePawnControlRotation = true;

	StimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimuliSource"));	

	PlayerAttributes = CreateDefaultSubobject<UPlayerAttributeSet>("PlayerAttributes");

	CornerPeekComponent = CreateDefaultSubobject<UCornerPeekComponent>("CornerPeek");

	BlinkTargetComponent = CreateDefaultSubobject<UNiagaraComponent>("BlinkTarget");
	BlinkTargetComponent->SetupAttachment(GetRootComponent());
	BlinkTargetComponent->bAutoActivate = false;

	TurnRate = 45.0f;
	LookUpRate = 45.0f;
	TurnAccelerationRate = 3.0f;
	TurnMaxAccelerationMul = 2.0f;
	TurnMaxAccelerationThreshold = 0.95f;
	InteractionDistance = 150.0f;

	FootstepMinSpeed = 100.0f;
	FootstepNormalDistance = 150.0f;
	FootstepCrouchDistance = 50.0f;

	SprintThreshold = 100.0f;
	SprintControlFactor = 0.333f;

	bIsSprinting = false;
	bIsPeeking = false;

	FootstepLoudness = 0.25f;
	MinLandingLoudness = 0.25f;
	MaxLandingLoudness = 1.0f;
	LandingNoNoiseZSpeedThreshold = 200.0f;
	LandingMaxNoiseZSpeedThreshold = 500.0f;

	CrouchTag = FGameplayTag::RequestGameplayTag("Character.Player.Crouch");
	SprintTag = FGameplayTag::RequestGameplayTag("Character.Player.Sprint");
	PeekTag = FGameplayTag::RequestGameplayTag("Character.Player.Peek");
}

void APlayerCharacterBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (bIsSprinting && !CanSprint())
	{
		SetSprint(false);
	}
	if (bIsPeeking && CornerPeekComponent->GetCurrentSide() == ECornerPeekSide::None)
	{
		SetPeek(false);
	}
	UpdateLook(DeltaSeconds);	
	UpdateFootsteps(DeltaSeconds);
	UpdateBlinkTarget();
}

void APlayerCharacterBase::UpdateBlinkTarget()
{
	BlinkTargetComponent->SetWorldLocation(LastBlinkLocation);
}

void APlayerCharacterBase::UpdateFootsteps(float DeltaTime)
{
	const float Speed = GetVelocity().Size();
	if (GetCharacterMovement()->IsFalling() || Speed < FootstepMinSpeed)
	{
		HeadBobHValue = FMath::FInterpTo(HeadBobHValue, 0.0f, DeltaTime, 2.0f);
		HeadBobVValue = FMath::FInterpTo(HeadBobVValue, 0.0f, DeltaTime, 3.0f);
		MovedDistance = 0.0f;
		return;
	}

	const float MinMoveDistance = bIsCrouched ? FootstepCrouchDistance : FootstepNormalDistance;
	const float Step = FMath::Min(MinMoveDistance, Speed * DeltaTime);
	HeadBobHValue += HeadBobDirection * Step / MinMoveDistance;
	HeadBobVValue = FMath::Sin(FMath::Abs(MovedDistance / MinMoveDistance) * PI);
	MovedDistance = FMath::Min(MinMoveDistance, MovedDistance + Step);	
	if (MovedDistance >= MinMoveDistance)
	{
		MovedDistance = 0.0f;
		HeadBobDirection *= -1;
		OnFootstep();
	}

	HeadBobHValue = FMath::Clamp(HeadBobHValue, -1.0f, 1.0f);
	HeadBobVValue = FMath::Clamp(HeadBobVValue, 0.0f, 1.0f);
}

void APlayerCharacterBase::GetHeadBob(float& OutVertical, float& OutHorizontal) const
{
	OutVertical = HeadBobVValue;
	OutHorizontal = HeadBobHValue;
}

void APlayerCharacterBase::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	float ZSpeed = GetVelocity().Z;
	if (FMath::Abs(ZSpeed) > LandingNoNoiseZSpeedThreshold)
	{
		float Loudness = FMath::GetMappedRangeValueClamped(FVector2D(LandingNoNoiseZSpeedThreshold, LandingMaxNoiseZSpeedThreshold), FVector2D(MinLandingLoudness, MaxLandingLoudness), ZSpeed);
		ReportNoise(Loudness, NoiseLandingTag);
	}
	MovedDistance = 0.0f;
}

void APlayerCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	MovedDistance = 0.0f;
	HeadBobDirection = 1.0f;
	HeadBobHValue = 0.0f;
	HeadBobVValue = 0.0f;
	SetCrouch(false);
	SetSprint(false);
	SetPeek(false);
}

void APlayerCharacterBase::UnPossessed()
{
	Super::UnPossessed();
}

void APlayerCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	check(PlayerInputComponent);

	AbilitySystemComponent->BindAbilityActivationToInputComponent(PlayerInputComponent, FGameplayAbilityInputBinds("Confirm", "Cancel", "EPlayerAction"));
	
	PlayerInputComponent->BindAxis(TurnAxis, this, &APlayerCharacterBase::OnTurnAxis);
	PlayerInputComponent->BindAxis(TurnRateAxis, this, &APlayerCharacterBase::OnTurnRateAxis);
	PlayerInputComponent->BindAxis(LookUpAxis, this, &APlayerCharacterBase::OnLookUpAxis);
	PlayerInputComponent->BindAxis(LookUpRateAxis, this, &APlayerCharacterBase::OnLookUpRateAxis);
	PlayerInputComponent->BindAxis(MoveForwardAxis, this, &APlayerCharacterBase::OnForwardAxis);
	PlayerInputComponent->BindAxis(MoveRightAxis, this, &APlayerCharacterBase::OnRightAxis);

	PlayerInputComponent->BindAction(CrouchAction, IE_Pressed, this, &APlayerCharacterBase::OnCrouchPressed);

	PlayerInputComponent->BindAction(SprintAction, IE_Pressed, this, &APlayerCharacterBase::OnSprintPressed);
	PlayerInputComponent->BindAction(SprintAction, IE_Released, this, &APlayerCharacterBase::OnSprintReleased);

	PlayerInputComponent->BindAction(PeekAction, IE_Pressed, this, &APlayerCharacterBase::OnPeekPressed);
	PlayerInputComponent->BindAction(PeekAction, IE_Released, this, &APlayerCharacterBase::OnPeekReleased);
}

void APlayerCharacterBase::OnForwardAxis(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value);
}

void APlayerCharacterBase::OnRightAxis(float Value)
{
	AddMovementInput(GetActorRightVector(), Value);
}

void APlayerCharacterBase::OnTurnAxis(float Value)
{
	AddControllerYawInput(Value * GetTurnControlFactor());
}

void APlayerCharacterBase::OnLookUpAxis(float Value)
{
	AddControllerPitchInput(Value * GetTurnControlFactor());
}

void APlayerCharacterBase::OnTurnRateAxis(float Value)
{
	DesiredLookSpeed.X = Value * GetTurnControlFactor();
}

void APlayerCharacterBase::OnLookUpRateAxis(float Value)
{
	DesiredLookSpeed.Y = Value * GetTurnControlFactor();
}

void APlayerCharacterBase::OnCrouchPressed()
{
	SetCrouch(!bIsCrouched);
}

void APlayerCharacterBase::SetCrouch(bool bCrouch)
{
	if (!bIsCrouched && bCrouch)
	{
		SetSprint(false);
		Crouch(true);
		AbilitySystemComponent->SetLooseGameplayTagCount(CrouchTag, 1);
	}
	else if (bIsCrouched && !bCrouch)
	{
		UnCrouch(true);
		AbilitySystemComponent->SetLooseGameplayTagCount(CrouchTag, 0);
	}
}

void APlayerCharacterBase::OnSprintPressed()
{
	SetSprint(true);
}

void APlayerCharacterBase::OnSprintReleased()
{
	SetSprint(false);
}


void APlayerCharacterBase::SetSprint(bool bSprint)
{
	if (!bIsSprinting && bSprint && CanSprint())
	{
		SetCrouch(false);
		SetPeek(false);
		bIsSprinting = true;
		AbilitySystemComponent->SetLooseGameplayTagCount(SprintTag, 1);		
	}
	else if (bIsSprinting && !bSprint)
	{
		bIsSprinting = false;		
		AbilitySystemComponent->SetLooseGameplayTagCount(SprintTag, 0);
	}
}

static bool IsNeedImmediateApply(float Current, float Desired)
{
	return (FMath::Abs(Desired) < FMath::Abs(Current)) || (FMath::Sign(Current) != FMath::Sign(Desired));
}

void APlayerCharacterBase::UpdateLook(float DeltaTime)
{
	FVector2D Target = DesiredLookSpeed.Size() >= TurnMaxAccelerationThreshold ? DesiredLookSpeed * TurnMaxAccelerationMul : DesiredLookSpeed;

	CurrentLookSpeed.X = IsNeedImmediateApply(CurrentLookSpeed.X, Target.X) ? Target.X : CurrentLookSpeed.X;
	CurrentLookSpeed.Y = IsNeedImmediateApply(CurrentLookSpeed.Y, Target.Y) ? Target.Y : CurrentLookSpeed.Y;
	
	CurrentLookSpeed = FMath::Vector2DInterpTo(CurrentLookSpeed, Target, DeltaTime, TurnAccelerationRate);

	AddControllerYawInput(CurrentLookSpeed.X * DeltaTime * TurnRate);
	AddControllerPitchInput(CurrentLookSpeed.Y * DeltaTime * LookUpRate);	
}

ECornerPeekSide APlayerCharacterBase::GetCornerPeekSide() const
{
	return bIsPeeking ? CornerPeekComponent->GetCurrentSide() : ECornerPeekSide::None;	
}

UCameraComponent* APlayerCharacterBase::GetCameraComponent() const
{
	return FirstPersonCamera;
}

bool APlayerCharacterBase::CanSprint() const
{
	const FVector Direction = GetVelocity().GetSafeNormal2D();
	const float Dot = FVector::DotProduct(Direction, GetActorForwardVector());
	return GetVelocity().Size2D() >= SprintThreshold && Dot >= 0.8f && !bIsPeeking;
}

float APlayerCharacterBase::GetTurnControlFactor() const
{
	return bIsSprinting ? SprintControlFactor : 1.0f;
}

void APlayerCharacterBase::ReportNoise(float Level, FName Tag)
{
	UAISense_Hearing::ReportNoiseEvent(GetWorld(), GetActorLocation(), Level * PlayerAttributes->GetNoiseLevel(), this, 0, Tag);
}

void APlayerCharacterBase::OnFootstep()
{
	Super::OnFootstep();
	ReportNoise(FootstepLoudness, NoiseFootstepTag);
}

void APlayerCharacterBase::Kill(APawn* Killer)
{
	if (Cast<AAIController>(Killer->GetController()) && IsAlive())
	{
		AbilitySystemComponent->SetTagMapCount(DyingTag, 1);
	}
}

void APlayerCharacterBase::SetPeek(bool bPeek)
{
	if (bIsSprinting && bPeek && !bIsPeeking)
	{
		return;
	}
	if (bPeek && CornerPeekComponent->GetCurrentSide() != ECornerPeekSide::None)
	{
		bIsPeeking = true;
	}
	else
	{
		bIsPeeking = false;
	}
	AbilitySystemComponent->SetTagMapCount(PeekTag, bIsPeeking ? 1 : 0);
}

void APlayerCharacterBase::OnPeekPressed()
{
	SetPeek(true);
}

void APlayerCharacterBase::OnPeekReleased()
{
	SetPeek(false);
}

void APlayerCharacterBase::ShowBlinkTarget(const FBlinkLocation& Location)
{
	
	if (Location.Spot == EBlinkSpot::None)
	{
		HideBlinkTarget();
		return;
	}
	bShowBlinkSpot = true;
	BlinkTargetComponent->Activate();
	LastBlinkLocation = Location.TargetLocation + FVector::DownVector * GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
}

void APlayerCharacterBase::HideBlinkTarget()
{
	BlinkTargetComponent->Deactivate();
}
