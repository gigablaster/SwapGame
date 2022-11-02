#include "AI/PatrolRouteComponent.h"

UPatrolRouteComponent::UPatrolRouteComponent()
{
	bFlipDirection = false;
	Waypoints.Add(FVector::ZeroVector);
#if WITH_EDITORONLY_DATA
	RouteColor = FColor::Magenta;
	WaypointColor = FColor::Green;
	SelectedWaypointColor = FColor::Blue;
	SelectedRouteColor = FColor::Red;
#endif 
}

#if WITH_EDITOR
void UPatrolRouteComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (Waypoints.Num() == 0)
	{
		Waypoints.Add(FVector::ZeroVector);
	}
}
#endif

int UPatrolRouteComponent::GetClosestWaypoint(const FVector& Location) const
{
	int Point = -1;

	if (Waypoints.Num() > 0)
	{
		const FTransform Transform = GetComponentTransform();
		const FVector Relative = Transform.InverseTransformPosition(Location);
		float ClosestSq = FVector::DistSquared(Relative, Waypoints[0]);
		Point = 0;
		for (int Index = 1; Index < Waypoints.Num(); ++Index)
		{
			float DistSq = FVector::DistSquared(Relative, Waypoints[Index]);
			if (DistSq < ClosestSq)
			{
				ClosestSq = DistSq;
				Point = Index;
			}
		}
	}

	return Point;
}

int UPatrolRouteComponent::GetNextWaypointIndex(int Current) const
{
	const int Direction = bFlipDirection ? -1 : 1;
	int NextPoint = Current + Direction;
	if (NextPoint < 0)
	{
		NextPoint = Waypoints.Num() - 1;
	}
	if (NextPoint >= Waypoints.Num())
	{
		NextPoint = 0;
	}

	return NextPoint;
}

FVector UPatrolRouteComponent::GetWaypoint(int Index) const
{
	if (Waypoints.IsValidIndex(Index))
	{
		return Waypoints[Index];	
	}

	return FVector::ZeroVector;	
}

FVector UPatrolRouteComponent::GetWaypointWorldSpace(int Index) const
{
	return GetComponentTransform().TransformPosition(GetWaypoint(Index));
}

int UPatrolRouteComponent::AddWaypoint(const FVector& Location)
{
	return Waypoints.Add(Location);
}

int UPatrolRouteComponent::AddWaypointWorldSpace(const FVector& Location)
{
	return AddWaypoint(GetComponentTransform().InverseTransformPosition(Location));
}

int UPatrolRouteComponent::GetWaypointsCount() const
{
	return Waypoints.Num();
}

void UPatrolRouteComponent::RemoveWaypoint(int Index)
{
	if (Waypoints.IsValidIndex(Index))
	{
		Waypoints.RemoveAt(Index);
		if (Waypoints.Num() == 0)
		{
			Waypoints.Add(FVector::ZeroVector);
		}	
	}
}

int UPatrolRouteComponent::InsertWaypoint(const FVector& Location, int Index)
{
	if (Waypoints.IsValidIndex(Index))
	{
		return Waypoints.Insert(Location, Index);
	}

	return -1;
}

int UPatrolRouteComponent::InsertWaypointWorldSpace(const FVector& Location, int Index)
{
	return InsertWaypoint(GetComponentTransform().InverseTransformPosition(Location), Index);
}

void UPatrolRouteComponent::SetWaypointLocation(const FVector& Location, int Index)
{
	if (Waypoints.IsValidIndex(Index))
	{
		Waypoints[Index] = Location;
	}
}

void UPatrolRouteComponent::SetWaypointLocationWorldSpace(const FVector& Location, int Index)
{
	SetWaypointLocation(GetComponentTransform().InverseTransformPosition(Location), Index);
}

void UPatrolRouteComponent::OffsetWaypointLocation(const FVector& Offset, int Index)
{
	if (Waypoints.IsValidIndex(Index))
	{
		Waypoints[Index] += Offset;
	}
}

void UPatrolRouteComponent::OffsetWaypointLocationWorldSpace(const FVector& Offset, int Index)
{
	OffsetWaypointLocation(GetComponentTransform().InverseTransformPosition(Offset), Index);
}
