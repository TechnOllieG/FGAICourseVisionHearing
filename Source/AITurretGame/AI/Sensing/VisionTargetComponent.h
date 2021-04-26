#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VisionTargetComponent.generated.h"

class UAITurretGameInstance;

UCLASS(ClassGroup=(AI), meta=(BlueprintSpawnableComponent))
class AITURRETGAME_API UVisionTargetComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UVisionTargetComponent();
	virtual void BeginPlay() override;

	virtual void BeginDestroy() override;

	FVector CollisionExtentsCollider;

private:
	UAITurretGameInstance* GameInstance;
};
