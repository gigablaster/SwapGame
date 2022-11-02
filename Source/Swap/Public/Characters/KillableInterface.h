// Copyright (c) 2020-2021 Vladimir Kuskov
#pragma once

#include "CoreMinimal.h"
#include "KillableInterface.generated.h"

UINTERFACE(meta = (CannotImplementInterfaceInBlueprint))
class SWAP_API UKillableInterface : public UInterface
{
	GENERATED_BODY()
};

class IKillableInterface
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
    virtual void Kill(APawn* Killer) = 0; 
};
