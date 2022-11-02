#pragma once

#include "CoreMinimal.h"
#include "TriggerableInterface.h"
#include "Engine/TriggerBox.h"
#include "TriggerAreaBase.generated.h"

UCLASS(Abstract)
class SWAP_API ATriggerAreaBase : public ATriggerBox, public ITriggerableInterface
{
	GENERATED_BODY()
public:
	ATriggerAreaBase();
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	void SetTriggerActive(bool bActive)
	{
		bIsActive = bActive;
	}
	bool IsTriggerActive()const
	{
		return bIsActive;
	}
protected:
	virtual void HandleTrigger(APawn* PlayerPawn, AActor* Trigger, ETriggerAction Action) override;
	
	virtual void HandleEnter(APawn* Pawn);
	virtual void HandleExit(APawn* Pawn);
	virtual bool CanBeTriggered(APawn* Pawn)const;

	UPROPERTY(BlueprintReadWrite, EditInstanceOnly)
	uint32 bIsActive : 1;
};
