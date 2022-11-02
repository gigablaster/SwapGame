#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "Characters/KillableInterface.h"
#include "CornerPeekInterface.h"
#include "HeadBobInterface.h"
#include "CameraInterface.h"
#include "BlinkTargetInterface.h"
#include "PlayerCharacterBase.generated.h"

class UAIPerceptionStimuliSourceComponent;
class UCameraComponent;
class UPlayerAttributeSet;
class UCornerPeekComponent;
class UNiagaraComponent;

UCLASS(Abstract)
class SWAP_API APlayerCharacterBase : public ACharacterBase, public ICornerPeekInterface, public IHeadBobInterface, public ICameraInterface, public IKillableInterface, public IBlinkTargetInterface
{
	GENERATED_BODY()
public:
	APlayerCharacterBase();

	// ACharacterBase
	virtual void OnFootstep() override;
	
	// AActor
	virtual void Tick(float DeltaSeconds) override;

	// ACharacter
	virtual void Landed(const FHitResult& Hit) override;

	// APawn
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// ICoverPeekInterface
	virtual ECornerPeekSide GetCornerPeekSide() const override;;

	// IHeadBobInterface
	virtual void GetHeadBob(float& OutVertical, float& OutHorizontal) const override;

	// ICameraInterface
	virtual UCameraComponent* GetCameraComponent()const override;

	// IKillableInterface
	virtual void Kill(APawn* Killer) override;

	// IBlinkTargetInterface
	virtual void ShowBlinkTarget(const FBlinkLocation& Location) override;
	virtual void HideBlinkTarget() override;

	UFUNCTION(BlueprintCallable)
	void ReportNoise(float Level, FName Tag);
	
	FORCEINLINE UCameraComponent* GetFirstPersonCameraComponent()const { return FirstPersonCamera; }
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Components)
	UCameraComponent* FirstPersonCamera;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Components)
	UAIPerceptionStimuliSourceComponent* StimuliSourceComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Components)
	UCornerPeekComponent* CornerPeekComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Components)
	UNiagaraComponent* BlinkTargetComponent;

	UPROPERTY()
	UPlayerAttributeSet* PlayerAttributes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Control")
	float TurnRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Control")
	float LookUpRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Control")
	float TurnMaxAccelerationThreshold;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Control")
	float TurnMaxAccelerationMul;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Control")
	float TurnAccelerationRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Movement")
	float SprintThreshold;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Movement")
	float SprintControlFactor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Camera")
	float InteractionDistance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Footsteps")
	float FootstepMinSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Footsteps")
	float FootstepNormalDistance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Footsteps")
	float FootstepCrouchDistance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Footsteps|Noise")
	float FootstepLoudness;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Footsteps|Noise")
	float MinLandingLoudness;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Footsteps|Noise")
	float MaxLandingLoudness;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Footsteps|Noise")
	float LandingNoNoiseZSpeedThreshold;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Footsteps|Noise")
	float LandingMaxNoiseZSpeedThreshold;
private:
	void UpdateLook(float DeltaTime);
	void UpdateFootsteps(float DeltaTime);
	void UpdateBlinkTarget();
	
	FVector2D DesiredLookSpeed;
	FVector2D CurrentLookSpeed;

	float MovedDistance;
	float HeadBobDirection;
	float HeadBobHValue;
	float HeadBobVValue;

	FGameplayTag CrouchTag;
	FGameplayTag SprintTag;
	FGameplayTag PeekTag;

	uint32 bIsSprinting : 1;
	uint32 bIsPeeking : 1;

	FVector LastBlinkLocation;
	uint32 bShowBlinkSpot : 1;

	bool CanSprint()const;
	float GetTurnControlFactor()const;
	void SetCrouch(bool bCrouch);
	void SetSprint(bool bSprint);
	void SetPeek(bool bPeek);
	void OnTurnAxis(float Value);
	void OnTurnRateAxis(float Value);
	void OnLookUpAxis(float Value);
	void OnLookUpRateAxis(float Value);
	void OnForwardAxis(float Value);
	void OnRightAxis(float Value);
	void OnCrouchPressed();
	void OnSprintPressed();
	void OnSprintReleased();
	void OnPeekPressed();
	void OnPeekReleased();
};
