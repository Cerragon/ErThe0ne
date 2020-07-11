#pragma once
#include "ErectusMemory.h"
#include "settings.h"

class Looter
{
public:
	static void Loot();

	static void RequestLootItems() { lootItemsRequested = true; }
	static void RequestLootScrap() { lootScrapRequested = true; }

private:
	static bool CheckEntityLooterSettings(const EntityLooterSettings& settings);
	static bool CheckOnlyUseEntityLooterList(const EntityLooterSettings& settings);
	static bool HarvestFlora(const TesObjectRefr& entityData, const TesItem& referenceData, const TesObjectRefr& localPlayer);
	static bool LootItemScrap(const TesObjectRefr& entityData, const TesItem& referenceData, const TesObjectRefr& localPlayer);
	static bool LootItem(const TesObjectRefr& entityData, const TesItem& referenceData, const TesObjectRefr& localPlayer);
	static bool ContainerValid(const TesItem& referenceData);
	static bool LootEntity(const TesObjectRefr& entityData, const TesItem& referenceData, const TesObjectRefr& localPlayer, bool onlyUseEntityLooterList, bool useEntityLooterBlacklist);
	static bool CheckItemLooterList();
	static bool CheckItemLooterBlacklist();
	static bool CheckEntityLooterList(const EntityLooterSettings& settings);
	static bool CheckEntityLooterBlacklist(const EntityLooterSettings& settings);
	static bool CheckIngredientList();
	static bool CheckScrapList();
	static bool CheckItemLooterSettings();
	static bool CheckOnlyUseItemLooterList();
	static bool CheckEnabledItem(DWORD formId, DWORD64 entityFlag, int normalDistance);
	static bool TransferEntityItems(const TesObjectRefr& entityData, const TesItem& referenceData, const TesObjectRefr& localPlayer, bool onlyUseEntityLooterList, bool useEntityLooterBlacklist);
	static bool ValidLegendary(BYTE legendaryRank, DWORD64 entityFlag, const EntityLooterSettings& customEntityLooterSettings, bool legendaryWeaponsEnabled, bool legendaryArmorEnabled);
	static BYTE GetLegendaryRank(DWORD64 displayPtr);
	static bool IsLegendaryFormId(DWORD formId);
	static bool AllowLegendaryWeapons(const EntityLooterSettings& settings);
	static bool AllowLegendaryArmor(const EntityLooterSettings& settings);
	static bool EntityInventoryValid(const TesObjectRefr& entityData);
	static bool CheckEntityLooterItem(DWORD formId, DWORD64 entityFlag, const EntityLooterSettings& settings, bool legendaryWeaponsEnabled, bool legendaryArmorEnabled);
	
	inline static bool lootItemsRequested = false;
	inline static bool lootScrapRequested = false;
};
