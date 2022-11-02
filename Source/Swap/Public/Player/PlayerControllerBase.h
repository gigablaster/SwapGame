#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerInterface.h"
#include "InventoryInterface.h"
#include "PlayerHUDInterface.h"
#include "PlayerControllerBase.generated.h"

UCLASS(Abstract)
class SWAP_API APlayerControllerBase : public APlayerController, public IPlayerControllerInterface, public IPlayerHUDInterface
{
	GENERATED_BODY()
public:
	APlayerControllerBase();

	// ISwapPlayerControllerInterface
	virtual void HandleKilled(APawn* PlayerPawn) override;

	// ISwapPlayerHUDInterface
	virtual void ShowHint(const FText& Text) override;
	virtual void ShowMessage(const FText& Text) override;
	virtual void ShowInteraction(float Progress) override;
	virtual void SetHasGoodTeleportation(bool bFound) override;
	virtual void SetLookForTeleportation(bool bLook) override;
	virtual void SetCanPeek(bool bCanPeek) override;
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayerKilled(APawn* PlayerPawn);

	UFUNCTION(BlueprintImplementableEvent)
	void OnShowHint(const FText& Text);

	UFUNCTION(BlueprintImplementableEvent)
	void OnShowMessage(const FText& Text);

	UFUNCTION(BlueprintImplementableEvent)
	void OnShowInteraction(float Progress);

	UFUNCTION(BlueprintImplementableEvent)
	void OnLookForTeleportation(bool bLook);

	UFUNCTION(BlueprintImplementableEvent)
	void OnHasGoodTeleportation(bool bFound);

	UFUNCTION(BlueprintImplementableEvent)
	void OnCanPeek(bool bCanPeek);
};
