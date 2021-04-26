#include "StateMachineComponent.h"
#include "AIGameBaseState.h"

DEFINE_LOG_CATEGORY(LogStateMachine);

UStateMachineComponent::UStateMachineComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UStateMachineComponent::BeginPlay()
{
	Super::BeginPlay();
	StateInstances.Reserve(States.Num());
	for(int i = 0; i < States.Num(); i++)
	{
		UAIGameBaseState* State = NewObject<UAIGameBaseState>(GetOwner(), States[i]);
		State->Owner = GetOwner();
		State->StateMachine = this;
		StateInstances.Add(State);
	}
	
	ChangeState(InitialState);
}

void UStateMachineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(CurrentState != nullptr)
		CurrentState->StateTick(DeltaTime);
}

void UStateMachineComponent::ChangeState(int StateIndex)
{
	if(StateIndex < 0 || StateIndex >= StateInstances.Num())
	{
		UE_LOG(LogStateMachine, Warning, TEXT("State Index is out of range, state was not changed"));
		return;
	}
	
	if(CurrentState != nullptr)
		CurrentState->StateExit();

	CurrentState = StateInstances[StateIndex];
	CurrentState->StateEnter();
}
