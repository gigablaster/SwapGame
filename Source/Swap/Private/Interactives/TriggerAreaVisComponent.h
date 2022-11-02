#pragma once

#include "CoreMinimal.h"
#include "TriggerAreaVisComponent.generated.h"

struct FTriggerTarget;

UCLASS(NotBlueprintable)
class SWAP_API UTriggerAreaVisComponent : public USceneComponent
{
	GENERATED_BODY()
public:
	UTriggerAreaVisComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
	void DrawArrows(UWorld* World, const TArray<FTriggerTarget>& Targets, FColor Color);
};
