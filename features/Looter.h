#pragma once
#include <unordered_set>

class TesObjectRefr;
class TesItem;
class LeveledList;
struct ItemInfo;
class LocalPlayer;

class Looter final
{
public:
	static void Loot();

	static void RequestLootItems() { lootItemsRequested = true; }

	inline static std::unordered_set<std::uint32_t> lootedEntities = {};

private:
	static bool ProcessEntity(const TesObjectRefr& entity, const LocalPlayer& localPlayer);

	static bool ContainerValid(const TesItem& referenceData);
	static int GetLegendaryRank(std::uintptr_t displayPtr);
	static bool ShouldLootJunk(const ItemInfo& item);
	static bool ShouldLootFloraLeveled(const LeveledList& list);
	static bool ShouldLootFlora(const ItemInfo& item);
	static bool ShouldLootItem(const ItemInfo& item, std::uintptr_t displayPtr);

	static bool LootGroundItem(const ItemInfo& item, const LocalPlayer& player);
	static bool LootContainer(const ItemInfo& item, const LocalPlayer& player);
	static bool LootFlora(const ItemInfo& item, const LocalPlayer& player);

	inline static bool lootItemsRequested = false;
	inline static std::uint32_t lastPlayerCellFormId = 0;
	
	virtual void Dummy() = 0;
};
