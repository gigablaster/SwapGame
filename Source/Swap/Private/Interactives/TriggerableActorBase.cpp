#include "Interactives/TriggerableActorBase.h"

ATriggerableActorBase::ATriggerableActorBase()
{
	PrimaryActorTick.bCanEverTick = PrimaryActorTick.bStartWithTickEnabled = false;	
}

void ATriggerableActorBase::HandleTrigger(APawn* PlayerPawn, AActor* Trigger, ETriggerAction Action)
{
	if (PlayerPawn != nullptr && Trigger != nullptr)
	{
		OnTriggered(PlayerPawn, Trigger, Action);
	}
}
