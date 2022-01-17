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

	APawn* PlayerPawn = Cast<APawn>(GetOwner());

	if (!PlayerPawn)
	{
		return;
	}

	AController* PlayerController = PlayerPawn->GetController();

	if (!PlayerController)
	{
		return;
	}

	FVector Location;
	FRotator Rotation;

	PlayerController->GetPlayerViewPoint(Location, Rotation);

	FVector End = Location + Rotation.Vector() * MaxRange;
	FHitResult HitResult;
	bool bObjectHit = GetWorld()->LineTraceSingleByChannel(HitResult, Location, End, ECC_GameTraceChannel1);

	if (!bObjectHit)
	{
		return;
	}

	if(ImpactParticles)
	{
		FRotator HitRotation = HitResult.ImpactNormal.Rotation();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, HitResult.Location, HitRotation);
	}

	AActor* HitActor = HitResult.GetActor();
	if (HitActor != nullptr)
	{
		FPointDamageEvent DamageEvent(Damage, HitResult, -Rotation.Vector(), nullptr);
		HitActor->TakeDamage(Damage, DamageEvent, PlayerController, this);
	}

}

