#include "Interactives/DoorBase.h"

ADoorBase::ADoorBase()
{
	PrimaryActorTick.bCanEverTick = PrimaryActorTick.bStartWithTickEnabled = false;
	bIsOpened = false;
	bIsInitiallyOpened = false;
}

void ADoorBase::Reset()
{
	Super::Reset();
	ResetDoor();	
}

void ADoorBase::BeginPlay()
{
	Super::BeginPlay();
	ResetDoor();
}

void ADoorBase::ResetDoor()
{
	bIsOpened = bIsInitiallyOpened;
	if (bIsOpened)
	{
		OnOpenDoor(true);
	}
	else
	{
		OnCloseDoor(true);
	}
}

