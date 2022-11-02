#pragma once

#include "CoreMinimal.h"
#include "DoorBase.h"
#include "TriggerableInterface.h"
#include "ControlledDoorBase.generated.h"

UCLASS(Abstract)
class SWAP_API AControlledDoorBase : public ADoorBase, public ITriggerableInterface
{
	GENERATED_BODY()
public:
	AControlledDoorBase();

	virtual void HandleTrigger(APawn* PlayerPawn, AActor* Trigger, ETriggerAction Action) override;
};
