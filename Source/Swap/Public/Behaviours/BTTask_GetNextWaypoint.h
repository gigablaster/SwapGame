#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GetNextWaypoint.generated.h"

UCLASS()
class SWAP_API UBTTask_GetNextWaypoint : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_GetNextWaypoint();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
protected:
	UPROPERTY(EditAnywhere, Category=Blackboard)
	FBlackboardKeySelector WaypointIndexKey;

	UPROPERTY(EditAnywhere, Category=Blackboard)
	FBlackboardKeySelector WaypointLocationKey;
};
