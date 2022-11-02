#pragma once

#include "CoreMinimal.h"
#include "TriggerableInterface.h"
#include "TriggerableActorBase.generated.h"

UCLASS(Abstract)
class SWAP_API ATriggerableActorBase : public AActor, public ITriggerableInterface
{
	GENERATED_BODY()
public:
	ATriggerableActorBase();

	virtual void HandleTrigger(APawn* PlayerPawn, AActor* Trigger, ETriggerAction Action) override;
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnTriggered(APawn* PlayerPawn, AActor* Trigger, ETriggerAction Action);
};
