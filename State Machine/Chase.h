// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "EnemyBase.h"
#include "State.h"

/**
 * 
 */
class SYNTHBIOTICDUNGEONC_API Chase : public State
{
	AAIController * _controller;
	AEnemyBase * _controlledEnemy;
public:
	Chase(int id, AAIController * controller, AEnemyBase * controlledEnemy);
	virtual void Tick() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;
};
