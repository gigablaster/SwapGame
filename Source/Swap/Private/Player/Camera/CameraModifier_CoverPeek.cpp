#include "Player/Camera/CameraModifier_CoverPeek.h"
#include "SwapTypes.h"
#include "Player/CornerPeekInterface.h"

UCameraModifier_CoverPeek::UCameraModifier_CoverPeek(const FObjectInitializer& ObjectInitializer)
{
	MaxSideAngle = 30.0f;
	MaxSideOffset = 40.0f;
	MaxVerticalOffset = 40.0f;
	Rate = 4.0f;
}

bool UCameraModifier_CoverPeek::ModifyCamera(float DeltaTime, FMinimalViewInfo& InOutPOV)
{
	AActor* ViewTarget = GetViewTarget();
	if (ICornerPeekInterface* CoverPeekInterface = Cast<ICornerPeekInterface>(ViewTarget))
	{
		const ECornerPeekSide Side = CoverPeekInterface->GetCornerPeekSide(); 
		float TargetOffset;
		float TargetRoll;
		switch (Side)
		{
		case ECornerPeekSide::Left:
			TargetOffset = -MaxSideOffset;
			TargetRoll = -MaxSideAngle;
			break;
		case ECornerPeekSide::Right:
			TargetOffset = MaxSideOffset;
			TargetRoll = MaxSideAngle;
			break;
		case ECornerPeekSide::Top:
			TargetOffset = 0;
			TargetRoll = 0;
			break;
		default:
			TargetOffset = 0;
			TargetRoll = 0;
			break;
		}
		CurrentOffset = FMath::FInterpTo(CurrentOffset, TargetOffset, DeltaTime, Rate);
		CurrentRoll = FMath::FInterpTo(CurrentRoll, TargetRoll, DeltaTime, Rate);

		FVector Offset = FVector(0, CurrentOffset, -FMath::Abs(CurrentOffset) / 4.0f);
		Offset = InOutPOV.Rotation.Quaternion() * Offset;
		InOutPOV.Location += Offset;
		InOutPOV.Rotation.Roll += CurrentRoll;
	}

	return false;
}
