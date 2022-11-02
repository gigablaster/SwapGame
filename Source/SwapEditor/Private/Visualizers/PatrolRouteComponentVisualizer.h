#pragma once

#include "ComponentVisualizer.h"
#include "Framework/Commands/UICommandList.h"

class UPatrolRouteComponent;

struct HPatrolRouteVisProxy : public HComponentVisProxy
{
	DECLARE_HIT_PROXY()
public:
	HPatrolRouteVisProxy(const UActorComponent* InComponent)
		: HComponentVisProxy(InComponent)
	{}
};

struct HPatrolRouteWaypointProxy : public HPatrolRouteVisProxy
{
	DECLARE_HIT_PROXY()
public:
	HPatrolRouteWaypointProxy(const UActorComponent* InComponent, int InWaypointIndex)
		: HPatrolRouteVisProxy(InComponent)
		, WaypointIndex(InWaypointIndex)
	{}
	
	FORCEINLINE int GetWaypointIndex()const { return WaypointIndex; }
private:
	int WaypointIndex;
};
class SWAPEDITOR_API FPatrolRouteComponentVisualizer : public FComponentVisualizer
{
public:
	FPatrolRouteComponentVisualizer();
	virtual ~FPatrolRouteComponentVisualizer();

	// FComponentVisualizer
	virtual void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override;
	virtual bool VisProxyHandleClick(FEditorViewportClient* InViewportClient, HComponentVisProxy* VisProxy, const FViewportClick& Click) override;
	virtual void EndEditing() override;
	virtual bool HandleInputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event) override;
	virtual bool HandleInputDelta(FEditorViewportClient* ViewportClient, FViewport* Viewport, FVector& DeltaTranslate, FRotator& DeltalRotate, FVector& DeltaScale) override;
	virtual bool GetWidgetLocation(const FEditorViewportClient* ViewportClient, FVector& OutLocation) const override;
	virtual TSharedPtr<SWidget> GenerateContextMenu() const override;
	virtual void OnRegister() override;
private:
	UPatrolRouteComponent* GetEditedPatrolRouteComponent()const;
	void DuplicateWaypoint();
	bool CanDuplicateWaypoint()const;
	void DeleteWaypoint();
	bool CanDeleteWaypoint()const;
private:
	int SelectedWaypoint;

	TWeakObjectPtr<AActor> RouteOwningActor;
	FComponentPropertyPath RoutePropertyPath;
	TSharedPtr<FUICommandList> PatrolRouteVisualizerActions;
};
