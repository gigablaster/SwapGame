#pragma once

#include "CoreMinimal.h"
#include "InteractiveInterface.generated.h"

UINTERFACE(meta = (CannotImplementInterfaceInBlueprint))
class SWAP_API UInteractiveInterface : public UInterface
{
	GENERATED_BODY()
};

class IInteractiveInterface
{
	GENERATED_BODY()
public:
	virtual bool CanInteract(APawn* PlayerPawn)const = 0;
	virtual float GetInteractionProgress(APawn* PlayerPawn)const = 0;
	virtual void StartLooking(APawn* PlayerPawn) = 0;
	virtual void StopLooking(APawn* PlayerPawn) = 0;
	virtual void StartInteract(APawn* PlayerPawn) = 0;
	virtual void StopInteract(APawn* PlayerPawn) = 0;
	virtual void UpdateInteraction(APawn* PlayerPawn, float DeltaTime) = 0;
	virtual FText GetInteractionHint(APawn* PlayerPawn)const = 0;
};
