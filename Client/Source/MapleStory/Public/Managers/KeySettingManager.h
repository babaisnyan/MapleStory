#pragma once

#include "CoreMinimal.h"
#include "game_struct.pb.h"
#include "Data/Enum/EKeyCode.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "KeySettingManager.generated.h"

class UQuickSlotKeyWidget;

UCLASS()
class MAPLESTORY_API UKeySettingManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Deinitialize() override;
	void Clear();
	void Set(EKeyCode KeyCode, const protocol::KeySetting& KeySetting);
	TSharedPtr<protocol::KeySetting> Get(EKeyCode KeyCode) const;
	
private:
	TMap<EKeyCode, TSharedPtr<protocol::KeySetting>> Keys;
};
