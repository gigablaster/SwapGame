#include "AI/BlackboardVectorQueryContext.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"

UBlackboardVectorQueryContext::UBlackboardVectorQueryContext()
{}

void UBlackboardVectorQueryContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	const APawn* Pawn = Cast<APawn>(QueryInstance.Owner.Get());
	if (Pawn != nullptr)
	{
		AAIController* Controller = Cast<AAIController>(Pawn->GetController());
		if (Controller != nullptr && Controller->GetBlackboardComponent() != nullptr)
		{
			UBlackboardComponent* BlackboardComponent = Controller->GetBlackboardComponent();
			FBlackboard::FKey RealKey = BlackboardComponent->GetKeyID(Key);
			if (BlackboardComponent->IsValidKey(RealKey))
			{
				UEnvQueryItemType_Point::SetContextHelper(ContextData, BlackboardComponent->GetValueAsVector(Key));
			}
		}
	}
}
