#include "Animation/AnimNotifies/AnimNotify_Footstep.h"
#include "Characters/FootstepInterface.h"

UAnimNotify_Footstep::UAnimNotify_Footstep()
{
}

void UAnimNotify_Footstep::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (IFootstepInterface* FootstepInterface = Cast<IFootstepInterface>(MeshComp->GetOwner()))
	{
		FootstepInterface->OnFootstep();
	}
}
