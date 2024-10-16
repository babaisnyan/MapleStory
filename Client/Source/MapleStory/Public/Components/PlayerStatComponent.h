#pragma once

#include "CoreMinimal.h"
#include "BasicStatComponent.h"
#include "game_struct.pb.h"
#include "Components/ActorComponent.h"
#include "PlayerStatComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MAPLESTORY_API UPlayerStatComponent : public UBasicStatComponent {
	GENERATED_BODY()

public:
	UPlayerStatComponent();

	void Setup(const protocol::PlayerInfo& Info);
	void Setup(const protocol::OtherPlayerInfo& Info);

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	bool bIsLocalPlayer = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 Mp = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 MaxMp = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 Str = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 Dex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 Int = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 Luk = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 Ap = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 Sp = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 Exp = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 Accuracy = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 Evasion = 0;
};
