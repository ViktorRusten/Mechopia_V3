// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Mechopia.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "MechopiaCharacter.h"
#include "PlayerBullet.h"

//////////////////////////////////////////////////////////////////////////
// AMechopiaCharacter

AMechopiaCharacter::AMechopiaCharacter()
{
	ShootDelay = 0.3;

	ShootAnimDelay = 1;

	ShootDelayTimer = 0;

	ShootAnimTimer = 0;

	GoingToShoot = false;

	PlayerHealth = 5;

	CurrentHealth = PlayerHealth;

	PlayerHealthHUD = 1;

	InvincibleTimer = 2;

	DamageTimer = 0;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

												// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

												   // Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
												   // are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	SpawnDistance = 100;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMechopiaCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMechopiaCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMechopiaCharacter::MoveRight);


	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	//PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	//PlayerInputComponent->BindAxis("TurnRate", this, &AMechopiaCharacter::TurnAtRate);
	//PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	//PlayerInputComponent->BindAxis("LookUpRate", this, &AMechopiaCharacter::LookUpAtRate);

	InputComponent->BindAction("Fire", IE_Pressed, this, &AMechopiaCharacter::Fire);
}

void AMechopiaCharacter::Tick(float DeltaTime)
{

	if (DamageTimer > 0) {
		DamageTimer -= DeltaTime;
	}

	if (ShootAnimTimer > 0) {
		ShootAnimTimer -= DeltaTime;
	}
	else {
		Shooting = false;
	}

	if (ShootDelayTimer > 0) {
		ShootDelayTimer -= DeltaTime;
	}else if(GoingToShoot == true) {
		//  I f.eks. Shoot()-funksjon:
		UWorld* World = GetWorld();	//Henter peker til spillverdenen
		if (World)			//tester at verdenen finnes
		{
			UE_LOG(LogTemp, Warning, TEXT("World is present"));

			FVector Location = GetActorLocation();

			FRotator Rotation = GetActorRotation();

			FVector Forward = GetActorForwardVector();

			World->SpawnActor<APlayerBullet>(BulletBlueprint, Location + (Forward * SpawnDistance), Rotation);

			GoingToShoot = false;
		}
	}

	if (MovingVector.IsZero())
	{
		Moving = false;
	}
	else {
		Moving = true;
		MovingVector.X = 0;
		MovingVector.Y = 0;
	}
}

void AMechopiaCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMechopiaCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMechopiaCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);

		MovingVector.Y = Value;
	}
}

void AMechopiaCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);

		MovingVector.X = Value;
	}
}

void AMechopiaCharacter::Fire()
{
	if (ShootAnimTimer < 0.001f) {
		UE_LOG(LogTemp, Warning, TEXT("Trying to fire"));

			Shooting = true;

			GoingToShoot = true;

			ShootDelayTimer = ShootDelay;

			ShootAnimTimer = ShootAnimDelay;
	}
}

int AMechopiaCharacter::TakingDamage(int Damage) {

	if (DamageTimer < 1) {
		UE_LOG(LogTemp, Warning, TEXT("Taking damage"));
		if (Damage) {

			CurrentHealth -= Damage;
			DamageTimer = InvincibleTimer;
			PlayerHealthHUD = (CurrentHealth / PlayerHealth);
		}
		if (CurrentHealth < 1) {
			UE_LOG(LogTemp, Warning, TEXT("Dead"));
			UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
		}

	}

	return 0;
}
