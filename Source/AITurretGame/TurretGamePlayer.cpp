#include "TurretGamePlayer.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"

ATurretGamePlayer::ATurretGamePlayer()
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = true;
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	RootComponent = Capsule;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
}

void ATurretGamePlayer::BeginPlay()
{
	Super::BeginPlay();
	DefaultCapsuleHalfHeight = Capsule->GetUnscaledCapsuleHalfHeight();
	BaseAccelerationSpeed = AccelerationSpeed;
}

void ATurretGamePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATurretGamePlayer::HandleMoveForwardInput);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ATurretGamePlayer::HandleMoveRightInput);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ATurretGamePlayer::HandleLookUpInput);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &ATurretGamePlayer::HandleLookRightInput);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ATurretGamePlayer::HandlePressJumpInput);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &ATurretGamePlayer::HandleReleaseJumpInput);
	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &ATurretGamePlayer::HandlePressCrouchInput);
	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Released, this, &ATurretGamePlayer::HandleReleaseCrouchInput);
}

void ATurretGamePlayer::HandleMoveForwardInput(float Value)
{
	ForwardInput = Value;
}

void ATurretGamePlayer::HandleMoveRightInput(float Value)
{
	RightInput = Value;
}

void ATurretGamePlayer::HandleLookUpInput(float Value)
{
	AddControllerPitchInput(Value);
}

void ATurretGamePlayer::HandleLookRightInput(float Value)
{
	AddControllerYawInput(Value);
}

void ATurretGamePlayer::HandlePressJumpInput()
{
	JumpInput = true;
}

void ATurretGamePlayer::HandleReleaseJumpInput()
{
	JumpInput = false;
}

void ATurretGamePlayer::HandlePressCrouchInput()
{
	IsCrouching = true;
}

void ATurretGamePlayer::HandleReleaseCrouchInput()
{
	IsCrouching = false;
}

bool ATurretGamePlayer::IsGrounded()
{
	FHitResult Hit;
	FVector Origin = GetActorLocation();
	FVector End = Origin + FVector::DownVector * (Capsule->Bounds.BoxExtent.Z + 0.2f);

	GetWorld()->LineTraceSingleByChannel(Hit, Origin, End, GroundTraceChannel.GetValue());

	return Hit.bBlockingHit;
}

void ATurretGamePlayer::HandleCrouch(float DeltaTime)
{
	const float TargetHalfHeight = IsCrouching ? CapsuleHalfHeightCrouch : DefaultCapsuleHalfHeight;
	const float CurrentHalfHeight = Capsule->GetUnscaledCapsuleHalfHeight();

	const float TargetAccelerationSpeed = IsCrouching ? AccelerationSpeedWhileCrouched : BaseAccelerationSpeed;

	if(FMath::IsNearlyZero(TargetHalfHeight - CurrentHalfHeight) && FMath::IsNearlyZero(AccelerationSpeed - TargetAccelerationSpeed))
		return;

	const float NewHalfHeight = UKismetMathLibrary::Lerp(CurrentHalfHeight, TargetHalfHeight, CrouchSpeed * DeltaTime);
	AccelerationSpeed = UKismetMathLibrary::Lerp(AccelerationSpeed, TargetAccelerationSpeed, CrouchSpeed * DeltaTime);
	
	AddActorLocalOffset(FVector::UpVector * (NewHalfHeight - CurrentHalfHeight));
	Capsule->SetCapsuleHalfHeight(NewHalfHeight);
}

void ATurretGamePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector Acceleration = (ForwardInput * GetActorForwardVector() + RightInput * GetActorRightVector()).GetSafeNormal() * AccelerationSpeed;
	Velocity += (Acceleration - Velocity * Friction) * DeltaTime;
	Velocity -= Gravity * FVector::UpVector * DeltaTime;

	HandleCrouch(DeltaTime);

	if(JumpInput && IsGrounded())
	{
		Velocity += GetActorUpVector() * JumpImpulse;
	}

	FVector DeltaToMove = Velocity * DeltaTime;
	
	FHitResult Hit;

	for(int i = 0; i < MovementIterativeDepth; i++)
	{
		AddActorWorldOffset(DeltaToMove, true, &Hit);

		DeltaToMove -= DeltaToMove * Hit.Time;
		
		if(Hit.bBlockingHit)
		{
			FVector DeprenetationDelta = FVector::DotProduct(Hit.Normal, Velocity) * Hit.Normal;
			Velocity -= DeprenetationDelta;
			
			if(DeltaToMove.IsNearlyZero())
				break;
			
			DeltaToMove -= FVector::DotProduct(DeltaToMove, Hit.Normal) * Hit.Normal;
		}
	}
}
