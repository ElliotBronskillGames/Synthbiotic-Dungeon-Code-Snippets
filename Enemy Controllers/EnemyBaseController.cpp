// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBaseController.h"

#include "DrawDebugHelpers.h"

/* Returns true if Target is in range and this drone can see it, false otherwise. */
bool AEnemyBaseController::InRangeAndHasLOS()
{
	bool inRange = _controlledEnemy->_PlayersInRange.Contains(_controlledEnemy->_Target);
	bool seesTarget = CheckLOSToTarget();
	if (inRange && seesTarget) return true;
	return false;
}

/* Returns true if the Target is out of range or this drone can't see it, false otherwise. */
bool AEnemyBaseController::OutOfRangeOrNoLOS()
{
	bool inRange = _controlledEnemy->_PlayersInRange.Contains(_controlledEnemy->_Target);
	if (!inRange) return true;
	
	bool seesTarget = CheckLOSToTarget();
	if (!seesTarget) return true;
	
	return false;
}

/* Returns true if the drone can see its Target. */
bool AEnemyBaseController::CheckLOSToTarget()
{
	// Cache locations.
	FVector thisLocation = _controlledEnemy->GetTransform().GetLocation();
	FVector targetLocation = _controlledEnemy->_Target->GetTransform().GetLocation();

	// Raycast to the Target to get hit actors
	FVector rayStart = thisLocation;
	FVector rayEnd = FVector(targetLocation.X, targetLocation.Y, thisLocation.Z);
	TArray<FHitResult> hitResult;
	GetWorld()->LineTraceMultiByChannel(hitResult, rayStart, rayEnd, ECC_Visibility);
	DrawDebugLine(GetWorld(), rayStart, rayEnd, FColor::Green, false, 1, 0, 5);
	
	// Check all hit actors. If an obstacle is hit before the Target, this drone does not have LOS.
	AActor * hitActor;
	bool hitTargetBeforeObstacle = true;
	for (int iter = 0; iter < hitResult.Num(); iter++)
	{
		hitActor = hitResult[iter].GetActor();
		
		if (hitActor == nullptr) // Just in case.
			{
			continue;
			}
		if (hitActor == _controlledEnemy->_Target) break;
		if (hitActor->ActorHasTag(TEXT("Runner"))) continue; // Ignore other players.
		if (!hitActor->ActorHasTag(TEXT("Runner"))) // Any other actors that are not players.
			{
			hitTargetBeforeObstacle = false;
			break;
			}
	}
	return hitTargetBeforeObstacle;
}

void AEnemyBaseController::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyBaseController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}