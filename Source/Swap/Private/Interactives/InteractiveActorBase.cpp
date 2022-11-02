#include "Interactives/InteractiveActorBase.h"

AInteractiveActorBase::AInteractiveActorBase()
{
	PrimaryActorTick.bCanEverTick = PrimaryActorTick.bStartWithTickEnabled = false;
	bIsActivated = true;
}

void AInteractiveActorBase::BeginPlay()
{
	Super::BeginPlay();
	bIsInteracting = false;
	CurrentProgress = 0;
}

void AInteractiveActorBase::Reset()
{
	Super::Reset();
	bIsInteracting = false;
	CurrentProgress = 0;
}

bool AInteractiveActorBase::CanInteract(APawn* PlayerPawn) const
{
	return bIsActivated;
}

void AInteractiveActorBase::StartInteract(APawn* PlayerPawn)
{
	if (!bIsInteracting && bIsActivated)
	{
		bIsInteracting = true;
		OnInteractionStarted(PlayerPawn);
	}
}

void AInteractiveActorBase::StopInteract(APawn* PlayerPawn)
{
	if (bIsInteracting && bIsActivated)
	{
		bIsInteracting = false;
		CurrentProgress = 0;
		OnInteractionFailed(PlayerPawn);
	}
}

void AInteractiveActorBase::StartLooking(APawn* PlayerPawn)
{
	if (bIsActivated)
	{
		OnPlayerStarLooking(PlayerPawn);
	}
}

void AInteractiveActorBase::StopLooking(APawn* PlayerPawn)
{
	if (bIsActivated)
	{
		CurrentProgress = 0;
		OnPlayerStopLooking(PlayerPawn);
	}
}

float AInteractiveActorBase::GetInteractionProgress(APawn* PlayerPawn) const
{
	if (bIsActivated && bIsInteracting)
	{
		return CurrentProgress;
	}

	return 0;
}

FText AInteractiveActorBase::GetInteractionHint(APawn* PlayerPawn) const
{
	if (bIsActivated)
	{
		return OnGetInteractionHint(PlayerPawn);
	}

	return FText::GetEmpty();
}

void AInteractiveActorBase::UpdateInteraction(APawn* PlayerPawn, float DeltaTime)
{
	if (bIsActivated && bIsInteracting)
	{
		if (OnUpdateInteraction(PlayerPawn, DeltaTime))
		{
			bIsInteracting = false;
			OnInteractionSucceeded(PlayerPawn);
		}
	}
}

void AInteractiveActorBase::HandleTrigger(APawn* PlayerPawn, AActor* Trigger, ETriggerAction Action)
{
	switch (Action)
	{
		case ETriggerAction::Activate:
			bIsActivated = true;
			OnTriggerActivated();
			break;
		case ETriggerAction::Deactivate:
			bIsActivated = false;
			bIsInteracting = false;
			OnTriggerDeactivated();
			break;
	}
}
