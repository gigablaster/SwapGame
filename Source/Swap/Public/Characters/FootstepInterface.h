#pragma once

#include "CoreMinimal.h"
#include "FootstepInterface.generated.h"

UINTERFACE(meta = (CannotImplementInterfaceInBlueprint))
class SWAP_API UFootstepInterface : public UInterface
{
	GENERATED_BODY()
};

class IFootstepInterface
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	virtual void OnFootstep() = 0;
};
