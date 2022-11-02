// Copyright (c) 2020-2021 Vladimir Kuskov
#pragma once

#include "CoreMinimal.h"
#include "SwapTypes.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_FindBlinkSpot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBlinkSpotUpdateDelegate, FBlinkLocation, Location);

UCLASS()
class UAbilityTask_FindBlinkSpot : public UAbilityTask
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FBlinkSpotUpdateDelegate BlinkSpotUpdate;

	UFUNCTION(BlueprintCallable, Category="Abilities|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_FindBlinkSpot* FindBlinkSpot(UGameplayAbility* OwningAbility, float MaxDistance, float MaxLedgeCheck, float Step);

	virtual void TickTask(float DeltaTime) override;
private:
	bool CheckDirectLine(UWorld* World, const FVector& Origin, const FVector& Direction, FVector& OutLocation, FVector& OutNormal)const;
	bool CheckLedge(UWorld* World, const FVector& HitPlace, const FVector& Normal, FVector& OutLocation, FVector& OutNormal)const;
	bool ValidateSpot(UWorld* World, const FVector& Spot, const FVector& CameraLocation)const;
	float MaxDistance;
	float MaxLedgeCheck;
	float Step;
	FCollisionShape Shape;
};
