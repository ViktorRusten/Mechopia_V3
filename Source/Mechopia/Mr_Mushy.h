// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Mr_Mushy.generated.h"




UCLASS()
class MECHOPIA_API AMr_Mushy : public ACharacter
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class AMushy_HBox> Mushy_HBox_BP;




public:
	// Sets default values for this character's properties
	AMr_Mushy();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Attacking = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		int Damage = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")       //Category gir en overskrift i Editoren
		AActor* ThePlayerActor;

	UFUNCTION(BlueprintPure, Category = "Damage")
		int DealDamage();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Active = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Close = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Dashing = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Dead = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float Health = 3.f;

	FVector ToPlayer;
	FVector Direction;
	FRotator Rotation;
	float Counter = 1.f;
	float Length;
	void Dash();
	void Move();
	void Attack();
	void OnHit();
	void Death();
	FTimerHandle DashTimerHandle;
	FTimerHandle AttackTimerHandle;
	FTimerHandle DeathTimerHandle;
};
