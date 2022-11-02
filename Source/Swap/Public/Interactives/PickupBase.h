#pragma once

#include "CoreMinimal.h"
#include "PickupBase.generated.h"

UCLASS(Abstract)
class SWAP_API APickupBase : public AActor
{
	GENERATED_BODY()
public:
	APickupBase();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnCollected(APawn* Pawn);
	
	UFUNCTION(BlueprintImplementableEvent)
	FName GetItemID()const;
	
	UFUNCTION(BlueprintImplementableEvent)
	int GetItemCount()const;	

	UPROPERTY(BlueprintReadOnly)
	uint32 bIsCollected : 1;
};
