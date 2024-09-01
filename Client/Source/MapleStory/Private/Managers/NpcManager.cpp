#include "Managers/NpcManager.h"

UNpcManager::UNpcManager() {
	static ConstructorHelpers::FObjectFinder<UDataTable> NpcDataFinder(TEXT("/Script/Engine.DataTable'/Game/Data/DT_Npc.DT_Npc'"));

	if (NpcDataFinder.Succeeded()) {
		for (const auto& Row : NpcDataFinder.Object->GetRowNames()) {
			FNpcTemplate* Data = NpcDataFinder.Object->FindRow<FNpcTemplate>(Row, "");

			if (Data) {
				NpcData.Add(Data->Id, *Data);
			}
		}
	}
}

const FNpcTemplate* UNpcManager::GetNpcTemplate(const int32 Id) const {
	if (NpcData.Contains(Id)) {
		return &NpcData[Id];
	}

	return nullptr;
}
