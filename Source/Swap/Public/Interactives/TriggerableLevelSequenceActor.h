#pragma once

#include "CoreMinimal.h"
#include "LevelSequenceActor.h"
#include "TriggerableInterface.h"
#include "TriggerableLevelSequenceActor.generated.h"

UCLASS()
class SWAP_API ATriggerableLevelSequenceActor : public ALevelSequenceActor, public ITriggerableInterface
{
	GENERATED_BODY()
public:
	ATriggerableLevelSequenceActor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// ISwapTriggerableInterface
	virtual void HandleTrigger(APawn* PlayerPawn, AActor* Trigger, ETriggerAction Action) override;
};
