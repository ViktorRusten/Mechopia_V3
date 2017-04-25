// Fill out your copyright notice in the Description page of Project Settings.

#include "Mechopia.h"
#include "ChickenLegs.h"
#include "MechopiaCharacter.h"


// Sets default values
AChickenLegs::AChickenLegs()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AChickenLegs::BeginPlay()
{
	Super::BeginPlay();
	
	GetCharacterMovement()->MaxWalkSpeed = 0.f;
	Active = false;
	Attacking = false;
}

// Called every frame
void AChickenLegs::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (UGameplayStatics::GetPlayerCharacter(GetWorld(), 0) && Dead == false)
	{
		ToPlayer = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation() - GetActorLocation();
		Length = ToPlayer.Size();

		if (Active == false && Length <= 1200)
		{
			Active = true;
			AChickenLegs::Move();
			UE_LOG(LogTemp, Log, TEXT("Player Seen"));
		}
		else if (Active == false)
		{
			GetCharacterMovement()->MaxWalkSpeed = 0.f;
		}


		if (Active == true)
		{
			//Measures the distance between the enemy and the player.


			if (Length <= 150.f)
			{
				Close = true;
				// Find out which way is forward
				Rotation = Controller->GetControlRotation();
				FRotator YawRotation(0, Rotation.Yaw, 0);

				// Get forward vector
				Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

				AChickenLegs::Move();

				// Makes the enemy stop it's forward movement.
				float Value = -600.f;
				AddMovementInput(Direction, Value);


				// If the enemy is currently not attacking, it wll attack.
				if (Attacking == false)
				{
					Attacking = true;
					GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &AChickenLegs::Attack, 1.0f, true);
					UE_LOG(LogTemp, Warning, TEXT("Starting attack"));

				}

				//GetRootPrimitiveComponent()->AddImpulse(GetActorForwardVector() * -500.0f);
				//GetRootPrimitiveComponent()->SetPhysicsLinearVelocity(Direction*0);		
			}
		}
		else
		{
			Close = false;
		}
	}
}

// Called to bind functionality to input
void AChickenLegs::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AChickenLegs::Move()
{

	//GetRootPrimitiveComponent()->SetPhysicsLinearVelocity(Direction * 200);
	GetCharacterMovement()->MaxWalkSpeed = 800.f;

	//Counter = 1.f;

}


int AChickenLegs::DealDamage()
{
	if (ThePlayerActor->IsA(AMechopiaCharacter::StaticClass())) {
		AMechopiaCharacter* Player = Cast<AMechopiaCharacter>(ThePlayerActor);
		Player->TakingDamage(Damage);
	}

	return 0;
}


void AChickenLegs::Attack()
{
	Attacking = false;
	UE_LOG(LogTemp, Warning, TEXT("Attack done"));

}

void AChickenLegs::OnHit()
{
	Health--;

	if (Health <= 0)
	{
		GetCharacterMovement()->MaxWalkSpeed = 0.f;
		Dead = true;
		GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &AChickenLegs::Death, 1.f, true);
	}


}

void AChickenLegs::Death()
{
	Destroy();
}