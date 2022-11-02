#pragma once

#include "CoreMinimal.h"
#include "SwapTypes.h"
#include "TriggerableInterface.generated.h"

UINTERFACE(meta = (CannotImplementInterfaceInBlueprint))
class SWAP_API UTriggerableInterface : public UInterface
{
	GENERATED_BODY()
};

class ITriggerableInterface
{
	GENERATED_BODY()
public:
	virtual void HandleTrigger(APawn* PlayerPawn, AActor* Trigger, ETriggerAction Action) = 0;
};
