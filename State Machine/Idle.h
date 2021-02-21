// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "State.h"

/**
 * 
 */
class SYNTHBIOTICDUNGEONC_API Idle : public State
{
public:
	Idle(int id);
	virtual void Tick() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;
};
