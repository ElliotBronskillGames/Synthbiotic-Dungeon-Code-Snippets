// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "GameFramework/Character.h"
#include "HealingDrone.generated.h"

UCLASS()
class SYNTHBIOTICDUNGEONC_API AHealingDrone : public AEnemyBase
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Set properties to be replicated
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// Sets default values for this character's properties
	AHealingDrone(const FObjectInitializer& ObjectInitializer);

	// Properties
	UPROPERTY(Replicated, VisibleDefaultsOnly, BlueprintReadOnly, Category = "Healing Drone Properties")
		AEnemyBase * _HealingTarget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Healing Drone Properties")
		float _HealingCooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Healing Drone Properties")
		float _HealAmount;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Healing Drone Properties")
		bool bShowHealBeam;

	// Heal the current _HealingTarget and this drone for the specified _HealAmount every _HealCooldown seconds.
	UFUNCTION(BlueprintCallable, Category = "Healing")
        void HealTargetAndSelf();
	// Overriden function to find this healing drones _HealingTarget
	virtual void FindTarget() override;
};
