// Copyright (c) 2020-2021 Vladimir Kuskov
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SwapTypes.h"
#include "GuardAnimInstance.generated.h"

UCLASS(Abstract)
class SWAP_API UGuardAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UGuardAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
protected:
	UPROPERTY(BlueprintReadOnly)
	float CharacterSpeed;

	UPROPERTY(BlueprintReadOnly)
	EGuardAlertLevel AlertLevel;

	UPROPERTY(BlueprintReadOnly)
	uint32 bIsMoving : 1;
};
