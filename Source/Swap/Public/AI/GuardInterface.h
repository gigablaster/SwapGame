#pragma once

#include "CoreMinimal.h"
#include "SwapTypes.h"
#include "GuardInterface.generated.h"

class UPatrolRouteComponent;

UINTERFACE(meta = (CannotImplementInterfaceInBlueprint))
class SWAP_API UGuardInterface : public UInterface
{
	GENERATED_BODY()
};

class IGuardInterface
{
	GENERATED_BODY()
public:
    virtual void AlertLevelChanged(EGuardAlertLevel AlertLevel) = 0;
	virtual UPatrolRouteComponent* GetPatrolRoute()const = 0;	
};
