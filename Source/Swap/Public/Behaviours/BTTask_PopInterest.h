#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_PopInterest.generated.h"

UCLASS()
class SWAP_API UBTTask_PopInterest : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_PopInterest();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
