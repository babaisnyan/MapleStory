#include "Managers/KeySettingManager.h"

void UKeySettingManager::Deinitialize() {
	Super::Deinitialize();
	Keys.Empty();
}

void UKeySettingManager::Set(const EKeyCode KeyCode, const protocol::KeySetting& KeySetting) {
	Keys.Add(KeyCode, MakeShared<protocol::KeySetting>(KeySetting));
}
