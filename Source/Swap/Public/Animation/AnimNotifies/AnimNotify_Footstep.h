#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_Footstep.generated.h"

UCLASS()
class SWAP_API UAnimNotify_Footstep : public UAnimNotify
{
	GENERATED_BODY()
public:
	UAnimNotify_Footstep();
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
