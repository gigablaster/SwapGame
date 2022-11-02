#include "Interactives/TriggerArea.h"
#include "TriggerAreaVisComponent.h"

ATriggerArea::ATriggerArea()
{
#if WITH_EDITORONLY_DATA
	if (TriggerVisComponent == nullptr)
	{
		TriggerVisComponent = CreateDefaultSubobject<UTriggerAreaVisComponent>(TEXT("DebugVis"), true);
		TriggerVisComponent->SetupAttachment(GetRootComponent());
		TriggerVisComponent->SetIsVisualizationComponent(true);
	}
#endif
}

bool ATriggerArea::ShouldTickIfViewportsOnly() const
{
	return true;
}

void ATriggerArea::HandleEnter(APawn* Pawn)
{
	for (const FTriggerTarget& Target : EnterTargets)
	{
		ITriggerableInterface* TriggerableInterface = Cast<ITriggerableInterface>(Target.Target);
		if (TriggerableInterface != nullptr)
		{
			TriggerableInterface->HandleTrigger(Pawn, this, Target.Action);
		}
	}
	OnTriggerEnter(Pawn);
}

void ATriggerArea::HandleExit(APawn* Pawn)
{
	for (const FTriggerTarget& Target : ExitTargets)
	{
		ITriggerableInterface* TriggerableInterface = Cast<ITriggerableInterface>(Target.Target);
		if (TriggerableInterface != nullptr)
		{
			TriggerableInterface->HandleTrigger(Pawn, this, Target.Action);
		}
	}

	OnTriggerExit(Pawn);
}

const TArray<FTriggerTarget>& ATriggerArea::GetEnterTargets() const
{
	return EnterTargets;
}

const TArray<FTriggerTarget>& ATriggerArea::GetExitTargets() const
{
	return ExitTargets;
}

bool ATriggerArea::KeepDebugDisplay() const
{
#if WITH_EDITORONLY_DATA
	return bKeepDebugDisplay;
#else
	return false;
#endif
}
