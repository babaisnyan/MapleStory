#include "Managers/ItemManager.h"

#include "Data/Table/ItemTemplate.h"

UItemManager::UItemManager() {
	static ConstructorHelpers::FObjectFinder<UDataTable> ItemDataFinder(TEXT("/Game/Data/DT_Item.DT_Item"));

	if (ItemDataFinder.Succeeded()) {
		ItemData.Empty();
		
		for (const auto& Row : ItemDataFinder.Object->GetRowNames()) {
			const FItemTemplate* Data = ItemDataFinder.Object->FindRow<FItemTemplate>(Row, "");

			if (Data) {
				ItemData.Add(Data->ItemId, *Data);
			}
		}
	}
}

FItemTemplate* UItemManager::GetItemTemplate(const int32 Id) {
	if (ItemData.Contains(Id)) {
		return &ItemData[Id];
	}

	return nullptr;
}
