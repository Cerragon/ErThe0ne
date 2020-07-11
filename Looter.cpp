#include "Looter.h"

#include "common.h"
#include "ErectusProcess.h"
#include "MsgSender.h"
#include "settings.h"
#include "utils.h"

namespace
{
	DWORD legendaryFormIdArray[]
	{
		0x00425E28, 0x004392CD, 0x0037F7D9, 0x001A7B80, 0x001A7AF6, 0x001A7BE2, 0x001A7BD3, 0x001A7AB2, 0x001A7B88,
		0x001A7BDA, 0x001A7C39, 0x0052BDC7, 0x0052BDC5, 0x0052BDC2, 0x0052BDC8, 0x0052BDB4, 0x0052BDB5, 0x0052BDB6,
		0x0052BDB7, 0x0052BDBA, 0x0052BDBC, 0x0052BDBF, 0x005299F5, 0x005299ED, 0x00529A14, 0x005299FE, 0x00529A0F,
		0x00529A0C, 0x00529A09, 0x005299F9, 0x005299FA, 0x005299FC, 0x00529A05, 0x00529A04, 0x005299FB, 0x00529A03,
		0x005299FD, 0x00529A02, 0x005281B8, 0x005281B4, 0x00527F6F, 0x00527F72, 0x00527F6E, 0x00527F7D, 0x00527F75,
		0x00527F6C, 0x00527F6D, 0x00527F74, 0x00527F84, 0x00527F82, 0x00527F8B, 0x00527F81, 0x00527F78, 0x00527F76,
		0x00527F7F, 0x00527F77, 0x00527F79, 0x00527F7A, 0x00527F7B, 0x00525400, 0x00525401, 0x005253FB, 0x0052414C,
		0x00524143, 0x0052414E, 0x0052414F, 0x00524150, 0x00524152, 0x00524153, 0x00524154, 0x00524146, 0x00524147,
		0x0052414A, 0x0052414B, 0x00521914, 0x00521915, 0x004F6D77, 0x004F6D7C, 0x004F6D86, 0x004F6D76, 0x004F6D85,
		0x004F6D84, 0x004F6D82, 0x004F6D83, 0x004F6D81, 0x004F6D80, 0x004F6D7F, 0x004F6D78, 0x004F6D7E, 0x004F6D7D,
		0x004F6AAE, 0x004F6AAB, 0x004F6AA1, 0x004F6AA0, 0x004F6AA7, 0x004F6AA5, 0x004F6AB1, 0x004F5772, 0x004F5778,
		0x004F5770, 0x004F5773, 0x004F577C, 0x004F5771, 0x004F5777, 0x004F5776, 0x004F577D, 0x004F577B, 0x004F577A,
		0x004F5779, 0x004EE548, 0x004EE54B, 0x004EE54C, 0x004EE54E, 0x004ED02B, 0x004ED02E, 0x004ED02C, 0x004ED02F,
		0x004E89B3, 0x004E89B2, 0x004E89AC, 0x004E89B4, 0x004E89B0, 0x004E89AF, 0x004E89AE, 0x004E89B6, 0x004E89AD,
		0x004E89B5, 0x003C4E27, 0x003C3458, 0x00357FBF, 0x001142A8, 0x0011410E, 0x0011410D, 0x0011410C, 0x0011410B,
		0x0011410A, 0x00114109, 0x00114108, 0x00114107, 0x00114106, 0x00114105, 0x00114104, 0x00114103, 0x00114101,
		0x001140FF, 0x001140FD, 0x001140FC, 0x001140FB, 0x001140FA, 0x001140F8, 0x001140F2, 0x001140F1, 0x001140F0,
		0x001140EF, 0x001140EE, 0x001140ED, 0x001140EC, 0x001140EB, 0x001140EA, 0x00113FC0, 0x001138DD, 0x0011384A,
		0x0011374F, 0x0011371F, 0x0010F599, 0x0010F598, 0x0010F596, 0x00226436, 0x001F81EB, 0x001F7A75, 0x001F1E47,
		0x001F1E0C, 0x001F1E0B, 0x001E73BD,
	};
}

void Looter::Loot()
{
	if (!MsgSender::IsEnabled())
		return;

	auto lootNpcs = Settings::npcLooter.enabled && CheckEntityLooterSettings(Settings::npcLooter);
	auto lootContainers = Settings::containerLooter.enabled && CheckEntityLooterSettings(Settings::containerLooter);
	auto lootFlora = Settings::harvester.enabled && CheckIngredientList();
	auto lootItems = (Settings::itemLooter.autoLootingEnabled || lootItemsRequested) && CheckItemLooterSettings();
	auto lootScrap = (Settings::scrapLooter.autoLootingEnabled || lootScrapRequested) && CheckScrapList();

	if (!lootNpcs && !lootContainers && !lootFlora && !lootItems && !lootScrap)
		return;

	auto localPlayerPtr = ErectusMemory::GetLocalPlayerPtr(true);
	if (!Utils::Valid(localPlayerPtr))
		return;

	TesObjectRefr localPlayer{};
	if (!ErectusProcess::Rpm(localPlayerPtr, &localPlayer, sizeof localPlayer))
		return;

	auto onlyUseNpcLooterList = false;
	auto useNpcLooterBlacklist = false;
	if (lootNpcs)
	{
		onlyUseNpcLooterList = CheckOnlyUseEntityLooterList(Settings::npcLooter);
		useNpcLooterBlacklist = CheckEntityLooterBlacklist(Settings::npcLooter);
	}

	auto onlyUseContainerLooterList = false;
	auto useContainerLooterBlacklist = false;
	if (lootContainers)
	{
		onlyUseContainerLooterList = CheckOnlyUseEntityLooterList(Settings::containerLooter);
		useContainerLooterBlacklist = CheckEntityLooterBlacklist(Settings::containerLooter);
	}

	auto entityPtrs = ErectusMemory::GetEntityPtrList();

	for (const auto& entityPtr : entityPtrs)
	{
		if (!Utils::Valid(entityPtr) || entityPtr == localPlayerPtr)
			continue;

		TesObjectRefr entityData{};
		if (!ErectusProcess::Rpm(entityPtr, &entityData, sizeof entityData))
			continue;

		if (!Utils::Valid(entityData.referencedItemPtr))
			continue;
		if (entityData.formType == static_cast<BYTE>(FormTypes::PlayerCharacter)) //players
			continue;
		if (entityData.formType == static_cast<BYTE>(FormTypes::TesObjectRefr) && !lootContainers && !lootFlora) //items
			continue;
		if (entityData.formType == static_cast<BYTE>(FormTypes::TesActor) && (!lootNpcs || ErectusMemory::CheckHealthFlag(entityData.healthFlag) != 0x3)) //npcs (dead)
			continue;
		if (entityData.spawnFlag != 0x02)
			continue;

		TesItem referenceData{};
		if (!ErectusProcess::Rpm(entityData.referencedItemPtr, &referenceData, sizeof referenceData))
			continue;

		if (lootNpcs && referenceData.formType == static_cast<BYTE>(FormTypes::TesNpc))
			LootEntity(entityData, referenceData, localPlayer, onlyUseNpcLooterList, useNpcLooterBlacklist);
		else if (lootContainers && referenceData.formType == static_cast<byte>(FormTypes::TesObjectCont))
			LootEntity(entityData, referenceData, localPlayer, onlyUseContainerLooterList, useContainerLooterBlacklist);
		else if (lootFlora && referenceData.formType == static_cast<byte>(FormTypes::TesFlora))
			HarvestFlora(entityData, referenceData, localPlayer);
		else
		{
			if (lootItems)
			{
				if (LootItem(entityData, referenceData, localPlayer))
					continue;
			}

			if (lootScrap)
				LootItemScrap(entityData, referenceData, localPlayer);
		}
	}

	lootItemsRequested = false;
	lootScrapRequested = false;
}

bool Looter::CheckEntityLooterSettings(const EntityLooterSettings& settings)
{
	if (settings.allWeaponsEnabled)
		return true;
	if (settings.allArmorEnabled)
		return true;
	if (settings.oneStarWeaponsEnabled)
		return true;
	if (settings.oneStarArmorEnabled)
		return true;
	if (settings.twoStarWeaponsEnabled)
		return true;
	if (settings.twoStarArmorEnabled)
		return true;
	if (settings.threeStarWeaponsEnabled)
		return true;
	if (settings.threeStarArmorEnabled)
		return true;
	if (settings.ammoEnabled)
		return true;
	if (settings.modsEnabled)
		return true;
	if (settings.capsEnabled)
		return true;
	if (settings.junkEnabled)
		return true;
	if (settings.aidEnabled)
		return true;
	if (settings.treasureMapsEnabled)
		return true;
	if (settings.knownPlansEnabled)
		return true;
	if (settings.unknownPlansEnabled)
		return true;
	if (settings.miscEnabled)
		return true;
	if (settings.unlistedEnabled)
		return true;
	if (settings.listEnabled)
		return CheckEntityLooterList(settings);

	return false;
}

bool Looter::CheckOnlyUseEntityLooterList(const EntityLooterSettings& settings)
{
	if (settings.allWeaponsEnabled)
		return false;
	if (settings.allArmorEnabled)
		return false;
	if (settings.oneStarWeaponsEnabled)
		return false;
	if (settings.oneStarArmorEnabled)
		return false;
	if (settings.twoStarWeaponsEnabled)
		return false;
	if (settings.twoStarArmorEnabled)
		return false;
	if (settings.threeStarWeaponsEnabled)
		return false;
	if (settings.threeStarArmorEnabled)
		return false;
	if (settings.ammoEnabled)
		return false;
	if (settings.modsEnabled)
		return false;
	if (settings.capsEnabled)
		return false;
	if (settings.junkEnabled)
		return false;
	if (settings.aidEnabled)
		return false;
	if (settings.treasureMapsEnabled)
		return false;
	if (settings.knownPlansEnabled)
		return false;
	if (settings.unknownPlansEnabled)
		return false;
	if (settings.miscEnabled)
		return false;
	if (settings.unlistedEnabled)
		return false;
	if (settings.listEnabled)
		return CheckEntityLooterList(settings);

	return false;
}

bool Looter::HarvestFlora(const TesObjectRefr& entityData, const TesItem& referenceData, const TesObjectRefr& localPlayer)
{
	if (!MsgSender::IsEnabled())
		return false;

	if (ErectusMemory::IsFloraHarvested(entityData.harvestFlagA, entityData.harvestFlagB))
		return false;

	auto normalDistance = static_cast<int>(Utils::GetDistance(entityData.position, localPlayer.position) * 0.01f);
	if (normalDistance > 6)
		return false;

	if (!ErectusMemory::FloraValid(referenceData))
		return false;

	RequestActivateRefMessage requestActivateRefMessageData{
		.vtable = ErectusProcess::exe + VTABLE_REQUESTACTIVATEREFMSG,
		.formId = entityData.formId,
		.choice = 0xFF,
		.forceActivate = 0
	};

	return MsgSender::Send(&requestActivateRefMessageData, sizeof requestActivateRefMessageData);
}

bool Looter::LootItemScrap(const TesObjectRefr& entityData, const TesItem& referenceData, const TesObjectRefr& localPlayer)
{
	auto entityFlag = CUSTOM_ENTRY_DEFAULT;
	DWORD64 entityNamePtr = 0;
	auto enabledDistance = 0;

	ErectusMemory::GetCustomEntityData(referenceData, &entityFlag, &entityNamePtr, &enabledDistance, true, false);
	if (!(entityFlag & CUSTOM_ENTRY_VALID_SCRAP))
		return false;

	auto distance = Utils::GetDistance(entityData.position, localPlayer.position);
	auto normalDistance = static_cast<int>(distance * 0.01f);

	if (normalDistance > Settings::scrapLooter.maxDistance)
		return false;

	RequestActivateRefMessage requestActivateRefMessageData{
		.vtable = ErectusProcess::exe + VTABLE_REQUESTACTIVATEREFMSG,
		.formId = entityData.formId,
		.choice = 0xFF,
		.forceActivate = 0
	};
	MsgSender::Send(&requestActivateRefMessageData, sizeof requestActivateRefMessageData);
	return true;
}

bool Looter::LootItem(const TesObjectRefr& entityData, const TesItem& referenceData, const TesObjectRefr& localPlayer)
{
	if (!MsgSender::IsEnabled())
		return false;

	auto onlyUseItemLooterList = CheckOnlyUseItemLooterList();
	auto useItemLooterBlacklist = CheckItemLooterBlacklist();

	if (useItemLooterBlacklist)
	{
		if (ErectusMemory::CheckFormIdArray(referenceData.formId, Settings::itemLooter.blacklistEnabled, Settings::itemLooter.blacklist, 64))
			return false;
	}

	if (onlyUseItemLooterList)
	{
		if (!ErectusMemory::CheckFormIdArray(referenceData.formId, Settings::itemLooter.enabledList, Settings::itemLooter.formIdList, 100))
			return false;
	}

	auto entityFlag = CUSTOM_ENTRY_DEFAULT;
	DWORD64 entityNamePtr = 0;
	auto enabledDistance = 0;

	ErectusMemory::GetCustomEntityData(referenceData, &entityFlag, &entityNamePtr, &enabledDistance, false, false);
	if (!(entityFlag & CUSTOM_ENTRY_VALID_ITEM))
		return false;
	if (entityFlag & CUSTOM_ENTRY_JUNK)
		return false;

	auto distance = Utils::GetDistance(entityData.position, localPlayer.position);
	auto normalDistance = static_cast<int>(distance * 0.01f);

	if (!onlyUseItemLooterList)
	{
		if (!CheckEnabledItem(referenceData.formId, entityFlag, normalDistance))
			return false;
	}
	else
	{
		if (normalDistance > Settings::itemLooter.lootListDistance)
			return false;
	}

	RequestActivateRefMessage requestActivateRefMessageData{
		.vtable = ErectusProcess::exe + VTABLE_REQUESTACTIVATEREFMSG,
		.formId = entityData.formId,
		.choice = 0xFF,
		.forceActivate = 0
	};
	MsgSender::Send(&requestActivateRefMessageData, sizeof requestActivateRefMessageData);

	return true;
}

bool Looter::ContainerValid(const TesItem& referenceData)
{
	if (!Utils::Valid(referenceData.keywordArrayData00C0))
		return false;

	int nifTextLength;
	if (!ErectusProcess::Rpm(referenceData.keywordArrayData00C0 + 0x10, &nifTextLength, sizeof nifTextLength))
		return false;
	if (nifTextLength == 41)
	{
		char containerMarkerCheck[sizeof"ContainerMarker"];
		if (!ErectusProcess::Rpm(referenceData.keywordArrayData00C0 + 0x2E, &containerMarkerCheck, sizeof containerMarkerCheck))
			return false;

		containerMarkerCheck[15] = '\0';
		if (!strcmp(containerMarkerCheck, "ContainerMarker"))
			return false;
	}

	if (!Utils::Valid(referenceData.namePtr00B0))
		return false;

	DWORD64 nameBuffer;
	if (!ErectusProcess::Rpm(referenceData.namePtr00B0 + 0x10, &nameBuffer, sizeof nameBuffer))
		return false;
	if (!nameBuffer)
		return false;

	if (!Utils::Valid(nameBuffer))
		nameBuffer = referenceData.namePtr00B0;

	int nameTextLength;
	if (!ErectusProcess::Rpm(nameBuffer + 0x10, &nameTextLength, sizeof nameTextLength))
		return false;
	if (!nameTextLength || nameTextLength > 0x7FFF)
		return false;

	return true;
}

bool Looter::LootEntity(const TesObjectRefr& entityData, const TesItem& referenceData, const TesObjectRefr& localPlayer,
	const bool onlyUseEntityLooterList, const bool useEntityLooterBlacklist)
{
	auto isEntityNpc = false;
	auto isEntityContainer = false;

	float maxDistance;
	switch (referenceData.formType)
	{
	case (static_cast<BYTE>(FormTypes::TesNpc)):
		isEntityNpc = true;
		maxDistance = 76.f;
		break;
	case (static_cast<BYTE>(FormTypes::TesObjectCont)):
		isEntityContainer = true;
		maxDistance = 6.f;
		break;
	default:
		return false;
	}

	if (isEntityNpc && (referenceData.formId == 0x00000007 || ErectusMemory::CheckHealthFlag(entityData.healthFlag) != 0x3))
		return false;

	if (Utils::GetDistance(entityData.position, localPlayer.position) * 0.01f > maxDistance)
		return false;

	if (isEntityContainer && !ContainerValid(referenceData))
		return false;

	if (!EntityInventoryValid(entityData))
		return false;

	return TransferEntityItems(entityData, referenceData, localPlayer, onlyUseEntityLooterList, useEntityLooterBlacklist);
}

bool Looter::CheckItemLooterList()
{
	for (auto i = 0; i < 100; i++)
	{
		if (Settings::itemLooter.formIdList[i] && Settings::itemLooter.enabledList[i])
			return true;
	}

	return false;
}

bool Looter::CheckItemLooterBlacklist()
{
	if (Settings::itemLooter.blacklistToggle)
	{
		for (auto i = 0; i < 64; i++)
		{
			if (Settings::itemLooter.blacklist[i] && Settings::itemLooter.blacklistEnabled[i])
				return true;
		}
	}

	return false;
}

bool Looter::CheckEntityLooterList(const EntityLooterSettings& settings)
{
	for (auto i = 0; i < 100; i++)
	{
		if (settings.formIdList[i] && settings.enabledList[i])
			return true;
	}

	return false;
}

bool Looter::CheckEntityLooterBlacklist(const EntityLooterSettings& settings)
{
	if (!settings.blacklistToggle)
		return false;

	for (auto i = 0; i < 64; i++)
	{
		if (settings.blacklist[i] && settings.blacklistEnabled[i])
			return true;
	}

	return false;
}

bool Looter::CheckIngredientList()
{
	for (auto i : Settings::harvester.enabledList)
	{
		if (i)
			return true;
	}

	return false;
}

bool Looter::CheckScrapList()
{
	for (auto i : Settings::scrapLooter.enabledList)
	{
		if (i)
			return true;
	}

	return false;
}

bool Looter::CheckItemLooterSettings()
{
	if (Settings::itemLooter.lootWeaponsEnabled && Settings::itemLooter.lootWeaponsDistance > 0)
		return true;
	if (Settings::itemLooter.lootArmorEnabled && Settings::itemLooter.lootArmorDistance > 0)
		return true;
	if (Settings::itemLooter.lootAmmoEnabled && Settings::itemLooter.lootAmmoDistance > 0)
		return true;
	if (Settings::itemLooter.lootModsEnabled && Settings::itemLooter.lootModsDistance > 0)
		return true;
	if (Settings::itemLooter.lootMagazinesEnabled && Settings::itemLooter.lootMagazinesDistance > 0)
		return true;
	if (Settings::itemLooter.lootBobbleheadsEnabled && Settings::itemLooter.lootBobbleheadsDistance > 0)
		return true;
	if (Settings::itemLooter.lootAidEnabled && Settings::itemLooter.lootAidDistance > 0)
		return true;
	if (Settings::itemLooter.lootKnownPlansEnabled && Settings::itemLooter.lootKnownPlansDistance > 0)
		return true;
	if (Settings::itemLooter.lootUnknownPlansEnabled && Settings::itemLooter.lootUnknownPlansDistance > 0)
		return true;
	if (Settings::itemLooter.lootMiscEnabled && Settings::itemLooter.lootMiscDistance > 0)
		return true;
	if (Settings::itemLooter.lootUnlistedEnabled && Settings::itemLooter.lootUnlistedDistance > 0)
		return true;
	if (Settings::itemLooter.lootListEnabled && Settings::itemLooter.lootListDistance > 0)
		return CheckItemLooterList();

	return false;
}

bool Looter::CheckOnlyUseItemLooterList()
{
	if (Settings::itemLooter.lootWeaponsEnabled && Settings::itemLooter.lootWeaponsDistance > 0)
		return false;
	if (Settings::itemLooter.lootArmorEnabled && Settings::itemLooter.lootArmorDistance > 0)
		return false;
	if (Settings::itemLooter.lootAmmoEnabled && Settings::itemLooter.lootAmmoDistance > 0)
		return false;
	if (Settings::itemLooter.lootModsEnabled && Settings::itemLooter.lootModsDistance > 0)
		return false;
	if (Settings::itemLooter.lootMagazinesEnabled && Settings::itemLooter.lootMagazinesDistance > 0)
		return false;
	if (Settings::itemLooter.lootBobbleheadsEnabled && Settings::itemLooter.lootBobbleheadsDistance > 0)
		return false;
	if (Settings::itemLooter.lootAidEnabled && Settings::itemLooter.lootAidDistance > 0)
		return false;
	if (Settings::itemLooter.lootKnownPlansEnabled && Settings::itemLooter.lootKnownPlansDistance > 0)
		return false;
	if (Settings::itemLooter.lootUnknownPlansEnabled && Settings::itemLooter.lootUnknownPlansDistance > 0)
		return false;
	if (Settings::itemLooter.lootMiscEnabled && Settings::itemLooter.lootMiscDistance > 0)
		return false;
	if (Settings::itemLooter.lootUnlistedEnabled && Settings::itemLooter.lootUnlistedDistance > 0)
		return false;
	if (Settings::itemLooter.lootListEnabled && Settings::itemLooter.lootListDistance > 0)
		return CheckItemLooterList();

	return false;
}

bool Looter::CheckEnabledItem(const DWORD formId, const DWORD64 entityFlag, const int normalDistance)
{
	if (Settings::itemLooter.lootListEnabled)
	{
		if (ErectusMemory::CheckFormIdArray(formId, Settings::itemLooter.enabledList, Settings::itemLooter.formIdList, 100))
		{
			if (normalDistance <= Settings::itemLooter.lootListDistance)
				return true;
		}
	}

	if (entityFlag & CUSTOM_ENTRY_WEAPON && normalDistance <= Settings::itemLooter.lootWeaponsDistance)
		return Settings::itemLooter.lootWeaponsEnabled;
	if (entityFlag & CUSTOM_ENTRY_ARMOR && normalDistance <= Settings::itemLooter.lootArmorDistance)
		return Settings::itemLooter.lootArmorEnabled;
	if (entityFlag & CUSTOM_ENTRY_AMMO && normalDistance <= Settings::itemLooter.lootAmmoDistance)
		return Settings::itemLooter.lootAmmoEnabled;
	if (entityFlag & CUSTOM_ENTRY_MOD && normalDistance <= Settings::itemLooter.lootModsDistance)
		return Settings::itemLooter.lootModsEnabled;
	if (entityFlag & CUSTOM_ENTRY_MAGAZINE && normalDistance <= Settings::itemLooter.lootMagazinesDistance)
		return Settings::itemLooter.lootMagazinesEnabled;
	if (entityFlag & CUSTOM_ENTRY_BOBBLEHEAD && normalDistance <= Settings::itemLooter.lootBobbleheadsDistance)
		return Settings::itemLooter.lootBobbleheadsEnabled;
	if (entityFlag & CUSTOM_ENTRY_AID && normalDistance <= Settings::itemLooter.lootAidDistance)
		return Settings::itemLooter.lootAidEnabled;
	if (entityFlag & CUSTOM_ENTRY_KNOWN_RECIPE && normalDistance <= Settings::itemLooter.lootKnownPlansDistance)
		return Settings::itemLooter.lootKnownPlansEnabled;
	if (entityFlag & CUSTOM_ENTRY_UNKNOWN_RECIPE && normalDistance <= Settings::itemLooter.lootUnknownPlansDistance)
		return Settings::itemLooter.lootUnknownPlansEnabled;
	if (entityFlag & CUSTOM_ENTRY_MISC && normalDistance <= Settings::itemLooter.lootMiscDistance)
		return Settings::itemLooter.lootMiscEnabled;
	if (Settings::itemLooter.lootUnlistedEnabled && normalDistance <= Settings::itemLooter.lootUnlistedDistance)
		return true;

	return false;
}

bool Looter::TransferEntityItems(const TesObjectRefr& entityData, const TesItem& referenceData, const TesObjectRefr& localPlayer, const	bool onlyUseEntityLooterList, const bool useEntityLooterBlacklist)
{
	EntityLooterSettings currentEntityLooterSettings = {};
	switch (referenceData.formType)
	{
	case (static_cast<BYTE>(FormTypes::TesNpc)):
		currentEntityLooterSettings = Settings::npcLooter;
		break;
	case (static_cast<BYTE>(FormTypes::TesObjectCont)):
		currentEntityLooterSettings = Settings::containerLooter;
		break;
	default:
		return false;
	}

	if (!Utils::Valid(entityData.inventoryPtr))
		return false;

	Inventory inventoryData{};
	if (!ErectusProcess::Rpm(entityData.inventoryPtr, &inventoryData, sizeof inventoryData))
		return false;
	if (!Utils::Valid(inventoryData.itemArrayPtr) || inventoryData.itemArrayEnd < inventoryData.itemArrayPtr)
		return false;

	auto itemArraySize = (inventoryData.itemArrayEnd - inventoryData.itemArrayPtr) / sizeof(Item);
	if (!itemArraySize || itemArraySize > 0x7FFF)
		return false;

	auto* itemData = new Item[itemArraySize];
	if (!ErectusProcess::Rpm(inventoryData.itemArrayPtr, &*itemData, itemArraySize * sizeof(Item)))
	{
		delete[]itemData;
		itemData = nullptr;
		return false;
	}

	auto legendaryWeaponsEnabled = AllowLegendaryWeapons(currentEntityLooterSettings);
	auto legendaryArmorEnabled = AllowLegendaryArmor(currentEntityLooterSettings);

	for (DWORD64 i = 0; i < itemArraySize; i++)
	{
		if (!Utils::Valid(itemData[i].referencePtr))
			continue;
		if (!Utils::Valid(itemData[i].displayPtr) || itemData[i].iterations < itemData[i].displayPtr)
			continue;

		TesItem itemReferenceData{};
		if (!ErectusProcess::Rpm(itemData[i].referencePtr, &itemReferenceData, sizeof itemReferenceData))
			continue;
		if (itemReferenceData.recordFlagA >> 2 & 1)
			continue;

		if (useEntityLooterBlacklist)
		{
			if (ErectusMemory::CheckFormIdArray(itemReferenceData.formId, currentEntityLooterSettings.blacklistEnabled, currentEntityLooterSettings.blacklist, 64))
				continue;
		}

		if (onlyUseEntityLooterList)
		{
			if (!ErectusMemory::CheckFormIdArray(itemReferenceData.formId, currentEntityLooterSettings.enabledList, currentEntityLooterSettings.formIdList, 100))
				continue;
		}

		auto entityFlag = CUSTOM_ENTRY_DEFAULT;
		DWORD64 entityNamePtr = 0;
		auto enabledDistance = 0;

		ErectusMemory::GetCustomEntityData(itemReferenceData, &entityFlag, &entityNamePtr, &enabledDistance, false, false);
		if (!(entityFlag & CUSTOM_ENTRY_VALID_ITEM))
			continue;

		if (!onlyUseEntityLooterList && !CheckEntityLooterItem(itemReferenceData.formId, entityFlag, currentEntityLooterSettings, legendaryWeaponsEnabled, legendaryArmorEnabled))
			continue;

		auto iterations = (itemData[i].iterations - itemData[i].displayPtr) / sizeof(ItemCount);
		if (!iterations || iterations > 0x7FFF)
			continue;

		auto* itemCountData = new ItemCount[iterations];
		if (!ErectusProcess::Rpm(itemData[i].displayPtr, &*itemCountData, iterations * sizeof(ItemCount)))
		{
			delete[]itemCountData;
			itemCountData = nullptr;
			continue;
		}

		auto count = 0;
		for (DWORD64 c = 0; c < iterations; c++)
		{
			count += itemCountData[c].count;
		}

		delete[]itemCountData;
		itemCountData = nullptr;

		if (count == 0)
			continue;

		if (entityFlag & CUSTOM_ENTRY_WEAPON)
		{
			if (legendaryWeaponsEnabled)
			{
				auto legendaryRank = GetLegendaryRank(itemData[i].displayPtr);
				if (!ValidLegendary(legendaryRank, entityFlag, currentEntityLooterSettings, legendaryWeaponsEnabled, legendaryArmorEnabled))
					continue;
			}
		}
		else if (entityFlag & CUSTOM_ENTRY_ARMOR)
		{
			if (legendaryArmorEnabled)
			{
				auto legendaryRank = GetLegendaryRank(itemData[i].displayPtr);
				if (!ValidLegendary(legendaryRank, entityFlag, currentEntityLooterSettings, legendaryWeaponsEnabled, legendaryArmorEnabled))
					continue;
			}
		}

		TransferMessage transferMessageData = {
			.vtable = ErectusProcess::exe + VTABLE_REQUESTTRANSFERITEMMSG,
			.sourceEntityId = entityData.formId,
			.playerEntityId = 0xE0000E4A,
			.bShouldSendResult = true,
			.destEntityId = localPlayer.formId,
			.itemServerHandleId = itemData[i].itemId,
			.stashAccessEntityId = 0x00000000,
			.bCreateIfMissing = false,
			.bIsExpectingResult = false,
			.count = count,
		};

		MsgSender::Send(&transferMessageData, sizeof transferMessageData);
	}

	delete[]itemData;
	itemData = nullptr;
	return true;
}

bool Looter::ValidLegendary(const BYTE legendaryRank, const DWORD64 entityFlag, const EntityLooterSettings& customEntityLooterSettings, const bool legendaryWeaponsEnabled, const bool legendaryArmorEnabled)
{
	if (entityFlag & CUSTOM_ENTRY_WEAPON)
	{
		if (legendaryWeaponsEnabled)
		{
			switch (legendaryRank)
			{
			case 0x01:
				return customEntityLooterSettings.oneStarWeaponsEnabled;
			case 0x02:
				return customEntityLooterSettings.twoStarWeaponsEnabled;
			case 0x03:
				return customEntityLooterSettings.threeStarWeaponsEnabled;
			default:
				return customEntityLooterSettings.allWeaponsEnabled;
			}
		}
	}
	else if (entityFlag & CUSTOM_ENTRY_ARMOR)
	{
		if (legendaryArmorEnabled)
		{
			switch (legendaryRank)
			{
			case 0x01:
				return customEntityLooterSettings.oneStarArmorEnabled;
			case 0x02:
				return customEntityLooterSettings.twoStarArmorEnabled;
			case 0x03:
				return customEntityLooterSettings.threeStarArmorEnabled;
			default:
				return customEntityLooterSettings.allArmorEnabled;
			}
		}
	}

	return false;
}

BYTE Looter::GetLegendaryRank(const DWORD64 displayPtr)
{
	if (!Utils::Valid(displayPtr))
		return 0;

	DWORD64 instancedArrayPtr;
	if (!ErectusProcess::Rpm(displayPtr, &instancedArrayPtr, sizeof instancedArrayPtr))
		return 0;
	if (!Utils::Valid(instancedArrayPtr))
		return 0;

	ItemInstancedArray itemInstancedArrayData{};
	if (!ErectusProcess::Rpm(instancedArrayPtr, &itemInstancedArrayData, sizeof itemInstancedArrayData))
		return 0;
	if (!Utils::Valid(itemInstancedArrayData.arrayPtr) || itemInstancedArrayData.arrayEnd < itemInstancedArrayData.arrayPtr)
		return 0;

	const auto instancedArraySize = (itemInstancedArrayData.arrayEnd - itemInstancedArrayData.arrayPtr) / sizeof(DWORD64);
	if (!instancedArraySize || instancedArraySize > 0x7FFF)
		return 0;

	auto* instancedArray = new DWORD64[instancedArraySize];
	if (!ErectusProcess::Rpm(itemInstancedArrayData.arrayPtr, &*instancedArray, instancedArraySize * sizeof(DWORD64)))
	{
		delete[]instancedArray;
		instancedArray = nullptr;
		return 0;
	}

	DWORD64 objectInstanceExtraPtr = 0;

	for (DWORD64 i = 0; i < instancedArraySize; i++)
	{
		if (!Utils::Valid(instancedArray[i]))
			continue;

		ExtraTextDisplayData extraTextDisplayDataData{};
		if (!ErectusProcess::Rpm(instancedArray[i], &extraTextDisplayDataData, sizeof extraTextDisplayDataData))
			continue;

		const auto rttiNamePtr = ErectusMemory::RttiGetNamePtr(extraTextDisplayDataData.vtable);
		if (!rttiNamePtr)
			continue;

		char rttiNameCheck[sizeof".?AVBGSObjectInstanceExtra@@"];
		if (!ErectusProcess::Rpm(rttiNamePtr, &rttiNameCheck, sizeof rttiNameCheck))
			continue;
		if (strcmp(rttiNameCheck, ".?AVBGSObjectInstanceExtra@@") != 0)
			continue;

		objectInstanceExtraPtr = instancedArray[i];
		break;
	}

	delete[]instancedArray;
	instancedArray = nullptr;

	if (!objectInstanceExtraPtr)
		return 0;

	ObjectInstanceExtra objectInstanceExtraData{};
	if (!ErectusProcess::Rpm(objectInstanceExtraPtr, &objectInstanceExtraData, sizeof objectInstanceExtraData))
		return 0;
	if (!Utils::Valid(objectInstanceExtraData.modDataPtr))
		return 0;

	ModInstance modInstanceData{};
	if (!ErectusProcess::Rpm(objectInstanceExtraData.modDataPtr, &modInstanceData, sizeof modInstanceData))
		return 0;
	if (!Utils::Valid(modInstanceData.modListPtr) || !modInstanceData.modListSize)
		return 0;

	const DWORD64 modArraySize = modInstanceData.modListSize / 0x8;
	if (!modArraySize || modArraySize > 0x7FFF)
		return 0;

	auto* modArray = new DWORD[modArraySize * 2];
	if (!ErectusProcess::Rpm(modInstanceData.modListPtr, &*modArray, modArraySize * 2 * sizeof(DWORD)))
	{
		delete[]modArray;
		return 0;
	}

	BYTE legendaryRank = 0;
	for (DWORD64 i = 0; i < modArraySize; i++)
	{
		if (IsLegendaryFormId(modArray[i * 2]))
		{
			legendaryRank++;
		}
	}

	delete[]modArray;
	return legendaryRank;
}

bool Looter::IsLegendaryFormId(const DWORD formId)
{
	for (const auto& i : legendaryFormIdArray)
	{
		if (formId == i)
			return true;
	}

	return false;
}

bool Looter::AllowLegendaryWeapons(const EntityLooterSettings& settings)
{
	if (!settings.allWeaponsEnabled)
	{
		if (settings.oneStarWeaponsEnabled)
			return true;
		if (settings.twoStarWeaponsEnabled)
			return true;
		if (settings.threeStarWeaponsEnabled)
			return true;
	}

	return false;
}

bool Looter::AllowLegendaryArmor(const EntityLooterSettings& settings)
{
	if (settings.allArmorEnabled || settings.oneStarArmorEnabled || settings.twoStarArmorEnabled || settings.threeStarArmorEnabled)
		return true;

	return false;
}

bool Looter::EntityInventoryValid(const TesObjectRefr& entityData)
{
	if (!Utils::Valid(entityData.inventoryPtr))
		return false;

	Inventory inventoryData{};
	if (!ErectusProcess::Rpm(entityData.inventoryPtr, &inventoryData, sizeof inventoryData))
		return false;
	if (!Utils::Valid(inventoryData.itemArrayPtr) || inventoryData.itemArrayEnd < inventoryData.itemArrayPtr)
		return false;

	const auto itemArraySize = (inventoryData.itemArrayEnd - inventoryData.itemArrayPtr) / sizeof(Item);
	if (!itemArraySize || itemArraySize > 0x7FFF)
		return false;

	auto* itemData = new Item[itemArraySize];
	if (!ErectusProcess::Rpm(inventoryData.itemArrayPtr, &*itemData, itemArraySize * sizeof(Item)))
	{
		delete[]itemData;
		itemData = nullptr;
		return false;
	}

	for (DWORD64 i = 0; i < itemArraySize; i++)
	{
		if (!Utils::Valid(itemData[i].referencePtr))
			continue;

		TesItem referenceData{};
		if (!ErectusProcess::Rpm(itemData[i].referencePtr, &referenceData, sizeof referenceData))
			continue;
		if (referenceData.recordFlagA >> 2 & 1)
			continue;

		delete[]itemData;
		itemData = nullptr;
		return true;
	}

	delete[]itemData;
	itemData = nullptr;
	return false;
}

bool Looter::CheckEntityLooterItem(const DWORD formId, const DWORD64 entityFlag, const EntityLooterSettings& settings, const bool legendaryWeaponsEnabled, const bool legendaryArmorEnabled)
{
	if (settings.capsEnabled && formId == 0x0000000F)
		return true;

	if (settings.listEnabled)
	{
		if (ErectusMemory::CheckFormIdArray(formId, settings.enabledList, settings.formIdList, 100))
			return true;
	}

	if (entityFlag & CUSTOM_ENTRY_WEAPON)
	{
		if (settings.allWeaponsEnabled)
			return true;

		return legendaryWeaponsEnabled;
	}
	if (entityFlag & CUSTOM_ENTRY_ARMOR)
	{
		if (settings.allArmorEnabled)
			return true;

		return legendaryArmorEnabled;
	}
	if (entityFlag & CUSTOM_ENTRY_AMMO)
		return settings.ammoEnabled;
	if (entityFlag & CUSTOM_ENTRY_MOD)
		return settings.modsEnabled;
	if (entityFlag & CUSTOM_ENTRY_JUNK)
		return settings.junkEnabled;
	if (entityFlag & CUSTOM_ENTRY_AID)
		return settings.aidEnabled;
	if (entityFlag & CUSTOM_ENTRY_TREASURE_MAP)
		return settings.treasureMapsEnabled;
	if (entityFlag & CUSTOM_ENTRY_PLAN)
	{
		if (entityFlag & CUSTOM_ENTRY_KNOWN_RECIPE)
			return settings.knownPlansEnabled;
		if (entityFlag & CUSTOM_ENTRY_UNKNOWN_RECIPE)
			return settings.unknownPlansEnabled;
	}
	else if (entityFlag & CUSTOM_ENTRY_MISC)
		return settings.miscEnabled;

	if (settings.unlistedEnabled)
		return true;

	return false;
}
