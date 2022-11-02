#include "Characters/SwapMovementComponent.h"
#include "Characters/CharacterBase.h"

USwapMovementComponent::USwapMovementComponent()
{}

float USwapMovementComponent::GetMaxSpeed() const
{
	ACharacterBase* CharacterBase = Cast<ACharacterBase>(GetOwner());
	if (CharacterBase == nullptr)
	{
		return Super::GetMaxSpeed();
	}
	return CharacterBase->GetMoveSpeed();
}

float USwapMovementComponent::GetGravityZ() const
{
	return Velocity.Z < 0 ? Super::GetGravityZ() * 2.0f : Super::GetGravityZ();
}
