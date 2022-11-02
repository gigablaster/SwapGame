#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindRandomLocation.generated.h"

UCLASS()
class SWAP_API UBTTask_FindRandomLocation : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_FindRandomLocation();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
protected:
	UPROPERTY(EditAnywhere, Category=Blackboard)
	float Radius;

	UPROPERTY(EditAnywhere, Category=Blackboard)
	FBlackboardKeySelector LocationKey;
	
	UPROPERTY(EditAnywhere, Category=Blackboard)
	FBlackboardKeySelector MoveTargetKey;
};
