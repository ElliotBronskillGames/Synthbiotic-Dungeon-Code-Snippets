// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include <list>


#include "AITrapEnemy.h"
#include "SynthbioticDungeonCCharacter.h"
#include "GameFramework/Character.h"
#include "Components/ArrowComponent.h"
#include "EnemyBase.generated.h"

UCLASS()
class SYNTHBIOTICDUNGEONC_API AEnemyBase : public AAITrapEnemy
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy Properties") float _Health;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy Properties") float _Damage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy Properties") float _Movespeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy Properties") float _SightRange;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy Properties") float _AttackRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Properties") TArray<ASynthbioticDungeonCCharacter*> _PlayersInRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Properties") ASynthbioticDungeonCCharacter * _Target; // Set by the enemy Character
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
