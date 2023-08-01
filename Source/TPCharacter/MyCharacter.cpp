// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//To stop the character from following camera's rotation
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	//The character will rotate to the direction of its movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->AirControl = 100.f;
	GetCharacterMovement()->JumpZVelocity = 800.f;

	//Setting up Spring arm component
	cameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	cameraBoom->TargetArmLength = 300.f;
	cameraBoom->bUsePawnControlRotation = true;
	cameraBoom->SetupAttachment(RootComponent);

	//Main third person camera
	followCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Follow Camera"));
	followCamera->bUsePawnControlRotation = false;
	followCamera->FieldOfView = 100.f;
	followCamera->SetupAttachment(cameraBoom, USpringArmComponent::SocketName);

	//Default Health of character
	health = 100.f;
	//healthDecreaseRate = 20.f;
	bIsDead = false;

	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMyCharacter::BeginOverlap);
	}
	
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (playerHUD)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player HUD Found"));
		playerWidget = CreateWidget(GetWorld(), playerHUD);
		playerWidget->AddToViewport(1);
	}
}


// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetCharacterMovement()->IsFalling())
	{
		GetCharacterMovement()->RotationRate = FRotator(0.f, 80.f, 0.f);
	}
	else
	{
		GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	}

	health -= DeltaTime * healthDecreaseRate;
	
	if (health <= 0 && !bIsDead)
	{
		bIsDead = true;
		UE_LOG(LogTemp, Warning, TEXT("Collided with Collectible"));
		GetMesh()->SetSimulatePhysics(true);

		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(UnusedHandle, this,
			&AMyCharacter::RestartGame, 3.0f, false);

		GetController()->SetIgnoreMoveInput(true);
		
		
	}
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	//Binding to look around
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &AMyCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, & AMyCharacter::AddControllerPitchInput);

	//Binding to make character jump
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMyCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMyCharacter::StopJumping);

	//Binding to move character
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AMyCharacter::MoveRight);

}

//On Keyboard input: W and S
void AMyCharacter::MoveForward(float axisValue)
{
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, axisValue);
}

//On Keyboard input: A and D
void AMyCharacter::MoveRight(float axisValue)
{
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, axisValue);
}

void AMyCharacter::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Collided with Collectible"));
	if (OtherActor->ActorHasTag("Collectible"))
	{
		health += 10.f;
		OtherActor->Destroy();
	}
}

void AMyCharacter::RestartGame()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);

}
