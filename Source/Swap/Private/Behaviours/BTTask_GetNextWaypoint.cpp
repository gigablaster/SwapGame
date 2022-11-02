#include "Behaviours/BTTask_GetNextWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "AI/GuardInterface.h"
#include "AI/PatrolRouteComponent.h"
#include "Swap.h"

UBTTask_GetNextWaypoint::UBTTask_GetNextWaypoint()
{
	WaypointIndexKey.AddIntFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_GetNextWaypoint, WaypointIndexKey));
	WaypointLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_GetNextWaypoint, WaypointLocationKey));
}

EBTNodeResult::Type UBTTask_GetNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	APawn* Pawn = Controller != nullptr ? Controller->GetPawn() : nullptr;
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();

	if (const IGuardInterface* GuardInterface = Cast<IGuardInterface>(Pawn))
	{
		if (const UPatrolRouteComponent* PatrolRouteComponent = GuardInterface->GetPatrolRoute())
		{
			const int LastIndex = BlackboardComponent->GetValueAsInt(WaypointIndexKey.SelectedKeyName);
			const int Index = PatrolRouteComponent->GetNextWaypointIndex(LastIndex);			
			if (Index >= 0)
			{
				UE_LOG(LogSwap, Verbose, TEXT("%s: move from waypoint %d to %d"), *Pawn->GetName(), LastIndex, Index);
				BlackboardComponent->SetValueAsInt(WaypointIndexKey.SelectedKeyName, Index);
				BlackboardComponent->SetValueAsVector(WaypointLocationKey.SelectedKeyName, PatrolRouteComponent->GetWaypointWorldSpace(Index));

				return EBTNodeResult::Succeeded;
			}
		}
	}

	return EBTNodeResult::Failed;
}
