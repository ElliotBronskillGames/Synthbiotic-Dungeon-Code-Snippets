// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Idle.h"
#include "Startled.h"
#include "Chase.h"
#include "Heal.h"
#include "HealingDrone.h"
#include "EnemyBaseController.h"

#include "HealingDroneController.generated.h"

/**
 * 
 */
UCLASS()
class SYNTHBIOTICDUNGEONC_API AHealingDroneController : public AEnemyBaseController
{
	GENERATED_BODY()

	enum EStates
	{
		IDLE,
		STARTLED,
		CHASE,
		HEAL
	};
	
	Idle * _idle;
	Startled * _startled;
	Chase * _chase;
	class Heal * _heal;
public:
	UPROPERTY(EditDefaultsOnly, Category="Controller Variables") AHealingDrone * _controlledHealingDrone;
protected:
	AHealingDroneController();
	~AHealingDroneController();
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void BuildStateMachine() override;
};
