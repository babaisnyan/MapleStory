// Fill out your copyright notice in the Description page of Project Settings.


#include "TestActor.h"

#include "PaperSprite.h"
#include "PaperSpriteComponent.h"
#include "Engine/Canvas.h"
#include "Engine/CanvasRenderTarget2D.h"
#include "Kismet/KismetRenderingLibrary.h"


// Sets default values
ATestActor::ATestActor() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent"));\
	PaperSprite = CreateDefaultSubobject<UPaperSprite>(TEXT("PaperSprite"));

	static ConstructorHelpers::FObjectFinder<UTexture2D> SpriteFinder(TEXT("/Game/Item/Texture/T_1000040_icon.T_1000040_icon"));
	if (SpriteFinder.Succeeded()) {
		Sprite = SpriteFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> Sprite2Finder(TEXT("/Game/Item/Texture/T_1001080_icon.T_1001080_icon"));
	if (Sprite2Finder.Succeeded()) {
		Sprite2 = Sprite2Finder.Object;
	}

	RootComponent = SpriteComponent;
}

// Called when the game starts or when spawned
void ATestActor::BeginPlay() {
	Super::BeginPlay();
	
	RenderTarget = UCanvasRenderTarget2D::CreateCanvasRenderTarget2D(GetWorld(), UCanvasRenderTarget2D::StaticClass(), 512, 512);
	RenderTarget->OnCanvasRenderTargetUpdate.AddDynamic(this, &ATestActor::OnRenderTargetUpdate);
}

// Called every frame
void ATestActor::Tick(const float DeltaTime) {
	Super::Tick(DeltaTime);

	if (RenderTarget) {
		RenderTarget->UpdateResource();
	}
}

void ATestActor::OnRenderTargetUpdate(UCanvas* Canvas, const int32 Width, const int32 Height) {
	if (Sprite && Sprite2) {
		Canvas->K2_DrawBox(FVector2D(0, 0), FVector2D(Width, Height), 10, FLinearColor::Blue);
		Canvas->K2_DrawTexture(Sprite, FVector2D(0, 0), FVector2D(Width, Height), FVector2D(0, 0), FVector2D(1, 1), FLinearColor::White, EBlendMode::BLEND_Translucent);
	}

	// Canvas to SpriteComponent

	if (SpriteComponent) {
		UTexture2D* Texture = UTexture2D::CreateTransient(Width, Height);
		UKismetRenderingLibrary::ConvertRenderTargetToTexture2DEditorOnly(GetWorld(), RenderTarget, Texture);

		
	}
}
