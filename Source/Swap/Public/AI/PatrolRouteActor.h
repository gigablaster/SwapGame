#pragma once

#include "CoreMinimal.h"
#include "PatrolRouteActor.generated.h"

class UPatrolRouteComponent;
class UBillboardComponent;

UCLASS()
class SWAP_API APatrolRouteActor : public AActor
{
	GENERATED_BODY()
public:
	APatrolRouteActor();

	FORCEINLINE UPatrolRouteComponent* GetPatrolRouteComponent() const { return PatrolRouteComponent; };
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Components)
	UPatrolRouteComponent* PatrolRouteComponent;

#if WITH_EDITORONLY_DATA
	UPROPERTY()
	UBillboardComponent* BillboardComponent;
#endif	
};
