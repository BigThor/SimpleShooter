// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class SIMPLESHOOTER_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PullTrigger();
	void Reload();

	int GetMaxAmmo();
	int GetCurrentAmmo();
	float GetReloadSeconds();

private:
	bool GunTrace(FHitResult &Hit, FVector &ShotDirection);
	AController* GetOwnerController() const;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* MeshGun;

	UPROPERTY(EditAnywhere, Category = "Components")
 	class UParticleSystem* MuzzleFlashParticles;

	UPROPERTY(EditAnywhere, Category = "Components")
	class USoundBase* MuzzleSound;

	UPROPERTY(EditAnywhere, Category = "Components")
	class UParticleSystem* ImpactParticles;

	UPROPERTY(EditAnywhere, Category = "Components")
	class USoundBase* ImpactSound;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float MaxRange = 2000.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float Damage = 10.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	int MaxAmmo = 16;
	int CurrentAmmo = 0;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float ReloadSeconds = 2.f;
};
