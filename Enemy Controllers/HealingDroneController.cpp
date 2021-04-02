// Fill out your copyright notice in the Description page of Project Settings.


#include "HealingDroneController.h"

AHealingDroneController::AHealingDroneController()
{
	_controlledHealingDrone = Cast<AHealingDrone>(_controlledEnemy);
}

AHealingDroneController::~AHealingDroneController()
{
	delete _idle;
	delete _startled;
	delete _chase;
	delete _heal;
}

void AHealingDroneController::BeginPlay()
{
	Super::BeginPlay();
	BuildStateMachine();
}

void AHealingDroneController::BuildStateMachine()
{
	_idle  = new Idle(IDLE);
	_startled = new Startled(STARTLED, this);
	_chase = new Chase(CHASE, this);
	_heal = new Heal(HEAL, this);
	
	// Setup statemachine
	_stateMachine->AddTransition(_idle, _startled, [this]()->bool
    {
        return IsValid(_controlledHealingDrone->_HealingTarget);
    });
	_stateMachine->AddTransition(_startled, _chase,[this]()->bool
    {
        return UKismetMathLibrary::Abs(AngleToTarget(_controlledHealingDrone->_HealingTarget)) < 10;
    });
	_stateMachine->AddTransition(_chase, _heal, [this]()->bool
    {
        return InRangeAndHasLOS(_controlledHealingDrone, _controlledHealingDrone->_HealingTarget);
    });
	_stateMachine->AddTransition(_heal, _chase,[this]()->bool
    {
        return OutOfRangeOrNoLOS(_controlledHealingDrone, _controlledHealingDrone->_HealingTarget)
		&& UKismetMathLibrary::Abs(AngleToTarget(_controlledHealingDrone->_HealingTarget)) < 10;
    });
	
	_stateMachine->AddAnyTransition(_idle, [this]()->bool
    {
        // Target is dead
        return !IsValid(_controlledEnemy->_Target);
    });

	_stateMachine->_currentState = _idle; // Set base state
	_stateMachine->_currentTransitions = _stateMachine->_transitions->at(_idle->_id); // Set base transitions
}

void AHealingDroneController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (_controlledHealingDrone == nullptr) _controlledHealingDrone = Cast<AHealingDrone>(_controlledEnemy);
	if (_controlledEnemy != nullptr && _controlledHealingDrone != nullptr && _stateMachine != nullptr)
		_stateMachine->Tick();
}
