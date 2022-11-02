#pragma once

#include "CoreMinimal.h"
#include "SwapTypes.h"
#include "GuardControllerInterface.generated.h"

UINTERFACE(meta = (CannotImplementInterfaceInBlueprint))
class SWAP_API UGuardControllerInterface : public UInterface
{
	GENERATED_BODY()
};

class IGuardControllerInterface
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
    virtual void SetAlertLevel(EGuardAlertLevel AlertLevel) = 0;
	UFUNCTION(BlueprintCallable)
    virtual EGuardAlertLevel GetAlertLevel()const = 0;
	UFUNCTION(BlueprintCallable)
    virtual void PushInterest(const FVector& Location, int Priority, float Range = 300.0f) = 0;
	UFUNCTION(BlueprintCallable)
    virtual void PopInterest() = 0;
};
