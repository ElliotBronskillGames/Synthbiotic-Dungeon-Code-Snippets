// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "State.h"

/**
 * 
 */
class SYNTHBIOTICDUNGEONC_API Chase : public State
{
	AAIController * _controller;
	AActor * _target;
public:
	Chase(int id, AAIController * controller, ACharacter * target);
	virtual void Tick() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;
};
