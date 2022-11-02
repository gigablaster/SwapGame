#include "Interactives/DoorInteractiveBase.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

ADoorInteractiveBase::ADoorInteractiveBase()
{
	LockCount = 0;
}

void ADoorInteractiveBase::NotifyActorBeginOverlap(AActor* OtherActor)
{
	APawn* Pawn = Cast<APawn>(OtherActor);
	if (Pawn != nullptr && LockCount == 0 && !bIsOpened && Cast<APlayerController>(Pawn->GetController()) != nullptr)
	{
		if (CanOpenDoorFor(Pawn))
		{
			bIsOpened = true;
			OnOpenDoor();	
		}
		else
		{
			OnOpenFailed(Pawn);
		}				
	}
}

void ADoorInteractiveBase::NotifyActorEndOverlap(AActor* OtherActor)
{
	APawn* Pawn = Cast<APawn>(OtherActor);
	if (Pawn != nullptr && bIsOpened && Cast<APlayerController>(Pawn->GetController()) != nullptr && MustCloseDoorFor(Pawn))
	{
		bIsOpened = false;
		OnCloseDoor();
	}
}

void ADoorInteractiveBase::HandleTrigger(APawn* PlayerPawn, AActor* Trigger, ETriggerAction Action)
{
	switch (Action)
	{
		case ETriggerAction::Lock:
			LockCount++;
			if (LockCount == 1)
			{
				OnLocked(PlayerPawn);
			}
			break;
		case ETriggerAction::Unlock:
			LockCount--;
			if (LockCount == 0)
			{
				OnUnlocked(PlayerPawn);
			}
			LockCount = FMath::Max<int>(LockCount, 0); // Sanity
			break;
	}
}
