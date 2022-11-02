#pragma once

#include "CoreMinimal.h"
#include "TriggerAreaBase.h"
#include "TriggerableInterface.h"
#include "TriggerVisInterface.h"
#include "TriggerArea.generated.h"

class UTriggerAreaVisComponent;
class USwapGameplayEffect;

UCLASS()
class SWAP_API ATriggerArea : public ATriggerAreaBase, public ITriggerVisInterface
{
	GENERATED_BODY()
public:
	ATriggerArea();
	
	virtual bool ShouldTickIfViewportsOnly() const override;

	// ISwapTriggerVisInterface
	virtual const TArray<FTriggerTarget>& GetEnterTargets()const override;
	virtual const TArray<FTriggerTarget>& GetExitTargets()const override;
	virtual bool KeepDebugDisplay()const override;
protected:
	virtual void HandleEnter(APawn* Pawn) override;
	virtual void HandleExit(APawn* Pawn) override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnTriggerEnter(APawn* Pawn);

	UFUNCTION(BlueprintImplementableEvent)
	void OnTriggerExit(APawn* Pawn);

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category=Targets)
	TArray<FTriggerTarget> EnterTargets;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category=Targets)
	TArray<FTriggerTarget> ExitTargets;

#if WITH_EDITORONLY_DATA
	UPROPERTY(Transient, EditAnywhere, AdvancedDisplay)
	uint32 bKeepDebugDisplay : 1;

	UPROPERTY(Transient)
	UTriggerAreaVisComponent* TriggerVisComponent;
#endif	
};
