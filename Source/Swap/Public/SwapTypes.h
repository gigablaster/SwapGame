#pragma once

#include "CoreMinimal.h"
#include "SwapTypes.generated.h"

extern const FName NoiseFootstepTag;
extern const FName NoiseLandingTag;

UENUM(Blueprintable)
enum class ETriggerAction : uint8
{
	Default,
    Activate,
    Deactivate,
    Open,
    Close,
	Lock,
	Unlock,
	Pause,
	Resume,
	PlayReverse
};

UENUM(Blueprintable)
enum class EGuardAlertLevel : uint8
{
	None = 0,
	Normal,
    Alert,
    Investigate,
    Attack
};

UENUM(Blueprintable)
enum class EPlayerAction : uint8
{
	Confirm,
	Cancel,
	Interact,
	Blink
};

USTRUCT(BlueprintType)
struct FTriggerTarget
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	AActor* Target;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ETriggerAction Action;
};

UENUM()
enum class ECornerPeekSide : uint8
{
	None,
    Left,
    Right,
    Top
};

UENUM(Blueprintable)
enum class EBlinkSpot : uint8
{
	None,
    Ground,
    Air,
    Ledge
};

USTRUCT(BlueprintType)
struct FBlinkLocation
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	EBlinkSpot Spot;

	UPROPERTY(BlueprintReadOnly)
	FVector MarkerLocation;

	UPROPERTY(BlueprintReadOnly)
	FVector MarkerNormal;

	UPROPERTY(BlueprintReadOnly)
	FVector TargetLocation;

	FBlinkLocation()
        : Spot(EBlinkSpot::None)
        , MarkerLocation(FVector::ZeroVector)
        , MarkerNormal(FVector::ZeroVector)
        , TargetLocation(FVector::ZeroVector)
	{}

	FBlinkLocation(EBlinkSpot Spot, const FVector& MarkerLocation, const FVector& MarkerNormal, const FVector& TargetLocation)
        : Spot(Spot)
        , MarkerLocation(MarkerLocation)
        , MarkerNormal(MarkerNormal)
        , TargetLocation(TargetLocation)
	{}
};

