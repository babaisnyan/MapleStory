// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestActor.generated.h"

class UPaperSpriteComponent;
class UPaperSprite;
class UCanvasRenderTarget2D;

UCLASS()
class MAPLESTORY_API ATestActor : public AActor {
	GENERATED_BODY()

public:
	UFUNCTION()
	void OnRenderTargetUpdate(UCanvas* Canvas, int32 Width, int32 Height);
	// Sets default values for this actor's properties
	ATestActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCanvasRenderTarget2D> RenderTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Sprite;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Sprite2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UPaperSpriteComponent> SpriteComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UPaperSprite> PaperSprite;
};
