#include <memory>

#include "Looter.h"
#include "MsgSender.h"
#include "../ErectusMemory.h"
#include "../ErectusProcess.h"
#include "../settings.h"
#include "../utils.h"
#include "../game/Game.h"

bool Looter::ShouldLootJunk(const ItemInfo& item)
{
	if (!item.base.componentArraySize || item.base.componentArraySize > 0x7FFF)
		return false;

	if (!Utils::Valid(item.base.componentArrayPtr))
		return false;

	const auto components = std::make_unique<Component[]>(item.base.componentArraySize);
	if (!ErectusProcess::Rpm(item.base.componentArrayPtr, components.get(), item.base.componentArraySize * sizeof(Component)))
		return false;

	for (size_t i = 0; i < item.base.componentArraySize; i++)
	{
		if (!Utils::Valid(components[i].componentReferencePtr))
			continue;
		if (!Utils::Valid(components[i].componentCountReferencePtr))
			continue;

		TesItem componentData{};
		if (!ErectusProcess::Rpm(components[i].componentReferencePtr, &componentData, sizeof componentData))
			continue;

		if (Settings::looter.selection.junk.components.contains(componentData.formId) && Settings::looter.selection.junk.components.find(componentData.formId)->second)
			return true;
	}

	return false;
}

bool Looter::ShouldLootFloraLeveled(const LeveledList& list)
{
	if (!Utils::Valid(list.listEntryArrayPtr) || !list.listEntryArraySize)
		return false;

	const auto listEntryData = std::make_unique<ListEntry[]>(list.listEntryArraySize);

	if (!ErectusProcess::Rpm(list.listEntryArrayPtr, listEntryData.get(), list.listEntryArraySize * sizeof(ListEntry)))
		return false;

	for (BYTE i = 0; i < list.listEntryArraySize; i++)
	{
		if (!Utils::Valid(listEntryData[i].referencePtr))
			continue;

		TesItem referenceData{};
		if (!ErectusProcess::Rpm(listEntryData[i].referencePtr, &referenceData, sizeof referenceData))
			continue;
		if (referenceData.GetFormType() == FormType::TesLevItem)
		{
			LeveledList innerList{};
			memcpy(&innerList, &referenceData, sizeof innerList);
			if (ShouldLootFloraLeveled(innerList))
				return true;
		}
		else if (Settings::looter.selection.flora.components.contains(referenceData.formId) && Settings::looter.selection.flora.components.find(referenceData.formId)->second)
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
	if (harvestedData.GetFormType() == FormType::TesLevItem)
	{
		LeveledList leveledListData{};
		memcpy(&leveledListData, &harvestedData, sizeof leveledListData);
		return ShouldLootFloraLeveled(leveledListData);
	}
	return Settings::looter.selection.flora.components.contains(harvestedData.formId) && Settings::looter.selection.flora.components.find(harvestedData.formId)->second;
}

bool Looter::ShouldLootItem(const ItemInfo& item, const std::uintptr_t displayPtr = 0)
{
	int rank;

	if (const auto found = Settings::looter.selection.blacklist.find(item.base.formId); found != Settings::looter.selection.blacklist.end()) {
		if (found->second)
			return false;
	}

	if (const auto found = Settings::looter.selection.whitelist.find(item.base.formId); found != Settings::looter.selection.whitelist.end()) {
		if (found->second)
			return true;
	}

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

bool Looter::LootGroundItem(const ItemInfo& item, const LocalPlayer& player)
{
	if (!MsgSender::IsEnabled())
		return false;

	if (!Settings::looter.looters.groundItems)
		return false;

	if (!ShouldLootItem(item))
		return false;

	if (item.refr.position.DistanceTo(player.position) * 0.01f > 76.f)
		return false;

	RequestActivateRefMessage requestActivateRefMessageData{
		.vtable = ErectusProcess::exe + VTABLE_REQUESTACTIVATEREFMSG,
		.formId = item.refr.formId,
		.choice = 0xFF,
		.forceActivate = 0
	};
	MsgSender::Send(&requestActivateRefMessageData, sizeof requestActivateRefMessageData);

	return true;
}

bool Looter::LootContainer(const ItemInfo& item, const LocalPlayer& player)
{
	if (!MsgSender::IsEnabled())
		return false;

	switch (item.type)
	{
	case ItemTypes::Npc:
		if (!Settings::looter.looters.npcs)
			return false;

		if (item.refr.GetFormType() == FormType::PlayerCharacter || item.refr.GetActorState() != ActorState::Dead)
			return false;
		if (item.refr.position.DistanceTo(player.position) * 0.01f > 76.f)
			return false;
		break;

	case ItemTypes::Container:
		if (!Settings::looter.looters.containers)
			return false;

		if (item.refr.position.DistanceTo(player.position) * 0.01f > 6.f)
			return false;
		if (!ContainerValid(item.base))
			return false;
		break;

	default:
		return false;
	}

	auto inventory = item.refr.GetInventory();
	
	for (const auto& inventoryEntry : inventory)
	{
		if (!Utils::Valid(inventoryEntry.baseObjectPtr))
			continue;
		if (!Utils::Valid(inventoryEntry.displayPtr) || inventoryEntry.iterations <= inventoryEntry.displayPtr)
			continue;

		TesObjectRefr ref = { .baseObjectPtr = inventoryEntry.baseObjectPtr };

//		if (baseItem.recordFlagA >> 2 & 1) //???
//			continue;

		auto inventoryItem = ErectusMemory::GetItemInfo(ref);
		if (!ShouldLootItem(inventoryItem, inventoryEntry.displayPtr))
			continue;

		auto iterations = (inventoryEntry.iterations - inventoryEntry.displayPtr) / sizeof(ItemCount);
		auto itemCount = std::make_unique<ItemCount[]>(iterations);
		if (!ErectusProcess::Rpm(inventoryEntry.displayPtr, itemCount.get(), iterations * sizeof(ItemCount)))
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
			.playerEntityId = 0,
			.bShouldSendResult = false,
			.destEntityId = player.formId,
			.itemServerHandleId = inventoryEntry.itemId,
			.stashAccessEntityId = 0x00000000,
			.bCreateIfMissing = false,
			.bIsExpectingResult = false,
			.count = count,
		};
		MsgSender::Send(&transferMessageData, sizeof transferMessageData);
	}
	return true;
}

bool Looter::LootFlora(const ItemInfo& item, const LocalPlayer& player)
{
	if (!MsgSender::IsEnabled())
		return false;

	if (!Settings::looter.looters.flora)
		return false;

	if (ErectusMemory::IsFloraHarvested(item.refr.harvestFlagA, item.refr.harvestFlagB))
		return false;

	if (item.refr.position.DistanceTo(player.position) * 0.01f > 6.f)
		return false;

	if (!ShouldLootItem(item))
		return false;

	RequestActivateRefMessage requestActivateRefMessageData{
		.vtable = ErectusProcess::exe + VTABLE_REQUESTACTIVATEREFMSG,
		.formId = item.refr.formId,
		.choice = 0xFF,
		.forceActivate = 0
	};
	MsgSender::Send(&requestActivateRefMessageData, sizeof requestActivateRefMessageData);

	return true;
}


bool Looter::ProcessEntity(const TesObjectRefr& entity, const LocalPlayer& localPlayer)
{
	if (entity.spawnFlag != 0x02)
		return false;

	switch (entity.GetFormType())
	{
	case FormType::TesActor:
		if (!Settings::looter.looters.npcs || entity.GetActorState() != ActorState::Dead)
			return false;
		break;
	case FormType::TesObjectRefr:
		if (!Settings::looter.looters.groundItems && !Settings::looter.looters.containers && !Settings::looter.looters.flora)
			return false;
		break;
	default:
		return false;
	}

	const auto item = ErectusMemory::GetItemInfo(entity);
	switch (item.type)
	{
	case ItemTypes::Npc:
	case ItemTypes::Container:
		return LootContainer(item, localPlayer); //might want to reevaluate that; may cause containers to be ignored incorrectly in subsequent runs if the 6m loot radius is inaccurate
	case ItemTypes::Flora:
		return LootFlora(item, localPlayer);
	case ItemTypes::Invalid:
	case ItemTypes::Other:
		return false;
	default:
		return LootGroundItem(item, localPlayer);
	}
}

void Looter::Loot()
{
	if (!MsgSender::IsEnabled())
		return;

	if (Settings::looter.mode == LooterSettings::Mode::Disabled || Settings::looter.mode == LooterSettings::Mode::Keybind && !lootItemsRequested)
		return;
	if (!Settings::looter.looters.npcs && !Settings::looter.looters.groundItems && !Settings::looter.looters.containers && !Settings::looter.looters.flora)
		return;
	if (!Settings::looter.selection.IsEnabled())
		return;

	const auto player = Game::GetLocalPlayer();
	if (!player.IsIngame())
		return;

	const auto currentCellFormId = player.GetCurrentCell().formId;
	if(currentCellFormId != lastPlayerCellFormId)
	{
		lootedEntities.clear();
		lastPlayerCellFormId = currentCellFormId;
	}

	auto cells = Game::GetLoadedCells();
	for(const auto& cell : cells)
	{
		auto entities = cell.GetObjectRefs();
		for(const auto& entity : entities)
		{
			if (lootedEntities.contains(entity.formId))
				continue;

			if (ProcessEntity(entity, player))
				lootedEntities.emplace(entity.formId);
		}
	}
	lootItemsRequested = false;
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

int Looter::GetLegendaryRank(const std::uintptr_t displayPtr)
{
	if (!Utils::Valid(displayPtr))
		return 0;

	std::uintptr_t instancedArrayPtr;
	if (!ErectusProcess::Rpm(displayPtr, &instancedArrayPtr, sizeof instancedArrayPtr))
		return 0;
	if (!Utils::Valid(instancedArrayPtr))
		return 0;

	ItemInstancedArray itemInstancedArrayData{};
	if (!ErectusProcess::Rpm(instancedArrayPtr, &itemInstancedArrayData, sizeof itemInstancedArrayData))
		return 0;
	if (!Utils::Valid(itemInstancedArrayData.arrayPtr) || itemInstancedArrayData.arrayEnd <= itemInstancedArrayData.arrayPtr)
		return 0;

	const auto instancedArraySize = (itemInstancedArrayData.arrayEnd - itemInstancedArrayData.arrayPtr) / sizeof(std::uintptr_t);
	const auto instancedArray = std::make_unique<std::uintptr_t[]>(instancedArraySize);
	if (!ErectusProcess::Rpm(itemInstancedArrayData.arrayPtr, instancedArray.get(), instancedArraySize * sizeof(std::uintptr_t)))
		return 0;

	std::uintptr_t objectInstanceExtraPtr = 0;
	for (size_t i = 0; i < instancedArraySize; i++)
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

	const size_t modArraySize = modInstanceData.modListSize / 0x8;
	if (!modArraySize || modArraySize > 0x7FFF)
		return 0;

	const auto modArray = std::make_unique<std::uint32_t[]>(modArraySize * 2);
	if (!ErectusProcess::Rpm(modInstanceData.modListPtr, modArray.get(), modArraySize * 2 * sizeof(std::uint32_t)))
		return 0;

	auto legendaryRank = 0;
	for (size_t i = 0; i < modArraySize; i++)
	{
		if (LEGENDARYEFFECT_FORMIDS.count(modArray[i * 2]) > 0)
			legendaryRank++;
	}
	return legendaryRank;
}
