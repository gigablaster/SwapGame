#include "Interactives/TriggerableLevelSequenceActor.h"
#include "LevelSequencePlayer.h"

ATriggerableLevelSequenceActor::ATriggerableLevelSequenceActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{}

void ATriggerableLevelSequenceActor::HandleTrigger(APawn* PlayerPawn, AActor* Trigger, ETriggerAction Action)
{
	ULevelSequencePlayer* Player = GetSequencePlayer();
	check(Player);
	
	switch (Action)
	{
		case ETriggerAction::Pause:
			Player->Pause();
			break;
		case ETriggerAction::Resume:
			Player->Play();
			break;
		case ETriggerAction::Activate:
			Player->Play();
			break;
		case ETriggerAction::Deactivate:
			Player->Stop();
			break;
		case ETriggerAction::PlayReverse:
			Player->PlayReverse();
			break;
	}
}
