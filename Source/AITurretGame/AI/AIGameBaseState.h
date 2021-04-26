#pragma once
#include "UObject/Object.h"
#include "AIGameBaseState.generated.h"

class UStateMachineComponent;

UCLASS(BlueprintType, Blueprintable, DefaultToInstanced, EditInlineNew)
class AITURRETGAME_API UAIGameBaseState : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	UStateMachineComponent* StateMachine;

	UPROPERTY(BlueprintReadOnly)
	AActor* Owner;
	
	virtual void StateEnter() { BP_StateEnter(); }

	UFUNCTION(BlueprintImplementableEvent)
	void BP_StateEnter();

	virtual void StateTick(float DeltaTime) { BP_StateTick(DeltaTime); }
	
	UFUNCTION(BlueprintImplementableEvent)
	void BP_StateTick(float DeltaTime);

	virtual void StateExit() { BP_StateExit(); }
	
	UFUNCTION(BlueprintImplementableEvent)
	void BP_StateExit();
};