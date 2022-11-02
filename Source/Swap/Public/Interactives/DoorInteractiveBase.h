#pragma once

#include "CoreMinimal.h"
#include "Interactives/DoorBase.h"
#include "Interactives/TriggerableInterface.h"
#include "DoorInteractiveBase.generated.h"

UCLASS(Abstract)
class SWAP_API ADoorInteractiveBase : public ADoorBase, public ITriggerableInterface
{
	GENERATED_BODY()
public:
	ADoorInteractiveBase();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	virtual void HandleTrigger(APawn* PlayerPawn, AActor* Trigger, ETriggerAction Action) override;
protected:
	UFUNCTION(BlueprintImplementableEvent)
	bool CanOpenDoorFor(APawn* Pawn)const;

	UFUNCTION(BlueprintImplementableEvent)
    bool MustCloseDoorFor(APawn* Pawn)const;

	UFUNCTION(BlueprintImplementableEvent)
	void OnLocked(APawn* Pawn);

	UFUNCTION(BlueprintImplementableEvent)
	void OnUnlocked(APawn* Pawn);

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	int LockCount;
};
