#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraModifier.h"
#include "CameraModifier_CoverPeek.generated.h"

UCLASS(Abstract)
class SWAP_API UCameraModifier_CoverPeek : public UCameraModifier
{
	GENERATED_BODY()
public:
	UCameraModifier_CoverPeek(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual bool ModifyCamera(float DeltaTime, FMinimalViewInfo& InOutPOV) override;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Peek)
	float MaxSideAngle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Peek)
	float MaxSideOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Peek)
	float MaxVerticalOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Peek)
	float Rate;
private:
	float CurrentOffset;
	float CurrentRoll;
};
