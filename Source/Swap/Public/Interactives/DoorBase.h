#pragma once

#include "CoreMinimal.h"
#include "DoorBase.generated.h"

UCLASS(Abstract)
class SWAP_API ADoorBase : public AActor
{
	GENERATED_BODY()
public:
	ADoorBase();

	virtual void Reset() override;
protected:
	virtual void BeginPlay() override;
	virtual void ResetDoor();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnOpenDoor(bool bInstant = false);

	UFUNCTION(BlueprintImplementableEvent)
	void OnCloseDoor(bool bInstant = false);

	UFUNCTION(BlueprintImplementableEvent)
	void OnOpenFailed(APawn* Pawn);

	UPROPERTY(BlueprintReadOnly)
	uint32 bIsOpened : 1;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	uint32 bIsInitiallyOpened : 1;
};
