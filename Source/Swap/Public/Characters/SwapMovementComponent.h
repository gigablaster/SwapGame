// Copyright (c) 2020-2021 Vladimir Kuskov
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SwapMovementComponent.generated.h"

UCLASS()
class SWAP_API USwapMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
public:
	USwapMovementComponent();

	virtual float GetMaxSpeed() const override;
	virtual float GetGravityZ() const override;
};
