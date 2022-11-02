#pragma once

#include "CoreMinimal.h"
#include "InventoryInterface.generated.h"

UINTERFACE(meta = (CannotImplementInterfaceInBlueprint))
class SWAP_API UInventoryInterface : public UInterface
{
	GENERATED_BODY()
};

class IInventoryInterface
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	virtual bool HasItem(FName ID, int Amount = 1)const = 0;

	UFUNCTION(BlueprintCallable)
	virtual void AddItem(FName ID, int Amount = 1) = 0;

	UFUNCTION(BlueprintCallable)
	virtual bool TryUseItem(FName ID, int Amount = 1) = 0;	
};