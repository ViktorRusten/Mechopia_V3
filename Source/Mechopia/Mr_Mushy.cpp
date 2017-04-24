// Fill out your copyright notice in the Description page of Project Settings.

#include "Mechopia.h"
#include "Mr_Mushy.h"
#include "Mushy_HBox.h"
#include "MechopiaCharacter.h"


// Sets default values
AMr_Mushy::AMr_Mushy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

// Called when the game starts or when spawned
void AMr_Mushy::BeginPlay()
{
	Super::BeginPlay();
	

	GetCharacterMovement()->MaxWalkSpeed = 0.f;
	Active = false;
	Attacking = false;
	
}

// Called every frame
void AMr_Mushy::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		ToPlayer = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation() - GetActorLocation();
		Length = ToPlayer.Size();

		if (Active == false && Length <= 1200)
		{
			Active = true;
			AMr_Mushy::Move();
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

				AMr_Mushy::Move();

				// Makes the enemy stop it's forward movement.
				float Value = -600.f;
				AddMovementInput(Direction, Value);


				// If the enemy is currently not attacking, it wll attack.
				if (Attacking == false)
				{
					Attacking = true;
					GetWorldTimerManager().SetTimer(DashTimerHandle, this, &AMr_Mushy::Attack, 1.0f, true);
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
void AMr_Mushy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMr_Mushy::Dash()
{
	if (Close == false)
	{
		GetCharacterMovement()->MaxWalkSpeed = 2300.f;
	}

	GetWorldTimerManager().SetTimer(DashTimerHandle, this, &AMr_Mushy::Move, 0.4f, true);

	//GetRootPrimitiveComponent()->AddImpulse(GetActorForwardVector() * 20000.0f);
}

void AMr_Mushy::Move()
{
	
	//GetRootPrimitiveComponent()->SetPhysicsLinearVelocity(Direction * 200);
	GetCharacterMovement()->MaxWalkSpeed = 300.f;

	
		GetWorldTimerManager().SetTimer(DashTimerHandle, this, &AMr_Mushy::Dash, 3.0f, true);
		//Counter = 1.f;
	
}

int AMr_Mushy::DealDamage()
{
	if (ThePlayerActor->IsA(AMechopiaCharacter::StaticClass())) {
		AMechopiaCharacter* Player = Cast<AMechopiaCharacter>(ThePlayerActor);
		Player->TakingDamage(Damage);
	}

	return 0;
}

void AMr_Mushy::Attack()
{
	/*
	UWorld* World = GetWorld();
	if (World)
	{
		FVector Location = GetActorLocation();
		World->SpawnActor<AMushy_HBox>(Mushy_HBox_BP, Location, Rotation);
	}
	*/
	Attacking = false;
	UE_LOG(LogTemp, Warning, TEXT("Attack done"));

}

void AMr_Mushy::OnHit()
{
	Health--;

	if (Health <= 0)
	{
		Destroy();
	}


}

