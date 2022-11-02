#include "Interactives/PickupBase.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerState.h"
#include "Player/InventoryInterface.h"

APickupBase::APickupBase()
{
	bIsCollected = false;
}

void APickupBase::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (!bIsCollected)
	{
		APawn* Pawn = Cast<APawn>(OtherActor);
		if (Pawn != nullptr)
		{
			IInventoryInterface* InventoryInterface = Cast<IInventoryInterface>(Pawn->GetPlayerState());
			if (InventoryInterface != nullptr)
			{
				InventoryInterface->AddItem(GetItemID(), GetItemCount());
				bIsCollected = true;
				OnCollected(Pawn);
			}
		}
	}
}
