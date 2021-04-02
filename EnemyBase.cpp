// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "EnemyBaseController.h"
#include "Healthbar.h"
#include "Projectile.h"
#include "Components/ProgressBar.h"
#include "GameFramework/GameSession.h"
#include "Math/Axis.h"

// Sets default values
AEnemyBase::AEnemyBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SetReplicates(true);
	
	//Defaults
	_MaxHealth = 100;
	_Damage = 10;
	_Movespeed = 300;
	_SightRange = 1000;
	_AbilityRange = 100;
	_RotateSpeed = 5;
	_PossessedMoveSpeed = 150;
	static ConstructorHelpers::FObjectFinder<USoundBase> deathSound(TEXT("/Game/Audio/Sound_FX/Sentinel/Enemies/Droid_Explosion/Droid_Explosion.Droid_Explosion"));
	if (deathSound.Succeeded()) _DeathSound = deathSound.Object;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	bAlwaysRelevant = true;
	
	// Setup Components
	PlacementBound = CreateDefaultSubobject<UBoxComponent>(TEXT("PlacementBoundComponent"));
	PlacementBound->SetupAttachment(RootComponent);
	PlacementBound->SetCollisionResponseToAllChannels(ECR_Ignore);
	PlacementBound->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	PlacementBound->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	PlacementBound->SetWorldLocation(FVector(0, 0, 20));
	PlacementBound->SetWorldScale3D(FVector(1, 1, 0.25));
	PlacementBound->SetBoxExtent(FVector(150,150,60));
	PlacementBound->SetCollisionProfileName(TEXT("PlacementBounds"));
	
	PlayerAndEnemyDetector = CreateDefaultSubobject<USphereComponent>(TEXT("PlayerAndEnemyDetectorComponent"));
	PlayerAndEnemyDetector->SetupAttachment(RootComponent);
	PlayerAndEnemyDetector->SetCollisionResponseToAllChannels(ECR_Ignore);
	PlayerAndEnemyDetector->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	PlayerAndEnemyDetector->SetGenerateOverlapEvents(true);
	PlayerAndEnemyDetector->SetSphereRadius(_SightRange);

	HitDetector = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitDetectorComponent"));
	HitDetector->SetupAttachment(RootComponent);
	HitDetector->SetCollisionResponseToAllChannels(ECR_Ignore);
	HitDetector->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	HitDetector->SetWorldLocation(FVector(2, 0, 97));
	HitDetector->SetWorldRotation(FRotator(90, 90, 0));
	HitDetector->SetWorldScale3D(FVector(4.875, 1.6, 1.018));
	HitDetector->SetCapsuleRadius(6);
	HitDetector->SetCapsuleHalfHeight(50);
	HitDetector->SetNotifyRigidBodyCollision(true);
	HitDetector->SetGenerateOverlapEvents(false);

	CircleDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("CircleDecalComponent"));
	CircleDecal->SetupAttachment(RootComponent);
	CircleDecal->SetWorldLocation(FVector(0, 0, -11.25));
	CircleDecal->SetWorldRotation(FRotator(-90, 0, 0));
	CircleDecal->SetWorldScale3D(FVector(0.5, 0.5, 0.5));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> decalMat(TEXT("/Game/Artwork/Effects/PlayerHighlight/EnemyHighlight_DECAL.EnemyHighlight_DECAL"));
	if (decalMat.Succeeded()) CircleDecal->SetDecalMaterial(decalMat.Object);

	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLightComponent"));
	PointLight->SetupAttachment(RootComponent);
	PointLight->SetWorldLocation(FVector(0, 0, 30));
	PointLight->SetWorldScale3D(FVector(1, 1, 0.25));
	PointLight->SetIntensity(1500);
	PointLight->SetLightColor(FVector(255, 0, 0));
	PointLight->SetAttenuationRadius(200);
	PointLight->CastShadows = false;
	
	DroneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DroneMeshComponent"));
	DroneMesh->SetWorldLocation(FVector(0, 0, 37));
	DroneMesh->SetWorldRotation(FRotator(0, -90, 0));
	DroneMesh->SetWorldScale3D(FVector(0.3, 0.3, 0.3));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> droneDefaultMesh(TEXT("/Game/Artwork/Geometry/Meshes/Enemies/Gun_Drone/VAT_GunDrone/gun_drone_processed.gun_drone_processed"));
	if (droneDefaultMesh.Succeeded()) DroneMesh->SetStaticMesh(droneDefaultMesh.Object);
	DroneMesh->SetupAttachment(RootComponent);

	BaseSightPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("BaseSightPointComponent"));
	BaseSightPoint->SetupAttachment(RootComponent);
	BaseSightPoint->SetWorldLocation(FVector(0, 0, DroneMesh->GetRelativeLocation().Z));
	BaseSightPoint->ComponentTags.Add("Raycast Start");

	HealthbarWidget = ObjectInitializer.CreateDefaultSubobject<UWidgetComponent>( this, TEXT("HealthBarWidget"));
	HealthbarWidget->AttachToComponent( RootComponent, FAttachmentTransformRules::KeepRelativeTransform );
	HealthbarWidget->SetWorldLocation(FVector(0, 0, 150));
	HealthbarWidget->SetHiddenInGame(true);
	HealthbarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> healthbarWidget(TEXT("/Game/Blueprints/Sentinel/Traps-Enemies/Drones/EnemyHealthbarBP"));
	if (healthbarWidget.Succeeded()) HealthbarWidget->SetWidgetClass(healthbarWidget.Class);
	HealthbarWidget->SetDrawAtDesiredSize(true);

	
	FloatingTextWidget = ObjectInitializer.CreateDefaultSubobject<UWidgetComponent>( this, TEXT("FloatingTextWidget"));
	FloatingTextWidget->AttachToComponent( RootComponent, FAttachmentTransformRules::KeepRelativeTransform );
	FloatingTextWidget->SetWorldLocation(FVector(0, 0, 200));
	FloatingTextWidget->SetHiddenInGame(true);
	FloatingTextWidget->SetWidgetSpace(EWidgetSpace::Screen);
	
	static ConstructorHelpers::FClassFinder<UUserWidget> floatingTextWidget(TEXT("/Game/Blueprints/Sentinel/Traps-Enemies/Drones/FloatingTextBP"));
	if (floatingTextWidget.Succeeded()) FloatingTextWidget->SetWidgetClass(floatingTextWidget.Class);
	FloatingTextWidget->SetDrawAtDesiredSize(true);

}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	// Setup
	_Health = _MaxHealth;
	GetCharacterMovement()->MaxWalkSpeed = _Movespeed;
	GetComponents<UArrowComponent>(_SightPoints);
	if (PlayerAndEnemyDetector)
	{
		PlayerAndEnemyDetector->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBase::BeginOverlapDelegate);
		PlayerAndEnemyDetector->OnComponentEndOverlap.AddDynamic(this, &AEnemyBase::EndOverlapDelegate);
	}
	if (HitDetector)
	{
		HitDetector->OnComponentHit.AddDynamic(this, &AEnemyBase::HitDelegate);
	}
	if (HasAuthority())
	{
		PlacementBound->OnBeginCursorOver.AddDynamic(this, &AEnemyBase::CustomOnBeginMouseOver);
		PlacementBound->OnEndCursorOver.AddDynamic(this, &AEnemyBase::CustomOnEndMouseOver);
	}
	
	_healthbar = Cast<UHealthbar>(HealthbarWidget->GetUserWidgetObject());
	if (_healthbar)
	{
		_healthbar->SetOwner(this);
		if (HasAuthority())
		{
			_healthbar->_Healthbar->SetFillColorAndOpacity(FVector(0,255,0));
		}
	}
	if (!HasAuthority()) CircleDecal->SetHiddenInGame(true);

	// Get initial enemies in range
	TArray<AActor*> outActors;
	TArray<AActor*> ignoreActors;
	ignoreActors.Init(this, 1);
	UClass* seekClass = AEnemyBase::StaticClass();
	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), _SightRange, traceObjectTypes, seekClass, ignoreActors, outActors);
	for (auto actor : outActors)
	{
		AEnemyBase * enemy = Cast<AEnemyBase>(actor);
		if (enemy) _EnemiesInRange.Add(enemy);
	}
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	if (possessed) PossessedRotateAndMove();
	else FindTarget();
	
	if (HasAuthority())
	{
		if (_Health == _MaxHealth && HealthbarWidget) HealthbarWidget->SetHiddenInGame(true);
	}
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyBase::BeginOverlapDelegate(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr || OtherActor == this) return;
	
	ASynthbioticDungeonCCharacter* player = Cast<ASynthbioticDungeonCCharacter>(OtherActor);
	if (player && !player->IsDead)
	{
		_PlayersInRange.Add(player);
		SmartTargetPlayers(player);
		return;
	}
	AEnemyBase * enemy = Cast<AEnemyBase>(OtherActor);
	if (enemy)
	{
		_EnemiesInRange.Add(enemy);
		if (!IsValid(_Target)) return;
		// Make sure all enemies in range have a target
		NotifyOthersTargetFound();
	}
}

void AEnemyBase::EndOverlapDelegate(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == nullptr || OtherActor == this) return;
	
	ASynthbioticDungeonCCharacter* player = Cast<ASynthbioticDungeonCCharacter>(OtherActor);
	if (player)
	{
		_PlayersInRange.Remove(player);
		return;
	}
	AEnemyBase * enemy = Cast<AEnemyBase>(OtherActor);
	if (enemy)
	{
		_EnemiesInRange.Remove(enemy);
	}
}

void AEnemyBase::HitDelegate(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	// cast actor to projectile c++ class
	AProjectile * projectile = Cast<AProjectile>(OtherActor);
	if (projectile)
	{
		HealthbarWidget->SetHiddenInGame(false);
		Damaged(10, _DeathSound);
		if (!IsValid(_Target))
		{
			// Set target to the owner of bullet
			_Target = Cast<ASynthbioticDungeonCCharacter>(projectile->OwnerActor);
		}
	}
}

void AEnemyBase::CustomOnBeginMouseOver(UPrimitiveComponent* TouchedComponent)
{
	if (!possessed) FloatingTextWidget->SetHiddenInGame(false);
}

void AEnemyBase::CustomOnEndMouseOver(UPrimitiveComponent* TouchedComponent)
{
	FloatingTextWidget->SetHiddenInGame(true);
}

void AEnemyBase::PossessedRotateAndMove()
{
	const FVector worldDirection = UKismetMathLibrary::Conv_Vector2DToVector(GetMoveDirection());
	AddMovementInput(worldDirection, _PossessedMoveSpeed * GetWorld()->GetDeltaSeconds(), false);
	const FRotator aimRot = UKismetMathLibrary::MakeRotator(0, 0, GetAimRotation());
	SetActorRotation(aimRot, ETeleportType::None);
}

void AEnemyBase::Damaged(float damage, USoundBase * deathSound)
{
	_Health -= damage;
	HealthbarWidget->SetHiddenInGame(false);
	if (_Health <= 0)
	{
		UGameplayStatics::PlaySoundAtLocation(this, deathSound, this->GetActorLocation());
		UnPossess();
		if (HasAuthority()) Destroy();
	} 
}

void AEnemyBase::SmartTargetPlayers(ASynthbioticDungeonCCharacter* player)
{
	// If we cant see this player, cannot target them.
	AEnemyBaseController * controller = Cast<AEnemyBaseController>(GetController());
	if (controller == nullptr) return;
	if (!controller->CanSeeTarget(player)) return;

	// If this enemy is not the only enemy currently targeting its target,
	// target first player that isnt targeted by another enemy.
	bool targetOfOtherDrones = false;
	bool onlyDroneOnCurTarget = true;
	for (auto drone : _EnemiesInRange)
	{
		if (drone->_Target != nullptr && drone->_Target == player)
		{
			targetOfOtherDrones = true;
			break;
		}
	}
	for (auto drone : _EnemiesInRange)
	{
		if (!IsValid(_Target) || drone->_Target != nullptr && drone->_Target == _Target)
		{
			onlyDroneOnCurTarget = false;
			break;
		}
	}
	if (!targetOfOtherDrones && !onlyDroneOnCurTarget)
	{
		_Target = player;
		return;
	}
	
	// If this is the only enemy targeting this player, then dont switch.
	if (onlyDroneOnCurTarget) return;

	// If no target yet then target this player.
	if (!IsValid(_Target)) _Target = player;
	
	// If this enemy is not the only one targeting its target, and there are no untargeted players,
	// have a chance to switch Targets
	if (player == _Target) return;
	int randNum = rand() % 100 + 1;
	int chanceToSwitch = 30;
	if (randNum > chanceToSwitch && controller && controller->CanSeeTarget(player))
	{
		_Target = player;
	}
}

void AEnemyBase::FindTarget()
{
	AEnemyBaseController * controller = Cast<AEnemyBaseController>(GetController());
	if (controller == nullptr) return;
	
	if (!IsValid(_Target))
	{
		for (auto player : _PlayersInRange) // O(1) runtime. Always 3 or less players.
		{
			if (controller && controller->CanSeeTarget(player))
			{
				_Target = player;
				NotifyOthersTargetFound();
				break;
			}
		}
	}
	else if (_Target->IsDead)
	{
		_PlayersInRange.Remove(_Target);
		_Target = nullptr;
	}

	// Internal switch cooldown.
	if (controller->GetWorld()->DeltaTimeSeconds < _switchTimer) return;

	for (auto player : _PlayersInRange)
	{
		SmartTargetPlayers(player);
	}
	
	// Reset cooldown
	_switchTimer = controller->GetWorld()->DeltaTimeSeconds + _switchCooldown;
}

void AEnemyBase::NotifyOthersTargetFound()
{
	for (auto enemy : _EnemiesInRange)
	{
		if (!IsValid(enemy->_Target))
			enemy->_Target = _Target;
	}
}

void AEnemyBase::RotateTowardsTarget(AActor * target)
{
	// Face target
	FRotator enemyRotation = GetActorRotation();
	FVector enemyLocation = GetActorLocation();
	FVector targetLocation = target->GetActorLocation();

	FRotator getYaw = UKismetMathLibrary::FindLookAtRotation(enemyLocation, targetLocation);
	FRotator newRot = FRotator(0, getYaw.Yaw, 0);
	
	K2_SetActorRotation(UKismetMathLibrary::RInterpTo(enemyRotation, newRot, GetWorld()->GetDeltaSeconds(), _RotateSpeed), false);
}

void AEnemyBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEnemyBase, _Health);
	DOREPLIFETIME(AEnemyBase, bStartled);
}

