#pragma once

#include "CoreMinimal.h"
#include "PlayerControllerInterface.generated.h"

UINTERFACE(meta = (CannotImplementInterfaceInBlueprint))
class SWAP_API UPlayerControllerInterface : public UInterface
{
	GENERATED_BODY()
};

class IPlayerControllerInterface
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	virtual void HandleKilled(APawn* PlayerPawn) = 0;
};
