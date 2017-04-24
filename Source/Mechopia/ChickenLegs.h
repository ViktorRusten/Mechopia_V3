// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "ChickenLegs.generated.h"

UCLASS()
class MECHOPIA_API AChickenLegs : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AChickenLegs();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Attacking = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		int Damage = 1;

	FVector ToPlayer;
	FVector Direction;
	FRotator Rotation;
	float Counter = 1.f;
	float Health = 3.f;
	float Length;
	void Move();
	void Attack();
	void OnHit();
	bool Close = false;
	bool Active = false;
	FTimerHandle AttackTimerHandle;
	
};
