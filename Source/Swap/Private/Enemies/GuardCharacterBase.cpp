#include "Enemies/GuardCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "AI/PatrolRouteActor.h"

AGuardCharacterBase::AGuardCharacterBase()
{
	NormalStateTag = FGameplayTag::RequestGameplayTag("Character.Guard.State.Normal");
	AlertStateTag = FGameplayTag::RequestGameplayTag("Character.Guard.State.Alert");
	InvestigateStateTag = FGameplayTag::RequestGameplayTag("Character.Guard.State.Investigate");
	AttackStateTag = FGameplayTag::RequestGameplayTag("Character.Guard.State.Attack");

	StateTags.AddTag(NormalStateTag);
	StateTags.AddTag(AlertStateTag);
	StateTags.AddTag(InvestigateStateTag);
	StateTags.AddTag(AttackStateTag);
}

void AGuardCharacterBase::AlertLevelChanged(EGuardAlertLevel AlertLevel)
{
	CurrentAlertLevel = AlertLevel;
	AbilitySystemComponent->RemoveLooseGameplayTags(StateTags);
	if (AlertLevel != EGuardAlertLevel::None)
	{
		const FGameplayTag Tag = GetTagForAlertLevel(AlertLevel);
		AbilitySystemComponent->AddLooseGameplayTag(Tag, 1);	
	}	
	OnAlertLevelChanged(AlertLevel);
}

UPatrolRouteComponent* AGuardCharacterBase::GetPatrolRoute() const
{
	return PatrolRoute != nullptr ? PatrolRoute->GetPatrolRouteComponent() : nullptr;
}

FGameplayTag AGuardCharacterBase::GetTagForAlertLevel(EGuardAlertLevel AlertLevel) const
{
	switch(AlertLevel)
	{
		case EGuardAlertLevel::Alert:
			return AlertStateTag;
		case EGuardAlertLevel::Attack:
			return AttackStateTag;
		case EGuardAlertLevel::Investigate:
			return InvestigateStateTag;
	}
	return NormalStateTag;
}
