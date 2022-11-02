#include "Behaviours/BTTask_FindRandomLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation()
{
	LocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FindRandomLocation, LocationKey));
	MoveTargetKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FindRandomLocation, MoveTargetKey));
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	UNavigationSystemV1* NavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (NavigationSystem != nullptr)
	{
		FNavLocation Result;
		const FVector Location = BlackboardComponent->GetValueAsVector(LocationKey.SelectedKeyName);
		if (NavigationSystem->GetRandomReachablePointInRadius(Location, Radius, Result))
		{
			BlackboardComponent->SetValueAsVector(MoveTargetKey.SelectedKeyName, Result.Location);

			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
