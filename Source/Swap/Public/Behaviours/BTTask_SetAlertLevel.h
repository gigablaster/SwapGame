#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "SwapTypes.h"
#include "BTTask_SetAlertLevel.generated.h"

UCLASS()
class SWAP_API UBTTask_SetAlertLevel : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_SetAlertLevel();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
protected:
	UPROPERTY(EditAnywhere, Category=Blackboard)
	EGuardAlertLevel AlertLevel;
};
