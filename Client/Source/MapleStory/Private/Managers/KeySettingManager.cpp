#include "Managers/KeySettingManager.h"

void UKeySettingManager::Deinitialize() {
	Super::Deinitialize();
	Clear();
}

void UKeySettingManager::Clear() {
	Keys.Empty();
}

void UKeySettingManager::Set(const EKeyCode KeyCode, const protocol::KeySetting& KeySetting) {
	Keys.Add(KeyCode, MakeShared<protocol::KeySetting>(KeySetting));
}

TSharedPtr<protocol::KeySetting> UKeySettingManager::Get(const EKeyCode KeyCode) const {
	return Keys.FindRef(KeyCode);
}
