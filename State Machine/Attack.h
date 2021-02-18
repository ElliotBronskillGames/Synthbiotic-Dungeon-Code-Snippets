// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "EnemyBaseController.h"
#include "State.h"

/**
 * 
 */
class SYNTHBIOTICDUNGEONC_API Attack : public State
{
	AEnemyBaseController * _controller;
public:
	Attack(int id,  AEnemyBaseController * controller);
	virtual void Tick() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;
};
