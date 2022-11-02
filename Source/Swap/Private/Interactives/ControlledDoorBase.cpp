#include "Interactives/ControlledDoorBase.h"

AControlledDoorBase::AControlledDoorBase()
{}

void AControlledDoorBase::HandleTrigger(APawn* PlayerPawn, AActor* Trigger, ETriggerAction Action)
{
	switch (Action)
	{
		case ETriggerAction::Open:
			OnOpenDoor();
			bIsInitiallyOpened = true;
			break;
		case ETriggerAction::Close:
			OnCloseDoor();
			bIsInitiallyOpened = false;
			break;		
	};
}
