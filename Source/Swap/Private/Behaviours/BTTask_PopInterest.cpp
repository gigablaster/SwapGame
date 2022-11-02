#include "Behaviours/BTTask_PopInterest.h"
#include "AI/GuardControllerInterface.h"
#include "AIController.h"

UBTTask_PopInterest::UBTTask_PopInterest()
{
}

EBTNodeResult::Type UBTTask_PopInterest::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAIController* Controller = OwnerComp.GetAIOwner())
	{
		if(IGuardControllerInterface* GuardControllerInterface = Cast<IGuardControllerInterface>(Controller))
		{
			GuardControllerInterface->PopInterest();

			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
