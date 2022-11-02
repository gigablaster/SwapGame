#include "Player/Camera/CameraModifier_HeadBob.h"
#include "Player/HeadBobInterface.h"

UCameraModifier_HeadBob::UCameraModifier_HeadBob(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MaxRoll = 0.5f;
	MaxVOffset = 25.0f;
	MaxHOffset = 5.0f;
	MinSpeed = 100.0f;
	MaxSpeed = 500.0f;
}

bool UCameraModifier_HeadBob::ModifyCamera(float DeltaTime, FMinimalViewInfo& InOutPOV)
{
	if (IHeadBobInterface* HeadBobInterface = Cast<IHeadBobInterface>(GetViewTarget()))
	{
		float HValue, VValue;
		HeadBobInterface->GetHeadBob(VValue, HValue);
		
		const FVector Velocity = GetViewTarget()->GetVelocity();
		const FVector Forward = GetViewTarget()->GetActorForwardVector().GetSafeNormal2D();
		const FVector Direction = Velocity.GetSafeNormal2D();
		const float Speed = Velocity.Size();
		const float Weight = FMath::GetMappedRangeValueClamped(FVector2D(MinSpeed, MaxSpeed), FVector2D(0.0f, 1.0f), Speed);
		const float RollWeight = Weight * FMath::Abs(FVector::DotProduct(Forward, Direction));
		const float HFactor = FMath::Sign(HValue) * HValue * HValue; 
		FVector Delta = FVector(0, MaxHOffset * RollWeight * HFactor,  MaxVOffset * VValue * Weight);
		InOutPOV.Location += InOutPOV.Rotation.RotateVector(Delta);
		InOutPOV.Rotation.Roll += MaxRoll * RollWeight * HFactor;
	}
	return false;
}
