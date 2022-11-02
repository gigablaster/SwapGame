#include "Behaviours/BTTask_SetAlertLevel.h"
#include "AI/GuardControllerInterface.h"
#include "AIController.h"

UBTTask_SetAlertLevel::UBTTask_SetAlertLevel()
{
	AlertLevel = EGuardAlertLevel::Normal;
}

EBTNodeResult::Type UBTTask_SetAlertLevel::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAIController* Controller = OwnerComp.GetAIOwner())
	{
		if(IGuardControllerInterface* GuardControllerInterface = Cast<IGuardControllerInterface>(Controller))
		{
			GuardControllerInterface->SetAlertLevel(AlertLevel);

			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
