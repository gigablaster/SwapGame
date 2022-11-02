// Copyright (c) 2020-2021 Vladimir Kuskov
#pragma once

#include "CoreMinimal.h"
#include "InputActionBindInterface.generated.h"

UINTERFACE(meta = (CannotImplementInterfaceInBlueprint))
class SWAP_API UInputActionBindInterface : public UInterface
{
	GENERATED_BODY()
};

class SWAP_API IInputActionBindInterface
{
	GENERATED_BODY()
public:
	virtual int GetActionBind()const = 0;
};