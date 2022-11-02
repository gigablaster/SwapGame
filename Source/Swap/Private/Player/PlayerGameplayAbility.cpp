#include "Player/PlayerGameplayAbility.h"

UPlayerGameplayAbility::UPlayerGameplayAbility()
{
}

int UPlayerGameplayAbility::GetActionBind() const
{
	return static_cast<int>(PlayerAction);
}
