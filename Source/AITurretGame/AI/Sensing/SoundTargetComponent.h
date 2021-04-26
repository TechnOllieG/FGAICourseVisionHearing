#pragma once
#include "Components/ActorComponent.h"
#include "SoundTargetComponent.generated.h"

class UAITurretGameInstance;

UCLASS(ClassGroup=(AI), meta=(BlueprintSpawnableComponent))
class AITURRETGAME_API USoundTargetComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USoundTargetComponent();
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void PlaySound(float Loudness);

private:
	UAITurretGameInstance* GameInstance;
};