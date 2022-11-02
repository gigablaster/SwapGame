#include "PatrolRouteComponentVisualizer.h"
#include "AI/PatrolRouteComponent.h"

static const float PointSize = 20.0f;
static const float LineSize = 5.0f;

IMPLEMENT_HIT_PROXY(HPatrolRouteVisProxy, HComponentVisProxy);
IMPLEMENT_HIT_PROXY(HPatrolRouteWaypointProxy, HPatrolRouteVisProxy);

#define LOCTEXT_NAMESPACE "PatrolRouteComponentVisualizer"

class FPatrolRouteVisualizerCommands : public TCommands<FPatrolRouteVisualizerCommands>
{
public:
	FPatrolRouteVisualizerCommands()
		: TCommands<FPatrolRouteVisualizerCommands>("PatrolRouteComponentVisualizer", LOCTEXT("PatrolRouteComponentVisualizerDsc", "Patrol Route Component Visualizer"), NAME_None, FEditorStyle::GetStyleSetName())
	{}

	void RegisterCommands() override
	{
		UI_COMMAND(DuplicateKey, "Duplicate Waypoint", "Duplicates currently selected waypoint", EUserInterfaceActionType::Button, FInputGesture());
		UI_COMMAND(DeleteKey, "Delete Waypoint", "Deletes currently selected waypoint", EUserInterfaceActionType::Button, FInputGesture());
	}

	TSharedPtr<FUICommandInfo> DuplicateKey;
	TSharedPtr<FUICommandInfo> DeleteKey;
};

FPatrolRouteComponentVisualizer::FPatrolRouteComponentVisualizer()
{
	FPatrolRouteVisualizerCommands::Register();
	SelectedWaypoint = INDEX_NONE;
}

FPatrolRouteComponentVisualizer::~FPatrolRouteComponentVisualizer()
{
}

void FPatrolRouteComponentVisualizer::OnRegister()
{
	const FPatrolRouteVisualizerCommands& Commands = FPatrolRouteVisualizerCommands::Get();
	
	PatrolRouteVisualizerActions = MakeShareable(new FUICommandList);
	PatrolRouteVisualizerActions->MapAction(
		Commands.DuplicateKey,
		FExecuteAction::CreateSP(this, &FPatrolRouteComponentVisualizer::DuplicateWaypoint),
		FCanExecuteAction::CreateSP(this, &FPatrolRouteComponentVisualizer::CanDuplicateWaypoint)
	);
	PatrolRouteVisualizerActions->MapAction(
		Commands.DeleteKey,
		FExecuteAction::CreateSP(this, &FPatrolRouteComponentVisualizer::DeleteWaypoint),
		FCanExecuteAction::CreateSP(this, &FPatrolRouteComponentVisualizer::CanDeleteWaypoint)
	);
}

TSharedPtr<SWidget> FPatrolRouteComponentVisualizer::GenerateContextMenu() const
{
	FMenuBuilder MenuBuilder(true, PatrolRouteVisualizerActions);
	MenuBuilder.BeginSection("Patrol Route Actiosn", LOCTEXT("WaypointHeading", "Waypoint"));
	{
		if (SelectedWaypoint != INDEX_NONE)
		{
			MenuBuilder.AddMenuEntry(FPatrolRouteVisualizerCommands::Get().DuplicateKey);
			MenuBuilder.AddMenuEntry(FPatrolRouteVisualizerCommands::Get().DeleteKey);
		}
	}
	MenuBuilder.EndSection();

	TSharedPtr<SWidget> MenuWidget = MenuBuilder.MakeWidget();

	return MenuWidget;
}

void FPatrolRouteComponentVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	if (const UPatrolRouteComponent* PatrolRouteComponent = Cast<const UPatrolRouteComponent>(Component))
	{
		const FLinearColor RouteColor = PatrolRouteComponent->GetRouteColor();
		const FLinearColor SelectedRouteColor = PatrolRouteComponent->GetSelectedRouteColor();
		const FLinearColor WaypointColor = PatrolRouteComponent->GetWaypointColor();
		const FLinearColor SelectedWaypointColor = PatrolRouteComponent->GetSelectedWaypointColor();

		const int Count = PatrolRouteComponent->GetWaypointsCount();
		if (Count == 1)
		{
			const FVector Location = PatrolRouteComponent->GetWaypointWorldSpace(0);
			const FLinearColor PointColor = SelectedWaypoint == 0 ? SelectedWaypointColor : WaypointColor;
			PDI->SetHitProxy(new HPatrolRouteWaypointProxy(Component, 0));
			PDI->DrawPoint(Location, PointColor, PointSize, SDPG_Foreground);
			PDI->SetHitProxy(nullptr);
		}
		else
		{
			for (int Index = 0; Index < Count; ++Index)
			{
				const int Next = (Index + 1) % Count;
				const FVector Start = PatrolRouteComponent->GetWaypointWorldSpace(Index);
				const FVector End = PatrolRouteComponent->GetWaypointWorldSpace(Next);

				const bool bIsSelected = SelectedWaypoint == Index;
				const bool bConnectsToSelected = bIsSelected ? true : Next == SelectedWaypoint;

				const FLinearColor PointColor = bIsSelected ? SelectedWaypointColor : WaypointColor;
				const FLinearColor LineColor = bConnectsToSelected ? SelectedRouteColor : RouteColor;

				PDI->SetHitProxy(new HPatrolRouteWaypointProxy(Component, Index));
				PDI->DrawPoint(Start, PointColor, PointSize, SDPG_Foreground);
				PDI->SetHitProxy(nullptr);

				PDI->DrawLine(Start, End, LineColor, SDPG_Foreground, LineSize);
			}
		}
	}
}

bool FPatrolRouteComponentVisualizer::VisProxyHandleClick(FEditorViewportClient* InViewportClient, HComponentVisProxy* VisProxy, const FViewportClick& Click)
{
	bool bEditing = false;

	if (VisProxy && VisProxy->Component.IsValid())
	{
		const UPatrolRouteComponent* PatrolRouteComponent = Cast<const UPatrolRouteComponent>(VisProxy->Component.Get());
		RoutePropertyPath = FComponentPropertyPath(PatrolRouteComponent);
		if (RoutePropertyPath.IsValid())
		{
			bEditing = true;

			AActor* OldOwner = RouteOwningActor.Get();
			RouteOwningActor = PatrolRouteComponent->GetOwner();

			if (OldOwner != RouteOwningActor)
			{
				SelectedWaypoint = INDEX_NONE;
			}

			if (VisProxy->IsA(HPatrolRouteWaypointProxy::StaticGetType()))
			{
				HPatrolRouteWaypointProxy* Proxy = static_cast<HPatrolRouteWaypointProxy*>(VisProxy);
				SelectedWaypoint = Proxy->GetWaypointIndex();
			}
		}
		else
		{
			RouteOwningActor.Reset();
		}
	}

	return bEditing;
}

void FPatrolRouteComponentVisualizer::EndEditing()
{
	RouteOwningActor.Reset();
	SelectedWaypoint = INDEX_NONE;
}

UPatrolRouteComponent* FPatrolRouteComponentVisualizer::GetEditedPatrolRouteComponent() const
{
	return Cast<UPatrolRouteComponent>(RoutePropertyPath.GetComponent());
}

void FPatrolRouteComponentVisualizer::DeleteWaypoint()
{
	UPatrolRouteComponent* PatrolRouteComponent = GetEditedPatrolRouteComponent();

	check(PatrolRouteComponent != nullptr);
	check(SelectedWaypoint != INDEX_NONE);

	const int Count = PatrolRouteComponent->GetWaypointsCount();
	const int Index = SelectedWaypoint;
	if (SelectedWaypoint >= Count - 1)
	{
		--SelectedWaypoint;
	}

	PatrolRouteComponent->RemoveWaypoint(Index);
}

void FPatrolRouteComponentVisualizer::DuplicateWaypoint()
{
	UPatrolRouteComponent* PatrolRouteComponent = GetEditedPatrolRouteComponent();

	check(PatrolRouteComponent != nullptr);
	check(SelectedWaypoint != INDEX_NONE);
	
	FVector NewWaypoint = PatrolRouteComponent->GetWaypointWorldSpace(SelectedWaypoint);
	NewWaypoint += FVector(10.0f, 0, 0);
	SelectedWaypoint = PatrolRouteComponent->InsertWaypointWorldSpace(NewWaypoint, SelectedWaypoint);
}

bool FPatrolRouteComponentVisualizer::CanDuplicateWaypoint() const
{
	UPatrolRouteComponent* PatrolRouteComponent = GetEditedPatrolRouteComponent();
	return PatrolRouteComponent != nullptr && SelectedWaypoint >= 0 && SelectedWaypoint < PatrolRouteComponent->GetWaypointsCount();
}

bool FPatrolRouteComponentVisualizer::CanDeleteWaypoint() const
{
	UPatrolRouteComponent* PatrolRouteComponent = GetEditedPatrolRouteComponent();
	return PatrolRouteComponent != nullptr && PatrolRouteComponent->GetWaypointsCount() > 1 && SelectedWaypoint >= 0 && SelectedWaypoint < PatrolRouteComponent->GetWaypointsCount();
}

bool FPatrolRouteComponentVisualizer::HandleInputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event)
{
	if (Key == EKeys::Delete && Event == EInputEvent::IE_Pressed)
	{
		UPatrolRouteComponent* PatrolRouteComponent = GetEditedPatrolRouteComponent();
		if (PatrolRouteComponent && PatrolRouteComponent->GetWaypointsCount() > 1)
		{
			int Index = SelectedWaypoint;
			int Count = PatrolRouteComponent->GetWaypointsCount();

			if (Index >= Count - 1)
			{
				--SelectedWaypoint;
			}

			PatrolRouteComponent->RemoveWaypoint(Index);
			return true;
		}
	}

	return false;
}

bool FPatrolRouteComponentVisualizer::HandleInputDelta(FEditorViewportClient* ViewportClient, FViewport* Viewport, FVector& DeltaTranslate, FRotator& DeltalRotate, FVector& DeltaScale)
{
	UPatrolRouteComponent* PatrolRouteComponent = GetEditedPatrolRouteComponent();
	if (PatrolRouteComponent && SelectedWaypoint != INDEX_NONE)
	{
		PatrolRouteComponent->OffsetWaypointLocation(DeltaTranslate, SelectedWaypoint);
		return true;
	}

	return false;
}

bool FPatrolRouteComponentVisualizer::GetWidgetLocation(const FEditorViewportClient* ViewportClient, FVector& OutLocation) const
{
	UPatrolRouteComponent* PatrolRouteComponent = GetEditedPatrolRouteComponent();
	if (PatrolRouteComponent && SelectedWaypoint != INDEX_NONE)
	{
		OutLocation = PatrolRouteComponent->GetWaypointWorldSpace(SelectedWaypoint);
		return true;
	}

	return false;
}

#undef LOCTEXT_NAMESPACE