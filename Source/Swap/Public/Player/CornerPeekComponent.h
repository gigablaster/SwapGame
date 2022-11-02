// Copyright (c) 2020-2021 Vladimir Kuskov
#pragma once

#include "CoreMinimal.h"
#include "SwapTypes.h"
#include "CornerPeekComponent.generated.h"

UCLASS()
class SWAP_API UCornerPeekComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UCornerPeekComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	FORCEINLINE ECornerPeekSide GetCurrentSide()const { return Side; }
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Peek)
	float SideDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Peek)
	float UpDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Peek)
	float ForwardDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Peek)
	float EmptyCheckDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Peek)
	float Step;

	UPROPERTY(BlueprintReadOnly, Category=Peek)
	ECornerPeekSide Side;
private:
	bool CheckIfWallAhead(UWorld* World, const FVector& Origin, const FVector& Forward, float Spread, FVector& OutOrigin, FVector& OutNormal)const;
	bool CheckIfWallAhead(UWorld* World, const FVector& Origin, const FVector& Forward, FVector& OutOrigin, FVector& OutNormal)const;
	float FindClosestEmptySpaceInDirection(UWorld* World, const FVector& Origin, const FVector& Direction, const FVector& Forward, float Distance)const;
};
