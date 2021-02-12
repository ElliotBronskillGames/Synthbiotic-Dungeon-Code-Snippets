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
	_chase = new Chase(CHASE, this, _controlledEnemy->Target);
	_attack = new Attack(ATTACK, this);
	
	// Setup statemachine
	_stateMachine->AddTransition(_idle, _chase,[this]()->bool{return _controlledEnemy->Target != nullptr;});
	_stateMachine->AddTransition(_chase, _attack, [this]()->bool
	{
		return InRangeAndHasLOS();
	});
	_stateMachine->AddTransition(_attack, _chase,[this]()->bool
	{
		return OutOfRangeOrNoLOS();
	});
	
	_stateMachine->AddAnyTransition(_idle, [this]()->bool
	{
		// Target is dead
		return _controlledEnemy->Target != nullptr;
	});
}

void ADroneController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	_stateMachine->Tick();
}

/* Returns true if Target is in range and this drone can see it, false otherwise. */
bool ADroneController::InRangeAndHasLOS()
{
	bool inRange = _controlledEnemy->PlayersInRange.Contains(_controlledEnemy->Target);
	bool seesTarget = CheckLOSToTarget();
	if (inRange && seesTarget) return true;
	return false;
}

/* Returns true if the Target is out of range or this drone can't see it, false otherwise. */
bool ADroneController::OutOfRangeOrNoLOS()
{
	bool inRange = _controlledEnemy->PlayersInRange.Contains(_controlledEnemy->Target);
	if (!inRange) return true;
	
	bool seesTarget = CheckLOSToTarget();
	if (!seesTarget) return true;
	
	return false;
}

/* Returns true if the drone can see its Target. */
bool ADroneController::CheckLOSToTarget()
{
	// Cache locations.
	FVector thisLocation = _controlledEnemy->GetTransform().GetLocation();
	FVector targetLocation = _controlledEnemy->Target->GetTransform().GetLocation();

	// Raycast to the Target to get hit actors
	FVector rayStart = thisLocation;
	FVector rayEnd = FVector(targetLocation.X, targetLocation.Y, thisLocation.Z);
	TArray<FHitResult> hitResult;
	GetWorld()->LineTraceMultiByChannel(hitResult, rayStart, rayEnd, ECC_Visibility);

	// Check all hit actors. If an obstacle is hit before the Target, this drone does not have LOS.
	AActor * hitActor;
	bool hitTargetBeforeObstacle = true;
	for (int iter = 0; iter < hitResult.Num(); iter++)
	{
		hitActor = hitResult[iter].GetActor();
		if (hitActor == nullptr) // Im assuming terrain pieces are not actors.
		{
			hitTargetBeforeObstacle = false;
			break;
		}
		if (hitActor == _controlledEnemy) break;
		if (hitActor->ActorHasTag(TEXT("Runner"))) continue; // Ignore other players.
		if (!hitActor->ActorHasTag(TEXT("Runner"))) // Any other actors that are not players.
		{
			hitTargetBeforeObstacle = false;
			break;
		}
	}
	return hitTargetBeforeObstacle;
}



