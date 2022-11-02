#pragma once

#include "CoreMinimal.h"
#include "InteractiveInterface.h"
#include "TriggerableInterface.h"
#include "InteractiveActorBase.generated.h"

UCLASS(Abstract)
class SWAP_API AInteractiveActorBase : public AActor, public IInteractiveInterface, public ITriggerableInterface
{
	GENERATED_BODY()
public:
	AInteractiveActorBase();

	// AActor
	virtual void Reset() override;
	
	// ISwapInteractiveInterface
	virtual bool CanInteract(APawn* PlayerPawn) const override;
	virtual float GetInteractionProgress(APawn* PlayerPawn) const override;
	virtual FText GetInteractionHint(APawn* PlayerPawn) const override;
	virtual void StartLooking(APawn* PlayerPawn) override;
	virtual void StopLooking(APawn* PlayerPawn) override;
	virtual void StartInteract(APawn* PlayerPawn) override;
	virtual void StopInteract(APawn* PlayerPawn) override;
	virtual void UpdateInteraction(APawn* PlayerPawn, float DeltaTime) override;

	// ISwapTriggerableInterface
	virtual void HandleTrigger(APawn* PlayerPawn, AActor* Trigger, ETriggerAction Action) override;
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayerStarLooking(APawn* PlayerPawn);

	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayerStopLooking(APawn* PlayerPawn);

	UFUNCTION(BlueprintImplementableEvent)
	void OnInteractionStarted(APawn* PlayerPawn);

	UFUNCTION(BlueprintImplementableEvent)
	void OnInteractionFailed(APawn* PlayerPawn);

	UFUNCTION(BlueprintImplementableEvent)
	void OnInteractionSucceeded(APawn* PlayerPawn);

	UFUNCTION(BlueprintImplementableEvent)
	FText OnGetInteractionHint(APawn* PlayerPawn)const;

	UFUNCTION(BlueprintImplementableEvent)
	bool OnUpdateInteraction(APawn* PlayerPawn, float DeltaTime);

	UFUNCTION(BlueprintImplementableEvent)
	void OnTriggerActivated();

	UFUNCTION(BlueprintImplementableEvent)
	void OnTriggerDeactivated();
	
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	uint32 bIsActivated : 1;

	UPROPERTY(BlueprintReadOnly)
	uint32 bIsInteracting : 1;

	UPROPERTY(BlueprintReadWrite)
	float CurrentProgress;
};
