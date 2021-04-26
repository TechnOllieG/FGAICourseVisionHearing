#pragma once
#include "Components/ActorComponent.h"
#include "StateMachineComponent.generated.h"

class UAIGameBaseState;

DECLARE_LOG_CATEGORY_EXTERN(LogStateMachine, Log, All);

UCLASS(ClassGroup=(AI), meta=(BlueprintSpawnableComponent))
class AITURRETGAME_API UStateMachineComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStateMachineComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** The state that will be entered on BeginPlay, indices out of range of the states array will mean no initial state */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int InitialState = 0;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TSubclassOf<UAIGameBaseState>> States;

	UFUNCTION(BlueprintCallable)
	void ChangeState(int StateIndex);

private:
	UAIGameBaseState* CurrentState = nullptr;
	TArray<UAIGameBaseState*> StateInstances;
};