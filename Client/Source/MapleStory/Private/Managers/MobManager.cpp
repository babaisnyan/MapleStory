#include "Managers/MobManager.h"

UMobManager::UMobManager() {
	static ConstructorHelpers::FObjectFinder<UDataTable> MobDataFinder(TEXT("/Script/Engine.DataTable'/Game/Data/DT_Mob.DT_Mob'"));

	if (MobDataFinder.Succeeded()) {
		for (const auto& Row : MobDataFinder.Object->GetRowNames()) {
			FMobTemplate* Data = MobDataFinder.Object->FindRow<FMobTemplate>(Row, "");

			if (Data) {
				MobData.Add(Data->MobId, *Data);
			}
		}
	}
}

FMobTemplate* UMobManager::GetMobTemplate(const int32 Id) {
	if (MobData.Contains(Id)) {
		return &MobData[Id];
	}

	return nullptr;
}
