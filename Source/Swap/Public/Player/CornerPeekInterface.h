#pragma once

#include "CoreMinimal.h"
#include "SwapTypes.h"
#include "CornerPeekInterface.generated.h"

class UCornerPeekComponent;

UINTERFACE(meta = (CannotImplementInterfaceInBlueprint))
class SWAP_API UCornerPeekInterface : public UInterface
{
	GENERATED_BODY()
};

class ICornerPeekInterface
{
	GENERATED_BODY()
public:
	virtual ECornerPeekSide GetCornerPeekSide()const = 0;
};
