#include "TriggerAreaVisComponent.h"
#include "Interactives/TriggerVisInterface.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

UTriggerAreaVisComponent::UTriggerAreaVisComponent()
{
	bTickInEditor = true;
	PrimaryComponentTick.bStartWithTickEnabled = PrimaryComponentTick.bCanEverTick = true;
}

void UTriggerAreaVisComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
#if WITH_EDITORONLY_DATA
	if (TickType == ELevelTick::LEVELTICK_ViewportsOnly)
	{
		if (const ITriggerVisInterface* Trigger = Cast<ITriggerVisInterface>(GetOwner()))
		{
			if (Trigger->KeepDebugDisplay() || IsOwnerSelected())
			{
				if (UWorld* World = GetWorld())
				{
					DrawArrows(World, Trigger->GetEnterTargets(), FColor::Red);
					DrawArrows(World, Trigger->GetExitTargets(), FColor::Blue);
				}
			}
		}
	}
#endif
}

void UTriggerAreaVisComponent::DrawArrows(UWorld* World, const TArray<FTriggerTarget>& Targets, FColor Color)
{
	const FVector Location = GetComponentLocation();
	for (const FTriggerTarget& Target : Targets)
	{
		if (Target.Target != nullptr)
		{
			FVector Origin, Extent;
			Target.Target->GetActorBounds(true, Origin, Extent);
			if (Origin == FVector::ZeroVector)
			{
				Origin = Target.Target->GetActorLocation();
			}
			DrawDebugDirectionalArrow(World, Location, Origin, 40.0f, Color, false, -1, 255, 2.0f);			
			
		}
	}
}
