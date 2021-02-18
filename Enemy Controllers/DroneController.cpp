// Fill out your copyright notice in the Description page of Project Settings.
#include "DroneController.h"


ADroneController::~ADroneController()
{
	delete _idle;
	delete _chase;
	delete _attack;
}

void ADroneController::BeginPlay()
{
	Super::BeginPlay();

	_controlledEnemy = Cast<AEnemyBase, APawn>(GetPawn());
		
	_idle  = new Idle(IDLE);
	_chase = new Chase(CHASE, this, _controlledEnemy);
	_attack = new Attack(ATTACK, this);
	
	// Setup statemachine
	_stateMachine->AddTransition(_idle, _chase,[this]()->bool
	{
		return IsValid(_controlledEnemy->_Target);
	}); 
	_stateMachine->AddTransition(_chase, _attack, [this]()->bool
	{
		//return InRangeAndHasLOS();
		return false;
	});
	_stateMachine->AddTransition(_attack, _chase,[this]()->bool
	{
		return OutOfRangeOrNoLOS();
	});
	
	_stateMachine->AddAnyTransition(_idle, [this]()->bool
	{
		// Target is dead
		return !IsValid(_controlledEnemy->_Target);
	});

	_stateMachine->_currentState = _idle; // Set base state
	_stateMachine->_currentTransitions = _stateMachine->_transitions->at(_idle->_id); // Set base transitions
}

void ADroneController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	_stateMachine->Tick();
}





