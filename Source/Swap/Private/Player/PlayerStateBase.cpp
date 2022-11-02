#include "Player/PlayerStateBase.h"

APlayerStateBase::APlayerStateBase()
{}

bool APlayerStateBase::HasItem(FName ID, int Amount) const
{
	if (Items.Contains(ID))
	{
		const int CurrentAmount = Items.FindChecked(ID);
		return CurrentAmount >= Amount;
	}

	return false;	
}

void APlayerStateBase::AddItem(FName ID, int Amount)
{
	check(Amount > 0);
	int CurrentAmount = 0;
	if (Items.Contains(ID))
	{
		CurrentAmount = Items.FindChecked(ID);
	}
	Items.Add(ID, CurrentAmount + Amount);
}

bool APlayerStateBase::TryUseItem(FName ID, int Amount)
{
	if (HasItem(ID, Amount))
	{
		int CurrentAmount = Items.FindChecked(ID);
		CurrentAmount -= Amount;
		if (CurrentAmount <= 0)
		{
			Items.Remove(ID);
		}
		else
		{
			Items.Add(ID, CurrentAmount);	
		}

		return true;
	}

	return false;
}
