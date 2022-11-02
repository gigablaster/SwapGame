#pragma once

#include "CoreMinimal.h"
#include "PatrolRouteComponent.generated.h"

UCLASS()
class SWAP_API UPatrolRouteComponent : public USceneComponent
{
	GENERATED_BODY()
public:
	UPatrolRouteComponent();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	UFUNCTION(BlueprintCallable, Category="Patrol Route")
	int GetNextWaypointIndex(int Current)const;

	UFUNCTION(BlueprintCallable, Category="Patrol Route")
	FVector GetWaypoint(int Index)const;

	UFUNCTION(BlueprintCallable, Category="Patrol Route")
	FVector GetWaypointWorldSpace(int Index)const;	

	UFUNCTION(BlueprintCallable, Category="Patrol Route")
	int GetClosestWaypoint(const FVector& Location)const;

	UFUNCTION(BlueprintCallable, Category="Patrol Route")
	int AddWaypoint(const FVector& Location);

	UFUNCTION(BlueprintCallable, Category="Patrol Route")
    int AddWaypointWorldSpace(const FVector& Location);

	UFUNCTION(BlueprintCallable, Category="Patrol Route")
	int InsertWaypoint(const FVector& Location, int Index);

	UFUNCTION(BlueprintCallable, Category="Patrol Route")
	int InsertWaypointWorldSpace(const FVector& Location, int Index);

	UFUNCTION(BlueprintCallable, Category="Patrol Route")
	void RemoveWaypoint(int Index);

	UFUNCTION(BlueprintCallable, Category="Patrol Route")
	void SetWaypointLocation(const FVector& Location, int Index);

	UFUNCTION(BlueprintCallable, Category="Patrol Route")
    void SetWaypointLocationWorldSpace(const FVector& Location, int Index);

	UFUNCTION(BlueprintCallable, Category="Patrol Route")
	void OffsetWaypointLocation(const FVector& Offset, int Index);

	UFUNCTION(BlueprintCallable, Category="Patrol Route")
	void OffsetWaypointLocationWorldSpace(const FVector& Offset, int Index);

	UFUNCTION(BlueprintCallable, Category="Patrol Route")
	int GetWaypointsCount()const;
#if WITH_EDITORONLY_DATA
	FORCEINLINE FLinearColor GetRouteColor()const { return RouteColor; }
	FORCEINLINE FLinearColor GetSelectedRouteColor()const { return SelectedRouteColor; }
	FORCEINLINE FLinearColor GetWaypointColor()const { return WaypointColor; }
	FORCEINLINE FLinearColor GetSelectedWaypointColor()const { return SelectedWaypointColor; }
#endif
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Patrol Route")
	uint8 bFlipDirection : 1;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category="Patrol Route")
	FLinearColor RouteColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category="Patrol Route")
	FLinearColor SelectedRouteColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category="Patrol Route")
	FLinearColor WaypointColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category="Patrol Route")
	FLinearColor SelectedWaypointColor;
#endif // WITH_EDITORONLY_DATA

	UPROPERTY(BlueprintReadOnly, NoClear, Category="Patrol Route")
	TArray<FVector> Waypoints;
};
