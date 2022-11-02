#include "Interactives/TriggerAreaBase.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"

ATriggerAreaBase::ATriggerAreaBase()
{
	bIsActive = true;
	PrimaryActorTick.bCanEverTick = PrimaryActorTick.bStartWithTickEnabled = false;
}

void ATriggerAreaBase::NotifyActorBeginOverlap(AActor* OtherActor)
{
	APawn* Pawn = Cast<APawn>(OtherActor);
	if (bIsActive && Pawn != nullptr && Cast<APlayerController>(Pawn->Controller) != nullptr && CanBeTriggered(Pawn))
	{
		HandleEnter(Pawn);
	}
}

void ATriggerAreaBase::NotifyActorEndOverlap(AActor* OtherActor)
{
	APawn* Pawn = Cast<APawn>(OtherActor);
	if (bIsActive && Pawn != nullptr && Cast<APlayerController>(Pawn->Controller) != nullptr && CanBeTriggered(Pawn))
	{
		HandleExit(Pawn);
	}
}

bool ATriggerAreaBase::CanBeTriggered(APawn* Pawn) const
{
	return true;
}

void ATriggerAreaBase::HandleEnter(APawn* Pawn)
{}

void ATriggerAreaBase::HandleExit(APawn* Pawn)
{}

void ATriggerAreaBase::HandleTrigger(APawn* PlayerPawn, AActor* Trigger, ETriggerAction Action)
{
	switch (Action)
	{
		case ETriggerAction::Activate:
			bIsActive = true;
			break;
		case ETriggerAction::Deactivate:
			bIsActive = false;
			break;
	}
}
