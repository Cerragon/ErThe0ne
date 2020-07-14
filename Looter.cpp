//FIXME: settings load/save

#include "Looter.h"

#include <memory>

#include "common.h"
#include "ErectusProcess.h"
#include "MsgSender.h"
#include "settings.h"
#include "utils.h"

namespace
{
	std::unordered_set<DWORD> legendaryFormIds = {
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


bool Looter::ShouldLootJunk(const ItemInfo& item)
{
	if (!item.base.componentArraySize || item.base.componentArraySize > 0x7FFF)
		return false;

	if (!Utils::Valid(item.base.componentArrayPtr))
		return false;

	auto components = std::make_unique<Component[]>(item.base.componentArraySize);
	if (!ErectusProcess::Rpm(item.base.componentArrayPtr, components.get(), item.base.componentArraySize * sizeof(Component)))
		return false;

	for (auto i = 0; i < item.base.componentArraySize; i++)
	{
		if (!Utils::Valid(components[i].componentReferencePtr))
			continue;
		if (!Utils::Valid(components[i].componentCountReferencePtr))
			continue;

		TesItem componentData{};
		if (!ErectusProcess::Rpm(components[i].componentReferencePtr, &componentData, sizeof componentData))
			continue;

		if (Settings::looter.selection.junk.components.find(componentData.formId)->second)
			return true;
	}

	return false;
}

bool Looter::ShouldLootFloraLeveled(const LeveledList& list)
{
	if (!Utils::Valid(list.listEntryArrayPtr) || !list.listEntryArraySize)
		return false;

	auto listEntryData = std::make_unique<ListEntry[]>(list.listEntryArraySize);

	if (!ErectusProcess::Rpm(list.listEntryArrayPtr, listEntryData.get(), list.listEntryArraySize * sizeof(ListEntry)))
		return false;

	for (BYTE i = 0; i < list.listEntryArraySize; i++)
	{
		if (!Utils::Valid(listEntryData[i].referencePtr))
			continue;

		TesItem referenceData{};
		if (!ErectusProcess::Rpm(listEntryData[i].referencePtr, &referenceData, sizeof referenceData))
			continue;
		if (referenceData.formType == static_cast<BYTE>(FormTypes::TesLevItem))
		{
			LeveledList innerList{};
			memcpy(&innerList, &referenceData, sizeof innerList);
			if (ShouldLootFloraLeveled(innerList))
				return true;
		}
		else if (Settings::looter.selection.flora.components.find(referenceData.formId)->second)
			return true;
	}

	return false;
}

bool Looter::ShouldLootFlora(const ItemInfo& item)
{
	if (item.base.formId == 0x000183C6)
		return Settings::looter.selection.flora.components.find(item.base.formId)->second;

	if (!Utils::Valid(item.base.harvestedPtr))
		return false;

	TesItem harvestedData{};
	if (!ErectusProcess::Rpm(item.base.harvestedPtr, &harvestedData, sizeof harvestedData))
		return false;
	if (item.base.formType == static_cast<BYTE>(FormTypes::TesLevItem))
	{
		LeveledList leveledListData{};
		memcpy(&leveledListData, &harvestedData, sizeof leveledListData);
		return ShouldLootFloraLeveled(leveledListData);
	}
	return Settings::looter.selection.flora.components.find(harvestedData.formId)->second;
}

bool Looter::ShouldLootItem(const ItemInfo& item, const DWORD64 displayPtr = 0)
{
	BYTE rank;

	if (Settings::looter.selection.blacklist.find(item.base.formId)->second)
		return false;

	if (Settings::looter.selection.whitelist.find(item.base.formId)->second)
		return true;
	
	switch (item.type)
	{
	case ItemTypes::Weapons:
		if (Settings::looter.selection.weapons.all)
			return true;
		rank = GetLegendaryRank(displayPtr);
		return Settings::looter.selection.weapons.oneStar && rank >= 1 || Settings::looter.selection.weapons.twoStar && rank >= 2 || Settings::looter.selection.weapons.threeStar && rank >= 3;
	case ItemTypes::Apparel:
		if (Settings::looter.selection.apparel.all)
			return true;
		rank = GetLegendaryRank(displayPtr);
		return Settings::looter.selection.apparel.oneStar && rank >= 1 || Settings::looter.selection.apparel.twoStar && rank >= 2 || Settings::looter.selection.apparel.threeStar && rank >= 3;
	case ItemTypes::Misc:
		return Settings::looter.selection.misc.all;
	case ItemTypes::Aid:
		return Settings::looter.selection.aid.all;
	case ItemTypes::AidBobblehead:
		return Settings::looter.selection.aid.all || Settings::looter.selection.aid.bobbleheads;
	case ItemTypes::AidMagazine:
		return Settings::looter.selection.aid.all || Settings::looter.selection.aid.magazines;
	case ItemTypes::Notes:
		return Settings::looter.selection.notes.all;
	case ItemTypes::NotesKnownPlan:
		return Settings::looter.selection.notes.all || Settings::looter.selection.notes.plansKnown;
	case ItemTypes::NotesUnknownPlan:
		return Settings::looter.selection.notes.all || Settings::looter.selection.notes.plansUnknown;
	case ItemTypes::NotesTreasureMap:
		return Settings::looter.selection.notes.all || Settings::looter.selection.notes.treasureMaps;
	case ItemTypes::Junk:
		return Settings::looter.selection.junk.all || ShouldLootJunk(item);
	case ItemTypes::Flora:
		return Settings::looter.selection.flora.all || ShouldLootFlora(item);
	case ItemTypes::Mod:
		return Settings::looter.selection.mods.all;
	case ItemTypes::Ammo:
		return Settings::looter.selection.ammo.all;
	case ItemTypes::Currency:
		return Settings::looter.selection.other.caps;
	default:
		return false;
	}
}

void Looter::LootGroundItem(const ItemInfo& item, const TesObjectRefr& player)
{
	if (!MsgSender::IsEnabled())
		return;

	if (!ShouldLootItem(item))
		return;

	if (Utils::GetDistance(item.refr.position, player.position) * 0.01f > 76.f)
		return;

	RequestActivateRefMessage requestActivateRefMessageData{
		.vtable = ErectusProcess::exe + VTABLE_REQUESTACTIVATEREFMSG,
		.formId = item.refr.formId,
		.choice = 0xFF,
		.forceActivate = 0
	};
	MsgSender::Send(&requestActivateRefMessageData, sizeof requestActivateRefMessageData);
}

void Looter::LootContainer(const ItemInfo& item, const TesObjectRefr& player)
{
	switch (item.type)
	{
	case ItemTypes::Npc:
		if (item.refr.formId == 0x00000007 || ErectusMemory::CheckHealthFlag(item.refr.healthFlag) != 0x3)
			return;
		if (Utils::GetDistance(item.refr.position, player.position) * 0.01f > 76.f)
			return;
		break;

	case ItemTypes::Container:
		if (Utils::GetDistance(item.refr.position, player.position) * 0.01f > 6.f)
			return;
		if (!ContainerValid(item.base))
			return;
		break;

	default:
		return;
	}

	if (!Utils::Valid(item.refr.inventoryPtr))
		return;

	Inventory inventory{};
	if (!ErectusProcess::Rpm(item.refr.inventoryPtr, &inventory, sizeof inventory))
		return;
	if (!Utils::Valid(inventory.entryArrayBegin) || inventory.entryArrayEnd <= inventory.entryArrayBegin)
		return;

	auto entryArraySize = (inventory.entryArrayEnd - inventory.entryArrayBegin) / sizeof(InventoryEntry);
	auto entries = std::make_unique<InventoryEntry[]>(entryArraySize);
	if (!ErectusProcess::Rpm(inventory.entryArrayBegin, entries.get(), entryArraySize * sizeof(InventoryEntry)))
		return;

	for (DWORD64 i = 0; i < entryArraySize; i++)
	{
		if (!Utils::Valid(entries[i].referencePtr))
			continue;
		if (!Utils::Valid(entries[i].displayPtr) || entries[i].iterations < entries[i].displayPtr) //???
			continue;

		TesItem baseItem{};
		if (!ErectusProcess::Rpm(entries[i].referencePtr, &baseItem, sizeof baseItem))
			continue;
		if (baseItem.recordFlagA >> 2 & 1) //???
			continue;

		auto inventoryItem = ErectusMemory::GetItemInfo(TesObjectRefr(), baseItem);
		if (!ShouldLootItem(inventoryItem, entries[i].displayPtr))
			continue;

		auto iterations = (entries[i].iterations - entries[i].displayPtr) / sizeof(ItemCount);
		if (!iterations || iterations > 0x7FFF)
			continue;

		auto itemCount = std::make_unique<ItemCount[]>(iterations);
		if (!ErectusProcess::Rpm(entries[i].displayPtr, itemCount.get(), iterations * sizeof(ItemCount)))
			continue;

		auto count = 0;
		for (DWORD64 c = 0; c < iterations; c++)
		{
			count += itemCount[c].count;
		}

		if (count == 0)
			continue;

		TransferMessage transferMessageData = {
			.vtable = ErectusProcess::exe + VTABLE_REQUESTTRANSFERITEMMSG,
			.sourceEntityId = item.refr.formId,
			.playerEntityId = 0xE0000E4A,
			.bShouldSendResult = true,
			.destEntityId = player.formId,
			.itemServerHandleId = entries[i].itemId,
			.stashAccessEntityId = 0x00000000,
			.bCreateIfMissing = false,
			.bIsExpectingResult = false,
			.count = count,
		};
		MsgSender::Send(&transferMessageData, sizeof transferMessageData);
	}
}

void Looter::LootFlora(const ItemInfo& item, const TesObjectRefr& localPlayer)
{
	if (!MsgSender::IsEnabled())
		return;

	if (ErectusMemory::IsFloraHarvested(item.refr.harvestFlagA, item.refr.harvestFlagB))
		return;

	if (Utils::GetDistance(item.refr.position, localPlayer.position) * 0.01f > 6.f)
		return;

	if (!ShouldLootItem(item))
		return;

	RequestActivateRefMessage requestActivateRefMessageData{
		.vtable = ErectusProcess::exe + VTABLE_REQUESTACTIVATEREFMSG,
		.formId = item.refr.formId,
		.choice = 0xFF,
		.forceActivate = 0
	};
	MsgSender::Send(&requestActivateRefMessageData, sizeof requestActivateRefMessageData);
}


void Looter::Loot()
{
	if (!MsgSender::IsEnabled())
		return;

	if (Settings::looter.mode == LooterSettings::Mode::Disabled || Settings::looter.mode == LooterSettings::Mode::Keybind && !lootItemsRequested)
		return;
	if (!Settings::looter.looters.npcs && !Settings::looter.looters.items && !Settings::looter.looters.containers && !Settings::looter.looters.flora)
		return;
	if (!Settings::looter.selection.IsEnabled())
		return;
	if (!ErectusMemory::GetLocalPlayerPtr(true))
		return;

	auto localPlayerPtr = ErectusMemory::GetLocalPlayerPtr(true);
	if (!Utils::Valid(localPlayerPtr))
		return;

	TesObjectRefr localPlayer{};
	if (!ErectusProcess::Rpm(localPlayerPtr, &localPlayer, sizeof localPlayer))
		return;

	auto entityPtrs = ErectusMemory::GetEntityPtrList();
	for (const auto& entityPtr : entityPtrs)
	{
		TesObjectRefr entity{};
		if (!ErectusProcess::Rpm(entityPtr, &entity, sizeof entity))
			continue;

		if (!Utils::Valid(entity.baseObjectPtr))
			continue;

		if (entity.spawnFlag != 0x02)
			continue;

		switch (entity.formType)
		{
		case (static_cast<BYTE>(FormTypes::TesActor)):
			if (!Settings::looter.looters.npcs || ErectusMemory::CheckHealthFlag(entity.healthFlag) != 0x3)
				continue;
			break;
		case (static_cast<BYTE>(FormTypes::TesObjectRefr)):
			if (!Settings::looter.looters.items && !Settings::looter.looters.containers && !Settings::looter.looters.flora)
				continue;
			break;
		default:
			continue;
		}

		TesItem baseItem{};
		if (!ErectusProcess::Rpm(entity.baseObjectPtr, &baseItem, sizeof baseItem))
			continue;

		auto item = ErectusMemory::GetItemInfo(entity, baseItem);
		switch (item.type)
		{
		case ItemTypes::Npc:
		case ItemTypes::Container:
			LootContainer(item, localPlayer);
			break;
		case ItemTypes::Flora:
			LootFlora(item, localPlayer);
			break;
		case ItemTypes::Invalid:
		case ItemTypes::Other:
			break;
		default:
			LootGroundItem(item, localPlayer);
		}
	}
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

	auto instancedArray = std::make_unique<DWORD64[]>(instancedArraySize);
	if (!ErectusProcess::Rpm(itemInstancedArrayData.arrayPtr, instancedArray.get(), instancedArraySize * sizeof(DWORD64)))
		return 0;

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

	auto modArray = std::make_unique<DWORD[]>(modArraySize * 2);
	if (!ErectusProcess::Rpm(modInstanceData.modListPtr, modArray.get(), modArraySize * 2 * sizeof(DWORD)))
		return 0;

	BYTE legendaryRank = 0;
	for (DWORD64 i = 0; i < modArraySize; i++)
	{
		if (legendaryFormIds.count(modArray[i * 2]) > 0)
			legendaryRank++;
	}
	return legendaryRank;
}
