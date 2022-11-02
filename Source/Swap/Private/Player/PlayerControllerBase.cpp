#include "Player/PlayerControllerBase.h"

APlayerControllerBase::APlayerControllerBase()
{}

void APlayerControllerBase::HandleKilled(APawn* PlayerPawn)
{
	if (PlayerPawn != nullptr)
	{
		OnPlayerKilled(PlayerPawn);
	}
}

void APlayerControllerBase::ShowInteraction(float Progress)
{
	OnShowInteraction(Progress);
}

void APlayerControllerBase::ShowHint(const FText& Text)
{
	OnShowHint(Text);
}

void APlayerControllerBase::ShowMessage(const FText& Text)
{
	OnShowMessage(Text);
}

void APlayerControllerBase::SetHasGoodTeleportation(bool bFound)
{
	OnHasGoodTeleportation(bFound);
}

void APlayerControllerBase::SetLookForTeleportation(bool bLook)
{
	OnLookForTeleportation(bLook);
}

void APlayerControllerBase::SetCanPeek(bool bCanPeek)
{
	OnCanPeek(bCanPeek);
}
