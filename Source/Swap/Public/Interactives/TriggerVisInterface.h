#pragma once

#include "CoreMinimal.h"
#include "SwapTypes.h"
#include "TriggerVisInterface.generated.h"

UINTERFACE(meta = (CannotImplementInterfaceInBlueprint))
class SWAP_API UTriggerVisInterface : public UInterface
{
	GENERATED_BODY()
};

class ITriggerVisInterface
{
	GENERATED_BODY()
public:
	virtual const TArray<FTriggerTarget>& GetEnterTargets()const = 0;
	virtual const TArray<FTriggerTarget>& GetExitTargets()const = 0;
	virtual bool KeepDebugDisplay()const = 0;
};
