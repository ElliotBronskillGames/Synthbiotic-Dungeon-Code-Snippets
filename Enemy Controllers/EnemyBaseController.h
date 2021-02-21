// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyBase.h"
#include "StateMachine.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnemyBaseController.generated.h"

/**
 * 
 */
UCLASS()
class SYNTHBIOTICDUNGEONC_API AEnemyBaseController : public AAIController
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category="Controller Variables") AEnemyBase * _controlledEnemy;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Controller Variables") TMap<FString, UBehaviorTree*> _behaviorTrees;
protected:
	~AEnemyBaseController() { delete _stateMachine; };
	StateMachine * _stateMachine = new StateMachine();
	bool InRangeAndHasLOS();
	bool OutOfRangeOrNoLOS();
	bool CanSeeTarget();
	float AngleToTarget();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
private:
	bool RaycastToTarget(FVector start);
};
