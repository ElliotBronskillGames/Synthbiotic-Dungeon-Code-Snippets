// Fill out your copyright notice in the Description page of Project Settings.


#include "Startled.h"

Startled::Startled(int id, AEnemyBase * controlledEnemy, AEnemyBaseController * controller)
{
	_id = id;
	_controlledEnemy = controlledEnemy;
	_controller = controller;
}

Startled::~Startled()
{
}

void Startled::Tick() {
	FRotator enemyRotation = _controlledEnemy->GetActorRotation();
	FVector enemyLocation = _controlledEnemy->GetActorLocation();
	FVector targetLocation = _controlledEnemy->_Target->GetActorLocation();

	FRotator getYaw = UKismetMathLibrary::FindLookAtRotation(enemyLocation, targetLocation);
	FRotator newRot = FRotator(0, getYaw.Yaw, 0);
	
	_controlledEnemy->K2_SetActorRotation(UKismetMathLibrary::RInterpTo(enemyRotation, newRot, _controlledEnemy->GetWorld()->GetDeltaSeconds(),5.0), false);
}

void Startled::OnEnter() {
	// Pop up !
	// Tell all other drones in range
	for (auto allyEnemy : _controlledEnemy->_EnemiesInRange)
	{
		if (!IsValid(allyEnemy->_Target))
		{
			allyEnemy->_Target = _controlledEnemy->_Target;
		}
	}
}

void Startled::OnExit() {
	// Hide !
}
