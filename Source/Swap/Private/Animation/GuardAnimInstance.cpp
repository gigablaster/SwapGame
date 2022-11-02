#include "Animation/GuardAnimInstance.h"
#include "Enemies/GuardCharacterBase.h"

UGuardAnimInstance::UGuardAnimInstance()
{
	AlertLevel = EGuardAlertLevel::None;	
}

void UGuardAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	CharacterSpeed = GetOwningActor()->GetVelocity().Size();
	if (const AGuardCharacterBase* Guard = Cast<AGuardCharacterBase>(GetOwningActor()))
	{
		AlertLevel = Guard->GetAlertLevel();
	}
	bIsMoving = CharacterSpeed > 10.0f;
}
