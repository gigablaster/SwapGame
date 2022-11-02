#include "Behaviours/BTTask_GetClosestWaypoint.h"
#include "AI/PatrolRouteComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AIController.h"
#include "AI/GuardInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Swap.h"

UBTTask_GetClosestWaypoint::UBTTask_GetClosestWaypoint()
{
	WaypointIndexKey.AddIntFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_GetClosestWaypoint, WaypointIndexKey));
	WaypointLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_GetClosestWaypoint, WaypointLocationKey));
}

EBTNodeResult::Type UBTTask_GetClosestWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	APawn* Pawn = Controller != nullptr ? Controller->GetPawn() : nullptr;
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();

	if (const IGuardInterface* GuardInterface = Cast<IGuardInterface>(Pawn))
	{
		if (const UPatrolRouteComponent* PatrolRouteComponent = GuardInterface->GetPatrolRoute())
		{
			const int Index = PatrolRouteComponent->GetClosestWaypoint(Pawn->GetActorLocation());
			if (Index >= 0)
			{
				UE_LOG(LogSwap, Verbose, TEXT("%s: closest waypoint: %d"), *Pawn->GetName(), Index);
				BlackboardComponent->SetValueAsInt(WaypointIndexKey.SelectedKeyName, Index);
				BlackboardComponent->SetValueAsVector(WaypointLocationKey.SelectedKeyName, PatrolRouteComponent->GetWaypointWorldSpace(Index));

				return EBTNodeResult::Succeeded;
			}
		}
	}
	
	return EBTNodeResult::Failed;
}
