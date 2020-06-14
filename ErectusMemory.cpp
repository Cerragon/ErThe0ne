#include "ErectusInclude.h"
using namespace MemoryClasses;
using namespace SettingsClasses;

DWORD knownRecipeArray[0x1000];

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

DWORD64 ErectusMemory::GetAddress(DWORD formId)
{
	DWORD64 v1;
	if (!Utils::Rpm(ErectusProcess::exe + OFFSET_GET_PTR_A1, &v1, sizeof v1)) return 0;
	if (!Utils::Valid(v1)) return 0;

	DWORD v2; //array size or count
	if (!Utils::Rpm(v1 + 32, &v2, sizeof v2)) return 0;
	if (!v2) return 0;


	//v3 = cr32(formId)
	DWORD crc = 0;
	for (auto i = 0; i < sizeof formId; i++)
	{
		const auto v4 = (crc ^ formId >> i * 0x8) & 0xFF;

		DWORD v5;
		if (!Utils::Rpm(ErectusProcess::exe + OFFSET_GET_PTR_A2 + v4 * 0x4, &v5, sizeof v5)) return 0;

		crc = crc >> 0x8 ^ v5;
	}

	auto v6 = crc & (v2 - 1); // our formId's array index;

	DWORD64 v7; //array of refs to items in the world(?), each item of the array is: { formid (dword64!!!), ptr to object (dword64), item index/key in the array (dword64) }

	//sanity check: array exists
	if (!Utils::Rpm(v1 + 24, &v7, sizeof v7)) return 0;
	if (!Utils::Valid(v7)) return 0;

	//sanity check: the item has valid key
	DWORD v8;
	if (!Utils::Rpm(v7 + v6 * 24 + 16, &v8, sizeof v8)) return 0;
	if (v8 == 0xFFFFFFFF) return 0; //item id in the array == -1

	auto v9 = v2;

	//no clue what this loop is for ...
	for (auto i = 0; i < 100; i++)
	{
		DWORD v10;
		if (!Utils::Rpm(v7 + v6 * 24, &v10, sizeof v10)) return 0;
		if (v10 == formId)
		{
			v9 = v6;
			if (v9 != v2) break;
		}
		else
		{
			if (!Utils::Rpm(v7 + v6 * 24 + 16, &v6, sizeof v6)) return 0; //  try  next entry in the array
			if (v6 == v2) break;
		}
	}

	if (v9 == v2) return 0;

	return v7 + v9 * 24 + 8;
}

DWORD64 ErectusMemory::GetPtr(const DWORD formId)
{
	const auto address = GetAddress(formId);
	if (!address) return 0;

	DWORD64 ptr;
	if (!Utils::Rpm(address, &ptr, sizeof ptr)) return 0;

	return ptr;
}

DWORD64 ErectusMemory::GetCameraPtr()
{
	DWORD64 cameraPtr;
	if (!Utils::Rpm(ErectusProcess::exe + OFFSET_CAMERA, &cameraPtr, sizeof cameraPtr)) return 0;
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
	if (!Utils::Rpm(ErectusProcess::exe + OFFSET_LOCAL_PLAYER, &localPlayerPtr, sizeof localPlayerPtr)) return 0;
	if (!Utils::Valid(localPlayerPtr)) return 0;

	if (checkMainMenu)
	{
		Entity localPlayerData{};
		if (!Utils::Rpm(localPlayerPtr, &localPlayerData, sizeof localPlayerData)) return 0;
		if (localPlayerData.formId == 0x00000014) return 0;
	}

	return localPlayerPtr;
}

DWORD64* ErectusMemory::GetEntityList(int* size)
{
	DWORD64 entityListTypePtr;
	if (!Utils::Rpm(ErectusProcess::exe + OFFSET_ENTITY_LIST, &entityListTypePtr, sizeof entityListTypePtr)) return
		nullptr;
	if (!Utils::Valid(entityListTypePtr)) return nullptr;

	EntityListType entityListTypeData{};
	if (!Utils::Rpm(entityListTypePtr, &entityListTypeData, sizeof entityListTypeData)) return nullptr;
	if (!Utils::Valid(entityListTypeData.interiorListPtr)) return nullptr;
	if (!Utils::Valid(entityListTypeData.interiorListCheck)) return nullptr;
	if (!Utils::Valid(entityListTypeData.exteriorListPtr)) return nullptr;
	if (!Utils::Valid(entityListTypeData.exteriorListCheck)) return nullptr;

	DWORD64 entityListArrayPtr = 0;
	auto entityListArraySize = 0;

	auto useInteriorList = false;
	if (entityListTypeData.interiorListPtr != entityListTypeData.interiorListCheck)
	{
		useInteriorList = true;
		entityListArrayPtr = entityListTypeData.interiorListPtr;
		entityListArraySize = 1;
	}

	auto useExteriorList = false;
	if (entityListTypeData.exteriorListPtr != entityListTypeData.exteriorListCheck)
	{
		useExteriorList = true;
		entityListArrayPtr = entityListTypeData.exteriorListPtr;
		entityListArraySize = 25;
	}

	if (useInteriorList && useExteriorList) return nullptr;
	if (!useInteriorList && !useExteriorList) return nullptr;

	auto* entityListPtr = new DWORD64[entityListArraySize * 2];
	if (!Utils::Rpm(entityListArrayPtr, &*entityListPtr, entityListArraySize * sizeof(DWORD64) * 2))
	{
		delete[]entityListPtr;
		entityListPtr = nullptr;
		return nullptr;
	}

	auto** entityPtr = new DWORD64 * [entityListArraySize];
	auto combinedListSize = 0;

	auto* entityListData = new EntityList[entityListArraySize];
	for (auto i = 0; i < entityListArraySize; i++)
	{
		entityPtr[i] = nullptr;
		if (!Utils::Valid(entityListPtr[i * 2])) continue;

		if (!Utils::Rpm(entityListPtr[i * 2], &entityListData[i], sizeof entityListData[i])) continue;
		if (!Utils::Valid(entityListData[i].listPtr) || !entityListData[i].listSize) continue;

		entityPtr[i] = new DWORD64[entityListData[i].listSize];
		if (!Utils::Rpm(entityListData[i].listPtr, &*entityPtr[i], entityListData[i].listSize * sizeof(DWORD64)))
		{
			delete[]entityPtr[i];
			entityPtr[i] = nullptr;
			continue;
		}

		combinedListSize += entityListData[i].listSize;
	}

	DWORD64* list = nullptr;
	*size = 0;

	if (combinedListSize)
	{
		list = new DWORD64[combinedListSize];
		*size = combinedListSize;

		auto offset = 0;
		for (auto i = 0; i < entityListArraySize; i++)
		{
			if (entityPtr[i] == nullptr) continue;
			memcpy(&list[offset], &*entityPtr[i], entityListData[i].listSize * sizeof(DWORD64));
			offset += entityListData[i].listSize;
		}
	}

	for (auto i = 0; i < entityListArraySize; i++)
	{
		if (entityPtr[i] != nullptr)
		{
			delete[]entityPtr[i];
			entityPtr[i] = nullptr;
		}
	}

	delete[]entityPtr;
	entityPtr = nullptr;

	delete[]entityListData;
	entityListData = nullptr;

	delete[]entityListPtr;
	entityListPtr = nullptr;

	return list;
}

DWORD64* ErectusMemory::GetNpcList(int* size)
{
	DWORD64 processListsPtr;
	if (!Utils::Rpm(ErectusProcess::exe + OFFSET_NPC_LIST, &processListsPtr, sizeof processListsPtr)) return nullptr;
	if (!Utils::Valid(processListsPtr)) return nullptr;

	ProcessLists processListsData{};
	if (!Utils::Rpm(processListsPtr, &processListsData, sizeof processListsData)) return nullptr;

	auto idArraySize = 0;

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
	{
		return nullptr;
	}

	const auto allocSize = sizeof(ExecutionList) + idArraySize * sizeof(DWORD64) * 2;
	const auto allocAddress = Utils::AllocEx(allocSize);
	if (allocAddress == 0) return nullptr;

	ExecutionList executionListData;
	executionListData.function = ErectusProcess::exe + OFFSET_NPC_PTR_FUNCTION;
	executionListData.idArraySize = idArraySize;
	executionListData.idArray = allocAddress + sizeof(ExecutionList);
	executionListData.ptrArray = allocAddress + sizeof(ExecutionList) + idArraySize * sizeof(DWORD64);

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
		return nullptr;
	}

	const auto paramAddress = allocAddress + sizeof ExecutionList::ASM;
	auto* const thread = CreateRemoteThread(ErectusProcess::handle, nullptr, 0, LPTHREAD_START_ROUTINE(allocAddress),
		LPVOID(paramAddress), 0, nullptr);

	if (thread == nullptr)
	{
		Utils::FreeEx(allocAddress);
		return nullptr;
	}

	const auto threadResult = WaitForSingleObject(thread, 3000);
	CloseHandle(thread);

	if (threadResult == WAIT_TIMEOUT)
	{
		return nullptr;
	}

	auto* executedList = new DWORD64[idArraySize];
	if (!Utils::Rpm(allocAddress + sizeof(ExecutionList) + idArraySize * sizeof(DWORD64), &*executedList,
		idArraySize * sizeof(DWORD64)))
	{
		delete[]executedList;
		executedList = nullptr;

		Utils::FreeEx(allocAddress);
		return nullptr;
	}

	Utils::FreeEx(allocAddress);

	*size = idArraySize;
	return executedList;
}

DWORD64* ErectusMemory::GetRecipeArray(int* size)
{
	DWORD64 dataHandlerPtr;
	if (!Utils::Rpm(ErectusProcess::exe + OFFSET_DATA_HANDLER, &dataHandlerPtr, sizeof dataHandlerPtr)) return nullptr;
	if (!Utils::Valid(dataHandlerPtr)) return nullptr;

	ReferenceList omodList{};
	if (!Utils::Rpm(dataHandlerPtr + 0xF70, &omodList, sizeof omodList)) return nullptr;
	if (!Utils::Valid(omodList.arrayPtr) || !omodList.arraySize || omodList.arraySize > 0x7FFF) return nullptr;

	auto* omodPtrArray = new DWORD64[omodList.arraySize];
	if (!Utils::Rpm(omodList.arrayPtr, &*omodPtrArray, omodList.arraySize * sizeof(DWORD64)))
	{
		delete[]omodPtrArray;
		omodPtrArray = nullptr;
		return nullptr;
	}

	auto bufferIndex = 0;
	auto* bufferArray = new DWORD64[omodList.arraySize];
	memset(bufferArray, 0x00, omodList.arraySize * sizeof(DWORD64));

	auto uniqueArrayIndex = 0;
	auto* uniqueArray = new DWORD64[omodList.arraySize];
	memset(uniqueArray, 0x00, omodList.arraySize * sizeof(DWORD64));

	for (auto i = 0; i < omodList.arraySize; i++)
	{
		if (!Utils::Valid(omodPtrArray[i]))
		{
			continue;
		}

		Reference referenceData{};
		if (!Utils::Rpm(omodPtrArray[i], &referenceData, sizeof referenceData)) continue;
		if (referenceData.omodFlag != 0x4) continue;

		const auto recipePtr = referenceData.namePtr00B0;
		if (!Utils::Valid(recipePtr)) continue;

		auto uniquePtr = true;
		for (auto c = 0; c < uniqueArrayIndex; c++)
		{
			if (uniqueArray[c] == recipePtr)
			{
				uniquePtr = false;
				break;
			}
		}

		if (!uniquePtr)
		{
			continue;
		}

		uniqueArray[uniqueArrayIndex] = recipePtr;
		uniqueArrayIndex++;

		bufferArray[bufferIndex] = omodPtrArray[i];
		bufferIndex++;
	}

	delete[]uniqueArray;
	uniqueArray = nullptr;

	delete[]omodPtrArray;
	omodPtrArray = nullptr;

	auto recipeArraySize = 0;
	DWORD64* recipeArray = nullptr;

	if (bufferIndex)
	{
		recipeArraySize = bufferIndex;
		recipeArray = new DWORD64[recipeArraySize];
		memset(recipeArray, 0x00, recipeArraySize * sizeof(DWORD64));
		for (auto i = 0; i < recipeArraySize; i++)
		{
			recipeArray[i] = bufferArray[i];
		}
	}

	delete[]bufferArray;
	bufferArray = nullptr;

	*size = recipeArraySize;
	return recipeArray;
}

bool ErectusMemory::UpdateKnownRecipes()
{
	const auto localPlayerPtr = GetLocalPlayerPtr(true);
	if (!Utils::Valid(localPlayerPtr)) return false;

	auto recipeArraySize = 0;
	auto* recipeArray = GetRecipeArray(&recipeArraySize);
	if (recipeArray == nullptr) return false;

	const auto allocSize = sizeof(ExecutionPlan) + (recipeArraySize * sizeof(DWORD64) + recipeArraySize * sizeof(bool));
	const auto allocAddress = Utils::AllocEx(allocSize);
	if (allocAddress == 0)
	{
		delete[]recipeArray;
		recipeArray = nullptr;
		return false;
	}

	ExecutionPlan executionPlanData;
	executionPlanData.function = allocAddress + sizeof ExecutionPlan::ASM;
	executionPlanData.localPlayerPtr = localPlayerPtr;
	executionPlanData.recipeArraySize = recipeArraySize;
	executionPlanData.recipeArray = allocAddress + sizeof(ExecutionPlan);
	executionPlanData.learnedRecipeArray = allocAddress + sizeof(ExecutionPlan) + recipeArraySize * sizeof(DWORD64);

	auto* pageData = new BYTE[allocSize];
	memset(pageData, 0x00, allocSize);
	memcpy(pageData, &executionPlanData, sizeof executionPlanData);

	for (auto i = 0; i < recipeArraySize; i++)
	{
		memcpy(&pageData[sizeof(ExecutionPlan) + i * sizeof(DWORD64)], &recipeArray[i], sizeof(DWORD64));
	}

	const auto written = Utils::Wpm(allocAddress, &*pageData, allocSize);

	delete[]pageData;
	pageData = nullptr;

	if (!written)
	{
		delete[]recipeArray;
		recipeArray = nullptr;

		Utils::FreeEx(allocAddress);
		return false;
	}

	const auto paramAddress = allocAddress + sizeof ExecutionPlan::ASM + sizeof ExecutionPlan::rbp;
	auto* thread = CreateRemoteThread(ErectusProcess::handle, nullptr, 0, LPTHREAD_START_ROUTINE(allocAddress),
		LPVOID(paramAddress), 0, nullptr);

	if (thread == nullptr)
	{
		delete[]recipeArray;
		recipeArray = nullptr;

		Utils::FreeEx(allocAddress);
		return false;
	}

	const auto threadResult = WaitForSingleObject(thread, 3000);
	CloseHandle(thread);

	if (threadResult == WAIT_TIMEOUT)
	{
		delete[]recipeArray;
		recipeArray = nullptr;
		return false;
	}

	auto* executedPlan = new bool[recipeArraySize];
	if (!Utils::Rpm(allocAddress + sizeof(ExecutionPlan) + recipeArraySize * sizeof(DWORD64), &*executedPlan,
		recipeArraySize * sizeof(bool)))
	{
		delete[]recipeArray;
		recipeArray = nullptr;

		delete[]executedPlan;
		executedPlan = nullptr;

		Utils::FreeEx(allocAddress);
		return false;
	}

	auto knownRecipeIndex = 0;

	for (auto i = 0; i < recipeArraySize; i++)
	{
		if (executedPlan[i])
		{
			DWORD64 buffer;
			if (Utils::Rpm(recipeArray[i] + 0xB0, &buffer, sizeof buffer))
			{
				if (Utils::Valid(buffer))
				{
					DWORD formId;
					if (Utils::Rpm(buffer + 0x20, &formId, sizeof formId))
					{
						knownRecipeArray[knownRecipeIndex] = formId;
						knownRecipeIndex++;
					}
				}
			}
		}
	}

	knownRecipeArraySize = knownRecipeIndex;

	delete[]recipeArray;
	recipeArray = nullptr;

	delete[]executedPlan;
	executedPlan = nullptr;

	Utils::FreeEx(allocAddress);
	return true;
}

BYTE ErectusMemory::IsKnownRecipe(const DWORD formId)
{
	if (!knownRecipeArraySize || knownRecipeArraySize >= 0x1000)
	{
		return 0x00;
	}

	for (auto i = 0; i < knownRecipeArraySize; i++)
	{
		if (formId == knownRecipeArray[i])
		{
			return 0x01;
		}
	}

	return 0x02;
}

bool ErectusMemory::CheckFormIdArray(const DWORD formId, bool* enabledArray, const DWORD* formIdArray, const int size)
{
	for (auto i = 0; i < size; i++)
	{
		if (formId == formIdArray[i])
		{
			return enabledArray[i];
		}
	}

	return false;
}

bool ErectusMemory::CheckReferenceJunk(const Reference referenceData)
{
	if (referenceData.componentArraySize)
	{
		if (!(referenceData.recordFlagA >> 7 & 1))
		{
			return true;
		}
	}

	return false;
}

bool ErectusMemory::CheckReferenceMod(const Reference referenceData)
{
	if (referenceData.recordFlagA >> 7 & 1)
	{
		return true;
	}

	return false;
}

bool ErectusMemory::CheckReferencePlan(const Reference referenceData)
{
	if (referenceData.planFlag >> 5 & 1)
	{
		return true;
	}

	return false;
}

bool ErectusMemory::CheckScrapList()
{
	if (ErectusIni::customScrapLooterSettings.scrapLooterDistance)
	{
		for (auto i : ErectusIni::customScrapLooterSettings.scrapEnabledList)
		{
			if (i)
				return true;
		}
	}

	return false;
}

bool ErectusMemory::CheckItemLooterList()
{
	for (auto i = 0; i < 100; i++)
	{
		if (ErectusIni::customItemLooterSettings.itemLooterFormIdList[i])
		{
			if (ErectusIni::customItemLooterSettings.itemLooterEnabledList[i])
			{
				return true;
			}
		}
	}

	return false;
}

bool ErectusMemory::CheckItemLooterBlacklist()
{
	if (ErectusIni::customItemLooterSettings.itemLooterBlacklistToggle)
	{
		for (auto i = 0; i < 64; i++)
		{
			if (ErectusIni::customItemLooterSettings.itemLooterBlacklist[i])
			{
				if (ErectusIni::customItemLooterSettings.itemLooterBlacklistEnabled[i])
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool ErectusMemory::CheckEntityLooterList(EntityLooterSettings* customEntityLooterSettings)
{
	for (auto i = 0; i < 100; i++)
	{
		if (customEntityLooterSettings->entityLooterFormIdList[i])
		{
			if (customEntityLooterSettings->entityLooterEnabledList[i])
			{
				return true;
			}
		}
	}

	return false;
}

bool ErectusMemory::CheckEntityLooterBlacklist(EntityLooterSettings* customEntityLooterSettings)
{
	if (customEntityLooterSettings->entityLooterBlacklistToggle)
	{
		for (auto i = 0; i < 64; i++)
		{
			if (customEntityLooterSettings->entityLooterBlacklist[i] && customEntityLooterSettings->entityLooterBlacklistEnabled[i])
				return true;
		}
	}

	return false;
}

bool ErectusMemory::CheckIngredientList()
{
	for (auto i : ErectusIni::customHarvesterSettings.harvesterEnabledList)
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
		if (!strcmp(ErectusIni::customHarvesterSettings.harvesterNameList[i], "Junk Pile"))
			return ErectusIni::customHarvesterSettings.harvesterEnabledList[i];
	}

	return false;
}

bool ErectusMemory::CheckComponentArray(const Reference referenceData)
{
	if (!referenceData.componentArraySize || referenceData.componentArraySize > 0x7FFF)
		return false;

	if (!Utils::Valid(referenceData.componentArrayPtr))
		return false;

	auto* componentArray = new Component[referenceData.componentArraySize];
	if (!Utils::Rpm(referenceData.componentArrayPtr, &*componentArray,
		referenceData.componentArraySize * sizeof(Component)))
	{
		delete[]componentArray;
		componentArray = nullptr;
		return false;
	}

	for (auto i = 0; i < referenceData.componentArraySize; i++)
	{
		if (!Utils::Valid(componentArray[i].componentReferencePtr)) continue;
		if (!Utils::Valid(componentArray[i].componentCountReferencePtr)) continue;

		Reference componentData{};
		if (!Utils::Rpm(componentArray[i].componentReferencePtr, &componentData, sizeof componentData)) continue;
		if (CheckFormIdArray(componentData.formId, ErectusIni::customScrapLooterSettings.scrapEnabledList,
			ErectusIni::customScrapLooterSettings.scrapFormIdList, 40))
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

bool ErectusMemory::CheckReferenceKeywordBook(const Reference referenceData, const DWORD formId)
{
	if (!referenceData.keywordArrayData01C0 || referenceData.keywordArrayData01C0 > 0x7FFF) return false;
	if (!Utils::Valid(referenceData.keywordArrayData01B8)) return false;

	auto* keywordArray = new DWORD64[referenceData.keywordArrayData01C0];
	if (!Utils::Rpm(referenceData.keywordArrayData01B8, &*keywordArray,
		referenceData.keywordArrayData01C0 * sizeof(DWORD64)))
	{
		delete[]keywordArray;
		keywordArray = nullptr;
		return false;
	}

	for (DWORD64 i = 0; i < referenceData.keywordArrayData01C0; i++)
	{
		if (!Utils::Valid(keywordArray[i])) continue;

		DWORD formIdCheck;
		if (!Utils::Rpm(keywordArray[i] + 0x20, &formIdCheck, sizeof formIdCheck)) continue;
		if (formIdCheck != formId) continue;

		delete[]keywordArray;
		keywordArray = nullptr;
		return true;
	}

	delete[]keywordArray;
	keywordArray = nullptr;
	return false;
}

bool ErectusMemory::CheckReferenceKeywordMisc(const Reference referenceData, const DWORD formId)
{
	if (!referenceData.keywordArrayData01B8 || referenceData.keywordArrayData01B8 > 0x7FFF) return false;
	if (!Utils::Valid(referenceData.keywordArrayData01B0)) return false;

	auto* keywordArray = new DWORD64[referenceData.keywordArrayData01B8];
	if (!Utils::Rpm(referenceData.keywordArrayData01B0, &*keywordArray,
		referenceData.keywordArrayData01B8 * sizeof(DWORD64)))
	{
		delete[]keywordArray;
		keywordArray = nullptr;
		return false;
	}

	for (DWORD64 i = 0; i < referenceData.keywordArrayData01B8; i++)
	{
		if (!Utils::Valid(keywordArray[i])) continue;

		DWORD formIdCheck;
		if (!Utils::Rpm(keywordArray[i] + 0x20, &formIdCheck, sizeof formIdCheck)) continue;
		if (formIdCheck != formId) continue;

		delete[]keywordArray;
		keywordArray = nullptr;
		return true;
	}

	delete[]keywordArray;
	keywordArray = nullptr;
	return false;
}

bool ErectusMemory::CheckWhitelistedFlux(const Reference referenceData)
{
	if (!Utils::Valid(referenceData.harvestedPtr)) return false;

	DWORD formIdCheck;
	if (!Utils::Rpm(referenceData.harvestedPtr + 0x20, &formIdCheck, sizeof formIdCheck)) return false;

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

bool ErectusMemory::FloraLeveledListValid(const LeveledList leveledListData)
{
	if (!Utils::Valid(leveledListData.listEntryArrayPtr) || !leveledListData.listEntryArraySize)
	{
		return false;
	}

	auto* listEntryData = new ListEntry[leveledListData.listEntryArraySize];
	if (!Utils::Rpm(leveledListData.listEntryArrayPtr, &*listEntryData,
		leveledListData.listEntryArraySize * sizeof(ListEntry)))
	{
		delete[]listEntryData;
		listEntryData = nullptr;
		return false;
	}

	for (BYTE i = 0; i < leveledListData.listEntryArraySize; i++)
	{
		if (!Utils::Valid(listEntryData[i].referencePtr))
		{
			continue;
		}

		Reference referenceData{};
		if (!Utils::Rpm(listEntryData[i].referencePtr, &referenceData, sizeof referenceData)) continue;
		if (referenceData.vtable == ErectusProcess::exe + VTABLE_TESLEVITEM)
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
		else if (CheckFormIdArray(referenceData.formId, ErectusIni::customHarvesterSettings.harvesterEnabledList,
			ErectusIni::customHarvesterSettings.harvesterFormIdList, 69))
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

bool ErectusMemory::FloraValid(const Reference referenceData)
{
	if (referenceData.formId == 0x000183C6)
	{
		return CheckJunkPileEnabled();
	}

	if (!Utils::Valid(referenceData.harvestedPtr))
	{
		return false;
	}

	Reference harvestedData{};
	if (!Utils::Rpm(referenceData.harvestedPtr, &harvestedData, sizeof harvestedData)) return false;
	if (harvestedData.vtable == ErectusProcess::exe + VTABLE_TESLEVITEM)
	{
		LeveledList leveledListData{};
		memcpy(&leveledListData, &harvestedData, sizeof leveledListData);
		return FloraLeveledListValid(leveledListData);
	}
	return CheckFormIdArray(harvestedData.formId, ErectusIni::customHarvesterSettings.harvesterEnabledList,
		ErectusIni::customHarvesterSettings.harvesterFormIdList, 69);
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

bool ErectusMemory::BlacklistedNpcFaction(const Reference referenceData)
{
	if (!Utils::Valid(referenceData.factionArrayPtr) || !referenceData.factionArraySize || referenceData.
		factionArraySize > 0x7FFF)
	{
		return false;
	}

	auto* factionArray = new DWORD64[referenceData.factionArraySize * 2];
	if (!Utils::Rpm(referenceData.factionArrayPtr, &*factionArray,
		referenceData.factionArraySize * 2 * sizeof(DWORD64)))
	{
		delete[]factionArray;
		factionArray = nullptr;
		return false;
	}

	auto blacklistedFactionFound = false;
	for (auto i = 0; i < referenceData.factionArraySize; i++)
	{
		if (!Utils::Valid(factionArray[i * 2]))
		{
			continue;
		}

		DWORD formId;
		if (!Utils::Rpm(factionArray[i * 2] + 0x20, &formId, sizeof formId)) continue;
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

bool ErectusMemory::CheckReferenceItem(const Reference referenceData)
{
	switch (referenceData.vtable - ErectusProcess::exe)
	{
	case VTABLE_TESUTILITYITEM:
	case VTABLE_BGSNOTE:
	case VTABLE_TESKEY:
	case VTABLE_CURRENCYOBJECT:
		return true;
	default:
		return false;
	}
}

void ErectusMemory::GetCustomEntityData(const Reference referenceData, DWORD64* entityFlag, DWORD64* entityNamePtr,
                                        int* enabledDistance, const bool checkScrap, const bool checkIngredient)
{
	switch (referenceData.vtable - ErectusProcess::exe)
	{
	case VTABLE_TESNPC:
		*entityFlag |= CUSTOM_ENTRY_NPC;
		*entityNamePtr = referenceData.namePtr0160;
		*enabledDistance = ErectusIni::npcSettings.enabledDistance;
		break;
	case VTABLE_TESOBJECTCONT:
		*entityFlag |= CUSTOM_ENTRY_CONTAINER;
		*entityNamePtr = referenceData.namePtr00B0;
		*enabledDistance = ErectusIni::containerSettings.enabledDistance;
		if (CheckFormIdArray(referenceData.formId, ErectusIni::containerSettings.whitelisted,
			ErectusIni::containerSettings.whitelist, 32))
		{
			*entityFlag |= CUSTOM_ENTRY_WHITELISTED;
		}
		else if (!ErectusIni::containerSettings.enabled)
		{
			*entityFlag |= CUSTOM_ENTRY_INVALID;
		}
		break;
	case VTABLE_TESOBJECTMISC:
		*entityFlag |= CUSTOM_ENTRY_VALID_ITEM;
		*entityNamePtr = referenceData.namePtr0098;
		if (CheckReferenceJunk(referenceData))
		{
			*entityFlag |= CUSTOM_ENTRY_JUNK;
			*enabledDistance = ErectusIni::junkSettings.enabledDistance;
			if (checkScrap)
			{
				if (CheckComponentArray(referenceData))
				{
					*entityFlag |= CUSTOM_ENTRY_VALID_SCRAP;
				}
				else
				{
					*entityFlag |= CUSTOM_ENTRY_INVALID;
				}
			}
			else
			{
				if (CheckFormIdArray(referenceData.formId, ErectusIni::junkSettings.whitelisted,
					ErectusIni::junkSettings.whitelist, 32))
				{
					*entityFlag |= CUSTOM_ENTRY_WHITELISTED;
				}
				else if (!ErectusIni::junkSettings.enabled)
				{
					*entityFlag |= CUSTOM_ENTRY_INVALID;
				}
			}
		}
		else if (CheckReferenceMod(referenceData))
		{
			*entityFlag |= CUSTOM_ENTRY_MOD;
			*entityFlag |= CUSTOM_ENTRY_ITEM;
			*enabledDistance = ErectusIni::itemSettings.enabledDistance;
			if (CheckFormIdArray(referenceData.formId, ErectusIni::itemSettings.whitelisted,
				ErectusIni::itemSettings.whitelist, 32))
			{
				*entityFlag |= CUSTOM_ENTRY_WHITELISTED;
			}
			else if (!ErectusIni::itemSettings.enabled)
			{
				*entityFlag |= CUSTOM_ENTRY_INVALID;
			}
		}
		else if (CheckReferenceKeywordMisc(referenceData, 0x00135E6C))
		{
			*entityFlag |= CUSTOM_ENTRY_BOBBLEHEAD;
			*enabledDistance = ErectusIni::bobbleheadSettings.enabledDistance;
			if (CheckFormIdArray(referenceData.formId, ErectusIni::bobbleheadSettings.whitelisted,
				ErectusIni::bobbleheadSettings.whitelist, 32))
			{
				*entityFlag |= CUSTOM_ENTRY_WHITELISTED;
			}
			else if (!ErectusIni::bobbleheadSettings.enabled)
			{
				*entityFlag |= CUSTOM_ENTRY_INVALID;
			}
		}
		else
		{
			*entityFlag |= CUSTOM_ENTRY_MISC;
			*entityFlag |= CUSTOM_ENTRY_ITEM;
			*enabledDistance = ErectusIni::itemSettings.enabledDistance;
			if (CheckFormIdArray(referenceData.formId, ErectusIni::itemSettings.whitelisted,
				ErectusIni::itemSettings.whitelist, 32))
			{
				*entityFlag |= CUSTOM_ENTRY_WHITELISTED;
			}
			else if (!ErectusIni::itemSettings.enabled)
			{
				*entityFlag |= CUSTOM_ENTRY_INVALID;
			}
		}
		break;
	case VTABLE_TESOBJECTBOOK:
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
			if (CheckFormIdArray(referenceData.formId, ErectusIni::planSettings.whitelisted,
				ErectusIni::planSettings.whitelist, 32))
			{
				*entityFlag |= CUSTOM_ENTRY_WHITELISTED;
			}
			else if (!ErectusIni::planSettings.enabled)
			{
				*entityFlag |= CUSTOM_ENTRY_INVALID;
			}
		}
		else if (CheckReferenceKeywordBook(referenceData, 0x001D4A70))
		{
			*entityFlag |= CUSTOM_ENTRY_MAGAZINE;
			*enabledDistance = ErectusIni::magazineSettings.enabledDistance;
			if (CheckFormIdArray(referenceData.formId, ErectusIni::magazineSettings.whitelisted,
				ErectusIni::magazineSettings.whitelist, 32))
			{
				*entityFlag |= CUSTOM_ENTRY_WHITELISTED;
			}
			else if (!ErectusIni::magazineSettings.enabled)
			{
				*entityFlag |= CUSTOM_ENTRY_INVALID;
			}
		}
		else
		{
			*entityFlag |= CUSTOM_ENTRY_ITEM;
			*enabledDistance = ErectusIni::itemSettings.enabledDistance;
			if (IsTreasureMap(referenceData.formId))
			{
				*entityFlag |= CUSTOM_ENTRY_TREASURE_MAP;
			}
			if (CheckFormIdArray(referenceData.formId, ErectusIni::itemSettings.whitelisted,
				ErectusIni::itemSettings.whitelist, 32))
			{
				*entityFlag |= CUSTOM_ENTRY_WHITELISTED;
			}
			else if (!ErectusIni::itemSettings.enabled)
			{
				*entityFlag |= CUSTOM_ENTRY_INVALID;
			}
		}
		break;
	case VTABLE_TESFLORA:
		*entityFlag |= CUSTOM_ENTRY_FLORA;
		*entityNamePtr = referenceData.namePtr0098;
		*enabledDistance = ErectusIni::floraSettings.enabledDistance;
		if (checkIngredient)
		{
			if (FloraValid(referenceData))
			{
				*entityFlag |= CUSTOM_ENTRY_VALID_INGREDIENT;
			}
			else
			{
				*entityFlag |= CUSTOM_ENTRY_INVALID;
			}
		}
		else
		{
			if (CheckWhitelistedFlux(referenceData) || CheckFormIdArray(referenceData.formId, ErectusIni::floraSettings.whitelisted, ErectusIni::floraSettings.whitelist, 32))
			{
				*entityFlag |= CUSTOM_ENTRY_WHITELISTED;
			}
			else if (!ErectusIni::floraSettings.enabled)
			{
				*entityFlag |= CUSTOM_ENTRY_INVALID;
			}
		}
		break;
	case VTABLE_TESOBJECTWEAP:
		*entityFlag |= CUSTOM_ENTRY_WEAPON;
		*entityFlag |= CUSTOM_ENTRY_ITEM;
		*entityFlag |= CUSTOM_ENTRY_VALID_ITEM;
		*entityNamePtr = referenceData.namePtr0098;
		*enabledDistance = ErectusIni::itemSettings.enabledDistance;
		if (CheckFormIdArray(referenceData.formId, ErectusIni::itemSettings.whitelisted,
			ErectusIni::itemSettings.whitelist, 32))
		{
			*entityFlag |= CUSTOM_ENTRY_WHITELISTED;
		}
		else if (!ErectusIni::itemSettings.enabled)
		{
			*entityFlag |= CUSTOM_ENTRY_INVALID;
		}
		break;
	case VTABLE_TESOBJECTARMO:
		*entityFlag |= CUSTOM_ENTRY_ARMOR;
		*entityFlag |= CUSTOM_ENTRY_ITEM;
		*entityFlag |= CUSTOM_ENTRY_VALID_ITEM;
		*entityNamePtr = referenceData.namePtr0098;
		*enabledDistance = ErectusIni::itemSettings.enabledDistance;
		if (CheckFormIdArray(referenceData.formId, ErectusIni::itemSettings.whitelisted,
			ErectusIni::itemSettings.whitelist, 32))
		{
			*entityFlag |= CUSTOM_ENTRY_WHITELISTED;
		}
		else if (!ErectusIni::itemSettings.enabled)
		{
			*entityFlag |= CUSTOM_ENTRY_INVALID;
		}
		break;
	case VTABLE_TESAMMO:
		*entityFlag |= CUSTOM_ENTRY_AMMO;
		*entityFlag |= CUSTOM_ENTRY_ITEM;
		*entityFlag |= CUSTOM_ENTRY_VALID_ITEM;
		*entityNamePtr = referenceData.namePtr0098;
		*enabledDistance = ErectusIni::itemSettings.enabledDistance;
		if (CheckFormIdArray(referenceData.formId, ErectusIni::itemSettings.whitelisted,
			ErectusIni::itemSettings.whitelist, 32))
		{
			*entityFlag |= CUSTOM_ENTRY_WHITELISTED;
		}
		else if (!ErectusIni::itemSettings.enabled)
		{
			*entityFlag |= CUSTOM_ENTRY_INVALID;
		}
		break;
	case VTABLE_ALCHEMYITEM:
		*entityFlag |= CUSTOM_ENTRY_AID;
		*entityFlag |= CUSTOM_ENTRY_ITEM;
		*entityFlag |= CUSTOM_ENTRY_VALID_ITEM;
		*entityNamePtr = referenceData.namePtr0098;
		*enabledDistance = ErectusIni::itemSettings.enabledDistance;
		if (CheckFormIdArray(referenceData.formId, ErectusIni::itemSettings.whitelisted,
			ErectusIni::itemSettings.whitelist, 32))
		{
			*entityFlag |= CUSTOM_ENTRY_WHITELISTED;
		}
		else if (!ErectusIni::itemSettings.enabled)
		{
			*entityFlag |= CUSTOM_ENTRY_INVALID;
		}
		break;
	default:
		if (CheckReferenceItem(referenceData))
		{
			*entityFlag |= CUSTOM_ENTRY_ITEM;
			*entityFlag |= CUSTOM_ENTRY_VALID_ITEM;
			*entityNamePtr = referenceData.namePtr0098;
			*enabledDistance = ErectusIni::itemSettings.enabledDistance;
			if (CheckFormIdArray(referenceData.formId, ErectusIni::itemSettings.whitelisted,
				ErectusIni::itemSettings.whitelist, 32))
			{
				*entityFlag |= CUSTOM_ENTRY_WHITELISTED;
			}
			else if (!ErectusIni::itemSettings.enabled)
			{
				*entityFlag |= CUSTOM_ENTRY_INVALID;
			}
		}
		else
		{
			*entityFlag |= CUSTOM_ENTRY_ENTITY;
			*entityNamePtr = 0;
			*enabledDistance = ErectusIni::entitySettings.enabledDistance;
			if (CheckFormIdArray(referenceData.formId, ErectusIni::entitySettings.whitelisted,
				ErectusIni::entitySettings.whitelist, 32))
			{
				*entityFlag |= CUSTOM_ENTRY_WHITELISTED;
			}
			else if (!ErectusIni::entitySettings.enabled || !ErectusIni::entitySettings.drawUnnamed)
			{
				*entityFlag |= CUSTOM_ENTRY_INVALID;
			}
		}
		break;
	}
}

bool ErectusMemory::GetActorSnapshotComponentData(const Entity entityData, ActorSnapshotComponent* actorSnapshotComponentData)
{
	if (!Utils::Valid(entityData.actorCorePtr)) return false;

	DWORD64 actorCoreBufferA;
	if (!Utils::Rpm(entityData.actorCorePtr + 0x70, &actorCoreBufferA, sizeof actorCoreBufferA)) return false;
	if (!Utils::Valid(actorCoreBufferA)) return false;

	DWORD64 actorCoreBufferB;
	if (!Utils::Rpm(actorCoreBufferA + 0x8, &actorCoreBufferB, sizeof actorCoreBufferB)) return false;
	if (!Utils::Valid(actorCoreBufferB)) return false;

	return Utils::Rpm(actorCoreBufferB, &*actorSnapshotComponentData, sizeof(ActorSnapshotComponent));
}

char* ErectusMemory::GetEntityName(const DWORD64 ptr)
{
	if (!Utils::Valid(ptr))
	{
		return nullptr;
	}

	auto nameLength = 0;
	auto namePtr = ptr;

	if (!Utils::Rpm(namePtr + 0x10, &nameLength, sizeof nameLength)) return nullptr;
	if (nameLength <= 0 || nameLength > 0x7FFF)
	{
		DWORD64 buffer;
		if (!Utils::Rpm(namePtr + 0x10, &buffer, sizeof buffer)) return nullptr;
		if (!Utils::Valid(buffer)) return nullptr;

		if (!Utils::Rpm(buffer + 0x10, &nameLength, sizeof nameLength)) return nullptr;
		if (nameLength <= 0 || nameLength > 0x7FFF) return nullptr;
		namePtr = buffer;
	}

	const auto nameSize = nameLength + 1;
	auto* name = new char[nameSize];

	if (!Utils::Rpm(namePtr + 0x18, &*name, nameSize))
	{
		delete[]name;
		name = nullptr;
		return nullptr;
	}

	if (Utils::GetTextLength(name) != nameLength)
	{
		delete[]name;
		name = nullptr;
		return nullptr;
	}

	return name;
}

bool ErectusMemory::UpdateBufferEntityList()
{
	auto localPlayerPtr = GetLocalPlayerPtr(true);
	if (!Utils::Valid(localPlayerPtr)) return false;

	Entity localPlayer{};
	if (!Utils::Rpm(localPlayerPtr, &localPlayer, sizeof localPlayer)) return false;

	auto bufferSize = 0;
	auto* bufferList = GetEntityList(&bufferSize);
	if (bufferList == nullptr) return false;

	auto customListSize = 0;
	auto* customList = new CustomEntry[bufferSize];
	auto customListIndex = 0;

	for (auto i = 0; i < bufferSize; i++)
	{
		if (!Utils::Valid(bufferList[i])) continue;
		if (bufferList[i] == localPlayerPtr) continue;

		Entity entityData{};
		if (!Utils::Rpm(bufferList[i], &entityData, sizeof entityData)) continue;
		if (!Utils::Valid(entityData.referencePtr)) continue;

		Reference referenceData{};
		if (!Utils::Rpm(entityData.referencePtr, &referenceData, sizeof referenceData)) continue;
		if (referenceData.vtable == ErectusProcess::exe + VTABLE_TESNPC) continue;

		auto entityFlag = CUSTOM_ENTRY_DEFAULT;
		DWORD64 entityNamePtr = 0;
		auto enabledDistance = 0;

		GetCustomEntityData(referenceData, &entityFlag, &entityNamePtr, &enabledDistance,
			ErectusIni::customScrapLooterSettings.scrapOverrideEnabled,
			ErectusIni::customHarvesterSettings.harvesterOverrideEnabled);
		if (entityFlag & CUSTOM_ENTRY_INVALID) continue;

		auto distance = Utils::GetDistance(entityData.position, localPlayer.position);
		auto normalDistance = static_cast<int>(distance * 0.01f);
		if (normalDistance > enabledDistance) continue;

		customListSize++;
		customList[customListIndex].entityPtr = bufferList[i];
		customList[customListIndex].referencePtr = entityData.referencePtr;
		customList[customListIndex].entityFormId = entityData.formId;
		customList[customListIndex].referenceFormId = referenceData.formId;
		customList[customListIndex].flag = entityFlag;
		customList[customListIndex].name = GetEntityName(entityNamePtr);
		customListIndex++;
	}

	auto returnValue = false;

	if (customListSize)
	{
		bufferEntityList = new CustomEntry[customListSize];
		memcpy(&*bufferEntityList, &*customList, customListSize * sizeof(CustomEntry));
		bufferEntityListSize = customListSize;
		returnValue = true;
	}

	delete[]customList;
	customList = nullptr;

	delete[]bufferList;
	bufferList = nullptr;

	return returnValue;
}

bool ErectusMemory::UpdateBufferNpcList()
{
	auto localPlayerPtr = GetLocalPlayerPtr(true);
	if (!Utils::Valid(localPlayerPtr)) return false;

	Entity localPlayer{};
	if (!Utils::Rpm(localPlayerPtr, &localPlayer, sizeof localPlayer)) return false;

	auto bufferSize = 0;
	auto* bufferList = GetNpcList(&bufferSize);
	if (bufferList == nullptr) return false;

	auto customListSize = 0;
	auto* customList = new CustomEntry[bufferSize];
	auto customListIndex = 0;

	for (auto i = 0; i < bufferSize; i++)
	{
		if (!Utils::Valid(bufferList[i])) continue;
		if (bufferList[i] == localPlayerPtr) continue;

		Entity entityData{};
		if (!Utils::Rpm(bufferList[i], &entityData, sizeof entityData)) continue;
		if (!Utils::Valid(entityData.referencePtr)) continue;

		Reference referenceData{};
		if (!Utils::Rpm(entityData.referencePtr, &referenceData, sizeof referenceData)) continue;
		if (referenceData.vtable != ErectusProcess::exe + VTABLE_TESNPC) continue;
		if (referenceData.formId == 0x00000007) continue;

		auto entityFlag = CUSTOM_ENTRY_DEFAULT;
		DWORD64 entityNamePtr = 0;
		auto enabledDistance = 0;

		GetCustomEntityData(referenceData, &entityFlag, &entityNamePtr, &enabledDistance,
			ErectusIni::customScrapLooterSettings.scrapOverrideEnabled,
			ErectusIni::customHarvesterSettings.harvesterOverrideEnabled);
		if (entityFlag & CUSTOM_ENTRY_INVALID) continue;

		if (BlacklistedNpcFaction(referenceData))
		{
			continue;
		}

		if (ErectusIni::customExtraNpcSettings.useNpcBlacklist)
		{
			if (CheckFormIdArray(referenceData.formId, ErectusIni::customExtraNpcSettings.npcBlacklistEnabled,
				ErectusIni::customExtraNpcSettings.npcBlacklist, 64))
			{
				continue;
			}
		}

		auto distance = Utils::GetDistance(entityData.position, localPlayer.position);
		auto normalDistance = static_cast<int>(distance * 0.01f);
		if (normalDistance > enabledDistance) continue;

		customListSize++;
		customList[customListIndex].entityPtr = bufferList[i];
		customList[customListIndex].referencePtr = entityData.referencePtr;
		customList[customListIndex].entityFormId = entityData.formId;
		customList[customListIndex].referenceFormId = referenceData.formId;
		customList[customListIndex].flag = entityFlag;
		customList[customListIndex].name = GetEntityName(entityNamePtr);
		customListIndex++;
	}

	auto returnValue = false;

	if (customListSize)
	{
		bufferNpcList = new CustomEntry[customListSize];
		memcpy(&*bufferNpcList, &*customList, customListSize * sizeof(CustomEntry));
		bufferNpcListSize = customListSize;
		returnValue = true;
	}

	delete[]customList;
	customList = nullptr;

	delete[]bufferList;
	bufferList = nullptr;

	return returnValue;
}

char* ErectusMemory::GetPlayerName(ClientAccount* clientAccountData)
{
	if (!clientAccountData->nameLength)
	{
		return nullptr;
	}

	const auto playerNameSize = clientAccountData->nameLength + 1;
	auto* playerName = new char[playerNameSize];

	if (clientAccountData->nameLength > 15)
	{
		DWORD64 buffer;
		memcpy(&buffer, clientAccountData->nameData, sizeof buffer);
		if (!Utils::Valid(buffer))
		{
			delete[]playerName;
			playerName = nullptr;
			return nullptr;
		}

		if (!Utils::Rpm(buffer, &*playerName, playerNameSize))
		{
			delete[]playerName;
			playerName = nullptr;
			return nullptr;
		}
	}
	else
	{
		memcpy(playerName, clientAccountData->nameData, playerNameSize);
	}

	if (Utils::GetTextLength(playerName) != clientAccountData->nameLength)
	{
		delete[]playerName;
		playerName = nullptr;
		return nullptr;
	}

	return playerName;
}

bool ErectusMemory::UpdateBufferPlayerList()
{
	auto localPlayerPtr = GetLocalPlayerPtr(true);
	if (!Utils::Valid(localPlayerPtr)) return false;

	DWORD64 falloutMainDataPtr;
	if (!Utils::Rpm(ErectusProcess::exe + OFFSET_MAIN, &falloutMainDataPtr, sizeof falloutMainDataPtr)) return false;
	if (!Utils::Valid(falloutMainDataPtr)) return false;

	FalloutMain falloutMainData{};
	if (!Utils::Rpm(falloutMainDataPtr, &falloutMainData, sizeof falloutMainData)) return false;
	if (!Utils::Valid(falloutMainData.platformSessionManagerPtr)) return false;

	PlatformSessionManager platformSessionManagerData{};
	if (!Utils::Rpm(falloutMainData.platformSessionManagerPtr, &platformSessionManagerData,
		sizeof platformSessionManagerData)) return false;
	if (!Utils::Valid(platformSessionManagerData.clientAccountManagerPtr)) return false;

	ClientAccountManager clientAccountManagerData{};
	if (!Utils::Rpm(platformSessionManagerData.clientAccountManagerPtr, &clientAccountManagerData,
		sizeof clientAccountManagerData)) return false;
	if (!Utils::Valid(clientAccountManagerData.clientAccountArrayPtr)) return false;

	auto clientAccountArraySize = 0;
	clientAccountArraySize += clientAccountManagerData.clientAccountArraySizeA;
	clientAccountArraySize += clientAccountManagerData.clientAccountArraySizeB;
	if (!clientAccountArraySize) return false;

	auto* clientAccountArray = new DWORD64[clientAccountArraySize];
	if (!Utils::Rpm(clientAccountManagerData.clientAccountArrayPtr, &*clientAccountArray,
		clientAccountArraySize * sizeof(DWORD64)))
	{
		delete[]clientAccountArray;
		clientAccountArray = nullptr;
		return false;
	}

	auto customListSize = 0;
	auto* customList = new CustomEntry[clientAccountArraySize];
	auto customListIndex = 0;

	customListSize = 0;
	customListIndex = 0;

	for (auto i = 0; i < clientAccountArraySize; i++)
	{
		if (!Utils::Valid(clientAccountArray[i])) continue;

		ClientAccountBuffer clientAccountBufferData{};
		if (!Utils::Rpm(clientAccountArray[i], &clientAccountBufferData, sizeof clientAccountBufferData)) continue;
		if (!Utils::Valid(clientAccountBufferData.clientAccountPtr)) continue;

		ClientAccount clientAccountData{};
		if (!Utils::Rpm(clientAccountBufferData.clientAccountPtr, &clientAccountData, sizeof clientAccountData))
			continue;
		if (clientAccountData.formId == 0xFFFFFFFF) continue;

		auto entityPtr = GetPtr(clientAccountData.formId);
		if (!Utils::Valid(entityPtr)) continue;
		if (entityPtr == localPlayerPtr) continue;

		Entity entityData{};
		if (!Utils::Rpm(entityPtr, &entityData, sizeof entityData)) continue;
		if (!Utils::Valid(entityData.referencePtr)) continue;

		Reference referenceData{};
		if (!Utils::Rpm(entityData.referencePtr, &referenceData, sizeof referenceData)) continue;
		if (referenceData.formId != 0x00000007) continue;

		customListSize++;
		customList[customListIndex].entityPtr = entityPtr;
		customList[customListIndex].referencePtr = entityData.referencePtr;
		customList[customListIndex].entityFormId = entityData.formId;
		customList[customListIndex].referenceFormId = referenceData.formId;
		customList[customListIndex].flag = CUSTOM_ENTRY_PLAYER;
		customList[customListIndex].name = GetPlayerName(&clientAccountData);
		customListIndex++;
	}

	auto returnValue = false;

	if (customListSize)
	{
		bufferPlayerList = new CustomEntry[customListSize];
		memcpy(&*bufferPlayerList, &*customList, customListSize * sizeof(CustomEntry));
		bufferPlayerListSize = customListSize;
		returnValue = true;
	}

	delete[]customList;
	customList = nullptr;

	delete[]clientAccountArray;
	clientAccountArray = nullptr;

	return returnValue;
}

void ErectusMemory::DeleteCustomEntityList()
{
	if (customEntityList != nullptr)
	{
		if (customEntityListSize)
		{
			for (auto i = 0; i < customEntityListSize; i++)
			{
				if (customEntityList[i].name != nullptr)
				{
					delete[]customEntityList[i].name;
					customEntityList[i].name = nullptr;
				}
			}
		}

		delete[]customEntityList;
		customEntityList = nullptr;
	}

	customEntityListSize = 0;
	customEntityListUpdated = false;
}

void ErectusMemory::DeleteBufferEntityList()
{
	if (bufferEntityList != nullptr)
	{
		if (bufferEntityListSize)
		{
			for (auto i = 0; i < bufferEntityListSize; i++)
			{
				if (bufferEntityList[i].name != nullptr)
				{
					delete[]bufferEntityList[i].name;
					bufferEntityList[i].name = nullptr;
				}
			}
		}

		delete[]bufferEntityList;
		bufferEntityList = nullptr;
	}

	bufferEntityListSize = 0;
	bufferEntityListUpdated = false;
}

void ErectusMemory::DeleteCustomNpcList()
{
	if (customNpcList != nullptr)
	{
		if (customNpcListSize)
		{
			for (auto i = 0; i < customNpcListSize; i++)
			{
				if (customNpcList[i].name != nullptr)
				{
					delete[]customNpcList[i].name;
					customNpcList[i].name = nullptr;
				}
			}
		}

		delete[]customNpcList;
		customNpcList = nullptr;
	}

	customNpcListSize = 0;
	customNpcListUpdated = false;
}

void ErectusMemory::DeleteBufferNpcList()
{
	if (bufferNpcList != nullptr)
	{
		if (bufferNpcListSize)
		{
			for (auto i = 0; i < bufferNpcListSize; i++)
			{
				if (bufferNpcList[i].name != nullptr)
				{
					delete[]bufferNpcList[i].name;
					bufferNpcList[i].name = nullptr;
				}
			}
		}

		delete[]bufferNpcList;
		bufferNpcList = nullptr;
	}

	bufferNpcListSize = 0;
	bufferNpcListUpdated = false;
}

void ErectusMemory::DeleteCustomPlayerList()
{
	if (customPlayerList != nullptr)
	{
		if (customPlayerListSize)
		{
			for (auto i = 0; i < customPlayerListSize; i++)
			{
				if (customPlayerList[i].name != nullptr)
				{
					delete[]customPlayerList[i].name;
					customPlayerList[i].name = nullptr;
				}
			}
		}

		delete[]customPlayerList;
		customPlayerList = nullptr;
	}

	customPlayerListSize = 0;
	customPlayerListUpdated = false;
}

void ErectusMemory::DeleteBufferPlayerList()
{
	if (bufferPlayerList != nullptr)
	{
		if (bufferPlayerListSize)
		{
			for (auto i = 0; i < bufferPlayerListSize; i++)
			{
				if (bufferPlayerList[i].name != nullptr)
				{
					delete[]bufferPlayerList[i].name;
					bufferPlayerList[i].name = nullptr;
				}
			}
		}

		delete[]bufferPlayerList;
		bufferPlayerList = nullptr;
	}

	bufferPlayerListSize = 0;
	bufferPlayerListUpdated = false;
}

bool ErectusMemory::TargetValid(const Entity entityData, const Reference referenceData)
{
	if (referenceData.vtable != ErectusProcess::exe + VTABLE_TESNPC)
	{
		return false;
	}

	if (entityData.spawnFlag != 0x02)
	{
		if (!ErectusIni::customTargetSettings.ignoreRenderDistance)
		{
			return false;
		}
	}

	ActorSnapshotComponent actorSnapshotComponentData{};
	if (GetActorSnapshotComponentData(entityData, &actorSnapshotComponentData))
	{
		if (ErectusIni::customTargetSettings.ignoreEssentialNpCs)
		{
			if (actorSnapshotComponentData.isEssential)
			{
				return false;
			}
		}
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

bool ErectusMemory::RenderCustomEntryA(CustomEntry* customEntryData, OverlaySettingsA customSettingsA)
{
	auto health = -1;
	BYTE epicRank = 0;
	auto allowNpc = false;
	if (customEntryData->flag & CUSTOM_ENTRY_NPC)
	{
		Entity npcData{};
		if (!Utils::Rpm(customEntryData->entityPtr, &npcData, sizeof npcData)) return false;

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

	if (!customSettingsA.enabled)
	{
		if (!allowNpc)
		{
			return false;
		}
	}

	if (!customSettingsA.drawEnabled && !customSettingsA.drawDisabled)
	{
		return false;
	}

	if (customSettingsA.enabledAlpha == 0.0f && customSettingsA.disabledAlpha == 0.0f)
	{
		return false;
	}

	if (!customSettingsA.drawNamed && !customSettingsA.drawUnnamed)
	{
		return false;
	}

	Entity entityData{};
	if (!Utils::Rpm(customEntryData->entityPtr, &entityData, sizeof entityData)) return false;

	if (customEntryData->flag & CUSTOM_ENTRY_PLAYER)
	{
		ActorSnapshotComponent actorSnapshotComponentData{};
		if (GetActorSnapshotComponentData(entityData, &actorSnapshotComponentData))
		{
			health = static_cast<int>(actorSnapshotComponentData.maxHealth + actorSnapshotComponentData.modifiedHealth + actorSnapshotComponentData.lostHealth);
		}
	}

	if (customEntryData->name == nullptr)
	{
		customEntryData->flag |= CUSTOM_ENTRY_UNNAMED;
		customEntryData->name = new char[sizeof"00000000"];
		sprintf_s(customEntryData->name, sizeof"00000000", "%08lX", entityData.formId);
	}

	if (customEntryData->flag & CUSTOM_ENTRY_UNNAMED)
	{
		if (!customSettingsA.drawUnnamed) return false;
	}
	else
	{
		if (!customSettingsA.drawNamed) return false;
	}

	float* alpha = nullptr;

	if (entityData.spawnFlag == 0x02)
	{
		if (customSettingsA.drawEnabled)
		{
			alpha = &customSettingsA.enabledAlpha;
		}
	}
	else
	{
		if (customSettingsA.drawDisabled)
		{
			alpha = &customSettingsA.disabledAlpha;
		}
	}

	if (alpha == nullptr)
	{
		return false;
	}

	auto showHealthText = false;

	float* color = nullptr;

	auto legendaryAlpha = 1.0f;

	switch (CheckHealthFlag(entityData.healthFlag))
	{
	case 0x01: //Alive
		showHealthText = customSettingsA.showHealth;
		if (allowNpc)
		{
			switch (epicRank)
			{
			case 1:
				color = ErectusIni::customLegendarySettings.livingOneStarColor;
				if (entityData.spawnFlag == 0x02)
				{
					alpha = &legendaryAlpha;
				}
				break;
			case 2:
				color = ErectusIni::customLegendarySettings.livingTwoStarColor;
				if (entityData.spawnFlag == 0x02)
				{
					alpha = &legendaryAlpha;
				}
				break;
			case 3:
				color = ErectusIni::customLegendarySettings.livingThreeStarColor;
				if (entityData.spawnFlag == 0x02)
				{
					alpha = &legendaryAlpha;
				}
				break;
			default:
				break;
			}
		}
		else if (customSettingsA.drawAlive)
		{
			color = customSettingsA.aliveColor;
		}
		break;
	case 0x02: //Downed
		showHealthText = customSettingsA.showHealth;
		if (allowNpc)
		{
			switch (epicRank)
			{
			case 1:
				color = ErectusIni::customLegendarySettings.livingOneStarColor;
				if (entityData.spawnFlag == 0x02)
				{
					alpha = &legendaryAlpha;
				}
				break;
			case 2:
				color = ErectusIni::customLegendarySettings.livingTwoStarColor;
				if (entityData.spawnFlag == 0x02)
				{
					alpha = &legendaryAlpha;
				}
				break;
			case 3:
				color = ErectusIni::customLegendarySettings.livingThreeStarColor;
				if (entityData.spawnFlag == 0x02)
				{
					alpha = &legendaryAlpha;
				}
				break;
			default:
				break;
			}
		}
		else if (customSettingsA.drawDowned)
		{
			color = customSettingsA.downedColor;
		}
		break;
	case 0x03: //Dead
		showHealthText = customSettingsA.showDeadHealth;
		if (allowNpc)
		{
			switch (epicRank)
			{
			case 1:
				color = ErectusIni::customLegendarySettings.deadOneStarColor;
				if (entityData.spawnFlag == 0x02)
				{
					alpha = &legendaryAlpha;
				}
				break;
			case 2:
				color = ErectusIni::customLegendarySettings.deadTwoStarColor;
				if (entityData.spawnFlag == 0x02)
				{
					alpha = &legendaryAlpha;
				}
				break;
			case 3:
				color = ErectusIni::customLegendarySettings.deadThreeStarColor;
				if (entityData.spawnFlag == 0x02)
				{
					alpha = &legendaryAlpha;
				}
				break;
			default:
				break;
			}
		}
		else if (customSettingsA.drawDead)
		{
			color = customSettingsA.deadColor;
		}
		break;
	default: //Unknown
		showHealthText = customSettingsA.showHealth;
		if (customSettingsA.drawUnknown)
		{
			color = customSettingsA.unknownColor;
		}
		break;
	}

	if (color == nullptr)
	{
		return false;
	}

	auto cameraPtr = GetCameraPtr();
	if (!Utils::Valid(cameraPtr)) return false;

	Camera cameraData{};
	if (!Utils::Rpm(cameraPtr, &cameraData, sizeof cameraData)) return false;

	auto distance = Utils::GetDistance(entityData.position, cameraData.origin);
	auto normalDistance = static_cast<int>(distance * 0.01f);
	if (normalDistance > customSettingsA.enabledDistance) return false;

	if ((customEntryData->flag & CUSTOM_ENTRY_PLAYER && ErectusIni::customTargetSettings.lockPlayers) || (customEntryData->flag & CUSTOM_ENTRY_NPC && ErectusIni::customTargetSettings.lockNpCs))
	{
		Reference referenceData{};
		if (Utils::Rpm(entityData.referencePtr, &referenceData, sizeof referenceData))
		{
			if (TargetValid(entityData, referenceData))
			{
				if (customEntryData->entityPtr == targetLockingPtr)
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
						targetLockingClosestPtr = customEntryData->entityPtr;
					}
				}
			}
		}
	}

	float screen[2] = { 0.0f, 0.0f };
	if (!Utils::Wts(cameraData.view, entityData.position, screen)) return false;

	char* nameText = nullptr;
	if (customSettingsA.showName && showHealthText && customSettingsA.showDistance) //Name, Health, Distance
	{
		auto textSize = static_cast<int>(Utils::GetTextLength(customEntryData->name) + sizeof"\n-2147483648 hp [-2147483648 m]");
		nameText = new char[textSize];
		sprintf_s(nameText, textSize, "%s\n%d hp [%d m]", customEntryData->name, health, normalDistance);
	}
	else if (customSettingsA.showName && showHealthText && !customSettingsA.showDistance) //Name, Health
	{
		auto textSize = static_cast<int>(Utils::GetTextLength(customEntryData->name) + sizeof"\n-2147483648 hp");
		nameText = new char[textSize];
		sprintf_s(nameText, textSize, "%s\n%d hp", customEntryData->name, health);
	}
	else if (customSettingsA.showName && !showHealthText && customSettingsA.showDistance) //Name, Distance
	{
		auto textSize = static_cast<int>(Utils::GetTextLength(customEntryData->name) + sizeof"\n[-2147483648 m]");
		nameText = new char[textSize];
		sprintf_s(nameText, textSize, "%s\n[%d m]", customEntryData->name, normalDistance);
	}
	else if (!customSettingsA.showName && showHealthText && customSettingsA.showDistance) //Health, Distance
	{
		int textSize = sizeof"-2147483648 hp [-2147483648 m]";
		nameText = new char[textSize];
		sprintf_s(nameText, textSize, "%d hp [%d m]", health, normalDistance);
	}
	else if (customSettingsA.showName && !showHealthText && !customSettingsA.showDistance) //Name
	{
		auto textSize = Utils::GetTextSize(customEntryData->name);
		nameText = new char[textSize];
		sprintf_s(nameText, textSize, "%s", customEntryData->name);
	}
	else if (!customSettingsA.showName && showHealthText && !customSettingsA.showDistance) //Health
	{
		int textSize = sizeof"-2147483648 hp";
		nameText = new char[textSize];
		sprintf_s(nameText, textSize, "%d hp", health);
	}
	else if (!customSettingsA.showName && !showHealthText && customSettingsA.showDistance) //Distance
	{
		int textSize = sizeof"[-2147483648 m]";
		nameText = new char[textSize];
		sprintf_s(nameText, textSize, "[%d m]", normalDistance);
	}

	if (nameText != nullptr)
	{
		if (ErectusIni::customUtilitySettings.debugEsp)
		{
			char debugText[sizeof"0000000000000000\n00000000\n0000000000000000\n00000000"];
			sprintf_s(debugText, "%llX\n%08lX\n%llX\n%08lX", customEntryData->entityPtr, customEntryData->entityFormId,
				customEntryData->referencePtr, customEntryData->referenceFormId);
			Renderer::DrawText(debugText, customSettingsA.textShadowed, customSettingsA.textCentered, screen, color, *alpha);
		}
		else
		{
			Renderer::DrawText(nameText, customSettingsA.textShadowed, customSettingsA.textCentered, screen, color, *alpha);
		}

		delete[]nameText;
		nameText = nullptr;
	}

	return true;
}

bool ErectusMemory::RenderCustomEntryB(CustomEntry* customEntryData, OverlaySettingsB customSettingsB)
{
	if (!(customEntryData->flag & CUSTOM_ENTRY_WHITELISTED) && !customSettingsB.enabled)
		return false;

	if (!customSettingsB.drawEnabled && !customSettingsB.drawDisabled)
		return false;

	if (customSettingsB.enabledAlpha == 0.0f && customSettingsB.disabledAlpha == 0.0f)
		return false;

	if (!customSettingsB.drawNamed && !customSettingsB.drawUnnamed)
		return false;

	Entity entityData{};
	if (!Utils::Rpm(customEntryData->entityPtr, &entityData, sizeof entityData)) return false;

	if (customEntryData->name == nullptr)
	{
		customEntryData->flag |= CUSTOM_ENTRY_UNNAMED;
		customEntryData->name = new char[sizeof"00000000"];
		sprintf_s(customEntryData->name, sizeof"00000000", "%08lX", entityData.formId);
	}

	if (customEntryData->flag & CUSTOM_ENTRY_UNNAMED)
	{
		if (!customSettingsB.drawUnnamed)
			return false;
	}
	else if (!customSettingsB.drawNamed)
		return false;

	if (customEntryData->flag & CUSTOM_ENTRY_PLAN)
	{
		if (!ErectusIni::customKnownRecipeSettings.knownRecipesEnabled && !ErectusIni::customKnownRecipeSettings.unknownRecipesEnabled)
			return false;

		if (!(customEntryData->flag & CUSTOM_ENTRY_FAILED_RECIPE))
		{
			if (!ErectusIni::customKnownRecipeSettings.knownRecipesEnabled && customEntryData->flag & CUSTOM_ENTRY_KNOWN_RECIPE)
			{
				return false;
			}
			if (!ErectusIni::customKnownRecipeSettings.unknownRecipesEnabled && customEntryData->flag & CUSTOM_ENTRY_UNKNOWN_RECIPE)
			{
				return false;
			}
		}
	}

	float* alpha = nullptr;

	if (entityData.spawnFlag == 0x02)
	{
		if (customSettingsB.drawEnabled)
		{
			if (customEntryData->flag & CUSTOM_ENTRY_FLORA)
			{
				if (!FloraHarvested(entityData.harvestFlagA, entityData.harvestFlagB))
				{
					alpha = &customSettingsB.enabledAlpha;
				}
				else if (customSettingsB.drawDisabled)
				{
					alpha = &customSettingsB.disabledAlpha;
				}
			}
			else
			{
				alpha = &customSettingsB.enabledAlpha;
			}
		}
	}
	else
	{
		if (customSettingsB.drawDisabled)
		{
			alpha = &customSettingsB.disabledAlpha;
		}
	}

	if (alpha == nullptr)
	{
		return false;
	}

	const auto cameraPtr = GetCameraPtr();
	if (!Utils::Valid(cameraPtr)) return false;

	Camera cameraData{};
	if (!Utils::Rpm(cameraPtr, &cameraData, sizeof cameraData)) return false;

	const auto distance = Utils::GetDistance(entityData.position, cameraData.origin);
	const auto normalDistance = static_cast<int>(distance * 0.01f);
	if (normalDistance > customSettingsB.enabledDistance) return false;

	float screen[2] = { 0.0f, 0.0f };
	if (!Utils::Wts(cameraData.view, entityData.position, screen)) return false;

	char* nameText = nullptr;
	if (customSettingsB.showName && customSettingsB.showDistance)
	{
		const auto distanceTextSize = static_cast<int>(Utils::GetTextLength(customEntryData->name) + sizeof"\n[-2147483648 m]");
		nameText = new char[distanceTextSize];
		sprintf_s(nameText, distanceTextSize, "%s\n[%d m]", customEntryData->name, normalDistance);
	}
	else if (customSettingsB.showName && !customSettingsB.showDistance)
	{
		const auto textSize = Utils::GetTextSize(customEntryData->name);
		nameText = new char[textSize];
		sprintf_s(nameText, textSize, "%s", customEntryData->name);
	}
	else if (!customSettingsB.showName && customSettingsB.showDistance)
	{
		const int distanceTextSize = sizeof"[-2147483648 m]";
		nameText = new char[distanceTextSize];
		sprintf_s(nameText, distanceTextSize, "[%d m]", normalDistance);
	}

	if (nameText != nullptr)
	{
		if (ErectusIni::customUtilitySettings.debugEsp)
		{
			char debugText[sizeof"0000000000000000\n00000000\n0000000000000000\n00000000"];
			sprintf_s(debugText, "%llX\n%08lX\n%llX\n%08lX", customEntryData->entityPtr, customEntryData->entityFormId,
				customEntryData->referencePtr, customEntryData->referenceFormId);
			Renderer::DrawText(debugText, customSettingsB.textShadowed, customSettingsB.textCentered, screen,
				customSettingsB.color, *alpha);
		}
		else
		{
			Renderer::DrawText(nameText, customSettingsB.textShadowed, customSettingsB.textCentered, screen,
				customSettingsB.color, *alpha);
		}

		delete[]nameText;
		nameText = nullptr;
	}

	return true;
}

bool ErectusMemory::RenderCustomEntityList()
{
	customEntityListCounter++;
	if (customEntityListCounter > 60)
	{
		customEntityListCounter = 0;
		if (bufferEntityListUpdated)
		{
			if (!bufferEntityListDestructionQueued)
			{
				DeleteCustomEntityList();
				customEntityList = new CustomEntry[bufferEntityListSize];
				memcpy(&*customEntityList, &*bufferEntityList, bufferEntityListSize * sizeof(CustomEntry));
				for (auto i = 0; i < bufferEntityListSize; i++) bufferEntityList[i].name = nullptr;
				customEntityListSize = bufferEntityListSize;
				customEntityListUpdated = true;
				bufferEntityListDestructionQueued = true;
			}
		}
	}

	if (customEntityListDestructionQueued)
	{
		customEntityListDestructionQueued = false;
		DeleteCustomEntityList();
	}

	if (!customEntityListUpdated)
	{
		return false;
	}

	for (auto i = 0; i < customEntityListSize; i++)
	{
		if (customEntityList[i].flag & CUSTOM_ENTRY_ENTITY)
		{
			RenderCustomEntryB(&customEntityList[i], ErectusIni::entitySettings);
		}
		else if (customEntityList[i].flag & CUSTOM_ENTRY_JUNK)
		{
			RenderCustomEntryB(&customEntityList[i], ErectusIni::junkSettings);
		}
		else if (customEntityList[i].flag & CUSTOM_ENTRY_ITEM)
		{
			RenderCustomEntryB(&customEntityList[i], ErectusIni::itemSettings);
		}
		else if (customEntityList[i].flag & CUSTOM_ENTRY_CONTAINER)
		{
			RenderCustomEntryB(&customEntityList[i], ErectusIni::containerSettings);
		}
		else if (customEntityList[i].flag & CUSTOM_ENTRY_PLAN)
		{
			RenderCustomEntryB(&customEntityList[i], ErectusIni::planSettings);
		}
		else if (customEntityList[i].flag & CUSTOM_ENTRY_MAGAZINE)
		{
			RenderCustomEntryB(&customEntityList[i], ErectusIni::magazineSettings);
		}
		else if (customEntityList[i].flag & CUSTOM_ENTRY_BOBBLEHEAD)
		{
			RenderCustomEntryB(&customEntityList[i], ErectusIni::bobbleheadSettings);
		}
		else if (customEntityList[i].flag & CUSTOM_ENTRY_FLORA)
		{
			RenderCustomEntryB(&customEntityList[i], ErectusIni::floraSettings);
		}
	}

	return true;
}

bool ErectusMemory::RenderCustomNpcList()
{
	customNpcListCounter++;
	if (customNpcListCounter > 60)
	{
		customNpcListCounter = 0;
		if (bufferNpcListUpdated)
		{
			if (!bufferNpcListDestructionQueued)
			{
				DeleteCustomNpcList();
				customNpcList = new CustomEntry[bufferNpcListSize];
				memcpy(&*customNpcList, &*bufferNpcList, bufferNpcListSize * sizeof(CustomEntry));
				for (auto i = 0; i < bufferNpcListSize; i++) bufferNpcList[i].name = nullptr;
				customNpcListSize = bufferNpcListSize;
				customNpcListUpdated = true;
				bufferNpcListDestructionQueued = true;
			}
		}
	}

	if (customNpcListDestructionQueued)
	{
		customNpcListDestructionQueued = false;
		DeleteCustomNpcList();
	}

	if (!customNpcListUpdated)
	{
		return false;
	}

	for (auto i = 0; i < customNpcListSize; i++)
	{
		if (customNpcList[i].flag & CUSTOM_ENTRY_NPC)
		{
			RenderCustomEntryA(&customNpcList[i], ErectusIni::npcSettings);
		}
	}

	return true;
}

bool ErectusMemory::RenderCustomPlayerList()
{
	customPlayerListCounter++;
	if (customPlayerListCounter > 60)
	{
		customPlayerListCounter = 0;
		if (bufferPlayerListUpdated)
		{
			if (!bufferPlayerListDestructionQueued)
			{
				DeleteCustomPlayerList();
				customPlayerList = new CustomEntry[bufferPlayerListSize];
				memcpy(&*customPlayerList, &*bufferPlayerList, bufferPlayerListSize * sizeof(CustomEntry));
				for (auto i = 0; i < bufferPlayerListSize; i++) bufferPlayerList[i].name = nullptr;
				customPlayerListSize = bufferPlayerListSize;
				customPlayerListUpdated = true;
				bufferPlayerListDestructionQueued = true;
			}
		}
	}

	if (customPlayerListDestructionQueued)
	{
		customPlayerListDestructionQueued = false;
		DeleteCustomPlayerList();
	}

	if (!customPlayerListUpdated)
	{
		return false;
	}

	for (auto i = 0; i < customPlayerListSize; i++)
	{
		if (customPlayerList[i].flag & CUSTOM_ENTRY_PLAYER)
		{
			RenderCustomEntryA(&customPlayerList[i], ErectusIni::playerSettings);
		}
	}

	return true;
}

bool ErectusMemory::MessagePatcher(const bool state)
{
	BYTE fakeMessagesCheck[2];
	if (!Utils::Rpm(ErectusProcess::exe + OFFSET_FAKE_MESSAGE, &fakeMessagesCheck, sizeof fakeMessagesCheck)) return
		false;

	BYTE fakeMessagesEnabled[] = { 0xB0, 0x01 };
	BYTE fakeMessagesDisabled[] = { 0x32, 0xC0 };

	if (!memcmp(fakeMessagesCheck, fakeMessagesEnabled, sizeof fakeMessagesEnabled))
	{
		if (state)
		{
			return true;
		}
		return Utils::Wpm(ErectusProcess::exe + OFFSET_FAKE_MESSAGE, &fakeMessagesDisabled,
			sizeof fakeMessagesDisabled);
	}

	if (!memcmp(fakeMessagesCheck, fakeMessagesDisabled, sizeof fakeMessagesDisabled))
	{
		if (state)
		{
			return Utils::Wpm(ErectusProcess::exe + OFFSET_FAKE_MESSAGE, &fakeMessagesEnabled,
				sizeof fakeMessagesEnabled);
		}
		return true;
	}

	return false;
}

bool ErectusMemory::SendMessageToServer(void* message, const size_t size)
{
	if (!MessagePatcher(allowMessages))
	{
		return false;
	}

	if (!allowMessages)
	{
		return false;
	}

	const auto allocSize = size + sizeof(ExternalFunction);
	const auto allocAddress = Utils::AllocEx(allocSize);
	if (allocAddress == 0) return false;

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
	{
		return false;
	}

	Utils::FreeEx(allocAddress);
	return true;
}

bool ErectusMemory::LootScrap()
{
	if (!MessagePatcher(allowMessages))
	{
		return false;
	}

	if (!allowMessages)
	{
		return false;
	}

	auto localPlayerPtr = GetLocalPlayerPtr(true);
	if (!Utils::Valid(localPlayerPtr))
	{
		ErectusIni::customScrapLooterSettings.scrapAutomaticLootingEnabled = false;
		return false;
	}

	Entity localPlayer{};
	if (!Utils::Rpm(localPlayerPtr, &localPlayer, sizeof localPlayer)) return false;

	auto bufferSize = 0;
	auto* bufferList = GetEntityList(&bufferSize);
	if (bufferList == nullptr) return false;

	for (auto i = 0; i < bufferSize; i++)
	{
		if (!Utils::Valid(bufferList[i])) continue;
		if (bufferList[i] == localPlayerPtr) continue;

		Entity entityData{};
		if (!Utils::Rpm(bufferList[i], &entityData, sizeof entityData)) continue;
		if (!Utils::Valid(entityData.referencePtr)) continue;

		if (entityData.spawnFlag != 0x02)
		{
			continue;
		}

		Reference referenceData{};
		if (!Utils::Rpm(entityData.referencePtr, &referenceData, sizeof referenceData)) continue;
		if (referenceData.formId == 0x00000007) continue;

		auto entityFlag = CUSTOM_ENTRY_DEFAULT;
		DWORD64 entityNamePtr = 0;
		auto enabledDistance = 0;

		GetCustomEntityData(referenceData, &entityFlag, &entityNamePtr, &enabledDistance, true, false);
		if (!(entityFlag & CUSTOM_ENTRY_VALID_SCRAP)) continue;

		auto distance = Utils::GetDistance(entityData.position, localPlayer.position);
		auto normalDistance = static_cast<int>(distance * 0.01f);

		if (normalDistance > ErectusIni::customScrapLooterSettings.scrapLooterDistance)
		{
			continue;
		}

		RequestActivateRefMessage requestActivateRefMessageData{};
		requestActivateRefMessageData.vtable = ErectusProcess::exe + VTABLE_REQUESTACTIVATEREFMSG;
		requestActivateRefMessageData.formId = entityData.formId;
		requestActivateRefMessageData.choice = 0xFF;
		requestActivateRefMessageData.forceActivate = 0;
		SendMessageToServer(&requestActivateRefMessageData, sizeof requestActivateRefMessageData);
	}

	delete[]bufferList;
	bufferList = nullptr;
	return true;
}

bool ErectusMemory::CheckItemLooterSettings()
{
	if (ErectusIni::customItemLooterSettings.itemLooterWeaponsEnabled && ErectusIni::customItemLooterSettings.
		itemLooterWeaponsDistance > 0)
	{
		return true;
	}
	if (ErectusIni::customItemLooterSettings.itemLooterArmorEnabled && ErectusIni::customItemLooterSettings.
		itemLooterArmorDistance > 0)
	{
		return true;
	}
	if (ErectusIni::customItemLooterSettings.itemLooterAmmoEnabled && ErectusIni::customItemLooterSettings.
		itemLooterAmmoDistance > 0)
	{
		return true;
	}
	if (ErectusIni::customItemLooterSettings.itemLooterModsEnabled && ErectusIni::customItemLooterSettings.
		itemLooterModsDistance > 0)
	{
		return true;
	}
	if (ErectusIni::customItemLooterSettings.itemLooterMagazinesEnabled && ErectusIni::customItemLooterSettings.
		itemLooterMagazinesDistance > 0)
	{
		return true;
	}
	if (ErectusIni::customItemLooterSettings.itemLooterBobbleheadsEnabled && ErectusIni::customItemLooterSettings.
		itemLooterBobbleheadsDistance > 0)
	{
		return true;
	}
	if (ErectusIni::customItemLooterSettings.itemLooterAidEnabled && ErectusIni::customItemLooterSettings.
		itemLooterAidDistance > 0)
	{
		return true;
	}
	if (ErectusIni::customItemLooterSettings.itemLooterKnownPlansEnabled && ErectusIni::customItemLooterSettings.
		itemLooterKnownPlansDistance > 0)
	{
		return true;
	}
	if (ErectusIni::customItemLooterSettings.itemLooterUnknownPlansEnabled && ErectusIni::customItemLooterSettings.
		itemLooterUnknownPlansDistance > 0)
	{
		return true;
	}
	if (ErectusIni::customItemLooterSettings.itemLooterMiscEnabled && ErectusIni::customItemLooterSettings.
		itemLooterMiscDistance > 0)
	{
		return true;
	}
	if (ErectusIni::customItemLooterSettings.itemLooterUnlistedEnabled && ErectusIni::customItemLooterSettings.
		itemLooterUnlistedDistance > 0)
	{
		return true;
	}
	if (ErectusIni::customItemLooterSettings.itemLooterListEnabled && ErectusIni::customItemLooterSettings.
		itemLooterListDistance > 0)
	{
		return CheckItemLooterList();
	}
	return false;
}

bool ErectusMemory::CheckOnlyUseItemLooterList()
{
	if (ErectusIni::customItemLooterSettings.itemLooterWeaponsEnabled && ErectusIni::customItemLooterSettings.
		itemLooterWeaponsDistance > 0)
	{
		return false;
	}
	if (ErectusIni::customItemLooterSettings.itemLooterArmorEnabled && ErectusIni::customItemLooterSettings.
		itemLooterArmorDistance > 0)
	{
		return false;
	}
	if (ErectusIni::customItemLooterSettings.itemLooterAmmoEnabled && ErectusIni::customItemLooterSettings.
		itemLooterAmmoDistance > 0)
	{
		return false;
	}
	if (ErectusIni::customItemLooterSettings.itemLooterModsEnabled && ErectusIni::customItemLooterSettings.
		itemLooterModsDistance > 0)
	{
		return false;
	}
	if (ErectusIni::customItemLooterSettings.itemLooterMagazinesEnabled && ErectusIni::customItemLooterSettings.
		itemLooterMagazinesDistance > 0)
	{
		return false;
	}
	if (ErectusIni::customItemLooterSettings.itemLooterBobbleheadsEnabled && ErectusIni::customItemLooterSettings.itemLooterBobbleheadsDistance > 0)
	{
		return false;
	}
	if (ErectusIni::customItemLooterSettings.itemLooterAidEnabled && ErectusIni::customItemLooterSettings.itemLooterAidDistance > 0)
	{
		return false;
	}
	if (ErectusIni::customItemLooterSettings.itemLooterKnownPlansEnabled && ErectusIni::customItemLooterSettings.itemLooterKnownPlansDistance > 0)
	{
		return false;
	}
	if (ErectusIni::customItemLooterSettings.itemLooterUnknownPlansEnabled && ErectusIni::customItemLooterSettings.itemLooterUnknownPlansDistance > 0)
	{
		return false;
	}
	if (ErectusIni::customItemLooterSettings.itemLooterMiscEnabled && ErectusIni::customItemLooterSettings.itemLooterMiscDistance > 0)
	{
		return false;
	}
	if (ErectusIni::customItemLooterSettings.itemLooterUnlistedEnabled && ErectusIni::customItemLooterSettings.itemLooterUnlistedDistance > 0)
	{
		return false;
	}
	if (ErectusIni::customItemLooterSettings.itemLooterListEnabled && ErectusIni::customItemLooterSettings.itemLooterListDistance > 0)
	{
		return CheckItemLooterList();
	}
	return false;
}

bool ErectusMemory::CheckEnabledItem(const DWORD formId, const DWORD64 entityFlag, const int normalDistance)
{
	if (ErectusIni::customItemLooterSettings.itemLooterListEnabled)
	{
		if (CheckFormIdArray(formId, ErectusIni::customItemLooterSettings.itemLooterEnabledList, ErectusIni::customItemLooterSettings.itemLooterFormIdList, 100))
		{
			if (normalDistance <= ErectusIni::customItemLooterSettings.itemLooterListDistance)
				return true;
		}
	}

	if (entityFlag & CUSTOM_ENTRY_WEAPON && normalDistance <= ErectusIni::customItemLooterSettings.itemLooterWeaponsDistance)
		return ErectusIni::customItemLooterSettings.itemLooterWeaponsEnabled;
	if (entityFlag & CUSTOM_ENTRY_ARMOR && normalDistance <= ErectusIni::customItemLooterSettings.itemLooterArmorDistance)
		return ErectusIni::customItemLooterSettings.itemLooterArmorEnabled;
	if (entityFlag & CUSTOM_ENTRY_AMMO && normalDistance <= ErectusIni::customItemLooterSettings.itemLooterAmmoDistance)
		return ErectusIni::customItemLooterSettings.itemLooterAmmoEnabled;
	if (entityFlag & CUSTOM_ENTRY_MOD && normalDistance <= ErectusIni::customItemLooterSettings.itemLooterModsDistance)
		return ErectusIni::customItemLooterSettings.itemLooterModsEnabled;
	if (entityFlag & CUSTOM_ENTRY_MAGAZINE && normalDistance <= ErectusIni::customItemLooterSettings.itemLooterMagazinesDistance)
		return ErectusIni::customItemLooterSettings.itemLooterMagazinesEnabled;
	if (entityFlag & CUSTOM_ENTRY_BOBBLEHEAD && normalDistance <= ErectusIni::customItemLooterSettings.itemLooterBobbleheadsDistance)
		return ErectusIni::customItemLooterSettings.itemLooterBobbleheadsEnabled;
	if (entityFlag & CUSTOM_ENTRY_AID && normalDistance <= ErectusIni::customItemLooterSettings.itemLooterAidDistance)
		return ErectusIni::customItemLooterSettings.itemLooterAidEnabled;
	if (entityFlag & CUSTOM_ENTRY_KNOWN_RECIPE && normalDistance <= ErectusIni::customItemLooterSettings.itemLooterKnownPlansDistance)
		return ErectusIni::customItemLooterSettings.itemLooterKnownPlansEnabled;
	if (entityFlag & CUSTOM_ENTRY_UNKNOWN_RECIPE && normalDistance <= ErectusIni::customItemLooterSettings.itemLooterUnknownPlansDistance)
		return ErectusIni::customItemLooterSettings.itemLooterUnknownPlansEnabled;
	if (entityFlag & CUSTOM_ENTRY_FAILED_RECIPE && normalDistance <= ErectusIni::customItemLooterSettings.itemLooterUnknownPlansDistance)
		return ErectusIni::customItemLooterSettings.itemLooterUnknownPlansEnabled;
	if (entityFlag & CUSTOM_ENTRY_MISC && normalDistance <= ErectusIni::customItemLooterSettings.itemLooterMiscDistance)
		return ErectusIni::customItemLooterSettings.itemLooterMiscEnabled;
	if (ErectusIni::customItemLooterSettings.itemLooterUnlistedEnabled && normalDistance <= ErectusIni::customItemLooterSettings.itemLooterUnlistedDistance)
		return true;

	return false;
}

bool ErectusMemory::LootItems()
{
	if (!MessagePatcher(allowMessages))
	{
		return false;
	}

	if (!allowMessages)
	{
		return false;
	}

	auto localPlayerPtr = GetLocalPlayerPtr(true);
	if (!Utils::Valid(localPlayerPtr))
	{
		ErectusIni::customItemLooterSettings.itemAutomaticLootingEnabled = false;
		return false;
	}

	Entity localPlayer{};
	if (!Utils::Rpm(localPlayerPtr, &localPlayer, sizeof localPlayer)) return false;

	auto bufferSize = 0;
	auto* bufferList = GetEntityList(&bufferSize);
	if (bufferList == nullptr) return false;

	auto onlyUseItemLooterList = CheckOnlyUseItemLooterList();
	auto useItemLooterBlacklist = CheckItemLooterBlacklist();

	for (auto i = 0; i < bufferSize; i++)
	{
		if (!Utils::Valid(bufferList[i])) continue;
		if (bufferList[i] == localPlayerPtr) continue;

		Entity entityData{};
		if (!Utils::Rpm(bufferList[i], &entityData, sizeof entityData)) continue;
		if (!Utils::Valid(entityData.referencePtr)) continue;

		if (entityData.spawnFlag != 0x02)
		{
			continue;
		}

		Reference referenceData{};
		if (!Utils::Rpm(entityData.referencePtr, &referenceData, sizeof referenceData)) continue;

		if (useItemLooterBlacklist)
		{
			if (CheckFormIdArray(referenceData.formId, ErectusIni::customItemLooterSettings.itemLooterBlacklistEnabled,
				ErectusIni::customItemLooterSettings.itemLooterBlacklist, 64))
			{
				continue;
			}
		}

		if (onlyUseItemLooterList)
		{
			if (!CheckFormIdArray(referenceData.formId, ErectusIni::customItemLooterSettings.itemLooterEnabledList,
				ErectusIni::customItemLooterSettings.itemLooterFormIdList, 100))
			{
				continue;
			}
		}

		auto entityFlag = CUSTOM_ENTRY_DEFAULT;
		DWORD64 entityNamePtr = 0;
		auto enabledDistance = 0;

		GetCustomEntityData(referenceData, &entityFlag, &entityNamePtr, &enabledDistance, false, false);
		if (!(entityFlag & CUSTOM_ENTRY_VALID_ITEM)) continue;
		if (entityFlag & CUSTOM_ENTRY_JUNK) continue;

		auto distance = Utils::GetDistance(entityData.position, localPlayer.position);
		auto normalDistance = static_cast<int>(distance * 0.01f);

		if (!onlyUseItemLooterList)
		{
			if (!CheckEnabledItem(referenceData.formId, entityFlag, normalDistance))
			{
				continue;
			}
		}
		else
		{
			if (normalDistance > ErectusIni::customItemLooterSettings.itemLooterListDistance)
			{
				continue;
			}
		}

		RequestActivateRefMessage requestActivateRefMessageData{};
		requestActivateRefMessageData.vtable = ErectusProcess::exe + VTABLE_REQUESTACTIVATEREFMSG;
		requestActivateRefMessageData.formId = entityData.formId;
		requestActivateRefMessageData.choice = 0xFF;
		requestActivateRefMessageData.forceActivate = 0;
		SendMessageToServer(&requestActivateRefMessageData, sizeof requestActivateRefMessageData);
	}

	delete[]bufferList;
	bufferList = nullptr;
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
	if (!Utils::Rpm(ErectusProcess::exe + OFFSET_DATA_HANDLER, &dataHandlerPtr, sizeof dataHandlerPtr)) return false;
	if (!Utils::Valid(dataHandlerPtr)) return false;

	ReferenceList weaponList{};
	if (!Utils::Rpm(dataHandlerPtr + 0x580, &weaponList, sizeof weaponList)) return false;
	if (!Utils::Valid(weaponList.arrayPtr) || !weaponList.arraySize || weaponList.arraySize > 0x7FFF) return false;

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

		if (!Utils::Valid(weaponPtrArray[i])) continue;

		Weapon weaponData{};
		if (!Utils::Rpm(weaponPtrArray[i], &weaponData, sizeof weaponData)) continue;

		oldWeaponList[i].weaponData = new Weapon;
		memcpy(&*oldWeaponList[i].weaponData, &weaponData, sizeof weaponData);

		if (!Utils::Valid(weaponData.aimModelPtr)) continue;

		AimModel aimModelData{};
		if (!Utils::Rpm(weaponData.aimModelPtr, &aimModelData, sizeof aimModelData)) continue;

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
		if ((oldWeaponList[i].weaponData != nullptr) && (oldWeaponList[i].weaponData->formId == formId))
		{
			return i;
		}
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
	if (!Utils::Rpm(ErectusProcess::exe + OFFSET_DATA_HANDLER, &dataHandlerPtr, sizeof dataHandlerPtr)) return false;
	if (!Utils::Valid(dataHandlerPtr)) return false;

	ReferenceList weaponList{};
	if (!Utils::Rpm(dataHandlerPtr + 0x580, &weaponList, sizeof weaponList)) return false;
	if (!Utils::Valid(weaponList.arrayPtr) || !weaponList.arraySize || weaponList.arraySize > 0x7FFF) return false;

	DWORD64 weaponPtr;
	if (!Utils::Rpm(weaponList.arrayPtr + index * sizeof(DWORD64), &weaponPtr, sizeof weaponPtr)) return false;
	if (!Utils::Valid(weaponPtr)) return false;

	Weapon weaponData{};
	if (!Utils::Rpm(weaponPtr, &weaponData, sizeof weaponData)) return false;
	if (oldWeaponList[index].weaponData == nullptr) return false;

	auto currentWeaponIndex = index;
	if (oldWeaponList[currentWeaponIndex].weaponData->formId != weaponData.formId)
	{
		const auto bufferIndex = GetOldWeaponIndex(weaponData.formId);
		if (bufferIndex == -1) return false;
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
	{
		Utils::Wpm(weaponPtr, &weaponData, sizeof weaponData);
	}

	if (!Utils::Valid(weaponData.aimModelPtr))
	{
		return true;
	}

	if (oldWeaponList[currentWeaponIndex].aimModelData == nullptr)
	{
		return false;
	}

	AimModel aimModelData{};
	if (!Utils::Rpm(weaponData.aimModelPtr, &aimModelData, sizeof aimModelData)) return false;

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
		if (memcmp(aimModelData.recoilData, oldWeaponList[currentWeaponIndex].aimModelData->recoilData,
			sizeof AimModel::recoilData) != 0)
		{
			memcpy(aimModelData.recoilData, oldWeaponList[currentWeaponIndex].aimModelData->recoilData,
				sizeof AimModel::recoilData);
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
	{
		return Utils::Wpm(weaponData.aimModelPtr, &aimModelData, sizeof aimModelData);
	}

	return true;
}

bool ErectusMemory::InfiniteAmmo(const bool state)
{
	BYTE infiniteAmmoOn[] = { 0x66, 0xB8, 0xE7, 0x03 };
	BYTE infiniteAmmoOff[] = { 0x8B, 0x44, 0x24, 0x50 };
	BYTE infiniteAmmoCheck[sizeof infiniteAmmoOff];

	if (!Utils::Rpm(ErectusProcess::exe + OFFSET_INFINITE_AMMO, &infiniteAmmoCheck, sizeof infiniteAmmoCheck))
	{
		return false;
	}

	if (state && !memcmp(infiniteAmmoCheck, infiniteAmmoOff, sizeof infiniteAmmoOff))
	{
		return Utils::Wpm(ErectusProcess::exe + OFFSET_INFINITE_AMMO, &infiniteAmmoOn, sizeof infiniteAmmoOn);
	}
	if (!state && !memcmp(infiniteAmmoCheck, infiniteAmmoOn, sizeof infiniteAmmoOn))
	{
		return Utils::Wpm(ErectusProcess::exe + OFFSET_INFINITE_AMMO, &infiniteAmmoOff, sizeof infiniteAmmoOff);
	}
	return false;
}

bool ErectusMemory::LockedTargetValid(bool* isPlayer)
{
	if (!Utils::Valid(targetLockingPtr))
	{
		return false;
	}

	Entity entityData{};
	if (!Utils::Rpm(targetLockingPtr, &entityData, sizeof entityData)) return false;
	if (!Utils::Valid(entityData.referencePtr)) return false;

	Reference referenceData{};
	if (!Utils::Rpm(entityData.referencePtr, &referenceData, sizeof referenceData)) return false;
	const auto result = ErectusMemory::TargetValid(entityData, referenceData);

	if (referenceData.formId == 0x00000007)
	{
		*isPlayer = true;
	}
	else
	{
		*isPlayer = false;
	}

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

	if (!Utils::Rpm(ErectusProcess::exe + OFFSET_REDIRECTION_JMP, &pageJmpCheck, sizeof pageJmpCheck)) return false;

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
		if (!page) return false;
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
		if (!Utils::Wpm(*targetingPage, &targetLockingData, sizeof targetLockingData)) return false;
		memcpy(&pageJmpOn[2], &*targetingPage, sizeof(DWORD64));
	}
	memcpy(&pageJmpOn[2], &*targetingPage, sizeof(DWORD64));

	auto isPlayer = false;
	auto targetValid = LockedTargetValid(&isPlayer);
	if ((!ErectusIni::customTargetSettings.indirectPlayers && isPlayer) || (!ErectusIni::customTargetSettings.indirectNpCs && !isPlayer))
		targetValid = false;

	const auto redirection = Utils::Rpm(ErectusProcess::exe + OFFSET_REDIRECTION, &redirectionCheck,
		sizeof redirectionCheck);

	if (*targetingPageValid && state && targetValid)
	{
		if (redirection && !memcmp(redirectionCheck, redirectionOff, sizeof redirectionOff))
		{
			Utils::Wpm(ErectusProcess::exe + OFFSET_REDIRECTION, &redirectionOn, sizeof redirectionOn);
		}
	}
	else
	{
		if (redirection && !memcmp(redirectionCheck, redirectionOn, sizeof redirectionOn))
		{
			Utils::Wpm(ErectusProcess::exe + OFFSET_REDIRECTION, &redirectionOff, sizeof redirectionOff);
		}
	}

	if (*targetingPageValid && !isExiting && !memcmp(pageJmpCheck, pageJmpOff, sizeof pageJmpOff))
	{
		return Utils::Wpm(ErectusProcess::exe + OFFSET_REDIRECTION_JMP, &pageJmpOn, sizeof pageJmpOn);
	}
	if (isExiting && !memcmp(pageJmpCheck, pageJmpOn, sizeof pageJmpOn))
	{
		return Utils::Wpm(ErectusProcess::exe + OFFSET_REDIRECTION_JMP, &pageJmpOff, sizeof pageJmpOff);
	}
	return true;
}

bool ErectusMemory::MovePlayer()
{
	const auto cameraPtr = GetCameraPtr();
	if (!Utils::Valid(cameraPtr)) return false;

	DWORD64 bhkCharProxyControllerPtr;
	if (!Utils::Rpm(ErectusProcess::exe + OFFSET_CHAR_CONTROLLER, &bhkCharProxyControllerPtr,
		sizeof bhkCharProxyControllerPtr)) return false;
	if (!Utils::Valid(bhkCharProxyControllerPtr)) return false;

	BhkCharProxyController bhkCharProxyControllerData{};
	if (!Utils::Rpm(bhkCharProxyControllerPtr, &bhkCharProxyControllerData, sizeof bhkCharProxyControllerData)) return
		false;
	if (!Utils::Valid(bhkCharProxyControllerData.hknpBsCharacterProxyPtr)) return false;

	HknpBsCharacterProxy hknpBsCharacterProxyData{};
	if (!Utils::Rpm(bhkCharProxyControllerData.hknpBsCharacterProxyPtr, &hknpBsCharacterProxyData,
		sizeof hknpBsCharacterProxyData)) return false;

	Camera cameraData{};
	if (!Utils::Rpm(cameraPtr, &cameraData, sizeof cameraData)) return false;

	float velocityA[4];
	memset(velocityA, 0x00, sizeof velocityA);

	float velocityB[4];
	memset(velocityB, 0x00, sizeof velocityB);

	auto speed = ErectusIni::customLocalPlayerSettings.noclipSpeed;
	if (GetAsyncKeyState(VK_SHIFT))
	{
		speed *= 1.5f;
	}

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
	{
		return Utils::Wpm(bhkCharProxyControllerData.hknpBsCharacterProxyPtr, &hknpBsCharacterProxyData,
			sizeof hknpBsCharacterProxyData);
	}

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
		{
			Utils::Wpm(ErectusProcess::exe + OFFSET_NOCLIP_A, &noclipOnA, sizeof noclipOnA);
		}

		if (noclipB && !memcmp(noclipCheckB, noclipOffB, sizeof noclipOffB))
		{
			Utils::Wpm(ErectusProcess::exe + OFFSET_NOCLIP_B, &noclipOnB, sizeof noclipOnB);
		}

		if (noclipC && !memcmp(noclipCheckC, noclipOffC, sizeof noclipOffC))
		{
			Utils::Wpm(ErectusProcess::exe + OFFSET_NOCLIP_C, &noclipOnC, sizeof noclipOnC);
		}

		if (noclipD && !memcmp(noclipCheckD, noclipOffD, sizeof noclipOffD))
		{
			Utils::Wpm(ErectusProcess::exe + OFFSET_NOCLIP_D, &noclipOnD, sizeof noclipOnD);
		}

		MovePlayer();
	}
	else
	{
		if (noclipA && !memcmp(noclipCheckA, noclipOnA, sizeof noclipOnA))
		{
			Utils::Wpm(ErectusProcess::exe + OFFSET_NOCLIP_A, &noclipOffA, sizeof noclipOffA);
		}

		if (noclipB && !memcmp(noclipCheckB, noclipOnB, sizeof noclipOnB))
		{
			Utils::Wpm(ErectusProcess::exe + OFFSET_NOCLIP_B, &noclipOffB, sizeof noclipOffB);
		}

		if (noclipC && !memcmp(noclipCheckC, noclipOnC, sizeof noclipOnC))
		{
			Utils::Wpm(ErectusProcess::exe + OFFSET_NOCLIP_C, &noclipOffC, sizeof noclipOffC);
		}

		if (noclipD && !memcmp(noclipCheckD, noclipOnD, sizeof noclipOnD))
		{
			Utils::Wpm(ErectusProcess::exe + OFFSET_NOCLIP_D, &noclipOffD, sizeof noclipOffD);
		}
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
	if (!Utils::Valid(localPlayerPtr)) return false;

	Entity localPlayer{};
	if (!Utils::Rpm(localPlayerPtr, &localPlayer, sizeof localPlayer)) return false;
	if (!Utils::Valid(localPlayer.vtable0050)) return false;

	DWORD64 actorValueFunction;
	if (!Utils::Rpm(localPlayer.vtable0050 + 0x48, &actorValueFunction, sizeof actorValueFunction)) return false;
	if (!Utils::Valid(actorValueFunction)) return false;

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
		{
			Utils::FreeEx(actorValueFunction);
		}
	}

	if (state)
	{
		if (*actorValuePageValid)
		{
			ActorValueHook actorValueHookCheck;
			if (!Utils::Rpm(*actorValuePage, &actorValueHookCheck, sizeof actorValueHookCheck)) return false;
			if (memcmp(&actorValueHookData, &actorValueHookCheck, sizeof actorValueHookCheck) != 0)
			{
				return Utils::Wpm(*actorValuePage, &actorValueHookData, sizeof actorValueHookData);
			}
		}
		else
		{
			if (!Utils::Wpm(*actorValuePage, &actorValueHookData, sizeof actorValueHookData)) return false;
			if (!Utils::VtableSwap(localPlayer.vtable0050 + 0x48, *actorValuePage)) return false;
			*actorValuePageValid = true;
		}
	}
	else
	{
		if (actorValueFunction != ErectusProcess::exe + OFFSET_ACTOR_VALUE)
		{
			if (Utils::VtableSwap(localPlayer.vtable0050 + 0x48, ErectusProcess::exe + OFFSET_ACTOR_VALUE))
			{
				Utils::FreeEx(actorValueFunction);
			}
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
	if (!Utils::Valid(actorValuePtr)) return false;

	ActorValueInformation actorValueData{};
	if (!Utils::Rpm(actorValuePtr, &actorValueData, sizeof actorValueData)) return false;

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
	{
		return false;
	}

	const auto originalFunction = ErectusProcess::exe + OFFSET_OPK + sizeof opkOff;
	memcpy(&opkOn[2], &*opkPage, sizeof(DWORD64));

	DWORD64 pageCheck;
	memcpy(&pageCheck, &opkCheck[2], sizeof(DWORD64));

	if (Utils::Valid(pageCheck) && pageCheck != *opkPage)
	{
		Opk buffer;
		if (!Utils::Rpm(pageCheck, &buffer, sizeof buffer)) return false;
		if (buffer.originalFunction != originalFunction) return false;
		if (!Utils::Wpm(ErectusProcess::exe + OFFSET_OPK, &opkOff, sizeof opkOff)) return false;
		Utils::FreeEx(pageCheck);
	}

	if (state)
	{
		if (*opkPageValid)
		{
			return true;
		}
		Opk opkData;
		opkData.opkPlayers = 0;
		opkData.opkNpcs = 0;
		opkData.originalFunction = originalFunction;
		memset(opkData.opkPlayerPosition, 0x00, sizeof opkData.opkPlayerPosition);
		memset(opkData.opkNpcPosition, 0x00, sizeof opkData.opkNpcPosition);

		if (!Utils::Wpm(*opkPage, &opkData, sizeof opkData)) return false;
		if (!Utils::Wpm(ErectusProcess::exe + OFFSET_OPK, &opkOn, sizeof opkOn)) return false;
		*opkPageValid = true;
	}
	else
	{
		if (pageCheck == *opkPage)
		{
			Utils::Wpm(ErectusProcess::exe + OFFSET_OPK, &opkOff, sizeof opkOff);
		}

		if ((*opkPage) && (Utils::FreeEx(*opkPage)))
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
	if (!Utils::Rpm(opkPage, &opkData, sizeof opkData)) return false;

	const auto cameraPtr = GetCameraPtr();
	if (!Utils::Valid(cameraPtr)) return false;

	Camera cameraData{};
	if (!Utils::Rpm(cameraPtr, &cameraData, sizeof cameraData)) return false;

	float editedOrigin[3];
	editedOrigin[0] = cameraData.origin[0] / 70.0f;
	editedOrigin[1] = cameraData.origin[1] / 70.0f;
	editedOrigin[2] = cameraData.origin[2] / 70.0f;

	if (players)
	{
		const auto distance = Utils::GetDistance(opkData.opkPlayerPosition, editedOrigin);
		if (distance > 20.0f) return false;
	}
	else
	{
		const auto distance = Utils::GetDistance(opkData.opkNpcPosition, editedOrigin);
		if (distance > 20.0f) return false;
	}

	return true;
}

bool ErectusMemory::SetOpkData(const DWORD64 opkPage, const bool players, const bool state)
{
	Opk opkData;
	if (!Utils::Rpm(opkPage, &opkData, sizeof opkData)) return false;

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
		{
			Utils::Wpm(opkPage, &opkData, sizeof opkData);
		}

		return true;
	}

	if (CheckOpkDistance(opkPage, players))
	{
		return true;
	}

	const auto cameraPtr = GetCameraPtr();
	if (!Utils::Valid(cameraPtr)) return false;

	Camera cameraData{};
	if (!Utils::Rpm(cameraPtr, &cameraData, sizeof cameraData)) return false;

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
	if (!Utils::Rpm(ErectusProcess::exe + OFFSET_ENTITY_LIST, &entityListTypePtr, sizeof entityListTypePtr)) return
		false;
	if (!Utils::Valid(entityListTypePtr)) return false;

	EntityListType entityListTypeData{};
	if (!Utils::Rpm(entityListTypePtr, &entityListTypeData, sizeof entityListTypeData)) return false;
	if (!Utils::Valid(entityListTypeData.interiorListPtr)) return false;
	if (!Utils::Valid(entityListTypeData.interiorListCheck)) return false;
	if (!Utils::Valid(entityListTypeData.exteriorListPtr)) return false;
	if (!Utils::Valid(entityListTypeData.exteriorListCheck)) return false;

	if (entityListTypeData.interiorListPtr != entityListTypeData.interiorListCheck)
	{
		return true;
	}
	return false;
}

int ErectusMemory::RenderLocalPlayerData()
{
	const auto localPlayerPtr = GetLocalPlayerPtr(false);
	if (!Utils::Valid(localPlayerPtr)) return 0;

	Entity localPlayer{};
	if (!Utils::Rpm(localPlayerPtr, &localPlayer, sizeof localPlayer)) return 0;

	DWORD cellFormId = 0;
	if (Utils::Valid(localPlayer.cellPtr))
	{
		if (!Utils::Rpm(localPlayer.cellPtr + 0x20, &cellFormId, sizeof cellFormId))
		{
			cellFormId = 0;
		}
	}

	auto playerHealth = -1.0f;
	ActorSnapshotComponent actorSnapshotComponentData{};
	if (GetActorSnapshotComponentData(localPlayer, &actorSnapshotComponentData))
	{
		playerHealth = actorSnapshotComponentData.maxHealth + actorSnapshotComponentData.modifiedHealth +
			actorSnapshotComponentData.lostHealth;
	}

	char entityFormIdText[256];
	sprintf_s(entityFormIdText, "Entity FormId: %08lX", localPlayer.formId);

	char stashFormIdText[256];
	sprintf_s(stashFormIdText, "STASH FormId: %08lX", localPlayer.formId0C24);

	char cellFormIdText[256];
	sprintf_s(cellFormIdText, "Cell FormId: %08lX", cellFormId);

	char positionTextX[256];
	sprintf_s(positionTextX, "X: %f", localPlayer.position[0]);

	char positionTextY[256];
	sprintf_s(positionTextY, "Y: %f", localPlayer.position[1]);

	char positionTextZ[256];
	sprintf_s(positionTextZ, "Z: %f", localPlayer.position[2]);

	char yawTest[256];
	sprintf_s(yawTest, "Yaw: %f", localPlayer.yaw);

	char pitchText[256];
	sprintf_s(pitchText, "Pitch: %f", localPlayer.pitch);

	char healthText[256];
	sprintf_s(healthText, "Health: %f", playerHealth);

	char* textArray[]
	{
		entityFormIdText,
		stashFormIdText,
		cellFormIdText,
		positionTextX,
		positionTextY,
		positionTextZ,
		yawTest,
		pitchText,
		healthText,
	};

	float textColor[3] = { 1.0f, 1.0f, 1.0f };
	const auto textAlpha = 1.0f;

	auto index = 0;

	while (index < sizeof textArray / sizeof(char*))
	{
		float textPosition[2] = { 0.0f, static_cast<float>(index) * 16.0f };
		Renderer::DrawText(textArray[index], true, false, textPosition, textColor, textAlpha);
		index++;
	}

	return index;
}

void ErectusMemory::RenderData()
{
	auto spacing = 0;
	if (ErectusIni::customUtilitySettings.debugPlayer)
	{
		spacing = RenderLocalPlayerData();
	}

	float enabledTextColor[3] = { 0.0f, 1.0f, 0.0f };
	float disabledTextColor[3] = { 1.0f, 0.0f, 0.0f };
	const auto textAlpha = 1.0f;

	char scrapLooterText[256];
	sprintf_s(scrapLooterText, "Scrap Looter (Automatic): %d",
		static_cast<int>(ErectusIni::customScrapLooterSettings.scrapAutomaticLootingEnabled));

	char itemLooterText[256];
	sprintf_s(itemLooterText, "Item Looter (Automatic): %d",
		static_cast<int>(ErectusIni::customItemLooterSettings.itemAutomaticLootingEnabled));

	char npcLooterText[256];
	sprintf_s(npcLooterText, "NPC Looter (76m Distance Limit): %d",
		static_cast<int>(ErectusIni::npcLooterSettings.entityLooterEnabled));

	char containerLooterText[256];
	sprintf_s(containerLooterText, "Container Looter (6m Distance Limit): %d",
		static_cast<int>(ErectusIni::containerLooterSettings.entityLooterEnabled));

	char harvesterText[256];
	sprintf_s(harvesterText, "Flora Harvester (6m Distance Limit): %d",
		static_cast<int>(ErectusIni::customHarvesterSettings.harvesterEnabled));

	char positionSpoofingText[256];
	sprintf_s(positionSpoofingText, "Position Spoofing (Active): %d (Height: %d)",
		static_cast<int>(ErectusThread::positionSpoofingToggle), ErectusIni::customLocalPlayerSettings.positionSpoofingHeight);

	char drawAlphaCodeText[256];
	sprintf_s(drawAlphaCodeText, "%d %d %d %d %d %d %d %d - Alpha", ErectusImGui::alphaCode[0],
		ErectusImGui::alphaCode[1], ErectusImGui::alphaCode[2], ErectusImGui::alphaCode[3],
		ErectusImGui::alphaCode[4], ErectusImGui::alphaCode[5], ErectusImGui::alphaCode[6],
		ErectusImGui::alphaCode[7]);

	char drawBravoCodeText[256];
	sprintf_s(drawBravoCodeText, "%d %d %d %d %d %d %d %d - Bravo", ErectusImGui::bravoCode[0],
		ErectusImGui::bravoCode[1], ErectusImGui::bravoCode[2], ErectusImGui::bravoCode[3],
		ErectusImGui::bravoCode[4], ErectusImGui::bravoCode[5], ErectusImGui::bravoCode[6],
		ErectusImGui::bravoCode[7]);

	char drawCharlieCodeText[256];
	sprintf_s(drawCharlieCodeText, "%d %d %d %d %d %d %d %d - Charlie", ErectusImGui::charlieCode[0],
		ErectusImGui::charlieCode[1], ErectusImGui::charlieCode[2], ErectusImGui::charlieCode[3],
		ErectusImGui::charlieCode[4], ErectusImGui::charlieCode[5], ErectusImGui::charlieCode[6],
		ErectusImGui::charlieCode[7]);

	const auto positionSpoofingState = InsideInteriorCell()
		? false
		: ErectusIni::customLocalPlayerSettings.positionSpoofingEnabled;

	auto alphaCodeState = false;
	for (auto i : ErectusImGui::alphaCode)
	{
		if (i)
		{
			alphaCodeState = true;
			break;
		}
	}

	auto bravoCodeState = false;
	for (auto i : ErectusImGui::bravoCode)
	{
		if (i)
		{
			bravoCodeState = true;
			break;
		}
	}

	auto charlieCodeState = false;
	for (auto i : ErectusImGui::charlieCode)
	{
		if (i)
		{
			charlieCodeState = true;
			break;
		}
	}

	char* textArray[]
	{
		scrapLooterText,
		itemLooterText,
		npcLooterText,
		containerLooterText,
		harvesterText,
		positionSpoofingText,
		drawAlphaCodeText,
		drawBravoCodeText,
		drawCharlieCodeText,
	};

	bool stateArray[]
	{
		ErectusIni::customScrapLooterSettings.scrapAutomaticLootingEnabled,
		ErectusIni::customItemLooterSettings.itemAutomaticLootingEnabled,
		ErectusIni::npcLooterSettings.entityLooterEnabled,
		ErectusIni::containerLooterSettings.entityLooterEnabled,
		ErectusIni::customHarvesterSettings.harvesterEnabled,
		positionSpoofingState,
		alphaCodeState,
		bravoCodeState,
		charlieCodeState,
	};

	bool enabledArray[]
	{
		ErectusIni::customScrapLooterSettings.scrapAutomaticStatus,
		ErectusIni::customItemLooterSettings.itemAutomaticStatus,
		ErectusIni::npcLooterSettings.entityLooterStatusEnabled,
		ErectusIni::containerLooterSettings.entityLooterStatusEnabled,
		ErectusIni::customHarvesterSettings.harvesterStatusEnabled,
		ErectusIni::customLocalPlayerSettings.drawPositionSpoofingEnabled,
		ErectusIni::customNukeCodeSettings.drawCodeAlpha,
		ErectusIni::customNukeCodeSettings.drawCodeBravo,
		ErectusIni::customNukeCodeSettings.drawCodeCharlie,
	};

	for (auto i = 0; i < sizeof textArray / sizeof(char*); i++)
	{
		if (!enabledArray[i]) continue;
		float* textColor;
		if (stateArray[i])
			textColor = enabledTextColor;
		else
			textColor = disabledTextColor;
		float textPosition[2] = { 0.0f, static_cast<float>(spacing) * 16.0f };
		Renderer::DrawText(textArray[i], true, false, textPosition, textColor, textAlpha);
		spacing++;
	}
}

bool ErectusMemory::ReferenceSwap(DWORD* sourceFormId, DWORD* destinationFormId)
{
	if (*sourceFormId == *destinationFormId)
	{
		return true;
	}

	auto sourcePointer = GetPtr(*sourceFormId);
	if (!Utils::Valid(sourcePointer))
	{
		*sourceFormId = 0x00000000;
		return false;
	}

	const auto destinationAddress = GetAddress(*destinationFormId);
	if (!Utils::Valid(destinationAddress))
	{
		*destinationFormId = 0x00000000;
		return false;
	}

	return Utils::Wpm(destinationAddress, &sourcePointer, sizeof sourcePointer);
}

bool ErectusMemory::CheckItemTransferList()
{
	for (auto i = 0; i < 32; i++)
	{
		if (ErectusIni::customTransferSettings.whitelist[i])
		{
			if (ErectusIni::customTransferSettings.whitelisted[i])
			{
				return true;
			}
		}
	}

	return false;
}

bool ErectusMemory::TransferItems(DWORD sourceFormId, DWORD destinationFormId)
{
	auto sourcePtr = GetPtr(sourceFormId);
	if (!Utils::Valid(sourcePtr)) return false;

	auto destinationPtr = GetPtr(destinationFormId);
	if (!Utils::Valid(destinationPtr)) return false;

	Entity entityData{};
	if (!Utils::Rpm(sourcePtr, &entityData, sizeof entityData)) return false;
	if (!Utils::Valid(entityData.inventoryPtr)) return false;

	Inventory inventoryData{};
	if (!Utils::Rpm(entityData.inventoryPtr, &inventoryData, sizeof inventoryData)) return false;
	if (!Utils::Valid(inventoryData.itemArrayPtr) || inventoryData.itemArrayEnd < inventoryData.itemArrayPtr) return
		false;

	auto itemArraySize = (inventoryData.itemArrayEnd - inventoryData.itemArrayPtr) / sizeof(Item);
	if (!itemArraySize || itemArraySize > 0x7FFF) return false;

	auto* itemData = new Item[itemArraySize];
	if (!Utils::Rpm(inventoryData.itemArrayPtr, &*itemData, itemArraySize * sizeof(Item)))
	{
		delete[]itemData;
		itemData = nullptr;
		return false;
	}

	for (DWORD64 i = 0; i < itemArraySize; i++)
	{
		if (!Utils::Valid(itemData[i].referencePtr)) continue;
		if (!Utils::Valid(itemData[i].displayPtr) || itemData[i].iterations < itemData[i].displayPtr) continue;

		if (ErectusIni::customTransferSettings.useWhitelist || ErectusIni::customTransferSettings.useBlacklist)
		{
			Reference referenceData{};
			if (!Utils::Rpm(itemData[i].referencePtr, &referenceData, sizeof referenceData)) continue;

			if (ErectusIni::customTransferSettings.useWhitelist)
			{
				if (!CheckFormIdArray(referenceData.formId, ErectusIni::customTransferSettings.whitelisted,
					ErectusIni::customTransferSettings.whitelist, 32))
				{
					continue;
				}
			}

			if (ErectusIni::customTransferSettings.useBlacklist)
			{
				if (CheckFormIdArray(referenceData.formId, ErectusIni::customTransferSettings.blacklisted,
					ErectusIni::customTransferSettings.blacklist, 32))
				{
					continue;
				}
			}
		}

		auto iterations = (itemData[i].iterations - itemData[i].displayPtr) / sizeof(ItemCount);
		if (!iterations || iterations > 0x7FFF) continue;

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
		{
			continue;
		}

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
	if (!Utils::Valid(localPlayerPtr)) return false;

	Entity localPlayer{};
	if (!Utils::Rpm(localPlayerPtr, &localPlayer, sizeof localPlayer)) return false;
	if (!Utils::Valid(localPlayer.cellPtr)) return false;

	DWORD cellFormId;
	if (!Utils::Rpm(localPlayer.cellPtr + 0x20, &cellFormId, sizeof cellFormId)) return false;

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
	if (!Utils::Valid(cellPtr)) return false;

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
	if (!Utils::Valid(localPlayerPtr)) return 0;

	Entity localPlayer{};
	if (!Utils::Rpm(localPlayerPtr, &localPlayer, sizeof localPlayer)) return 0;

	return localPlayer.formId;
}

DWORD ErectusMemory::GetStashFormId()
{
	const auto localPlayerPtr = GetLocalPlayerPtr(true);
	if (!Utils::Valid(localPlayerPtr)) return 0;

	Entity localPlayer{};
	if (!Utils::Rpm(localPlayerPtr, &localPlayer, sizeof localPlayer)) return 0;

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
	{
		return false;
	}

	DWORD64 pageCheck;
	memcpy(&pageCheck, &freezeApCheck[0x6], sizeof(DWORD64));

	if (Utils::Valid(pageCheck) && pageCheck != *freezeApPage)
	{
		for (auto i = 0; i < 0x6; i++) if (freezeApCheck[i] != freezeApOn[i]) return false;
		if (!Utils::Wpm(ErectusProcess::exe + OFFSET_AV_REGEN, &freezeApOff, sizeof freezeApOff)) return false;
		Utils::FreeEx(pageCheck);
	}

	if (state)
	{
		FreezeAp freezeApData;
		freezeApData.freezeApEnabled = ErectusIni::customLocalPlayerSettings.freezeApEnabled;

		if (*freezeApPageValid)
		{
			FreezeAp freezeApPageCheck;
			if (!Utils::Rpm(*freezeApPage, &freezeApPageCheck, sizeof freezeApPageCheck)) return false;
			if (!memcmp(&freezeApData, &freezeApPageCheck, sizeof freezeApPageCheck)) return true;
			return Utils::Wpm(*freezeApPage, &freezeApData, sizeof freezeApData);
		}
		if (!Utils::Wpm(*freezeApPage, &freezeApData, sizeof freezeApData)) return false;
		memcpy(&freezeApOn[0x6], &*freezeApPage, sizeof(DWORD64));
		if (!Utils::Wpm(ErectusProcess::exe + OFFSET_AV_REGEN, &freezeApOn, sizeof freezeApOn)) return false;
		*freezeApPageValid = true;
	}
	else
	{
		if (pageCheck == *freezeApPage)
		{
			Utils::Wpm(ErectusProcess::exe + OFFSET_AV_REGEN, &freezeApOff, sizeof freezeApOff);
		}

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

	if (!Utils::Rpm(ErectusProcess::exe + OFFSET_SERVER_POSITION, &positionSpoofingCheck,
		sizeof positionSpoofingCheck))
	{
		return false;
	}

	if (!state)
	{
		positionSpoofingCheck[1] = 0x00;
		positionSpoofingCheck[2] = 0x00;
		positionSpoofingCheck[3] = 0x00;
		positionSpoofingCheck[4] = 0x00;

		if (!memcmp(positionSpoofingCheck, positionSpoofingOn, sizeof positionSpoofingOn))
		{
			return Utils::Wpm(ErectusProcess::exe + OFFSET_SERVER_POSITION, &positionSpoofingOff,
				sizeof positionSpoofingOff);
		}
		return true;
	}

	int spoofingHeightCheck;
	memcpy(&spoofingHeightCheck, &positionSpoofingCheck[1], sizeof spoofingHeightCheck);
	memcpy(&positionSpoofingOn[1], &ErectusIni::customLocalPlayerSettings.positionSpoofingHeight,
		sizeof ErectusIni::customLocalPlayerSettings.positionSpoofingHeight);

	if (!memcmp(positionSpoofingCheck, positionSpoofingOn, sizeof positionSpoofingOn))
	{
		return true;
	}
	if (!memcmp(positionSpoofingCheck, positionSpoofingOff, sizeof positionSpoofingOff))
	{
		return Utils::Wpm(ErectusProcess::exe + OFFSET_SERVER_POSITION, &positionSpoofingOn,
			sizeof positionSpoofingOn);
	}
	if (spoofingHeightCheck != ErectusIni::customLocalPlayerSettings.positionSpoofingHeight)
	{
		if (positionSpoofingCheck[0] != 0xBA || spoofingHeightCheck < -524287 || spoofingHeightCheck > 524287)
		{
			return false;
		}
		return Utils::Wpm(ErectusProcess::exe + OFFSET_SERVER_POSITION, &positionSpoofingOn,
			sizeof positionSpoofingOn);
	}
	return false;
}

DWORD ErectusMemory::GetEntityId(Entity entityData)
{
	if (!(entityData.idValue[0] & 1)) return 0;

	DWORD v1;
	memcpy(&v1, entityData.idValue, sizeof v1);

	const auto v2 = v1 >> 1;
	const auto v3 = v2 + v2;

	DWORD v4;
	if (!Utils::Rpm(ErectusProcess::exe + OFFSET_ENTITY_ID + v3 * 0x8, &v4, sizeof v4)) return 0;

	const auto v5 = v4 & 0x7FF80000;
	const auto v6 = v5 | v2;

	return v6;
}

bool ErectusMemory::SendHitsToServer(Hits* hitsData, const size_t hitsDataSize)
{
	const auto allocSize = sizeof(ExternalFunction) + sizeof(RequestHitsOnActors) + hitsDataSize;
	const auto allocAddress = Utils::AllocEx(allocSize);
	if (allocAddress == 0) return false;

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
	{
		return false;
	}

	Utils::FreeEx(allocAddress);
	return true;
}

bool ErectusMemory::SendDamage(const DWORD weaponId, BYTE* shotsHit, BYTE* shotsFired, const BYTE count)
{
	if (!MessagePatcher(allowMessages))
	{
		return false;
	}

	if (!allowMessages)
	{
		return false;
	}

	if (!weaponId)
	{
		return false;
	}

	auto isPlayer = false;
	LockedTargetValid(&isPlayer);
	if ((!ErectusIni::customTargetSettings.directPlayers && isPlayer) || (!ErectusIni::customTargetSettings.directNpCs && !isPlayer))
		return false;

	const auto localPlayerPtr = GetLocalPlayerPtr(true);
	if (!Utils::Valid(localPlayerPtr)) return false;

	Entity localPlayer{};
	if (!Utils::Rpm(localPlayerPtr, &localPlayer, sizeof localPlayer)) return false;

	const auto localPlayerId = GetEntityId(localPlayer);
	if (!localPlayerId) return false;

	Entity target{};
	if (!Utils::Rpm(targetLockingPtr, &target, sizeof target)) return false;

	const auto targetId = GetEntityId(target);
	if (!targetId) return false;

	auto* hitsData = new Hits[count];
	memset(hitsData, 0x00, count * sizeof(Hits));

	if (*shotsHit == 0 || *shotsHit == 255)
	{
		*shotsHit = 1;
	}

	if (*shotsFired == 255)
	{
		*shotsFired = 0;
	}

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
				{
					*shotsHit = 1;
				}
				else
				{
					*shotsHit += 1;
				}
			}
			else
			{
				*shotsHit = 1;
			}
		}
		else
		{
			*shotsHit = 1;
		}

		for (auto c = 0; c < Utils::GetRangedInt(1, 6); c++)
		{
			if (*shotsFired == 255)
			{
				*shotsFired = 0;
			}
			else
			{
				*shotsFired += 1;
			}
		}
	}

	const auto result = SendHitsToServer(hitsData, count * sizeof(Hits));

	delete[]hitsData;

	return result;
}

DWORD64 ErectusMemory::GetNukeCodePtr(const DWORD formId)
{
	ReferenceList questTextList{};
	if (!Utils::Rpm(ErectusProcess::exe + OFFSET_NUKE_CODE, &questTextList, sizeof questTextList)) return 0;
	if (!Utils::Valid(questTextList.arrayPtr) || !questTextList.arraySize || questTextList.arraySize > 0x7FFF) return 0;

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
		if (!Utils::Valid(questTextArray[i])) continue;

		BgsQuestText bgsQuestTextData{};
		if (!Utils::Rpm(questTextArray[i], &bgsQuestTextData, sizeof bgsQuestTextData)) continue;
		if (!Utils::Valid(bgsQuestTextData.formIdPtr) || !Utils::Valid(bgsQuestTextData.codePtr)) continue;

		DWORD formIdCheck;
		if (!Utils::Rpm(bgsQuestTextData.formIdPtr + 0x4, &formIdCheck, sizeof formIdCheck)) continue;
		if (formIdCheck != formId) continue;

		nukeCodePtr = bgsQuestTextData.codePtr;
		break;
	}

	delete[]questTextArray;
	questTextArray = nullptr;
	return nukeCodePtr;
}

bool ErectusMemory::GetNukeCode(const DWORD formId, int* nukeCode)
{
	const auto nukeCodePtr = GetNukeCodePtr(formId);
	if (!nukeCodePtr) return false;

	float nukeCodeArray[16];
	if (!Utils::Rpm(nukeCodePtr, &nukeCodeArray, sizeof nukeCodeArray)) return false;

	for (auto i = 0; i < 8; i++)
	{
		if (nukeCodeArray[i * 2 + 1] < 0.0f || nukeCodeArray[i * 2 + 1] > 9.0f)
		{
			memset(nukeCode, 0x00, sizeof(int) * 8);
			return false;
		}
		nukeCode[i] = static_cast<int>(nukeCodeArray[i * 2 + 1]);
	}

	return true;
}

DWORD ErectusMemory::GetFavoritedWeaponId(const BYTE index)
{
	const auto localPlayerPtr = GetLocalPlayerPtr(true);
	if (!Utils::Valid(localPlayerPtr)) return 0;

	Entity localPlayer{};
	if (!Utils::Rpm(localPlayerPtr, &localPlayer, sizeof localPlayer)) return 0;
	if (!Utils::Valid(localPlayer.inventoryPtr)) return 0;

	Inventory inventoryData{};
	if (!Utils::Rpm(localPlayer.inventoryPtr, &inventoryData, sizeof inventoryData)) return 0;
	if (!Utils::Valid(inventoryData.itemArrayPtr) || inventoryData.itemArrayEnd < inventoryData.itemArrayPtr) return 0;

	const auto itemArraySize = (inventoryData.itemArrayEnd - inventoryData.itemArrayPtr) / sizeof(Item);
	if (!itemArraySize || itemArraySize > 0x7FFF) return 0;

	auto* itemData = new Item[itemArraySize];
	if (!Utils::Rpm(inventoryData.itemArrayPtr, &*itemData, itemArraySize * sizeof(Item)))
	{
		delete[]itemData;
		itemData = nullptr;
		return 0;
	}

	for (DWORD64 i = 0; i < itemArraySize; i++)
	{
		if (!Utils::Valid(itemData[i].referencePtr)) continue;
		if (itemData[i].favoriteIndex != index) continue;

		Reference referenceData{};
		if (!Utils::Rpm(itemData[i].referencePtr, &referenceData, sizeof referenceData)) break;
		if (referenceData.vtable != ErectusProcess::exe + VTABLE_TESOBJECTWEAP) break;

		const auto weaponId = itemData[i].itemId;
		delete[]itemData;
		itemData = nullptr;
		return weaponId;
	}

	delete[]itemData;
	itemData = nullptr;
	return 0;
}

char ErectusMemory::GetFavoriteSlot(const BYTE favoriteIndex)
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
	if (!Utils::Rpm(vtable - 0x8, &buffer, sizeof buffer)) return 0;
	if (!Utils::Valid(buffer)) return 0;

	DWORD offset;
	if (!Utils::Rpm(buffer + 0xC, &offset, sizeof offset)) return 0;
	if (offset == 0 || offset > 0x7FFFFFFF) return 0;

	return ErectusProcess::exe + offset + 0x10;
}

char* ErectusMemory::GetInstancedItemName(const DWORD64 displayPtr)
{
	if (!Utils::Valid(displayPtr)) return nullptr;

	DWORD64 instancedArrayPtr;
	if (!Utils::Rpm(displayPtr, &instancedArrayPtr, sizeof instancedArrayPtr)) return nullptr;
	if (!Utils::Valid(instancedArrayPtr)) return nullptr;

	ItemInstancedArray itemInstancedArrayData{};
	if (!Utils::Rpm(instancedArrayPtr, &itemInstancedArrayData, sizeof itemInstancedArrayData)) return nullptr;
	if (!Utils::Valid(itemInstancedArrayData.arrayPtr) || itemInstancedArrayData.arrayEnd < itemInstancedArrayData.
		arrayPtr) return nullptr;

	const auto instancedArraySize = (itemInstancedArrayData.arrayEnd - itemInstancedArrayData.arrayPtr) / sizeof(DWORD64);
	if (!instancedArraySize || instancedArraySize > 0x7FFF) return nullptr;

	auto* instancedArray = new DWORD64[instancedArraySize];
	if (!Utils::Rpm(itemInstancedArrayData.arrayPtr, &*instancedArray, instancedArraySize * sizeof(DWORD64)))
	{
		delete[]instancedArray;
		instancedArray = nullptr;
		return nullptr;
	}

	for (DWORD64 i = 0; i < instancedArraySize; i++)
	{
		if (!Utils::Valid(instancedArray[i])) continue;

		ExtraTextDisplayData extraTextDisplayDataData{};
		if (!Utils::Rpm(instancedArray[i], &extraTextDisplayDataData, sizeof extraTextDisplayDataData)) continue;

		const auto rttiNamePtr = RttiGetNamePtr(extraTextDisplayDataData.vtable);
		if (!rttiNamePtr) continue;

		char rttiNameCheck[sizeof".?AVExtraTextDisplayData@@"];
		if (!Utils::Rpm(rttiNamePtr, &rttiNameCheck, sizeof rttiNameCheck)) continue;
		if (strcmp(rttiNameCheck, ".?AVExtraTextDisplayData@@") != 0) continue;

		delete[]instancedArray;
		instancedArray = nullptr;
		return GetEntityName(extraTextDisplayDataData.instancedNamePtr);
	}

	delete[]instancedArray;
	instancedArray = nullptr;
	return nullptr;
}

char** ErectusMemory::GetFavoritedWeapons()
{
	const auto localPlayerPtr = GetLocalPlayerPtr(true);
	if (!Utils::Valid(localPlayerPtr)) return nullptr;

	Entity localPlayer{};
	if (!Utils::Rpm(localPlayerPtr, &localPlayer, sizeof localPlayer)) return nullptr;
	if (!Utils::Valid(localPlayer.inventoryPtr)) return nullptr;

	Inventory inventoryData{};
	if (!Utils::Rpm(localPlayer.inventoryPtr, &inventoryData, sizeof inventoryData)) return nullptr;
	if (!Utils::Valid(inventoryData.itemArrayPtr) || inventoryData.itemArrayEnd < inventoryData.itemArrayPtr) return
		nullptr;

	const auto itemArraySize = (inventoryData.itemArrayEnd - inventoryData.itemArrayPtr) / sizeof(Item);
	if (!itemArraySize || itemArraySize > 0x7FFF) return nullptr;

	auto* itemData = new Item[itemArraySize];
	if (!Utils::Rpm(inventoryData.itemArrayPtr, &*itemData, itemArraySize * sizeof(Item)))
	{
		delete[]itemData;
		itemData = nullptr;
		return nullptr;
	}

	auto* array = new char* [13];
	array[0] = new char[sizeof"[?] No Weapon Selected"];
	sprintf_s(array[0], sizeof"[?] No Weapon Selected", "[?] No Weapon Selected");
	for (auto i = 1; i < 13; i++)
	{
		array[i] = nullptr;
	}

	for (DWORD64 i = 0; i < itemArraySize; i++)
	{
		if (!Utils::Valid(itemData[i].referencePtr)) continue;
		if (itemData[i].favoriteIndex > 12) continue;

		Reference referenceData{};
		if (!Utils::Rpm(itemData[i].referencePtr, &referenceData, sizeof referenceData)) continue;
		if (referenceData.vtable != ErectusProcess::exe + VTABLE_TESOBJECTWEAP) continue;

		auto* tempWeaponName = ErectusMemory::GetInstancedItemName(itemData[i].displayPtr);
		if (tempWeaponName == nullptr)
		{
			tempWeaponName = GetEntityName(referenceData.namePtr0098);
			if (tempWeaponName == nullptr)
			{
				continue;
			}
		}

		const auto arrayTextSize = static_cast <int>(sizeof"[?] " + Utils::GetTextLength(tempWeaponName));
		auto* arrayText = new char[arrayTextSize];
		sprintf_s(arrayText, arrayTextSize, "[%c] %s", GetFavoriteSlot(itemData[i].favoriteIndex), tempWeaponName);

		array[itemData[i].favoriteIndex + 1] = arrayText;
		delete[]tempWeaponName;
		tempWeaponName = nullptr;
	}

	for (auto i = 1; i < 13; i++)
	{
		if (array[i] == nullptr)
		{
			array[i] = new char[sizeof"[?] Favorited Item InValid"];
			sprintf_s(array[i], sizeof"[?] Favorited Item InValid", "[%c] Favorited Item InValid",
				GetFavoriteSlot(BYTE(i - 1)));
		}
	}

	delete[]itemData;
	itemData = nullptr;
	return array;
}

char* ErectusMemory::GetFavoritedWeaponText(const BYTE index)
{
	const auto localPlayerPtr = GetLocalPlayerPtr(true);
	if (!Utils::Valid(localPlayerPtr)) return nullptr;

	Entity localPlayer{};
	if (!Utils::Rpm(localPlayerPtr, &localPlayer, sizeof localPlayer)) return nullptr;
	if (!Utils::Valid(localPlayer.inventoryPtr)) return nullptr;

	Inventory inventoryData{};
	if (!Utils::Rpm(localPlayer.inventoryPtr, &inventoryData, sizeof inventoryData)) return nullptr;
	if (!Utils::Valid(inventoryData.itemArrayPtr) || inventoryData.itemArrayEnd < inventoryData.itemArrayPtr) return
		nullptr;

	const auto itemArraySize = (inventoryData.itemArrayEnd - inventoryData.itemArrayPtr) / sizeof(Item);
	if (!itemArraySize || itemArraySize > 0x7FFF) return nullptr;

	auto* itemData = new Item[itemArraySize];
	if (!Utils::Rpm(inventoryData.itemArrayPtr, &*itemData, itemArraySize * sizeof(Item)))
	{
		delete[]itemData;
		itemData = nullptr;
		return nullptr;
	}

	for (DWORD64 i = 0; i < itemArraySize; i++)
	{
		if (!Utils::Valid(itemData[i].referencePtr)) continue;
		if (itemData[i].favoriteIndex != index) continue;

		Reference referenceData{};
		if (!Utils::Rpm(itemData[i].referencePtr, &referenceData, sizeof referenceData)) break;
		if (referenceData.vtable != ErectusProcess::exe + VTABLE_TESOBJECTWEAP) break;

		auto* tempWeaponName = ErectusMemory::GetInstancedItemName(itemData[i].displayPtr);
		if (tempWeaponName == nullptr)
		{
			tempWeaponName = GetEntityName(referenceData.namePtr0098);
			if (tempWeaponName == nullptr)
			{
				continue;
			}
		}

		const auto weaponTextSize = static_cast <int>(sizeof"[?] " + Utils::GetTextLength(tempWeaponName));
		auto* weaponText = new char[weaponTextSize];
		sprintf_s(weaponText, weaponTextSize, "[%c] %s", GetFavoriteSlot(itemData[i].favoriteIndex), tempWeaponName);

		delete[]tempWeaponName;
		tempWeaponName = nullptr;

		delete[]itemData;
		itemData = nullptr;
		return weaponText;
	}

	delete[]itemData;
	itemData = nullptr;
	return nullptr;
}

bool ErectusMemory::EntityInventoryValid(const Entity entityData)
{
	if (!Utils::Valid(entityData.inventoryPtr))
	{
		return false;
	}

	Inventory inventoryData{};
	if (!Utils::Rpm(entityData.inventoryPtr, &inventoryData, sizeof inventoryData)) return false;
	if (!Utils::Valid(inventoryData.itemArrayPtr) || inventoryData.itemArrayEnd < inventoryData.itemArrayPtr) return
		false;

	const auto itemArraySize = (inventoryData.itemArrayEnd - inventoryData.itemArrayPtr) / sizeof(Item);
	if (!itemArraySize || itemArraySize > 0x7FFF) return false;

	auto* itemData = new Item[itemArraySize];
	if (!Utils::Rpm(inventoryData.itemArrayPtr, &*itemData, itemArraySize * sizeof(Item)))
	{
		delete[]itemData;
		itemData = nullptr;
		return false;
	}

	for (DWORD64 i = 0; i < itemArraySize; i++)
	{
		if (!Utils::Valid(itemData[i].referencePtr)) continue;

		Reference referenceData{};
		if (!Utils::Rpm(itemData[i].referencePtr, &referenceData, sizeof referenceData)) continue;
		if (referenceData.recordFlagA >> 2 & 1) continue;

		delete[]itemData;
		itemData = nullptr;
		return true;
	}

	delete[]itemData;
	itemData = nullptr;
	return false;
}

bool ErectusMemory::AllowLegendaryWeapons(EntityLooterSettings* customEntityLooterSettings)
{
	if (!customEntityLooterSettings->entityLooterAllWeaponsEnabled)
	{
		if (customEntityLooterSettings->entityLooterOneStarWeaponsEnabled)
		{
			return true;
		}
		if (customEntityLooterSettings->entityLooterTwoStarWeaponsEnabled)
		{
			return true;
		}
		if (customEntityLooterSettings->entityLooterThreeStarWeaponsEnabled)
		{
			return true;
		}
	}

	return false;
}

bool ErectusMemory::AllowLegendaryArmor(EntityLooterSettings* customEntityLooterSettings)
{
	if (customEntityLooterSettings->entityLooterAllArmorEnabled || customEntityLooterSettings->entityLooterOneStarArmorEnabled || customEntityLooterSettings->entityLooterTwoStarArmorEnabled || customEntityLooterSettings->entityLooterThreeStarArmorEnabled)
		return true;

	return false;
}

bool ErectusMemory::CheckEntityLooterItem(const DWORD formId, const DWORD64 entityFlag, EntityLooterSettings* customEntityLooterSettings, const bool legendaryWeaponsEnabled, const bool legendaryArmorEnabled)
{
	if (customEntityLooterSettings->entityLooterCapsEnabled && formId == 0x0000000F)
	{
		return true;
	}

	if (customEntityLooterSettings->entityLooterListEnabled)
	{
		if (CheckFormIdArray(formId, customEntityLooterSettings->entityLooterEnabledList,
			customEntityLooterSettings->entityLooterFormIdList, 100))
		{
			return true;
		}
	}

	if (entityFlag & CUSTOM_ENTRY_WEAPON)
	{
		if (customEntityLooterSettings->entityLooterAllWeaponsEnabled)
		{
			return true;
		}
		return legendaryWeaponsEnabled;
	}
	if (entityFlag & CUSTOM_ENTRY_ARMOR)
	{
		if (customEntityLooterSettings->entityLooterAllArmorEnabled)
		{
			return true;
		}
		return legendaryArmorEnabled;
	}
	if (entityFlag & CUSTOM_ENTRY_AMMO)
	{
		return customEntityLooterSettings->entityLooterAmmoEnabled;
	}
	if (entityFlag & CUSTOM_ENTRY_MOD)
	{
		return customEntityLooterSettings->entityLooterModsEnabled;
	}
	if (entityFlag & CUSTOM_ENTRY_JUNK)
	{
		return customEntityLooterSettings->entityLooterJunkEnabled;
	}
	if (entityFlag & CUSTOM_ENTRY_AID)
	{
		return customEntityLooterSettings->entityLooterAidEnabled;
	}
	if (entityFlag & CUSTOM_ENTRY_TREASURE_MAP)
	{
		return customEntityLooterSettings->entityLooterTreasureMapsEnabled;
	}
	if (entityFlag & CUSTOM_ENTRY_PLAN)
	{
		if (entityFlag & CUSTOM_ENTRY_KNOWN_RECIPE)
		{
			return customEntityLooterSettings->entityLooterKnownPlansEnabled;
		}
		if (entityFlag & CUSTOM_ENTRY_UNKNOWN_RECIPE)
		{
			return customEntityLooterSettings->entityLooterUnknownPlansEnabled;
		}
		if (entityFlag & CUSTOM_ENTRY_FAILED_RECIPE)
		{
			return customEntityLooterSettings->entityLooterUnknownPlansEnabled;
		}
	}
	else if (entityFlag & CUSTOM_ENTRY_MISC)
	{
		return customEntityLooterSettings->entityLooterMiscEnabled;
	}

	if (customEntityLooterSettings->entityLooterUnlistedEnabled)
	{
		return true;
	}

	return false;
}

bool ErectusMemory::IsLegendaryFormId(const DWORD formId)
{
	for (auto i : legendaryFormIdArray)
	{
		if (formId == i)
		{
			return true;
		}
	}

	return false;
}

BYTE ErectusMemory::GetLegendaryRank(const DWORD64 displayPtr)
{
	if (!Utils::Valid(displayPtr)) return 0;

	DWORD64 instancedArrayPtr;
	if (!Utils::Rpm(displayPtr, &instancedArrayPtr, sizeof instancedArrayPtr)) return 0;
	if (!Utils::Valid(instancedArrayPtr)) return 0;

	ItemInstancedArray itemInstancedArrayData{};
	if (!Utils::Rpm(instancedArrayPtr, &itemInstancedArrayData, sizeof itemInstancedArrayData)) return 0;
	if (!Utils::Valid(itemInstancedArrayData.arrayPtr) || itemInstancedArrayData.arrayEnd < itemInstancedArrayData.
		arrayPtr) return 0;

	const auto instancedArraySize = (itemInstancedArrayData.arrayEnd - itemInstancedArrayData.arrayPtr) / sizeof(DWORD64);
	if (!instancedArraySize || instancedArraySize > 0x7FFF) return 0;

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
		if (!Utils::Valid(instancedArray[i])) continue;

		ExtraTextDisplayData extraTextDisplayDataData{};
		if (!Utils::Rpm(instancedArray[i], &extraTextDisplayDataData, sizeof extraTextDisplayDataData)) continue;

		const auto rttiNamePtr = RttiGetNamePtr(extraTextDisplayDataData.vtable);
		if (!rttiNamePtr) continue;

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
	{
		return 0;
	}

	ObjectInstanceExtra objectInstanceExtraData{};
	if (!Utils::Rpm(objectInstanceExtraPtr, &objectInstanceExtraData, sizeof objectInstanceExtraData)) return 0;
	if (!Utils::Valid(objectInstanceExtraData.modDataPtr)) return 0;

	ModInstance modInstanceData{};
	if (!Utils::Rpm(objectInstanceExtraData.modDataPtr, &modInstanceData, sizeof modInstanceData)) return 0;
	if (!Utils::Valid(modInstanceData.modListPtr) || !modInstanceData.modListSize) return 0;

	const DWORD64 modArraySize = modInstanceData.modListSize / 0x8;
	if (!modArraySize || modArraySize > 0x7FFF) return 0;

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

bool ErectusMemory::ValidLegendary(const BYTE legendaryRank, const DWORD64 entityFlag, EntityLooterSettings* customEntityLooterSettings, const bool legendaryWeaponsEnabled, const bool legendaryArmorEnabled)
{
	if (entityFlag & CUSTOM_ENTRY_WEAPON)
	{
		if (legendaryWeaponsEnabled)
		{
			switch (legendaryRank)
			{
			case 0x01:
				return customEntityLooterSettings->entityLooterOneStarWeaponsEnabled;
			case 0x02:
				return customEntityLooterSettings->entityLooterTwoStarWeaponsEnabled;
			case 0x03:
				return customEntityLooterSettings->entityLooterThreeStarWeaponsEnabled;
			default:
				return customEntityLooterSettings->entityLooterAllWeaponsEnabled;
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
				return customEntityLooterSettings->entityLooterOneStarArmorEnabled;
			case 0x02:
				return customEntityLooterSettings->entityLooterTwoStarArmorEnabled;
			case 0x03:
				return customEntityLooterSettings->entityLooterThreeStarArmorEnabled;
			default:
				return customEntityLooterSettings->entityLooterAllArmorEnabled;
			}
		}
	}

	return false;
}

bool ErectusMemory::TransferEntityItems(Entity entityData, Reference referenceData, Entity localPlayer,
	bool onlyUseEntityLooterList, bool useEntityLooterBlacklist)
{
	EntityLooterSettings* currentEntityLooterSettings = nullptr;
	switch (referenceData.vtable - ErectusProcess::exe)
	{
	case VTABLE_TESNPC:
		currentEntityLooterSettings = &ErectusIni::npcLooterSettings;
		break;
	case VTABLE_TESOBJECTCONT:
		currentEntityLooterSettings = &ErectusIni::containerLooterSettings;
		break;
	default:
		return false;
	}

	if (!Utils::Valid(entityData.inventoryPtr))
		return false;

	Inventory inventoryData{};
	if (!Utils::Rpm(entityData.inventoryPtr, &inventoryData, sizeof inventoryData)) return false;
	if (!Utils::Valid(inventoryData.itemArrayPtr) || inventoryData.itemArrayEnd < inventoryData.itemArrayPtr) return
		false;

	auto itemArraySize = (inventoryData.itemArrayEnd - inventoryData.itemArrayPtr) / sizeof(Item);
	if (!itemArraySize || itemArraySize > 0x7FFF) return false;

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
		if (!Utils::Valid(itemData[i].referencePtr)) continue;
		if (!Utils::Valid(itemData[i].displayPtr) || itemData[i].iterations < itemData[i].displayPtr) continue;

		Reference itemReferenceData{};
		if (!Utils::Rpm(itemData[i].referencePtr, &itemReferenceData, sizeof itemReferenceData)) continue;
		if (itemReferenceData.recordFlagA >> 2 & 1) continue;

		if (useEntityLooterBlacklist)
		{
			if (CheckFormIdArray(itemReferenceData.formId, currentEntityLooterSettings->entityLooterBlacklistEnabled,
				currentEntityLooterSettings->entityLooterBlacklist, 64))
			{
				continue;
			}
		}

		if (onlyUseEntityLooterList)
		{
			if (!CheckFormIdArray(itemReferenceData.formId, currentEntityLooterSettings->entityLooterEnabledList,
				currentEntityLooterSettings->entityLooterFormIdList, 100))
			{
				continue;
			}
		}

		auto entityFlag = CUSTOM_ENTRY_DEFAULT;
		DWORD64 entityNamePtr = 0;
		auto enabledDistance = 0;

		GetCustomEntityData(itemReferenceData, &entityFlag, &entityNamePtr, &enabledDistance, false, false);
		if (!(entityFlag & CUSTOM_ENTRY_VALID_ITEM)) continue;

		if (!onlyUseEntityLooterList)
		{
			if (!CheckEntityLooterItem(itemReferenceData.formId, entityFlag, currentEntityLooterSettings,
				legendaryWeaponsEnabled, legendaryArmorEnabled))
			{
				continue;
			}
		}

		auto iterations = (itemData[i].iterations - itemData[i].displayPtr) / sizeof(ItemCount);
		if (!iterations || iterations > 0x7FFF) continue;

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
		{
			continue;
		}

		if (entityFlag & CUSTOM_ENTRY_WEAPON)
		{
			if (legendaryWeaponsEnabled)
			{
				auto legendaryRank = GetLegendaryRank(itemData[i].displayPtr);
				if (!ValidLegendary(legendaryRank, entityFlag, currentEntityLooterSettings, legendaryWeaponsEnabled,
					legendaryArmorEnabled))
				{
					continue;
				}
			}
		}
		else if (entityFlag & CUSTOM_ENTRY_ARMOR)
		{
			if (legendaryArmorEnabled)
			{
				auto legendaryRank = GetLegendaryRank(itemData[i].displayPtr);
				if (!ValidLegendary(legendaryRank, entityFlag, currentEntityLooterSettings, legendaryWeaponsEnabled,
					legendaryArmorEnabled))
				{
					continue;
				}
			}
		}

		TransferMessage transferMessageData{};
		transferMessageData.vtable = ErectusProcess::exe + VTABLE_REQUESTTRANSFERITEMMSG;
		transferMessageData.srcFormId = entityData.formId;
		transferMessageData.unknownId = 0xE0001F7A;
		transferMessageData.dstFormId = localPlayer.formId;
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

bool ErectusMemory::ContainerValid(const Reference referenceData)
{
	if (!Utils::Valid(referenceData.keywordArrayData00C0))
	{
		return false;
	}

	int nifTextLength;
	if (!Utils::Rpm(referenceData.keywordArrayData00C0 + 0x10, &nifTextLength, sizeof nifTextLength)) return false;
	if (nifTextLength == 41)
	{
		char containerMarkerCheck[sizeof"ContainerMarker"];
		if (!Utils::Rpm(referenceData.keywordArrayData00C0 + 0x2E, &containerMarkerCheck,
			sizeof containerMarkerCheck)) return false;

		containerMarkerCheck[15] = '\0';
		if (!strcmp(containerMarkerCheck, "ContainerMarker"))
		{
			return false;
		}
	}

	if (!Utils::Valid(referenceData.namePtr00B0))
	{
		return false;
	}

	DWORD64 nameBuffer;
	if (!Utils::Rpm(referenceData.namePtr00B0 + 0x10, &nameBuffer, sizeof nameBuffer)) return false;
	if (!nameBuffer) return false;

	if (!Utils::Valid(nameBuffer))
	{
		nameBuffer = referenceData.namePtr00B0;
	}

	int nameTextLength;
	if (!Utils::Rpm(nameBuffer + 0x10, &nameTextLength, sizeof nameTextLength)) return false;
	if (!nameTextLength || nameTextLength > 0x7FFF) return false;

	return true;
}

bool ErectusMemory::LootEntity(Entity entityData, Reference referenceData, Entity localPlayer,
	bool onlyUseEntityLooterList, bool useEntityLooterBlacklist)
{
	auto isEntityNpc = false;
	auto isEntityContainer = false;

	auto maxDistance = 0;
	switch (referenceData.vtable - ErectusProcess::exe)
	{
	case VTABLE_TESNPC:
		isEntityNpc = true;
		maxDistance = 76;
		break;
	case VTABLE_TESOBJECTCONT:
		isEntityContainer = true;
		maxDistance = 6;
		break;
	default:
		return false;
	}

	if (isEntityNpc && (referenceData.formId == 0x00000007 || CheckHealthFlag(entityData.healthFlag) != 0x3))
		return false;

	auto normalDistance = static_cast<int>(Utils::GetDistance(entityData.position, localPlayer.position) * 0.01f);

	if (normalDistance > maxDistance)
		return false;

	if (isEntityContainer && !ContainerValid(referenceData))
		return false;

	if (!EntityInventoryValid(entityData))
		return false;

	return TransferEntityItems(entityData, referenceData, localPlayer, onlyUseEntityLooterList, useEntityLooterBlacklist);
}

bool ErectusMemory::CheckEntityLooterSettings(EntityLooterSettings* customEntityLooterSettings)
{
	if (customEntityLooterSettings->entityLooterAllWeaponsEnabled)
		return true;
	if (customEntityLooterSettings->entityLooterAllArmorEnabled)
		return true;
	if (customEntityLooterSettings->entityLooterOneStarWeaponsEnabled)
		return true;
	if (customEntityLooterSettings->entityLooterOneStarArmorEnabled)
		return true;
	if (customEntityLooterSettings->entityLooterTwoStarWeaponsEnabled)
		return true;
	if (customEntityLooterSettings->entityLooterTwoStarArmorEnabled)
		return true;
	if (customEntityLooterSettings->entityLooterThreeStarWeaponsEnabled)
		return true;
	if (customEntityLooterSettings->entityLooterThreeStarArmorEnabled)
		return true;
	if (customEntityLooterSettings->entityLooterAmmoEnabled)
		return true;
	if (customEntityLooterSettings->entityLooterModsEnabled)
		return true;
	if (customEntityLooterSettings->entityLooterCapsEnabled)
		return true;
	if (customEntityLooterSettings->entityLooterJunkEnabled)
		return true;
	if (customEntityLooterSettings->entityLooterAidEnabled)
		return true;
	if (customEntityLooterSettings->entityLooterTreasureMapsEnabled)
		return true;
	if (customEntityLooterSettings->entityLooterKnownPlansEnabled)
		return true;
	if (customEntityLooterSettings->entityLooterUnknownPlansEnabled)
		return true;
	if (customEntityLooterSettings->entityLooterMiscEnabled)
		return true;
	if (customEntityLooterSettings->entityLooterUnlistedEnabled)
		return true;
	if (customEntityLooterSettings->entityLooterListEnabled)
		return CheckEntityLooterList(customEntityLooterSettings);

	return false;
}

bool ErectusMemory::CheckOnlyUseEntityLooterList(EntityLooterSettings* customEntityLooterSettings)
{
	if (customEntityLooterSettings->entityLooterAllWeaponsEnabled)
		return false;
	if (customEntityLooterSettings->entityLooterAllArmorEnabled)
		return false;
	if (customEntityLooterSettings->entityLooterOneStarWeaponsEnabled)
		return false;
	if (customEntityLooterSettings->entityLooterOneStarArmorEnabled)
		return false;
	if (customEntityLooterSettings->entityLooterTwoStarWeaponsEnabled)
		return false;
	if (customEntityLooterSettings->entityLooterTwoStarArmorEnabled)
		return false;
	if (customEntityLooterSettings->entityLooterThreeStarWeaponsEnabled)
		return false;
	if (customEntityLooterSettings->entityLooterThreeStarArmorEnabled)
		return false;
	if (customEntityLooterSettings->entityLooterAmmoEnabled)
		return false;
	if (customEntityLooterSettings->entityLooterModsEnabled)
		return false;
	if (customEntityLooterSettings->entityLooterCapsEnabled)
		return false;
	if (customEntityLooterSettings->entityLooterJunkEnabled)
		return false;
	if (customEntityLooterSettings->entityLooterAidEnabled)
		return false;
	if (customEntityLooterSettings->entityLooterTreasureMapsEnabled)
		return false;
	if (customEntityLooterSettings->entityLooterKnownPlansEnabled)
		return false;
	if (customEntityLooterSettings->entityLooterUnknownPlansEnabled)
		return false;
	if (customEntityLooterSettings->entityLooterMiscEnabled)
		return false;
	if (customEntityLooterSettings->entityLooterUnlistedEnabled)
		return false;
	if (customEntityLooterSettings->entityLooterListEnabled)
		return CheckEntityLooterList(customEntityLooterSettings);

	return false;
}

bool ErectusMemory::HarvestFlora(Entity entityData, Reference referenceData, Entity localPlayer)
{
	if (FloraHarvested(entityData.harvestFlagA, entityData.harvestFlagB))
		return false;

	auto normalDistance = static_cast<int>(Utils::GetDistance(entityData.position, localPlayer.position) * 0.01f);
	if (normalDistance > 6)
		return false;

	if (!FloraValid(referenceData))
		return false;

	RequestActivateRefMessage requestActivateRefMessageData{};
	requestActivateRefMessageData.vtable = ErectusProcess::exe + VTABLE_REQUESTACTIVATEREFMSG;
	requestActivateRefMessageData.formId = entityData.formId;
	requestActivateRefMessageData.choice = 0xFF;
	requestActivateRefMessageData.forceActivate = 0;

	return SendMessageToServer(&requestActivateRefMessageData, sizeof requestActivateRefMessageData);
}

bool ErectusMemory::Harvester()
{
	if (!MessagePatcher(allowMessages))
	{
		return false;
	}

	if (!allowMessages)
	{
		return false;
	}

	auto useNpcLooter = ErectusIni::npcLooterSettings.entityLooterEnabled && CheckEntityLooterSettings(&ErectusIni::npcLooterSettings);

	auto useContainerLooter = ErectusIni::containerLooterSettings.entityLooterEnabled && CheckEntityLooterSettings(&ErectusIni::containerLooterSettings);

	auto useFloraHarvester = ErectusIni::customHarvesterSettings.harvesterEnabled && CheckIngredientList();

	if (!useNpcLooter && !useContainerLooter && !useFloraHarvester)
	{
		return false;
	}

	auto localPlayerPtr = GetLocalPlayerPtr(true);
	if (!Utils::Valid(localPlayerPtr)) return false;

	Entity localPlayer{};
	if (!Utils::Rpm(localPlayerPtr, &localPlayer, sizeof localPlayer)) return false;

	auto onlyUseNpcLooterList = false;
	auto useNpcLooterBlacklist = false;
	if (useNpcLooter)
	{
		onlyUseNpcLooterList = CheckOnlyUseEntityLooterList(&ErectusIni::npcLooterSettings);
		useNpcLooterBlacklist = CheckEntityLooterBlacklist(&ErectusIni::npcLooterSettings);
	}

	auto onlyUseContainerLooterList = false;
	auto useContainerLooterBlacklist = false;
	if (useContainerLooter)
	{
		onlyUseContainerLooterList = CheckOnlyUseEntityLooterList(&ErectusIni::containerLooterSettings);
		useContainerLooterBlacklist = CheckEntityLooterBlacklist(&ErectusIni::containerLooterSettings);
	}

	if (useNpcLooter)
	{
		auto temporaryNpcSize = 0;
		auto* temporaryNpcList = GetNpcList(&temporaryNpcSize);
		if (temporaryNpcList == nullptr) return false;

		for (auto i = 0; i < temporaryNpcSize; i++)
		{
			if (!Utils::Valid(temporaryNpcList[i])) continue;
			if (temporaryNpcList[i] == localPlayerPtr) continue;

			Entity entityData{};
			if (!Utils::Rpm(temporaryNpcList[i], &entityData, sizeof entityData)) continue;
			if (!Utils::Valid(entityData.referencePtr)) continue;

			if (entityData.spawnFlag != 0x02)
			{
				continue;
			}

			Reference referenceData{};
			if (!Utils::Rpm(entityData.referencePtr, &referenceData, sizeof referenceData)) continue;
			if (referenceData.vtable != ErectusProcess::exe + VTABLE_TESNPC) continue;
			if (referenceData.formId == 0x00000007) continue;

			LootEntity(entityData, referenceData, localPlayer, onlyUseNpcLooterList, useNpcLooterBlacklist);
		}

		delete[]temporaryNpcList;
		temporaryNpcList = nullptr;
	}

	if (useContainerLooter || useFloraHarvester)
	{
		auto temporaryEntitySize = 0;
		auto* temporaryEntityList = GetEntityList(&temporaryEntitySize);
		if (temporaryEntityList == nullptr) return false;

		for (auto i = 0; i < temporaryEntitySize; i++)
		{
			if (!Utils::Valid(temporaryEntityList[i])) continue;
			if (temporaryEntityList[i] == localPlayerPtr) continue;

			Entity entityData{};
			if (!Utils::Rpm(temporaryEntityList[i], &entityData, sizeof entityData)) continue;
			if (!Utils::Valid(entityData.referencePtr)) continue;

			if (entityData.spawnFlag != 0x02)
			{
				continue;
			}

			Reference referenceData{};
			if (!Utils::Rpm(entityData.referencePtr, &referenceData, sizeof referenceData)) continue;

			if (referenceData.vtable == ErectusProcess::exe + VTABLE_TESOBJECTCONT)
			{
				if (useContainerLooter)
				{
					LootEntity(entityData, referenceData, localPlayer, onlyUseContainerLooterList,
						useContainerLooterBlacklist);
				}
			}
			else if (referenceData.vtable == ErectusProcess::exe + VTABLE_TESFLORA)
			{
				if (useFloraHarvester)
				{
					HarvestFlora(entityData, referenceData, localPlayer);
				}
			}
		}

		delete[]temporaryEntityList;
		temporaryEntityList = nullptr;
	}

	return true;
}

bool ErectusMemory::MeleeAttack()
{
	if (!MessagePatcher(allowMessages))
	{
		return false;
	}

	if (!allowMessages)
	{
		return false;
	}

	const auto localPlayerPtr = GetLocalPlayerPtr(true);
	if (!Utils::Valid(localPlayerPtr)) return false;

	const auto allocAddress = Utils::AllocEx(sizeof(ExternalFunction));
	if (allocAddress == 0) return false;

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
	{
		return false;
	}

	Utils::FreeEx(allocAddress);
	return true;
}

bool ErectusMemory::ChargenEditing()
{
	DWORD64 chargenPtr;
	if (!Utils::Rpm(ErectusProcess::exe + OFFSET_CHARGEN, &chargenPtr, sizeof chargenPtr)) return false;
	if (!Utils::Valid(chargenPtr)) return false;

	Chargen chargenData{};
	if (!Utils::Rpm(chargenPtr, &chargenData, sizeof chargenData)) return false;

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
	{
		return Utils::Wpm(chargenPtr, &chargenData, sizeof chargenData);
	}

	return true;
}

bool ErectusMemory::CreateProjectile(const DWORD itemId, const float* position, const float* rotation)
{
	if (!MessagePatcher(allowMessages))
	{
		return false;
	}

	if (!allowMessages)
	{
		return false;
	}

	const auto allocAddress = Utils::AllocEx(sizeof(ExecutionProjectile));
	if (allocAddress == 0) return false;

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
	{
		return false;
	}

	Utils::FreeEx(allocAddress);
	return true;
}

bool ErectusMemory::CreateForwardProjectile(const DWORD itemId)
{
	if (itemId == 0)
	{
		return false;
	}

	const auto cameraPtr = GetCameraPtr();
	if (!Utils::Valid(cameraPtr)) return false;

	Camera cameraData{};
	if (!Utils::Rpm(cameraPtr, &cameraData, sizeof cameraData)) return false;

	float rotation[3];
	rotation[0] = -atan2f(cameraData.forward[2],
		sqrtf(powf(cameraData.forward[0], 2.0f) + powf(cameraData.forward[1], 2.0f)));
	rotation[1] = 0.0f;
	rotation[2] = -atan2f(-cameraData.forward[0], cameraData.forward[1]);

	return CreateProjectile(itemId, cameraData.origin, rotation);
}
