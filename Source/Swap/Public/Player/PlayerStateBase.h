#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "InventoryInterface.h"
#include "PlayerStateBase.generated.h"

UCLASS(Abstract)
class APlayerStateBase : public APlayerState, public IInventoryInterface
{
	GENERATED_BODY()
public:
	APlayerStateBase();

	// ISwapInventoryInterface
	virtual bool HasItem(FName ID, int Amount) const override;
	virtual void AddItem(FName ID, int Amount) override;
	virtual bool TryUseItem(FName ID, int Amount) override;
private:
	TMap<FName, int> Items;
};
