#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraModifier.h"
#include "CameraModifier_HeadBob.generated.h"

UCLASS(Abstract)
class SWAP_API UCameraModifier_HeadBob : public UCameraModifier
{
	GENERATED_BODY()
public:
	UCameraModifier_HeadBob(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual bool ModifyCamera(float DeltaTime, FMinimalViewInfo& InOutPOV) override;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxRoll;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxVOffset;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxHOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MinSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxSpeed;
};
