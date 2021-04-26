#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VisionSensingComponent.generated.h"

class UVisionTargetComponent;
class UAITurretGameInstance;

USTRUCT(BlueprintType)
struct FVisionSensingData
{
	GENERATED_BODY()

	FVisionSensingData() {}

	FVisionSensingData(AActor* Actor, UVisionTargetComponent* Component)
	{
		TargetActor = Actor;
		TargetComponent = Component;
	}

	UPROPERTY(BlueprintReadOnly)
	AActor* TargetActor;

	UPROPERTY(BlueprintReadOnly)
	UVisionTargetComponent* TargetComponent;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FVisionSenseDelegate, const FVisionSensingData&, SensingData);

class UCustomVisionRoot;

UCLASS(ClassGroup=(AI), meta=(BlueprintSpawnableComponent))
class AITURRETGAME_API UVisionSensingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UVisionSensingComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Vision angle of sensing component, 90 means a 180 degree arc in front of the actor will be sensed */
	UPROPERTY(EditAnywhere)
	float VisionAngle;

	/** Draw vision angle to debug */
	UPROPERTY(EditAnywhere)
	bool bDebugVisionAngle;

	/** If root component of the vision target is within this distance and in view, it will be sensed */
	UPROPERTY(EditAnywhere)
	float DistanceMinimum;

	/** The trace channel that will be used to check if the player is obstructed by anything */
	UPROPERTY(EditAnywhere)
	TEnumAsByte<ECollisionChannel> ObstructionTraceChannel;

	/** When the owner of this vision sensing component establishes direct line of sight with an actor that has a vision target component */
	UPROPERTY(BlueprintAssignable)
	FVisionSenseDelegate OnFindVisionTarget;

	/** Called every tick for each target component this vision component has direct line of sight of */
	UPROPERTY(BlueprintAssignable)
	FVisionSenseDelegate OnTickVisionTarget;

	/** When the owner of this vision sensing component looses direct line of sight with an actor that has a vision target component */
	UPROPERTY(BlueprintAssignable)
	FVisionSenseDelegate OnLooseVisionTarget;

	UFUNCTION(BlueprintCallable)
	bool IsTargetSeen(UVisionTargetComponent* TargetComponent);

private:
	UAITurretGameInstance* GameInstance;
	TSet<UVisionTargetComponent*> SensedComponents;
	UCustomVisionRoot* OptionalSceneComponentOverride;

	bool IsPointVisible(const FVector& PointToTest, const FVector& Origin, const FVector& Forward) const;
	bool IsTargetObstructed(const FVector& VisionTarget, const FVector& Origin) const;
};
