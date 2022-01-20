// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	MeshGun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh Gun"));
	MeshGun->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	

}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGun::PullTrigger()
{
	if (MuzzleFlashParticles)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleFlashParticles, MeshGun, TEXT("MuzzleFlashSocket"));
	}
	if (MuzzleSound)
	{
		UGameplayStatics::SpawnSoundAttached(MuzzleSound, MeshGun, TEXT("MuzzleSound"));
	}

	AController* OwnerController = GetOwnerController();
	if (!OwnerController)
	{
		return;
	}

	FHitResult HitResult;
	FVector ShotDirection;

	if (!GunTrace(HitResult, ShotDirection))
	{
		return;
	}

	if(ImpactParticles)
	{
		FRotator HitRotation = HitResult.ImpactNormal.Rotation();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, HitResult.Location, HitRotation);
	}
	if (ImpactSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ImpactSound, HitResult.Location);
	}

	AActor* HitActor = HitResult.GetActor();
	if (HitActor)
	{
		FPointDamageEvent DamageEvent(Damage, HitResult, -ShotDirection, nullptr);
		HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
	}

}

bool AGun::GunTrace(FHitResult& Hit, FVector& ShotDirection)
{
	AController* OwnerController = GetOwnerController();
	if (!OwnerController)
	{
		return false;
	}
		
	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location, Rotation);

	ShotDirection = Rotation.Vector();
	FVector End = Location + ShotDirection * MaxRange;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	return GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECC_GameTraceChannel1, Params);
}

AController* AGun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());

	if (!OwnerPawn)
	{
		return nullptr;
	}

	return OwnerPawn->GetController();
}