#pragma once
#include "ErectusMemory.h"
#include "settings.h"

class Looter
{
public:
	static void Loot();

	static void RequestLootItems() { lootItemsRequested = true; }

private:
	static bool ContainerValid(const TesItem& referenceData);
	static BYTE GetLegendaryRank(DWORD64 displayPtr);
	static bool ShouldLootJunk(const ItemInfo& item);
	static bool ShouldLootFloraLeveled(const LeveledList& list);
	static bool ShouldLootFlora(const ItemInfo& item);
	static bool ShouldLootItem(const ItemInfo& item, DWORD64 displayPtr);
	
	static void LootGroundItem(const ItemInfo& item, const TesObjectRefr& player);
	static void LootContainer(const ItemInfo& item, const TesObjectRefr& player);
	static void LootFlora(const ItemInfo& item, const TesObjectRefr& player);

	inline static bool lootItemsRequested = false;
};
