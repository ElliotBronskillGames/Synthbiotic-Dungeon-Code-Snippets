// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AITrapEnemy.h"
#include "SynthbioticDungeonCCharacter.h"
#include "GameFramework/Character.h"
#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/DecalComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "EnemyBase.generated.h"

UCLASS()
class SYNTHBIOTICDUNGEONC_API AEnemyBase : public AAITrapEnemy
{
	GENERATED_BODY()

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Sets default values for this character's properties
	AEnemyBase(const FObjectInitializer& ObjectInitializer);

	// Properties
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy Properties")
		float _MaxHealth;
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Enemy Properties")
		float _Health;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy Properties")
		float _Damage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy Properties")
		float _Movespeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy Properties")
		float _SightRange;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy Properties")
		float _AbilityRange;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy Properties")
		float _RotateSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy Properties")
		float _PossessedMoveSpeed;
	UPROPERTY(BlueprintReadOnly, Category = "Enemy Properties")
		USoundBase* _DeathSound;
	UPROPERTY(BlueprintReadOnly, Category = "Enemy Properties")
		TArray<ASynthbioticDungeonCCharacter*> _PlayersInRange;
	UPROPERTY(BlueprintReadOnly, Category = "Enemy Properties")
		TSet<AEnemyBase*> _EnemiesInRange;
	UPROPERTY(BlueprintReadOnly, Category = "Enemy Properties")
		TArray<UArrowComponent*> _SightPoints; // Requires UArrowComponents to have tag "Raycast Start"
	UPROPERTY(BlueprintReadWrite, Category = "Enemy Properties")
		ASynthbioticDungeonCCharacter * _Target; // Set by the enemy Character
	UPROPERTY(Replicated, BlueprintReadOnly)
		bool bStartled = false;
	UPROPERTY()
		class UHealthbar * _healthbar;

	// Rotates this enemy towards the input target actor
	void RotateTowardsTarget(AActor* target);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Components
	UPROPERTY(VisibleAnywhere, Category = "Detection", SkipSerialization)
		class USphereComponent * PlayerAndEnemyDetector;
	UPROPERTY(VisibleAnywhere, Category = "Detection")
		UBoxComponent * PlacementBound;
	UPROPERTY(VisibleAnywhere, Category = "Detection")
		UBoxComponent * TextTriggerBox;
	UPROPERTY(VisibleAnywhere, Category = "Detection")
		UCapsuleComponent * HitDetector;
	UPROPERTY(VisibleAnywhere, Category = "LOS")
		UArrowComponent * BaseSightPoint;
	UPROPERTY( VisibleAnywhere, Category = "Healthbar", SkipSerialization)
		class UWidgetComponent* HealthbarWidget;
	UPROPERTY( VisibleAnywhere, Category = "Aesthetics")
		UDecalComponent* CircleDecal;
	UPROPERTY( VisibleAnywhere, Category = "Aesthetics")
		UPointLightComponent* PointLight;
	UPROPERTY( VisibleAnywhere, Category = "Aesthetics")
		UStaticMeshComponent* DroneMesh;
	UPROPERTY( VisibleAnywhere, Category = "UI", SkipSerialization)
		class UWidgetComponent* FloatingTextWidget;

	float _switchCooldown = 5.0;
	float _switchTimer = 0.0;

	// Delegates for Overlap Events
	UFUNCTION()
		void BeginOverlapDelegate(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void EndOverlapDelegate(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
		void HitDelegate(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
		void CustomOnBeginMouseOver(UPrimitiveComponent* TouchedComponent);
	UFUNCTION()
        void CustomOnEndMouseOver(UPrimitiveComponent* TouchedComponent);

	// Called when this enemy has taken damage
	UFUNCTION(BlueprintCallable, Category = "Healthbar")
		void Damaged(float damage, USoundBase * deathSound);
	// Handles the input for when the sentinel possesses this enemy
	void PossessedRotateAndMove();
	// Targets players based off of many factors. See declaration for all
	void SmartTargetPlayers(ASynthbioticDungeonCCharacter * player);
	// Checks for any new players within this enemy's sight range
	virtual void FindTarget();
	// Sets all other in-range drones to also target this drone's target
	void NotifyOthersTargetFound();
};
