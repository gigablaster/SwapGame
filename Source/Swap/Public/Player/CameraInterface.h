// Copyright (c) 2020-2021 Vladimir Kuskov
#pragma once

#include "CoreMinimal.h"
#include "CameraInterface.generated.h"

class UCameraComponent;

UINTERFACE(meta = (CannotImplementInterfaceInBlueprint))
class SWAP_API UCameraInterface : public UInterface
{
	GENERATED_BODY()
};

class ICameraInterface
{
	GENERATED_BODY()
public:
	virtual UCameraComponent* GetCameraComponent()const = 0;
};
