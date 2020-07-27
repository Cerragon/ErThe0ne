#include "ErectusMemory.h"

#include "common.h"

#include "settings.h"
#include "renderer.h"
#include "utils.h"

#include "fmt/format.h"
#include <array>
#include <set>

#include "ErectusProcess.h"
#include "MsgSender.h"

DWORD64 ErectusMemory::GetAddress(const DWORD formId)
{
	//this is basically hashmap.find(key)
	//v1+24 == hashmap start (_entries)
	//v1+32 == capacity (_capacity)
	//
	//here the hashmap is hashmap<formId, TesObjectRefr*>
	//item/entry is { std::pair<formId, TesObjectRefr*> value, Entry* next }
	//
	//all this is doing is:
	//find(key) {
	//	if(!_entries)
	//		return 0;
	//
	//	item = _entries[calc_idx(key)]; //ideal position
	//
	//	if(!item->next)
	//		return 0;
	//	do{
	//		if(item->value.first == key)
	//			return item->value.second;
	//		else
	//			item = item->next;
	//	}
	//}
	//
	//calc_idx(key) {
	//	return crc32hash(key) & (_capacity - 1); //note, this is a homegrown crc32 implementation
	//}
	//
	//==> should be able to refactor this into reading an array of struct {DWORD64 formId, DWORD64 address, DWORD64 pad}
	//==> then just loop over it and check formid
	//==> the array will be huge but probably still worth it


	DWORD64 v1;
	if (!ErectusProcess::Rpm(ErectusProcess::exe + OFFSET_GET_PTR_A1, &v1, sizeof v1))
		return 0;
	if (!Utils::Valid(v1))
		return 0;

	DWORD capacity;
	if (!ErectusProcess::Rpm(v1 + 32, &capacity, sizeof capacity))
		return 0;
	if (!capacity)
		return 0;

	//hash = crc32hash(formId)
	DWORD hash = 0;
	for (auto i = 0; i < sizeof formId; i++)
	{
		const auto v4 = (hash ^ formId >> i * 0x8) & 0xFF;

		DWORD v5;
		if (!ErectusProcess::Rpm(ErectusProcess::exe + OFFSET_GET_PTR_A2 + v4 * 0x4, &v5, sizeof v5)) //OFFSET_GET_PTR_A2 is just the start of a crc32 lookup table
			return 0;

		hash = hash >> 0x8 ^ v5;
	}

	auto index = hash & capacity - 1;

	DWORD64 entries;

	//sanity check: array exists
	if (!ErectusProcess::Rpm(v1 + 24, &entries, sizeof entries))
		return 0;
	if (!Utils::Valid(entries))
		return 0;

	//check item->next != -1
	DWORD next;
	if (!ErectusProcess::Rpm(entries + index * 24 + 16, &next, sizeof next))
		return 0;
	if (next == 0xFFFFFFFF)
		return 0;

	auto v9 = capacity;


	for (auto i = 0; i < 100; i++)
	{
		DWORD v10;
		if (!ErectusProcess::Rpm(entries + index * 24, &v10, sizeof v10)) //item->value.first
			return 0;
		if (v10 == formId)
		{
			v9 = index; //item->value
			if (v9 != capacity)
				break;
		}
		else
		{
			if (!ErectusProcess::Rpm(entries + index * 24 + 16, &index, sizeof index)) //item = item->next
				return 0;
			if (index == capacity)
				break;
		}
	}

	if (v9 == capacity) return 0;

	return entries + v9 * 24 + 8; //item->value.second
}

DWORD64 ErectusMemory::GetPtr(const DWORD formId)
{
	const auto address = GetAddress(formId);
	if (!address)
		return 0;

	DWORD64 ptr;
	if (!ErectusProcess::Rpm(address, &ptr, sizeof ptr))
		return 0;

	return ptr;
}

DWORD64 ErectusMemory::GetCameraPtr()
{
	DWORD64 cameraPtr;
	if (!ErectusProcess::Rpm(ErectusProcess::exe + OFFSET_CAMERA, &cameraPtr, sizeof cameraPtr))
		return 0;

	return cameraPtr;
}

BYTE ErectusMemory::CheckHealthFlag(const BYTE healthFlag)
{
	auto flag = healthFlag;
	flag &= ~(1 << 7);
	flag &= ~(1 << 6);
	flag &= ~(1 << 5);
	switch (flag)
	{
	case 0x00: //Alive
		return 0x01;
	case 0x02: //Dead
	case 0x04: //Dead
		return 0x03;
	case 0x10: //Downed
	case 0x12: //Downed
		return 0x02;
	default: //Unknown
		return 0x00;
	}
}

DWORD64 ErectusMemory::GetLocalPlayerPtr(const bool checkMainMenu)
{
	DWORD64 localPlayerPtr;
	if (!ErectusProcess::Rpm(ErectusProcess::exe + OFFSET_LOCAL_PLAYER, &localPlayerPtr, sizeof localPlayerPtr))
		return 0;
	if (!Utils::Valid(localPlayerPtr))
		return 0;

	if (checkMainMenu)
	{
		TesObjectRefr localPlayerData{};
		if (!ErectusProcess::Rpm(localPlayerPtr, &localPlayerData, sizeof localPlayerData))
			return 0;
		if (localPlayerData.formId == 0x00000014)
			return 0;
	}

	return localPlayerPtr;
}

std::vector<DWORD64> ErectusMemory::GetEntityPtrList()
{
	std::vector<DWORD64> result;

	DWORD64 entityListTypePtr;
	if (!ErectusProcess::Rpm(ErectusProcess::exe + OFFSET_ENTITY_LIST, &entityListTypePtr, sizeof entityListTypePtr))
		return result;
	if (!Utils::Valid(entityListTypePtr))
		return result;

	//1) Get LoadedAreaManager
	LoadedAreaManager manager{};
	if (!ErectusProcess::Rpm(entityListTypePtr, &manager, sizeof manager))
		return result;
	if (!Utils::Valid(manager.interiorCellArrayBegin) || !Utils::Valid(manager.interiorCellArrayEnd) || !Utils::Valid(manager.exteriorCellArrayBegin) || !Utils::Valid(manager.exteriorCellArrayEnd))
		return result;

	DWORD64 cellPtrArrayPtr;
	int cellPtrArraySize;

	//2) Select  interior or exterior objectlist
	if (manager.interiorCellArrayBegin != manager.interiorCellArrayEnd)
	{
		cellPtrArrayPtr = manager.interiorCellArrayBegin;
		cellPtrArraySize = 2;
	}
	else if (manager.exteriorCellArrayBegin != manager.exteriorCellArrayEnd)
	{
		cellPtrArrayPtr = manager.exteriorCellArrayBegin;
		cellPtrArraySize = 50;
	}
	else return result; // sthg went wrong

	//3) Read the array of pointers to cells
	auto cellPtrArray = std::make_unique<DWORD64[]>(cellPtrArraySize);
	if (!ErectusProcess::Rpm(cellPtrArrayPtr, cellPtrArray.get(), cellPtrArraySize * sizeof DWORD64))
		return result;

	//4) Read each cell and push object pointers into objectPtrs
	//this is actually a linked list presenting as an array, odd entries are just pointers to 'next' element, so we skip them
	for (auto i = 0; i < cellPtrArraySize; i++)
	{
		if (i % 2 != 0)
			continue;

		TesObjectCell cell{};
		if (!ErectusProcess::Rpm(cellPtrArray[i], &cell, sizeof TesObjectCell))
			continue;
		if (cell.loadedState != 7) // attached == 7
			continue;
		if (!Utils::Valid(cell.objectListBeginPtr) || !Utils::Valid(cell.objectListEndPtr))
			continue;

		auto itemArraySize = (cell.objectListEndPtr - cell.objectListBeginPtr) / sizeof(DWORD64);
		auto objectPtrArray = std::make_unique<DWORD64[]>(itemArraySize);
		if (!ErectusProcess::Rpm(cell.objectListBeginPtr, objectPtrArray.get(), itemArraySize * sizeof DWORD64))
			continue;

		result.insert(result.end(), objectPtrArray.get(), objectPtrArray.get() + itemArraySize);
	}

	return  result;
}

bool ErectusMemory::IsRecipeKnown(const DWORD formId)
{
	//the list of known recipes is implemented as a set / rb-tree at [localplayer + 0xDB0]+0x8.
	struct SetEntry
	{
		DWORD64 left; //0x0000
		DWORD64  parent; //0x0008
		DWORD64  right; //0x0010
		char pad0018[1]; //0x0018
		BYTE isLeaf; //0x0019
		char pad001A[2]; //0x001A
		DWORD value; //0x001C
	} setEntry = {};

	auto localPlayerPtr = GetLocalPlayerPtr(true);
	if (!Utils::Valid(localPlayerPtr))
		return false;

	DWORD64 setPtr;
	if (!ErectusProcess::Rpm(localPlayerPtr + 0xDB0, &setPtr, sizeof setPtr))
		return false;

	if (!ErectusProcess::Rpm(setPtr + 0x8, &setPtr, sizeof setPtr))
		return false;

	if (!ErectusProcess::Rpm(setPtr, &setEntry, sizeof setEntry))
		return false;

	while (!setEntry.isLeaf)
	{
		if (setEntry.value == formId)
			return true;
		if (setEntry.value > formId)
		{
			if (!ErectusProcess::Rpm(setEntry.left, &setEntry, sizeof setEntry))
				return false;
		}
		else
		{
			if (!ErectusProcess::Rpm(setEntry.right, &setEntry, sizeof setEntry))
				return false;
		}
	}
	return false;
}

bool ErectusMemory::CheckFormIdArray(const DWORD formId, const bool* enabledArray, const DWORD* formIdArray, const int size)
{
	for (auto i = 0; i < size; i++)
	{
		if (formId == formIdArray[i])
			return enabledArray[i];
	}

	return false;
}

bool ErectusMemory::IsJunk(const TesItem& referenceData)
{
	if (referenceData.componentArraySize && !(referenceData.recordFlagA >> 7 & 1))
		return true;

	return false;
}

bool ErectusMemory::IsMod(const TesItem& referenceData)
{
	if (referenceData.recordFlagA >> 7 & 1)
		return true;

	return false;
}

bool ErectusMemory::IsPlan(const TesItem& referenceData)
{
	if (referenceData.planFlag >> 5 & 1)
		return true;

	return false;
}

bool ErectusMemory::CheckReferenceKeywordBook(const TesItem& referenceData, const DWORD formId)
{
	if (!referenceData.keywordArrayData01C0 || referenceData.keywordArrayData01C0 > 0x7FFF)
		return false;
	if (!Utils::Valid(referenceData.keywordArrayData01B8))
		return false;

	auto keywordArray = std::make_unique<DWORD64[]>(referenceData.keywordArrayData01C0);
	if (!ErectusProcess::Rpm(referenceData.keywordArrayData01B8, keywordArray.get(), referenceData.keywordArrayData01C0 * sizeof(DWORD64)))
		return false;

	for (DWORD64 i = 0; i < referenceData.keywordArrayData01C0; i++)
	{
		if (!Utils::Valid(keywordArray[i]))
			continue;

		DWORD formIdCheck;
		if (!ErectusProcess::Rpm(keywordArray[i] + 0x20, &formIdCheck, sizeof formIdCheck))
			continue;
		if (formIdCheck != formId)
			continue;

		return true;
	}

	return false;
}

bool ErectusMemory::CheckReferenceKeywordMisc(const TesItem& referenceData, const DWORD formId)
{
	if (!referenceData.keywordArrayData01B8 || referenceData.keywordArrayData01B8 > 0x7FFF)
		return false;
	if (!Utils::Valid(referenceData.keywordArrayData01B0))
		return false;

	auto keywordArray = std::make_unique<DWORD64[]>(referenceData.keywordArrayData01B8);
	if (!ErectusProcess::Rpm(referenceData.keywordArrayData01B0, keywordArray.get(), referenceData.keywordArrayData01B8 * sizeof(DWORD64)))
		return false;

	for (DWORD64 i = 0; i < referenceData.keywordArrayData01B8; i++)
	{
		if (!Utils::Valid(keywordArray[i]))
			continue;

		DWORD formIdCheck;
		if (!ErectusProcess::Rpm(keywordArray[i] + 0x20, &formIdCheck, sizeof formIdCheck))
			continue;
		if (formIdCheck != formId)
			continue;

		return true;
	}

	return false;
}

bool ErectusMemory::CheckWhitelistedFlux(const TesItem& referenceData)
{
	if (!Utils::Valid(referenceData.harvestedPtr))
		return false;

	DWORD formIdCheck;
	if (!ErectusProcess::Rpm(referenceData.harvestedPtr + 0x20, &formIdCheck, sizeof formIdCheck))
		return false;

	switch (formIdCheck)
	{
	case 0x002DDD45: //Raw Crimson Flux
		return Settings::esp.floraExt.crimsonFluxEnabled;
	case 0x002DDD46: //Raw Cobalt Flux
		return Settings::esp.floraExt.cobaltFluxEnabled;
	case 0x002DDD49: //Raw Yellowcake Flux
		return Settings::esp.floraExt.yellowcakeFluxEnabled;
	case 0x002DDD4B: //Raw Fluorescent Flux
		return Settings::esp.floraExt.fluorescentFluxEnabled;
	case 0x002DDD4D: //Raw Violet Flux
		return Settings::esp.floraExt.violetFluxEnabled;
	default:
		return false;
	}
}

bool ErectusMemory::IsTreasureMap(const TesItem& referenceData)
{
	return TREASUREMAP_FORMIDS.contains(referenceData.formId);
}

bool ErectusMemory::CheckReferenceItem(const TesItem& referenceData)
{
	switch (referenceData.formType)
	{
	case (static_cast<BYTE>(FormTypes::TesUtilityItem)):
	case (static_cast<BYTE>(FormTypes::BgsNote)):
	case (static_cast<BYTE>(FormTypes::TesKey)):
	case (static_cast<BYTE>(FormTypes::CurrencyObject)):
		return true;
	default:
		return false;
	}
}

bool ErectusMemory::IsBobblehead(const TesItem& tesItem)
{
	return CheckReferenceKeywordMisc(tesItem, 0x00135E6C);
}

bool ErectusMemory::IsMagazine(const TesItem& tesItem)
{
	return CheckReferenceKeywordBook(tesItem, 0x001D4A70);
}

ItemInfo ErectusMemory::GetItemInfo(const TesObjectRefr& entity, const TesItem& base)
{
	ItemInfo result = {};

	result.refr = entity;
	result.base = base;

	switch (base.formType)
	{
	case (static_cast<byte>(FormTypes::BgsIdleMarker)):
	case (static_cast<byte>(FormTypes::BgsStaticCollection)):
	case (static_cast<byte>(FormTypes::TesObjectLigh)):
	case (static_cast<byte>(FormTypes::TesObjectStat)):
	case (static_cast<byte>(FormTypes::BgsMovableStatic)):
	case (static_cast<byte>(FormTypes::TesSound)):
	case (static_cast<byte>(FormTypes::BgsTextureSet)):
	case (static_cast<byte>(FormTypes::BgsBendableSpline)):
	case (static_cast<byte>(FormTypes::BgsAcousticSpace)):
		result.type = ItemTypes::Invalid;
		break;

	case (static_cast<byte>(FormTypes::TesNpc)):
		result.namePtr = base.namePtr0160;
		result.type = ItemTypes::Npc;
		break;

	case (static_cast<byte>(FormTypes::TesObjectCont)):
		result.namePtr = base.namePtr00B0;
		result.type = ItemTypes::Container;
		break;

	case (static_cast<byte>(FormTypes::TesObjectMisc)):
		result.namePtr = base.namePtr0098;
		if (IsJunk(base))
			result.type = ItemTypes::Junk;
		else if (IsMod(base))
			result.type = ItemTypes::Mod;
		else if (IsBobblehead(base))
			result.type = ItemTypes::AidBobblehead;
		else
			result.type = ItemTypes::Misc;
		break;

	case (static_cast<byte>(FormTypes::TesObjectBook)):
		result.namePtr = base.namePtr0098;
		if (IsPlan(base))
		{
			if (IsRecipeKnown(base.formId))
				result.type = ItemTypes::NotesKnownPlan;
			else
				result.type = ItemTypes::NotesUnknownPlan;
		}
		else if (IsMagazine(base))
			result.type = ItemTypes::AidMagazine;
		else if (IsTreasureMap(base))
			result.type = ItemTypes::NotesTreasureMap;
		else
			result.type = ItemTypes::Notes;
		break;

	case (static_cast<byte>(FormTypes::TesFlora)):
		result.namePtr = base.namePtr0098;
		result.type = ItemTypes::Flora;
		break;

	case (static_cast<byte>(FormTypes::TesObjectWeap)):
		result.namePtr = base.namePtr0098;
		result.type = ItemTypes::Weapons;
		break;

	case (static_cast<byte>(FormTypes::TesObjectArmo)):
		result.namePtr = base.namePtr0098;
		result.type = ItemTypes::Apparel;
		break;

	case (static_cast<byte>(FormTypes::TesAmmo)):
		result.namePtr = base.namePtr0098;
		result.type = ItemTypes::Ammo;
		break;

	case (static_cast<byte>(FormTypes::AlchemyItem)):
		result.namePtr = base.namePtr0098;
		result.type = ItemTypes::Aid;
		break;

	case (static_cast<BYTE>(FormTypes::BgsNote)):
		result.namePtr = base.namePtr0098;
		result.type = ItemTypes::Notes;
		break;

	case (static_cast<BYTE>(FormTypes::TesUtilityItem)):
		result.namePtr = base.namePtr0098;
		result.type = ItemTypes::Aid;
		break;

	case (static_cast<BYTE>(FormTypes::TesKey)):
		result.namePtr = base.namePtr0098;
		result.type = ItemTypes::Misc;
		break;

	case (static_cast<BYTE>(FormTypes::CurrencyObject)):
		result.namePtr = base.namePtr0098;
		result.type = ItemTypes::Currency;
		break;

	default:
		result.type = ItemTypes::Other;
	}
	return result;
}

void ErectusMemory::GetCustomEntityData(const TesItem& referenceData, DWORD64* entityFlag, DWORD64* entityNamePtr, int* enabledDistance)
{
	if (auto found1 = Settings::esp.blacklist.find(referenceData.formId); found1 != Settings::esp.blacklist.end()) {
		if (found1->second) {
			*entityFlag |= CUSTOM_ENTRY_INVALID;
			return;
		}
	}
	
	if (auto found2 = Settings::esp.whitelist.find(referenceData.formId); found2 != Settings::esp.whitelist.end()) {
		if (found2->second)
			*entityFlag |= CUSTOM_ENTRY_WHITELISTED;
	}

	switch (referenceData.formType)
	{
	case (static_cast<byte>(FormTypes::BgsIdleMarker)):
	case (static_cast<byte>(FormTypes::BgsStaticCollection)):
	case (static_cast<byte>(FormTypes::TesObjectLigh)):
	case (static_cast<byte>(FormTypes::TesObjectStat)):
	case (static_cast<byte>(FormTypes::BgsMovableStatic)):
	case (static_cast<byte>(FormTypes::TesSound)):
	case (static_cast<byte>(FormTypes::BgsTextureSet)):
	case (static_cast<byte>(FormTypes::BgsBendableSpline)):
	case (static_cast<byte>(FormTypes::BgsAcousticSpace)):
		*entityFlag |= CUSTOM_ENTRY_INVALID;
		break;
	case (static_cast<byte>(FormTypes::TesNpc)):
		*entityFlag |= CUSTOM_ENTRY_NPC;
		*entityNamePtr = referenceData.namePtr0160;
		*enabledDistance = Settings::esp.npcs.enabledDistance;
		break;
	case (static_cast<byte>(FormTypes::TesObjectCont)):
		*entityFlag |= CUSTOM_ENTRY_CONTAINER;
		*entityNamePtr = referenceData.namePtr00B0;
		*enabledDistance = Settings::esp.containers.enabledDistance;
		if (!Settings::esp.containers.enabled && !(*entityFlag & CUSTOM_ENTRY_WHITELISTED))
			*entityFlag |= CUSTOM_ENTRY_INVALID;
		break;
	case (static_cast<byte>(FormTypes::TesObjectMisc)):
		*entityFlag |= CUSTOM_ENTRY_VALID_ITEM;
		*entityNamePtr = referenceData.namePtr0098;
		if (IsJunk(referenceData))
		{
			*entityFlag |= CUSTOM_ENTRY_JUNK;
			*enabledDistance = Settings::esp.junk.enabledDistance;
			if (!Settings::esp.junk.enabled && !(*entityFlag & CUSTOM_ENTRY_WHITELISTED))
				*entityFlag |= CUSTOM_ENTRY_INVALID;
		}
		else if (IsMod(referenceData))
		{
			*entityFlag |= CUSTOM_ENTRY_MOD;
			*entityFlag |= CUSTOM_ENTRY_ITEM;
			*enabledDistance = Settings::esp.items.enabledDistance;
			if (!Settings::esp.items.enabled)
				*entityFlag |= CUSTOM_ENTRY_INVALID;
		}
		else if (IsBobblehead(referenceData))
		{
			*entityFlag |= CUSTOM_ENTRY_BOBBLEHEAD;
			*enabledDistance = Settings::esp.bobbleheads.enabledDistance;
			if (!Settings::esp.bobbleheads.enabled && !(*entityFlag & CUSTOM_ENTRY_WHITELISTED))
				*entityFlag |= CUSTOM_ENTRY_INVALID;
		}
		else
		{
			*entityFlag |= CUSTOM_ENTRY_MISC;
			*entityFlag |= CUSTOM_ENTRY_ITEM;
			*enabledDistance = Settings::esp.items.enabledDistance;
			if (!Settings::esp.items.enabled)
				*entityFlag |= CUSTOM_ENTRY_INVALID;
		}
		break;
	case (static_cast<byte>(FormTypes::TesObjectBook)):
		*entityFlag |= CUSTOM_ENTRY_VALID_ITEM;
		*entityNamePtr = referenceData.namePtr0098;
		if (IsPlan(referenceData))
		{
			*entityFlag |= CUSTOM_ENTRY_PLAN;
			*enabledDistance = Settings::esp.plans.enabledDistance;

			if (IsRecipeKnown(referenceData.formId))
				*entityFlag |= CUSTOM_ENTRY_KNOWN_RECIPE;
			else
				*entityFlag |= CUSTOM_ENTRY_UNKNOWN_RECIPE;

			if (!Settings::esp.plans.enabled && !(*entityFlag & CUSTOM_ENTRY_WHITELISTED))
				*entityFlag |= CUSTOM_ENTRY_INVALID;
		}
		else if (IsMagazine(referenceData))
		{
			*entityFlag |= CUSTOM_ENTRY_MAGAZINE;
			*enabledDistance = Settings::esp.magazines.enabledDistance;
			if (!Settings::esp.magazines.enabled && !(*entityFlag & CUSTOM_ENTRY_WHITELISTED))
				*entityFlag |= CUSTOM_ENTRY_INVALID;
		}
		else
		{
			*entityFlag |= CUSTOM_ENTRY_ITEM;
			*enabledDistance = Settings::esp.items.enabledDistance;
			if (IsTreasureMap(referenceData))
				*entityFlag |= CUSTOM_ENTRY_TREASURE_MAP;
			if (!Settings::esp.items.enabled && !(*entityFlag & CUSTOM_ENTRY_WHITELISTED))
				*entityFlag |= CUSTOM_ENTRY_INVALID;
		}
		break;
	case (static_cast<byte>(FormTypes::TesFlora)):
		*entityFlag |= CUSTOM_ENTRY_FLORA;
		*entityNamePtr = referenceData.namePtr0098;
		*enabledDistance = Settings::esp.flora.enabledDistance;

		if (CheckWhitelistedFlux(referenceData))
			*entityFlag |= CUSTOM_ENTRY_WHITELISTED;
		if (!Settings::esp.flora.enabled && !(*entityFlag & CUSTOM_ENTRY_WHITELISTED))
			*entityFlag |= CUSTOM_ENTRY_INVALID;

		break;
	case (static_cast<byte>(FormTypes::TesObjectWeap)):
		*entityFlag |= CUSTOM_ENTRY_WEAPON;
		*entityFlag |= CUSTOM_ENTRY_ITEM;
		*entityFlag |= CUSTOM_ENTRY_VALID_ITEM;
		*entityNamePtr = referenceData.namePtr0098;
		*enabledDistance = Settings::esp.items.enabledDistance;
		if (!Settings::esp.items.enabled && !(*entityFlag & CUSTOM_ENTRY_WHITELISTED))
			*entityFlag |= CUSTOM_ENTRY_INVALID;
		break;
	case (static_cast<byte>(FormTypes::TesObjectArmo)):
		*entityFlag |= CUSTOM_ENTRY_ARMOR;
		*entityFlag |= CUSTOM_ENTRY_ITEM;
		*entityFlag |= CUSTOM_ENTRY_VALID_ITEM;
		*entityNamePtr = referenceData.namePtr0098;
		*enabledDistance = Settings::esp.items.enabledDistance;
		if (!Settings::esp.items.enabled && !(*entityFlag & CUSTOM_ENTRY_WHITELISTED))
			*entityFlag |= CUSTOM_ENTRY_INVALID;
		break;
	case (static_cast<byte>(FormTypes::TesAmmo)):
		*entityFlag |= CUSTOM_ENTRY_AMMO;
		*entityFlag |= CUSTOM_ENTRY_ITEM;
		*entityFlag |= CUSTOM_ENTRY_VALID_ITEM;
		*entityNamePtr = referenceData.namePtr0098;
		*enabledDistance = Settings::esp.items.enabledDistance;
		if (!Settings::esp.items.enabled && !(*entityFlag & CUSTOM_ENTRY_WHITELISTED))
			*entityFlag |= CUSTOM_ENTRY_INVALID;
		break;
	case (static_cast<byte>(FormTypes::AlchemyItem)):
		*entityFlag |= CUSTOM_ENTRY_AID;
		*entityFlag |= CUSTOM_ENTRY_ITEM;
		*entityFlag |= CUSTOM_ENTRY_VALID_ITEM;
		*entityNamePtr = referenceData.namePtr0098;
		*enabledDistance = Settings::esp.items.enabledDistance;
		if (!Settings::esp.items.enabled && !(*entityFlag & CUSTOM_ENTRY_WHITELISTED))
			*entityFlag |= CUSTOM_ENTRY_INVALID;
		break;
	default:
		if (CheckReferenceItem(referenceData))
		{
			*entityFlag |= CUSTOM_ENTRY_ITEM;
			*entityFlag |= CUSTOM_ENTRY_VALID_ITEM;
			*entityNamePtr = referenceData.namePtr0098;
			*enabledDistance = Settings::esp.items.enabledDistance;
			if (!Settings::esp.items.enabled && !(*entityFlag & CUSTOM_ENTRY_WHITELISTED))
				*entityFlag |= CUSTOM_ENTRY_INVALID;
		}
		else
		{
			*entityFlag |= CUSTOM_ENTRY_ENTITY;
			*entityNamePtr = 0;
			*enabledDistance = Settings::esp.entities.enabledDistance;
			if ((!Settings::esp.entities.enabled || !Settings::esp.entities.drawUnnamed) && !(*entityFlag & CUSTOM_ENTRY_WHITELISTED))
				*entityFlag |= CUSTOM_ENTRY_INVALID;
		}
		break;
	}
}

bool ErectusMemory::GetActorSnapshotComponentData(const TesObjectRefr& entityData, ActorSnapshotComponent* actorSnapshotComponentData)
{
	if (!Utils::Valid(entityData.actorCorePtr))
		return false;

	DWORD64 actorCoreBufferA;
	if (!ErectusProcess::Rpm(entityData.actorCorePtr + 0x70, &actorCoreBufferA, sizeof actorCoreBufferA))
		return false;
	if (!Utils::Valid(actorCoreBufferA))
		return false;

	DWORD64 actorCoreBufferB;
	if (!ErectusProcess::Rpm(actorCoreBufferA + 0x8, &actorCoreBufferB, sizeof actorCoreBufferB))
		return false;
	if (!Utils::Valid(actorCoreBufferB))
		return false;

	return ErectusProcess::Rpm(actorCoreBufferB, &*actorSnapshotComponentData, sizeof(ActorSnapshotComponent));
}

std::string ErectusMemory::GetEntityName(const DWORD64 ptr)
{
	std::string result{};

	if (!Utils::Valid(ptr))
		return result;

	auto nameLength = 0;
	auto namePtr = ptr;

	if (!ErectusProcess::Rpm(namePtr + 0x10, &nameLength, sizeof nameLength))
		return result;
	if (nameLength <= 0 || nameLength > 0x7FFF)
	{
		DWORD64 buffer;
		if (!ErectusProcess::Rpm(namePtr + 0x10, &buffer, sizeof buffer))
			return result;
		if (!Utils::Valid(buffer))
			return result;
		if (!ErectusProcess::Rpm(buffer + 0x10, &nameLength, sizeof nameLength))
			return result;
		if (nameLength <= 0 || nameLength > 0x7FFF)
			return result;
		namePtr = buffer;
	}

	const auto nameSize = nameLength + 1;
	auto name = std::make_unique<char[]>(nameSize);

	if (!ErectusProcess::Rpm(namePtr + 0x18, name.get(), nameSize))
		return result;

	result = name.get();
	return result;
}

bool ErectusMemory::UpdateBufferEntityList()
{
	std::vector<CustomEntry> entities{};

	auto localPlayerPtr = GetLocalPlayerPtr(true);
	if (!Utils::Valid(localPlayerPtr))
		return false;

	TesObjectRefr localPlayer{};
	if (!ErectusProcess::Rpm(localPlayerPtr, &localPlayer, sizeof localPlayer))
		return false;

	auto bufferList = GetEntityPtrList();
	if (bufferList.empty())
		return false;

	for (auto entityPtr : bufferList)
	{
		if (!Utils::Valid(entityPtr))
			continue;
		if (entityPtr == localPlayerPtr)
			continue;

		TesObjectRefr entityData{};
		if (!ErectusProcess::Rpm(entityPtr, &entityData, sizeof entityData))
			continue;
		if (!Utils::Valid(entityData.baseObjectPtr))
			continue;

		if (entityData.formType == static_cast<BYTE>(FormTypes::PlayerCharacter))
			continue;

		TesItem referenceData{};
		if (!ErectusProcess::Rpm(entityData.baseObjectPtr, &referenceData, sizeof referenceData))
			continue;

		auto entityFlag = CUSTOM_ENTRY_DEFAULT;
		DWORD64 entityNamePtr = 0;
		auto enabledDistance = 0;

		GetCustomEntityData(referenceData, &entityFlag, &entityNamePtr, &enabledDistance);
		if (entityFlag & CUSTOM_ENTRY_INVALID)
			continue;

		auto distance = Utils::GetDistance(entityData.position, localPlayer.position);
		auto normalDistance = static_cast<int>(distance * 0.01f);
		if (normalDistance > enabledDistance)
			continue;

		auto entityName = GetEntityName(entityNamePtr);
		if (entityName.empty())
		{
			entityFlag |= CUSTOM_ENTRY_UNNAMED;
			entityName = fmt::format("{0:X} [{1:X}]", entityData.formId, referenceData.formType);
		}

		CustomEntry entry{
			.entityPtr = entityPtr,
			.baseObjectPtr = entityData.baseObjectPtr,
			.entityFormId = entityData.formId,
			.baseObjectFormId = referenceData.formId,
			.flag = entityFlag,
			.name = entityName
		};

		entities.push_back(entry);
	}
	entityDataBuffer = entities;

	return entityDataBuffer.empty() ? false : true;
}

std::string ErectusMemory::GetPlayerName(const ClientAccount& clientAccountData)
{
	std::string result = {};
	if (!clientAccountData.nameLength)
		return result;

	const auto playerNameSize = clientAccountData.nameLength + 1;
	auto playerName = std::make_unique<char[]>(playerNameSize);

	if (clientAccountData.nameLength > 15)
	{
		DWORD64 buffer;
		memcpy(&buffer, clientAccountData.nameData, sizeof buffer);
		if (!Utils::Valid(buffer))
			return result;

		if (!ErectusProcess::Rpm(buffer, playerName.get(), playerNameSize))
			return result;
	}
	else
		memcpy(playerName.get(), clientAccountData.nameData, playerNameSize);

	if (Utils::GetTextLength(playerName.get()) != clientAccountData.nameLength)
		return result;

	result.assign(playerName.get());

	return result;
}

bool ErectusMemory::UpdateBufferPlayerList()
{
	std::vector<CustomEntry> players{};

	auto localPlayerPtr = GetLocalPlayerPtr(true);
	if (!Utils::Valid(localPlayerPtr))
		return false;

	DWORD64 falloutMainDataPtr;
	if (!ErectusProcess::Rpm(ErectusProcess::exe + OFFSET_MAIN, &falloutMainDataPtr, sizeof falloutMainDataPtr))
		return false;
	if (!Utils::Valid(falloutMainDataPtr))
		return false;

	FalloutMain falloutMainData{};
	if (!ErectusProcess::Rpm(falloutMainDataPtr, &falloutMainData, sizeof falloutMainData))
		return false;
	if (!Utils::Valid(falloutMainData.platformSessionManagerPtr))
		return false;

	PlatformSessionManager platformSessionManagerData{};
	if (!ErectusProcess::Rpm(falloutMainData.platformSessionManagerPtr, &platformSessionManagerData, sizeof platformSessionManagerData))
		return false;
	if (!Utils::Valid(platformSessionManagerData.clientAccountManagerPtr))
		return false;

	ClientAccountManager clientAccountManagerData{};
	if (!ErectusProcess::Rpm(platformSessionManagerData.clientAccountManagerPtr, &clientAccountManagerData, sizeof clientAccountManagerData))
		return false;
	if (!Utils::Valid(clientAccountManagerData.clientAccountArrayPtr))
		return false;

	auto clientAccountArraySize = 0;
	clientAccountArraySize += clientAccountManagerData.clientAccountArraySizeA;
	clientAccountArraySize += clientAccountManagerData.clientAccountArraySizeB;
	if (!clientAccountArraySize)
		return false;

	auto clientAccountArray = std::make_unique<DWORD64[]>(clientAccountArraySize);
	if (!ErectusProcess::Rpm(clientAccountManagerData.clientAccountArrayPtr, clientAccountArray.get(), clientAccountArraySize * sizeof(DWORD64)))
		return false;

	for (auto i = 0; i < clientAccountArraySize; i++)
	{
		if (!Utils::Valid(clientAccountArray[i]))
			continue;

		ClientAccountBuffer clientAccountBufferData{};
		if (!ErectusProcess::Rpm(clientAccountArray[i], &clientAccountBufferData, sizeof clientAccountBufferData))
			continue;
		if (!Utils::Valid(clientAccountBufferData.clientAccountPtr))
			continue;

		ClientAccount clientAccountData{};
		if (!ErectusProcess::Rpm(clientAccountBufferData.clientAccountPtr, &clientAccountData, sizeof clientAccountData))
			continue;
		if (clientAccountData.formId == 0xFFFFFFFF)
			continue;

		auto entityPtr = GetPtr(clientAccountData.formId);
		if (!Utils::Valid(entityPtr))
			continue;
		if (entityPtr == localPlayerPtr)
			continue;

		TesObjectRefr entityData{};
		if (!ErectusProcess::Rpm(entityPtr, &entityData, sizeof entityData))
			continue;
		if (!Utils::Valid(entityData.baseObjectPtr))
			continue;

		TesItem referenceData{};
		if (!ErectusProcess::Rpm(entityData.baseObjectPtr, &referenceData, sizeof referenceData))
			continue;
		if (referenceData.formId != 0x00000007)
			continue;

		auto entityFlag = CUSTOM_ENTRY_PLAYER;
		auto entityName = GetPlayerName(clientAccountData);
		if (entityName.empty())
		{
			entityFlag |= CUSTOM_ENTRY_UNNAMED;
			entityName = fmt::format("{:x}", entityData.formId);
		}

		CustomEntry entry{
			.entityPtr = entityPtr,
			.baseObjectPtr = entityData.baseObjectPtr,
			.entityFormId = entityData.formId,
			.baseObjectFormId = referenceData.formId,
			.flag = entityFlag,
			.name = GetPlayerName(clientAccountData),
		};
		players.push_back(entry);
	}
	playerDataBuffer = players;

	return playerDataBuffer.empty() ? false : true;
}
bool ErectusMemory::IsTargetValid(const DWORD64 targetPtr)
{
	TesObjectRefr target{};
	if (!ErectusProcess::Rpm(targetPtr, &target, sizeof target))
		return false;

	return IsTargetValid(target);
}

bool ErectusMemory::IsTargetValid(const TesObjectRefr& targetData)
{
	if (targetData.formType != static_cast<BYTE>(FormTypes::TesActor))
		return false;

	if (targetData.spawnFlag != 0x02)
		return false;

	if (Settings::targetting.ignoreEssentialNpcs)
	{
		ActorSnapshotComponent actorSnapshotComponentData{};
		if (GetActorSnapshotComponentData(targetData, &actorSnapshotComponentData))
		{
			if (actorSnapshotComponentData.isEssential)
				return false;
		}
	}
	
	switch (CheckHealthFlag(targetData.healthFlag))
	{
	case 0x01: //Alive
		return true;
	case 0x02: //Downed
	case 0x03: //Dead
		return false;
	default: //Unknown
		return Settings::targetting.targetUnknown;
	}
}

bool ErectusMemory::IsFloraHarvested(const BYTE harvestFlagA, const BYTE harvestFlagB)
{
	return harvestFlagA >> 5 & 1 || harvestFlagB >> 5 & 1;
}

void ErectusMemory::DeleteOldWeaponList()
{
	if (oldWeaponList != nullptr)
	{
		if (oldWeaponListSize)
		{
			for (auto i = 0; i < oldWeaponListSize; i++)
			{
				if (oldWeaponList[i].weaponData != nullptr)
				{
					delete[]oldWeaponList[i].weaponData;
					oldWeaponList[i].weaponData = nullptr;
				}

				if (oldWeaponList[i].aimModelData != nullptr)
				{
					delete[]oldWeaponList[i].aimModelData;
					oldWeaponList[i].aimModelData = nullptr;
				}
			}
		}

		delete[]oldWeaponList;
		oldWeaponList = nullptr;
	}

	oldWeaponListSize = 0;
	oldWeaponListUpdated = false;
}

bool ErectusMemory::UpdateOldWeaponData()
{
	DWORD64 dataHandlerPtr;
	if (!ErectusProcess::Rpm(ErectusProcess::exe + OFFSET_DATA_HANDLER, &dataHandlerPtr, sizeof dataHandlerPtr))
		return false;
	if (!Utils::Valid(dataHandlerPtr))
		return false;

	ReferenceList weaponList{};
	if (!ErectusProcess::Rpm(dataHandlerPtr + 0x598, &weaponList, sizeof weaponList))
		return false;
	if (!Utils::Valid(weaponList.arrayPtr) || !weaponList.arraySize || weaponList.arraySize > 0x7FFF)
		return false;

	auto weaponPtrArray = std::make_unique<DWORD64[]>(weaponList.arraySize);
	if (!ErectusProcess::Rpm(weaponList.arrayPtr, weaponPtrArray.get(), weaponList.arraySize * sizeof(DWORD64)))
		return false;

	oldWeaponList = new OldWeapon[weaponList.arraySize];
	oldWeaponListSize = weaponList.arraySize;

	for (auto i = 0; i < weaponList.arraySize; i++)
	{
		oldWeaponList[i].weaponData = nullptr;
		oldWeaponList[i].aimModelData = nullptr;

		if (!Utils::Valid(weaponPtrArray[i]))
			continue;

		Weapon weaponData{};
		if (!ErectusProcess::Rpm(weaponPtrArray[i], &weaponData, sizeof weaponData))
			continue;

		oldWeaponList[i].weaponData = new Weapon;
		memcpy(&*oldWeaponList[i].weaponData, &weaponData, sizeof weaponData);

		if (!Utils::Valid(weaponData.aimModelPtr))
			continue;

		AimModel aimModelData{};
		if (!ErectusProcess::Rpm(weaponData.aimModelPtr, &aimModelData, sizeof aimModelData))
			continue;

		oldWeaponList[i].aimModelData = new AimModel;
		memcpy(&*oldWeaponList[i].aimModelData, &aimModelData, sizeof aimModelData);
	}

	return true;
}

int ErectusMemory::GetOldWeaponIndex(const DWORD formId)
{
	for (auto i = 0; i < oldWeaponListSize; i++)
	{
		if (oldWeaponList[i].weaponData != nullptr && oldWeaponList[i].weaponData->formId == formId)
			return i;
	}

	return -1;
}

bool ErectusMemory::WeaponEditingEnabled()
{
	auto bufferSettings = Settings::weapons;
	bufferSettings.capacity = Settings::defaultWeaponSettings.capacity;
	bufferSettings.speed = Settings::defaultWeaponSettings.speed;
	bufferSettings.reach = Settings::defaultWeaponSettings.reach;
	if (!memcmp(&bufferSettings, &Settings::defaultWeaponSettings, sizeof(WeaponEditorSettings)))
		return false;

	return true;
}

bool ErectusMemory::EditWeapon(const int index, const bool revertWeaponData)
{
	DWORD64 dataHandlerPtr;
	if (!ErectusProcess::Rpm(ErectusProcess::exe + OFFSET_DATA_HANDLER, &dataHandlerPtr, sizeof dataHandlerPtr))
		return false;
	if (!Utils::Valid(dataHandlerPtr))
		return false;

	ReferenceList weaponList{};
	if (!ErectusProcess::Rpm(dataHandlerPtr + 0x598, &weaponList, sizeof weaponList))
		return false;
	if (!Utils::Valid(weaponList.arrayPtr) || !weaponList.arraySize || weaponList.arraySize > 0x7FFF)
		return false;

	DWORD64 weaponPtr;
	if (!ErectusProcess::Rpm(weaponList.arrayPtr + index * sizeof(DWORD64), &weaponPtr, sizeof weaponPtr))
		return false;
	if (!Utils::Valid(weaponPtr)) return false;

	Weapon weaponData{};
	if (!ErectusProcess::Rpm(weaponPtr, &weaponData, sizeof weaponData))
		return false;
	if (oldWeaponList[index].weaponData == nullptr)
		return false;

	auto currentWeaponIndex = index;
	if (oldWeaponList[currentWeaponIndex].weaponData->formId != weaponData.formId)
	{
		const auto bufferIndex = GetOldWeaponIndex(weaponData.formId);
		if (bufferIndex == -1)
			return false;
		currentWeaponIndex = bufferIndex;
	}

	auto writeWeaponData = false;
	auto writeAimModelData = false;

	if (!revertWeaponData && Settings::weapons.instantReload)
	{
		if (weaponData.reloadSpeed != 100.0f)
		{
			weaponData.reloadSpeed = 100.0f;
			writeWeaponData = true;
		}
	}
	else
	{
		if (weaponData.reloadSpeed != oldWeaponList[currentWeaponIndex].weaponData->reloadSpeed)
		{
			weaponData.reloadSpeed = oldWeaponList[currentWeaponIndex].weaponData->reloadSpeed;
			writeWeaponData = true;
		}
	}

	if (!revertWeaponData && Settings::weapons.automaticflag)
	{
		if (!(weaponData.flagB >> 7 & 1))
		{
			weaponData.flagB |= 1 << 7;
			writeWeaponData = true;
		}
	}
	else
	{
		if (weaponData.flagB != oldWeaponList[currentWeaponIndex].weaponData->flagB)
		{
			weaponData.flagB = oldWeaponList[currentWeaponIndex].weaponData->flagB;
			writeWeaponData = true;
		}
	}

	if (!revertWeaponData && Settings::weapons.capacityEnabled)
	{
		if (weaponData.capacity != static_cast<short>(Settings::weapons.capacity))
		{
			weaponData.capacity = static_cast<short>(Settings::weapons.capacity);
			writeWeaponData = true;
		}
	}
	else
	{
		if (weaponData.capacity != oldWeaponList[currentWeaponIndex].weaponData->capacity)
		{
			weaponData.capacity = oldWeaponList[currentWeaponIndex].weaponData->capacity;
			writeWeaponData = true;
		}
	}

	if (!revertWeaponData && Settings::weapons.speedEnabled)
	{
		if (weaponData.speed != Settings::weapons.speed)
		{
			weaponData.speed = Settings::weapons.speed;
			writeWeaponData = true;
		}
	}
	else
	{
		if (weaponData.speed != oldWeaponList[currentWeaponIndex].weaponData->speed)
		{
			weaponData.speed = oldWeaponList[currentWeaponIndex].weaponData->speed;
			writeWeaponData = true;
		}
	}

	if (!revertWeaponData && Settings::weapons.reachEnabled)
	{
		if (weaponData.reach != Settings::weapons.reach)
		{
			weaponData.reach = Settings::weapons.reach;
			writeWeaponData = true;
		}
	}
	else
	{
		if (weaponData.reach != oldWeaponList[currentWeaponIndex].weaponData->reach)
		{
			weaponData.reach = oldWeaponList[currentWeaponIndex].weaponData->reach;
			writeWeaponData = true;
		}
	}

	if (writeWeaponData)
		ErectusProcess::Wpm(weaponPtr, &weaponData, sizeof weaponData);

	if (!Utils::Valid(weaponData.aimModelPtr))
		return true;

	if (oldWeaponList[currentWeaponIndex].aimModelData == nullptr)
		return false;

	AimModel aimModelData{};
	if (!ErectusProcess::Rpm(weaponData.aimModelPtr, &aimModelData, sizeof aimModelData))
		return false;

	BYTE noRecoil[sizeof AimModel::recoilData];
	memset(noRecoil, 0x00, sizeof noRecoil);

	BYTE noSpread[sizeof AimModel::spreadData];
	memset(noSpread, 0x00, sizeof noSpread);

	if (!revertWeaponData && Settings::weapons.noRecoil)
	{
		if (memcmp(aimModelData.recoilData, noRecoil, sizeof AimModel::recoilData) != 0)
		{
			memcpy(aimModelData.recoilData, noRecoil, sizeof AimModel::recoilData);
			writeAimModelData = true;
		}
	}
	else
	{
		if (memcmp(aimModelData.recoilData, oldWeaponList[currentWeaponIndex].aimModelData->recoilData, sizeof AimModel::recoilData) != 0)
		{
			memcpy(aimModelData.recoilData, oldWeaponList[currentWeaponIndex].aimModelData->recoilData, sizeof AimModel::recoilData);
			writeAimModelData = true;
		}
	}

	if (!revertWeaponData && Settings::weapons.noSpread)
	{
		if (memcmp(aimModelData.spreadData, noSpread, sizeof AimModel::spreadData) != 0)
		{
			memcpy(aimModelData.spreadData, noSpread, sizeof AimModel::spreadData);
			writeAimModelData = true;
		}
	}
	else
	{
		if (memcmp(aimModelData.spreadData, oldWeaponList[currentWeaponIndex].aimModelData->spreadData, sizeof AimModel::spreadData) != 0)
		{
			memcpy(aimModelData.spreadData, oldWeaponList[currentWeaponIndex].aimModelData->spreadData, sizeof AimModel::spreadData);
			writeAimModelData = true;
		}
	}

	if (!revertWeaponData && Settings::weapons.noSway)
	{
		if (aimModelData.sway != 100.0f)
		{
			aimModelData.sway = 100.0f;
			writeAimModelData = true;
		}
	}
	else
	{
		if (aimModelData.sway != oldWeaponList[currentWeaponIndex].aimModelData->sway)
		{
			aimModelData.sway = oldWeaponList[currentWeaponIndex].aimModelData->sway;
			writeAimModelData = true;
		}
	}

	if (writeAimModelData)
		return ErectusProcess::Wpm(weaponData.aimModelPtr, &aimModelData, sizeof aimModelData);

	return true;
}

bool ErectusMemory::InfiniteAmmo(const bool state)
{
	BYTE infiniteAmmoOn[] = { 0x66, 0xB8, 0xE7, 0x03 };
	BYTE infiniteAmmoOff[] = { 0x8B, 0x44, 0x24, 0x50 };
	BYTE infiniteAmmoCheck[sizeof infiniteAmmoOff];

	if (!ErectusProcess::Rpm(ErectusProcess::exe + OFFSET_INFINITE_AMMO, &infiniteAmmoCheck, sizeof infiniteAmmoCheck))
		return false;

	if (state && !memcmp(infiniteAmmoCheck, infiniteAmmoOff, sizeof infiniteAmmoOff))
		return ErectusProcess::Wpm(ErectusProcess::exe + OFFSET_INFINITE_AMMO, &infiniteAmmoOn, sizeof infiniteAmmoOn);
	if (!state && !memcmp(infiniteAmmoCheck, infiniteAmmoOn, sizeof infiniteAmmoOn))
		return ErectusProcess::Wpm(ErectusProcess::exe + OFFSET_INFINITE_AMMO, &infiniteAmmoOff, sizeof infiniteAmmoOff);
	return false;
}

bool ErectusMemory::DamageRedirection(const DWORD64 targetPtr, DWORD64* targetingPage, bool* targetingPageValid, const bool isExiting, const bool state)
{
	BYTE pageJmpOn[] = { 0x48, 0xBB, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xE3 };
	BYTE pageJmpOff[] = { 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC };
	BYTE pageJmpCheck[sizeof pageJmpOff];

	BYTE redirectionOn[] = { 0xE9, 0x8D, 0xFE, 0xFF, 0xFF };
	BYTE redirectionOff[] = { 0x48, 0x8B, 0x5C, 0x24, 0x50 };
	BYTE redirectionCheck[sizeof redirectionOff];

	if (!ErectusProcess::Rpm(ErectusProcess::exe + OFFSET_REDIRECTION_JMP, &pageJmpCheck, sizeof pageJmpCheck))
		return false;

	DWORD64 pageCheck;
	memcpy(&pageCheck, &pageJmpCheck[2], sizeof pageCheck);
	if (Utils::Valid(pageCheck) && pageCheck != *targetingPage)
	{
		BYTE pageOpcode[] = { 0x48, 0xBB, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x00 };
		BYTE pageOpcodeCheck[sizeof pageOpcode];
		if (!ErectusProcess::Rpm(pageCheck, &pageOpcodeCheck, sizeof pageOpcodeCheck))
			return false;
		if (memcmp(pageOpcodeCheck, pageOpcode, sizeof pageOpcode) != 0)
			return false;
		if (!ErectusProcess::Wpm(ErectusProcess::exe + OFFSET_REDIRECTION_JMP, &pageJmpOff, sizeof pageJmpOff))
			return false;
		if (!ErectusProcess::FreeEx(pageCheck))
			return false;
	}

	if (!*targetingPage)
	{
		const auto page = ErectusProcess::AllocEx(sizeof(Opk));
		if (!page)
			return false;
		*targetingPage = page;
	}

	if (!*targetingPageValid)
	{
		TargetLocking targetLockingData;
		targetLockingData.targetLockingPtr = targetPtr;
		auto originalFunction = ErectusProcess::exe + OFFSET_REDIRECTION + sizeof redirectionOff;
		DWORD64 originalFunctionCheck;
		if (!ErectusProcess::Rpm(*targetingPage + 0x30, &originalFunctionCheck, sizeof originalFunctionCheck))
			return false;

		if (originalFunctionCheck != originalFunction)
			memcpy(&targetLockingData.redirectionAsm[0x30], &originalFunction, sizeof originalFunction);

		if (!ErectusProcess::Wpm(*targetingPage, &targetLockingData, sizeof targetLockingData))
			return false;

		*targetingPageValid = true;
		return false;
	}
	TargetLocking targetLockingData;
	if (!ErectusProcess::Rpm(*targetingPage, &targetLockingData, sizeof targetLockingData))
		return false;

	if (targetLockingData.targetLockingPtr != targetPtr)
	{
		targetLockingData.targetLockingPtr = targetPtr;
		if (!ErectusProcess::Wpm(*targetingPage, &targetLockingData, sizeof targetLockingData))
			return false;
		memcpy(&pageJmpOn[2], &*targetingPage, sizeof(DWORD64));
	}
	memcpy(&pageJmpOn[2], &*targetingPage, sizeof(DWORD64));

	const auto redirection = ErectusProcess::Rpm(ErectusProcess::exe + OFFSET_REDIRECTION, &redirectionCheck,
		sizeof redirectionCheck);

	if (*targetingPageValid && state && IsTargetValid(targetPtr))
	{
		if (redirection && !memcmp(redirectionCheck, redirectionOff, sizeof redirectionOff))
			ErectusProcess::Wpm(ErectusProcess::exe + OFFSET_REDIRECTION, &redirectionOn, sizeof redirectionOn);
	}
	else
	{
		if (redirection && !memcmp(redirectionCheck, redirectionOn, sizeof redirectionOn))
			ErectusProcess::Wpm(ErectusProcess::exe + OFFSET_REDIRECTION, &redirectionOff, sizeof redirectionOff);
	}

	if (*targetingPageValid && !isExiting && !memcmp(pageJmpCheck, pageJmpOff, sizeof pageJmpOff))
		return ErectusProcess::Wpm(ErectusProcess::exe + OFFSET_REDIRECTION_JMP, &pageJmpOn, sizeof pageJmpOn);
	if (isExiting && !memcmp(pageJmpCheck, pageJmpOn, sizeof pageJmpOn))
		return ErectusProcess::Wpm(ErectusProcess::exe + OFFSET_REDIRECTION_JMP, &pageJmpOff, sizeof pageJmpOff);
	return true;
}

bool ErectusMemory::MovePlayer()
{
	DWORD64 bhkCharProxyControllerPtr;
	if (!ErectusProcess::Rpm(ErectusProcess::exe + OFFSET_CHAR_CONTROLLER, &bhkCharProxyControllerPtr, sizeof bhkCharProxyControllerPtr))
		return false;
	if (!Utils::Valid(bhkCharProxyControllerPtr))
		return false;

	BhkCharProxyController bhkCharProxyControllerData{};
	if (!ErectusProcess::Rpm(bhkCharProxyControllerPtr, &bhkCharProxyControllerData, sizeof bhkCharProxyControllerData))
		return false;
	if (!Utils::Valid(bhkCharProxyControllerData.hknpBsCharacterProxyPtr))
		return false;

	HknpBsCharacterProxy hknpBsCharacterProxyData{};
	if (!ErectusProcess::Rpm(bhkCharProxyControllerData.hknpBsCharacterProxyPtr, &hknpBsCharacterProxyData, sizeof hknpBsCharacterProxyData))
		return false;


	float velocityA[4];
	memset(velocityA, 0x00, sizeof velocityA);

	float velocityB[4];
	memset(velocityB, 0x00, sizeof velocityB);

	auto speed = Settings::localPlayer.noclipSpeed;
	if (GetAsyncKeyState(VK_SHIFT))
		speed *= 1.5f;

	auto writePosition = false;
	auto cameraData = GetCameraInfo();

	if (GetAsyncKeyState('W'))
	{
		hknpBsCharacterProxyData.position[0] += cameraData.forward[0] * speed;
		hknpBsCharacterProxyData.position[1] += cameraData.forward[1] * speed;
		hknpBsCharacterProxyData.position[2] += cameraData.forward[2] * speed;
		writePosition = true;
	}

	if (GetAsyncKeyState('A'))
	{
		hknpBsCharacterProxyData.position[0] -= cameraData.forward[1] * speed;
		hknpBsCharacterProxyData.position[1] += cameraData.forward[0] * speed;
		writePosition = true;
	}

	if (GetAsyncKeyState('S'))
	{
		hknpBsCharacterProxyData.position[0] -= cameraData.forward[0] * speed;
		hknpBsCharacterProxyData.position[1] -= cameraData.forward[1] * speed;
		hknpBsCharacterProxyData.position[2] -= cameraData.forward[2] * speed;
		writePosition = true;
	}

	if (GetAsyncKeyState('D'))
	{
		hknpBsCharacterProxyData.position[0] += cameraData.forward[1] * speed;
		hknpBsCharacterProxyData.position[1] -= cameraData.forward[0] * speed;
		writePosition = true;
	}

	if (memcmp(hknpBsCharacterProxyData.velocityA, velocityA, sizeof velocityA) != 0)
	{
		memcpy(hknpBsCharacterProxyData.velocityA, velocityA, sizeof velocityA);
		writePosition = true;
	}

	if (memcmp(hknpBsCharacterProxyData.velocityB, velocityB, sizeof velocityB) != 0)
	{
		memcpy(hknpBsCharacterProxyData.velocityB, velocityB, sizeof velocityB);
		writePosition = true;
	}

	if (writePosition)
		return ErectusProcess::Wpm(bhkCharProxyControllerData.hknpBsCharacterProxyPtr, &hknpBsCharacterProxyData, sizeof hknpBsCharacterProxyData);

	return true;
}

void ErectusMemory::Noclip(const bool state)
{
	BYTE noclipOnA[] = { 0x0F, 0x1F, 0x44, 0x00, 0x00 };
	BYTE noclipOffA[] = { 0xE8, 0xC3, 0xC5, 0xFE, 0xFF };
	BYTE noclipCheckA[sizeof noclipOffA];

	BYTE noclipOnB[] = { 0x0F, 0x1F, 0x40, 0x00 };
	BYTE noclipOffB[] = { 0x41, 0xFF, 0x50, 0x40 };
	BYTE noclipCheckB[sizeof noclipOffB];

	BYTE noclipOnC[] = { 0x0F, 0x1F, 0x44, 0x00, 0x00 };
	BYTE noclipOffC[] = { 0xE8, 0x9A, 0xA1, 0x34, 0x01 };
	BYTE noclipCheckC[sizeof noclipOffC];

	BYTE noclipOnD[] = { 0x66, 0x0F, 0x1F, 0x44, 0x00, 0x00 };
	BYTE noclipOffD[] = { 0xFF, 0x15, 0x59, 0xEC, 0xFF, 0x01 };
	BYTE noclipCheckD[sizeof noclipOffD];

	const auto noclipA = ErectusProcess::Rpm(ErectusProcess::exe + OFFSET_NOCLIP_A, &noclipCheckA, sizeof noclipCheckA);
	const auto noclipB = ErectusProcess::Rpm(ErectusProcess::exe + OFFSET_NOCLIP_B, &noclipCheckB, sizeof noclipCheckB);
	const auto noclipC = ErectusProcess::Rpm(ErectusProcess::exe + OFFSET_NOCLIP_C, &noclipCheckC, sizeof noclipCheckC);
	const auto noclipD = ErectusProcess::Rpm(ErectusProcess::exe + OFFSET_NOCLIP_D, &noclipCheckD, sizeof noclipCheckD);

	if (state)
	{
		if (noclipA && !memcmp(noclipCheckA, noclipOffA, sizeof noclipOffA))
			ErectusProcess::Wpm(ErectusProcess::exe + OFFSET_NOCLIP_A, &noclipOnA, sizeof noclipOnA);

		if (noclipB && !memcmp(noclipCheckB, noclipOffB, sizeof noclipOffB))
			ErectusProcess::Wpm(ErectusProcess::exe + OFFSET_NOCLIP_B, &noclipOnB, sizeof noclipOnB);

		if (noclipC && !memcmp(noclipCheckC, noclipOffC, sizeof noclipOffC))
			ErectusProcess::Wpm(ErectusProcess::exe + OFFSET_NOCLIP_C, &noclipOnC, sizeof noclipOnC);

		if (noclipD && !memcmp(noclipCheckD, noclipOffD, sizeof noclipOffD))
			ErectusProcess::Wpm(ErectusProcess::exe + OFFSET_NOCLIP_D, &noclipOnD, sizeof noclipOnD);

		MovePlayer();
	}
	else
	{
		if (noclipA && !memcmp(noclipCheckA, noclipOnA, sizeof noclipOnA))
			ErectusProcess::Wpm(ErectusProcess::exe + OFFSET_NOCLIP_A, &noclipOffA, sizeof noclipOffA);

		if (noclipB && !memcmp(noclipCheckB, noclipOnB, sizeof noclipOnB))
			ErectusProcess::Wpm(ErectusProcess::exe + OFFSET_NOCLIP_B, &noclipOffB, sizeof noclipOffB);

		if (noclipC && !memcmp(noclipCheckC, noclipOnC, sizeof noclipOnC))
			ErectusProcess::Wpm(ErectusProcess::exe + OFFSET_NOCLIP_C, &noclipOffC, sizeof noclipOffC);

		if (noclipD && !memcmp(noclipCheckD, noclipOnD, sizeof noclipOnD))
			ErectusProcess::Wpm(ErectusProcess::exe + OFFSET_NOCLIP_D, &noclipOffD, sizeof noclipOffD);
	}
}

bool ErectusMemory::ActorValue(DWORD64* actorValuePage, bool* actorValuePageValid, const bool state)
{
	if (!*actorValuePage)
	{
		const auto page = ErectusProcess::AllocEx(sizeof(ActorValueHook));
		if (!page) return false;
		*actorValuePage = page;
	}

	const auto localPlayerPtr = GetLocalPlayerPtr(false);
	if (!Utils::Valid(localPlayerPtr))
		return false;

	TesObjectRefr localPlayer{};
	if (!ErectusProcess::Rpm(localPlayerPtr, &localPlayer, sizeof localPlayer))
		return false;
	if (!Utils::Valid(localPlayer.vtable0050))
		return false;

	DWORD64 actorValueFunction;
	if (!ErectusProcess::Rpm(localPlayer.vtable0050 + 0x48, &actorValueFunction, sizeof actorValueFunction))
		return false;
	if (!Utils::Valid(actorValueFunction))
		return false;

	ActorValueHook actorValueHookData;
	actorValueHookData.actionPointsEnabled = static_cast<int>(Settings::localPlayer.actionPointsEnabled);
	actorValueHookData.actionPoints = static_cast<float>(Settings::localPlayer.actionPoints);
	actorValueHookData.strengthEnabled = static_cast<int>(Settings::localPlayer.strengthEnabled);
	actorValueHookData.strength = static_cast<float>(Settings::localPlayer.strength);
	actorValueHookData.perceptionEnabled = static_cast<int>(Settings::localPlayer.perceptionEnabled);
	actorValueHookData.perception = static_cast<float>(Settings::localPlayer.perception);
	actorValueHookData.enduranceEnabled = static_cast<int>(Settings::localPlayer.enduranceEnabled);
	actorValueHookData.endurance = static_cast<float>(Settings::localPlayer.endurance);
	actorValueHookData.charismaEnabled = static_cast<int>(Settings::localPlayer.charismaEnabled);
	actorValueHookData.charisma = static_cast<float>(Settings::localPlayer.charisma);
	actorValueHookData.intelligenceEnabled = static_cast<int>(Settings::localPlayer.intelligenceEnabled);
	actorValueHookData.intelligence = static_cast<float>(Settings::localPlayer.intelligence);
	actorValueHookData.agilityEnabled = static_cast<int>(Settings::localPlayer.agilityEnabled);
	actorValueHookData.agility = static_cast<float>(Settings::localPlayer.agility);
	actorValueHookData.luckEnabled = static_cast<int>(Settings::localPlayer.luckEnabled);
	actorValueHookData.luck = static_cast<float>(Settings::localPlayer.luck);
	actorValueHookData.originalFunction = ErectusProcess::exe + OFFSET_ACTOR_VALUE;

	if (actorValueFunction != ErectusProcess::exe + OFFSET_ACTOR_VALUE && actorValueFunction != *actorValuePage)
	{
		if (VtableSwap(localPlayer.vtable0050 + 0x48, ErectusProcess::exe + OFFSET_ACTOR_VALUE))
			ErectusProcess::FreeEx(actorValueFunction);
	}

	if (state)
	{
		if (*actorValuePageValid)
		{
			ActorValueHook actorValueHookCheck;
			if (!ErectusProcess::Rpm(*actorValuePage, &actorValueHookCheck, sizeof actorValueHookCheck))
				return false;
			if (memcmp(&actorValueHookData, &actorValueHookCheck, sizeof actorValueHookCheck) != 0)
				return ErectusProcess::Wpm(*actorValuePage, &actorValueHookData, sizeof actorValueHookData);
		}
		else
		{
			if (!ErectusProcess::Wpm(*actorValuePage, &actorValueHookData, sizeof actorValueHookData))
				return false;
			if (!VtableSwap(localPlayer.vtable0050 + 0x48, *actorValuePage))
				return false;
			*actorValuePageValid = true;
		}
	}
	else
	{
		if (actorValueFunction != ErectusProcess::exe + OFFSET_ACTOR_VALUE)
		{
			if (VtableSwap(localPlayer.vtable0050 + 0x48, ErectusProcess::exe + OFFSET_ACTOR_VALUE))
				ErectusProcess::FreeEx(actorValueFunction);
		}

		if (*actorValuePage)
		{
			if (ErectusProcess::FreeEx(*actorValuePage))
			{
				*actorValuePage = 0;
				*actorValuePageValid = false;
			}
		}
	}

	return true;
}

bool ErectusMemory::SetActorValueMaximum(const DWORD formId, const float defaultValue, const float customValue, const bool state)
{
	const auto actorValuePtr = GetPtr(formId);
	if (!Utils::Valid(actorValuePtr))
		return false;

	ActorValueInformation actorValueData{};
	if (!ErectusProcess::Rpm(actorValuePtr, &actorValueData, sizeof actorValueData))
		return false;

	if (state)
	{
		if (actorValueData.maximumValue != customValue)
		{
			actorValueData.maximumValue = customValue;
			return ErectusProcess::Wpm(actorValuePtr, &actorValueData, sizeof actorValueData);
		}
	}
	else
	{
		if (actorValueData.maximumValue != defaultValue)
		{
			actorValueData.maximumValue = defaultValue;
			return ErectusProcess::Wpm(actorValuePtr, &actorValueData, sizeof actorValueData);
		}
	}

	return true;
}

bool ErectusMemory::OnePositionKill(DWORD64* opkPage, bool* opkPageValid, const bool state)
{
	if (!*opkPage)
	{
		const auto page = ErectusProcess::AllocEx(sizeof(Opk));
		if (!page)
			return false;
		*opkPage = page;
	}

	BYTE opkOn[] = { 0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xE0, 0xCC, 0xCC, 0xCC };
	BYTE opkOff[] = { 0x0F, 0x10, 0x87, 0x90, 0x04, 0x00, 0x00, 0x0F, 0x58, 0x45, 0xA7, 0x0F, 0x29, 0x45, 0xF7 };
	BYTE opkCheck[sizeof opkOff];

	if (!ErectusProcess::Rpm(ErectusProcess::exe + OFFSET_OPK, &opkCheck, sizeof opkCheck))
		return false;

	const auto originalFunction = ErectusProcess::exe + OFFSET_OPK + sizeof opkOff;
	memcpy(&opkOn[2], &*opkPage, sizeof(DWORD64));

	DWORD64 pageCheck;
	memcpy(&pageCheck, &opkCheck[2], sizeof(DWORD64));

	if (Utils::Valid(pageCheck) && pageCheck != *opkPage)
	{
		Opk buffer;
		if (!ErectusProcess::Rpm(pageCheck, &buffer, sizeof buffer))
			return false;
		if (buffer.originalFunction != originalFunction)
			return false;
		if (!ErectusProcess::Wpm(ErectusProcess::exe + OFFSET_OPK, &opkOff, sizeof opkOff))
			return false;
		ErectusProcess::FreeEx(pageCheck);
	}

	if (state)
	{
		if (*opkPageValid)
			return true;

		Opk opkData;
		opkData.opkNpcs = 0;
		opkData.originalFunction = originalFunction;
		memset(opkData.opkNpcPosition, 0x00, sizeof opkData.opkNpcPosition);

		if (!ErectusProcess::Wpm(*opkPage, &opkData, sizeof opkData))
			return false;
		if (!ErectusProcess::Wpm(ErectusProcess::exe + OFFSET_OPK, &opkOn, sizeof opkOn))
			return false;
		*opkPageValid = true;
	}
	else
	{
		if (pageCheck == *opkPage)
			ErectusProcess::Wpm(ErectusProcess::exe + OFFSET_OPK, &opkOff, sizeof opkOff);

		if (*opkPage && ErectusProcess::FreeEx(*opkPage))
		{
			*opkPage = 0;
			*opkPageValid = false;
		}
	}

	return true;
}

bool ErectusMemory::CheckOpkDistance(const DWORD64 opkPage)
{
	Opk opkData;
	if (!ErectusProcess::Rpm(opkPage, &opkData, sizeof opkData))
		return false;

	auto cameraData = GetCameraInfo();

	float editedOrigin[3];
	editedOrigin[0] = cameraData.origin[0] / 70.0f;
	editedOrigin[1] = cameraData.origin[1] / 70.0f;
	editedOrigin[2] = cameraData.origin[2] / 70.0f;

	const auto distance = Utils::GetDistance(opkData.opkNpcPosition, editedOrigin);
	if (distance > 20.0f)
		return false;


	return true;
}

bool ErectusMemory::SetOpkData(const DWORD64 opkPage, const bool enabled)
{
	Opk opkData;
	if (!ErectusProcess::Rpm(opkPage, &opkData, sizeof opkData))
		return false;

	if (!enabled)
	{
		opkData.opkNpcs = 0;
		memset(opkData.opkNpcPosition, 0x00, sizeof opkData.opkNpcPosition);
		ErectusProcess::Wpm(opkPage, &opkData, sizeof opkData);

		return true;
	}

	if (CheckOpkDistance(opkPage))
		return true;

	auto cameraData = GetCameraInfo();

	float editedOrigin[3];
	editedOrigin[0] = cameraData.origin[0] / 70.0f;
	editedOrigin[1] = cameraData.origin[1] / 70.0f;
	editedOrigin[2] = cameraData.origin[2] / 70.0f;

	float opkPosition[3];
	Utils::ProjectView(opkPosition, cameraData.forward, editedOrigin, 3.0f);

	opkData.opkNpcPosition[0] = opkPosition[0];
	opkData.opkNpcPosition[1] = opkPosition[1];
	opkData.opkNpcPosition[2] = opkPosition[2];
	opkData.opkNpcs = 1;

	return ErectusProcess::Wpm(opkPage, &opkData, sizeof opkData);
}

bool ErectusMemory::InsideInteriorCell()
{
	auto result = false;
	const auto localPlayerPtr = GetLocalPlayerPtr(false);
	if (!Utils::Valid(localPlayerPtr))
		return result;

	TesObjectRefr localPlayer = {};
	if (!ErectusProcess::Rpm(localPlayerPtr, &localPlayer, sizeof localPlayer))
		return result;

	TesObjectCell cell = {};
	if (!ErectusProcess::Rpm(localPlayer.cellPtr, &cell, sizeof TesObjectCell))
		return false;

	if (cell.isInterior)
		return true;

	return false;
}

LocalPlayerInfo ErectusMemory::GetLocalPlayerInfo()
{
	LocalPlayerInfo result = {};

	const auto localPlayerPtr = GetLocalPlayerPtr(false);
	if (!Utils::Valid(localPlayerPtr))
		return result;

	TesObjectRefr localPlayer{};
	if (!ErectusProcess::Rpm(localPlayerPtr, &localPlayer, sizeof localPlayer))
		return result;

	DWORD cellFormId = 0;
	if (Utils::Valid(localPlayer.cellPtr) && !ErectusProcess::Rpm(localPlayer.cellPtr + 0x20, &cellFormId, sizeof cellFormId))
		cellFormId = 0;

	auto playerHealth = -1.0f;
	ActorSnapshotComponent actorSnapshotComponentData{};
	if (GetActorSnapshotComponentData(localPlayer, &actorSnapshotComponentData))
		playerHealth = actorSnapshotComponentData.maxHealth + actorSnapshotComponentData.modifiedHealth + actorSnapshotComponentData.lostHealth;

	result.formId = localPlayer.formId;
	result.stashFormId = localPlayer.formId0C24;
	result.cellFormId = cellFormId;
	result.yaw = localPlayer.yaw;
	result.pitch = localPlayer.pitch;
	result.currentHealth = playerHealth;

	result.position[0] = localPlayer.position[0];
	result.position[1] = localPlayer.position[1];
	result.position[2] = localPlayer.position[2];

	return result;
}

bool ErectusMemory::ReferenceSwap(DWORD& sourceFormId, DWORD& destinationFormId)
{
	if (sourceFormId == destinationFormId)
		return true;

	auto sourcePointer = GetPtr(sourceFormId);
	if (!Utils::Valid(sourcePointer))
	{
		sourceFormId = 0x00000000;
		return false;
	}

	const auto destinationAddress = GetAddress(destinationFormId);
	if (!Utils::Valid(destinationAddress))
	{
		destinationFormId = 0x00000000;
		return false;
	}

	return ErectusProcess::Wpm(destinationAddress, &sourcePointer, sizeof sourcePointer);
}

bool ErectusMemory::CheckItemTransferList()
{
	for (auto i = 0; i < 32; i++)
	{
		if (Settings::customTransferSettings.whitelist[i] && Settings::customTransferSettings.whitelisted[i])
			return true;
	}

	return false;
}

bool ErectusMemory::TransferItems(const DWORD sourceFormId, const DWORD destinationFormId)
{
	auto sourcePtr = GetPtr(sourceFormId);
	if (!Utils::Valid(sourcePtr))
		return false;

	auto destinationPtr = GetPtr(destinationFormId);
	if (!Utils::Valid(destinationPtr))
		return false;

	TesObjectRefr entityData{};
	if (!ErectusProcess::Rpm(sourcePtr, &entityData, sizeof entityData))
		return false;
	if (!Utils::Valid(entityData.inventoryPtr))
		return false;

	Inventory inventoryData{};
	if (!ErectusProcess::Rpm(entityData.inventoryPtr, &inventoryData, sizeof inventoryData))
		return false;
	if (!Utils::Valid(inventoryData.entryArrayBegin) || inventoryData.entryArrayEnd < inventoryData.entryArrayBegin)
		return false;

	auto itemArraySize = (inventoryData.entryArrayEnd - inventoryData.entryArrayBegin) / sizeof(InventoryEntry);
	if (!itemArraySize || itemArraySize > 0x7FFF)
		return false;

	auto itemData = std::make_unique<InventoryEntry[]>(itemArraySize);
	if (!ErectusProcess::Rpm(inventoryData.entryArrayBegin, itemData.get(), itemArraySize * sizeof(InventoryEntry)))
		return false;

	for (DWORD64 i = 0; i < itemArraySize; i++)
	{
		if (!Utils::Valid(itemData[i].referencePtr))
			continue;
		if (!Utils::Valid(itemData[i].displayPtr) || itemData[i].iterations < itemData[i].displayPtr)
			continue;

		if (Settings::customTransferSettings.useWhitelist || Settings::customTransferSettings.useBlacklist)
		{
			TesItem referenceData{};
			if (!ErectusProcess::Rpm(itemData[i].referencePtr, &referenceData, sizeof referenceData))
				continue;

			if (Settings::customTransferSettings.useWhitelist)
			{
				if (!CheckFormIdArray(referenceData.formId, Settings::customTransferSettings.whitelisted, Settings::customTransferSettings.whitelist, 32))
					continue;
			}

			if (Settings::customTransferSettings.useBlacklist)
			{
				if (CheckFormIdArray(referenceData.formId, Settings::customTransferSettings.blacklisted, Settings::customTransferSettings.blacklist, 32))
					continue;
			}
		}

		auto iterations = (itemData[i].iterations - itemData[i].displayPtr) / sizeof(ItemCount);
		if (!iterations || iterations > 0x7FFF)
			continue;

		auto itemCountData = std::make_unique<ItemCount[]>(iterations);
		if (!ErectusProcess::Rpm(itemData[i].displayPtr, itemCountData.get(), iterations * sizeof(ItemCount)))
			continue;

		auto count = 0;
		for (DWORD64 c = 0; c < iterations; c++)
		{
			count += itemCountData[c].count;
		}
		if (count == 0)
			continue;

		TransferMessage transferMessageData = {
			.vtable = ErectusProcess::exe + VTABLE_REQUESTTRANSFERITEMMSG,
			.sourceEntityId = sourceFormId,
			.playerEntityId = 0xE0000E4A,
			.bShouldSendResult = true,
			.destEntityId = destinationFormId,
			.itemServerHandleId = itemData[i].itemId,
			.stashAccessEntityId = 0x00000000,
			.bCreateIfMissing = false,
			.bIsExpectingResult = false,
			.count = count,
		};
		MsgSender::Send(&transferMessageData, sizeof transferMessageData);
	}

	return true;
}

bool ErectusMemory::GetTeleportPosition(const int index)
{
	const auto localPlayerPtr = GetLocalPlayerPtr(true);
	if (!Utils::Valid(localPlayerPtr))
		return false;

	TesObjectRefr localPlayer{};
	if (!ErectusProcess::Rpm(localPlayerPtr, &localPlayer, sizeof localPlayer))
		return false;
	if (!Utils::Valid(localPlayer.cellPtr))
		return false;

	DWORD cellFormId;
	if (!ErectusProcess::Rpm(localPlayer.cellPtr + 0x20, &cellFormId, sizeof cellFormId))
		return false;

	Settings::teleporter.entries[index].destination[0] = localPlayer.position[0];
	Settings::teleporter.entries[index].destination[1] = localPlayer.position[1];
	Settings::teleporter.entries[index].destination[2] = localPlayer.position[2];
	Settings::teleporter.entries[index].destination[3] = localPlayer.yaw;
	Settings::teleporter.entries[index].cellFormId = cellFormId;

	return true;
}

bool ErectusMemory::RequestTeleport(const int index)
{
	const auto cellPtr = GetPtr(Settings::teleporter.entries[index].cellFormId);
	if (!Utils::Valid(cellPtr))
		return false;

	RequestTeleportMessage requestTeleportMessageData =
	{
		.vtable = ErectusProcess::exe + VTABLE_REQUESTTELEPORTTOLOCATIONMSG,
		.positionX = Settings::teleporter.entries[index].destination[0],
		.positionY = Settings::teleporter.entries[index].destination[1],
		.positionZ = Settings::teleporter.entries[index].destination[2],
		.rotationX = 0.0f,
		.rotationY = 0.0f,
		.rotationZ = Settings::teleporter.entries[index].destination[3],
		.cellPtr = cellPtr
	};

	return MsgSender::Send(&requestTeleportMessageData, sizeof requestTeleportMessageData);
}

DWORD ErectusMemory::GetLocalPlayerFormId()
{
	const auto localPlayerPtr = GetLocalPlayerPtr(true);
	if (!Utils::Valid(localPlayerPtr))
		return 0;

	TesObjectRefr localPlayer{};
	if (!ErectusProcess::Rpm(localPlayerPtr, &localPlayer, sizeof localPlayer))
		return 0;

	return localPlayer.formId;
}

DWORD ErectusMemory::GetStashFormId()
{
	const auto localPlayerPtr = GetLocalPlayerPtr(true);
	if (!Utils::Valid(localPlayerPtr))
		return 0;

	TesObjectRefr localPlayer{};
	if (!ErectusProcess::Rpm(localPlayerPtr, &localPlayer, sizeof localPlayer))
		return 0;

	return localPlayer.formId0C24;
}

void ErectusMemory::UpdateNukeCodes()
{
	GetNukeCode(0x000921AE, alphaCode);
	GetNukeCode(0x00092213, bravoCode);
	GetNukeCode(0x00092214, charlieCode);
}

bool ErectusMemory::FreezeActionPoints(DWORD64* freezeApPage, bool* freezeApPageValid, const bool state)
{
	if (!*freezeApPage)
	{
		const auto page = ErectusProcess::AllocEx(sizeof(FreezeAp));
		if (!page)
			return false;

		*freezeApPage = page;
	}

	BYTE freezeApOn[]
	{
		0x0F, 0x1F, 0x40, 0x00, //nop [rax+00]
		0x48, 0xBF, //mov rdi (Page)
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //Page (mov rdi)
		0xFF, 0xE7, //jmp rdi
		0x0F, 0x1F, 0x40, 0x00, //nop [rax+00]
	};

	BYTE freezeApOff[]
	{
		0x8B, 0xD6, //mov edx, esi
		0x48, 0x8B, 0xC8, //mov rcx, rax
		0x48, 0x8B, 0x5C, 0x24, 0x30, //mov rbx, [rsp+30]
		0x48, 0x8B, 0x74, 0x24, 0x38, //mov rsi, [rsp+38]
		0x48, 0x83, 0xC4, 0x20, //add rsp, 20
		0x5F, //pop rdi
	};

	BYTE freezeApCheck[sizeof freezeApOff];

	if (!ErectusProcess::Rpm(ErectusProcess::exe + OFFSET_AV_REGEN, &freezeApCheck, sizeof freezeApCheck))
		return false;

	DWORD64 pageCheck;
	memcpy(&pageCheck, &freezeApCheck[0x6], sizeof(DWORD64));

	if (Utils::Valid(pageCheck) && pageCheck != *freezeApPage)
	{
		for (auto i = 0; i < 0x6; i++) if (freezeApCheck[i] != freezeApOn[i])
			return false;
		if (!ErectusProcess::Wpm(ErectusProcess::exe + OFFSET_AV_REGEN, &freezeApOff, sizeof freezeApOff))
			return false;
		ErectusProcess::FreeEx(pageCheck);
	}

	if (state)
	{
		FreezeAp freezeApData;
		freezeApData.freezeApEnabled = Settings::localPlayer.freezeApEnabled;

		if (*freezeApPageValid)
		{
			FreezeAp freezeApPageCheck;
			if (!ErectusProcess::Rpm(*freezeApPage, &freezeApPageCheck, sizeof freezeApPageCheck))
				return false;
			if (!memcmp(&freezeApData, &freezeApPageCheck, sizeof freezeApPageCheck))
				return true;
			return ErectusProcess::Wpm(*freezeApPage, &freezeApData, sizeof freezeApData);
		}
		if (!ErectusProcess::Wpm(*freezeApPage, &freezeApData, sizeof freezeApData))
			return false;
		memcpy(&freezeApOn[0x6], &*freezeApPage, sizeof(DWORD64));
		if (!ErectusProcess::Wpm(ErectusProcess::exe + OFFSET_AV_REGEN, &freezeApOn, sizeof freezeApOn))
			return false;
		*freezeApPageValid = true;
	}
	else
	{
		if (pageCheck == *freezeApPage)
			ErectusProcess::Wpm(ErectusProcess::exe + OFFSET_AV_REGEN, &freezeApOff, sizeof freezeApOff);

		if (*freezeApPage)
		{
			if (ErectusProcess::FreeEx(*freezeApPage))
			{
				*freezeApPage = 0;
				*freezeApPageValid = false;
			}
		}
	}

	return true;
}

bool ErectusMemory::SetClientState(const DWORD64 clientState)
{
	const auto localPlayerPtr = GetLocalPlayerPtr(true);
	if (!Utils::Valid(localPlayerPtr))
		return false;

	if (InsideInteriorCell())
		return false;

	ClientStateMsg clientStateMsgData{ .vtable = ErectusProcess::exe + VTABLE_CLIENTSTATEMSG, .clientState = clientState };

	return MsgSender::Send(&clientStateMsgData, sizeof clientStateMsgData);
}

bool ErectusMemory::PositionSpoofing(const bool state)
{
	BYTE positionSpoofingOn[] = {
		0xBA, 0x00, 0x00, 0x00, 0x00, 0xEB, 0x11, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC,
		0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC
	};
	BYTE positionSpoofingOff[] = {
		0xBA, 0x01, 0x00, 0xF8, 0xFF, 0x3B, 0xC2, 0x7C, 0x0F, 0x8B, 0xD0, 0x41, 0xB8, 0xFF, 0xFF, 0x07, 0x00, 0x41,
		0x3B, 0xC0, 0x41, 0x0F, 0x4F, 0xD0
	};
	BYTE positionSpoofingCheck[sizeof positionSpoofingOff];

	if (!ErectusProcess::Rpm(ErectusProcess::exe + OFFSET_SERVER_POSITION, &positionSpoofingCheck, sizeof positionSpoofingCheck))
		return false;

	if (!state)
	{
		positionSpoofingCheck[1] = 0x00;
		positionSpoofingCheck[2] = 0x00;
		positionSpoofingCheck[3] = 0x00;
		positionSpoofingCheck[4] = 0x00;

		if (!memcmp(positionSpoofingCheck, positionSpoofingOn, sizeof positionSpoofingOn))
			return ErectusProcess::Wpm(ErectusProcess::exe + OFFSET_SERVER_POSITION, &positionSpoofingOff, sizeof positionSpoofingOff);

		return true;
	}

	int spoofingHeightCheck;
	memcpy(&spoofingHeightCheck, &positionSpoofingCheck[1], sizeof spoofingHeightCheck);
	memcpy(&positionSpoofingOn[1], &Settings::localPlayer.positionSpoofingHeight,
		sizeof Settings::localPlayer.positionSpoofingHeight);

	if (!memcmp(positionSpoofingCheck, positionSpoofingOn, sizeof positionSpoofingOn))
		return true;
	if (!memcmp(positionSpoofingCheck, positionSpoofingOff, sizeof positionSpoofingOff))
		return ErectusProcess::Wpm(ErectusProcess::exe + OFFSET_SERVER_POSITION, &positionSpoofingOn, sizeof positionSpoofingOn);
	if (spoofingHeightCheck != Settings::localPlayer.positionSpoofingHeight)
	{
		if (positionSpoofingCheck[0] != 0xBA || spoofingHeightCheck < -524287 || spoofingHeightCheck > 524287)
			return false;

		return ErectusProcess::Wpm(ErectusProcess::exe + OFFSET_SERVER_POSITION, &positionSpoofingOn,
			sizeof positionSpoofingOn);
	}
	return false;
}

DWORD ErectusMemory::GetEntityId(const TesObjectRefr& entityData)
{
	if (!(entityData.idValue[0] & 1))
		return 0;

	DWORD v1;
	memcpy(&v1, entityData.idValue, sizeof v1);

	const auto v2 = v1 >> 1;
	const auto v3 = v2 + v2;

	DWORD v4;
	if (!ErectusProcess::Rpm(ErectusProcess::exe + OFFSET_ENTITY_ID + v3 * 0x8, &v4, sizeof v4))
		return 0;

	const auto v5 = v4 & 0x7FF80000;
	const auto v6 = v5 | v2;

	return v6;
}

bool ErectusMemory::SendHitsToServer(Hits* hitsData, const size_t hitsDataSize)
{
	const auto allocSize = sizeof(ExternalFunction) + sizeof(RequestHitsOnActors) + hitsDataSize;
	const auto allocAddress = ErectusProcess::AllocEx(allocSize);
	if (allocAddress == 0)
		return false;

	ExternalFunction externalFunctionData;
	externalFunctionData.address = ErectusProcess::exe + OFFSET_MESSAGE_SENDER;
	externalFunctionData.rcx = allocAddress + sizeof(ExternalFunction);
	externalFunctionData.rdx = 0;
	externalFunctionData.r8 = 0;
	externalFunctionData.r9 = 0;

	auto pageData = std::make_unique<BYTE[]>(allocSize);
	memset(pageData.get(), 0x00, allocSize);

	RequestHitsOnActors requestHitsOnActorsData{};
	memset(&requestHitsOnActorsData, 0x00, sizeof(RequestHitsOnActors));

	requestHitsOnActorsData.vtable = ErectusProcess::exe + VTABLE_REQUESTHITSONACTORS;
	requestHitsOnActorsData.hitsArrayPtr = allocAddress + sizeof(ExternalFunction) + sizeof(RequestHitsOnActors);
	requestHitsOnActorsData.hitsArrayEnd = allocAddress + sizeof(ExternalFunction) + sizeof(RequestHitsOnActors) + hitsDataSize;

	memcpy(pageData.get(), &externalFunctionData, sizeof externalFunctionData);
	memcpy(&pageData[sizeof(ExternalFunction)], &requestHitsOnActorsData, sizeof requestHitsOnActorsData);
	memcpy(&pageData[sizeof(ExternalFunction) + sizeof(RequestHitsOnActors)], &*hitsData, hitsDataSize);

	const auto pageWritten = ErectusProcess::Wpm(allocAddress, pageData.get(), allocSize);

	if (!pageWritten)
	{
		ErectusProcess::FreeEx(allocAddress);
		return false;
	}

	const auto paramAddress = allocAddress + sizeof ExternalFunction::ASM;
	auto* const thread = CreateRemoteThread(ErectusProcess::handle, nullptr, 0, LPTHREAD_START_ROUTINE(allocAddress),
		LPVOID(paramAddress), 0, nullptr);

	if (thread == nullptr)
	{
		ErectusProcess::FreeEx(allocAddress);
		return false;
	}

	const auto threadResult = WaitForSingleObject(thread, 3000);
	CloseHandle(thread);

	if (threadResult == WAIT_TIMEOUT)
		return false;

	ErectusProcess::FreeEx(allocAddress);

	return true;
}

bool ErectusMemory::SendDamage(const DWORD64 targetPtr, const DWORD weaponId, BYTE* shotsHit, BYTE* shotsFired, const BYTE count)
{
	if (!Settings::targetting.dmgSend)
		return false;

	if (!weaponId)
		return false;

	if (!MsgSender::IsEnabled())
		return false;

	const auto localPlayerPtr = GetLocalPlayerPtr(true);
	if (!Utils::Valid(localPlayerPtr))
		return false;

	TesObjectRefr localPlayer{};
	if (!ErectusProcess::Rpm(localPlayerPtr, &localPlayer, sizeof localPlayer))
		return false;

	TesObjectRefr target{};
	if (!ErectusProcess::Rpm(targetPtr, &target, sizeof target))
		return false;

	if (target.formType == static_cast<BYTE>(FormTypes::PlayerCharacter))
		return false;

	if (!IsTargetValid(target))
		return false;

	const auto localPlayerId = GetEntityId(localPlayer);
	if (!localPlayerId)
		return false;

	const auto targetId = GetEntityId(target);
	if (!targetId)
		return false;

	auto hitsData = std::make_unique<Hits[]>(count);
	memset(hitsData.get(), 0x00, count * sizeof(Hits));

	if (*shotsHit == 0 || *shotsHit == 255)
		*shotsHit = 1;

	if (*shotsFired == 255)
		*shotsFired = 0;

	for (BYTE i = 0; i < count; i++)
	{
		hitsData[i].valueA = localPlayerId;
		hitsData[i].valueB = targetId;
		hitsData[i].valueC = 0;
		hitsData[i].initializationType = 0x3;
		hitsData[i].uiWeaponServerId = weaponId;
		hitsData[i].limbEnum = 0xFFFFFFFF;
		hitsData[i].hitEffectId = 0;
		hitsData[i].uEquipIndex = 0;
		hitsData[i].uAckIndex = *shotsHit;
		hitsData[i].uFireId = *shotsFired;
		hitsData[i].bPredictedKill = 0;
		hitsData[i].padding0023 = 0;
		hitsData[i].explosionLocationX = 0.0f;
		hitsData[i].explosionLocationY = 0.0f;
		hitsData[i].explosionLocationZ = 0.0f;
		hitsData[i].fProjectilePower = 1.0f;
		hitsData[i].bVatsAttack = 0;
		hitsData[i].bVatsCritical = 0;
		hitsData[i].bTargetWasDead = 0;
		hitsData[i].padding0037 = 0;

		if (Settings::targetting.sendDamageMax < 10)
		{
			if (Utils::GetRangedInt(1, 10) <= static_cast<int>(10 - Settings::targetting.sendDamageMax))
			{
				if (*shotsHit == 0 || *shotsHit == 255)
					*shotsHit = 1;
				else
					*shotsHit += 1;
			}
			else
			{
				*shotsHit = 1;
			}
		}
		else
			*shotsHit = 1;

		for (auto c = 0; c < Utils::GetRangedInt(1, 6); c++)
		{
			if (*shotsFired == 255)
				*shotsFired = 0;
			else
				*shotsFired += 1;
		}
	}

	const auto result = SendHitsToServer(hitsData.get(), count * sizeof(Hits));

	return result;
}

DWORD64 ErectusMemory::GetNukeCodePtr(const DWORD formId)
{
	ReferenceList questTextList{};
	if (!ErectusProcess::Rpm(ErectusProcess::exe + OFFSET_NUKE_CODE, &questTextList, sizeof questTextList))
		return 0;
	if (!Utils::Valid(questTextList.arrayPtr) || !questTextList.arraySize || questTextList.arraySize > 0x7FFF)
		return 0;

	auto questTextArray = std::make_unique<DWORD64[]>(questTextList.arraySize);
	if (!ErectusProcess::Rpm(questTextList.arrayPtr, questTextArray.get(), questTextList.arraySize * sizeof(DWORD64)))
		return 0;

	DWORD64 nukeCodePtr = 0;
	for (auto i = 0; i < questTextList.arraySize; i++)
	{
		if (!Utils::Valid(questTextArray[i]))
			continue;

		BgsQuestText bgsQuestTextData{};
		if (!ErectusProcess::Rpm(questTextArray[i], &bgsQuestTextData, sizeof bgsQuestTextData))
			continue;
		if (!Utils::Valid(bgsQuestTextData.formIdPtr) || !Utils::Valid(bgsQuestTextData.codePtr))
			continue;

		DWORD formIdCheck;
		if (!ErectusProcess::Rpm(bgsQuestTextData.formIdPtr + 0x4, &formIdCheck, sizeof formIdCheck))
			continue;
		if (formIdCheck != formId)
			continue;

		nukeCodePtr = bgsQuestTextData.codePtr;
		break;
	}

	return nukeCodePtr;
}

bool ErectusMemory::GetNukeCode(const DWORD formId, std::array<int, 8>& nukeCode)
{
	const auto nukeCodePtr = GetNukeCodePtr(formId);
	if (!nukeCodePtr)
		return false;

	float nukeCodeArray[16];
	if (!ErectusProcess::Rpm(nukeCodePtr, &nukeCodeArray, sizeof nukeCodeArray))
		return false;

	for (auto i = 0; i < 8; i++)
	{
		if (nukeCodeArray[i * 2 + 1] < 0.0f || nukeCodeArray[i * 2 + 1] > 9.0f)
		{
			nukeCode = {};
			return false;
		}
		nukeCode[i] = static_cast<int>(nukeCodeArray[i * 2 + 1]);
	}

	return true;
}

DWORD ErectusMemory::GetFavoritedWeaponId(const BYTE favouriteIndex)
{

	if (Settings::targetting.favoriteIndex >= 12)
		return 0;

	const auto localPlayerPtr = GetLocalPlayerPtr(true);
	if (!Utils::Valid(localPlayerPtr))
		return 0;

	TesObjectRefr localPlayer{};
	if (!ErectusProcess::Rpm(localPlayerPtr, &localPlayer, sizeof localPlayer))
		return 0;
	if (!Utils::Valid(localPlayer.inventoryPtr))
		return 0;

	Inventory inventoryData{};
	if (!ErectusProcess::Rpm(localPlayer.inventoryPtr, &inventoryData, sizeof inventoryData))
		return 0;
	if (!Utils::Valid(inventoryData.entryArrayBegin) || inventoryData.entryArrayEnd < inventoryData.entryArrayBegin)
		return 0;

	const auto itemArraySize = (inventoryData.entryArrayEnd - inventoryData.entryArrayBegin) / sizeof(InventoryEntry);
	if (!itemArraySize || itemArraySize > 0x7FFF)
		return 0;

	auto itemData = std::make_unique<InventoryEntry[]>(itemArraySize);
	if (!ErectusProcess::Rpm(inventoryData.entryArrayBegin, itemData.get(), itemArraySize * sizeof(InventoryEntry)))
		return 0;

	for (DWORD64 i = 0; i < itemArraySize; i++)
	{
		if (!Utils::Valid(itemData[i].referencePtr))
			continue;
		if (itemData[i].favoriteIndex != favouriteIndex)
			continue;

		TesItem referenceData{};
		if (!ErectusProcess::Rpm(itemData[i].referencePtr, &referenceData, sizeof referenceData))
			break;
		if (referenceData.formType != static_cast<BYTE>(FormTypes::TesObjectWeap))
			break;

		return itemData[i].itemId;
	}

	return 0;
}

char ErectusMemory::FavoriteIndex2Slot(const BYTE favoriteIndex)
{
	switch (favoriteIndex)
	{
	case 0x00:
		return '1';
	case 0x01:
		return '2';
	case 0x02:
		return '3';
	case 0x03:
		return '4';
	case 0x04:
		return '5';
	case 0x05:
		return '6';
	case 0x06:
		return '7';
	case 0x07:
		return '8';
	case 0x08:
		return '9';
	case 0x09:
		return '0';
	case 0x0A:
		return '-';
	case 0x0B:
		return '=';
	default:
		return '?';
	}
}

DWORD64 ErectusMemory::RttiGetNamePtr(const DWORD64 vtable)
{
	DWORD64 buffer;
	if (!ErectusProcess::Rpm(vtable - 0x8, &buffer, sizeof buffer))
		return 0;
	if (!Utils::Valid(buffer))
		return 0;

	DWORD offset;
	if (!ErectusProcess::Rpm(buffer + 0xC, &offset, sizeof offset))
		return 0;
	if (offset == 0 || offset > 0x7FFFFFFF)
		return 0;

	return ErectusProcess::exe + offset + 0x10;
}

std::string ErectusMemory::GetInstancedItemName(const DWORD64 displayPtr)
{
	std::string result{};

	if (!Utils::Valid(displayPtr))
		return result;

	DWORD64 instancedArrayPtr;
	if (!ErectusProcess::Rpm(displayPtr, &instancedArrayPtr, sizeof instancedArrayPtr))
		return result;
	if (!Utils::Valid(instancedArrayPtr))
		return result;

	ItemInstancedArray itemInstancedArrayData{};
	if (!ErectusProcess::Rpm(instancedArrayPtr, &itemInstancedArrayData, sizeof itemInstancedArrayData))
		return result;
	if (!Utils::Valid(itemInstancedArrayData.arrayPtr) || itemInstancedArrayData.arrayEnd < itemInstancedArrayData.arrayPtr)
		return result;

	const auto instancedArraySize = (itemInstancedArrayData.arrayEnd - itemInstancedArrayData.arrayPtr) / sizeof(DWORD64);
	if (!instancedArraySize || instancedArraySize > 0x7FFF)
		return result;

	auto instancedArray = std::make_unique<DWORD64[]>(instancedArraySize);
	if (!ErectusProcess::Rpm(itemInstancedArrayData.arrayPtr, instancedArray.get(), instancedArraySize * sizeof(DWORD64)))
		return result;

	for (DWORD64 i = 0; i < instancedArraySize; i++)
	{
		if (!Utils::Valid(instancedArray[i]))
			continue;

		ExtraTextDisplayData extraTextDisplayDataData{};
		if (!ErectusProcess::Rpm(instancedArray[i], &extraTextDisplayDataData, sizeof extraTextDisplayDataData))
			continue;

		const auto rttiNamePtr = RttiGetNamePtr(extraTextDisplayDataData.vtable);
		if (!rttiNamePtr)
			continue;

		char rttiNameCheck[sizeof".?AVExtraTextDisplayData@@"];
		if (!ErectusProcess::Rpm(rttiNamePtr, &rttiNameCheck, sizeof rttiNameCheck))
			continue;
		if (strcmp(rttiNameCheck, ".?AVExtraTextDisplayData@@") != 0)
			continue;

		result = GetEntityName(extraTextDisplayDataData.instancedNamePtr);
		return result;
	}

	return result;
}

std::unordered_map<int, std::string> ErectusMemory::GetFavoritedWeapons()
{
	std::unordered_map<int, std::string> result = {
		{0, "[?] No Weapon Selected"},
		{1, "[1] Favorited Item Invalid"},
		{2, "[2] Favorited Item Invalid"},
		{3, "[3] Favorited Item Invalid"},
		{4, "[4] Favorited Item Invalid"},
		{5, "[5] Favorited Item Invalid"},
		{6, "[6] Favorited Item Invalid"},
		{7, "[7] Favorited Item Invalid"},
		{8, "[8] Favorited Item Invalid"},
		{9, "[9] Favorited Item Invalid"},
		{10, "[0] Favorited Item Invalid"},
		{11, "[-] Favorited Item Invalid"},
		{12, "[=] Favorited Item Invalid"},
		{13, "[?] Favorited Item Invalid"},
	};

	const auto localPlayerPtr = GetLocalPlayerPtr(true);
	if (!Utils::Valid(localPlayerPtr))
		return result;

	TesObjectRefr localPlayer{};
	if (!ErectusProcess::Rpm(localPlayerPtr, &localPlayer, sizeof localPlayer))
		return result;
	if (!Utils::Valid(localPlayer.inventoryPtr))
		return result;

	Inventory inventoryData{};
	if (!ErectusProcess::Rpm(localPlayer.inventoryPtr, &inventoryData, sizeof inventoryData))
		return result;
	if (!Utils::Valid(inventoryData.entryArrayBegin) || inventoryData.entryArrayEnd < inventoryData.entryArrayBegin)
		return result;

	const auto itemArraySize = (inventoryData.entryArrayEnd - inventoryData.entryArrayBegin) / sizeof(InventoryEntry);
	if (!itemArraySize || itemArraySize > 0x7FFF)
		return result;

	auto itemData = std::make_unique<InventoryEntry[]>(itemArraySize);
	if (!ErectusProcess::Rpm(inventoryData.entryArrayBegin, itemData.get(), itemArraySize * sizeof(InventoryEntry)))
		return result;

	for (DWORD64 i = 0; i < itemArraySize; i++)
	{
		if (!Utils::Valid(itemData[i].referencePtr))
			continue;
		if (itemData[i].favoriteIndex > 12)
			continue;

		TesItem referenceData{};
		if (!ErectusProcess::Rpm(itemData[i].referencePtr, &referenceData, sizeof referenceData))
			continue;
		if (referenceData.formType != static_cast<BYTE>(FormTypes::TesObjectWeap))
			continue;

		auto weaponName = GetInstancedItemName(itemData[i].displayPtr);
		if (weaponName.empty())
		{
			weaponName = GetEntityName(referenceData.namePtr0098);
			if (weaponName.empty())
				continue;
		}

		result[itemData[i].favoriteIndex + 1] = fmt::format("[{}] {}", FavoriteIndex2Slot(itemData[i].favoriteIndex), weaponName);
	}

	return result;
}

std::string ErectusMemory::GetFavoritedWeaponText(const BYTE index)
{
	std::string result = {};

	const auto localPlayerPtr = GetLocalPlayerPtr(true);
	if (!Utils::Valid(localPlayerPtr))
		return result;

	TesObjectRefr localPlayer{};
	if (!ErectusProcess::Rpm(localPlayerPtr, &localPlayer, sizeof localPlayer))
		return result;
	if (!Utils::Valid(localPlayer.inventoryPtr))
		return result;

	Inventory inventoryData{};
	if (!ErectusProcess::Rpm(localPlayer.inventoryPtr, &inventoryData, sizeof inventoryData))
		return result;
	if (!Utils::Valid(inventoryData.entryArrayBegin) || inventoryData.entryArrayEnd < inventoryData.entryArrayBegin)
		return result;

	const auto itemArraySize = (inventoryData.entryArrayEnd - inventoryData.entryArrayBegin) / sizeof(InventoryEntry);
	if (!itemArraySize || itemArraySize > 0x7FFF)
		return result;

	auto itemData = std::make_unique<InventoryEntry[]>(itemArraySize);
	if (!ErectusProcess::Rpm(inventoryData.entryArrayBegin, itemData.get(), itemArraySize * sizeof(InventoryEntry)))
		return result;

	for (DWORD64 i = 0; i < itemArraySize; i++)
	{
		if (!Utils::Valid(itemData[i].referencePtr))
			continue;
		if (itemData[i].favoriteIndex != index)
			continue;

		TesItem referenceData{};
		if (!ErectusProcess::Rpm(itemData[i].referencePtr, &referenceData, sizeof referenceData))
			break;
		if (referenceData.formType != static_cast<BYTE>(FormTypes::TesObjectWeap))
			break;

		auto tempWeaponName = GetInstancedItemName(itemData[i].displayPtr);
		if (tempWeaponName.empty())
		{
			tempWeaponName = GetEntityName(referenceData.namePtr0098);
			if (tempWeaponName.empty())
				continue;
		}

		result = fmt::format("[{0}] {1}", FavoriteIndex2Slot(itemData[i].favoriteIndex), tempWeaponName);
		return result;
	}
	return result;
}

bool ErectusMemory::MeleeAttack()
{
	if (!MsgSender::IsEnabled())
		return false;

	const auto localPlayerPtr = GetLocalPlayerPtr(true);
	if (!Utils::Valid(localPlayerPtr))
		return false;

	const auto allocAddress = ErectusProcess::AllocEx(sizeof(ExternalFunction));
	if (allocAddress == 0)
		return false;

	ExternalFunction externalFunctionData = {
		.address = ErectusProcess::exe + OFFSET_MELEE_ATTACK,
		.rcx = localPlayerPtr,
		.rdx = 0,
		.r8 = 1,
		.r9 = 0,
	};

	const auto written = ErectusProcess::Wpm(allocAddress, &externalFunctionData, sizeof(ExternalFunction));

	if (!written)
	{
		ErectusProcess::FreeEx(allocAddress);
		return false;
	}

	const auto paramAddress = allocAddress + sizeof ExternalFunction::ASM;
	auto* const thread = CreateRemoteThread(ErectusProcess::handle, nullptr, 0, LPTHREAD_START_ROUTINE(allocAddress),
		LPVOID(paramAddress), 0, nullptr);

	if (thread == nullptr)
	{
		ErectusProcess::FreeEx(allocAddress);
		return false;
	}

	const auto threadResult = WaitForSingleObject(thread, 3000);
	CloseHandle(thread);

	if (threadResult == WAIT_TIMEOUT)
		return false;

	ErectusProcess::FreeEx(allocAddress);
	return true;
}

bool ErectusMemory::ChargenEditing()
{
	if (!Settings::characterEditor.enabled)
		return false;

	DWORD64 chargenPtr;
	if (!ErectusProcess::Rpm(ErectusProcess::exe + OFFSET_CHARGEN, &chargenPtr, sizeof chargenPtr))
		return false;
	if (!Utils::Valid(chargenPtr))
		return false;

	Chargen chargenData{};
	if (!ErectusProcess::Rpm(chargenPtr, &chargenData, sizeof chargenData))
		return false;

	auto shouldEdit = false;

	if (chargenData.thin != Settings::characterEditor.thin)
	{
		chargenData.thin = Settings::characterEditor.thin;
		shouldEdit = true;
	}

	if (chargenData.muscular != Settings::characterEditor.muscular)
	{
		chargenData.muscular = Settings::characterEditor.muscular;
		shouldEdit = true;
	}

	if (chargenData.large != Settings::characterEditor.large)
	{
		chargenData.large = Settings::characterEditor.large;
		shouldEdit = true;
	}

	if (shouldEdit)
		return ErectusProcess::Wpm(chargenPtr, &chargenData, sizeof chargenData);

	return true;
}

Camera ErectusMemory::GetCameraInfo()
{
	Camera result = {};
	const auto cameraPtr = GetCameraPtr();
	if (!Utils::Valid(cameraPtr))
		return result;

	if (!ErectusProcess::Rpm(cameraPtr, &result, sizeof result))
		return result;

	return result;
}

bool ErectusMemory::VtableSwap(const DWORD64 dst, DWORD64 src)
{
	DWORD oldProtect;
	if (!VirtualProtectEx(ErectusProcess::handle, reinterpret_cast<void*>(dst), sizeof(DWORD64), PAGE_READWRITE, &oldProtect))
		return false;

	const auto result = ErectusProcess::Wpm(dst, &src, sizeof src);

	DWORD buffer;
	if (!VirtualProtectEx(ErectusProcess::handle, reinterpret_cast<void*>(dst), sizeof(DWORD64), oldProtect, &buffer))
		return false;

	return result;
}