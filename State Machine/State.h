// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

/**
 * 
 */
class SYNTHBIOTICDUNGEONC_API State
{
public:
	virtual ~State(){};
	int _id;
	virtual void Tick();
	virtual void OnEnter();
	virtual void OnExit();
};
