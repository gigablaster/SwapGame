#pragma once

#include "CoreMinimal.h"
#include "HeadBobInterface.generated.h"

UINTERFACE(meta = (CannotImplementInterfaceInBlueprint))
class SWAP_API UHeadBobInterface : public UInterface
{
	GENERATED_BODY()
};

class IHeadBobInterface
{
	GENERATED_BODY()
public:
	virtual void GetHeadBob(float& OutVertical, float& OutHorizontal)const = 0;
};
