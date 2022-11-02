#pragma once

#include "CoreMinimal.h"
#include "Characters//CharacterBase.h"
#include "AI/GuardInterface.h"
#include "GuardCharacterBase.generated.h"

class APatrolRouteActor;

UCLASS(Abstract)
class SWAP_API AGuardCharacterBase : public ACharacterBase, public IGuardInterface
{
	GENERATED_BODY()
public:
	AGuardCharacterBase();

	// IGuardInterface
	virtual UPatrolRouteComponent* GetPatrolRoute() const override;
	virtual void AlertLevelChanged(EGuardAlertLevel AlertLevel) override;

	FORCEINLINE EGuardAlertLevel GetAlertLevel()const { return CurrentAlertLevel; }
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnAlertLevelChanged(EGuardAlertLevel AlertLevel);

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category=Route)
	APatrolRouteActor* PatrolRoute;

	FGameplayTag NormalStateTag;
	FGameplayTag AlertStateTag;
	FGameplayTag InvestigateStateTag;
	FGameplayTag AttackStateTag;
	
	FGameplayTagContainer StateTags;

	EGuardAlertLevel CurrentAlertLevel;

	FGameplayTag GetTagForAlertLevel(EGuardAlertLevel AlertLevel)const;
};
