// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Idle.h"
#include "Chase.h"
#include "Attack.h"
#include "EnemyBaseController.h"

#include "DroneController.generated.h"

/**
 * 
 */
UCLASS()
class SYNTHBIOTICDUNGEONC_API ADroneController : public AEnemyBaseController
{
	GENERATED_BODY()
	enum EStates
	{
		IDLE,
		CHASE,
		ATTACK
	};
	Idle * _idle;
	Chase * _chase;
	Attack * _attack;
protected:
	~ADroneController();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};

