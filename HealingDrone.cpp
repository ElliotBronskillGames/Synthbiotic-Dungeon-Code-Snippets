// Fill out your copyright notice in the Description page of Project Settings.


#include "HealingDrone.h"

#include "Healthbar.h"

// Sets default values
AHealingDrone::AHealingDrone(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AHealingDrone::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AHealingDrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AHealingDrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AHealingDrone::HealTargetAndSelf()
{
	// Heal target
	_HealingTarget->_Health = FMath::Clamp<float>(_HealingTarget->_Health + _HealAmount, 0.0, _HealingTarget->_MaxHealth);
	
	// Heal self
	_Health = FMath::Clamp<float>(_Health + _HealAmount, 0.0, _MaxHealth);
}

void AHealingDrone::FindTarget()
{
	float timeSeconds =  GetWorld()->GetTimeSeconds();
	// Only switch targets if timer is up and your cur target is not full health
	if (_switchTimer > timeSeconds && _HealingTarget && _HealingTarget->_Health <= _HealingTarget->_MaxHealth) return;
	
	AEnemyBase * lowestEnemy = nullptr;
	float lowestHealthSoFar = 1000000.0;
	for (auto enemy : _EnemiesInRange)
	{
		if (enemy == this) continue;
		AHealingDrone * hdrone = Cast<AHealingDrone>(enemy);
		if (hdrone && hdrone->_HealingTarget == this) continue;
		if (enemy->_Health < lowestHealthSoFar)
		{
			lowestEnemy = enemy;
			lowestHealthSoFar = enemy->_Health;
		}
	}
	_HealingTarget = lowestEnemy;
	_switchTimer = _switchCooldown + timeSeconds;
}

void AHealingDrone::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AHealingDrone, _HealingTarget);
	DOREPLIFETIME(AHealingDrone, bShowHealBeam);
}

