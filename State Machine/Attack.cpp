// Fill out your copyright notice in the Description page of Project Settings.


#include "Attack.h"

#include "BrainComponent.h"
#include "EnemyBaseController.h"

Attack::Attack(int id,  AEnemyBaseController * controller)
{
	_id = id;
	_controller = controller;
}

void Attack::Tick() {
	// Might need to check vars used in BT
}

void Attack::OnEnter() {
	// Run Attack BT
	_controller->RunBehaviorTree(*_controller->_behaviorTrees.Find("ATTACK"));
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Entered ATTACK")));
}

void Attack::OnExit() {
	// Stop Running Attack BT
	_controller->BrainComponent->StopLogic("Exited Attack State");
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Exited ATTACK")));
}
