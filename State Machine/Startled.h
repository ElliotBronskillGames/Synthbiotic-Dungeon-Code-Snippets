// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "State.h"
#include "CoreMinimal.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnemyBase.h"
#include "EnemyBaseController.h"

/**
 * 
 */
class SYNTHBIOTICDUNGEONC_API Startled : public State
{
public:
	Startled(int id, AEnemyBase * controlledEnemy, AEnemyBaseController* controller);
	~Startled();
	virtual void Tick() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;
private:
	AEnemyBase * _controlledEnemy;
	AEnemyBaseController * _controller;
};
