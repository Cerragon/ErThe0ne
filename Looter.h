#pragma once
#include "ErectusMemory.h"
#include "LocalPlayer.h"
#include "settings.h"

class Looter final
{
public:
	static void Loot();

	static void RequestLootItems() { lootItemsRequested = true; }

	inline static std::unordered_set<DWORD> lootedEntities = {};

private:
	static bool ProcessEntity(const TesObjectRefr& entity, const LocalPlayer& localPlayer);

	static bool ContainerValid(const TesItem& referenceData);
	static BYTE GetLegendaryRank(DWORD64 displayPtr);
	static bool ShouldLootJunk(const ItemInfo& item);
	static bool ShouldLootFloraLeveled(const LeveledList& list);
	static bool ShouldLootFlora(const ItemInfo& item);
	static bool ShouldLootItem(const ItemInfo& item, DWORD64 displayPtr);

	static bool LootGroundItem(const ItemInfo& item, const LocalPlayer& player);
	static bool LootContainer(const ItemInfo& item, const LocalPlayer& player);
	static bool LootFlora(const ItemInfo& item, const LocalPlayer& player);

	inline static bool lootItemsRequested = false;
	inline static DWORD lastPlayerCellFormId = 0;
	
	virtual void Dummy() = 0;
};
