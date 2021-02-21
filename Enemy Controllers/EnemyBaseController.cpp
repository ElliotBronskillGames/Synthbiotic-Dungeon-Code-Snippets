// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBaseController.h"

#include "DrawDebugHelpers.h"


void AEnemyBaseController::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyBaseController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/* Returns true if Target is in range and this drone can see it, false otherwise. */
bool AEnemyBaseController::InRangeAndHasLOS()
{
	bool inRange = _controlledEnemy->_PlayersInRange.Contains(_controlledEnemy->_Target);
	bool seesTarget = CanSeeTarget();
	if (inRange && seesTarget) return true;
	return false;
}

/* Returns true if the Target is out of range or this drone can't see it, false otherwise. */
bool AEnemyBaseController::OutOfRangeOrNoLOS()
{
	bool inRange = _controlledEnemy->_PlayersInRange.Contains(_controlledEnemy->_Target);
	if (!inRange) return true;
	
	bool seesTarget = CanSeeTarget();
	if (!seesTarget) return true;
	
	return false;
}

/* Returns true if the drone can see its Target. */
bool AEnemyBaseController::CanSeeTarget()
{
	FVector thisLocation = _controlledEnemy->GetTransform().GetLocation();
	
	if (_controlledEnemy->_SightPoints.Num() > 0)
	{
		int used = 0;
		for (auto sightPoint : _controlledEnemy->_SightPoints)
		{
			if (!sightPoint->ComponentHasTag("Raycast Start")) continue;
			used++;
			if (!RaycastToTarget(sightPoint->GetComponentLocation()))
			{
				return false;
			}
		}
	}
	else if (!RaycastToTarget(thisLocation))
	{
		return false;
	}
	return true;
}

bool AEnemyBaseController::RaycastToTarget(FVector start)
{
	FVector targetLocation = _controlledEnemy->_Target->GetTransform().GetLocation();

	// Raycast to the Target to get hit actors
	FVector rayEnd = FVector(targetLocation.X, targetLocation.Y, start.Z);
	TArray<FHitResult> hitResult;
	GetWorld()->LineTraceMultiByChannel(hitResult, start, rayEnd, ECC_Visibility);
	DrawDebugLine(GetWorld(), start, rayEnd, FColor::Green, false, 0.05, 0, 5);
	// Check all hit actors. If an obstacle is hit before the Target, this drone does not have LOS.
	AActor * hitActor;
	bool hitTargetBeforeObstacle = true;
	for (int iter = 0; iter < hitResult.Num(); iter++)
	{
		hitActor = hitResult[iter].GetActor();
		if (hitActor == nullptr) continue;
		if (hitActor == _controlledEnemy->_Target) break;
		if (hitActor->ActorHasTag(TEXT("Runner"))) continue; // Ignore other players.
		if (hitActor == _controlledEnemy) continue; // Ignore self.
		if (!hitActor->ActorHasTag(TEXT("Runner"))) // Any other actors that are not players.
		{
			hitTargetBeforeObstacle = false;
			break;
		}
	}
	return hitTargetBeforeObstacle;
}

float AEnemyBaseController::AngleToTarget()
{
	FVector forward =  _controlledEnemy->GetActorForwardVector();
	FVector xzForward = FVector(forward.X, forward.Y, 0);
	FVector vecToTarget = _controlledEnemy->_Target->GetActorLocation() - _controlledEnemy->GetActorLocation();
	FVector xzVecToTarget = FVector(vecToTarget.X, vecToTarget.Y, 0);
	xzForward.Normalize();
	xzVecToTarget.Normalize();
	float dot = UKismetMathLibrary::Dot_VectorVector(xzForward, xzVecToTarget);
	return  UKismetMathLibrary::DegAcos(dot);
}
