// Fill out your copyright notice in the Description page of Project Settings.


#include "Chase.h"

#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"

Chase::Chase(int id, AAIController * controller, AEnemyBase * controlledEnemy)
{
	_id = id;
	_controller = controller;
	_controlledEnemy = controlledEnemy;
}

void Chase::Tick() {
	// Might not need to do anything.
}

void Chase::OnEnter() {
	// Turn on Movement
	_controller->MoveToActor(_controlledEnemy->_Target);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Entered CHASE")));
}

void Chase::OnExit() {
	// Turn off Movement
	_controller->StopMovement();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Exited CHASE")));
}

