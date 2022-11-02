#pragma once

#include "CoreMinimal.h"
#include "PlayerHUDInterface.generated.h"

UINTERFACE(meta = (CannotImplementInterfaceInBlueprint))
class SWAP_API UPlayerHUDInterface : public UInterface
{
	GENERATED_BODY()
};

class IPlayerHUDInterface
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	virtual void ShowHint(const FText &Text) = 0;

	UFUNCTION(BlueprintCallable)
	virtual void ShowMessage(const FText &Text) = 0;

	virtual void ShowInteraction(float Progress) = 0;
	virtual void SetLookForTeleportation(bool bLook) = 0;
	virtual void SetHasGoodTeleportation(bool bFound) = 0;

	virtual void SetCanPeek(bool bCanPeek) = 0;
};
