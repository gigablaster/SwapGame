#include "AI/PatrolRouteActor.h"
#include "AI/PatrolRouteComponent.h"
#include "Components/BillboardComponent.h"

APatrolRouteActor::APatrolRouteActor()
{
	PrimaryActorTick.bCanEverTick = PrimaryActorTick.bStartWithTickEnabled = false;
	PatrolRouteComponent = CreateDefaultSubobject<UPatrolRouteComponent>("Route");
	SetRootComponent(PatrolRouteComponent);
	
#if WITH_EDITORONLY_DATA
	BillboardComponent = CreateEditorOnlyDefaultSubobject<UBillboardComponent>("Billboard");
	BillboardComponent->SetupAttachment(GetRootComponent());
	BillboardComponent->bIsEditorOnly = true;

	static ConstructorHelpers::FObjectFinder<UTexture2D> WaypointTexture(TEXT("/Engine/EditorResources/Waypoint.Waypoint"));
	if (WaypointTexture.Succeeded())
	{
		BillboardComponent->SetSprite(WaypointTexture.Object);
	}

#endif
}
