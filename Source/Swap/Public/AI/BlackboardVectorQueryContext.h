#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "BlackboardVectorQueryContext.generated.h"

UCLASS(Blueprintable)
class SWAP_API UBlackboardVectorQueryContext : public UEnvQueryContext
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName Key;

	UBlackboardVectorQueryContext();

	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
};
