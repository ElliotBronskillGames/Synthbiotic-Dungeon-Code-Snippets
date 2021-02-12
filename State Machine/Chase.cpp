// Fill out your copyright notice in the Description page of Project Settings.


#include "Chase.h"

#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"

Chase::Chase(int id, AAIController * controller, ACharacter * target)
{
	_id = id;
	_controller = controller;
	_target = target;
}

void Chase::Tick() {
	// Might not need to do anything.
}

void Chase::OnEnter() {
	// Turn on Movement
	_controller->K2_ClearFocus(); // Might need to use other clearfocus()
	_controller->MoveToActor(_target);
}

void Chase::OnExit() {
	// Turn off Movement
	_controller->StopMovement();
}

