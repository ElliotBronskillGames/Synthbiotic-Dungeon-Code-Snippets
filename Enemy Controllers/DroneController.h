// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include <string>

#include "AIController.h"
#include "StateMachine.h"
#include "Idle.h"
#include "Chase.h"
#include "Attack.h"
#include "EnemyBase.h"

#include "DroneController.generated.h"

/**
 * 
 */
UCLASS()
class SYNTHBIOTICDUNGEONC_API ADroneController : public AAIController
{
public:
	UPROPERTY(EditDefaultsOnly, Category="Controller Variables") AEnemyBase * _controlledEnemy;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Controller Variables") TMap<FString, UBehaviorTree*> _behaviorTrees;
private:
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
	StateMachine * _stateMachine = new StateMachine();
protected:
	~ADroneController();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	bool InRangeAndHasLOS();
	bool OutOfRangeOrNoLOS();
	bool CheckLOSToTarget();
};

