// Copyright (c) 2020-2021 Vladimir Kuskov
#pragma once

#include "CoreMinimal.h"
#include "SwapTypes.h"
#include "BlinkTargetInterface.generated.h"

UINTERFACE(meta = (CannotImplementInterfaceInBlueprint))
class SWAP_API UBlinkTargetInterface : public UInterface
{
	GENERATED_BODY()
};

class IBlinkTargetInterface
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	virtual void ShowBlinkTarget(const FBlinkLocation& Location) = 0;

	UFUNCTION(BlueprintCallable)
	virtual void HideBlinkTarget() = 0;
};
