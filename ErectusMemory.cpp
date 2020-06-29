#include <array>
#include <unordered_set>

#include "ErectusInclude.h"
#include "fmt/format.h"

using namespace MemoryClasses;
using namespace SettingsClasses;

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

DWORD64 ErectusMemory::GetAddress(const DWORD formId)
{
	//this is basically hashmap.find(key)
	//v1+24 == hashmap start (_entries)
	//v1+32 == capacity (_capacity)
	//
	//here the hashmap is hashmap<formId, TesObjectRefr*>
	//item/entry is { std::tuple<formId, TesObjectRefr*> value, Entry* next }
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
	if (!Utils::Rpm(ErectusProcess::exe + OFFSET_GET_PTR_A1, &v1, sizeof v1))
		return 0;
	if (!Utils::Valid(v1))
		return 0;

	DWORD _capacity;
	if (!Utils::Rpm(v1 + 32, &_capacity, sizeof _capacity))
		return 0;
	if (!_capacity)
		return 0;

	//hash = crc32hash(formId)
	DWORD hash = 0;
	for (auto i = 0; i < sizeof formId; i++)
	{
		const auto v4 = (hash ^ formId >> i * 0x8) & 0xFF;

		DWORD v5;
		if (!Utils::Rpm(ErectusProcess::exe + OFFSET_GET_PTR_A2 + v4 * 0x4, &v5, sizeof v5)) //OFFSET_GET_PTR_A2 is just the start of a crc32 lookup table
			return 0;

		hash = hash >> 0x8 ^ v5;
	}

	auto _index = hash & _capacity - 1;

	DWORD64 entries;

	//sanity check: array exists
	if (!Utils::Rpm(v1 + 24, &entries, sizeof entries))
		return 0;
	if (!Utils::Valid(entries))
		return 0;

	//check item->next != -1
	DWORD next;
	if (!Utils::Rpm(entries + _index * 24 + 16, &next, sizeof next))
		return 0;
	if (next == 0xFFFFFFFF)
		return 0;

	auto v9 = _capacity;


	for (auto i = 0; i < 100; i++)
	{
		DWORD v10;
		if (!Utils::Rpm(entries + _index * 24, &v10, sizeof v10)) //item->value.first
			return 0;
		if (v10 == formId)
		{
			v9 = _index; //item->value
			if (v9 != _capacity)
				break;
		}
		else
		{
			if (!Utils::Rpm(entries + _index * 24 + 16, &_index, sizeof _index)) //item = item->next
				return 0;
			if (_index == _capacity)
				break;
		}
	}

	if (v9 == _capacity) return 0;

	return entries + v9 * 24 + 8; //item->value.second
}

DWORD64 ErectusMemory::GetPtr(const DWORD formId)
{
	const auto address = GetAddress(formId);
	if (!address)
		return 0;

	DWORD64 ptr;
	if (!Utils::Rpm(address, &ptr, sizeof ptr))
		return 0;

	return ptr;
}

DWORD64 ErectusMemory::GetCameraPtr()
{
	DWORD64 cameraPtr;
	if (!Utils::Rpm(ErectusProcess::exe + OFFSET_CAMERA, &cameraPtr, sizeof cameraPtr))
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
	if (!Utils::Rpm(ErectusProcess::exe + OFFSET_LOCAL_PLAYER, &localPlayerPtr, sizeof localPlayerPtr))
		return 0;
	if (!Utils::Valid(localPlayerPtr))
		return 0;

	if (checkMainMenu)
	{
		TesObjectRefr localPlayerData{};
		if (!Utils::Rpm(localPlayerPtr, &localPlayerData, sizeof localPlayerData))
			return 0;
		if (localPlayerData.formId == 0x00000014)
			return 0;
	}

	return localPlayerPtr;
}

std::vector<DWORD64> ErectusMemory::GetEntityList()
{
	std::vector<DWORD64> result;

	DWORD64 entityListTypePtr;
	if (!Utils::Rpm(ErectusProcess::exe + OFFSET_ENTITY_LIST, &entityListTypePtr, sizeof entityListTypePtr))
		return result;
	if (!Utils::Valid(entityListTypePtr))
		return result;

	//1) Get LoadedAreaManager
	LoadedAreaManager manager{};
	if (!Utils::Rpm(entityListTypePtr, &manager, sizeof manager))
		return result;
	if (!Utils::Valid(manager.interiorCellArrayPtr) || !Utils::Valid(manager.interiorCellArrayPtr2) || !Utils::Valid(manager.exteriorCellArrayPtr) || !Utils::Valid(manager.exteriorCellArrayPtr2))
		return result;

	DWORD64 cellPtrArrayPtr;
	int cellPtrArraySize;

	//2) Select  interior or exterior objectlist
	if (manager.interiorCellArrayPtr != manager.interiorCellArrayPtr2)
	{
		cellPtrArrayPtr = manager.interiorCellArrayPtr;
		cellPtrArraySize = 2;
	}
	else if (manager.exteriorCellArrayPtr != manager.exteriorCellArrayPtr2)
	{
		cellPtrArrayPtr = manager.exteriorCellArrayPtr;
		cellPtrArraySize = 50;
	}
	else return result; // sthg went wrong

	//3) Read the array of pointers to cells
	auto cellPtrArray = std::make_unique<DWORD64[]>(cellPtrArraySize);
	if (!Utils::Rpm(cellPtrArrayPtr, cellPtrArray.get(), cellPtrArraySize * sizeof DWORD64))
		return result;

	//4) Read each cell and push object pointers into objectPtrs
	for (auto i = 0; i < cellPtrArraySize; i++)
	{
		if (i % 2 == 0)
			continue;

		TesObjectCell cell{};
		if (!Utils::Rpm(cellPtrArray[i], &cell, sizeof TesObjectCell))
			continue;
		if (!Utils::Valid(cell.listPtr) || !cell.listSize)
			continue;

		auto objectPtrArray = std::make_unique<DWORD64[]>(cell.listSize);
		if (!Utils::Rpm(cell.listPtr, objectPtrArray.get(), cell.listSize * sizeof DWORD64))
			continue;

		result.insert(result.end(), objectPtrArray.get(), objectPtrArray.get() + cell.listSize);
	}

	return  result;
}

std::vector<DWORD64> ErectusMemory::GetNpcPtrList()
{
	std::vector<DWORD64> result;

	DWORD64 processListsPtr;
	if (!Utils::Rpm(ErectusProcess::exe + OFFSET_NPC_LIST, &processListsPtr, sizeof processListsPtr))
		return result;
	if (!Utils::Valid(processListsPtr))
		return result;

	ProcessLists processListsData{};
	if (!Utils::Rpm(processListsPtr, &processListsData, sizeof processListsData))
		return result;

	DWORD64 idArraySize = 0;

	auto useArrayA = false;
	if (Utils::Valid(processListsData.npcIdArrayPtrA) && processListsData.npcIdArraySizeA)
	{
		idArraySize += processListsData.npcIdArraySizeA;
		useArrayA = true;
	}

	auto useArrayB = false;
	if (Utils::Valid(processListsData.npcIdArrayPtrB) && processListsData.npcIdArraySizeB)
	{
		idArraySize += processListsData.npcIdArraySizeB;
		useArrayB = true;
	}

	if (!idArraySize)
		return result;

	const auto allocSize = sizeof(ExecutionList) + idArraySize * sizeof(DWORD64) * 2;
	const auto allocAddress = Utils::AllocEx(allocSize);
	if (allocAddress == 0)
		return result;

	ExecutionList executionListData = {
		.function = ErectusProcess::exe + OFFSET_NPC_PTR_FUNCTION,
		.idArraySize = idArraySize,
		.idArray = allocAddress + sizeof(ExecutionList),
		.ptrArray = allocAddress + sizeof(ExecutionList) + idArraySize * sizeof(DWORD64),
	};

	auto* pageData = new BYTE[allocSize];
	memset(pageData, 0x00, allocSize);
	memcpy(pageData, &executionListData, sizeof executionListData);

	auto index = 0;

	if (useArrayA)
	{
		for (auto i = 0; i < processListsData.npcIdArraySizeA; i++)
		{
			auto idAddress = processListsData.npcIdArrayPtrA + 0x4 * i;
			memcpy(&pageData[sizeof(ExecutionList) + index * sizeof(DWORD64)], &idAddress, sizeof idAddress);
			index++;
		}
	}

	if (useArrayB)
	{
		for (auto i = 0; i < processListsData.npcIdArraySizeB; i++)
		{
			auto idAddress = processListsData.npcIdArrayPtrB + 0x4 * i;
			memcpy(&pageData[sizeof(ExecutionList) + index * sizeof(DWORD64)], &idAddress, sizeof idAddress);
			index++;
		}
	}

	const auto written = Utils::Wpm(allocAddress, &*pageData, allocSize);

	delete[]pageData;
	pageData = nullptr;

	if (!written)
	{
		Utils::FreeEx(allocAddress);
		return result;
	}

	const auto paramAddress = allocAddress + sizeof ExecutionList::ASM;
	auto* const thread = CreateRemoteThread(ErectusProcess::handle, nullptr, 0, LPTHREAD_START_ROUTINE(allocAddress),
		LPVOID(paramAddress), 0, nullptr);

	if (thread == nullptr)
	{
		Utils::FreeEx(allocAddress);
		return result;
	}

	const auto threadResult = WaitForSingleObject(thread, 3000);
	CloseHandle(thread);

	if (threadResult == WAIT_TIMEOUT)
		return result;

	auto* executedList = new DWORD64[idArraySize];
	if (!Utils::Rpm(allocAddress + sizeof(ExecutionList) + idArraySize * sizeof(DWORD64), &*executedList, idArraySize * sizeof(DWORD64)))
	{
		delete[]executedList;
		executedList = nullptr;

		Utils::FreeEx(allocAddress);
		return result;
	}

	result.assign(executedList, executedList + idArraySize);

	Utils::FreeEx(allocAddress);
	delete[] executedList;
	return result;
}

std::vector<DWORD64> ErectusMemory::GetRecipeArray()
{
	std::vector<DWORD64> result;

	DWORD64 dataHandlerPtr;
	if (!Utils::Rpm(ErectusProcess::exe + OFFSET_DATA_HANDLER, &dataHandlerPtr, sizeof dataHandlerPtr))
		return result;
	if (!Utils::Valid(dataHandlerPtr))
		return result;

	ReferenceList omodList{};
	if (!Utils::Rpm(dataHandlerPtr + 0xF70, &omodList, sizeof omodList))
		return result;
	if (!Utils::Valid(omodList.arrayPtr) || !omodList.arraySize || omodList.arraySize > 0x7FFF)
		return result;

	auto* omodPtrArray = new DWORD64[omodList.arraySize];
	if (!Utils::Rpm(omodList.arrayPtr, &*omodPtrArray, omodList.arraySize * sizeof(DWORD64)))
	{
		delete[]omodPtrArray;
		omodPtrArray = nullptr;
		return result;
	}

	std::unordered_set<DWORD64> uniqueNamePtrs;
	for (auto i = 0; i < omodList.arraySize; i++)
	{
		if (!Utils::Valid(omodPtrArray[i]))
			continue;

		TesItem referenceData{};
		if (!Utils::Rpm(omodPtrArray[i], &referenceData, sizeof referenceData))
			continue;
		if (referenceData.omodFlag != 0x4)
			continue;


		if (!Utils::Valid(referenceData.namePtr00B0))
			continue;

		auto r = uniqueNamePtrs.insert(referenceData.namePtr00B0);
		if (r.second) //not inserted -> not unique
			result.push_back(omodPtrArray[i]);
	}

	delete[]omodPtrArray;
	omodPtrArray = nullptr;

	return result;
}

bool ErectusMemory::UpdateKnownRecipes()
{
	const auto localPlayerPtr = GetLocalPlayerPtr(true);
	if (!Utils::Valid(localPlayerPtr))
		return false;

	auto recipes = GetRecipeArray();
	if (recipes.empty())
		return false;

	const auto allocSize = sizeof(ExecutionPlan) + (recipes.size() * sizeof(DWORD64) + recipes.size() * sizeof(bool));
	const auto allocAddress = Utils::AllocEx(allocSize);
	if (allocAddress == 0)
	{
		return false;
	}

	ExecutionPlan executionPlanData = {
		.function = allocAddress + sizeof ExecutionPlan::ASM,
		.localPlayerPtr = localPlayerPtr,
		.recipeArraySize = recipes.size(),
		.recipeArray = allocAddress + sizeof(ExecutionPlan),
		.learnedRecipeArray = allocAddress + sizeof(ExecutionPlan) + recipes.size() * sizeof(DWORD64),
	};

	auto* pageData = new BYTE[allocSize];
	memset(pageData, 0x00, allocSize);
	memcpy(pageData, &executionPlanData, sizeof executionPlanData);

	for (auto i = 0; i < recipes.size(); i++)
	{
		memcpy(&pageData[sizeof(ExecutionPlan) + i * sizeof(DWORD64)], &recipes[i], sizeof(DWORD64));
	}

	const auto written = Utils::Wpm(allocAddress, &*pageData, allocSize);

	delete[]pageData;
	pageData = nullptr;

	if (!written)
	{
		Utils::FreeEx(allocAddress);
		return false;
	}

	const auto paramAddress = allocAddress + sizeof ExecutionPlan::ASM + sizeof ExecutionPlan::rbp;
	auto* thread = CreateRemoteThread(ErectusProcess::handle, nullptr, 0, LPTHREAD_START_ROUTINE(allocAddress),
		LPVOID(paramAddress), 0, nullptr);

	if (thread == nullptr)
	{
		Utils::FreeEx(allocAddress);
		return false;
	}

	const auto threadResult = WaitForSingleObject(thread, 3000);
	CloseHandle(thread);

	if (threadResult == WAIT_TIMEOUT)
	{
		return false;
	}

	auto* executedPlan = new bool[recipes.size()];
	if (!Utils::Rpm(allocAddress + sizeof(ExecutionPlan) + recipes.size() * sizeof(DWORD64), &*executedPlan, recipes.size() * sizeof(bool)))
	{
		delete[]executedPlan;
		executedPlan = nullptr;

		Utils::FreeEx(allocAddress);
		return false;
	}

	for (auto i = 0; i < recipes.size(); i++)
	{
		if (executedPlan[i])
		{
			DWORD64 buffer;
			if (Utils::Rpm(recipes.at(i) + 0xB0, &buffer, sizeof buffer))
			{
				if (Utils::Valid(buffer))
				{
					DWORD formId;
					if (Utils::Rpm(buffer + 0x20, &formId, sizeof formId))
					{
						knownRecipes.insert(formId);
					}
				}
			}
		}
	}
	delete[]executedPlan;
	executedPlan = nullptr;
	Utils::FreeEx(allocAddress);

	return true;
}

BYTE ErectusMemory::IsKnownRecipe(const DWORD formId)
{
	if (knownRecipes.empty())
		return 0x00;

	if (knownRecipes.count(formId))
		return 0x01;

	return 0x02;
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

bool ErectusMemory::CheckReferenceJunk(const TesItem& referenceData)
{
	if (referenceData.componentArraySize)
	{
		if (!(referenceData.recordFlagA >> 7 & 1))
			return true;
	}

	return false;
}

bool ErectusMemory::CheckReferenceMod(const TesItem& referenceData)
{
	if (referenceData.recordFlagA >> 7 & 1)
		return true;

	return false;
}

bool ErectusMemory::CheckReferencePlan(const TesItem& referenceData)
{
	if (referenceData.planFlag >> 5 & 1)
	{
		return true;
	}

	return false;
}

bool ErectusMemory::CheckScrapList()
{
	for (auto i : ErectusIni::scrapLooter.enabledList)
	{
		if (i)
			return true;
	}


	return false;
}

bool ErectusMemory::CheckItemLooterList()
{
	for (auto i = 0; i < 100; i++)
	{
		if (ErectusIni::itemLooter.formIdList[i] && ErectusIni::itemLooter.enabledList[i])
			return true;
	}

	return false;
}

bool ErectusMemory::CheckItemLooterBlacklist()
{
	if (ErectusIni::itemLooter.blacklistToggle)
	{
		for (auto i = 0; i < 64; i++)
		{
			if (ErectusIni::itemLooter.blacklist[i] && ErectusIni::itemLooter.blacklistEnabled[i])
				return true;
		}
	}

	return false;
}

bool ErectusMemory::CheckEntityLooterList(const EntityLooterSettings& settings)
{
	for (auto i = 0; i < 100; i++)
	{
		if (settings.formIdList[i] && settings.enabledList[i])
			return true;
	}

	return false;
}

bool ErectusMemory::CheckEntityLooterBlacklist(const EntityLooterSettings& settings)
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

bool ErectusMemory::CheckIngredientList()
{
	for (auto i : ErectusIni::harvester.enabledList)
	{
		if (i)
			return true;
	}

	return false;
}

bool ErectusMemory::CheckJunkPileEnabled()
{
	for (auto i = 0; i < 69; i++)
	{
		if (!strcmp(ErectusIni::harvester.nameList[i], "Junk Pile"))
			return ErectusIni::harvester.enabledList[i];
	}

	return false;
}

bool ErectusMemory::CheckComponentArray(const TesItem& referenceData)
{
	if (!referenceData.componentArraySize || referenceData.componentArraySize > 0x7FFF)
		return false;

	if (!Utils::Valid(referenceData.componentArrayPtr))
		return false;

	auto* componentArray = new Component[referenceData.componentArraySize];
	if (!Utils::Rpm(referenceData.componentArrayPtr, &*componentArray, referenceData.componentArraySize * sizeof(Component)))
	{
		delete[]componentArray;
		componentArray = nullptr;
		return false;
	}

	for (auto i = 0; i < referenceData.componentArraySize; i++)
	{
		if (!Utils::Valid(componentArray[i].componentReferencePtr))
			continue;
		if (!Utils::Valid(componentArray[i].componentCountReferencePtr))
			continue;

		TesItem componentData{};
		if (!Utils::Rpm(componentArray[i].componentReferencePtr, &componentData, sizeof componentData))
			continue;
		if (CheckFormIdArray(componentData.formId, ErectusIni::scrapLooter.enabledList, ErectusIni::scrapLooter.formIdList, 40))
		{
			delete[]componentArray;
			componentArray = nullptr;
			return true;
		}
	}

	delete[]componentArray;
	componentArray = nullptr;
	return false;
}

bool ErectusMemory::CheckReferenceKeywordBook(const TesItem& referenceData, const DWORD formId)
{
	if (!referenceData.keywordArrayData01C0 || referenceData.keywordArrayData01C0 > 0x7FFF)
		return false;
	if (!Utils::Valid(referenceData.keywordArrayData01B8))
		return false;

	auto* keywordArray = new DWORD64[referenceData.keywordArrayData01C0];
	if (!Utils::Rpm(referenceData.keywordArrayData01B8, &*keywordArray, referenceData.keywordArrayData01C0 * sizeof(DWORD64)))
	{
		delete[]keywordArray;
		keywordArray = nullptr;
		return false;
	}

	for (DWORD64 i = 0; i < referenceData.keywordArrayData01C0; i++)
	{
		if (!Utils::Valid(keywordArray[i]))
			continue;

		DWORD formIdCheck;
		if (!Utils::Rpm(keywordArray[i] + 0x20, &formIdCheck, sizeof formIdCheck))
			continue;
		if (formIdCheck != formId)
			continue;

		delete[]keywordArray;
		keywordArray = nullptr;
		return true;
	}

	delete[]keywordArray;
	keywordArray = nullptr;
	return false;
}

bool ErectusMemory::CheckReferenceKeywordMisc(const TesItem& referenceData, const DWORD formId)
{
	if (!referenceData.keywordArrayData01B8 || referenceData.keywordArrayData01B8 > 0x7FFF)
		return false;
	if (!Utils::Valid(referenceData.keywordArrayData01B0))
		return false;

	auto* keywordArray = new DWORD64[referenceData.keywordArrayData01B8];
	if (!Utils::Rpm(referenceData.keywordArrayData01B0, &*keywordArray, referenceData.keywordArrayData01B8 * sizeof(DWORD64)))
	{
		delete[]keywordArray;
		keywordArray = nullptr;
		return false;
	}

	for (DWORD64 i = 0; i < referenceData.keywordArrayData01B8; i++)
	{
		if (!Utils::Valid(keywordArray[i]))
			continue;

		DWORD formIdCheck;
		if (!Utils::Rpm(keywordArray[i] + 0x20, &formIdCheck, sizeof formIdCheck))
			continue;
		if (formIdCheck != formId)
			continue;

		delete[]keywordArray;
		keywordArray = nullptr;
		return true;
	}

	delete[]keywordArray;
	keywordArray = nullptr;
	return false;
}

bool ErectusMemory::CheckWhitelistedFlux(const TesItem& referenceData)
{
	if (!Utils::Valid(referenceData.harvestedPtr))
		return false;

	DWORD formIdCheck;
	if (!Utils::Rpm(referenceData.harvestedPtr + 0x20, &formIdCheck, sizeof formIdCheck))
		return false;

	switch (formIdCheck)
	{
	case 0x002DDD45: //Raw Crimson Flux
		return ErectusIni::customFluxSettings.crimsonFluxEnabled;
	case 0x002DDD46: //Raw Cobalt Flux
		return ErectusIni::customFluxSettings.cobaltFluxEnabled;
	case 0x002DDD49: //Raw Yellowcake Flux
		return ErectusIni::customFluxSettings.yellowcakeFluxEnabled;
	case 0x002DDD4B: //Raw Fluorescent Flux
		return ErectusIni::customFluxSettings.fluorescentFluxEnabled;
	case 0x002DDD4D: //Raw Violet Flux
		return ErectusIni::customFluxSettings.violetFluxEnabled;
	default:
		return false;
	}
}

bool ErectusMemory::FloraLeveledListValid(const LeveledList& leveledListData)
{
	if (!Utils::Valid(leveledListData.listEntryArrayPtr) || !leveledListData.listEntryArraySize)
		return false;

	auto* listEntryData = new ListEntry[leveledListData.listEntryArraySize];
	if (!Utils::Rpm(leveledListData.listEntryArrayPtr, &*listEntryData, leveledListData.listEntryArraySize * sizeof(ListEntry)))
	{
		delete[]listEntryData;
		listEntryData = nullptr;
		return false;
	}

	for (BYTE i = 0; i < leveledListData.listEntryArraySize; i++)
	{
		if (!Utils::Valid(listEntryData[i].referencePtr))
			continue;

		TesItem referenceData{};
		if (!Utils::Rpm(listEntryData[i].referencePtr, &referenceData, sizeof referenceData))
			continue;
		if (referenceData.formType == static_cast<BYTE>(FormTypes::TesLevItem))
		{
			LeveledList recursiveLeveledListData{};
			memcpy(&recursiveLeveledListData, &referenceData, sizeof recursiveLeveledListData);
			if (FloraLeveledListValid(recursiveLeveledListData))
			{
				delete[]listEntryData;
				listEntryData = nullptr;
				return true;
			}
		}
		else if (CheckFormIdArray(referenceData.formId, ErectusIni::harvester.enabledList, ErectusIni::harvester.formIdList, 69))
		{
			delete[]listEntryData;
			listEntryData = nullptr;
			return true;
		}
	}

	delete[]listEntryData;
	listEntryData = nullptr;
	return false;
}

bool ErectusMemory::FloraValid(const TesItem& referenceData)
{
	if (referenceData.formId == 0x000183C6)
		return CheckJunkPileEnabled();

	if (!Utils::Valid(referenceData.harvestedPtr))
		return false;

	TesItem harvestedData{};
	if (!Utils::Rpm(referenceData.harvestedPtr, &harvestedData, sizeof harvestedData))
		return false;
	if (referenceData.formType == static_cast<BYTE>(FormTypes::TesLevItem))
	{
		LeveledList leveledListData{};
		memcpy(&leveledListData, &harvestedData, sizeof leveledListData);
		return FloraLeveledListValid(leveledListData);
	}
	return CheckFormIdArray(harvestedData.formId, ErectusIni::harvester.enabledList, ErectusIni::harvester.formIdList, 69);
}

bool ErectusMemory::IsTreasureMap(const DWORD formId)
{
	switch (formId)
	{
	case 0x0051B8CD: //Cranberry Bog Treasure Map #01
	case 0x0051B8D6: //Cranberry Bog Treasure Map #02
	case 0x0051B8D9: //Cranberry Bog Treasure Map #03
	case 0x0051B8DE: //Cranberry Bog Treasure Map #04
	case 0x0051B8CE: //Mire Treasure Map #01
	case 0x0051B8D2: //Mire Treasure Map #02
	case 0x0051B8D7: //Mire Treasure Map #03
	case 0x0051B8D8: //Mire Treasure Map #04
	case 0x0051B8DB: //Mire Treasure Map #05
	case 0x0051B8BA: //Savage Divide Treasure Map #01
	case 0x0051B8C0: //Savage Divide Treasure Map #02
	case 0x0051B8C2: //Savage Divide Treasure Map #03
	case 0x0051B8C4: //Savage Divide Treasure Map #04
	case 0x0051B8C6: //Savage Divide Treasure Map #05
	case 0x0051B8C7: //Savage Divide Treasure Map #06
	case 0x0051B8C8: //Savage Divide Treasure Map #07
	case 0x0051B8CA: //Savage Divide Treasure Map #08
	case 0x0051B8CC: //Savage Divide Treasure Map #09
	case 0x0051B8D4: //Savage Divide Treasure Map #10
	case 0x0051B8B1: //Toxic Valley Treasure Map #01
	case 0x0051B8B8: //Toxic Valley Treasure Map #02
	case 0x0051B8BC: //Toxic Valley Treasure Map #03
	case 0x0051B8C1: //Toxic Valley Treasure Map #04
	case 0x0051B7A2: //Forest Treasure Map #01
	case 0x0051B8A6: //Forest Treasure Map #02
	case 0x0051B8A7: //Forest Treasure Map #03
	case 0x0051B8A9: //Forest Treasure Map #04
	case 0x0051B8AA: //Forest Treasure Map #05
	case 0x0051B8AE: //Forest Treasure Map #06
	case 0x0051B8B0: //Forest Treasure Map #07
	case 0x0051B8B2: //Forest Treasure Map #08
	case 0x0051B8B6: //Forest Treasure Map #09
	case 0x0051B8B9: //Forest Treasure Map #10
	case 0x0051B8A8: //Ash Heap Treasure Map #01
	case 0x0051B8AC: //Ash Heap Treasure Map #02
		return true;
	default:
		return false;
	}
}

bool ErectusMemory::CheckFactionFormId(const DWORD formId)
{
	switch (formId)
	{
	case 0x003FC008: //W05_SettlerFaction
		return ErectusIni::customExtraNpcSettings.hideSettlerFaction;
	case 0x003FE94A: //W05_CraterRaiderFaction
		return ErectusIni::customExtraNpcSettings.hideCraterRaiderFaction;
	case 0x003FBC00: //W05_DieHardFaction
		return ErectusIni::customExtraNpcSettings.hideDieHardFaction;
	case 0x005427B2: //W05_SecretServiceFaction
		return ErectusIni::customExtraNpcSettings.hideSecretServiceFaction;
	default:
		return false;
	}
}

bool ErectusMemory::BlacklistedNpcFaction(const TesItem& referenceData)
{
	if (!Utils::Valid(referenceData.factionArrayPtr) || !referenceData.factionArraySize || referenceData.factionArraySize > 0x7FFF)
		return false;

	auto* factionArray = new DWORD64[referenceData.factionArraySize * 2];
	if (!Utils::Rpm(referenceData.factionArrayPtr, &*factionArray, referenceData.factionArraySize * 2 * sizeof(DWORD64)))
	{
		delete[]factionArray;
		factionArray = nullptr;
		return false;
	}

	auto blacklistedFactionFound = false;
	for (auto i = 0; i < referenceData.factionArraySize; i++)
	{
		if (!Utils::Valid(factionArray[i * 2]))
			continue;

		DWORD formId;
		if (!Utils::Rpm(factionArray[i * 2] + 0x20, &formId, sizeof formId))
			continue;
		if (CheckFactionFormId(formId))
		{
			blacklistedFactionFound = true;
			break;
		}
	}

	delete[]factionArray;
	factionArray = nullptr;
	return blacklistedFactionFound;
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

void ErectusMemory::GetCustomEntityData(const TesItem& referenceData, DWORD64* entityFlag, DWORD64* entityNamePtr,
	int* enabledDistance, const bool checkScrap, const bool checkIngredient)
{
	switch (referenceData.formType)
	{
	case (static_cast<byte>(FormTypes::TesNpc)):
		*entityFlag |= CUSTOM_ENTRY_NPC;
		*entityNamePtr = referenceData.namePtr0160;
		*enabledDistance = ErectusIni::npcSettings.enabledDistance;
		break;
	case (static_cast<byte>(FormTypes::TesObjectCont)):
		*entityFlag |= CUSTOM_ENTRY_CONTAINER;
		*entityNamePtr = referenceData.namePtr00B0;
		*enabledDistance = ErectusIni::containerSettings.enabledDistance;
		if (CheckFormIdArray(referenceData.formId, ErectusIni::containerSettings.whitelisted, ErectusIni::containerSettings.whitelist, 32))
			*entityFlag |= CUSTOM_ENTRY_WHITELISTED;
		else if (!ErectusIni::containerSettings.enabled)
			*entityFlag |= CUSTOM_ENTRY_INVALID;
		break;
	case (static_cast<byte>(FormTypes::TesObjectMisc)):
		*entityFlag |= CUSTOM_ENTRY_VALID_ITEM;
		*entityNamePtr = referenceData.namePtr0098;
		if (CheckReferenceJunk(referenceData))
		{
			*entityFlag |= CUSTOM_ENTRY_JUNK;
			*enabledDistance = ErectusIni::junkSettings.enabledDistance;
			if (checkScrap)
			{
				if (CheckComponentArray(referenceData))
					*entityFlag |= CUSTOM_ENTRY_VALID_SCRAP;
				else
					*entityFlag |= CUSTOM_ENTRY_INVALID;
			}
			else
			{
				if (CheckFormIdArray(referenceData.formId, ErectusIni::junkSettings.whitelisted, ErectusIni::junkSettings.whitelist, 32))
					*entityFlag |= CUSTOM_ENTRY_WHITELISTED;
				else if (!ErectusIni::junkSettings.enabled)
					*entityFlag |= CUSTOM_ENTRY_INVALID;
			}
		}
		else if (CheckReferenceMod(referenceData))
		{
			*entityFlag |= CUSTOM_ENTRY_MOD;
			*entityFlag |= CUSTOM_ENTRY_ITEM;
			*enabledDistance = ErectusIni::itemSettings.enabledDistance;
			if (CheckFormIdArray(referenceData.formId, ErectusIni::itemSettings.whitelisted, ErectusIni::itemSettings.whitelist, 32))
				*entityFlag |= CUSTOM_ENTRY_WHITELISTED;
			else if (!ErectusIni::itemSettings.enabled)
				*entityFlag |= CUSTOM_ENTRY_INVALID;
		}
		else if (CheckReferenceKeywordMisc(referenceData, 0x00135E6C))
		{
			*entityFlag |= CUSTOM_ENTRY_BOBBLEHEAD;
			*enabledDistance = ErectusIni::bobbleheadSettings.enabledDistance;
			if (CheckFormIdArray(referenceData.formId, ErectusIni::bobbleheadSettings.whitelisted, ErectusIni::bobbleheadSettings.whitelist, 32))
				*entityFlag |= CUSTOM_ENTRY_WHITELISTED;
			else if (!ErectusIni::bobbleheadSettings.enabled)
				*entityFlag |= CUSTOM_ENTRY_INVALID;
		}
		else
		{
			*entityFlag |= CUSTOM_ENTRY_MISC;
			*entityFlag |= CUSTOM_ENTRY_ITEM;
			*enabledDistance = ErectusIni::itemSettings.enabledDistance;
			if (CheckFormIdArray(referenceData.formId, ErectusIni::itemSettings.whitelisted, ErectusIni::itemSettings.whitelist, 32))
				*entityFlag |= CUSTOM_ENTRY_WHITELISTED;
			else if (!ErectusIni::itemSettings.enabled)
				*entityFlag |= CUSTOM_ENTRY_INVALID;
		}
		break;
	case (static_cast<byte>(FormTypes::TesObjectBook)):
		*entityFlag |= CUSTOM_ENTRY_VALID_ITEM;
		*entityNamePtr = referenceData.namePtr0098;
		if (CheckReferencePlan(referenceData))
		{
			*entityFlag |= CUSTOM_ENTRY_PLAN;
			*enabledDistance = ErectusIni::planSettings.enabledDistance;
			switch (IsKnownRecipe(referenceData.formId))
			{
			case 0x01:
				*entityFlag |= CUSTOM_ENTRY_KNOWN_RECIPE;
				break;
			case 0x02:
				*entityFlag |= CUSTOM_ENTRY_UNKNOWN_RECIPE;
				break;
			default:
				*entityFlag |= CUSTOM_ENTRY_FAILED_RECIPE;
				break;
			}
			if (CheckFormIdArray(referenceData.formId, ErectusIni::planSettings.whitelisted, ErectusIni::planSettings.whitelist, 32))
				*entityFlag |= CUSTOM_ENTRY_WHITELISTED;
			else if (!ErectusIni::planSettings.enabled)
				*entityFlag |= CUSTOM_ENTRY_INVALID;
		}
		else if (CheckReferenceKeywordBook(referenceData, 0x001D4A70))
		{
			*entityFlag |= CUSTOM_ENTRY_MAGAZINE;
			*enabledDistance = ErectusIni::magazineSettings.enabledDistance;
			if (CheckFormIdArray(referenceData.formId, ErectusIni::magazineSettings.whitelisted, ErectusIni::magazineSettings.whitelist, 32))
				*entityFlag |= CUSTOM_ENTRY_WHITELISTED;
			else if (!ErectusIni::magazineSettings.enabled)
				*entityFlag |= CUSTOM_ENTRY_INVALID;
		}
		else
		{
			*entityFlag |= CUSTOM_ENTRY_ITEM;
			*enabledDistance = ErectusIni::itemSettings.enabledDistance;
			if (IsTreasureMap(referenceData.formId))
				*entityFlag |= CUSTOM_ENTRY_TREASURE_MAP;
			if (CheckFormIdArray(referenceData.formId, ErectusIni::itemSettings.whitelisted, ErectusIni::itemSettings.whitelist, 32))
				*entityFlag |= CUSTOM_ENTRY_WHITELISTED;
			else if (!ErectusIni::itemSettings.enabled)
				*entityFlag |= CUSTOM_ENTRY_INVALID;
		}
		break;
	case (static_cast<byte>(FormTypes::TesFlora)):
		*entityFlag |= CUSTOM_ENTRY_FLORA;
		*entityNamePtr = referenceData.namePtr0098;
		*enabledDistance = ErectusIni::floraSettings.enabledDistance;
		if (checkIngredient)
		{
			if (FloraValid(referenceData))
				*entityFlag |= CUSTOM_ENTRY_VALID_INGREDIENT;
			else
				*entityFlag |= CUSTOM_ENTRY_INVALID;
		}
		else
		{
			if (CheckWhitelistedFlux(referenceData) || CheckFormIdArray(referenceData.formId, ErectusIni::floraSettings.whitelisted, ErectusIni::floraSettings.whitelist, 32))
				*entityFlag |= CUSTOM_ENTRY_WHITELISTED;
			else if (!ErectusIni::floraSettings.enabled)
				*entityFlag |= CUSTOM_ENTRY_INVALID;
		}
		break;
	case (static_cast<byte>(FormTypes::TesObjectWeap)):
		*entityFlag |= CUSTOM_ENTRY_WEAPON;
		*entityFlag |= CUSTOM_ENTRY_ITEM;
		*entityFlag |= CUSTOM_ENTRY_VALID_ITEM;
		*entityNamePtr = referenceData.namePtr0098;
		*enabledDistance = ErectusIni::itemSettings.enabledDistance;
		if (CheckFormIdArray(referenceData.formId, ErectusIni::itemSettings.whitelisted, ErectusIni::itemSettings.whitelist, 32))
			*entityFlag |= CUSTOM_ENTRY_WHITELISTED;
		else if (!ErectusIni::itemSettings.enabled)
			*entityFlag |= CUSTOM_ENTRY_INVALID;
		break;
	case (static_cast<byte>(FormTypes::TesObjectArmo)):
		*entityFlag |= CUSTOM_ENTRY_ARMOR;
		*entityFlag |= CUSTOM_ENTRY_ITEM;
		*entityFlag |= CUSTOM_ENTRY_VALID_ITEM;
		*entityNamePtr = referenceData.namePtr0098;
		*enabledDistance = ErectusIni::itemSettings.enabledDistance;
		if (CheckFormIdArray(referenceData.formId, ErectusIni::itemSettings.whitelisted, ErectusIni::itemSettings.whitelist, 32))
			*entityFlag |= CUSTOM_ENTRY_WHITELISTED;
		else if (!ErectusIni::itemSettings.enabled)
			*entityFlag |= CUSTOM_ENTRY_INVALID;
		break;
	case (static_cast<byte>(FormTypes::TesAmmo)):
		*entityFlag |= CUSTOM_ENTRY_AMMO;
		*entityFlag |= CUSTOM_ENTRY_ITEM;
		*entityFlag |= CUSTOM_ENTRY_VALID_ITEM;
		*entityNamePtr = referenceData.namePtr0098;
		*enabledDistance = ErectusIni::itemSettings.enabledDistance;
		if (CheckFormIdArray(referenceData.formId, ErectusIni::itemSettings.whitelisted, ErectusIni::itemSettings.whitelist, 32))
			*entityFlag |= CUSTOM_ENTRY_WHITELISTED;
		else if (!ErectusIni::itemSettings.enabled)
			*entityFlag |= CUSTOM_ENTRY_INVALID;
		break;
	case (static_cast<byte>(FormTypes::AlchemyItem)):
		*entityFlag |= CUSTOM_ENTRY_AID;
		*entityFlag |= CUSTOM_ENTRY_ITEM;
		*entityFlag |= CUSTOM_ENTRY_VALID_ITEM;
		*entityNamePtr = referenceData.namePtr0098;
		*enabledDistance = ErectusIni::itemSettings.enabledDistance;
		if (CheckFormIdArray(referenceData.formId, ErectusIni::itemSettings.whitelisted, ErectusIni::itemSettings.whitelist, 32))
			*entityFlag |= CUSTOM_ENTRY_WHITELISTED;
		else if (!ErectusIni::itemSettings.enabled)
			*entityFlag |= CUSTOM_ENTRY_INVALID;
		break;
	default:
		if (CheckReferenceItem(referenceData))
		{
			*entityFlag |= CUSTOM_ENTRY_ITEM;
			*entityFlag |= CUSTOM_ENTRY_VALID_ITEM;
			*entityNamePtr = referenceData.namePtr0098;
			*enabledDistance = ErectusIni::itemSettings.enabledDistance;
			if (CheckFormIdArray(referenceData.formId, ErectusIni::itemSettings.whitelisted, ErectusIni::itemSettings.whitelist, 32))
				*entityFlag |= CUSTOM_ENTRY_WHITELISTED;
			else if (!ErectusIni::itemSettings.enabled)
				*entityFlag |= CUSTOM_ENTRY_INVALID;
		}
		else
		{
			*entityFlag |= CUSTOM_ENTRY_ENTITY;
			*entityNamePtr = 0;
			*enabledDistance = ErectusIni::entitySettings.enabledDistance;
			if (CheckFormIdArray(referenceData.formId, ErectusIni::entitySettings.whitelisted, ErectusIni::entitySettings.whitelist, 32))
				*entityFlag |= CUSTOM_ENTRY_WHITELISTED;
			else if (!ErectusIni::entitySettings.enabled || !ErectusIni::entitySettings.drawUnnamed)
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
	if (!Utils::Rpm(entityData.actorCorePtr + 0x70, &actorCoreBufferA, sizeof actorCoreBufferA))
		return false;
	if (!Utils::Valid(actorCoreBufferA))
		return false;

	DWORD64 actorCoreBufferB;
	if (!Utils::Rpm(actorCoreBufferA + 0x8, &actorCoreBufferB, sizeof actorCoreBufferB))
		return false;
	if (!Utils::Valid(actorCoreBufferB))
		return false;

	return Utils::Rpm(actorCoreBufferB, &*actorSnapshotComponentData, sizeof(ActorSnapshotComponent));
}

std::string ErectusMemory::GetEntityName(const DWORD64 ptr)
{
	std::string result{};

	if (!Utils::Valid(ptr))
		return nullptr;

	auto nameLength = 0;
	auto namePtr = ptr;

	if (!Utils::Rpm(namePtr + 0x10, &nameLength, sizeof nameLength))
		return result;
	if (nameLength <= 0 || nameLength > 0x7FFF)
	{
		DWORD64 buffer;
		if (!Utils::Rpm(namePtr + 0x10, &buffer, sizeof buffer))
			return result;
		if (!Utils::Valid(buffer))
			return result;
		if (!Utils::Rpm(buffer + 0x10, &nameLength, sizeof nameLength))
			return result;
		if (nameLength <= 0 || nameLength > 0x7FFF)
			return result;
		namePtr = buffer;
	}

	const auto nameSize = nameLength + 1;
	auto name = std::make_unique<char[]>(nameSize);

	if (!Utils::Rpm(namePtr + 0x18, name.get(), nameSize))
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
	if (!Utils::Rpm(localPlayerPtr, &localPlayer, sizeof localPlayer))
		return false;

	auto bufferList = GetEntityList();
	if (bufferList.empty())
		return false;

	for (auto entityPtr : bufferList)
	{
		if (!Utils::Valid(entityPtr))
			continue;
		if (entityPtr == localPlayerPtr)
			continue;

		TesObjectRefr entityData{};
		if (!Utils::Rpm(entityPtr, &entityData, sizeof entityData))
			continue;
		if (!Utils::Valid(entityData.referencedItemPtr))
			continue;

		TesItem referenceData{};
		if (!Utils::Rpm(entityData.referencedItemPtr, &referenceData, sizeof referenceData))
			continue;
		if (referenceData.formType == static_cast<byte>(FormTypes::TesNpc))
			continue;

		auto entityFlag = CUSTOM_ENTRY_DEFAULT;
		DWORD64 entityNamePtr = 0;
		auto enabledDistance = 0;

		GetCustomEntityData(referenceData, &entityFlag, &entityNamePtr, &enabledDistance, ErectusIni::scrapLooter.scrapOverrideEnabled, ErectusIni::harvester.overrideEnabled);
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
			entityName = fmt::format("{:x}", entityData.formId);
		}

		CustomEntry entry{
			.entityPtr = entityPtr,
			.referencePtr = entityData.referencedItemPtr,
			.entityFormId = entityData.formId,
			.referenceFormId = referenceData.formId,
			.flag = entityFlag,
			.name = entityName
		};

		entities.push_back(entry);
	}
	entityDataBuffer = entities;

	return entityDataBuffer.empty() ? false : true;
}

bool ErectusMemory::UpdateBufferNpcList()
{
	std::vector<CustomEntry> npcs{};

	auto localPlayerPtr = GetLocalPlayerPtr(true);
	if (!Utils::Valid(localPlayerPtr))
		return false;

	TesObjectRefr localPlayer{};
	if (!Utils::Rpm(localPlayerPtr, &localPlayer, sizeof localPlayer))
		return false;

	auto npcPtrs = GetNpcPtrList();
	if (npcPtrs.empty())
		return false;

	for (auto npcPtr : npcPtrs)
	{
		if (!Utils::Valid(npcPtr))
			continue;
		if (npcPtr == localPlayerPtr)
			continue;

		TesObjectRefr entityData{};
		if (!Utils::Rpm(npcPtr, &entityData, sizeof entityData))
			continue;
		if (!Utils::Valid(entityData.referencedItemPtr))
			continue;

		TesItem referenceData{};
		if (!Utils::Rpm(entityData.referencedItemPtr, &referenceData, sizeof referenceData))
			continue;
		if (referenceData.formType != static_cast<BYTE>(FormTypes::TesNpc))
			continue;
		if (referenceData.formId == 0x00000007)
			continue;

		auto entityFlag = CUSTOM_ENTRY_DEFAULT;
		DWORD64 entityNamePtr = 0;
		auto enabledDistance = 0;

		GetCustomEntityData(referenceData, &entityFlag, &entityNamePtr, &enabledDistance,
			ErectusIni::scrapLooter.scrapOverrideEnabled,
			ErectusIni::harvester.overrideEnabled);
		if (entityFlag & CUSTOM_ENTRY_INVALID)
			continue;

		if (BlacklistedNpcFaction(referenceData))
			continue;

		if (ErectusIni::customExtraNpcSettings.useNpcBlacklist)
		{
			if (CheckFormIdArray(referenceData.formId, ErectusIni::customExtraNpcSettings.npcBlacklistEnabled, ErectusIni::customExtraNpcSettings.npcBlacklist, 64))
				continue;
		}

		auto distance = Utils::GetDistance(entityData.position, localPlayer.position);
		auto normalDistance = static_cast<int>(distance * 0.01f);
		if (normalDistance > enabledDistance)
			continue;

		auto entityName = GetEntityName(entityNamePtr);
		if (entityName.empty())
		{
			entityFlag |= CUSTOM_ENTRY_UNNAMED;
			entityName = fmt::format("{:x}", entityData.formId);
		}
		CustomEntry entry{
			.entityPtr = npcPtr,
			.referencePtr = entityData.referencedItemPtr,
			.entityFormId = entityData.formId,
			.referenceFormId = referenceData.formId,
			.flag = entityFlag,
			.name = entityName
		};
		npcs.push_back(entry);
	}

	npcDataBuffer = npcs;

	return npcDataBuffer.empty() ? false : true;
}

std::string ErectusMemory::GetPlayerName(const ClientAccount& clientAccountData)
{
	std::string result = {};
	if (!clientAccountData.nameLength)
		return result;

	const auto playerNameSize = clientAccountData.nameLength + 1;
	auto* playerName = new char[playerNameSize];

	if (clientAccountData.nameLength > 15)
	{
		DWORD64 buffer;
		memcpy(&buffer, clientAccountData.nameData, sizeof buffer);
		if (!Utils::Valid(buffer))
		{
			delete[]playerName;
			playerName = nullptr;
			return result;
		}

		if (!Utils::Rpm(buffer, &*playerName, playerNameSize))
		{
			delete[]playerName;
			playerName = nullptr;
			return result;
		}
	}
	else
		memcpy(playerName, clientAccountData.nameData, playerNameSize);

	if (Utils::GetTextLength(playerName) != clientAccountData.nameLength)
	{
		delete[]playerName;
		playerName = nullptr;
		return result;
	}
	result.assign(playerName);

	delete[]playerName;
	playerName = nullptr;

	return result;
}

bool ErectusMemory::UpdateBufferPlayerList()
{
	std::vector<CustomEntry> players{};

	auto localPlayerPtr = GetLocalPlayerPtr(true);
	if (!Utils::Valid(localPlayerPtr))
		return false;

	DWORD64 falloutMainDataPtr;
	if (!Utils::Rpm(ErectusProcess::exe + OFFSET_MAIN, &falloutMainDataPtr, sizeof falloutMainDataPtr))
		return false;
	if (!Utils::Valid(falloutMainDataPtr))
		return false;

	FalloutMain falloutMainData{};
	if (!Utils::Rpm(falloutMainDataPtr, &falloutMainData, sizeof falloutMainData))
		return false;
	if (!Utils::Valid(falloutMainData.platformSessionManagerPtr))
		return false;

	PlatformSessionManager platformSessionManagerData{};
	if (!Utils::Rpm(falloutMainData.platformSessionManagerPtr, &platformSessionManagerData, sizeof platformSessionManagerData))
		return false;
	if (!Utils::Valid(platformSessionManagerData.clientAccountManagerPtr))
		return false;

	ClientAccountManager clientAccountManagerData{};
	if (!Utils::Rpm(platformSessionManagerData.clientAccountManagerPtr, &clientAccountManagerData, sizeof clientAccountManagerData))
		return false;
	if (!Utils::Valid(clientAccountManagerData.clientAccountArrayPtr))
		return false;

	auto clientAccountArraySize = 0;
	clientAccountArraySize += clientAccountManagerData.clientAccountArraySizeA;
	clientAccountArraySize += clientAccountManagerData.clientAccountArraySizeB;
	if (!clientAccountArraySize)
		return false;

	auto* clientAccountArray = new DWORD64[clientAccountArraySize];
	if (!Utils::Rpm(clientAccountManagerData.clientAccountArrayPtr, &*clientAccountArray, clientAccountArraySize * sizeof(DWORD64)))
	{
		delete[]clientAccountArray;
		clientAccountArray = nullptr;
		return false;
	}

	for (auto i = 0; i < clientAccountArraySize; i++)
	{
		if (!Utils::Valid(clientAccountArray[i]))
			continue;

		ClientAccountBuffer clientAccountBufferData{};
		if (!Utils::Rpm(clientAccountArray[i], &clientAccountBufferData, sizeof clientAccountBufferData))
			continue;
		if (!Utils::Valid(clientAccountBufferData.clientAccountPtr))
			continue;

		ClientAccount clientAccountData{};
		if (!Utils::Rpm(clientAccountBufferData.clientAccountPtr, &clientAccountData, sizeof clientAccountData))
			continue;
		if (clientAccountData.formId == 0xFFFFFFFF)
			continue;

		auto entityPtr = GetPtr(clientAccountData.formId);
		if (!Utils::Valid(entityPtr))
			continue;
		if (entityPtr == localPlayerPtr)
			continue;

		TesObjectRefr entityData{};
		if (!Utils::Rpm(entityPtr, &entityData, sizeof entityData))
			continue;
		if (!Utils::Valid(entityData.referencedItemPtr))
			continue;

		TesItem referenceData{};
		if (!Utils::Rpm(entityData.referencedItemPtr, &referenceData, sizeof referenceData))
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
			.referencePtr = entityData.referencedItemPtr,
			.entityFormId = entityData.formId,
			.referenceFormId = referenceData.formId,
			.flag = entityFlag,
			.name = GetPlayerName(clientAccountData),
		};
		players.push_back(entry);
	}

	delete[]clientAccountArray;
	clientAccountArray = nullptr;

	playerDataBuffer = players;

	return playerDataBuffer.empty() ? false : true;
}

bool ErectusMemory::TargetValid(const TesObjectRefr& entityData, const TesItem& referenceData)
{
	if (referenceData.formType != static_cast<BYTE>(FormTypes::TesNpc))
	{
		return false;
	}

	if (entityData.spawnFlag != 0x02 && !ErectusIni::customTargetSettings.ignoreRenderDistance)
		return false;

	ActorSnapshotComponent actorSnapshotComponentData{};
	if (GetActorSnapshotComponentData(entityData, &actorSnapshotComponentData))
	{
		if (ErectusIni::customTargetSettings.ignoreEssentialNpCs && actorSnapshotComponentData.isEssential)
			return false;
	}

	switch (CheckHealthFlag(entityData.healthFlag))
	{
	case 0x01: //Alive
		return ErectusIni::customTargetSettings.targetLiving;
	case 0x02: //Downed
		return ErectusIni::customTargetSettings.targetDowned;
	case 0x03: //Dead
		return ErectusIni::customTargetSettings.targetDead;
	default: //Unknown
		return ErectusIni::customTargetSettings.targetUnknown;
	}
}

bool ErectusMemory::FloraHarvested(const BYTE harvestFlagA, const BYTE harvestFlagB)
{
	return harvestFlagA >> 5 & 1 || harvestFlagB >> 5 & 1;
}

bool ErectusMemory::RenderCustomEntryA(const CustomEntry& entry, const OverlaySettingsA& settings)
{
	auto health = -1;
	BYTE epicRank = 0;
	auto allowNpc = false;
	if (entry.flag & CUSTOM_ENTRY_NPC)
	{
		TesObjectRefr npcData{};
		if (!Utils::Rpm(entry.entityPtr, &npcData, sizeof npcData))
			return false;

		ActorSnapshotComponent actorSnapshotComponentData{};
		if (GetActorSnapshotComponentData(npcData, &actorSnapshotComponentData))
		{
			health = static_cast<int>(actorSnapshotComponentData.maxHealth + actorSnapshotComponentData.modifiedHealth + actorSnapshotComponentData.lostHealth);
			epicRank = actorSnapshotComponentData.epicRank;
			if (epicRank)
			{
				switch (CheckHealthFlag(npcData.healthFlag))
				{
				case 0x01: //Alive
				case 0x02: //Downed
				case 0x03: //Dead
					switch (epicRank)
					{
					case 1:
						allowNpc = ErectusIni::customLegendarySettings.overrideLivingOneStar;
						break;
					case 2:
						allowNpc = ErectusIni::customLegendarySettings.overrideLivingTwoStar;
						break;
					case 3:
						allowNpc = ErectusIni::customLegendarySettings.overrideLivingThreeStar;
						break;
					default:
						break;
					}
				default:
					break;
				}
			}
		}
	}

	if (!settings.enabled && !allowNpc)
		return false;

	if (!settings.drawEnabled && !settings.drawDisabled)
		return false;

	if (settings.enabledAlpha == 0.0f && settings.disabledAlpha == 0.0f)
		return false;

	if (!settings.drawNamed && !settings.drawUnnamed)
		return false;

	TesObjectRefr entityData{};
	if (!Utils::Rpm(entry.entityPtr, &entityData, sizeof entityData))
		return false;

	if (entry.flag & CUSTOM_ENTRY_PLAYER)
	{
		ActorSnapshotComponent actorSnapshotComponentData{};
		if (GetActorSnapshotComponentData(entityData, &actorSnapshotComponentData))
			health = static_cast<int>(actorSnapshotComponentData.maxHealth + actorSnapshotComponentData.modifiedHealth + actorSnapshotComponentData.lostHealth);
	}

	if (entry.flag & CUSTOM_ENTRY_UNNAMED)
	{
		if (!settings.drawUnnamed)
			return false;
	}
	else
	{
		if (!settings.drawNamed)
			return false;
	}

	auto alpha = 0.f;

	if (entityData.spawnFlag == 0x02)
	{
		if (settings.drawEnabled)
			alpha = settings.enabledAlpha;
	}
	else
	{
		if (settings.drawDisabled)
			alpha = settings.disabledAlpha;
	}

	if (alpha == 0.f)
		return false;

	auto showHealthText = false;

	const float* color = nullptr;

	auto legendaryAlpha = 1.0f;

	switch (CheckHealthFlag(entityData.healthFlag))
	{
	case 0x01: //Alive
		showHealthText = settings.showHealth;
		if (allowNpc)
		{
			switch (epicRank)
			{
			case 1:
				color = ErectusIni::customLegendarySettings.livingOneStarColor;
				if (entityData.spawnFlag == 0x02)
					alpha = legendaryAlpha;
				break;
			case 2:
				color = ErectusIni::customLegendarySettings.livingTwoStarColor;
				if (entityData.spawnFlag == 0x02)
					alpha = legendaryAlpha;
				break;
			case 3:
				color = ErectusIni::customLegendarySettings.livingThreeStarColor;
				if (entityData.spawnFlag == 0x02)
					alpha = legendaryAlpha;
				break;
			default:
				break;
			}
		}
		else if (settings.drawAlive)
			color = settings.aliveColor;
		break;
	case 0x02: //Downed
		showHealthText = settings.showHealth;
		if (allowNpc)
		{
			switch (epicRank)
			{
			case 1:
				color = ErectusIni::customLegendarySettings.livingOneStarColor;
				if (entityData.spawnFlag == 0x02)
					alpha = legendaryAlpha;
				break;
			case 2:
				color = ErectusIni::customLegendarySettings.livingTwoStarColor;
				if (entityData.spawnFlag == 0x02)
					alpha = legendaryAlpha;
				break;
			case 3:
				color = ErectusIni::customLegendarySettings.livingThreeStarColor;
				if (entityData.spawnFlag == 0x02)
					alpha = legendaryAlpha;
				break;
			default:
				break;
			}
		}
		else if (settings.drawDowned)
			color = settings.downedColor;
		break;
	case 0x03: //Dead
		showHealthText = settings.showDeadHealth;
		if (allowNpc)
		{
			switch (epicRank)
			{
			case 1:
				color = ErectusIni::customLegendarySettings.deadOneStarColor;
				if (entityData.spawnFlag == 0x02)
					alpha = legendaryAlpha;
				break;
			case 2:
				color = ErectusIni::customLegendarySettings.deadTwoStarColor;
				if (entityData.spawnFlag == 0x02)
					alpha = legendaryAlpha;
				break;
			case 3:
				color = ErectusIni::customLegendarySettings.deadThreeStarColor;
				if (entityData.spawnFlag == 0x02)
					alpha = legendaryAlpha;
				break;
			default:
				break;
			}
		}
		else if (settings.drawDead)
			color = settings.deadColor;
		break;
	default: //Unknown
		showHealthText = settings.showHealth;
		if (settings.drawUnknown)
			color = settings.unknownColor;
		break;
	}

	if (color == nullptr)
		return false;

	auto cameraPtr = GetCameraPtr();
	if (!Utils::Valid(cameraPtr))
		return false;

	Camera cameraData{};
	if (!Utils::Rpm(cameraPtr, &cameraData, sizeof cameraData))
		return false;

	auto distance = Utils::GetDistance(entityData.position, cameraData.origin);
	auto normalDistance = static_cast<int>(distance * 0.01f);
	if (normalDistance > settings.enabledDistance)
		return false;

	if (entry.flag & CUSTOM_ENTRY_PLAYER && ErectusIni::customTargetSettings.lockPlayers || entry.flag & CUSTOM_ENTRY_NPC && ErectusIni::customTargetSettings.lockNpCs)
	{
		TesItem referenceData{};
		if (Utils::Rpm(entityData.referencedItemPtr, &referenceData, sizeof referenceData))
		{
			if (TargetValid(entityData, referenceData))
			{
				if (entry.entityPtr == targetLockingPtr)
				{
					targetLockingValid = true;
					color = ErectusIni::customTargetSettings.lockingColor;
				}
				else if (targetLockingKeyPressed && !targetLockingCooldown)
				{
					auto degrees = Utils::GetDegrees(entityData.position, cameraData.forward, cameraData.origin);
					if (degrees < targetLockingClosestDegrees)
					{
						targetLockingClosestDegrees = degrees;
						targetLockingClosestPtr = entry.entityPtr;
					}
				}
			}
		}
	}

	float screen[2] = { 0.0f, 0.0f };
	if (!Utils::WorldToScreen(cameraData.view, entityData.position, screen))
		return false;

	std::string itemText;
	if (settings.showName && showHealthText && settings.showDistance) //Name, Health, Distance
		itemText = fmt::format("{0}\n{1:d} hp [{2:d} m]", entry.name, health, normalDistance);
	else if (settings.showName && showHealthText && !settings.showDistance) //Name, Health
		itemText = fmt::format("{0}\n{1:d} hp", entry.name, health);
	else if (settings.showName && !showHealthText && settings.showDistance) //Name, Distance
		itemText = fmt::format("{0}\n[{1:d} m]", entry.name, normalDistance);
	else if (!settings.showName && showHealthText && settings.showDistance) //Health, Distance
		itemText = fmt::format("{0:d} hp [{1:d} m]", health, normalDistance);
	else if (settings.showName && !showHealthText && !settings.showDistance) //Name
		itemText = entry.name;
	else if (!settings.showName && showHealthText && !settings.showDistance) //Health
		itemText = fmt::format("{:d} hp", health);
	else if (!settings.showName && !showHealthText && settings.showDistance) //Distance
		itemText = fmt::format("[{:d} m]", normalDistance);

	if (!itemText.empty())
	{
		if (ErectusIni::customUtilitySettings.debugEsp)
			itemText = fmt::format("{0:16x}\n{1:08x}\n{2:16x}\n{3:08x}", entry.entityPtr, entry.entityFormId, entry.referencePtr, entry.referenceFormId);

		Renderer::DrawTextA(itemText.c_str(), settings.textShadowed, settings.textCentered, screen, color, alpha);
	}

	return true;
}

bool ErectusMemory::RenderCustomEntryB(const CustomEntry& entry, const OverlaySettingsB& settings)
{
	if (!(entry.flag & CUSTOM_ENTRY_WHITELISTED) && !settings.enabled)
		return false;

	if (!settings.drawEnabled && !settings.drawDisabled)
		return false;

	if (settings.enabledAlpha == 0.0f && settings.disabledAlpha == 0.0f)
		return false;

	if (!settings.drawNamed && !settings.drawUnnamed)
		return false;

	TesObjectRefr entityData{};
	if (!Utils::Rpm(entry.entityPtr, &entityData, sizeof entityData))
		return false;

	if (entry.flag & CUSTOM_ENTRY_UNNAMED)
	{
		if (!settings.drawUnnamed)
			return false;
	}
	else if (!settings.drawNamed)
		return false;

	if (entry.flag & CUSTOM_ENTRY_PLAN)
	{
		if (!ErectusIni::customKnownRecipeSettings.knownRecipesEnabled && !ErectusIni::customKnownRecipeSettings.unknownRecipesEnabled)
			return false;

		if (!(entry.flag & CUSTOM_ENTRY_FAILED_RECIPE))
		{
			if (!ErectusIni::customKnownRecipeSettings.knownRecipesEnabled && entry.flag & CUSTOM_ENTRY_KNOWN_RECIPE)
				return false;
			if (!ErectusIni::customKnownRecipeSettings.unknownRecipesEnabled && entry.flag & CUSTOM_ENTRY_UNKNOWN_RECIPE)
				return false;
		}
	}

	auto alpha = 0.f;

	if (entityData.spawnFlag == 0x02)
	{
		if (settings.drawEnabled)
		{
			if (entry.flag & CUSTOM_ENTRY_FLORA)
			{
				if (!FloraHarvested(entityData.harvestFlagA, entityData.harvestFlagB))
					alpha = settings.enabledAlpha;
				else if (settings.drawDisabled)
					alpha = settings.disabledAlpha;
			}
			else
				alpha = settings.enabledAlpha;
		}
	}
	else
	{
		if (settings.drawDisabled)
			alpha = settings.disabledAlpha;
	}

	if (alpha == 0.f)
		return false;

	const auto cameraPtr = GetCameraPtr();
	if (!Utils::Valid(cameraPtr))
		return false;

	Camera cameraData{};
	if (!Utils::Rpm(cameraPtr, &cameraData, sizeof cameraData))
		return false;

	const auto distance = Utils::GetDistance(entityData.position, cameraData.origin);
	const auto normalDistance = static_cast<int>(distance * 0.01f);
	if (normalDistance > settings.enabledDistance)
		return false;

	float screen[2] = { 0.0f, 0.0f };
	if (!Utils::WorldToScreen(cameraData.view, entityData.position, screen))
		return false;

	std::string itemText{};
	if (settings.showName && settings.showDistance)
		itemText = fmt::format("{0}\n[{1:d} m]", entry.name, normalDistance);
	else if (settings.showName && !settings.showDistance)
		itemText = entry.name;
	else if (!settings.showName && settings.showDistance)
		itemText = fmt::format("[{0:d} m]", normalDistance);

	if (!itemText.empty())
	{
		if (ErectusIni::customUtilitySettings.debugEsp)
			itemText = fmt::format("{0:16x}\n{1:08x}\n{2:16x}\n{3:08x}", entry.entityPtr, entry.entityFormId, entry.referencePtr, entry.referenceFormId);

		Renderer::DrawTextA(itemText.c_str(), settings.textShadowed, settings.textCentered, screen, settings.color, alpha);
	}

	return true;
}

bool ErectusMemory::RenderCustomEntityList()
{
	auto entities = entityDataBuffer;
	for (const auto& entity : entities)
	{
		if (entity.flag & CUSTOM_ENTRY_ENTITY)
			RenderCustomEntryB(entity, ErectusIni::entitySettings);
		else if (entity.flag & CUSTOM_ENTRY_JUNK)
			RenderCustomEntryB(entity, ErectusIni::junkSettings);
		else if (entity.flag & CUSTOM_ENTRY_ITEM)
			RenderCustomEntryB(entity, ErectusIni::itemSettings);
		else if (entity.flag & CUSTOM_ENTRY_CONTAINER)
			RenderCustomEntryB(entity, ErectusIni::containerSettings);
		else if (entity.flag & CUSTOM_ENTRY_PLAN)
			RenderCustomEntryB(entity, ErectusIni::planSettings);
		else if (entity.flag & CUSTOM_ENTRY_MAGAZINE)
			RenderCustomEntryB(entity, ErectusIni::magazineSettings);
		else if (entity.flag & CUSTOM_ENTRY_BOBBLEHEAD)
			RenderCustomEntryB(entity, ErectusIni::bobbleheadSettings);
		else if (entity.flag & CUSTOM_ENTRY_FLORA)
			RenderCustomEntryB(entity, ErectusIni::floraSettings);
	}
	return true;
}

bool ErectusMemory::RenderCustomNpcList()
{
	auto npcs = npcDataBuffer;
	for (const auto& npc : npcs)
	{
		if (npc.flag & CUSTOM_ENTRY_NPC)
			RenderCustomEntryA(npc, ErectusIni::npcSettings);
	}
	return true;
}

bool ErectusMemory::RenderCustomPlayerList()
{
	auto players = playerDataBuffer;
	for (const auto& player : players) {
		if (player.flag & CUSTOM_ENTRY_PLAYER)
			RenderCustomEntryA(player, ErectusIni::playerSettings);
	}
	return true;
}

bool ErectusMemory::MessagePatcher(const bool state)
{
	BYTE fakeMessagesCheck[2];
	if (!Utils::Rpm(ErectusProcess::exe + OFFSET_FAKE_MESSAGE, &fakeMessagesCheck, sizeof fakeMessagesCheck))
		return false;

	BYTE fakeMessagesEnabled[] = { 0xB0, 0x01 };
	BYTE fakeMessagesDisabled[] = { 0x32, 0xC0 };

	if (!memcmp(fakeMessagesCheck, fakeMessagesEnabled, sizeof fakeMessagesEnabled))
	{
		if (state)
			return true;

		return Utils::Wpm(ErectusProcess::exe + OFFSET_FAKE_MESSAGE, &fakeMessagesDisabled, sizeof fakeMessagesDisabled);
	}

	if (!memcmp(fakeMessagesCheck, fakeMessagesDisabled, sizeof fakeMessagesDisabled))
	{
		if (state)
			return Utils::Wpm(ErectusProcess::exe + OFFSET_FAKE_MESSAGE, &fakeMessagesEnabled, sizeof fakeMessagesEnabled);
		return true;
	}

	return false;
}

bool ErectusMemory::SendMessageToServer(void* message, const size_t size)
{
	if (!MessagePatcher(allowMessages))
		return false;

	if (!allowMessages)
		return false;

	const auto allocSize = size + sizeof(ExternalFunction);
	const auto allocAddress = Utils::AllocEx(allocSize);
	if (allocAddress == 0)
		return false;

	ExternalFunction externalFunctionData;
	externalFunctionData.address = ErectusProcess::exe + OFFSET_MESSAGE_SENDER;
	externalFunctionData.rcx = allocAddress + sizeof(ExternalFunction);
	externalFunctionData.rdx = 0;
	externalFunctionData.r8 = 0;
	externalFunctionData.r9 = 0;

	auto* pageData = new BYTE[allocSize];
	memset(pageData, 0x00, allocSize);
	memcpy(pageData, &externalFunctionData, sizeof externalFunctionData);
	memcpy(&pageData[sizeof(ExternalFunction)], message, size);
	const auto written = Utils::Wpm(allocAddress, &*pageData, allocSize);

	delete[]pageData;
	pageData = nullptr;

	if (!written)
	{
		Utils::FreeEx(allocAddress);
		return false;
	}

	const auto paramAddress = allocAddress + sizeof ExternalFunction::ASM;
	auto* const thread = CreateRemoteThread(ErectusProcess::handle, nullptr, 0, LPTHREAD_START_ROUTINE(allocAddress),
		LPVOID(paramAddress), 0, nullptr);

	if (thread == nullptr)
	{
		Utils::FreeEx(allocAddress);
		return false;
	}

	const auto threadResult = WaitForSingleObject(thread, 3000);
	CloseHandle(thread);

	if (threadResult == WAIT_TIMEOUT)
		return false;

	Utils::FreeEx(allocAddress);
	return true;
}

bool ErectusMemory::LootScrap()
{
	if (!MessagePatcher(allowMessages))
		return false;

	if (!allowMessages)
		return false;

	if (!CheckScrapList())
		return false;

	auto localPlayerPtr = GetLocalPlayerPtr(true);
	if (!Utils::Valid(localPlayerPtr))
	{
		ErectusIni::scrapLooter.autoLootingEnabled = false;
		return false;
	}

	TesObjectRefr localPlayer{};
	if (!Utils::Rpm(localPlayerPtr, &localPlayer, sizeof localPlayer))
		return false;

	auto bufferList = GetEntityList();
	if (bufferList.empty()) return false;

	for (auto item : bufferList)
	{
		if (!Utils::Valid(item))
			continue;
		if (item == localPlayerPtr)
			continue;

		TesObjectRefr entityData{};
		if (!Utils::Rpm(item, &entityData, sizeof entityData))
			continue;
		if (!Utils::Valid(entityData.referencedItemPtr))
			continue;

		if (entityData.spawnFlag != 0x02)
			continue;

		TesItem referenceData{};
		if (!Utils::Rpm(entityData.referencedItemPtr, &referenceData, sizeof referenceData))
			continue;
		if (referenceData.formId == 0x00000007)
			continue;

		auto entityFlag = CUSTOM_ENTRY_DEFAULT;
		DWORD64 entityNamePtr = 0;
		auto enabledDistance = 0;

		GetCustomEntityData(referenceData, &entityFlag, &entityNamePtr, &enabledDistance, true, false);
		if (!(entityFlag & CUSTOM_ENTRY_VALID_SCRAP))
			continue;

		auto distance = Utils::GetDistance(entityData.position, localPlayer.position);
		auto normalDistance = static_cast<int>(distance * 0.01f);

		if (normalDistance > ErectusIni::scrapLooter.maxDistance)
			continue;

		RequestActivateRefMessage requestActivateRefMessageData{};
		requestActivateRefMessageData.vtable = ErectusProcess::exe + VTABLE_REQUESTACTIVATEREFMSG;
		requestActivateRefMessageData.formId = entityData.formId;
		requestActivateRefMessageData.choice = 0xFF;
		requestActivateRefMessageData.forceActivate = 0;
		SendMessageToServer(&requestActivateRefMessageData, sizeof requestActivateRefMessageData);
	}

	return true;
}

bool ErectusMemory::CheckItemLooterSettings()
{
	if (ErectusIni::itemLooter.lootWeaponsEnabled && ErectusIni::itemLooter.lootWeaponsDistance > 0)
		return true;
	if (ErectusIni::itemLooter.lootArmorEnabled && ErectusIni::itemLooter.lootArmorDistance > 0)
		return true;
	if (ErectusIni::itemLooter.lootAmmoEnabled && ErectusIni::itemLooter.lootAmmoDistance > 0)
		return true;
	if (ErectusIni::itemLooter.lootModsEnabled && ErectusIni::itemLooter.lootModsDistance > 0)
		return true;
	if (ErectusIni::itemLooter.lootMagazinesEnabled && ErectusIni::itemLooter.lootMagazinesDistance > 0)
		return true;
	if (ErectusIni::itemLooter.lootBobbleheadsEnabled && ErectusIni::itemLooter.lootBobbleheadsDistance > 0)
		return true;
	if (ErectusIni::itemLooter.lootAidEnabled && ErectusIni::itemLooter.lootAidDistance > 0)
		return true;
	if (ErectusIni::itemLooter.lootKnownPlansEnabled && ErectusIni::itemLooter.lootKnownPlansDistance > 0)
		return true;
	if (ErectusIni::itemLooter.lootUnknownPlansEnabled && ErectusIni::itemLooter.lootUnknownPlansDistance > 0)
		return true;
	if (ErectusIni::itemLooter.lootMiscEnabled && ErectusIni::itemLooter.lootMiscDistance > 0)
		return true;
	if (ErectusIni::itemLooter.lootUnlistedEnabled && ErectusIni::itemLooter.lootUnlistedDistance > 0)
		return true;
	if (ErectusIni::itemLooter.lootListEnabled && ErectusIni::itemLooter.lootListDistance > 0)
		return CheckItemLooterList();

	return false;
}

bool ErectusMemory::CheckOnlyUseItemLooterList()
{
	if (ErectusIni::itemLooter.lootWeaponsEnabled && ErectusIni::itemLooter.lootWeaponsDistance > 0)
		return false;
	if (ErectusIni::itemLooter.lootArmorEnabled && ErectusIni::itemLooter.lootArmorDistance > 0)
		return false;
	if (ErectusIni::itemLooter.lootAmmoEnabled && ErectusIni::itemLooter.lootAmmoDistance > 0)
		return false;
	if (ErectusIni::itemLooter.lootModsEnabled && ErectusIni::itemLooter.lootModsDistance > 0)
		return false;
	if (ErectusIni::itemLooter.lootMagazinesEnabled && ErectusIni::itemLooter.lootMagazinesDistance > 0)
		return false;
	if (ErectusIni::itemLooter.lootBobbleheadsEnabled && ErectusIni::itemLooter.lootBobbleheadsDistance > 0)
		return false;
	if (ErectusIni::itemLooter.lootAidEnabled && ErectusIni::itemLooter.lootAidDistance > 0)
		return false;
	if (ErectusIni::itemLooter.lootKnownPlansEnabled && ErectusIni::itemLooter.lootKnownPlansDistance > 0)
		return false;
	if (ErectusIni::itemLooter.lootUnknownPlansEnabled && ErectusIni::itemLooter.lootUnknownPlansDistance > 0)
		return false;
	if (ErectusIni::itemLooter.lootMiscEnabled && ErectusIni::itemLooter.lootMiscDistance > 0)
		return false;
	if (ErectusIni::itemLooter.lootUnlistedEnabled && ErectusIni::itemLooter.lootUnlistedDistance > 0)
		return false;
	if (ErectusIni::itemLooter.lootListEnabled && ErectusIni::itemLooter.lootListDistance > 0)
		return CheckItemLooterList();

	return false;
}

bool ErectusMemory::CheckEnabledItem(const DWORD formId, const DWORD64 entityFlag, const int normalDistance)
{
	if (ErectusIni::itemLooter.lootListEnabled)
	{
		if (CheckFormIdArray(formId, ErectusIni::itemLooter.enabledList, ErectusIni::itemLooter.formIdList, 100))
		{
			if (normalDistance <= ErectusIni::itemLooter.lootListDistance)
				return true;
		}
	}

	if (entityFlag & CUSTOM_ENTRY_WEAPON && normalDistance <= ErectusIni::itemLooter.lootWeaponsDistance)
		return ErectusIni::itemLooter.lootWeaponsEnabled;
	if (entityFlag & CUSTOM_ENTRY_ARMOR && normalDistance <= ErectusIni::itemLooter.lootArmorDistance)
		return ErectusIni::itemLooter.lootArmorEnabled;
	if (entityFlag & CUSTOM_ENTRY_AMMO && normalDistance <= ErectusIni::itemLooter.lootAmmoDistance)
		return ErectusIni::itemLooter.lootAmmoEnabled;
	if (entityFlag & CUSTOM_ENTRY_MOD && normalDistance <= ErectusIni::itemLooter.lootModsDistance)
		return ErectusIni::itemLooter.lootModsEnabled;
	if (entityFlag & CUSTOM_ENTRY_MAGAZINE && normalDistance <= ErectusIni::itemLooter.lootMagazinesDistance)
		return ErectusIni::itemLooter.lootMagazinesEnabled;
	if (entityFlag & CUSTOM_ENTRY_BOBBLEHEAD && normalDistance <= ErectusIni::itemLooter.lootBobbleheadsDistance)
		return ErectusIni::itemLooter.lootBobbleheadsEnabled;
	if (entityFlag & CUSTOM_ENTRY_AID && normalDistance <= ErectusIni::itemLooter.lootAidDistance)
		return ErectusIni::itemLooter.lootAidEnabled;
	if (entityFlag & CUSTOM_ENTRY_KNOWN_RECIPE && normalDistance <= ErectusIni::itemLooter.lootKnownPlansDistance)
		return ErectusIni::itemLooter.lootKnownPlansEnabled;
	if (entityFlag & CUSTOM_ENTRY_UNKNOWN_RECIPE && normalDistance <= ErectusIni::itemLooter.lootUnknownPlansDistance)
		return ErectusIni::itemLooter.lootUnknownPlansEnabled;
	if (entityFlag & CUSTOM_ENTRY_FAILED_RECIPE && normalDistance <= ErectusIni::itemLooter.lootUnknownPlansDistance)
		return ErectusIni::itemLooter.lootUnknownPlansEnabled;
	if (entityFlag & CUSTOM_ENTRY_MISC && normalDistance <= ErectusIni::itemLooter.lootMiscDistance)
		return ErectusIni::itemLooter.lootMiscEnabled;
	if (ErectusIni::itemLooter.lootUnlistedEnabled && normalDistance <= ErectusIni::itemLooter.lootUnlistedDistance)
		return true;

	return false;
}

bool ErectusMemory::LootItems()
{
	if (!MessagePatcher(allowMessages))
		return false;

	if (!allowMessages)
		return false;

	if (!CheckItemLooterSettings())
		return false;

	auto localPlayerPtr = GetLocalPlayerPtr(true);
	if (!Utils::Valid(localPlayerPtr))
	{
		ErectusIni::itemLooter.autoLootingEnabled = false;
		return false;
	}

	TesObjectRefr localPlayer{};
	if (!Utils::Rpm(localPlayerPtr, &localPlayer, sizeof localPlayer))
		return false;


	auto bufferList = GetEntityList();
	if (bufferList.empty())
		return false;

	auto onlyUseItemLooterList = CheckOnlyUseItemLooterList();
	auto useItemLooterBlacklist = CheckItemLooterBlacklist();

	for (auto item : bufferList)
	{
		if (!Utils::Valid(item))
			continue;
		if (item == localPlayerPtr)
			continue;

		TesObjectRefr entityData{};
		if (!Utils::Rpm(item, &entityData, sizeof entityData))
			continue;
		if (!Utils::Valid(entityData.referencedItemPtr))
			continue;

		if (entityData.spawnFlag != 0x02)
			continue;

		TesItem referenceData{};
		if (!Utils::Rpm(entityData.referencedItemPtr, &referenceData, sizeof referenceData))
			continue;

		if (useItemLooterBlacklist)
		{
			if (CheckFormIdArray(referenceData.formId, ErectusIni::itemLooter.blacklistEnabled, ErectusIni::itemLooter.blacklist, 64))
				continue;
		}

		if (onlyUseItemLooterList)
		{
			if (!CheckFormIdArray(referenceData.formId, ErectusIni::itemLooter.enabledList, ErectusIni::itemLooter.formIdList, 100))
				continue;
		}

		auto entityFlag = CUSTOM_ENTRY_DEFAULT;
		DWORD64 entityNamePtr = 0;
		auto enabledDistance = 0;

		GetCustomEntityData(referenceData, &entityFlag, &entityNamePtr, &enabledDistance, false, false);
		if (!(entityFlag & CUSTOM_ENTRY_VALID_ITEM))
			continue;
		if (entityFlag & CUSTOM_ENTRY_JUNK)
			continue;

		auto distance = Utils::GetDistance(entityData.position, localPlayer.position);
		auto normalDistance = static_cast<int>(distance * 0.01f);

		if (!onlyUseItemLooterList)
		{
			if (!CheckEnabledItem(referenceData.formId, entityFlag, normalDistance))
				continue;
		}
		else
		{
			if (normalDistance > ErectusIni::itemLooter.lootListDistance)
				continue;
		}

		RequestActivateRefMessage requestActivateRefMessageData{};
		requestActivateRefMessageData.vtable = ErectusProcess::exe + VTABLE_REQUESTACTIVATEREFMSG;
		requestActivateRefMessageData.formId = entityData.formId;
		requestActivateRefMessageData.choice = 0xFF;
		requestActivateRefMessageData.forceActivate = 0;
		SendMessageToServer(&requestActivateRefMessageData, sizeof requestActivateRefMessageData);
	}

	return true;
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
	if (!Utils::Rpm(ErectusProcess::exe + OFFSET_DATA_HANDLER, &dataHandlerPtr, sizeof dataHandlerPtr))
		return false;
	if (!Utils::Valid(dataHandlerPtr))
		return false;

	ReferenceList weaponList{};
	if (!Utils::Rpm(dataHandlerPtr + 0x580, &weaponList, sizeof weaponList))
		return false;
	if (!Utils::Valid(weaponList.arrayPtr) || !weaponList.arraySize || weaponList.arraySize > 0x7FFF)
		return false;

	auto* weaponPtrArray = new DWORD64[weaponList.arraySize];
	if (!Utils::Rpm(weaponList.arrayPtr, &*weaponPtrArray, weaponList.arraySize * sizeof(DWORD64)))
	{
		delete[]weaponPtrArray;
		weaponPtrArray = nullptr;
		return false;
	}

	oldWeaponList = new OldWeapon[weaponList.arraySize];
	oldWeaponListSize = weaponList.arraySize;

	for (auto i = 0; i < weaponList.arraySize; i++)
	{
		oldWeaponList[i].weaponData = nullptr;
		oldWeaponList[i].aimModelData = nullptr;

		if (!Utils::Valid(weaponPtrArray[i]))
			continue;

		Weapon weaponData{};
		if (!Utils::Rpm(weaponPtrArray[i], &weaponData, sizeof weaponData))
			continue;

		oldWeaponList[i].weaponData = new Weapon;
		memcpy(&*oldWeaponList[i].weaponData, &weaponData, sizeof weaponData);

		if (!Utils::Valid(weaponData.aimModelPtr))
			continue;

		AimModel aimModelData{};
		if (!Utils::Rpm(weaponData.aimModelPtr, &aimModelData, sizeof aimModelData))
			continue;

		oldWeaponList[i].aimModelData = new AimModel;
		memcpy(&*oldWeaponList[i].aimModelData, &aimModelData, sizeof aimModelData);
	}

	delete[]weaponPtrArray;
	weaponPtrArray = nullptr;
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
	auto bufferSettings = ErectusIni::customWeaponSettings;
	bufferSettings.capacity = ErectusIni::defaultWeaponSettings.capacity;
	bufferSettings.speed = ErectusIni::defaultWeaponSettings.speed;
	bufferSettings.reach = ErectusIni::defaultWeaponSettings.reach;
	if (!memcmp(&bufferSettings, &ErectusIni::defaultWeaponSettings, sizeof(WeaponSettings)))
		return false;

	return true;
}

bool ErectusMemory::EditWeapon(const int index, const bool revertWeaponData)
{
	DWORD64 dataHandlerPtr;
	if (!Utils::Rpm(ErectusProcess::exe + OFFSET_DATA_HANDLER, &dataHandlerPtr, sizeof dataHandlerPtr))
		return false;
	if (!Utils::Valid(dataHandlerPtr))
		return false;

	ReferenceList weaponList{};
	if (!Utils::Rpm(dataHandlerPtr + 0x580, &weaponList, sizeof weaponList))
		return false;
	if (!Utils::Valid(weaponList.arrayPtr) || !weaponList.arraySize || weaponList.arraySize > 0x7FFF)
		return false;

	DWORD64 weaponPtr;
	if (!Utils::Rpm(weaponList.arrayPtr + index * sizeof(DWORD64), &weaponPtr, sizeof weaponPtr))
		return false;
	if (!Utils::Valid(weaponPtr)) return false;

	Weapon weaponData{};
	if (!Utils::Rpm(weaponPtr, &weaponData, sizeof weaponData))
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

	if (!revertWeaponData && ErectusIni::customWeaponSettings.instantReload)
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

	if (!revertWeaponData && ErectusIni::customWeaponSettings.automaticflag)
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

	if (!revertWeaponData && ErectusIni::customWeaponSettings.capacityEnabled)
	{
		if (weaponData.capacity != static_cast<short>(ErectusIni::customWeaponSettings.capacity))
		{
			weaponData.capacity = static_cast<short>(ErectusIni::customWeaponSettings.capacity);
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

	if (!revertWeaponData && ErectusIni::customWeaponSettings.speedEnabled)
	{
		if (weaponData.speed != ErectusIni::customWeaponSettings.speed)
		{
			weaponData.speed = ErectusIni::customWeaponSettings.speed;
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

	if (!revertWeaponData && ErectusIni::customWeaponSettings.reachEnabled)
	{
		if (weaponData.reach != ErectusIni::customWeaponSettings.reach)
		{
			weaponData.reach = ErectusIni::customWeaponSettings.reach;
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
		Utils::Wpm(weaponPtr, &weaponData, sizeof weaponData);

	if (!Utils::Valid(weaponData.aimModelPtr))
		return true;

	if (oldWeaponList[currentWeaponIndex].aimModelData == nullptr)
		return false;

	AimModel aimModelData{};
	if (!Utils::Rpm(weaponData.aimModelPtr, &aimModelData, sizeof aimModelData))
		return false;

	BYTE noRecoil[sizeof AimModel::recoilData];
	memset(noRecoil, 0x00, sizeof noRecoil);

	BYTE noSpread[sizeof AimModel::spreadData];
	memset(noSpread, 0x00, sizeof noSpread);

	if (!revertWeaponData && ErectusIni::customWeaponSettings.noRecoil)
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

	if (!revertWeaponData && ErectusIni::customWeaponSettings.noSpread)
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

	if (!revertWeaponData && ErectusIni::customWeaponSettings.noSway)
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
		return Utils::Wpm(weaponData.aimModelPtr, &aimModelData, sizeof aimModelData);

	return true;
}

bool ErectusMemory::InfiniteAmmo(const bool state)
{
	BYTE infiniteAmmoOn[] = { 0x66, 0xB8, 0xE7, 0x03 };
	BYTE infiniteAmmoOff[] = { 0x8B, 0x44, 0x24, 0x50 };
	BYTE infiniteAmmoCheck[sizeof infiniteAmmoOff];

	if (!Utils::Rpm(ErectusProcess::exe + OFFSET_INFINITE_AMMO, &infiniteAmmoCheck, sizeof infiniteAmmoCheck))
		return false;

	if (state && !memcmp(infiniteAmmoCheck, infiniteAmmoOff, sizeof infiniteAmmoOff))
		return Utils::Wpm(ErectusProcess::exe + OFFSET_INFINITE_AMMO, &infiniteAmmoOn, sizeof infiniteAmmoOn);
	if (!state && !memcmp(infiniteAmmoCheck, infiniteAmmoOn, sizeof infiniteAmmoOn))
		return Utils::Wpm(ErectusProcess::exe + OFFSET_INFINITE_AMMO, &infiniteAmmoOff, sizeof infiniteAmmoOff);
	return false;
}

bool ErectusMemory::LockedTargetValid(bool* isPlayer)
{
	if (!Utils::Valid(targetLockingPtr))
		return false;

	TesObjectRefr entityData{};
	if (!Utils::Rpm(targetLockingPtr, &entityData, sizeof entityData))
		return false;
	if (!Utils::Valid(entityData.referencedItemPtr))
		return false;

	TesItem referenceData{};
	if (!Utils::Rpm(entityData.referencedItemPtr, &referenceData, sizeof referenceData))
		return false;
	const auto result = TargetValid(entityData, referenceData);

	if (referenceData.formId == 0x00000007)
		*isPlayer = true;
	else
		*isPlayer = false;

	return result;
}

bool ErectusMemory::DamageRedirection(DWORD64* targetingPage, bool* targetingPageValid, const bool isExiting, const bool state)
{
	BYTE pageJmpOn[] = { 0x48, 0xBB, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xE3 };
	BYTE pageJmpOff[] = { 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC };
	BYTE pageJmpCheck[sizeof pageJmpOff];

	BYTE redirectionOn[] = { 0xE9, 0x69, 0xFE, 0xFF, 0xFF };
	BYTE redirectionOff[] = { 0x48, 0x8B, 0x5C, 0x24, 0x50 };
	BYTE redirectionCheck[sizeof redirectionOff];

	if (!Utils::Rpm(ErectusProcess::exe + OFFSET_REDIRECTION_JMP, &pageJmpCheck, sizeof pageJmpCheck))
		return false;

	DWORD64 pageCheck;
	memcpy(&pageCheck, &pageJmpCheck[2], sizeof pageCheck);
	if (Utils::Valid(pageCheck) && pageCheck != *targetingPage)
	{
		BYTE pageOpcode[] = { 0x48, 0xBB, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x00 };
		BYTE pageOpcodeCheck[sizeof pageOpcode];
		if (!Utils::Rpm(pageCheck, &pageOpcodeCheck, sizeof pageOpcodeCheck))
			return false;
		if (memcmp(pageOpcodeCheck, pageOpcode, sizeof pageOpcode) != 0)
			return false;
		if (!Utils::Wpm(ErectusProcess::exe + OFFSET_REDIRECTION_JMP, &pageJmpOff, sizeof pageJmpOff))
			return false;
		if (!Utils::FreeEx(pageCheck))
			return false;
	}

	if (!*targetingPage)
	{
		const auto page = Utils::AllocEx(sizeof(Opk));
		if (!page)
			return false;
		*targetingPage = page;
	}

	if (!*targetingPageValid)
	{
		TargetLocking targetLockingData;
		targetLockingData.targetLockingPtr = targetLockingPtr;
		auto originalFunction = ErectusProcess::exe + OFFSET_REDIRECTION + sizeof redirectionOff;
		DWORD64 originalFunctionCheck;
		if (!Utils::Rpm(*targetingPage + 0x30, &originalFunctionCheck, sizeof originalFunctionCheck))
			return false;

		if (originalFunctionCheck != originalFunction)
			memcpy(&targetLockingData.redirectionAsm[0x30], &originalFunction, sizeof originalFunction);

		if (!Utils::Wpm(*targetingPage, &targetLockingData, sizeof targetLockingData))
			return false;

		*targetingPageValid = true;
		return false;
	}
	TargetLocking targetLockingData;
	if (!Utils::Rpm(*targetingPage, &targetLockingData, sizeof targetLockingData))
		return false;

	if (targetLockingData.targetLockingPtr != targetLockingPtr)
	{
		targetLockingData.targetLockingPtr = targetLockingPtr;
		if (!Utils::Wpm(*targetingPage, &targetLockingData, sizeof targetLockingData))
			return false;
		memcpy(&pageJmpOn[2], &*targetingPage, sizeof(DWORD64));
	}
	memcpy(&pageJmpOn[2], &*targetingPage, sizeof(DWORD64));

	auto isPlayer = false;
	auto targetValid = LockedTargetValid(&isPlayer);
	if (!ErectusIni::customTargetSettings.indirectPlayers && isPlayer || !ErectusIni::customTargetSettings.indirectNpCs && !isPlayer)
		targetValid = false;

	const auto redirection = Utils::Rpm(ErectusProcess::exe + OFFSET_REDIRECTION, &redirectionCheck,
		sizeof redirectionCheck);

	if (*targetingPageValid && state && targetValid)
	{
		if (redirection && !memcmp(redirectionCheck, redirectionOff, sizeof redirectionOff))
			Utils::Wpm(ErectusProcess::exe + OFFSET_REDIRECTION, &redirectionOn, sizeof redirectionOn);
	}
	else
	{
		if (redirection && !memcmp(redirectionCheck, redirectionOn, sizeof redirectionOn))
			Utils::Wpm(ErectusProcess::exe + OFFSET_REDIRECTION, &redirectionOff, sizeof redirectionOff);
	}

	if (*targetingPageValid && !isExiting && !memcmp(pageJmpCheck, pageJmpOff, sizeof pageJmpOff))
		return Utils::Wpm(ErectusProcess::exe + OFFSET_REDIRECTION_JMP, &pageJmpOn, sizeof pageJmpOn);
	if (isExiting && !memcmp(pageJmpCheck, pageJmpOn, sizeof pageJmpOn))
		return Utils::Wpm(ErectusProcess::exe + OFFSET_REDIRECTION_JMP, &pageJmpOff, sizeof pageJmpOff);
	return true;
}

bool ErectusMemory::MovePlayer()
{
	const auto cameraPtr = GetCameraPtr();
	if (!Utils::Valid(cameraPtr))
		return false;

	DWORD64 bhkCharProxyControllerPtr;
	if (!Utils::Rpm(ErectusProcess::exe + OFFSET_CHAR_CONTROLLER, &bhkCharProxyControllerPtr, sizeof bhkCharProxyControllerPtr))
		return false;
	if (!Utils::Valid(bhkCharProxyControllerPtr))
		return false;

	BhkCharProxyController bhkCharProxyControllerData{};
	if (!Utils::Rpm(bhkCharProxyControllerPtr, &bhkCharProxyControllerData, sizeof bhkCharProxyControllerData))
		return false;
	if (!Utils::Valid(bhkCharProxyControllerData.hknpBsCharacterProxyPtr))
		return false;

	HknpBsCharacterProxy hknpBsCharacterProxyData{};
	if (!Utils::Rpm(bhkCharProxyControllerData.hknpBsCharacterProxyPtr, &hknpBsCharacterProxyData, sizeof hknpBsCharacterProxyData))
		return false;

	Camera cameraData{};
	if (!Utils::Rpm(cameraPtr, &cameraData, sizeof cameraData))
		return false;

	float velocityA[4];
	memset(velocityA, 0x00, sizeof velocityA);

	float velocityB[4];
	memset(velocityB, 0x00, sizeof velocityB);

	auto speed = ErectusIni::customLocalPlayerSettings.noclipSpeed;
	if (GetAsyncKeyState(VK_SHIFT))
		speed *= 1.5f;

	auto writePosition = false;

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
		return Utils::Wpm(bhkCharProxyControllerData.hknpBsCharacterProxyPtr, &hknpBsCharacterProxyData, sizeof hknpBsCharacterProxyData);

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

	const auto noclipA = Utils::Rpm(ErectusProcess::exe + OFFSET_NOCLIP_A, &noclipCheckA, sizeof noclipCheckA);
	const auto noclipB = Utils::Rpm(ErectusProcess::exe + OFFSET_NOCLIP_B, &noclipCheckB, sizeof noclipCheckB);
	const auto noclipC = Utils::Rpm(ErectusProcess::exe + OFFSET_NOCLIP_C, &noclipCheckC, sizeof noclipCheckC);
	const auto noclipD = Utils::Rpm(ErectusProcess::exe + OFFSET_NOCLIP_D, &noclipCheckD, sizeof noclipCheckD);

	if (state)
	{
		if (noclipA && !memcmp(noclipCheckA, noclipOffA, sizeof noclipOffA))
			Utils::Wpm(ErectusProcess::exe + OFFSET_NOCLIP_A, &noclipOnA, sizeof noclipOnA);

		if (noclipB && !memcmp(noclipCheckB, noclipOffB, sizeof noclipOffB))
			Utils::Wpm(ErectusProcess::exe + OFFSET_NOCLIP_B, &noclipOnB, sizeof noclipOnB);

		if (noclipC && !memcmp(noclipCheckC, noclipOffC, sizeof noclipOffC))
			Utils::Wpm(ErectusProcess::exe + OFFSET_NOCLIP_C, &noclipOnC, sizeof noclipOnC);

		if (noclipD && !memcmp(noclipCheckD, noclipOffD, sizeof noclipOffD))
			Utils::Wpm(ErectusProcess::exe + OFFSET_NOCLIP_D, &noclipOnD, sizeof noclipOnD);

		MovePlayer();
	}
	else
	{
		if (noclipA && !memcmp(noclipCheckA, noclipOnA, sizeof noclipOnA))
			Utils::Wpm(ErectusProcess::exe + OFFSET_NOCLIP_A, &noclipOffA, sizeof noclipOffA);

		if (noclipB && !memcmp(noclipCheckB, noclipOnB, sizeof noclipOnB))
			Utils::Wpm(ErectusProcess::exe + OFFSET_NOCLIP_B, &noclipOffB, sizeof noclipOffB);

		if (noclipC && !memcmp(noclipCheckC, noclipOnC, sizeof noclipOnC))
			Utils::Wpm(ErectusProcess::exe + OFFSET_NOCLIP_C, &noclipOffC, sizeof noclipOffC);

		if (noclipD && !memcmp(noclipCheckD, noclipOnD, sizeof noclipOnD))
			Utils::Wpm(ErectusProcess::exe + OFFSET_NOCLIP_D, &noclipOffD, sizeof noclipOffD);
	}
}

bool ErectusMemory::ActorValue(DWORD64* actorValuePage, bool* actorValuePageValid, const bool state)
{
	if (!*actorValuePage)
	{
		const auto page = Utils::AllocEx(sizeof(ActorValueHook));
		if (!page) return false;
		*actorValuePage = page;
	}

	const auto localPlayerPtr = GetLocalPlayerPtr(false);
	if (!Utils::Valid(localPlayerPtr))
		return false;

	TesObjectRefr localPlayer{};
	if (!Utils::Rpm(localPlayerPtr, &localPlayer, sizeof localPlayer))
		return false;
	if (!Utils::Valid(localPlayer.vtable0050))
		return false;

	DWORD64 actorValueFunction;
	if (!Utils::Rpm(localPlayer.vtable0050 + 0x48, &actorValueFunction, sizeof actorValueFunction))
		return false;
	if (!Utils::Valid(actorValueFunction))
		return false;

	ActorValueHook actorValueHookData;
	actorValueHookData.actionPointsEnabled = static_cast<int>(ErectusIni::customLocalPlayerSettings.actionPointsEnabled);
	actorValueHookData.actionPoints = static_cast<float>(ErectusIni::customLocalPlayerSettings.actionPoints);
	actorValueHookData.strengthEnabled = static_cast<int>(ErectusIni::customLocalPlayerSettings.strengthEnabled);
	actorValueHookData.strength = static_cast<float>(ErectusIni::customLocalPlayerSettings.strength);
	actorValueHookData.perceptionEnabled = static_cast<int>(ErectusIni::customLocalPlayerSettings.perceptionEnabled);
	actorValueHookData.perception = static_cast<float>(ErectusIni::customLocalPlayerSettings.perception);
	actorValueHookData.enduranceEnabled = static_cast<int>(ErectusIni::customLocalPlayerSettings.enduranceEnabled);
	actorValueHookData.endurance = static_cast<float>(ErectusIni::customLocalPlayerSettings.endurance);
	actorValueHookData.charismaEnabled = static_cast<int>(ErectusIni::customLocalPlayerSettings.charismaEnabled);
	actorValueHookData.charisma = static_cast<float>(ErectusIni::customLocalPlayerSettings.charisma);
	actorValueHookData.intelligenceEnabled = static_cast<int>(ErectusIni::customLocalPlayerSettings.intelligenceEnabled);
	actorValueHookData.intelligence = static_cast<float>(ErectusIni::customLocalPlayerSettings.intelligence);
	actorValueHookData.agilityEnabled = static_cast<int>(ErectusIni::customLocalPlayerSettings.agilityEnabled);
	actorValueHookData.agility = static_cast<float>(ErectusIni::customLocalPlayerSettings.agility);
	actorValueHookData.luckEnabled = static_cast<int>(ErectusIni::customLocalPlayerSettings.luckEnabled);
	actorValueHookData.luck = static_cast<float>(ErectusIni::customLocalPlayerSettings.luck);
	actorValueHookData.originalFunction = ErectusProcess::exe + OFFSET_ACTOR_VALUE;

	if (actorValueFunction != ErectusProcess::exe + OFFSET_ACTOR_VALUE && actorValueFunction != *actorValuePage)
	{
		if (Utils::VtableSwap(localPlayer.vtable0050 + 0x48, ErectusProcess::exe + OFFSET_ACTOR_VALUE))
			Utils::FreeEx(actorValueFunction);
	}

	if (state)
	{
		if (*actorValuePageValid)
		{
			ActorValueHook actorValueHookCheck;
			if (!Utils::Rpm(*actorValuePage, &actorValueHookCheck, sizeof actorValueHookCheck))
				return false;
			if (memcmp(&actorValueHookData, &actorValueHookCheck, sizeof actorValueHookCheck) != 0)
				return Utils::Wpm(*actorValuePage, &actorValueHookData, sizeof actorValueHookData);
		}
		else
		{
			if (!Utils::Wpm(*actorValuePage, &actorValueHookData, sizeof actorValueHookData))
				return false;
			if (!Utils::VtableSwap(localPlayer.vtable0050 + 0x48, *actorValuePage))
				return false;
			*actorValuePageValid = true;
		}
	}
	else
	{
		if (actorValueFunction != ErectusProcess::exe + OFFSET_ACTOR_VALUE)
		{
			if (Utils::VtableSwap(localPlayer.vtable0050 + 0x48, ErectusProcess::exe + OFFSET_ACTOR_VALUE))
				Utils::FreeEx(actorValueFunction);
		}

		if (*actorValuePage)
		{
			if (Utils::FreeEx(*actorValuePage))
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
	if (!Utils::Rpm(actorValuePtr, &actorValueData, sizeof actorValueData))
		return false;

	if (state)
	{
		if (actorValueData.maximumValue != customValue)
		{
			actorValueData.maximumValue = customValue;
			return Utils::Wpm(actorValuePtr, &actorValueData, sizeof actorValueData);
		}
	}
	else
	{
		if (actorValueData.maximumValue != defaultValue)
		{
			actorValueData.maximumValue = defaultValue;
			return Utils::Wpm(actorValuePtr, &actorValueData, sizeof actorValueData);
		}
	}

	return true;
}

bool ErectusMemory::OnePositionKill(DWORD64* opkPage, bool* opkPageValid, const bool state)
{
	if (!*opkPage)
	{
		const auto page = Utils::AllocEx(sizeof(Opk));
		if (!page)
			return false;
		*opkPage = page;
	}

	BYTE opkOn[] = { 0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xE0, 0xCC, 0xCC, 0xCC };
	BYTE opkOff[] = { 0x0F, 0x10, 0x87, 0x90, 0x04, 0x00, 0x00, 0x0F, 0x58, 0x45, 0xA7, 0x0F, 0x29, 0x45, 0xF7 };
	BYTE opkCheck[sizeof opkOff];

	if (!Utils::Rpm(ErectusProcess::exe + OFFSET_OPK, &opkCheck, sizeof opkCheck))
		return false;

	const auto originalFunction = ErectusProcess::exe + OFFSET_OPK + sizeof opkOff;
	memcpy(&opkOn[2], &*opkPage, sizeof(DWORD64));

	DWORD64 pageCheck;
	memcpy(&pageCheck, &opkCheck[2], sizeof(DWORD64));

	if (Utils::Valid(pageCheck) && pageCheck != *opkPage)
	{
		Opk buffer;
		if (!Utils::Rpm(pageCheck, &buffer, sizeof buffer))
			return false;
		if (buffer.originalFunction != originalFunction)
			return false;
		if (!Utils::Wpm(ErectusProcess::exe + OFFSET_OPK, &opkOff, sizeof opkOff))
			return false;
		Utils::FreeEx(pageCheck);
	}

	if (state)
	{
		if (*opkPageValid)
			return true;

		Opk opkData;
		opkData.opkPlayers = 0;
		opkData.opkNpcs = 0;
		opkData.originalFunction = originalFunction;
		memset(opkData.opkPlayerPosition, 0x00, sizeof opkData.opkPlayerPosition);
		memset(opkData.opkNpcPosition, 0x00, sizeof opkData.opkNpcPosition);

		if (!Utils::Wpm(*opkPage, &opkData, sizeof opkData))
			return false;
		if (!Utils::Wpm(ErectusProcess::exe + OFFSET_OPK, &opkOn, sizeof opkOn))
			return false;
		*opkPageValid = true;
	}
	else
	{
		if (pageCheck == *opkPage)
			Utils::Wpm(ErectusProcess::exe + OFFSET_OPK, &opkOff, sizeof opkOff);

		if (*opkPage && Utils::FreeEx(*opkPage))
		{
			*opkPage = 0;
			*opkPageValid = false;
		}

	}

	return true;
}

bool ErectusMemory::CheckOpkDistance(const DWORD64 opkPage, const bool players)
{
	Opk opkData;
	if (!Utils::Rpm(opkPage, &opkData, sizeof opkData))
		return false;

	const auto cameraPtr = GetCameraPtr();
	if (!Utils::Valid(cameraPtr))
		return false;

	Camera cameraData{};
	if (!Utils::Rpm(cameraPtr, &cameraData, sizeof cameraData))
		return false;

	float editedOrigin[3];
	editedOrigin[0] = cameraData.origin[0] / 70.0f;
	editedOrigin[1] = cameraData.origin[1] / 70.0f;
	editedOrigin[2] = cameraData.origin[2] / 70.0f;

	if (players)
	{
		const auto distance = Utils::GetDistance(opkData.opkPlayerPosition, editedOrigin);
		if (distance > 20.0f)
			return false;
	}
	else
	{
		const auto distance = Utils::GetDistance(opkData.opkNpcPosition, editedOrigin);
		if (distance > 20.0f)
			return false;
	}

	return true;
}

bool ErectusMemory::SetOpkData(const DWORD64 opkPage, const bool players, const bool state)
{
	Opk opkData;
	if (!Utils::Rpm(opkPage, &opkData, sizeof opkData))
		return false;

	if (!state)
	{
		auto writeData = false;

		if (players && opkData.opkPlayers)
		{
			opkData.opkPlayers = 0;
			memset(opkData.opkPlayerPosition, 0x00, sizeof opkData.opkPlayerPosition);
			writeData = true;
		}
		else if (!players && opkData.opkNpcs)
		{
			opkData.opkNpcs = 0;
			memset(opkData.opkNpcPosition, 0x00, sizeof opkData.opkNpcPosition);
			writeData = true;
		}

		if (writeData)
			Utils::Wpm(opkPage, &opkData, sizeof opkData);

		return true;
	}

	if (CheckOpkDistance(opkPage, players))
		return true;

	const auto cameraPtr = GetCameraPtr();
	if (!Utils::Valid(cameraPtr))
		return false;

	Camera cameraData{};
	if (!Utils::Rpm(cameraPtr, &cameraData, sizeof cameraData))
		return false;

	float editedOrigin[3];
	editedOrigin[0] = cameraData.origin[0] / 70.0f;
	editedOrigin[1] = cameraData.origin[1] / 70.0f;
	editedOrigin[2] = cameraData.origin[2] / 70.0f;

	float opkPosition[3];
	Utils::ProjectView(opkPosition, cameraData.forward, editedOrigin, 3.0f);

	if (players)
	{
		opkData.opkPlayerPosition[0] = opkPosition[0];
		opkData.opkPlayerPosition[1] = opkPosition[1];
		opkData.opkPlayerPosition[2] = opkPosition[2];
		opkData.opkPlayers = 1;
	}
	else
	{
		opkData.opkNpcPosition[0] = opkPosition[0];
		opkData.opkNpcPosition[1] = opkPosition[1];
		opkData.opkNpcPosition[2] = opkPosition[2];
		opkData.opkNpcs = 1;
	}

	return Utils::Wpm(opkPage, &opkData, sizeof opkData);
}

bool ErectusMemory::InsideInteriorCell()
{
	DWORD64 entityListTypePtr;
	if (!Utils::Rpm(ErectusProcess::exe + OFFSET_ENTITY_LIST, &entityListTypePtr, sizeof entityListTypePtr))
		return false;
	if (!Utils::Valid(entityListTypePtr))
		return false;

	LoadedAreaManager entityListTypeData{};
	if (!Utils::Rpm(entityListTypePtr, &entityListTypeData, sizeof entityListTypeData))
		return false;
	if (!Utils::Valid(entityListTypeData.interiorCellArrayPtr))
		return false;
	if (!Utils::Valid(entityListTypeData.interiorCellArrayPtr2))
		return false;
	if (!Utils::Valid(entityListTypeData.exteriorCellArrayPtr))
		return false;
	if (!Utils::Valid(entityListTypeData.exteriorCellArrayPtr2))
		return false;

	if (entityListTypeData.interiorCellArrayPtr != entityListTypeData.interiorCellArrayPtr2)
		return true;

	return false;
}

int ErectusMemory::RenderLocalPlayerData()
{
	std::vector<std::string> infoTexts = {};

	const auto localPlayerPtr = GetLocalPlayerPtr(false);
	if (!Utils::Valid(localPlayerPtr))
		return 0;

	TesObjectRefr localPlayer{};
	if (!Utils::Rpm(localPlayerPtr, &localPlayer, sizeof localPlayer))
		return 0;

	DWORD cellFormId = 0;
	if (Utils::Valid(localPlayer.cellPtr) && !Utils::Rpm(localPlayer.cellPtr + 0x20, &cellFormId, sizeof cellFormId))
		cellFormId = 0;

	auto playerHealth = -1.0f;
	ActorSnapshotComponent actorSnapshotComponentData{};
	if (GetActorSnapshotComponentData(localPlayer, &actorSnapshotComponentData))
		playerHealth = actorSnapshotComponentData.maxHealth + actorSnapshotComponentData.modifiedHealth + actorSnapshotComponentData.lostHealth;


	infoTexts.push_back(fmt::format("Entity FormId: {:08x}", localPlayer.formId));
	infoTexts.push_back(fmt::format("STASH FormId: {:08x}", localPlayer.formId0C24));
	infoTexts.push_back(fmt::format("Cell FormId: {:08x}", cellFormId));
	infoTexts.push_back(fmt::format("X: {:f}", localPlayer.position[0]));
	infoTexts.push_back(fmt::format("Y: {:f}", localPlayer.position[1]));
	infoTexts.push_back(fmt::format("Z: {:f}", localPlayer.position[2]));
	infoTexts.push_back(fmt::format("Yaw: {:f}", localPlayer.yaw));
	infoTexts.push_back(fmt::format("Pitch: {:f}", localPlayer.pitch));
	infoTexts.push_back(fmt::format("Health: {:f}", playerHealth));

	float textColor[3] = { 1.0f, 1.0f, 1.0f };
	auto index = 0.f;
	for (const auto& item : infoTexts)
	{

		float position[2] = { 0.f, index * 16.f };
		Renderer::DrawTextA(item.c_str(), true, false, position, textColor, 1.f);

		index++;
	}

	return index;
}

void ErectusMemory::RenderData()
{
	auto spacing = 0;
	if (ErectusIni::customUtilitySettings.debugPlayer)
		spacing = RenderLocalPlayerData();

	float enabledTextColor[3] = { 0.0f, 1.0f, 0.0f };
	float disabledTextColor[3] = { 1.0f, 0.0f, 0.0f };

	std::vector<std::pair<std::string, bool>> infoTexts = {};

	std::string featureText = {};
	auto featureState = false;

	if (ErectusIni::scrapLooter.drawStatus)
	{
		featureText = fmt::format("Scrap Looter (Automatic): {:d}", static_cast<int>(ErectusIni::scrapLooter.autoLootingEnabled));
		featureState = ErectusIni::scrapLooter.autoLootingEnabled;
		infoTexts.push_back({ featureText, featureState });
	}

	if (ErectusIni::itemLooter.drawStatus)
	{
		featureText = fmt::format("Item Looter (Automatic): {:d}", static_cast<int>(ErectusIni::itemLooter.autoLootingEnabled));
		featureState = ErectusIni::itemLooter.autoLootingEnabled;
		infoTexts.push_back({ featureText, featureState });
	}

	if (ErectusIni::npcLooter.drawStatus)
	{
		featureText = fmt::format("NPC Looter (76m Distance Limit): {:d}", static_cast<int>(ErectusIni::npcLooter.enabled));
		featureState = ErectusIni::npcLooter.enabled;
		infoTexts.push_back({ featureText, featureState });
	}

	if (ErectusIni::containerLooter.drawStatus)
	{
		featureText = fmt::format("Container Looter (6m Distance Limit): {:d}", static_cast<int>(ErectusIni::containerLooter.enabled));
		featureState = ErectusIni::containerLooter.enabled;
		infoTexts.push_back({ featureText, featureState });
	}

	if (ErectusIni::harvester.drawStatus)
	{
		featureText = fmt::format("Flora Harvester (6m Distance Limit): {:d}", static_cast<int>(ErectusIni::harvester.enabled));
		featureState = ErectusIni::harvester.enabled;
		infoTexts.push_back({ featureText, featureState });
	}

	if (ErectusIni::customLocalPlayerSettings.drawPositionSpoofingStatus)
	{
		featureText = fmt::format("Position Spoofing (Active): {0:d} (Height: {1:d})", static_cast<int>(ErectusThread::positionSpoofingToggle), ErectusIni::customLocalPlayerSettings.positionSpoofingHeight);
		featureState = InsideInteriorCell() ? false : ErectusIni::customLocalPlayerSettings.positionSpoofingEnabled;
		infoTexts.push_back({ featureText, featureState });
	}

	if (ErectusIni::customNukeCodeSettings.drawCodeAlpha)
	{
		featureText = fmt::format("{} - Alpha", fmt::join(ErectusImGui::alphaCode, " "));
		featureState = ErectusImGui::alphaCode == std::array<int, 8>{} ? false : true;
		infoTexts.push_back({ featureText, featureState });
	}

	if (ErectusIni::customNukeCodeSettings.drawCodeBravo)
	{
		featureText = fmt::format("{} - Bravo", fmt::join(ErectusImGui::bravoCode, " "));
		featureState = ErectusImGui::bravoCode == std::array<int, 8>{} ? false : true;
		infoTexts.push_back({ featureText, featureState });
	}

	if (ErectusIni::customNukeCodeSettings.drawCodeCharlie)
	{
		featureText = fmt::format("{} - Charlie", fmt::join(ErectusImGui::charlieCode, " "));
		featureState = ErectusImGui::charlieCode == std::array<int, 8>{} ? false : true;
		infoTexts.push_back({ featureText, featureState });
	}

	for (const auto& item : infoTexts)
	{
		float textPosition[2] = { 0.0f, static_cast<float>(spacing) * 16.0f };
		auto textColor = item.second ? enabledTextColor : disabledTextColor;
		Renderer::DrawTextA(item.first.c_str(), true, false, textPosition, textColor, 1.f);
		spacing++;
	}
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

	return Utils::Wpm(destinationAddress, &sourcePointer, sizeof sourcePointer);
}

bool ErectusMemory::CheckItemTransferList()
{
	for (auto i = 0; i < 32; i++)
	{
		if (ErectusIni::customTransferSettings.whitelist[i] && ErectusIni::customTransferSettings.whitelisted[i])
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
	if (!Utils::Rpm(sourcePtr, &entityData, sizeof entityData))
		return false;
	if (!Utils::Valid(entityData.inventoryPtr))
		return false;

	Inventory inventoryData{};
	if (!Utils::Rpm(entityData.inventoryPtr, &inventoryData, sizeof inventoryData))
		return false;
	if (!Utils::Valid(inventoryData.itemArrayPtr) || inventoryData.itemArrayEnd < inventoryData.itemArrayPtr)
		return false;

	auto itemArraySize = (inventoryData.itemArrayEnd - inventoryData.itemArrayPtr) / sizeof(Item);
	if (!itemArraySize || itemArraySize > 0x7FFF)
		return false;

	auto* itemData = new Item[itemArraySize];
	if (!Utils::Rpm(inventoryData.itemArrayPtr, &*itemData, itemArraySize * sizeof(Item)))
	{
		delete[]itemData;
		itemData = nullptr;
		return false;
	}

	for (DWORD64 i = 0; i < itemArraySize; i++)
	{
		if (!Utils::Valid(itemData[i].referencePtr))
			continue;
		if (!Utils::Valid(itemData[i].displayPtr) || itemData[i].iterations < itemData[i].displayPtr)
			continue;

		if (ErectusIni::customTransferSettings.useWhitelist || ErectusIni::customTransferSettings.useBlacklist)
		{
			TesItem referenceData{};
			if (!Utils::Rpm(itemData[i].referencePtr, &referenceData, sizeof referenceData))
				continue;

			if (ErectusIni::customTransferSettings.useWhitelist)
			{
				if (!CheckFormIdArray(referenceData.formId, ErectusIni::customTransferSettings.whitelisted, ErectusIni::customTransferSettings.whitelist, 32))
					continue;
			}

			if (ErectusIni::customTransferSettings.useBlacklist)
			{
				if (CheckFormIdArray(referenceData.formId, ErectusIni::customTransferSettings.blacklisted, ErectusIni::customTransferSettings.blacklist, 32))
					continue;
			}
		}

		auto iterations = (itemData[i].iterations - itemData[i].displayPtr) / sizeof(ItemCount);
		if (!iterations || iterations > 0x7FFF)
			continue;

		auto* itemCountData = new ItemCount[iterations];
		if (!Utils::Rpm(itemData[i].displayPtr, &*itemCountData, iterations * sizeof(ItemCount)))
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

		TransferMessage transferMessageData{};
		transferMessageData.vtable = ErectusProcess::exe + VTABLE_REQUESTTRANSFERITEMMSG;
		transferMessageData.srcFormId = sourceFormId;
		transferMessageData.unknownId = 0xE0001F7A;
		transferMessageData.dstFormId = destinationFormId;
		transferMessageData.itemId = itemData[i].itemId;
		transferMessageData.count = count;
		transferMessageData.unknownA = 0x00000000;
		transferMessageData.unknownB = 0x00;
		transferMessageData.unknownC = 0x01;
		transferMessageData.unknownD = 0x00;
		transferMessageData.unknownE = 0x02;
		SendMessageToServer(&transferMessageData, sizeof transferMessageData);
	}

	delete[]itemData;
	itemData = nullptr;
	return true;
}

bool ErectusMemory::GetTeleportPosition(const int index)
{
	const auto localPlayerPtr = GetLocalPlayerPtr(true);
	if (!Utils::Valid(localPlayerPtr))
		return false;

	TesObjectRefr localPlayer{};
	if (!Utils::Rpm(localPlayerPtr, &localPlayer, sizeof localPlayer))
		return false;
	if (!Utils::Valid(localPlayer.cellPtr))
		return false;

	DWORD cellFormId;
	if (!Utils::Rpm(localPlayer.cellPtr + 0x20, &cellFormId, sizeof cellFormId))
		return false;

	ErectusIni::customTeleportSettings.teleportEntryData[index].destination[0] = localPlayer.position[0];
	ErectusIni::customTeleportSettings.teleportEntryData[index].destination[1] = localPlayer.position[1];
	ErectusIni::customTeleportSettings.teleportEntryData[index].destination[2] = localPlayer.position[2];
	ErectusIni::customTeleportSettings.teleportEntryData[index].destination[3] = localPlayer.yaw;
	ErectusIni::customTeleportSettings.teleportEntryData[index].cellFormId = cellFormId;

	return true;
}

bool ErectusMemory::RequestTeleport(const int index)
{
	const auto cellPtr = GetPtr(ErectusIni::customTeleportSettings.teleportEntryData[index].cellFormId);
	if (!Utils::Valid(cellPtr))
		return false;

	RequestTeleportMessage requestTeleportMessageData =
	{
		.vtable = ErectusProcess::exe + VTABLE_REQUESTTELEPORTTOLOCATIONMSG,
		.positionX = ErectusIni::customTeleportSettings.teleportEntryData[index].destination[0],
		.positionY = ErectusIni::customTeleportSettings.teleportEntryData[index].destination[1],
		.positionZ = ErectusIni::customTeleportSettings.teleportEntryData[index].destination[2],
		.rotationX = 0.0f,
		.rotationY = 0.0f,
		.rotationZ = ErectusIni::customTeleportSettings.teleportEntryData[index].destination[3],
		.cellPtr = cellPtr
	};

	return SendMessageToServer(&requestTeleportMessageData, sizeof requestTeleportMessageData);
}

DWORD ErectusMemory::GetLocalPlayerFormId()
{
	const auto localPlayerPtr = GetLocalPlayerPtr(true);
	if (!Utils::Valid(localPlayerPtr))
		return 0;

	TesObjectRefr localPlayer{};
	if (!Utils::Rpm(localPlayerPtr, &localPlayer, sizeof localPlayer))
		return 0;

	return localPlayer.formId;
}

DWORD ErectusMemory::GetStashFormId()
{
	const auto localPlayerPtr = GetLocalPlayerPtr(true);
	if (!Utils::Valid(localPlayerPtr))
		return 0;

	TesObjectRefr localPlayer{};
	if (!Utils::Rpm(localPlayerPtr, &localPlayer, sizeof localPlayer))
		return 0;

	return localPlayer.formId0C24;
}

bool ErectusMemory::FreezeActionPoints(DWORD64* freezeApPage, bool* freezeApPageValid, const bool state)
{
	if (!*freezeApPage)
	{
		const auto page = Utils::AllocEx(sizeof(FreezeAp));
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

	if (!Utils::Rpm(ErectusProcess::exe + OFFSET_AV_REGEN, &freezeApCheck, sizeof freezeApCheck))
		return false;

	DWORD64 pageCheck;
	memcpy(&pageCheck, &freezeApCheck[0x6], sizeof(DWORD64));

	if (Utils::Valid(pageCheck) && pageCheck != *freezeApPage)
	{
		for (auto i = 0; i < 0x6; i++) if (freezeApCheck[i] != freezeApOn[i])
			return false;
		if (!Utils::Wpm(ErectusProcess::exe + OFFSET_AV_REGEN, &freezeApOff, sizeof freezeApOff))
			return false;
		Utils::FreeEx(pageCheck);
	}

	if (state)
	{
		FreezeAp freezeApData;
		freezeApData.freezeApEnabled = ErectusIni::customLocalPlayerSettings.freezeApEnabled;

		if (*freezeApPageValid)
		{
			FreezeAp freezeApPageCheck;
			if (!Utils::Rpm(*freezeApPage, &freezeApPageCheck, sizeof freezeApPageCheck))
				return false;
			if (!memcmp(&freezeApData, &freezeApPageCheck, sizeof freezeApPageCheck))
				return true;
			return Utils::Wpm(*freezeApPage, &freezeApData, sizeof freezeApData);
		}
		if (!Utils::Wpm(*freezeApPage, &freezeApData, sizeof freezeApData))
			return false;
		memcpy(&freezeApOn[0x6], &*freezeApPage, sizeof(DWORD64));
		if (!Utils::Wpm(ErectusProcess::exe + OFFSET_AV_REGEN, &freezeApOn, sizeof freezeApOn))
			return false;
		*freezeApPageValid = true;
	}
	else
	{
		if (pageCheck == *freezeApPage)
			Utils::Wpm(ErectusProcess::exe + OFFSET_AV_REGEN, &freezeApOff, sizeof freezeApOff);

		if (*freezeApPage)
		{
			if (Utils::FreeEx(*freezeApPage))
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

	return SendMessageToServer(&clientStateMsgData, sizeof clientStateMsgData);
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

	if (!Utils::Rpm(ErectusProcess::exe + OFFSET_SERVER_POSITION, &positionSpoofingCheck, sizeof positionSpoofingCheck))
		return false;

	if (!state)
	{
		positionSpoofingCheck[1] = 0x00;
		positionSpoofingCheck[2] = 0x00;
		positionSpoofingCheck[3] = 0x00;
		positionSpoofingCheck[4] = 0x00;

		if (!memcmp(positionSpoofingCheck, positionSpoofingOn, sizeof positionSpoofingOn))
			return Utils::Wpm(ErectusProcess::exe + OFFSET_SERVER_POSITION, &positionSpoofingOff, sizeof positionSpoofingOff);

		return true;
	}

	int spoofingHeightCheck;
	memcpy(&spoofingHeightCheck, &positionSpoofingCheck[1], sizeof spoofingHeightCheck);
	memcpy(&positionSpoofingOn[1], &ErectusIni::customLocalPlayerSettings.positionSpoofingHeight,
		sizeof ErectusIni::customLocalPlayerSettings.positionSpoofingHeight);

	if (!memcmp(positionSpoofingCheck, positionSpoofingOn, sizeof positionSpoofingOn))
		return true;
	if (!memcmp(positionSpoofingCheck, positionSpoofingOff, sizeof positionSpoofingOff))
		return Utils::Wpm(ErectusProcess::exe + OFFSET_SERVER_POSITION, &positionSpoofingOn, sizeof positionSpoofingOn);
	if (spoofingHeightCheck != ErectusIni::customLocalPlayerSettings.positionSpoofingHeight)
	{
		if (positionSpoofingCheck[0] != 0xBA || spoofingHeightCheck < -524287 || spoofingHeightCheck > 524287)
			return false;

		return Utils::Wpm(ErectusProcess::exe + OFFSET_SERVER_POSITION, &positionSpoofingOn,
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
	if (!Utils::Rpm(ErectusProcess::exe + OFFSET_ENTITY_ID + v3 * 0x8, &v4, sizeof v4))
		return 0;

	const auto v5 = v4 & 0x7FF80000;
	const auto v6 = v5 | v2;

	return v6;
}

bool ErectusMemory::SendHitsToServer(Hits* hitsData, const size_t hitsDataSize)
{
	const auto allocSize = sizeof(ExternalFunction) + sizeof(RequestHitsOnActors) + hitsDataSize;
	const auto allocAddress = Utils::AllocEx(allocSize);
	if (allocAddress == 0)
		return false;

	ExternalFunction externalFunctionData;
	externalFunctionData.address = ErectusProcess::exe + OFFSET_MESSAGE_SENDER;
	externalFunctionData.rcx = allocAddress + sizeof(ExternalFunction);
	externalFunctionData.rdx = 0;
	externalFunctionData.r8 = 0;
	externalFunctionData.r9 = 0;

	auto* pageData = new BYTE[allocSize];
	memset(pageData, 0x00, allocSize);

	RequestHitsOnActors requestHitsOnActorsData{};
	memset(&requestHitsOnActorsData, 0x00, sizeof(RequestHitsOnActors));

	requestHitsOnActorsData.vtable = ErectusProcess::exe + VTABLE_REQUESTHITSONACTORS;
	requestHitsOnActorsData.hitsArrayPtr = allocAddress + sizeof(ExternalFunction) + sizeof(RequestHitsOnActors);
	requestHitsOnActorsData.hitsArrayEnd = allocAddress + sizeof(ExternalFunction) + sizeof(RequestHitsOnActors) +
		hitsDataSize;

	memcpy(pageData, &externalFunctionData, sizeof externalFunctionData);
	memcpy(&pageData[sizeof(ExternalFunction)], &requestHitsOnActorsData, sizeof requestHitsOnActorsData);
	memcpy(&pageData[sizeof(ExternalFunction) + sizeof(RequestHitsOnActors)], &*hitsData, hitsDataSize);

	const auto pageWritten = Utils::Wpm(allocAddress, &*pageData, allocSize);

	delete[]pageData;
	pageData = nullptr;

	if (!pageWritten)
	{
		Utils::FreeEx(allocAddress);
		return false;
	}

	const auto paramAddress = allocAddress + sizeof ExternalFunction::ASM;
	auto* const thread = CreateRemoteThread(ErectusProcess::handle, nullptr, 0, LPTHREAD_START_ROUTINE(allocAddress),
		LPVOID(paramAddress), 0, nullptr);

	if (thread == nullptr)
	{
		Utils::FreeEx(allocAddress);
		return false;
	}

	const auto threadResult = WaitForSingleObject(thread, 3000);
	CloseHandle(thread);

	if (threadResult == WAIT_TIMEOUT)
		return false;

	Utils::FreeEx(allocAddress);

	return true;
}

bool ErectusMemory::SendDamage(const DWORD weaponId, BYTE* shotsHit, BYTE* shotsFired, const BYTE count)
{
	if (!MessagePatcher(allowMessages))
		return false;

	if (!allowMessages)
		return false;

	if (!weaponId)
		return false;

	auto isPlayer = false;
	LockedTargetValid(&isPlayer);
	if (!ErectusIni::customTargetSettings.directPlayers && isPlayer || !ErectusIni::customTargetSettings.directNpCs && !isPlayer)
		return false;

	const auto localPlayerPtr = GetLocalPlayerPtr(true);
	if (!Utils::Valid(localPlayerPtr))
		return false;

	TesObjectRefr localPlayer{};
	if (!Utils::Rpm(localPlayerPtr, &localPlayer, sizeof localPlayer))
		return false;

	const auto localPlayerId = GetEntityId(localPlayer);
	if (!localPlayerId)
		return false;

	TesObjectRefr target{};
	if (!Utils::Rpm(targetLockingPtr, &target, sizeof target)) return false;

	const auto targetId = GetEntityId(target);
	if (!targetId)
		return false;

	auto* hitsData = new Hits[count];
	memset(hitsData, 0x00, count * sizeof(Hits));

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

		if (ErectusIni::customTargetSettings.sendDamageMax < 10)
		{
			if (Utils::GetRangedInt(1, 10) <= static_cast<int>(10 - ErectusIni::customTargetSettings.sendDamageMax))
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

	const auto result = SendHitsToServer(hitsData, count * sizeof(Hits));

	delete[]hitsData;

	return result;
}

DWORD64 ErectusMemory::GetNukeCodePtr(const DWORD formId)
{
	ReferenceList questTextList{};
	if (!Utils::Rpm(ErectusProcess::exe + OFFSET_NUKE_CODE, &questTextList, sizeof questTextList))
		return 0;
	if (!Utils::Valid(questTextList.arrayPtr) || !questTextList.arraySize || questTextList.arraySize > 0x7FFF)
		return 0;

	auto* questTextArray = new DWORD64[questTextList.arraySize];
	if (!Utils::Rpm(questTextList.arrayPtr, &*questTextArray, questTextList.arraySize * sizeof(DWORD64)))
	{
		delete[]questTextArray;
		questTextArray = nullptr;
		return 0;
	}

	DWORD64 nukeCodePtr = 0;
	for (auto i = 0; i < questTextList.arraySize; i++)
	{
		if (!Utils::Valid(questTextArray[i]))
			continue;

		BgsQuestText bgsQuestTextData{};
		if (!Utils::Rpm(questTextArray[i], &bgsQuestTextData, sizeof bgsQuestTextData))
			continue;
		if (!Utils::Valid(bgsQuestTextData.formIdPtr) || !Utils::Valid(bgsQuestTextData.codePtr))
			continue;

		DWORD formIdCheck;
		if (!Utils::Rpm(bgsQuestTextData.formIdPtr + 0x4, &formIdCheck, sizeof formIdCheck))
			continue;
		if (formIdCheck != formId)
			continue;

		nukeCodePtr = bgsQuestTextData.codePtr;
		break;
	}

	delete[]questTextArray;
	questTextArray = nullptr;

	return nukeCodePtr;
}

bool ErectusMemory::GetNukeCode(const DWORD formId, std::array<int, 8>& nukeCode)
{
	const auto nukeCodePtr = GetNukeCodePtr(formId);
	if (!nukeCodePtr)
		return false;

	float nukeCodeArray[16];
	if (!Utils::Rpm(nukeCodePtr, &nukeCodeArray, sizeof nukeCodeArray))
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

	if (ErectusIni::customTargetSettings.favoriteIndex >= 12)
		return 0;

	const auto localPlayerPtr = GetLocalPlayerPtr(true);
	if (!Utils::Valid(localPlayerPtr))
		return 0;

	TesObjectRefr localPlayer{};
	if (!Utils::Rpm(localPlayerPtr, &localPlayer, sizeof localPlayer))
		return 0;
	if (!Utils::Valid(localPlayer.inventoryPtr))
		return 0;

	Inventory inventoryData{};
	if (!Utils::Rpm(localPlayer.inventoryPtr, &inventoryData, sizeof inventoryData))
		return 0;
	if (!Utils::Valid(inventoryData.itemArrayPtr) || inventoryData.itemArrayEnd < inventoryData.itemArrayPtr)
		return 0;

	const auto itemArraySize = (inventoryData.itemArrayEnd - inventoryData.itemArrayPtr) / sizeof(Item);
	if (!itemArraySize || itemArraySize > 0x7FFF)
		return 0;

	auto itemData = std::make_unique<Item[]>(itemArraySize);
	if (!Utils::Rpm(inventoryData.itemArrayPtr, itemData.get(), itemArraySize * sizeof(Item)))
		return 0;

	for (DWORD64 i = 0; i < itemArraySize; i++)
	{
		if (!Utils::Valid(itemData[i].referencePtr))
			continue;
		if (itemData[i].favoriteIndex != favouriteIndex)
			continue;

		TesItem referenceData{};
		if (!Utils::Rpm(itemData[i].referencePtr, &referenceData, sizeof referenceData))
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
	if (!Utils::Rpm(vtable - 0x8, &buffer, sizeof buffer))
		return 0;
	if (!Utils::Valid(buffer))
		return 0;

	DWORD offset;
	if (!Utils::Rpm(buffer + 0xC, &offset, sizeof offset))
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
	if (!Utils::Rpm(displayPtr, &instancedArrayPtr, sizeof instancedArrayPtr))
		return result;
	if (!Utils::Valid(instancedArrayPtr))
		return result;

	ItemInstancedArray itemInstancedArrayData{};
	if (!Utils::Rpm(instancedArrayPtr, &itemInstancedArrayData, sizeof itemInstancedArrayData))
		return result;
	if (!Utils::Valid(itemInstancedArrayData.arrayPtr) || itemInstancedArrayData.arrayEnd < itemInstancedArrayData.arrayPtr)
		return result;

	const auto instancedArraySize = (itemInstancedArrayData.arrayEnd - itemInstancedArrayData.arrayPtr) / sizeof(DWORD64);
	if (!instancedArraySize || instancedArraySize > 0x7FFF)
		return result;

	auto instancedArray = std::make_unique<DWORD64[]>(instancedArraySize);
	if (!Utils::Rpm(itemInstancedArrayData.arrayPtr, instancedArray.get(), instancedArraySize * sizeof(DWORD64)))
		return result;

	for (DWORD64 i = 0; i < instancedArraySize; i++)
	{
		if (!Utils::Valid(instancedArray[i]))
			continue;

		ExtraTextDisplayData extraTextDisplayDataData{};
		if (!Utils::Rpm(instancedArray[i], &extraTextDisplayDataData, sizeof extraTextDisplayDataData))
			continue;

		const auto rttiNamePtr = RttiGetNamePtr(extraTextDisplayDataData.vtable);
		if (!rttiNamePtr)
			continue;

		char rttiNameCheck[sizeof".?AVExtraTextDisplayData@@"];
		if (!Utils::Rpm(rttiNamePtr, &rttiNameCheck, sizeof rttiNameCheck))
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
	if (!Utils::Rpm(localPlayerPtr, &localPlayer, sizeof localPlayer))
		return result;
	if (!Utils::Valid(localPlayer.inventoryPtr))
		return result;

	Inventory inventoryData{};
	if (!Utils::Rpm(localPlayer.inventoryPtr, &inventoryData, sizeof inventoryData))
		return result;
	if (!Utils::Valid(inventoryData.itemArrayPtr) || inventoryData.itemArrayEnd < inventoryData.itemArrayPtr)
		return result;

	const auto itemArraySize = (inventoryData.itemArrayEnd - inventoryData.itemArrayPtr) / sizeof(Item);
	if (!itemArraySize || itemArraySize > 0x7FFF)
		return result;

	auto itemData = std::make_unique<Item[]>(itemArraySize);
	if (!Utils::Rpm(inventoryData.itemArrayPtr, itemData.get(), itemArraySize * sizeof(Item)))
		return result;

	for (DWORD64 i = 0; i < itemArraySize; i++)
	{
		if (!Utils::Valid(itemData[i].referencePtr))
			continue;
		if (itemData[i].favoriteIndex > 12)
			continue;

		TesItem referenceData{};
		if (!Utils::Rpm(itemData[i].referencePtr, &referenceData, sizeof referenceData))
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
	if (!Utils::Rpm(localPlayerPtr, &localPlayer, sizeof localPlayer))
		return result;
	if (!Utils::Valid(localPlayer.inventoryPtr))
		return result;

	Inventory inventoryData{};
	if (!Utils::Rpm(localPlayer.inventoryPtr, &inventoryData, sizeof inventoryData))
		return result;
	if (!Utils::Valid(inventoryData.itemArrayPtr) || inventoryData.itemArrayEnd < inventoryData.itemArrayPtr)
		return result;

	const auto itemArraySize = (inventoryData.itemArrayEnd - inventoryData.itemArrayPtr) / sizeof(Item);
	if (!itemArraySize || itemArraySize > 0x7FFF)
		return result;

	auto itemData = std::make_unique<Item[]>(itemArraySize);
	if (!Utils::Rpm(inventoryData.itemArrayPtr, itemData.get(), itemArraySize * sizeof(Item)))
		return result;

	for (DWORD64 i = 0; i < itemArraySize; i++)
	{
		if (!Utils::Valid(itemData[i].referencePtr))
			continue;
		if (itemData[i].favoriteIndex != index)
			continue;

		TesItem referenceData{};
		if (!Utils::Rpm(itemData[i].referencePtr, &referenceData, sizeof referenceData))
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

bool ErectusMemory::EntityInventoryValid(const TesObjectRefr& entityData)
{
	if (!Utils::Valid(entityData.inventoryPtr))
		return false;

	Inventory inventoryData{};
	if (!Utils::Rpm(entityData.inventoryPtr, &inventoryData, sizeof inventoryData))
		return false;
	if (!Utils::Valid(inventoryData.itemArrayPtr) || inventoryData.itemArrayEnd < inventoryData.itemArrayPtr)
		return false;

	const auto itemArraySize = (inventoryData.itemArrayEnd - inventoryData.itemArrayPtr) / sizeof(Item);
	if (!itemArraySize || itemArraySize > 0x7FFF)
		return false;

	auto* itemData = new Item[itemArraySize];
	if (!Utils::Rpm(inventoryData.itemArrayPtr, &*itemData, itemArraySize * sizeof(Item)))
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
		if (!Utils::Rpm(itemData[i].referencePtr, &referenceData, sizeof referenceData))
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

bool ErectusMemory::AllowLegendaryWeapons(const EntityLooterSettings& settings)
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

bool ErectusMemory::AllowLegendaryArmor(const EntityLooterSettings& settings)
{
	if (settings.allArmorEnabled || settings.oneStarArmorEnabled || settings.twoStarArmorEnabled || settings.threeStarArmorEnabled)
		return true;

	return false;
}

bool ErectusMemory::CheckEntityLooterItem(const DWORD formId, const DWORD64 entityFlag, const EntityLooterSettings& settings, const bool legendaryWeaponsEnabled, const bool legendaryArmorEnabled)
{
	if (settings.capsEnabled && formId == 0x0000000F)
		return true;

	if (settings.listEnabled)
	{
		if (CheckFormIdArray(formId, settings.enabledList, settings.formIdList, 100))
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
		if (entityFlag & CUSTOM_ENTRY_FAILED_RECIPE)
			return settings.unknownPlansEnabled;
	}
	else if (entityFlag & CUSTOM_ENTRY_MISC)
		return settings.miscEnabled;

	if (settings.unlistedEnabled)
		return true;

	return false;
}

bool ErectusMemory::IsLegendaryFormId(const DWORD formId)
{
	for (const auto& i : legendaryFormIdArray)
	{
		if (formId == i)
			return true;
	}

	return false;
}

BYTE ErectusMemory::GetLegendaryRank(const DWORD64 displayPtr)
{
	if (!Utils::Valid(displayPtr))
		return 0;

	DWORD64 instancedArrayPtr;
	if (!Utils::Rpm(displayPtr, &instancedArrayPtr, sizeof instancedArrayPtr))
		return 0;
	if (!Utils::Valid(instancedArrayPtr))
		return 0;

	ItemInstancedArray itemInstancedArrayData{};
	if (!Utils::Rpm(instancedArrayPtr, &itemInstancedArrayData, sizeof itemInstancedArrayData))
		return 0;
	if (!Utils::Valid(itemInstancedArrayData.arrayPtr) || itemInstancedArrayData.arrayEnd < itemInstancedArrayData.arrayPtr)
		return 0;

	const auto instancedArraySize = (itemInstancedArrayData.arrayEnd - itemInstancedArrayData.arrayPtr) / sizeof(DWORD64);
	if (!instancedArraySize || instancedArraySize > 0x7FFF)
		return 0;

	auto* instancedArray = new DWORD64[instancedArraySize];
	if (!Utils::Rpm(itemInstancedArrayData.arrayPtr, &*instancedArray, instancedArraySize * sizeof(DWORD64)))
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
		if (!Utils::Rpm(instancedArray[i], &extraTextDisplayDataData, sizeof extraTextDisplayDataData))
			continue;

		const auto rttiNamePtr = RttiGetNamePtr(extraTextDisplayDataData.vtable);
		if (!rttiNamePtr)
			continue;

		char rttiNameCheck[sizeof".?AVBGSObjectInstanceExtra@@"];
		if (!Utils::Rpm(rttiNamePtr, &rttiNameCheck, sizeof rttiNameCheck))
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
	if (!Utils::Rpm(objectInstanceExtraPtr, &objectInstanceExtraData, sizeof objectInstanceExtraData))
		return 0;
	if (!Utils::Valid(objectInstanceExtraData.modDataPtr))
		return 0;

	ModInstance modInstanceData{};
	if (!Utils::Rpm(objectInstanceExtraData.modDataPtr, &modInstanceData, sizeof modInstanceData))
		return 0;
	if (!Utils::Valid(modInstanceData.modListPtr) || !modInstanceData.modListSize)
		return 0;

	const DWORD64 modArraySize = modInstanceData.modListSize / 0x8;
	if (!modArraySize || modArraySize > 0x7FFF)
		return 0;

	auto* modArray = new DWORD[modArraySize * 2];
	if (!Utils::Rpm(modInstanceData.modListPtr, &*modArray, modArraySize * 2 * sizeof(DWORD)))
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

bool ErectusMemory::ValidLegendary(const BYTE legendaryRank, const DWORD64 entityFlag, const EntityLooterSettings& customEntityLooterSettings, const bool legendaryWeaponsEnabled, const bool legendaryArmorEnabled)
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

bool ErectusMemory::TransferEntityItems(const TesObjectRefr& entityData, const TesItem& referenceData, const TesObjectRefr& localPlayer, const	bool onlyUseEntityLooterList, const bool useEntityLooterBlacklist)
{
	EntityLooterSettings currentEntityLooterSettings;
	switch (referenceData.formType)
	{
	case (static_cast<BYTE>(FormTypes::TesNpc)):
		currentEntityLooterSettings = ErectusIni::npcLooter;
		break;
	case (static_cast<BYTE>(FormTypes::TesObjectCont)):
		currentEntityLooterSettings = ErectusIni::containerLooter;
		break;
	default:
		return false;
	}

	if (!Utils::Valid(entityData.inventoryPtr))
		return false;

	Inventory inventoryData{};
	if (!Utils::Rpm(entityData.inventoryPtr, &inventoryData, sizeof inventoryData))
		return false;
	if (!Utils::Valid(inventoryData.itemArrayPtr) || inventoryData.itemArrayEnd < inventoryData.itemArrayPtr)
		return false;

	auto itemArraySize = (inventoryData.itemArrayEnd - inventoryData.itemArrayPtr) / sizeof(Item);
	if (!itemArraySize || itemArraySize > 0x7FFF)
		return false;

	auto* itemData = new Item[itemArraySize];
	if (!Utils::Rpm(inventoryData.itemArrayPtr, &*itemData, itemArraySize * sizeof(Item)))
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
		if (!Utils::Rpm(itemData[i].referencePtr, &itemReferenceData, sizeof itemReferenceData))
			continue;
		if (itemReferenceData.recordFlagA >> 2 & 1)
			continue;

		if (useEntityLooterBlacklist)
		{
			if (CheckFormIdArray(itemReferenceData.formId, currentEntityLooterSettings.blacklistEnabled, currentEntityLooterSettings.blacklist, 64))
				continue;
		}

		if (onlyUseEntityLooterList)
		{
			if (!CheckFormIdArray(itemReferenceData.formId, currentEntityLooterSettings.enabledList, currentEntityLooterSettings.formIdList, 100))
				continue;
		}

		auto entityFlag = CUSTOM_ENTRY_DEFAULT;
		DWORD64 entityNamePtr = 0;
		auto enabledDistance = 0;

		GetCustomEntityData(itemReferenceData, &entityFlag, &entityNamePtr, &enabledDistance, false, false);
		if (!(entityFlag & CUSTOM_ENTRY_VALID_ITEM))
			continue;

		if (!onlyUseEntityLooterList && !CheckEntityLooterItem(itemReferenceData.formId, entityFlag, currentEntityLooterSettings, legendaryWeaponsEnabled, legendaryArmorEnabled))
			continue;

		auto iterations = (itemData[i].iterations - itemData[i].displayPtr) / sizeof(ItemCount);
		if (!iterations || iterations > 0x7FFF)
			continue;

		auto* itemCountData = new ItemCount[iterations];
		if (!Utils::Rpm(itemData[i].displayPtr, &*itemCountData, iterations * sizeof(ItemCount)))
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
			.srcFormId = entityData.formId,
			.unknownId = 0xE0001F7A,
			.dstFormId = localPlayer.formId,
			.itemId = itemData[i].itemId,
			.count = count,
			.unknownA = 0x00000000,
			.unknownB = 0x00,
			.unknownC = 0x01,
			.unknownD = 0x00,
			.unknownE = 0x02,
		};

		SendMessageToServer(&transferMessageData, sizeof transferMessageData);
	}

	delete[]itemData;
	itemData = nullptr;
	return true;
}

bool ErectusMemory::ContainerValid(const TesItem& referenceData)
{
	if (!Utils::Valid(referenceData.keywordArrayData00C0))
		return false;

	int nifTextLength;
	if (!Utils::Rpm(referenceData.keywordArrayData00C0 + 0x10, &nifTextLength, sizeof nifTextLength))
		return false;
	if (nifTextLength == 41)
	{
		char containerMarkerCheck[sizeof"ContainerMarker"];
		if (!Utils::Rpm(referenceData.keywordArrayData00C0 + 0x2E, &containerMarkerCheck, sizeof containerMarkerCheck))
			return false;

		containerMarkerCheck[15] = '\0';
		if (!strcmp(containerMarkerCheck, "ContainerMarker"))
			return false;
	}

	if (!Utils::Valid(referenceData.namePtr00B0))
		return false;

	DWORD64 nameBuffer;
	if (!Utils::Rpm(referenceData.namePtr00B0 + 0x10, &nameBuffer, sizeof nameBuffer))
		return false;
	if (!nameBuffer)
		return false;

	if (!Utils::Valid(nameBuffer))
		nameBuffer = referenceData.namePtr00B0;

	int nameTextLength;
	if (!Utils::Rpm(nameBuffer + 0x10, &nameTextLength, sizeof nameTextLength))
		return false;
	if (!nameTextLength || nameTextLength > 0x7FFF)
		return false;

	return true;
}

bool ErectusMemory::LootEntity(const TesObjectRefr& entityData, const TesItem& referenceData, const TesObjectRefr& localPlayer,
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

	if (isEntityNpc && (referenceData.formId == 0x00000007 || CheckHealthFlag(entityData.healthFlag) != 0x3))
		return false;

	if (Utils::GetDistance(entityData.position, localPlayer.position) * 0.01f > maxDistance)
		return false;

	if (isEntityContainer && !ContainerValid(referenceData))
		return false;

	if (!EntityInventoryValid(entityData))
		return false;

	return TransferEntityItems(entityData, referenceData, localPlayer, onlyUseEntityLooterList, useEntityLooterBlacklist);
}

bool ErectusMemory::CheckEntityLooterSettings(const EntityLooterSettings& settings)
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

bool ErectusMemory::CheckOnlyUseEntityLooterList(const EntityLooterSettings& settings)
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

bool ErectusMemory::HarvestFlora(const TesObjectRefr& entityData, const TesItem& referenceData, const TesObjectRefr& localPlayer)
{
	if (FloraHarvested(entityData.harvestFlagA, entityData.harvestFlagB))
		return false;

	auto normalDistance = static_cast<int>(Utils::GetDistance(entityData.position, localPlayer.position) * 0.01f);
	if (normalDistance > 6)
		return false;

	if (!FloraValid(referenceData))
		return false;

	RequestActivateRefMessage requestActivateRefMessageData{
		.vtable = ErectusProcess::exe + VTABLE_REQUESTACTIVATEREFMSG,
		.formId = entityData.formId,
		.choice = 0xFF,
		.forceActivate = 0
	};

	return SendMessageToServer(&requestActivateRefMessageData, sizeof requestActivateRefMessageData);
}

bool ErectusMemory::Harvester()
{
	if (!MessagePatcher(allowMessages))
		return false;

	if (!allowMessages)
		return false;

	auto useNpcLooter = ErectusIni::npcLooter.enabled && CheckEntityLooterSettings(ErectusIni::npcLooter);

	auto useContainerLooter = ErectusIni::containerLooter.enabled && CheckEntityLooterSettings(ErectusIni::containerLooter);

	auto useFloraHarvester = ErectusIni::harvester.enabled && CheckIngredientList();

	if (!useNpcLooter && !useContainerLooter && !useFloraHarvester)
		return false;

	auto localPlayerPtr = GetLocalPlayerPtr(true);
	if (!Utils::Valid(localPlayerPtr))
		return false;

	TesObjectRefr localPlayer{};
	if (!Utils::Rpm(localPlayerPtr, &localPlayer, sizeof localPlayer))
		return false;

	auto onlyUseNpcLooterList = false;
	auto useNpcLooterBlacklist = false;
	if (useNpcLooter)
	{
		onlyUseNpcLooterList = CheckOnlyUseEntityLooterList(ErectusIni::npcLooter);
		useNpcLooterBlacklist = CheckEntityLooterBlacklist(ErectusIni::npcLooter);
	}

	auto onlyUseContainerLooterList = false;
	auto useContainerLooterBlacklist = false;
	if (useContainerLooter)
	{
		onlyUseContainerLooterList = CheckOnlyUseEntityLooterList(ErectusIni::containerLooter);
		useContainerLooterBlacklist = CheckEntityLooterBlacklist(ErectusIni::containerLooter);
	}

	if (useNpcLooter)
	{
		auto temporaryNpcList = GetNpcPtrList();
		if (temporaryNpcList.empty())
			return false;

		for (const auto& npcPtr : temporaryNpcList)
		{
			if (!Utils::Valid(npcPtr))
				continue;
			if (npcPtr == localPlayerPtr)
				continue;

			TesObjectRefr entityData{};
			if (!Utils::Rpm(npcPtr, &entityData, sizeof entityData))
				continue;
			if (!Utils::Valid(entityData.referencedItemPtr))
				continue;

			if (entityData.spawnFlag != 0x02)
				continue;

			TesItem referenceData{};
			if (!Utils::Rpm(entityData.referencedItemPtr, &referenceData, sizeof referenceData))
				continue;
			if (referenceData.formType != static_cast<BYTE>(FormTypes::TesNpc))
				continue;
			if (referenceData.formId == 0x00000007)
				continue;

			LootEntity(entityData, referenceData, localPlayer, onlyUseNpcLooterList, useNpcLooterBlacklist);
		}
	}

	if (useContainerLooter || useFloraHarvester)
	{
		auto temporaryEntityList = GetEntityList();
		if (temporaryEntityList.empty())
			return false;

		for (const auto& entityPtr : temporaryEntityList)
		{
			if (!Utils::Valid(entityPtr))
				continue;
			if (entityPtr == localPlayerPtr)
				continue;

			TesObjectRefr entityData{};
			if (!Utils::Rpm(entityPtr, &entityData, sizeof entityData))
				continue;
			if (!Utils::Valid(entityData.referencedItemPtr))
				continue;

			if (entityData.spawnFlag != 0x02)
				continue;

			TesItem referenceData{};
			if (!Utils::Rpm(entityData.referencedItemPtr, &referenceData, sizeof referenceData))
				continue;

			if (referenceData.formType == static_cast<byte>(FormTypes::TesObjectCont))
			{
				if (useContainerLooter)
					LootEntity(entityData, referenceData, localPlayer, onlyUseContainerLooterList, useContainerLooterBlacklist);
			}
			else if (referenceData.formType == static_cast<byte>(FormTypes::TesFlora))
			{
				if (useFloraHarvester)
					HarvestFlora(entityData, referenceData, localPlayer);
			}
		}
	}

	return true;
}

bool ErectusMemory::MeleeAttack()
{
	if (!MessagePatcher(allowMessages))
		return false;

	if (!allowMessages)
		return false;

	const auto localPlayerPtr = GetLocalPlayerPtr(true);
	if (!Utils::Valid(localPlayerPtr))
		return false;

	const auto allocAddress = Utils::AllocEx(sizeof(ExternalFunction));
	if (allocAddress == 0)
		return false;

	ExternalFunction externalFunctionData;
	externalFunctionData.address = ErectusProcess::exe + OFFSET_MELEE_ATTACK;
	externalFunctionData.rcx = localPlayerPtr;
	externalFunctionData.rdx = 0;
	externalFunctionData.r8 = 1;
	externalFunctionData.r9 = 0;

	const auto written = Utils::Wpm(allocAddress, &externalFunctionData, sizeof(ExternalFunction));

	if (!written)
	{
		Utils::FreeEx(allocAddress);
		return false;
	}

	const auto paramAddress = allocAddress + sizeof ExternalFunction::ASM;
	auto* const thread = CreateRemoteThread(ErectusProcess::handle, nullptr, 0, LPTHREAD_START_ROUTINE(allocAddress),
		LPVOID(paramAddress), 0, nullptr);

	if (thread == nullptr)
	{
		Utils::FreeEx(allocAddress);
		return false;
	}

	const auto threadResult = WaitForSingleObject(thread, 3000);
	CloseHandle(thread);

	if (threadResult == WAIT_TIMEOUT)
		return false;

	Utils::FreeEx(allocAddress);
	return true;
}

bool ErectusMemory::ChargenEditing()
{
	if (!ErectusIni::customChargenSettings.chargenEditingEnabled)
		return false;

	DWORD64 chargenPtr;
	if (!Utils::Rpm(ErectusProcess::exe + OFFSET_CHARGEN, &chargenPtr, sizeof chargenPtr))
		return false;
	if (!Utils::Valid(chargenPtr))
		return false;

	Chargen chargenData{};
	if (!Utils::Rpm(chargenPtr, &chargenData, sizeof chargenData))
		return false;

	auto shouldEdit = false;

	if (chargenData.thin != ErectusIni::customChargenSettings.thin)
	{
		chargenData.thin = ErectusIni::customChargenSettings.thin;
		shouldEdit = true;
	}

	if (chargenData.muscular != ErectusIni::customChargenSettings.muscular)
	{
		chargenData.muscular = ErectusIni::customChargenSettings.muscular;
		shouldEdit = true;
	}

	if (chargenData.large != ErectusIni::customChargenSettings.large)
	{
		chargenData.large = ErectusIni::customChargenSettings.large;
		shouldEdit = true;
	}

	if (shouldEdit)
		return Utils::Wpm(chargenPtr, &chargenData, sizeof chargenData);

	return true;
}

bool ErectusMemory::CreateProjectile(const DWORD itemId, const float* position, const float* rotation)
{
	if (!MessagePatcher(allowMessages))
		return false;

	if (!allowMessages)
		return false;

	const auto allocAddress = Utils::AllocEx(sizeof(ExecutionProjectile));
	if (allocAddress == 0)
		return false;

	CreateProjectileMessageClient createProjectileMessageClientData{};
	createProjectileMessageClientData.vtable = ErectusProcess::exe + VTABLE_CREATEPROJECTILEMESSAGECLIENT;
	createProjectileMessageClientData.positionX = position[0];
	createProjectileMessageClientData.positionY = position[1];
	createProjectileMessageClientData.positionZ = position[2];
	createProjectileMessageClientData.rotationArrayPtr = allocAddress + 0xD0;
	createProjectileMessageClientData.rotationArrayEnd = allocAddress + 0xD0 + sizeof(float[3]);
	createProjectileMessageClientData.rotationArrayPad = allocAddress + 0xD0 + sizeof(float[3]);
	createProjectileMessageClientData.itemId = itemId;
	createProjectileMessageClientData.unknownA = 0xFFFFFFFF;
	createProjectileMessageClientData.unknownB = 0xFFFFFFFF;
	createProjectileMessageClientData.unknownC = 0x00000000;
	createProjectileMessageClientData.unknownD = 1.0f;
	createProjectileMessageClientData.unknownE = 0x00000000;
	createProjectileMessageClientData.unknownArrayPtrA = allocAddress + 0xE0;
	createProjectileMessageClientData.unknownArrayEndA = allocAddress + 0xE0 + sizeof(WORD[1]);
	createProjectileMessageClientData.unknownArrayPadA = allocAddress + 0xE0 + sizeof(WORD[1]);
	createProjectileMessageClientData.unknownF = 0xFF;
	createProjectileMessageClientData.unknownArrayPtrB = allocAddress + 0xF0;
	createProjectileMessageClientData.unknownArrayEndB = allocAddress + 0xF0 + sizeof(BYTE[1]);
	createProjectileMessageClientData.unknownArrayPadB = allocAddress + 0xF0 + sizeof(BYTE[1]);
	createProjectileMessageClientData.unknownG = 0x00;

	ExecutionProjectile executionProjectileData;
	executionProjectileData.address = ErectusProcess::exe + OFFSET_MESSAGE_SENDER;
	executionProjectileData.rcx = allocAddress + 0x40;
	executionProjectileData.rdx = 0;
	memcpy(executionProjectileData.message, &createProjectileMessageClientData, sizeof createProjectileMessageClientData);
	executionProjectileData.rotationX = rotation[0];
	executionProjectileData.rotationY = rotation[1];
	executionProjectileData.rotationZ = rotation[2];
	executionProjectileData.unknownArrayValueA = WORD(Utils::GetRangedInt(999, 9999));
	executionProjectileData.unknownArrayValueB = 0x01;

	if (!Utils::Wpm(allocAddress, &executionProjectileData, sizeof executionProjectileData))
	{
		Utils::FreeEx(allocAddress);
		return false;
	}

	const auto paramAddress = allocAddress + sizeof ExecutionProjectile::ASM;
	auto* const thread = CreateRemoteThread(ErectusProcess::handle, nullptr, 0, LPTHREAD_START_ROUTINE(allocAddress),
		LPVOID(paramAddress), 0, nullptr);

	if (thread == nullptr)
	{
		Utils::FreeEx(allocAddress);
		return false;
	}

	const auto threadResult = WaitForSingleObject(thread, 3000);
	CloseHandle(thread);

	if (threadResult == WAIT_TIMEOUT)
		return false;

	Utils::FreeEx(allocAddress);
	return true;
}

bool ErectusMemory::CreateForwardProjectile(const DWORD itemId)
{
	if (itemId == 0)
		return false;

	const auto cameraPtr = GetCameraPtr();
	if (!Utils::Valid(cameraPtr))
		return false;

	Camera cameraData{};
	if (!Utils::Rpm(cameraPtr, &cameraData, sizeof cameraData))
		return false;

	float rotation[3];
	rotation[0] = -atan2f(cameraData.forward[2], sqrtf(powf(cameraData.forward[0], 2.0f) + powf(cameraData.forward[1], 2.0f)));
	rotation[1] = 0.0f;
	rotation[2] = -atan2f(-cameraData.forward[0], cameraData.forward[1]);

	return CreateProjectile(itemId, cameraData.origin, rotation);
}
