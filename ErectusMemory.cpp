#include "ErectusInclude.h"
using namespace memoryClasses;
using namespace settingsClasses;

DWORD KnownRecipeArray[0x1000];

DWORD LegendaryFormidArray[]
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

DWORD64 ErectusMemory::GetAddress(DWORD Formid)
{
	DWORD64 v1;
	if (!Erectus::RPM(ErectusProcess::Exe + OFFSET_GET_PTR_A1, &v1, sizeof(v1))) return 0;
	if (!Erectus::Valid(v1)) return 0;

	DWORD v2;
	if (!Erectus::RPM(v1 + 0x8 + 0x18, &v2, sizeof(v2))) return 0;
	if (!v2) return 0;

	DWORD v3 = 0;

	for (int i = 0; i < sizeof(Formid); i++)
	{
		DWORD v4 = ((v3 ^ (Formid >> (i * 0x8))) & 0xFF);

		DWORD v5;
		if (!Erectus::RPM(ErectusProcess::Exe + OFFSET_GET_PTR_A2 + v4 * 0x4, &v5, sizeof(v5))) return 0;

		v3 = ((v3 >> 0x8) ^ v5);
	}

	DWORD v6 = (v3 & (v2 - 1));

	DWORD64 v7;
	if (!Erectus::RPM(v1 + 0x8 + 0x10, &v7, sizeof(v7))) return 0;
	if (!Erectus::Valid(v7)) return 0;

	DWORD v8;
	if (!Erectus::RPM(v7 + (v6 + v6 * 2) * 0x8 + 0x10, &v8, sizeof(v8))) return 0;
	if (v8 == 0xFFFFFFFF) return 0;

	DWORD v9 = v2;

	for (int i = 0; i < 100; i++)
	{
		DWORD v10;
		if (!Erectus::RPM(v7 + (v6 + v6 * 2) * 0x8, &v10, sizeof(v10))) return 0;
		if (v10 == Formid)
		{
			v9 = v6;
			if (v9 != v2) break;
		}
		else
		{
			if (!Erectus::RPM(v7 + (v6 + v6 * 2) * 0x8 + 0x10, &v6, sizeof(v6))) return 0;
			if (v6 == v2) break;
		}
	}

	if (v9 == v2) return 0;

	return v7 + (v9 + v9 * 2) * 0x8 + 0x8;
}

DWORD64 ErectusMemory::GetPtr(DWORD Formid)
{
	DWORD64 Address = GetAddress(Formid);
	if (!Address) return 0;

	DWORD64 Ptr;
	if (!Erectus::RPM(Address, &Ptr, sizeof(Ptr))) return 0;

	return Ptr;
}

DWORD64 ErectusMemory::GetCameraPtr()
{
	DWORD64 CameraPtr;
	if (!Erectus::RPM(ErectusProcess::Exe + OFFSET_CAMERA, &CameraPtr, sizeof(CameraPtr))) return 0;
	return CameraPtr;
}

BYTE ErectusMemory::CheckHealthFlag(BYTE HealthFlag)
{
	BYTE Flag = HealthFlag;
	Flag &= ~(1 << 7);
	Flag &= ~(1 << 6);
	Flag &= ~(1 << 5);
	switch (Flag)
	{
	case 0x00://Alive
		return 0x01;
	case 0x02://Dead
		return 0x03;
	case 0x04://Dead
		return 0x03;
	case 0x10://Downed
		return 0x02;
	case 0x12://Downed
		return 0x02;
	default://Unknown
		return 0x00;
	}
}

DWORD64 ErectusMemory::GetLocalPlayerPtr(bool CheckMainMenu)
{
	DWORD64 LocalPlayerPtr;
	if (!Erectus::RPM(ErectusProcess::Exe + OFFSET_LOCAL_PLAYER, &LocalPlayerPtr, sizeof(LocalPlayerPtr))) return 0;
	if (!Erectus::Valid(LocalPlayerPtr)) return 0;

	if (CheckMainMenu)
	{
		Entity LocalPlayerData;
		if (!Erectus::RPM(LocalPlayerPtr, &LocalPlayerData, sizeof(LocalPlayerData))) return 0;
		if (LocalPlayerData.Formid == 0x00000014) return 0;
	}

	return LocalPlayerPtr;
}

DWORD64* ErectusMemory::GetEntityList(int* Size)
{
	DWORD64 EntityListTypePtr;
	if (!Erectus::RPM(ErectusProcess::Exe + OFFSET_ENTITY_LIST, &EntityListTypePtr, sizeof(EntityListTypePtr))) return nullptr;
	if (!Erectus::Valid(EntityListTypePtr)) return nullptr;

	EntityListType EntityListTypeData;
	if (!Erectus::RPM(EntityListTypePtr, &EntityListTypeData, sizeof(EntityListTypeData))) return nullptr;
	if (!Erectus::Valid(EntityListTypeData.InteriorListPtr)) return nullptr;
	if (!Erectus::Valid(EntityListTypeData.InteriorListCheck)) return nullptr;
	if (!Erectus::Valid(EntityListTypeData.ExteriorListPtr)) return nullptr;
	if (!Erectus::Valid(EntityListTypeData.ExteriorListCheck)) return nullptr;

	DWORD64 EntityListArrayPtr = 0;
	int EntityListArraySize = 0;

	bool UseInteriorList = false;
	if (EntityListTypeData.InteriorListPtr != EntityListTypeData.InteriorListCheck)
	{
		UseInteriorList = true;
		EntityListArrayPtr = EntityListTypeData.InteriorListPtr;
		EntityListArraySize = 1;
	}

	bool UseExteriorList = false;
	if (EntityListTypeData.ExteriorListPtr != EntityListTypeData.ExteriorListCheck)
	{
		UseExteriorList = true;
		EntityListArrayPtr = EntityListTypeData.ExteriorListPtr;
		EntityListArraySize = 25;
	}

	if (UseInteriorList && UseExteriorList) return nullptr;
	if (!UseInteriorList && !UseExteriorList) return nullptr;

	DWORD64* EntityListPtr = new DWORD64[EntityListArraySize * 2];
	if (!Erectus::RPM(EntityListArrayPtr, &*EntityListPtr, EntityListArraySize * sizeof(DWORD64) * 2))
	{
		delete[]EntityListPtr;
		EntityListPtr = nullptr;
		return nullptr;
	}

	DWORD64** EntityPtr = new DWORD64 * [EntityListArraySize];
	int CombinedListSize = 0;

	EntityList* EntityListData = new EntityList[EntityListArraySize];
	for (int i = 0; i < EntityListArraySize; i++)
	{
		EntityPtr[i] = nullptr;
		if (!Erectus::Valid(EntityListPtr[i * 2])) continue;

		if (!Erectus::RPM(EntityListPtr[i * 2], &EntityListData[i], sizeof(EntityListData[i]))) continue;
		if (!Erectus::Valid(EntityListData[i].ListPtr) || !EntityListData[i].ListSize) continue;

		EntityPtr[i] = new DWORD64[EntityListData[i].ListSize];
		if (!Erectus::RPM(EntityListData[i].ListPtr, &*EntityPtr[i], EntityListData[i].ListSize * sizeof(DWORD64)))
		{
			delete[]EntityPtr[i];
			EntityPtr[i] = nullptr;
			continue;
		}

		CombinedListSize += EntityListData[i].ListSize;
	}

	DWORD64* List = nullptr;
	*Size = 0;

	if (CombinedListSize)
	{
		List = new DWORD64[CombinedListSize];
		*Size = CombinedListSize;

		int Offset = 0;
		for (int i = 0; i < EntityListArraySize; i++)
		{
			if (EntityPtr[i] == nullptr) continue;
			memcpy(&List[Offset], &*EntityPtr[i], EntityListData[i].ListSize * sizeof(DWORD64));
			Offset += EntityListData[i].ListSize;
		}
	}

	for (int i = 0; i < EntityListArraySize; i++)
	{
		if (EntityPtr[i] != nullptr)
		{
			delete[]EntityPtr[i];
			EntityPtr[i] = nullptr;
		}
	}

	delete[]EntityPtr;
	EntityPtr = nullptr;

	delete[]EntityListData;
	EntityListData = nullptr;

	delete[]EntityListPtr;
	EntityListPtr = nullptr;

	return List;
}

DWORD64* ErectusMemory::GetNPCList(int* Size)
{
	DWORD64 ProcessListsPtr;
	if (!Erectus::RPM(ErectusProcess::Exe + OFFSET_NPC_LIST, &ProcessListsPtr, sizeof(ProcessListsPtr))) return nullptr;
	if (!Erectus::Valid(ProcessListsPtr)) return nullptr;

	ProcessLists ProcessListsData;
	if (!Erectus::RPM(ProcessListsPtr, &ProcessListsData, sizeof(ProcessListsData))) return nullptr;

	int IdArraySize = 0;

	bool UseArrayA = false;
	if (Erectus::Valid(ProcessListsData.NPCIdArrayPtrA) && ProcessListsData.NPCIdArraySizeA)
	{
		IdArraySize += ProcessListsData.NPCIdArraySizeA;
		UseArrayA = true;
	}

	bool UseArrayB = false;
	if (Erectus::Valid(ProcessListsData.NPCIdArrayPtrB) && ProcessListsData.NPCIdArraySizeB)
	{
		IdArraySize += ProcessListsData.NPCIdArraySizeB;
		UseArrayB = true;
	}

	if (!IdArraySize)
	{
		return nullptr;
	}

	size_t AllocSize = sizeof(ExecutionList) + IdArraySize * sizeof(DWORD64) * 2;
	DWORD64 AllocAddress = Erectus::AllocEx(AllocSize);
	if (AllocAddress == 0) return nullptr;

	ExecutionList ExecutionListData;
	ExecutionListData.Function = ErectusProcess::Exe + OFFSET_NPC_PTR_FUNCTION;
	ExecutionListData.IdArraySize = IdArraySize;
	ExecutionListData.IdArray = AllocAddress + sizeof(ExecutionList);
	ExecutionListData.PtrArray = AllocAddress + sizeof(ExecutionList) + IdArraySize * sizeof(DWORD64);

	BYTE* PageData = new BYTE[AllocSize];
	memset(PageData, 0x00, AllocSize);
	memcpy(PageData, &ExecutionListData, sizeof(ExecutionListData));

	int Index = 0;

	if (UseArrayA)
	{
		for (int i = 0; i < ProcessListsData.NPCIdArraySizeA; i++)
		{
			DWORD64 IdAddress = ProcessListsData.NPCIdArrayPtrA + 0x4 * i;
			memcpy(&PageData[sizeof(ExecutionList) + Index * sizeof(DWORD64)], &IdAddress, sizeof(IdAddress));
			Index++;
		}
	}

	if (UseArrayB)
	{
		for (int i = 0; i < ProcessListsData.NPCIdArraySizeB; i++)
		{
			DWORD64 IdAddress = ProcessListsData.NPCIdArrayPtrB + 0x4 * i;
			memcpy(&PageData[sizeof(ExecutionList) + Index * sizeof(DWORD64)], &IdAddress, sizeof(IdAddress));
			Index++;
		}
	}

	bool Written = Erectus::WPM(AllocAddress, &*PageData, AllocSize);

	delete[]PageData;
	PageData = nullptr;

	if (!Written)
	{
		Erectus::FreeEx(AllocAddress);
		return nullptr;
	}

	DWORD64 ParamAddress = AllocAddress + sizeof(ExecutionList::ASM);
	HANDLE Thread = CreateRemoteThread(ErectusProcess::Handle, NULL, 0, LPTHREAD_START_ROUTINE(AllocAddress), LPVOID(ParamAddress), 0, 0);

	if (Thread == NULL)
	{
		Erectus::FreeEx(AllocAddress);
		return nullptr;
	}

	DWORD ThreadResult = WaitForSingleObject(Thread, 3000);
	CloseHandle(Thread);

	if (ThreadResult == WAIT_TIMEOUT)
	{
		return nullptr;
	}

	DWORD64* ExecutedList = new DWORD64[IdArraySize];
	if (!Erectus::RPM(AllocAddress + sizeof(ExecutionList) + IdArraySize * sizeof(DWORD64), &*ExecutedList, IdArraySize * sizeof(DWORD64)))
	{
		delete[]ExecutedList;
		ExecutedList = nullptr;

		Erectus::FreeEx(AllocAddress);
		return nullptr;
	}

	Erectus::FreeEx(AllocAddress);

	*Size = IdArraySize;
	return ExecutedList;
}

DWORD64* ErectusMemory::GetRecipeArray(int* Size)
{
	DWORD64 DataHandlerPtr;
	if (!Erectus::RPM(ErectusProcess::Exe + OFFSET_DATA_HANDLER, &DataHandlerPtr, sizeof(DataHandlerPtr))) return nullptr;
	if (!Erectus::Valid(DataHandlerPtr)) return nullptr;

	ReferenceList OMODList;
	if (!Erectus::RPM(DataHandlerPtr + 0xF70, &OMODList, sizeof(OMODList))) return nullptr;
	if (!Erectus::Valid(OMODList.ArrayPtr) || !OMODList.ArraySize || OMODList.ArraySize > 0x7FFF) return nullptr;

	DWORD64* OMODPtrArray = new DWORD64[OMODList.ArraySize];
	if (!Erectus::RPM(OMODList.ArrayPtr, &*OMODPtrArray, OMODList.ArraySize * sizeof(DWORD64)))
	{
		delete[]OMODPtrArray;
		OMODPtrArray = nullptr;
		return nullptr;
	}

	int BufferIndex = 0;
	DWORD64* BufferArray = new DWORD64[OMODList.ArraySize];
	memset(BufferArray, 0x00, OMODList.ArraySize * sizeof(DWORD64));

	int UniqueArrayIndex = 0;
	DWORD64* UniqueArray = new DWORD64[OMODList.ArraySize];
	memset(UniqueArray, 0x00, OMODList.ArraySize * sizeof(DWORD64));

	for (int i = 0; i < OMODList.ArraySize; i++)
	{
		if (!Erectus::Valid(OMODPtrArray[i]))
		{
			continue;
		}

		Reference ReferenceData;
		if (!Erectus::RPM(OMODPtrArray[i], &ReferenceData, sizeof(ReferenceData))) continue;
		if (ReferenceData.OMODFlag != 0x4) continue;

		DWORD64 RecipePtr = ReferenceData.NamePtr00B0;
		if (!Erectus::Valid(RecipePtr)) continue;

		bool UniquePtr = true;
		for (int c = 0; c < UniqueArrayIndex; c++)
		{
			if (UniqueArray[c] == RecipePtr)
			{
				UniquePtr = false;
				break;
			}
		}

		if (!UniquePtr)
		{
			continue;
		}

		UniqueArray[UniqueArrayIndex] = RecipePtr;
		UniqueArrayIndex++;

		BufferArray[BufferIndex] = OMODPtrArray[i];
		BufferIndex++;
	}

	delete[]UniqueArray;
	UniqueArray = nullptr;

	delete[]OMODPtrArray;
	OMODPtrArray = nullptr;

	int RecipeArraySize = 0;
	DWORD64* RecipeArray = nullptr;

	if (BufferIndex)
	{
		RecipeArraySize = BufferIndex;
		RecipeArray = new DWORD64[RecipeArraySize];
		memset(RecipeArray, 0x00, RecipeArraySize * sizeof(DWORD64));
		for (int i = 0; i < RecipeArraySize; i++)
		{
			RecipeArray[i] = BufferArray[i];
		}
	}

	delete[]BufferArray;
	BufferArray = nullptr;

	*Size = RecipeArraySize;
	return RecipeArray;
}

bool ErectusMemory::UpdateKnownRecipes()
{
	DWORD64 LocalPlayerPtr = GetLocalPlayerPtr(true);
	if (!Erectus::Valid(LocalPlayerPtr)) return false;

	int RecipeArraySize = 0;
	DWORD64* RecipeArray = GetRecipeArray(&RecipeArraySize);
	if (RecipeArray == nullptr) return false;

	size_t AllocSize = sizeof(ExecutionPlan) + (RecipeArraySize * sizeof(DWORD64) + (RecipeArraySize * sizeof(bool)));
	DWORD64 AllocAddress = Erectus::AllocEx(AllocSize);
	if (AllocAddress == 0)
	{
		delete[]RecipeArray;
		RecipeArray = nullptr;
		return false;
	}

	ExecutionPlan ExecutionPlanData;
	ExecutionPlanData.Function = AllocAddress + sizeof(ExecutionPlan::ASM);
	ExecutionPlanData.LocalPlayerPtr = LocalPlayerPtr;
	ExecutionPlanData.RecipeArraySize = RecipeArraySize;
	ExecutionPlanData.RecipeArray = AllocAddress + sizeof(ExecutionPlan);
	ExecutionPlanData.LearnedRecipeArray = AllocAddress + sizeof(ExecutionPlan) + RecipeArraySize * sizeof(DWORD64);

	BYTE* PageData = new BYTE[AllocSize];
	memset(PageData, 0x00, AllocSize);
	memcpy(PageData, &ExecutionPlanData, sizeof(ExecutionPlanData));

	for (int i = 0; i < RecipeArraySize; i++)
	{
		memcpy(&PageData[sizeof(ExecutionPlan) + i * sizeof(DWORD64)], &RecipeArray[i], sizeof(DWORD64));
	}

	bool Written = Erectus::WPM(AllocAddress, &*PageData, AllocSize);

	delete[]PageData;
	PageData = nullptr;

	if (!Written)
	{
		delete[]RecipeArray;
		RecipeArray = nullptr;

		Erectus::FreeEx(AllocAddress);
		return false;
	}

	DWORD64 ParamAddress = AllocAddress + sizeof(ExecutionPlan::ASM) + sizeof(ExecutionPlan::RBP);
	HANDLE Thread = CreateRemoteThread(ErectusProcess::Handle, NULL, 0, LPTHREAD_START_ROUTINE(AllocAddress), LPVOID(ParamAddress), 0, 0);

	if (Thread == NULL)
	{
		delete[]RecipeArray;
		RecipeArray = nullptr;

		Erectus::FreeEx(AllocAddress);
		return false;
	}

	DWORD ThreadResult = WaitForSingleObject(Thread, 3000);
	CloseHandle(Thread);

	if (ThreadResult == WAIT_TIMEOUT)
	{
		delete[]RecipeArray;
		RecipeArray = nullptr;
		return false;
	}

	bool* ExecutedPlan = new bool[RecipeArraySize];
	if (!Erectus::RPM(AllocAddress + sizeof(ExecutionPlan) + RecipeArraySize * sizeof(DWORD64), &*ExecutedPlan, RecipeArraySize * sizeof(bool)))
	{
		delete[]RecipeArray;
		RecipeArray = nullptr;

		delete[]ExecutedPlan;
		ExecutedPlan = nullptr;

		Erectus::FreeEx(AllocAddress);
		return false;
	}

	int KnownRecipeIndex = 0;

	for (int i = 0; i < RecipeArraySize; i++)
	{
		if (ExecutedPlan[i])
		{
			DWORD64 Buffer;
			if (Erectus::RPM(RecipeArray[i] + 0xB0, &Buffer, sizeof(Buffer)))
			{
				if (Erectus::Valid(Buffer))
				{
					DWORD Formid;
					if (Erectus::RPM(Buffer + 0x20, &Formid, sizeof(Formid)))
					{
						KnownRecipeArray[KnownRecipeIndex] = Formid;
						KnownRecipeIndex++;
					}
				}
			}
		}
	}

	KnownRecipeArraySize = KnownRecipeIndex;

	delete[]RecipeArray;
	RecipeArray = nullptr;

	delete[]ExecutedPlan;
	ExecutedPlan = nullptr;

	Erectus::FreeEx(AllocAddress);
	return true;
}

BYTE ErectusMemory::IsKnownRecipe(DWORD Formid)
{
	if (!KnownRecipeArraySize || KnownRecipeArraySize >= 0x1000)
	{
		return 0x00;
	}

	for (int i = 0; i < KnownRecipeArraySize; i++)
	{
		if (Formid == KnownRecipeArray[i])
		{
			return 0x01;
		}
	}

	return 0x02;
}

bool ErectusMemory::CheckFormidArray(DWORD Formid, bool* EnabledArray, DWORD* FormidArray, int Size)
{
	for (int i = 0; i < Size; i++)
	{
		if (Formid == FormidArray[i])
		{
			return EnabledArray[i];
		}
	}

	return false;
}

bool ErectusMemory::CheckReferenceJunk(Reference ReferenceData)
{
	if (ReferenceData.ComponentArraySize)
	{
		if (!((ReferenceData.RecordFlagA >> 7) & 1))
		{
			return true;
		}
	}

	return false;
}

bool ErectusMemory::CheckReferenceMod(Reference ReferenceData)
{
	if ((ReferenceData.RecordFlagA >> 7) & 1)
	{
		return true;
	}

	return false;
}

bool ErectusMemory::CheckReferencePlan(Reference ReferenceData)
{
	if ((ReferenceData.PlanFlag >> 5) & 1)
	{
		return true;
	}

	return false;
}

bool ErectusMemory::CheckScrapList()
{
	if (ErectusIni::CustomScrapLooterSettings.ScrapLooterDistance)
	{
		for (int i = 0; i < 40; i++)
		{
			if (ErectusIni::CustomScrapLooterSettings.ScrapEnabledList[i])
			{
				return true;
			}
		}
	}

	return false;
}

bool ErectusMemory::CheckItemLooterList()
{
	for (int i = 0; i < 100; i++)
	{
		if (ErectusIni::CustomItemLooterSettings.ItemLooterFormidList[i])
		{
			if (ErectusIni::CustomItemLooterSettings.ItemLooterEnabledList[i])
			{
				return true;
			}
		}
	}

	return false;
}

bool ErectusMemory::CheckItemLooterBlacklist()
{
	if (ErectusIni::CustomItemLooterSettings.ItemLooterBlacklistToggle)
	{
		for (int i = 0; i < 64; i++)
		{
			if (ErectusIni::CustomItemLooterSettings.ItemLooterBlacklist[i])
			{
				if (ErectusIni::CustomItemLooterSettings.ItemLooterBlacklistEnabled[i])
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool ErectusMemory::CheckEntityLooterList(EntityLooterSettings* CustomEntityLooterSettings)
{
	for (int i = 0; i < 100; i++)
	{
		if (CustomEntityLooterSettings->EntityLooterFormidList[i])
		{
			if (CustomEntityLooterSettings->EntityLooterEnabledList[i])
			{
				return true;
			}
		}
	}

	return false;
}

bool ErectusMemory::CheckEntityLooterBlacklist(EntityLooterSettings* CustomEntityLooterSettings)
{
	if (CustomEntityLooterSettings->EntityLooterBlacklistToggle)
	{
		for (int i = 0; i < 64; i++)
		{
			if (CustomEntityLooterSettings->EntityLooterBlacklist[i])
			{
				if (CustomEntityLooterSettings->EntityLooterBlacklistEnabled[i])
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool ErectusMemory::CheckIngredientList()
{
	for (int i = 0; i < 69; i++)
	{
		if (ErectusIni::CustomHarvesterSettings.HarvesterEnabledList[i])
		{
			return true;
		}
	}

	return false;
}

bool ErectusMemory::CheckJunkPileEnabled()
{
	for (int i = 0; i < 69; i++)
	{
		if (!strcmp(ErectusIni::CustomHarvesterSettings.HarvesterNameList[i], "Junk Pile"))
		{
			return ErectusIni::CustomHarvesterSettings.HarvesterEnabledList[i];
		}
	}

	return false;
}

bool ErectusMemory::CheckComponentArray(Reference ReferenceData)
{
	if (!ReferenceData.ComponentArraySize || ReferenceData.ComponentArraySize > 0x7FFF) return false;
	if (!Erectus::Valid(ReferenceData.ComponentArrayPtr)) return false;

	Component* ComponentArray = new Component[ReferenceData.ComponentArraySize];
	if (!Erectus::RPM(ReferenceData.ComponentArrayPtr, &*ComponentArray, ReferenceData.ComponentArraySize * sizeof(Component)))
	{
		delete[]ComponentArray;
		ComponentArray = nullptr;
		return false;
	}

	for (int i = 0; i < ReferenceData.ComponentArraySize; i++)
	{
		if (!Erectus::Valid(ComponentArray[i].ComponentReferencePtr)) continue;
		if (!Erectus::Valid(ComponentArray[i].ComponentCountReferencePtr)) continue;

		Reference ComponentData;
		if (!Erectus::RPM(ComponentArray[i].ComponentReferencePtr, &ComponentData, sizeof(ComponentData))) continue;
		if (CheckFormidArray(ComponentData.Formid, ErectusIni::CustomScrapLooterSettings.ScrapEnabledList, ErectusIni::CustomScrapLooterSettings.ScrapFormidList, 40))
		{
			delete[]ComponentArray;
			ComponentArray = nullptr;
			return true;
		}
	}

	delete[]ComponentArray;
	ComponentArray = nullptr;
	return false;
}

bool ErectusMemory::CheckReferenceKeywordBook(Reference ReferenceData, DWORD Formid)
{
	if (!ReferenceData.KeywordArrayData01C0 || ReferenceData.KeywordArrayData01C0 > 0x7FFF) return false;
	if (!Erectus::Valid(ReferenceData.KeywordArrayData01B8)) return false;

	DWORD64* KeywordArray = new DWORD64[ReferenceData.KeywordArrayData01C0];
	if (!Erectus::RPM(ReferenceData.KeywordArrayData01B8, &*KeywordArray, ReferenceData.KeywordArrayData01C0 * sizeof(DWORD64)))
	{
		delete[]KeywordArray;
		KeywordArray = nullptr;
		return false;
	}

	for (DWORD64 i = 0; i < ReferenceData.KeywordArrayData01C0; i++)
	{
		if (!Erectus::Valid(KeywordArray[i])) continue;

		DWORD FormidCheck;
		if (!Erectus::RPM(KeywordArray[i] + 0x20, &FormidCheck, sizeof(FormidCheck))) continue;
		if (FormidCheck != Formid) continue;

		delete[]KeywordArray;
		KeywordArray = nullptr;
		return true;
	}

	delete[]KeywordArray;
	KeywordArray = nullptr;
	return false;
}

bool ErectusMemory::CheckReferenceKeywordMisc(Reference ReferenceData, DWORD Formid)
{
	if (!ReferenceData.KeywordArrayData01B8 || ReferenceData.KeywordArrayData01B8 > 0x7FFF) return false;
	if (!Erectus::Valid(ReferenceData.KeywordArrayData01B0)) return false;

	DWORD64* KeywordArray = new DWORD64[ReferenceData.KeywordArrayData01B8];
	if (!Erectus::RPM(ReferenceData.KeywordArrayData01B0, &*KeywordArray, ReferenceData.KeywordArrayData01B8 * sizeof(DWORD64)))
	{
		delete[]KeywordArray;
		KeywordArray = nullptr;
		return false;
	}

	for (DWORD64 i = 0; i < ReferenceData.KeywordArrayData01B8; i++)
	{
		if (!Erectus::Valid(KeywordArray[i])) continue;

		DWORD FormidCheck;
		if (!Erectus::RPM(KeywordArray[i] + 0x20, &FormidCheck, sizeof(FormidCheck))) continue;
		if (FormidCheck != Formid) continue;

		delete[]KeywordArray;
		KeywordArray = nullptr;
		return true;
	}

	delete[]KeywordArray;
	KeywordArray = nullptr;
	return false;
}

bool ErectusMemory::CheckWhitelistedFlux(Reference ReferenceData)
{
	if (!Erectus::Valid(ReferenceData.HarvestedPtr)) return false;

	DWORD FormidCheck;
	if (!Erectus::RPM(ReferenceData.HarvestedPtr + 0x20, &FormidCheck, sizeof(FormidCheck))) return false;

	switch (FormidCheck)
	{
	case 0x002DDD45://Raw Crimson Flux
		return ErectusIni::CustomFluxSettings.CrimsonFluxEnabled;
	case 0x002DDD46://Raw Cobalt Flux
		return ErectusIni::CustomFluxSettings.CobaltFluxEnabled;
	case 0x002DDD49://Raw Yellowcake Flux
		return ErectusIni::CustomFluxSettings.YellowcakeFluxEnabled;
	case 0x002DDD4B://Raw Fluorescent Flux
		return ErectusIni::CustomFluxSettings.FluorescentFluxEnabled;
	case 0x002DDD4D://Raw Violet Flux
		return ErectusIni::CustomFluxSettings.VioletFluxEnabled;
	default:
		return false;
	}
}

bool ErectusMemory::FloraLeveledListValid(LeveledList LeveledListData)
{
	if (!Erectus::Valid(LeveledListData.ListEntryArrayPtr) || !LeveledListData.ListEntryArraySize)
	{
		return false;
	}

	ListEntry* ListEntryData = new ListEntry[LeveledListData.ListEntryArraySize];
	if (!Erectus::RPM(LeveledListData.ListEntryArrayPtr, &*ListEntryData, LeveledListData.ListEntryArraySize * sizeof(ListEntry)))
	{
		delete[]ListEntryData;
		ListEntryData = nullptr;
		return false;
	}

	for (BYTE i = 0; i < LeveledListData.ListEntryArraySize; i++)
	{
		if (!Erectus::Valid(ListEntryData[i].ReferencePtr))
		{
			continue;
		}

		Reference ReferenceData;
		if (!Erectus::RPM(ListEntryData[i].ReferencePtr, &ReferenceData, sizeof(ReferenceData))) continue;
		if (ReferenceData.vtable == ErectusProcess::Exe + VTABLE_TESLEVITEM)
		{
			LeveledList RecursiveLeveledListData;
			memcpy(&RecursiveLeveledListData, &ReferenceData, sizeof(RecursiveLeveledListData));
			if (FloraLeveledListValid(RecursiveLeveledListData))
			{
				delete[]ListEntryData;
				ListEntryData = nullptr;
				return true;
			}
		}
		else if (CheckFormidArray(ReferenceData.Formid, ErectusIni::CustomHarvesterSettings.HarvesterEnabledList, ErectusIni::CustomHarvesterSettings.HarvesterFormidList, 69))
		{
			delete[]ListEntryData;
			ListEntryData = nullptr;
			return true;
		}
	}

	delete[]ListEntryData;
	ListEntryData = nullptr;
	return false;
}

bool ErectusMemory::FloraValid(Reference ReferenceData)
{
	if (ReferenceData.Formid == 0x000183C6)
	{
		return CheckJunkPileEnabled();
	}

	if (!Erectus::Valid(ReferenceData.HarvestedPtr))
	{
		return false;
	}

	Reference HarvestedData;
	if (!Erectus::RPM(ReferenceData.HarvestedPtr, &HarvestedData, sizeof(HarvestedData))) return false;
	if (HarvestedData.vtable == ErectusProcess::Exe + VTABLE_TESLEVITEM)
	{
		LeveledList LeveledListData;
		memcpy(&LeveledListData, &HarvestedData, sizeof(LeveledListData));
		return FloraLeveledListValid(LeveledListData);
	}
	else
	{
		return CheckFormidArray(HarvestedData.Formid, ErectusIni::CustomHarvesterSettings.HarvesterEnabledList, ErectusIni::CustomHarvesterSettings.HarvesterFormidList, 69);
	}
}

bool ErectusMemory::IsTreasureMap(DWORD Formid)
{
	switch (Formid)
	{
	case 0x0051B8CD://Cranberry Bog Treasure Map #01
		return true;
	case 0x0051B8D6://Cranberry Bog Treasure Map #02
		return true;
	case 0x0051B8D9://Cranberry Bog Treasure Map #03
		return true;
	case 0x0051B8DE://Cranberry Bog Treasure Map #04
		return true;
	case 0x0051B8CE://Mire Treasure Map #01
		return true;
	case 0x0051B8D2://Mire Treasure Map #02
		return true;
	case 0x0051B8D7://Mire Treasure Map #03
		return true;
	case 0x0051B8D8://Mire Treasure Map #04
		return true;
	case 0x0051B8DB://Mire Treasure Map #05
		return true;
	case 0x0051B8BA://Savage Divide Treasure Map #01
		return true;
	case 0x0051B8C0://Savage Divide Treasure Map #02
		return true;
	case 0x0051B8C2://Savage Divide Treasure Map #03
		return true;
	case 0x0051B8C4://Savage Divide Treasure Map #04
		return true;
	case 0x0051B8C6://Savage Divide Treasure Map #05
		return true;
	case 0x0051B8C7://Savage Divide Treasure Map #06
		return true;
	case 0x0051B8C8://Savage Divide Treasure Map #07
		return true;
	case 0x0051B8CA://Savage Divide Treasure Map #08
		return true;
	case 0x0051B8CC://Savage Divide Treasure Map #09
		return true;
	case 0x0051B8D4://Savage Divide Treasure Map #10
		return true;
	case 0x0051B8B1://Toxic Valley Treasure Map #01
		return true;
	case 0x0051B8B8://Toxic Valley Treasure Map #02
		return true;
	case 0x0051B8BC://Toxic Valley Treasure Map #03
		return true;
	case 0x0051B8C1://Toxic Valley Treasure Map #04
		return true;
	case 0x0051B7A2://Forest Treasure Map #01
		return true;
	case 0x0051B8A6://Forest Treasure Map #02
		return true;
	case 0x0051B8A7://Forest Treasure Map #03
		return true;
	case 0x0051B8A9://Forest Treasure Map #04
		return true;
	case 0x0051B8AA://Forest Treasure Map #05
		return true;
	case 0x0051B8AE://Forest Treasure Map #06
		return true;
	case 0x0051B8B0://Forest Treasure Map #07
		return true;
	case 0x0051B8B2://Forest Treasure Map #08
		return true;
	case 0x0051B8B6://Forest Treasure Map #09
		return true;
	case 0x0051B8B9://Forest Treasure Map #10
		return true;
	case 0x0051B8A8://Ash Heap Treasure Map #01
		return true;
	case 0x0051B8AC://Ash Heap Treasure Map #02
		return true;
	default:
		return false;
	}
}

bool ErectusMemory::CheckFactionFormid(DWORD Formid)
{
	switch (Formid)
	{
	case 0x003FC008://W05_SettlerFaction
		return ErectusIni::CustomExtraNPCSettings.HideSettlerFaction;
	case 0x003FE94A://W05_CraterRaiderFaction
		return ErectusIni::CustomExtraNPCSettings.HideCraterRaiderFaction;
	case 0x003FBC00://W05_DieHardFaction
		return ErectusIni::CustomExtraNPCSettings.HideDieHardFaction;
	case 0x005427B2://W05_SecretServiceFaction
		return ErectusIni::CustomExtraNPCSettings.HideSecretServiceFaction;
	default:
		return false;
	}
}

bool ErectusMemory::BlacklistedNPCFaction(Reference ReferenceData)
{
	if (!Erectus::Valid(ReferenceData.FactionArrayPtr) || !ReferenceData.FactionArraySize || ReferenceData.FactionArraySize > 0x7FFF)
	{
		return false;
	}

	DWORD64* FactionArray = new DWORD64[ReferenceData.FactionArraySize * 2];
	if (!Erectus::RPM(ReferenceData.FactionArrayPtr, &*FactionArray, ReferenceData.FactionArraySize * 2 * sizeof(DWORD64)))
	{
		delete[]FactionArray;
		FactionArray = nullptr;
		return false;
	}

	bool BlacklistedFactionFound = false;
	for (int i = 0; i < ReferenceData.FactionArraySize; i++)
	{
		if (!Erectus::Valid(FactionArray[i * 2]))
		{
			continue;
		}

		DWORD Formid;
		if (!Erectus::RPM(FactionArray[i * 2] + 0x20, &Formid, sizeof(Formid))) continue;
		if (CheckFactionFormid(Formid))
		{
			BlacklistedFactionFound = true;
			break;
		}
	}

	delete[]FactionArray;
	FactionArray = nullptr;
	return BlacklistedFactionFound;
}

bool ErectusMemory::CheckReferenceItem(Reference ReferenceData)
{
	switch (ReferenceData.vtable - ErectusProcess::Exe)
	{
	case VTABLE_TESUTILITYITEM:
		return true;
	case VTABLE_BGSNOTE:
		return true;
	case VTABLE_TESKEY:
		return true;
	case VTABLE_CURRENCYOBJECT:
		return true;
	default:
		return false;
	}
}

void ErectusMemory::GetCustomEntityData(Reference ReferenceData, DWORD64* EntityFlag, DWORD64* EntityNamePtr, int* EnabledDistance, bool CheckScrap, bool CheckIngredient)
{
	switch (ReferenceData.vtable - ErectusProcess::Exe)
	{
	case VTABLE_TESNPC:
		*EntityFlag |= CUSTOM_ENTRY_NPC;
		*EntityNamePtr = ReferenceData.NamePtr0160;
		*EnabledDistance = ErectusIni::NpcSettings.EnabledDistance;
		break;
	case VTABLE_TESOBJECTCONT:
		*EntityFlag |= CUSTOM_ENTRY_CONTAINER;
		*EntityNamePtr = ReferenceData.NamePtr00B0;
		*EnabledDistance = ErectusIni::ContainerSettings.EnabledDistance;
		if (CheckFormidArray(ReferenceData.Formid, ErectusIni::ContainerSettings.Whitelisted, ErectusIni::ContainerSettings.Whitelist, 32))
		{
			*EntityFlag |= CUSTOM_ENTRY_WHITELISTED;
		}
		else if (!ErectusIni::ContainerSettings.Enabled)
		{
			*EntityFlag |= CUSTOM_ENTRY_INVALID;
		}
		break;
	case VTABLE_TESOBJECTMISC:
		*EntityFlag |= CUSTOM_ENTRY_VALID_ITEM;
		*EntityNamePtr = ReferenceData.NamePtr0098;
		if (CheckReferenceJunk(ReferenceData))
		{
			*EntityFlag |= CUSTOM_ENTRY_JUNK;
			*EnabledDistance = ErectusIni::JunkSettings.EnabledDistance;
			if (CheckScrap)
			{
				if (CheckComponentArray(ReferenceData))
				{
					*EntityFlag |= CUSTOM_ENTRY_VALID_SCRAP;
				}
				else
				{
					*EntityFlag |= CUSTOM_ENTRY_INVALID;
				}
			}
			else
			{
				if (CheckFormidArray(ReferenceData.Formid, ErectusIni::JunkSettings.Whitelisted, ErectusIni::JunkSettings.Whitelist, 32))
				{
					*EntityFlag |= CUSTOM_ENTRY_WHITELISTED;
				}
				else if (!ErectusIni::JunkSettings.Enabled)
				{
					*EntityFlag |= CUSTOM_ENTRY_INVALID;
				}
			}
		}
		else if (CheckReferenceMod(ReferenceData))
		{
			*EntityFlag |= CUSTOM_ENTRY_MOD;
			*EntityFlag |= CUSTOM_ENTRY_ITEM;
			*EnabledDistance = ErectusIni::ItemSettings.EnabledDistance;
			if (CheckFormidArray(ReferenceData.Formid, ErectusIni::ItemSettings.Whitelisted, ErectusIni::ItemSettings.Whitelist, 32))
			{
				*EntityFlag |= CUSTOM_ENTRY_WHITELISTED;
			}
			else if (!ErectusIni::ItemSettings.Enabled)
			{
				*EntityFlag |= CUSTOM_ENTRY_INVALID;
			}
		}
		else if (CheckReferenceKeywordMisc(ReferenceData, 0x00135E6C))
		{
			*EntityFlag |= CUSTOM_ENTRY_BOBBLEHEAD;
			*EnabledDistance = ErectusIni::BobbleheadSettings.EnabledDistance;
			if (CheckFormidArray(ReferenceData.Formid, ErectusIni::BobbleheadSettings.Whitelisted, ErectusIni::BobbleheadSettings.Whitelist, 32))
			{
				*EntityFlag |= CUSTOM_ENTRY_WHITELISTED;
			}
			else if (!ErectusIni::BobbleheadSettings.Enabled)
			{
				*EntityFlag |= CUSTOM_ENTRY_INVALID;
			}
		}
		else
		{
			*EntityFlag |= CUSTOM_ENTRY_MISC;
			*EntityFlag |= CUSTOM_ENTRY_ITEM;
			*EnabledDistance = ErectusIni::ItemSettings.EnabledDistance;
			if (CheckFormidArray(ReferenceData.Formid, ErectusIni::ItemSettings.Whitelisted, ErectusIni::ItemSettings.Whitelist, 32))
			{
				*EntityFlag |= CUSTOM_ENTRY_WHITELISTED;
			}
			else if (!ErectusIni::ItemSettings.Enabled)
			{
				*EntityFlag |= CUSTOM_ENTRY_INVALID;
			}
		}
		break;
	case VTABLE_TESOBJECTBOOK:
		*EntityFlag |= CUSTOM_ENTRY_VALID_ITEM;
		*EntityNamePtr = ReferenceData.NamePtr0098;
		if (CheckReferencePlan(ReferenceData))
		{
			*EntityFlag |= CUSTOM_ENTRY_PLAN;
			*EnabledDistance = ErectusIni::PlanSettings.EnabledDistance;
			switch (IsKnownRecipe(ReferenceData.Formid))
			{
			case 0x01:
				*EntityFlag |= CUSTOM_ENTRY_KNOWN_RECIPE;
				break;
			case 0x02:
				*EntityFlag |= CUSTOM_ENTRY_UNKNOWN_RECIPE;
				break;
			default:
				*EntityFlag |= CUSTOM_ENTRY_FAILED_RECIPE;
				break;
			}
			if (CheckFormidArray(ReferenceData.Formid, ErectusIni::PlanSettings.Whitelisted, ErectusIni::PlanSettings.Whitelist, 32))
			{
				*EntityFlag |= CUSTOM_ENTRY_WHITELISTED;
			}
			else if (!ErectusIni::PlanSettings.Enabled)
			{
				*EntityFlag |= CUSTOM_ENTRY_INVALID;
			}
		}
		else if (CheckReferenceKeywordBook(ReferenceData, 0x001D4A70))
		{
			*EntityFlag |= CUSTOM_ENTRY_MAGAZINE;
			*EnabledDistance = ErectusIni::MagazineSettings.EnabledDistance;
			if (CheckFormidArray(ReferenceData.Formid, ErectusIni::MagazineSettings.Whitelisted, ErectusIni::MagazineSettings.Whitelist, 32))
			{
				*EntityFlag |= CUSTOM_ENTRY_WHITELISTED;
			}
			else if (!ErectusIni::MagazineSettings.Enabled)
			{
				*EntityFlag |= CUSTOM_ENTRY_INVALID;
			}
		}
		else
		{
			*EntityFlag |= CUSTOM_ENTRY_ITEM;
			*EnabledDistance = ErectusIni::ItemSettings.EnabledDistance;
			if (IsTreasureMap(ReferenceData.Formid))
			{
				*EntityFlag |= CUSTOM_ENTRY_TREASURE_MAP;
			}
			if (CheckFormidArray(ReferenceData.Formid, ErectusIni::ItemSettings.Whitelisted, ErectusIni::ItemSettings.Whitelist, 32))
			{
				*EntityFlag |= CUSTOM_ENTRY_WHITELISTED;
			}
			else if (!ErectusIni::ItemSettings.Enabled)
			{
				*EntityFlag |= CUSTOM_ENTRY_INVALID;
			}
		}
		break;
	case VTABLE_TESFLORA:
		*EntityFlag |= CUSTOM_ENTRY_FLORA;
		*EntityNamePtr = ReferenceData.NamePtr0098;
		*EnabledDistance = ErectusIni::FloraSettings.EnabledDistance;
		if (CheckIngredient)
		{
			if (FloraValid(ReferenceData))
			{
				*EntityFlag |= CUSTOM_ENTRY_VALID_INGREDIENT;
			}
			else
			{
				*EntityFlag |= CUSTOM_ENTRY_INVALID;
			}
		}
		else
		{
			if (CheckWhitelistedFlux(ReferenceData))
			{
				*EntityFlag |= CUSTOM_ENTRY_WHITELISTED;
			}
			else if (CheckFormidArray(ReferenceData.Formid, ErectusIni::FloraSettings.Whitelisted, ErectusIni::FloraSettings.Whitelist, 32))
			{
				*EntityFlag |= CUSTOM_ENTRY_WHITELISTED;
			}
			else if (!ErectusIni::FloraSettings.Enabled)
			{
				*EntityFlag |= CUSTOM_ENTRY_INVALID;
			}
		}
		break;
	case VTABLE_TESOBJECTWEAP:
		*EntityFlag |= CUSTOM_ENTRY_WEAPON;
		*EntityFlag |= CUSTOM_ENTRY_ITEM;
		*EntityFlag |= CUSTOM_ENTRY_VALID_ITEM;
		*EntityNamePtr = ReferenceData.NamePtr0098;
		*EnabledDistance = ErectusIni::ItemSettings.EnabledDistance;
		if (CheckFormidArray(ReferenceData.Formid, ErectusIni::ItemSettings.Whitelisted, ErectusIni::ItemSettings.Whitelist, 32))
		{
			*EntityFlag |= CUSTOM_ENTRY_WHITELISTED;
		}
		else if (!ErectusIni::ItemSettings.Enabled)
		{
			*EntityFlag |= CUSTOM_ENTRY_INVALID;
		}
		break;
	case VTABLE_TESOBJECTARMO:
		*EntityFlag |= CUSTOM_ENTRY_ARMOR;
		*EntityFlag |= CUSTOM_ENTRY_ITEM;
		*EntityFlag |= CUSTOM_ENTRY_VALID_ITEM;
		*EntityNamePtr = ReferenceData.NamePtr0098;
		*EnabledDistance = ErectusIni::ItemSettings.EnabledDistance;
		if (CheckFormidArray(ReferenceData.Formid, ErectusIni::ItemSettings.Whitelisted, ErectusIni::ItemSettings.Whitelist, 32))
		{
			*EntityFlag |= CUSTOM_ENTRY_WHITELISTED;
		}
		else if (!ErectusIni::ItemSettings.Enabled)
		{
			*EntityFlag |= CUSTOM_ENTRY_INVALID;
		}
		break;
	case VTABLE_TESAMMO:
		*EntityFlag |= CUSTOM_ENTRY_AMMO;
		*EntityFlag |= CUSTOM_ENTRY_ITEM;
		*EntityFlag |= CUSTOM_ENTRY_VALID_ITEM;
		*EntityNamePtr = ReferenceData.NamePtr0098;
		*EnabledDistance = ErectusIni::ItemSettings.EnabledDistance;
		if (CheckFormidArray(ReferenceData.Formid, ErectusIni::ItemSettings.Whitelisted, ErectusIni::ItemSettings.Whitelist, 32))
		{
			*EntityFlag |= CUSTOM_ENTRY_WHITELISTED;
		}
		else if (!ErectusIni::ItemSettings.Enabled)
		{
			*EntityFlag |= CUSTOM_ENTRY_INVALID;
		}
		break;
	case VTABLE_ALCHEMYITEM:
		*EntityFlag |= CUSTOM_ENTRY_AID;
		*EntityFlag |= CUSTOM_ENTRY_ITEM;
		*EntityFlag |= CUSTOM_ENTRY_VALID_ITEM;
		*EntityNamePtr = ReferenceData.NamePtr0098;
		*EnabledDistance = ErectusIni::ItemSettings.EnabledDistance;
		if (CheckFormidArray(ReferenceData.Formid, ErectusIni::ItemSettings.Whitelisted, ErectusIni::ItemSettings.Whitelist, 32))
		{
			*EntityFlag |= CUSTOM_ENTRY_WHITELISTED;
		}
		else if (!ErectusIni::ItemSettings.Enabled)
		{
			*EntityFlag |= CUSTOM_ENTRY_INVALID;
		}
		break;
	default:
		if (CheckReferenceItem(ReferenceData))
		{
			*EntityFlag |= CUSTOM_ENTRY_ITEM;
			*EntityFlag |= CUSTOM_ENTRY_VALID_ITEM;
			*EntityNamePtr = ReferenceData.NamePtr0098;
			*EnabledDistance = ErectusIni::ItemSettings.EnabledDistance;
			if (CheckFormidArray(ReferenceData.Formid, ErectusIni::ItemSettings.Whitelisted, ErectusIni::ItemSettings.Whitelist, 32))
			{
				*EntityFlag |= CUSTOM_ENTRY_WHITELISTED;
			}
			else if (!ErectusIni::ItemSettings.Enabled)
			{
				*EntityFlag |= CUSTOM_ENTRY_INVALID;
			}
		}
		else
		{
			*EntityFlag |= CUSTOM_ENTRY_ENTITY;
			*EntityNamePtr = 0;
			*EnabledDistance = ErectusIni::EntitySettings.EnabledDistance;
			if (CheckFormidArray(ReferenceData.Formid, ErectusIni::EntitySettings.Whitelisted, ErectusIni::EntitySettings.Whitelist, 32))
			{
				*EntityFlag |= CUSTOM_ENTRY_WHITELISTED;
			}
			else if (!ErectusIni::EntitySettings.Enabled)
			{
				*EntityFlag |= CUSTOM_ENTRY_INVALID;
			}
			else if (!ErectusIni::EntitySettings.DrawUnnamed)
			{
				*EntityFlag |= CUSTOM_ENTRY_INVALID;
			}
		}
		break;
	}
}

bool ErectusMemory::GetActorSnapshotComponentData(Entity EntityData, ActorSnapshotComponent* ActorSnapshotComponentData)
{
	if (!Erectus::Valid(EntityData.ActorCorePtr)) return false;

	DWORD64 ActorCoreBufferA;
	if (!Erectus::RPM(EntityData.ActorCorePtr + 0x70, &ActorCoreBufferA, sizeof(ActorCoreBufferA))) return false;
	if (!Erectus::Valid(ActorCoreBufferA)) return false;

	DWORD64 ActorCoreBufferB;
	if (!Erectus::RPM(ActorCoreBufferA + 0x8, &ActorCoreBufferB, sizeof(ActorCoreBufferB))) return false;
	if (!Erectus::Valid(ActorCoreBufferB)) return false;

	return Erectus::RPM(ActorCoreBufferB, &*ActorSnapshotComponentData, sizeof(ActorSnapshotComponent));
}

char* ErectusMemory::GetEntityName(DWORD64 Ptr)
{
	if (!Erectus::Valid(Ptr))
	{
		return nullptr;
	}

	int NameLength = 0;
	DWORD64 NamePtr = Ptr;

	if (!Erectus::RPM(NamePtr + 0x10, &NameLength, sizeof(NameLength))) return nullptr;
	if (NameLength <= 0 || NameLength > 0x7FFF)
	{
		DWORD64 Buffer;
		if (!Erectus::RPM(NamePtr + 0x10, &Buffer, sizeof(Buffer))) return nullptr;
		if (!Erectus::Valid(Buffer)) return nullptr;

		if (!Erectus::RPM(Buffer + 0x10, &NameLength, sizeof(NameLength))) return nullptr;
		if (NameLength <= 0 || NameLength > 0x7FFF) return nullptr;
		NamePtr = Buffer;
	}

	int NameSize = NameLength + 1;
	char* Name = new char[NameSize];

	if (!Erectus::RPM(NamePtr + 0x18, &*Name, NameSize))
	{
		delete[]Name;
		Name = nullptr;
		return nullptr;
	}

	if (Erectus::GetTextLength(Name) != NameLength)
	{
		delete[]Name;
		Name = nullptr;
		return nullptr;
	}

	return Name;
}

bool ErectusMemory::UpdateBufferEntityList()
{
	DWORD64 LocalPlayerPtr = GetLocalPlayerPtr(true);
	if (!Erectus::Valid(LocalPlayerPtr)) return false;

	Entity LocalPlayer;
	if (!Erectus::RPM(LocalPlayerPtr, &LocalPlayer, sizeof(LocalPlayer))) return false;

	int BufferSize = 0;
	DWORD64* BufferList = GetEntityList(&BufferSize);
	if (BufferList == nullptr) return false;

	int CustomListSize = 0;
	CustomEntry* CustomList = new CustomEntry[BufferSize];
	int CustomListIndex = 0;

	for (int i = 0; i < BufferSize; i++)
	{
		if (!Erectus::Valid(BufferList[i])) continue;
		if (BufferList[i] == LocalPlayerPtr) continue;

		Entity EntityData;
		if (!Erectus::RPM(BufferList[i], &EntityData, sizeof(EntityData))) continue;
		if (!Erectus::Valid(EntityData.ReferencePtr)) continue;

		Reference ReferenceData;
		if (!Erectus::RPM(EntityData.ReferencePtr, &ReferenceData, sizeof(ReferenceData))) continue;
		if (ReferenceData.vtable == ErectusProcess::Exe + VTABLE_TESNPC) continue;

		DWORD64 EntityFlag = CUSTOM_ENTRY_DEFAULT;
		DWORD64 EntityNamePtr = 0;
		int EnabledDistance = 0;

		GetCustomEntityData(ReferenceData, &EntityFlag, &EntityNamePtr, &EnabledDistance, ErectusIni::CustomScrapLooterSettings.ScrapOverrideEnabled, ErectusIni::CustomHarvesterSettings.HarvesterOverrideEnabled);
		if (EntityFlag & CUSTOM_ENTRY_INVALID) continue;

		float Distance = Erectus::GetDistance(EntityData.Position, LocalPlayer.Position);
		int NormalDistance = int(Distance * 0.01f);
		if (NormalDistance > EnabledDistance) continue;

		CustomListSize++;
		CustomList[CustomListIndex].EntityPtr = BufferList[i];
		CustomList[CustomListIndex].ReferencePtr = EntityData.ReferencePtr;
		CustomList[CustomListIndex].EntityFormid = EntityData.Formid;
		CustomList[CustomListIndex].ReferenceFormid = ReferenceData.Formid;
		CustomList[CustomListIndex].Flag = EntityFlag;
		CustomList[CustomListIndex].Name = GetEntityName(EntityNamePtr);
		CustomListIndex++;
	}

	bool ReturnValue = false;

	if (CustomListSize)
	{
		BufferEntityList = new CustomEntry[CustomListSize];
		memcpy(&*BufferEntityList, &*CustomList, CustomListSize * sizeof(CustomEntry));
		BufferEntityListSize = CustomListSize;
		ReturnValue = true;
	}

	delete[]CustomList;
	CustomList = nullptr;

	delete[]BufferList;
	BufferList = nullptr;

	return ReturnValue;
}

bool ErectusMemory::UpdateBufferNPCList()
{
	DWORD64 LocalPlayerPtr = GetLocalPlayerPtr(true);
	if (!Erectus::Valid(LocalPlayerPtr)) return false;

	Entity LocalPlayer;
	if (!Erectus::RPM(LocalPlayerPtr, &LocalPlayer, sizeof(LocalPlayer))) return false;

	int BufferSize = 0;
	DWORD64* BufferList = GetNPCList(&BufferSize);
	if (BufferList == nullptr) return false;

	int CustomListSize = 0;
	CustomEntry* CustomList = new CustomEntry[BufferSize];
	int CustomListIndex = 0;

	for (int i = 0; i < BufferSize; i++)
	{
		if (!Erectus::Valid(BufferList[i])) continue;
		if (BufferList[i] == LocalPlayerPtr) continue;

		Entity EntityData;
		if (!Erectus::RPM(BufferList[i], &EntityData, sizeof(EntityData))) continue;
		if (!Erectus::Valid(EntityData.ReferencePtr)) continue;

		Reference ReferenceData;
		if (!Erectus::RPM(EntityData.ReferencePtr, &ReferenceData, sizeof(ReferenceData))) continue;
		if (ReferenceData.vtable != ErectusProcess::Exe + VTABLE_TESNPC) continue;
		if (ReferenceData.Formid == 0x00000007) continue;

		DWORD64 EntityFlag = CUSTOM_ENTRY_DEFAULT;
		DWORD64 EntityNamePtr = 0;
		int EnabledDistance = 0;

		GetCustomEntityData(ReferenceData, &EntityFlag, &EntityNamePtr, &EnabledDistance, ErectusIni::CustomScrapLooterSettings.ScrapOverrideEnabled, ErectusIni::CustomHarvesterSettings.HarvesterOverrideEnabled);
		if (EntityFlag & CUSTOM_ENTRY_INVALID) continue;

		if (BlacklistedNPCFaction(ReferenceData))
		{
			continue;
		}

		if (ErectusIni::CustomExtraNPCSettings.UseNPCBlacklist)
		{
			if (CheckFormidArray(ReferenceData.Formid, ErectusIni::CustomExtraNPCSettings.NPCBlacklistEnabled, ErectusIni::CustomExtraNPCSettings.NPCBlacklist, 64))
			{
				continue;
			}
		}

		float Distance = Erectus::GetDistance(EntityData.Position, LocalPlayer.Position);
		int NormalDistance = int(Distance * 0.01f);
		if (NormalDistance > EnabledDistance) continue;

		CustomListSize++;
		CustomList[CustomListIndex].EntityPtr = BufferList[i];
		CustomList[CustomListIndex].ReferencePtr = EntityData.ReferencePtr;
		CustomList[CustomListIndex].EntityFormid = EntityData.Formid;
		CustomList[CustomListIndex].ReferenceFormid = ReferenceData.Formid;
		CustomList[CustomListIndex].Flag = EntityFlag;
		CustomList[CustomListIndex].Name = GetEntityName(EntityNamePtr);
		CustomListIndex++;
	}

	bool ReturnValue = false;

	if (CustomListSize)
	{
		BufferNPCList = new CustomEntry[CustomListSize];
		memcpy(&*BufferNPCList, &*CustomList, CustomListSize * sizeof(CustomEntry));
		BufferNPCListSize = CustomListSize;
		ReturnValue = true;
	}

	delete[]CustomList;
	CustomList = nullptr;

	delete[]BufferList;
	BufferList = nullptr;

	return ReturnValue;
}

char* ErectusMemory::GetPlayerName(ClientAccount* ClientAccountData)
{
	if (!ClientAccountData->NameLength)
	{
		return nullptr;
	}

	int PlayerNameSize = ClientAccountData->NameLength + 1;
	char* PlayerName = new char[PlayerNameSize];

	if (ClientAccountData->NameLength > 15)
	{
		DWORD64 Buffer;
		memcpy(&Buffer, ClientAccountData->NameData, sizeof(Buffer));
		if (!Erectus::Valid(Buffer))
		{
			delete[]PlayerName;
			PlayerName = nullptr;
			return nullptr;
		}

		if (!Erectus::RPM(Buffer, &*PlayerName, PlayerNameSize))
		{
			delete[]PlayerName;
			PlayerName = nullptr;
			return nullptr;
		}
	}
	else
	{
		memcpy(PlayerName, ClientAccountData->NameData, PlayerNameSize);
	}

	if (Erectus::GetTextLength(PlayerName) != ClientAccountData->NameLength)
	{
		delete[]PlayerName;
		PlayerName = nullptr;
		return nullptr;
	}

	return PlayerName;
}

bool ErectusMemory::UpdateBufferPlayerList()
{
	DWORD64 LocalPlayerPtr = GetLocalPlayerPtr(true);
	if (!Erectus::Valid(LocalPlayerPtr)) return false;

	DWORD64 FalloutMainDataPtr;
	if (!Erectus::RPM(ErectusProcess::Exe + OFFSET_MAIN, &FalloutMainDataPtr, sizeof(FalloutMainDataPtr))) return false;
	if (!Erectus::Valid(FalloutMainDataPtr)) return false;

	FalloutMain FalloutMainData;
	if (!Erectus::RPM(FalloutMainDataPtr, &FalloutMainData, sizeof(FalloutMainData))) return false;
	if (!Erectus::Valid(FalloutMainData.PlatformSessionManagerPtr)) return false;

	PlatformSessionManager PlatformSessionManagerData;
	if (!Erectus::RPM(FalloutMainData.PlatformSessionManagerPtr, &PlatformSessionManagerData, sizeof(PlatformSessionManagerData))) return false;
	if (!Erectus::Valid(PlatformSessionManagerData.ClientAccountManagerPtr)) return false;

	ClientAccountManager ClientAccountManagerData;
	if (!Erectus::RPM(PlatformSessionManagerData.ClientAccountManagerPtr, &ClientAccountManagerData, sizeof(ClientAccountManagerData))) return false;
	if (!Erectus::Valid(ClientAccountManagerData.ClientAccountArrayPtr)) return false;

	int ClientAccountArraySize = 0;
	ClientAccountArraySize += ClientAccountManagerData.ClientAccountArraySizeA;
	ClientAccountArraySize += ClientAccountManagerData.ClientAccountArraySizeB;
	if (!ClientAccountArraySize) return false;

	DWORD64* ClientAccountArray = new DWORD64[ClientAccountArraySize];
	if (!Erectus::RPM(ClientAccountManagerData.ClientAccountArrayPtr, &*ClientAccountArray, ClientAccountArraySize * sizeof(DWORD64)))
	{
		delete[]ClientAccountArray;
		ClientAccountArray = nullptr;
		return false;
	}

	int CustomListSize = 0;
	CustomEntry* CustomList = new CustomEntry[ClientAccountArraySize];
	int CustomListIndex = 0;

	CustomListSize = 0;
	CustomListIndex = 0;

	for (int i = 0; i < ClientAccountArraySize; i++)
	{
		if (!Erectus::Valid(ClientAccountArray[i])) continue;

		ClientAccountBuffer ClientAccountBufferData;
		if (!Erectus::RPM(ClientAccountArray[i], &ClientAccountBufferData, sizeof(ClientAccountBufferData))) continue;
		if (!Erectus::Valid(ClientAccountBufferData.ClientAccountPtr)) continue;

		ClientAccount ClientAccountData;
		if (!Erectus::RPM(ClientAccountBufferData.ClientAccountPtr, &ClientAccountData, sizeof(ClientAccountData))) continue;
		if (ClientAccountData.Formid == 0xFFFFFFFF) continue;

		DWORD64 EntityPtr = GetPtr(ClientAccountData.Formid);
		if (!Erectus::Valid(EntityPtr)) continue;
		if (EntityPtr == LocalPlayerPtr) continue;

		Entity EntityData;
		if (!Erectus::RPM(EntityPtr, &EntityData, sizeof(EntityData))) continue;
		if (!Erectus::Valid(EntityData.ReferencePtr)) continue;

		Reference ReferenceData;
		if (!Erectus::RPM(EntityData.ReferencePtr, &ReferenceData, sizeof(ReferenceData))) continue;
		if (ReferenceData.Formid != 0x00000007) continue;

		CustomListSize++;
		CustomList[CustomListIndex].EntityPtr = EntityPtr;
		CustomList[CustomListIndex].ReferencePtr = EntityData.ReferencePtr;
		CustomList[CustomListIndex].EntityFormid = EntityData.Formid;
		CustomList[CustomListIndex].ReferenceFormid = ReferenceData.Formid;
		CustomList[CustomListIndex].Flag = CUSTOM_ENTRY_PLAYER;
		CustomList[CustomListIndex].Name = GetPlayerName(&ClientAccountData);
		CustomListIndex++;
	}

	bool ReturnValue = false;

	if (CustomListSize)
	{
		BufferPlayerList = new CustomEntry[CustomListSize];
		memcpy(&*BufferPlayerList, &*CustomList, CustomListSize * sizeof(CustomEntry));
		BufferPlayerListSize = CustomListSize;
		ReturnValue = true;
	}

	delete[]CustomList;
	CustomList = nullptr;

	delete[]ClientAccountArray;
	ClientAccountArray = nullptr;

	return ReturnValue;
}

void ErectusMemory::DeleteCustomEntityList()
{
	if (CustomEntityList != nullptr)
	{
		if (CustomEntityListSize)
		{
			for (int i = 0; i < CustomEntityListSize; i++)
			{
				if (CustomEntityList[i].Name != nullptr)
				{
					delete[]CustomEntityList[i].Name;
					CustomEntityList[i].Name = nullptr;
				}
			}
		}

		delete[]CustomEntityList;
		CustomEntityList = nullptr;
	}

	CustomEntityListSize = 0;
	CustomEntityListUpdated = false;
}

void ErectusMemory::DeleteBufferEntityList()
{
	if (BufferEntityList != nullptr)
	{
		if (BufferEntityListSize)
		{
			for (int i = 0; i < BufferEntityListSize; i++)
			{
				if (BufferEntityList[i].Name != nullptr)
				{
					delete[]BufferEntityList[i].Name;
					BufferEntityList[i].Name = nullptr;
				}
			}
		}

		delete[]BufferEntityList;
		BufferEntityList = nullptr;
	}

	BufferEntityListSize = 0;
	BufferEntityListUpdated = false;
}

void ErectusMemory::DeleteCustomNPCList()
{
	if (CustomNPCList != nullptr)
	{
		if (CustomNPCListSize)
		{
			for (int i = 0; i < CustomNPCListSize; i++)
			{
				if (CustomNPCList[i].Name != nullptr)
				{
					delete[]CustomNPCList[i].Name;
					CustomNPCList[i].Name = nullptr;
				}
			}
		}

		delete[]CustomNPCList;
		CustomNPCList = nullptr;
	}

	CustomNPCListSize = 0;
	CustomNPCListUpdated = false;
}

void ErectusMemory::DeleteBufferNPCList()
{
	if (BufferNPCList != nullptr)
	{
		if (BufferNPCListSize)
		{
			for (int i = 0; i < BufferNPCListSize; i++)
			{
				if (BufferNPCList[i].Name != nullptr)
				{
					delete[]BufferNPCList[i].Name;
					BufferNPCList[i].Name = nullptr;
				}
			}
		}

		delete[]BufferNPCList;
		BufferNPCList = nullptr;
	}

	BufferNPCListSize = 0;
	BufferNPCListUpdated = false;
}

void ErectusMemory::DeleteCustomPlayerList()
{
	if (CustomPlayerList != nullptr)
	{
		if (CustomPlayerListSize)
		{
			for (int i = 0; i < CustomPlayerListSize; i++)
			{
				if (CustomPlayerList[i].Name != nullptr)
				{
					delete[]CustomPlayerList[i].Name;
					CustomPlayerList[i].Name = nullptr;
				}
			}
		}

		delete[]CustomPlayerList;
		CustomPlayerList = nullptr;
	}

	CustomPlayerListSize = 0;
	CustomPlayerListUpdated = false;
}

void ErectusMemory::DeleteBufferPlayerList()
{
	if (BufferPlayerList != nullptr)
	{
		if (BufferPlayerListSize)
		{
			for (int i = 0; i < BufferPlayerListSize; i++)
			{
				if (BufferPlayerList[i].Name != nullptr)
				{
					delete[]BufferPlayerList[i].Name;
					BufferPlayerList[i].Name = nullptr;
				}
			}
		}

		delete[]BufferPlayerList;
		BufferPlayerList = nullptr;
	}

	BufferPlayerListSize = 0;
	BufferPlayerListUpdated = false;
}

bool ErectusMemory::TargetValid(Entity EntityData, Reference ReferenceData)
{
	if (ReferenceData.vtable != ErectusProcess::Exe + VTABLE_TESNPC)
	{
		return false;
	}

	if (EntityData.SpawnFlag != 0x02)
	{
		if (!ErectusIni::CustomTargetSettings.IgnoreRenderDistance)
		{
			return false;
		}
	}

	ActorSnapshotComponent ActorSnapshotComponentData;
	if (GetActorSnapshotComponentData(EntityData, &ActorSnapshotComponentData))
	{
		if (ErectusIni::CustomTargetSettings.IgnoreEssentialNPCs)
		{
			if (ActorSnapshotComponentData.IsEssential)
			{
				return false;
			}
		}
	}

	switch (CheckHealthFlag(EntityData.HealthFlag))
	{
	case 0x01://Alive
		return ErectusIni::CustomTargetSettings.TargetLiving;
	case 0x02://Downed
		return ErectusIni::CustomTargetSettings.TargetDowned;
	case 0x03://Dead
		return ErectusIni::CustomTargetSettings.TargetDead;
	default://Unknown
		return ErectusIni::CustomTargetSettings.TargetUnknown;
	}
}

bool ErectusMemory::FloraHarvested(BYTE HarvestFlagA, BYTE HarvestFlagB)
{
	bool HarvestedA = false;
	if ((HarvestFlagA >> 5) & 1)
	{
		HarvestedA = true;
	}

	bool HarvestedB = false;
	if ((HarvestFlagB >> 5) & 1)
	{
		HarvestedB = true;
	}

	if (HarvestedA || HarvestedB)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool ErectusMemory::RenderCustomEntryA(CustomEntry* CustomEntryData, OverlaySettingsA CustomSettingsA)
{
	int Health = -1;
	BYTE EpicRank = 0;
	bool AllowNPC = false;
	if (CustomEntryData->Flag & CUSTOM_ENTRY_NPC)
	{
		Entity NPCData;
		if (!Erectus::RPM(CustomEntryData->EntityPtr, &NPCData, sizeof(NPCData))) return false;

		ActorSnapshotComponent ActorSnapshotComponentData;
		if (GetActorSnapshotComponentData(NPCData, &ActorSnapshotComponentData))
		{
			Health = int(ActorSnapshotComponentData.MaxHealth + ActorSnapshotComponentData.ModifiedHealth + ActorSnapshotComponentData.LostHealth);
			EpicRank = ActorSnapshotComponentData.EpicRank;
			if (EpicRank)
			{
				switch (CheckHealthFlag(NPCData.HealthFlag))
				{
				case 0x01://Alive
					switch (EpicRank)
					{
					case 1:
						AllowNPC = ErectusIni::CustomLegendarySettings.OverrideLivingOneStar;
						break;
					case 2:
						AllowNPC = ErectusIni::CustomLegendarySettings.OverrideLivingTwoStar;
						break;
					case 3:
						AllowNPC = ErectusIni::CustomLegendarySettings.OverrideLivingThreeStar;
						break;
					default:
						break;
					}
					break;
				case 0x02://Downed
					switch (EpicRank)
					{
					case 1:
						AllowNPC = ErectusIni::CustomLegendarySettings.OverrideLivingOneStar;
						break;
					case 2:
						AllowNPC = ErectusIni::CustomLegendarySettings.OverrideLivingTwoStar;
						break;
					case 3:
						AllowNPC = ErectusIni::CustomLegendarySettings.OverrideLivingThreeStar;
						break;
					default:
						break;
					}
					break;
				case 0x03://Dead
					switch (EpicRank)
					{
					case 1:
						AllowNPC = ErectusIni::CustomLegendarySettings.OverrideDeadOneStar;
						break;
					case 2:
						AllowNPC = ErectusIni::CustomLegendarySettings.OverrideDeadTwoStar;
						break;
					case 3:
						AllowNPC = ErectusIni::CustomLegendarySettings.OverrideDeadThreeStar;
						break;
					default:
						break;
					}
					break;
				default:
					break;
				}
			}
		}
	}

	if (!CustomSettingsA.Enabled)
	{
		if (!AllowNPC)
		{
			return false;
		}
	}

	if (!CustomSettingsA.DrawEnabled && !CustomSettingsA.DrawDisabled)
	{
		return false;
	}

	if (CustomSettingsA.EnabledAlpha == 0.0f && CustomSettingsA.DisabledAlpha == 0.0f)
	{
		return false;
	}

	if (!CustomSettingsA.DrawNamed && !CustomSettingsA.DrawUnnamed)
	{
		return false;
	}

	Entity EntityData;
	if (!Erectus::RPM(CustomEntryData->EntityPtr, &EntityData, sizeof(EntityData))) return false;

	if (CustomEntryData->Flag & CUSTOM_ENTRY_PLAYER)
	{
		ActorSnapshotComponent ActorSnapshotComponentData;
		if (GetActorSnapshotComponentData(EntityData, &ActorSnapshotComponentData))
		{
			Health = int(ActorSnapshotComponentData.MaxHealth + ActorSnapshotComponentData.ModifiedHealth + ActorSnapshotComponentData.LostHealth);
		}
	}

	if (CustomEntryData->Name == nullptr)
	{
		CustomEntryData->Flag |= CUSTOM_ENTRY_UNNAMED;
		CustomEntryData->Name = new char[sizeof("00000000")];
		sprintf_s(CustomEntryData->Name, sizeof("00000000"), "%08lX", EntityData.Formid);
	}

	if (CustomEntryData->Flag & CUSTOM_ENTRY_UNNAMED)
	{
		if (!CustomSettingsA.DrawUnnamed) return false;
	}
	else
	{
		if (!CustomSettingsA.DrawNamed) return false;
	}

	float* Alpha = nullptr;

	if (EntityData.SpawnFlag == 0x02)
	{
		if (CustomSettingsA.DrawEnabled)
		{
			Alpha = &CustomSettingsA.EnabledAlpha;
		}
	}
	else
	{
		if (CustomSettingsA.DrawDisabled)
		{
			Alpha = &CustomSettingsA.DisabledAlpha;
		}
	}

	if (Alpha == nullptr)
	{
		return false;
	}

	bool ShowHealthText = false;

	float* Color = nullptr;

	float LegendaryAlpha = 1.0f;

	switch (CheckHealthFlag(EntityData.HealthFlag))
	{
	case 0x01://Alive
		ShowHealthText = CustomSettingsA.ShowHealth;
		if (AllowNPC)
		{
			switch (EpicRank)
			{
			case 1:
				Color = ErectusIni::CustomLegendarySettings.LivingOneStarColor;
				if (EntityData.SpawnFlag == 0x02)
				{
					Alpha = &LegendaryAlpha;
				}
				break;
			case 2:
				Color = ErectusIni::CustomLegendarySettings.LivingTwoStarColor;
				if (EntityData.SpawnFlag == 0x02)
				{
					Alpha = &LegendaryAlpha;
				}
				break;
			case 3:
				Color = ErectusIni::CustomLegendarySettings.LivingThreeStarColor;
				if (EntityData.SpawnFlag == 0x02)
				{
					Alpha = &LegendaryAlpha;
				}
				break;
			default:
				break;
			}
		}
		else if (CustomSettingsA.DrawAlive)
		{
			Color = CustomSettingsA.AliveColor;
		}
		break;
	case 0x02://Downed
		ShowHealthText = CustomSettingsA.ShowHealth;
		if (AllowNPC)
		{
			switch (EpicRank)
			{
			case 1:
				Color = ErectusIni::CustomLegendarySettings.LivingOneStarColor;
				if (EntityData.SpawnFlag == 0x02)
				{
					Alpha = &LegendaryAlpha;
				}
				break;
			case 2:
				Color = ErectusIni::CustomLegendarySettings.LivingTwoStarColor;
				if (EntityData.SpawnFlag == 0x02)
				{
					Alpha = &LegendaryAlpha;
				}
				break;
			case 3:
				Color = ErectusIni::CustomLegendarySettings.LivingThreeStarColor;
				if (EntityData.SpawnFlag == 0x02)
				{
					Alpha = &LegendaryAlpha;
				}
				break;
			default:
				break;
			}
		}
		else if (CustomSettingsA.DrawDowned)
		{
			Color = CustomSettingsA.DownedColor;
		}
		break;
	case 0x03://Dead
		ShowHealthText = CustomSettingsA.ShowDeadHealth;
		if (AllowNPC)
		{
			switch (EpicRank)
			{
			case 1:
				Color = ErectusIni::CustomLegendarySettings.DeadOneStarColor;
				if (EntityData.SpawnFlag == 0x02)
				{
					Alpha = &LegendaryAlpha;
				}
				break;
			case 2:
				Color = ErectusIni::CustomLegendarySettings.DeadTwoStarColor;
				if (EntityData.SpawnFlag == 0x02)
				{
					Alpha = &LegendaryAlpha;
				}
				break;
			case 3:
				Color = ErectusIni::CustomLegendarySettings.DeadThreeStarColor;
				if (EntityData.SpawnFlag == 0x02)
				{
					Alpha = &LegendaryAlpha;
				}
				break;
			default:
				break;
			}
		}
		else if (CustomSettingsA.DrawDead)
		{
			Color = CustomSettingsA.DeadColor;
		}
		break;
	default://Unknown
		ShowHealthText = CustomSettingsA.ShowHealth;
		if (CustomSettingsA.DrawUnknown)
		{
			Color = CustomSettingsA.UnknownColor;
		}
		break;
	}

	if (Color == nullptr)
	{
		return false;
	}

	DWORD64 CameraPtr = GetCameraPtr();
	if (!Erectus::Valid(CameraPtr)) return false;

	Camera CameraData;
	if (!Erectus::RPM(CameraPtr, &CameraData, sizeof(CameraData))) return false;

	float Distance = Erectus::GetDistance(EntityData.Position, CameraData.Origin);
	int NormalDistance = int(Distance * 0.01f);
	if (NormalDistance > CustomSettingsA.EnabledDistance) return false;

	bool ValidPlayer = false;
	if ((CustomEntryData->Flag & CUSTOM_ENTRY_PLAYER) && ErectusIni::CustomTargetSettings.LockPlayers)
	{
		ValidPlayer = true;
	}

	bool ValidNPC = false;
	if ((CustomEntryData->Flag & CUSTOM_ENTRY_NPC) && ErectusIni::CustomTargetSettings.LockNPCs)
	{
		ValidNPC = true;
	}

	if (ValidPlayer || ValidNPC)
	{
		Reference ReferenceData;
		if (Erectus::RPM(EntityData.ReferencePtr, &ReferenceData, sizeof(ReferenceData)))
		{
			if (ErectusMemory::TargetValid(EntityData, ReferenceData))
			{
				if (CustomEntryData->EntityPtr == TargetLockingPtr)
				{
					TargetLockingValid = true;
					Color = ErectusIni::CustomTargetSettings.LockingColor;
				}
				else if (TargetLockingKeyPressed && !TargetLockingCooldown)
				{
					float Degrees = Erectus::GetDegrees(EntityData.Position, CameraData.Forward, CameraData.Origin);
					if (Degrees < TargetLockingClosestDegrees)
					{
						TargetLockingClosestDegrees = Degrees;
						TargetLockingClosestPtr = CustomEntryData->EntityPtr;
					}
				}
			}
		}
	}

	float Screen[2] = { 0.0f, 0.0f };
	if (!Erectus::WTS(CameraData.View, EntityData.Position, Screen)) return false;

	char* NameText = nullptr;
	if (CustomSettingsA.ShowName && ShowHealthText && CustomSettingsA.ShowDistance)//Name, Health, Distance
	{
		int TextSize = Erectus::GetTextLength(CustomEntryData->Name) + sizeof("\n-2147483648 hp [-2147483648 m]");
		NameText = new char[TextSize];
		sprintf_s(NameText, TextSize, "%s\n%d hp [%d m]", CustomEntryData->Name, Health, NormalDistance);
	}
	else if (CustomSettingsA.ShowName && ShowHealthText && !CustomSettingsA.ShowDistance)//Name, Health
	{
		int TextSize = Erectus::GetTextLength(CustomEntryData->Name) + sizeof("\n-2147483648 hp");
		NameText = new char[TextSize];
		sprintf_s(NameText, TextSize, "%s\n%d hp", CustomEntryData->Name, Health);
	}
	else if (CustomSettingsA.ShowName && !ShowHealthText && CustomSettingsA.ShowDistance)//Name, Distance
	{
		int TextSize = Erectus::GetTextLength(CustomEntryData->Name) + sizeof("\n[-2147483648 m]");
		NameText = new char[TextSize];
		sprintf_s(NameText, TextSize, "%s\n[%d m]", CustomEntryData->Name, NormalDistance);
	}
	else if (!CustomSettingsA.ShowName && ShowHealthText && CustomSettingsA.ShowDistance)//Health, Distance
	{
		int TextSize = sizeof("-2147483648 hp [-2147483648 m]");
		NameText = new char[TextSize];
		sprintf_s(NameText, TextSize, "%d hp [%d m]", Health, NormalDistance);
	}
	else if (CustomSettingsA.ShowName && !ShowHealthText && !CustomSettingsA.ShowDistance)//Name
	{
		int TextSize = Erectus::GetTextSize(CustomEntryData->Name);
		NameText = new char[TextSize];
		sprintf_s(NameText, TextSize, "%s", CustomEntryData->Name);
	}
	else if (!CustomSettingsA.ShowName && ShowHealthText && !CustomSettingsA.ShowDistance)//Health
	{
		int TextSize = sizeof("-2147483648 hp");
		NameText = new char[TextSize];
		sprintf_s(NameText, TextSize, "%d hp", Health);
	}
	else if (!CustomSettingsA.ShowName && !ShowHealthText && CustomSettingsA.ShowDistance)//Distance
	{
		int TextSize = sizeof("[-2147483648 m]");
		NameText = new char[TextSize];
		sprintf_s(NameText, TextSize, "[%d m]", NormalDistance);
	}

	if (NameText != nullptr)
	{
		if (ErectusIni::CustomUtilitySettings.DebugEsp)
		{
			char DebugText[sizeof("0000000000000000\n00000000\n0000000000000000\n00000000")];
			sprintf_s(DebugText, "%llX\n%08lX\n%llX\n%08lX", CustomEntryData->EntityPtr, CustomEntryData->EntityFormid, CustomEntryData->ReferencePtr, CustomEntryData->ReferenceFormid);
			ErectusD3D9::D3D9DrawText(DebugText, CustomSettingsA.TextShadowed, CustomSettingsA.TextCentered, Screen, Color, *Alpha);
		}
		else
		{
			ErectusD3D9::D3D9DrawText(NameText, CustomSettingsA.TextShadowed, CustomSettingsA.TextCentered, Screen, Color, *Alpha);
		}

		delete[]NameText;
		NameText = nullptr;
	}

	return true;
}

bool ErectusMemory::RenderCustomEntryB(CustomEntry* CustomEntryData, OverlaySettingsB CustomSettingsB)
{
	if (!(CustomEntryData->Flag & CUSTOM_ENTRY_WHITELISTED))
	{
		if (!CustomSettingsB.Enabled) return false;
	}

	if (!CustomSettingsB.DrawEnabled && !CustomSettingsB.DrawDisabled)
	{
		return false;
	}

	if (CustomSettingsB.EnabledAlpha == 0.0f && CustomSettingsB.DisabledAlpha == 0.0f)
	{
		return false;
	}

	if (!CustomSettingsB.DrawNamed && !CustomSettingsB.DrawUnnamed)
	{
		return false;
	}

	Entity EntityData;
	if (!Erectus::RPM(CustomEntryData->EntityPtr, &EntityData, sizeof(EntityData))) return false;

	if (CustomEntryData->Name == nullptr)
	{
		CustomEntryData->Flag |= CUSTOM_ENTRY_UNNAMED;
		CustomEntryData->Name = new char[sizeof("00000000")];
		sprintf_s(CustomEntryData->Name, sizeof("00000000"), "%08lX", EntityData.Formid);
	}

	if (CustomEntryData->Flag & CUSTOM_ENTRY_UNNAMED)
	{
		if (!CustomSettingsB.DrawUnnamed) return false;
	}
	else
	{
		if (!CustomSettingsB.DrawNamed) return false;
	}

	if (CustomEntryData->Flag & CUSTOM_ENTRY_PLAN)
	{
		if (!ErectusIni::CustomKnownRecipeSettings.KnownRecipesEnabled && !ErectusIni::CustomKnownRecipeSettings.UnknownRecipesEnabled)
		{
			return false;
		}

		if (!(CustomEntryData->Flag & CUSTOM_ENTRY_FAILED_RECIPE))
		{
			if (!ErectusIni::CustomKnownRecipeSettings.KnownRecipesEnabled && (CustomEntryData->Flag & CUSTOM_ENTRY_KNOWN_RECIPE))
			{
				return false;
			}
			else if (!ErectusIni::CustomKnownRecipeSettings.UnknownRecipesEnabled && (CustomEntryData->Flag & CUSTOM_ENTRY_UNKNOWN_RECIPE))
			{
				return false;
			}
		}
	}

	float* Alpha = nullptr;

	if (EntityData.SpawnFlag == 0x02)
	{
		if (CustomSettingsB.DrawEnabled)
		{
			if (CustomEntryData->Flag & CUSTOM_ENTRY_FLORA)
			{
				if (!FloraHarvested(EntityData.HarvestFlagA, EntityData.HarvestFlagB))
				{
					Alpha = &CustomSettingsB.EnabledAlpha;
				}
				else if (CustomSettingsB.DrawDisabled)
				{
					Alpha = &CustomSettingsB.DisabledAlpha;
				}
			}
			else
			{
				Alpha = &CustomSettingsB.EnabledAlpha;
			}
		}
	}
	else
	{
		if (CustomSettingsB.DrawDisabled)
		{
			Alpha = &CustomSettingsB.DisabledAlpha;
		}
	}

	if (Alpha == nullptr)
	{
		return false;
	}

	DWORD64 CameraPtr = GetCameraPtr();
	if (!Erectus::Valid(CameraPtr)) return false;

	Camera CameraData;
	if (!Erectus::RPM(CameraPtr, &CameraData, sizeof(CameraData))) return false;

	float Distance = Erectus::GetDistance(EntityData.Position, CameraData.Origin);
	int NormalDistance = int(Distance * 0.01f);
	if (NormalDistance > CustomSettingsB.EnabledDistance) return false;

	float Screen[2] = { 0.0f, 0.0f };
	if (!Erectus::WTS(CameraData.View, EntityData.Position, Screen)) return false;

	char* NameText = nullptr;
	if (CustomSettingsB.ShowName && CustomSettingsB.ShowDistance)
	{
		int DistanceTextSize = Erectus::GetTextLength(CustomEntryData->Name) + sizeof("\n[-2147483648 m]");
		NameText = new char[DistanceTextSize];
		sprintf_s(NameText, DistanceTextSize, "%s\n[%d m]", CustomEntryData->Name, NormalDistance);
	}
	else if (CustomSettingsB.ShowName && !CustomSettingsB.ShowDistance)
	{
		int TextSize = Erectus::GetTextSize(CustomEntryData->Name);
		NameText = new char[TextSize];
		sprintf_s(NameText, TextSize, "%s", CustomEntryData->Name);
	}
	else if (!CustomSettingsB.ShowName && CustomSettingsB.ShowDistance)
	{
		int DistanceTextSize = sizeof("[-2147483648 m]");
		NameText = new char[DistanceTextSize];
		sprintf_s(NameText, DistanceTextSize, "[%d m]", NormalDistance);
	}

	if (NameText != nullptr)
	{
		if (ErectusIni::CustomUtilitySettings.DebugEsp)
		{
			char DebugText[sizeof("0000000000000000\n00000000\n0000000000000000\n00000000")];
			sprintf_s(DebugText, "%llX\n%08lX\n%llX\n%08lX", CustomEntryData->EntityPtr, CustomEntryData->EntityFormid, CustomEntryData->ReferencePtr, CustomEntryData->ReferenceFormid);
			ErectusD3D9::D3D9DrawText(DebugText, CustomSettingsB.TextShadowed, CustomSettingsB.TextCentered, Screen, CustomSettingsB.Color, *Alpha);
		}
		else
		{
			ErectusD3D9::D3D9DrawText(NameText, CustomSettingsB.TextShadowed, CustomSettingsB.TextCentered, Screen, CustomSettingsB.Color, *Alpha);
		}

		delete[]NameText;
		NameText = nullptr;
	}

	return true;
}

bool ErectusMemory::RenderCustomEntityList()
{
	CustomEntityListCounter++;
	if (CustomEntityListCounter > 60)
	{
		CustomEntityListCounter = 0;
		if (BufferEntityListUpdated)
		{
			if (!BufferEntityListDestructionQueued)
			{
				DeleteCustomEntityList();
				CustomEntityList = new CustomEntry[BufferEntityListSize];
				memcpy(&*CustomEntityList, &*BufferEntityList, BufferEntityListSize * sizeof(CustomEntry));
				for (int i = 0; i < BufferEntityListSize; i++) BufferEntityList[i].Name = nullptr;
				CustomEntityListSize = BufferEntityListSize;
				CustomEntityListUpdated = true;
				BufferEntityListDestructionQueued = true;
			}
		}
	}

	if (CustomEntityListDestructionQueued)
	{
		CustomEntityListDestructionQueued = false;
		DeleteCustomEntityList();
	}

	if (!CustomEntityListUpdated)
	{
		return false;
	}

	for (int i = 0; i < CustomEntityListSize; i++)
	{
		if (CustomEntityList[i].Flag & CUSTOM_ENTRY_ENTITY)
		{
			RenderCustomEntryB(&CustomEntityList[i], ErectusIni::EntitySettings);
		}
		else if (CustomEntityList[i].Flag & CUSTOM_ENTRY_JUNK)
		{
			RenderCustomEntryB(&CustomEntityList[i], ErectusIni::JunkSettings);
		}
		else if (CustomEntityList[i].Flag & CUSTOM_ENTRY_ITEM)
		{
			RenderCustomEntryB(&CustomEntityList[i], ErectusIni::ItemSettings);
		}
		else if (CustomEntityList[i].Flag & CUSTOM_ENTRY_CONTAINER)
		{
			RenderCustomEntryB(&CustomEntityList[i], ErectusIni::ContainerSettings);
		}
		else if (CustomEntityList[i].Flag & CUSTOM_ENTRY_PLAN)
		{
			RenderCustomEntryB(&CustomEntityList[i], ErectusIni::PlanSettings);
		}
		else if (CustomEntityList[i].Flag & CUSTOM_ENTRY_MAGAZINE)
		{
			RenderCustomEntryB(&CustomEntityList[i], ErectusIni::MagazineSettings);
		}
		else if (CustomEntityList[i].Flag & CUSTOM_ENTRY_BOBBLEHEAD)
		{
			RenderCustomEntryB(&CustomEntityList[i], ErectusIni::BobbleheadSettings);
		}
		else if (CustomEntityList[i].Flag & CUSTOM_ENTRY_FLORA)
		{
			RenderCustomEntryB(&CustomEntityList[i], ErectusIni::FloraSettings);
		}
	}

	return true;
}

bool ErectusMemory::RenderCustomNPCList()
{
	CustomNPCListCounter++;
	if (CustomNPCListCounter > 60)
	{
		CustomNPCListCounter = 0;
		if (BufferNPCListUpdated)
		{
			if (!BufferNPCListDestructionQueued)
			{
				DeleteCustomNPCList();
				CustomNPCList = new CustomEntry[BufferNPCListSize];
				memcpy(&*CustomNPCList, &*BufferNPCList, BufferNPCListSize * sizeof(CustomEntry));
				for (int i = 0; i < BufferNPCListSize; i++) BufferNPCList[i].Name = nullptr;
				CustomNPCListSize = BufferNPCListSize;
				CustomNPCListUpdated = true;
				BufferNPCListDestructionQueued = true;
			}
		}
	}

	if (CustomNPCListDestructionQueued)
	{
		CustomNPCListDestructionQueued = false;
		DeleteCustomNPCList();
	}

	if (!CustomNPCListUpdated)
	{
		return false;
	}

	for (int i = 0; i < CustomNPCListSize; i++)
	{
		if (CustomNPCList[i].Flag & CUSTOM_ENTRY_NPC)
		{
			RenderCustomEntryA(&CustomNPCList[i], ErectusIni::NpcSettings);
		}
	}

	return true;
}

bool ErectusMemory::RenderCustomPlayerList()
{
	CustomPlayerListCounter++;
	if (CustomPlayerListCounter > 60)
	{
		CustomPlayerListCounter = 0;
		if (BufferPlayerListUpdated)
		{
			if (!BufferPlayerListDestructionQueued)
			{
				DeleteCustomPlayerList();
				CustomPlayerList = new CustomEntry[BufferPlayerListSize];
				memcpy(&*CustomPlayerList, &*BufferPlayerList, BufferPlayerListSize * sizeof(CustomEntry));
				for (int i = 0; i < BufferPlayerListSize; i++) BufferPlayerList[i].Name = nullptr;
				CustomPlayerListSize = BufferPlayerListSize;
				CustomPlayerListUpdated = true;
				BufferPlayerListDestructionQueued = true;
			}
		}
	}

	if (CustomPlayerListDestructionQueued)
	{
		CustomPlayerListDestructionQueued = false;
		DeleteCustomPlayerList();
	}

	if (!CustomPlayerListUpdated)
	{
		return false;
	}

	for (int i = 0; i < CustomPlayerListSize; i++)
	{
		if (CustomPlayerList[i].Flag & CUSTOM_ENTRY_PLAYER)
		{
			RenderCustomEntryA(&CustomPlayerList[i], ErectusIni::PlayerSettings);
		}
	}

	return true;
}

bool ErectusMemory::MessagePatcher(bool State)
{
	BYTE FakeMessagesCheck[2];
	if (!Erectus::RPM(ErectusProcess::Exe + OFFSET_FAKE_MESSAGE, &FakeMessagesCheck, sizeof(FakeMessagesCheck))) return false;

	BYTE FakeMessagesEnabled[] = { 0xB0, 0x01 };
	BYTE FakeMessagesDisabled[] = { 0x32, 0xC0 };

	if (!memcmp(FakeMessagesCheck, FakeMessagesEnabled, sizeof(FakeMessagesEnabled)))
	{
		if (State)
		{
			return true;
		}
		else
		{
			return Erectus::WPM(ErectusProcess::Exe + OFFSET_FAKE_MESSAGE, &FakeMessagesDisabled, sizeof(FakeMessagesDisabled));
		}
	}

	if (!memcmp(FakeMessagesCheck, FakeMessagesDisabled, sizeof(FakeMessagesDisabled)))
	{
		if (State)
		{
			return Erectus::WPM(ErectusProcess::Exe + OFFSET_FAKE_MESSAGE, &FakeMessagesEnabled, sizeof(FakeMessagesEnabled));
		}
		else
		{
			return true;
		}
	}

	return false;
}

bool ErectusMemory::SendMessageToServer(void* Message, size_t Size)
{
	if (!MessagePatcher(AllowMessages))
	{
		return false;
	}

	if (!AllowMessages)
	{
		return false;
	}

	size_t AllocSize = Size + sizeof(ExternalFunction);
	DWORD64 AllocAddress = Erectus::AllocEx(AllocSize);
	if (AllocAddress == 0) return false;

	ExternalFunction ExternalFunctionData;
	ExternalFunctionData.Address = ErectusProcess::Exe + OFFSET_MESSAGE_SENDER;
	ExternalFunctionData.RCX = AllocAddress + sizeof(ExternalFunction);
	ExternalFunctionData.RDX = 0;
	ExternalFunctionData.R8 = 0;
	ExternalFunctionData.R9 = 0;

	BYTE* PageData = new BYTE[AllocSize];
	memset(PageData, 0x00, AllocSize);
	memcpy(PageData, &ExternalFunctionData, sizeof(ExternalFunctionData));
	memcpy(&PageData[sizeof(ExternalFunction)], Message, Size);
	bool Written = Erectus::WPM(AllocAddress, &*PageData, AllocSize);

	delete[]PageData;
	PageData = nullptr;

	if (!Written)
	{
		Erectus::FreeEx(AllocAddress);
		return false;
	}

	DWORD64 ParamAddress = AllocAddress + sizeof(ExternalFunction::ASM);
	HANDLE Thread = CreateRemoteThread(ErectusProcess::Handle, NULL, 0, LPTHREAD_START_ROUTINE(AllocAddress), LPVOID(ParamAddress), 0, 0);

	if (Thread == NULL)
	{
		Erectus::FreeEx(AllocAddress);
		return false;
	}

	DWORD ThreadResult = WaitForSingleObject(Thread, 3000);
	CloseHandle(Thread);

	if (ThreadResult == WAIT_TIMEOUT)
	{
		return false;
	}

	Erectus::FreeEx(AllocAddress);
	return true;
}

bool ErectusMemory::LootScrap()
{
	if (!MessagePatcher(AllowMessages))
	{
		return false;
	}

	if (!AllowMessages)
	{
		return false;
	}

	DWORD64 LocalPlayerPtr = GetLocalPlayerPtr(true);
	if (!Erectus::Valid(LocalPlayerPtr))
	{
		ErectusIni::CustomScrapLooterSettings.ScrapAutomaticLootingEnabled = false;
		return false;
	}

	Entity LocalPlayer;
	if (!Erectus::RPM(LocalPlayerPtr, &LocalPlayer, sizeof(LocalPlayer))) return false;

	int BufferSize = 0;
	DWORD64* BufferList = GetEntityList(&BufferSize);
	if (BufferList == nullptr) return false;

	for (int i = 0; i < BufferSize; i++)
	{
		if (!Erectus::Valid(BufferList[i])) continue;
		if (BufferList[i] == LocalPlayerPtr) continue;

		Entity EntityData;
		if (!Erectus::RPM(BufferList[i], &EntityData, sizeof(EntityData))) continue;
		if (!Erectus::Valid(EntityData.ReferencePtr)) continue;

		if (EntityData.SpawnFlag != 0x02)
		{
			continue;
		}

		Reference ReferenceData;
		if (!Erectus::RPM(EntityData.ReferencePtr, &ReferenceData, sizeof(ReferenceData))) continue;
		if (ReferenceData.Formid == 0x00000007) continue;

		DWORD64 EntityFlag = CUSTOM_ENTRY_DEFAULT;
		DWORD64 EntityNamePtr = 0;
		int EnabledDistance = 0;

		GetCustomEntityData(ReferenceData, &EntityFlag, &EntityNamePtr, &EnabledDistance, true, false);
		if (!(EntityFlag & CUSTOM_ENTRY_VALID_SCRAP)) continue;

		float Distance = Erectus::GetDistance(EntityData.Position, LocalPlayer.Position);
		int NormalDistance = int(Distance * 0.01f);

		if (NormalDistance > ErectusIni::CustomScrapLooterSettings.ScrapLooterDistance)
		{
			continue;
		}

		RequestActivateRefMessage RequestActivateRefMessageData;
		RequestActivateRefMessageData.vtable = ErectusProcess::Exe + VTABLE_REQUESTACTIVATEREFMSG;
		RequestActivateRefMessageData.Formid = EntityData.Formid;
		RequestActivateRefMessageData.Choice = 0xFF;
		RequestActivateRefMessageData.ForceActivate = 0;
		SendMessageToServer(&RequestActivateRefMessageData, sizeof(RequestActivateRefMessageData));
	}

	delete[]BufferList;
	BufferList = nullptr;
	return true;
}

bool ErectusMemory::CheckItemLooterSettings()
{
	if (ErectusIni::CustomItemLooterSettings.ItemLooterWeaponsEnabled && ErectusIni::CustomItemLooterSettings.ItemLooterWeaponsDistance > 0)
	{
		return true;
	}
	else if (ErectusIni::CustomItemLooterSettings.ItemLooterArmorEnabled && ErectusIni::CustomItemLooterSettings.ItemLooterArmorDistance > 0)
	{
		return true;
	}
	else if (ErectusIni::CustomItemLooterSettings.ItemLooterAmmoEnabled && ErectusIni::CustomItemLooterSettings.ItemLooterAmmoDistance > 0)
	{
		return true;
	}
	else if (ErectusIni::CustomItemLooterSettings.ItemLooterModsEnabled && ErectusIni::CustomItemLooterSettings.ItemLooterModsDistance > 0)
	{
		return true;
	}
	else if (ErectusIni::CustomItemLooterSettings.ItemLooterMagazinesEnabled && ErectusIni::CustomItemLooterSettings.ItemLooterMagazinesDistance > 0)
	{
		return true;
	}
	else if (ErectusIni::CustomItemLooterSettings.ItemLooterBobbleheadsEnabled && ErectusIni::CustomItemLooterSettings.ItemLooterBobbleheadsDistance > 0)
	{
		return true;
	}
	else if (ErectusIni::CustomItemLooterSettings.ItemLooterAidEnabled && ErectusIni::CustomItemLooterSettings.ItemLooterAidDistance > 0)
	{
		return true;
	}
	else if (ErectusIni::CustomItemLooterSettings.ItemLooterKnownPlansEnabled && ErectusIni::CustomItemLooterSettings.ItemLooterKnownPlansDistance > 0)
	{
		return true;
	}
	else if (ErectusIni::CustomItemLooterSettings.ItemLooterUnknownPlansEnabled && ErectusIni::CustomItemLooterSettings.ItemLooterUnknownPlansDistance > 0)
	{
		return true;
	}
	else if (ErectusIni::CustomItemLooterSettings.ItemLooterMiscEnabled && ErectusIni::CustomItemLooterSettings.ItemLooterMiscDistance > 0)
	{
		return true;
	}
	else if (ErectusIni::CustomItemLooterSettings.ItemLooterUnlistedEnabled && ErectusIni::CustomItemLooterSettings.ItemLooterUnlistedDistance > 0)
	{
		return true;
	}
	else if (ErectusIni::CustomItemLooterSettings.ItemLooterListEnabled && ErectusIni::CustomItemLooterSettings.ItemLooterListDistance > 0)
	{
		return CheckItemLooterList();
	}
	else
	{
		return false;
	}
}

bool ErectusMemory::CheckOnlyUseItemLooterList()
{
	if (ErectusIni::CustomItemLooterSettings.ItemLooterWeaponsEnabled && ErectusIni::CustomItemLooterSettings.ItemLooterWeaponsDistance > 0)
	{
		return false;
	}
	else if (ErectusIni::CustomItemLooterSettings.ItemLooterArmorEnabled && ErectusIni::CustomItemLooterSettings.ItemLooterArmorDistance > 0)
	{
		return false;
	}
	else if (ErectusIni::CustomItemLooterSettings.ItemLooterAmmoEnabled && ErectusIni::CustomItemLooterSettings.ItemLooterAmmoDistance > 0)
	{
		return false;
	}
	else if (ErectusIni::CustomItemLooterSettings.ItemLooterModsEnabled && ErectusIni::CustomItemLooterSettings.ItemLooterModsDistance > 0)
	{
		return false;
	}
	else if (ErectusIni::CustomItemLooterSettings.ItemLooterMagazinesEnabled && ErectusIni::CustomItemLooterSettings.ItemLooterMagazinesDistance > 0)
	{
		return false;
	}
	else if (ErectusIni::CustomItemLooterSettings.ItemLooterBobbleheadsEnabled && ErectusIni::CustomItemLooterSettings.ItemLooterBobbleheadsDistance > 0)
	{
		return false;
	}
	else if (ErectusIni::CustomItemLooterSettings.ItemLooterAidEnabled && ErectusIni::CustomItemLooterSettings.ItemLooterAidDistance > 0)
	{
		return false;
	}
	else if (ErectusIni::CustomItemLooterSettings.ItemLooterKnownPlansEnabled && ErectusIni::CustomItemLooterSettings.ItemLooterKnownPlansDistance > 0)
	{
		return false;
	}
	else if (ErectusIni::CustomItemLooterSettings.ItemLooterUnknownPlansEnabled && ErectusIni::CustomItemLooterSettings.ItemLooterUnknownPlansDistance > 0)
	{
		return false;
	}
	else if (ErectusIni::CustomItemLooterSettings.ItemLooterMiscEnabled && ErectusIni::CustomItemLooterSettings.ItemLooterMiscDistance > 0)
	{
		return false;
	}
	else if (ErectusIni::CustomItemLooterSettings.ItemLooterUnlistedEnabled && ErectusIni::CustomItemLooterSettings.ItemLooterUnlistedDistance > 0)
	{
		return false;
	}
	else if (ErectusIni::CustomItemLooterSettings.ItemLooterListEnabled && ErectusIni::CustomItemLooterSettings.ItemLooterListDistance > 0)
	{
		return CheckItemLooterList();
	}
	else
	{
		return false;
	}
}

bool ErectusMemory::CheckEnabledItem(DWORD Formid, DWORD64 EntityFlag, int NormalDistance)
{
	if (ErectusIni::CustomItemLooterSettings.ItemLooterListEnabled)
	{
		if (CheckFormidArray(Formid, ErectusIni::CustomItemLooterSettings.ItemLooterEnabledList, ErectusIni::CustomItemLooterSettings.ItemLooterFormidList, 100))
		{
			if (NormalDistance <= ErectusIni::CustomItemLooterSettings.ItemLooterListDistance)
			{
				return true;
			}
		}
	}

	if (EntityFlag & CUSTOM_ENTRY_WEAPON)
	{
		if (NormalDistance <= ErectusIni::CustomItemLooterSettings.ItemLooterWeaponsDistance)
		{
			return ErectusIni::CustomItemLooterSettings.ItemLooterWeaponsEnabled;
		}
	}
	else if (EntityFlag & CUSTOM_ENTRY_ARMOR)
	{
		if (NormalDistance <= ErectusIni::CustomItemLooterSettings.ItemLooterArmorDistance)
		{
			return ErectusIni::CustomItemLooterSettings.ItemLooterArmorEnabled;
		}
	}
	else if (EntityFlag & CUSTOM_ENTRY_AMMO)
	{
		if (NormalDistance <= ErectusIni::CustomItemLooterSettings.ItemLooterAmmoDistance)
		{
			return ErectusIni::CustomItemLooterSettings.ItemLooterAmmoEnabled;
		}
	}
	else if (EntityFlag & CUSTOM_ENTRY_MOD)
	{
		if (NormalDistance <= ErectusIni::CustomItemLooterSettings.ItemLooterModsDistance)
		{
			return ErectusIni::CustomItemLooterSettings.ItemLooterModsEnabled;
		}
	}
	else if (EntityFlag & CUSTOM_ENTRY_MAGAZINE)
	{
		if (NormalDistance <= ErectusIni::ErectusIni::CustomItemLooterSettings.ItemLooterMagazinesDistance)
		{
			return ErectusIni::CustomItemLooterSettings.ItemLooterMagazinesEnabled;
		}
	}
	else if (EntityFlag & CUSTOM_ENTRY_BOBBLEHEAD)
	{
		if (NormalDistance <= ErectusIni::CustomItemLooterSettings.ItemLooterBobbleheadsDistance)
		{
			return ErectusIni::CustomItemLooterSettings.ItemLooterBobbleheadsEnabled;
		}
	}
	else if (EntityFlag & CUSTOM_ENTRY_AID)
	{
		if (NormalDistance <= ErectusIni::CustomItemLooterSettings.ItemLooterAidDistance)
		{
			return ErectusIni::CustomItemLooterSettings.ItemLooterAidEnabled;
		}
	}
	else if (EntityFlag & CUSTOM_ENTRY_PLAN)
	{
		if (EntityFlag & CUSTOM_ENTRY_KNOWN_RECIPE)
		{
			if (NormalDistance <= ErectusIni::CustomItemLooterSettings.ItemLooterKnownPlansDistance)
			{
				return ErectusIni::CustomItemLooterSettings.ItemLooterKnownPlansEnabled;
			}
		}
		else if (EntityFlag & CUSTOM_ENTRY_UNKNOWN_RECIPE)
		{
			if (NormalDistance <= ErectusIni::CustomItemLooterSettings.ItemLooterUnknownPlansDistance)
			{
				return ErectusIni::CustomItemLooterSettings.ItemLooterUnknownPlansEnabled;
			}
		}
		else if (EntityFlag & CUSTOM_ENTRY_FAILED_RECIPE)
		{
			if (NormalDistance <= ErectusIni::CustomItemLooterSettings.ItemLooterUnknownPlansDistance)
			{
				return ErectusIni::CustomItemLooterSettings.ItemLooterUnknownPlansEnabled;
			}
		}
	}
	else if (EntityFlag & CUSTOM_ENTRY_MISC)
	{
		if (NormalDistance <= ErectusIni::CustomItemLooterSettings.ItemLooterMiscDistance)
		{
			return ErectusIni::CustomItemLooterSettings.ItemLooterMiscEnabled;
		}
	}

	if (ErectusIni::CustomItemLooterSettings.ItemLooterUnlistedEnabled)
	{
		if (NormalDistance <= ErectusIni::CustomItemLooterSettings.ItemLooterUnlistedDistance)
		{
			return true;
		}
	}

	return false;
}

bool ErectusMemory::LootItems()
{
	if (!MessagePatcher(AllowMessages))
	{
		return false;
	}

	if (!AllowMessages)
	{
		return false;
	}

	DWORD64 LocalPlayerPtr = GetLocalPlayerPtr(true);
	if (!Erectus::Valid(LocalPlayerPtr))
	{
		ErectusIni::CustomItemLooterSettings.ItemAutomaticLootingEnabled = false;
		return false;
	}

	Entity LocalPlayer;
	if (!Erectus::RPM(LocalPlayerPtr, &LocalPlayer, sizeof(LocalPlayer))) return false;

	int BufferSize = 0;
	DWORD64* BufferList = GetEntityList(&BufferSize);
	if (BufferList == nullptr) return false;

	bool OnlyUseItemLooterList = CheckOnlyUseItemLooterList();
	bool UseItemLooterBlacklist = CheckItemLooterBlacklist();

	for (int i = 0; i < BufferSize; i++)
	{
		if (!Erectus::Valid(BufferList[i])) continue;
		if (BufferList[i] == LocalPlayerPtr) continue;

		Entity EntityData;
		if (!Erectus::RPM(BufferList[i], &EntityData, sizeof(EntityData))) continue;
		if (!Erectus::Valid(EntityData.ReferencePtr)) continue;

		if (EntityData.SpawnFlag != 0x02)
		{
			continue;
		}

		Reference ReferenceData;
		if (!Erectus::RPM(EntityData.ReferencePtr, &ReferenceData, sizeof(ReferenceData))) continue;

		if (UseItemLooterBlacklist)
		{
			if (CheckFormidArray(ReferenceData.Formid, ErectusIni::CustomItemLooterSettings.ItemLooterBlacklistEnabled, ErectusIni::CustomItemLooterSettings.ItemLooterBlacklist, 64))
			{
				continue;
			}
		}

		if (OnlyUseItemLooterList)
		{
			if (!CheckFormidArray(ReferenceData.Formid, ErectusIni::CustomItemLooterSettings.ItemLooterEnabledList, ErectusIni::CustomItemLooterSettings.ItemLooterFormidList, 100))
			{
				continue;
			}
		}

		DWORD64 EntityFlag = CUSTOM_ENTRY_DEFAULT;
		DWORD64 EntityNamePtr = 0;
		int EnabledDistance = 0;

		GetCustomEntityData(ReferenceData, &EntityFlag, &EntityNamePtr, &EnabledDistance, false, false);
		if (!(EntityFlag & CUSTOM_ENTRY_VALID_ITEM)) continue;
		if (EntityFlag & CUSTOM_ENTRY_JUNK) continue;

		float Distance = Erectus::GetDistance(EntityData.Position, LocalPlayer.Position);
		int NormalDistance = int(Distance * 0.01f);

		if (!OnlyUseItemLooterList)
		{
			if (!CheckEnabledItem(ReferenceData.Formid, EntityFlag, NormalDistance))
			{
				continue;
			}
		}
		else
		{
			if (NormalDistance > ErectusIni::CustomItemLooterSettings.ItemLooterListDistance)
			{
				continue;
			}
		}

		RequestActivateRefMessage RequestActivateRefMessageData;
		RequestActivateRefMessageData.vtable = ErectusProcess::Exe + VTABLE_REQUESTACTIVATEREFMSG;
		RequestActivateRefMessageData.Formid = EntityData.Formid;
		RequestActivateRefMessageData.Choice = 0xFF;
		RequestActivateRefMessageData.ForceActivate = 0;
		SendMessageToServer(&RequestActivateRefMessageData, sizeof(RequestActivateRefMessageData));
	}

	delete[]BufferList;
	BufferList = nullptr;
	return true;
}

void ErectusMemory::DeleteOldWeaponList()
{
	if (OldWeaponList != nullptr)
	{
		if (OldWeaponListSize)
		{
			for (int i = 0; i < OldWeaponListSize; i++)
			{
				if (OldWeaponList[i].WeaponData != nullptr)
				{
					delete[]OldWeaponList[i].WeaponData;
					OldWeaponList[i].WeaponData = nullptr;
				}

				if (OldWeaponList[i].AimModelData != nullptr)
				{
					delete[]OldWeaponList[i].AimModelData;
					OldWeaponList[i].AimModelData = nullptr;
				}
			}
		}

		delete[]OldWeaponList;
		OldWeaponList = nullptr;
	}

	OldWeaponListSize = 0;
	OldWeaponListUpdated = false;
}

bool ErectusMemory::UpdateOldWeaponData()
{
	DWORD64 DataHandlerPtr;
	if (!Erectus::RPM(ErectusProcess::Exe + OFFSET_DATA_HANDLER, &DataHandlerPtr, sizeof(DataHandlerPtr))) return false;
	if (!Erectus::Valid(DataHandlerPtr)) return false;

	ReferenceList WeaponList;
	if (!Erectus::RPM(DataHandlerPtr + 0x580, &WeaponList, sizeof(WeaponList))) return false;
	if (!Erectus::Valid(WeaponList.ArrayPtr) || !WeaponList.ArraySize || WeaponList.ArraySize > 0x7FFF) return false;

	DWORD64* WeaponPtrArray = new DWORD64[WeaponList.ArraySize];
	if (!Erectus::RPM(WeaponList.ArrayPtr, &*WeaponPtrArray, WeaponList.ArraySize * sizeof(DWORD64)))
	{
		delete[]WeaponPtrArray;
		WeaponPtrArray = nullptr;
		return false;
	}

	OldWeaponList = new OldWeapon[WeaponList.ArraySize];
	OldWeaponListSize = WeaponList.ArraySize;

	for (int i = 0; i < WeaponList.ArraySize; i++)
	{
		OldWeaponList[i].WeaponData = nullptr;
		OldWeaponList[i].AimModelData = nullptr;

		if (!Erectus::Valid(WeaponPtrArray[i])) continue;

		Weapon WeaponData;
		if (!Erectus::RPM(WeaponPtrArray[i], &WeaponData, sizeof(WeaponData))) continue;

		OldWeaponList[i].WeaponData = new Weapon;
		memcpy(&*OldWeaponList[i].WeaponData, &WeaponData, sizeof(WeaponData));

		if (!Erectus::Valid(WeaponData.AimModelPtr)) continue;

		AimModel AimModelData;
		if (!Erectus::RPM(WeaponData.AimModelPtr, &AimModelData, sizeof(AimModelData))) continue;

		OldWeaponList[i].AimModelData = new AimModel;
		memcpy(&*OldWeaponList[i].AimModelData, &AimModelData, sizeof(AimModelData));
	}

	delete[]WeaponPtrArray;
	WeaponPtrArray = nullptr;
	return true;
}

int ErectusMemory::GetOldWeaponIndex(DWORD Formid)
{
	for (int i = 0; i < OldWeaponListSize; i++)
	{
		if (OldWeaponList[i].WeaponData != nullptr)
		{
			if (OldWeaponList[i].WeaponData->Formid == Formid)
			{
				return i;
			}
		}
	}

	return -1;
}

bool ErectusMemory::WeaponEditingEnabled()
{
	WeaponSettings BufferSettings = ErectusIni::CustomWeaponSettings;
	BufferSettings.Capacity = ErectusIni::DefaultWeaponSettings.Capacity;
	BufferSettings.Speed = ErectusIni::DefaultWeaponSettings.Speed;
	BufferSettings.Reach = ErectusIni::DefaultWeaponSettings.Reach;
	if (!memcmp(&BufferSettings, &ErectusIni::DefaultWeaponSettings, sizeof(WeaponSettings))) return false;
	else return true;
}

bool ErectusMemory::EditWeapon(int Index, bool RevertWeaponData)
{
	DWORD64 DataHandlerPtr;
	if (!Erectus::RPM(ErectusProcess::Exe + OFFSET_DATA_HANDLER, &DataHandlerPtr, sizeof(DataHandlerPtr))) return false;
	if (!Erectus::Valid(DataHandlerPtr)) return false;

	ReferenceList WeaponList;
	if (!Erectus::RPM(DataHandlerPtr + 0x580, &WeaponList, sizeof(WeaponList))) return false;
	if (!Erectus::Valid(WeaponList.ArrayPtr) || !WeaponList.ArraySize || WeaponList.ArraySize > 0x7FFF) return false;

	DWORD64 WeaponPtr;
	if (!Erectus::RPM(WeaponList.ArrayPtr + Index * sizeof(DWORD64), &WeaponPtr, sizeof(WeaponPtr))) return false;
	if (!Erectus::Valid(WeaponPtr)) return false;

	Weapon WeaponData;
	if (!Erectus::RPM(WeaponPtr, &WeaponData, sizeof(WeaponData))) return false;
	if (OldWeaponList[Index].WeaponData == nullptr) return false;

	int CurrentWeaponIndex = Index;
	if (OldWeaponList[CurrentWeaponIndex].WeaponData->Formid != WeaponData.Formid)
	{
		int BufferIndex = GetOldWeaponIndex(WeaponData.Formid);
		if (BufferIndex == -1) return false;
		else CurrentWeaponIndex = BufferIndex;
	}

	bool WriteWeaponData = false;
	bool WriteAimModelData = false;

	if (!RevertWeaponData && ErectusIni::CustomWeaponSettings.InstantReload)
	{
		if (WeaponData.ReloadSpeed != 100.0f)
		{
			WeaponData.ReloadSpeed = 100.0f;
			WriteWeaponData = true;
		}
	}
	else
	{
		if (WeaponData.ReloadSpeed != OldWeaponList[CurrentWeaponIndex].WeaponData->ReloadSpeed)
		{
			WeaponData.ReloadSpeed = OldWeaponList[CurrentWeaponIndex].WeaponData->ReloadSpeed;
			WriteWeaponData = true;
		}
	}

	if (!RevertWeaponData && ErectusIni::CustomWeaponSettings.Automaticflag)
	{
		if (!((WeaponData.FlagB >> 7) & 1))
		{
			WeaponData.FlagB |= (1 << 7);
			WriteWeaponData = true;
		}
	}
	else
	{
		if (WeaponData.FlagB != OldWeaponList[CurrentWeaponIndex].WeaponData->FlagB)
		{
			WeaponData.FlagB = OldWeaponList[CurrentWeaponIndex].WeaponData->FlagB;
			WriteWeaponData = true;
		}
	}

	if (!RevertWeaponData && ErectusIni::CustomWeaponSettings.CapacityEnabled)
	{
		if (WeaponData.Capacity != short(ErectusIni::CustomWeaponSettings.Capacity))
		{
			WeaponData.Capacity = short(ErectusIni::CustomWeaponSettings.Capacity);
			WriteWeaponData = true;
		}
	}
	else
	{
		if (WeaponData.Capacity != OldWeaponList[CurrentWeaponIndex].WeaponData->Capacity)
		{
			WeaponData.Capacity = OldWeaponList[CurrentWeaponIndex].WeaponData->Capacity;
			WriteWeaponData = true;
		}
	}

	if (!RevertWeaponData && ErectusIni::CustomWeaponSettings.SpeedEnabled)
	{
		if (WeaponData.Speed != ErectusIni::CustomWeaponSettings.Speed)
		{
			WeaponData.Speed = ErectusIni::CustomWeaponSettings.Speed;
			WriteWeaponData = true;
		}
	}
	else
	{
		if (WeaponData.Speed != OldWeaponList[CurrentWeaponIndex].WeaponData->Speed)
		{
			WeaponData.Speed = OldWeaponList[CurrentWeaponIndex].WeaponData->Speed;
			WriteWeaponData = true;
		}
	}

	if (!RevertWeaponData && ErectusIni::CustomWeaponSettings.ReachEnabled)
	{
		if (WeaponData.Reach != ErectusIni::CustomWeaponSettings.Reach)
		{
			WeaponData.Reach = ErectusIni::CustomWeaponSettings.Reach;
			WriteWeaponData = true;
		}
	}
	else
	{
		if (WeaponData.Reach != OldWeaponList[CurrentWeaponIndex].WeaponData->Reach)
		{
			WeaponData.Reach = OldWeaponList[CurrentWeaponIndex].WeaponData->Reach;
			WriteWeaponData = true;
		}
	}

	if (WriteWeaponData)
	{
		Erectus::WPM(WeaponPtr, &WeaponData, sizeof(WeaponData));
	}

	if (!Erectus::Valid(WeaponData.AimModelPtr))
	{
		return true;
	}

	if (OldWeaponList[CurrentWeaponIndex].AimModelData == nullptr)
	{
		return false;
	}

	AimModel AimModelData;
	if (!Erectus::RPM(WeaponData.AimModelPtr, &AimModelData, sizeof(AimModelData))) return false;

	BYTE NoRecoil[sizeof(AimModel::RecoilData)];
	memset(NoRecoil, 0x00, sizeof(NoRecoil));

	BYTE NoSpread[sizeof(AimModel::SpreadData)];
	memset(NoSpread, 0x00, sizeof(NoSpread));

	if (!RevertWeaponData && ErectusIni::CustomWeaponSettings.NoRecoil)
	{
		if (memcmp(AimModelData.RecoilData, NoRecoil, sizeof(AimModel::RecoilData)))
		{
			memcpy(AimModelData.RecoilData, NoRecoil, sizeof(AimModel::RecoilData));
			WriteAimModelData = true;
		}
	}
	else
	{
		if (memcmp(AimModelData.RecoilData, OldWeaponList[CurrentWeaponIndex].AimModelData->RecoilData, sizeof(AimModel::RecoilData)))
		{
			memcpy(AimModelData.RecoilData, OldWeaponList[CurrentWeaponIndex].AimModelData->RecoilData, sizeof(AimModel::RecoilData));
			WriteAimModelData = true;
		}
	}

	if (!RevertWeaponData && ErectusIni::CustomWeaponSettings.NoSpread)
	{
		if (memcmp(AimModelData.SpreadData, NoSpread, sizeof(AimModel::SpreadData)))
		{
			memcpy(AimModelData.SpreadData, NoSpread, sizeof(AimModel::SpreadData));
			WriteAimModelData = true;
		}
	}
	else
	{
		if (memcmp(AimModelData.SpreadData, OldWeaponList[CurrentWeaponIndex].AimModelData->SpreadData, sizeof(AimModel::SpreadData)))
		{
			memcpy(AimModelData.SpreadData, OldWeaponList[CurrentWeaponIndex].AimModelData->SpreadData, sizeof(AimModel::SpreadData));
			WriteAimModelData = true;
		}
	}

	if (!RevertWeaponData && ErectusIni::CustomWeaponSettings.NoSway)
	{
		if (AimModelData.Sway != 100.0f)
		{
			AimModelData.Sway = 100.0f;
			WriteAimModelData = true;
		}
	}
	else
	{
		if (AimModelData.Sway != OldWeaponList[CurrentWeaponIndex].AimModelData->Sway)
		{
			AimModelData.Sway = OldWeaponList[CurrentWeaponIndex].AimModelData->Sway;
			WriteAimModelData = true;
		}
	}

	if (WriteAimModelData)
	{
		return Erectus::WPM(WeaponData.AimModelPtr, &AimModelData, sizeof(AimModelData));
	}

	return true;
}

bool ErectusMemory::InfiniteAmmo(bool State)
{
	BYTE InfiniteAmmoOn[] = { 0x66, 0xB8, 0xE7, 0x03 };
	BYTE InfiniteAmmoOff[] = { 0x8B, 0x44, 0x24, 0x50 };
	BYTE InfiniteAmmoCheck[sizeof(InfiniteAmmoOff)];

	if (!Erectus::RPM(ErectusProcess::Exe + OFFSET_INFINITE_AMMO, &InfiniteAmmoCheck, sizeof(InfiniteAmmoCheck)))
	{
		return false;
	}

	if (State && !memcmp(InfiniteAmmoCheck, InfiniteAmmoOff, sizeof(InfiniteAmmoOff)))
	{
		return Erectus::WPM(ErectusProcess::Exe + OFFSET_INFINITE_AMMO, &InfiniteAmmoOn, sizeof(InfiniteAmmoOn));
	}
	else if (!State && !memcmp(InfiniteAmmoCheck, InfiniteAmmoOn, sizeof(InfiniteAmmoOn)))
	{
		return Erectus::WPM(ErectusProcess::Exe + OFFSET_INFINITE_AMMO, &InfiniteAmmoOff, sizeof(InfiniteAmmoOff));
	}
	else
	{
		return false;
	}
}

bool ErectusMemory::LockedTargetValid(bool* IsPlayer)
{
	if (!Erectus::Valid(TargetLockingPtr))
	{
		return false;
	}

	Entity EntityData;
	if (!Erectus::RPM(TargetLockingPtr, &EntityData, sizeof(EntityData))) return false;
	if (!Erectus::Valid(EntityData.ReferencePtr)) return false;

	Reference ReferenceData;
	if (!Erectus::RPM(EntityData.ReferencePtr, &ReferenceData, sizeof(ReferenceData))) return false;
	bool Result = ErectusMemory::TargetValid(EntityData, ReferenceData);

	if (ReferenceData.Formid == 0x00000007)
	{
		*IsPlayer = true;
	}
	else
	{
		*IsPlayer = false;
	}

	return Result;
}

bool ErectusMemory::DamageRedirection(DWORD64* TargetingPage, bool* TargetingPageValid, bool IsExiting, bool State)
{
	BYTE PageJmpOn[] = { 0x48, 0xBB, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xE3 };
	BYTE PageJmpOff[] = { 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC };
	BYTE PageJmpCheck[sizeof(PageJmpOff)];

	BYTE RedirectionOn[] = { 0xE9, 0x69, 0xFE, 0xFF, 0xFF };
	BYTE RedirectionOff[] = { 0x48, 0x8B, 0x5C, 0x24, 0x50 };
	BYTE RedirectionCheck[sizeof(RedirectionOff)];

	if (!Erectus::RPM(ErectusProcess::Exe + OFFSET_REDIRECTION_JMP, &PageJmpCheck, sizeof(PageJmpCheck))) return false;

	DWORD64 PageCheck;
	memcpy(&PageCheck, &PageJmpCheck[2], sizeof(PageCheck));
	if (Erectus::Valid(PageCheck) && PageCheck != *TargetingPage)
	{
		BYTE PageOpcode[] = { 0x48, 0xBB, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x00 };
		BYTE PageOpcodeCheck[sizeof(PageOpcode)];
		if (!Erectus::RPM(PageCheck, &PageOpcodeCheck, sizeof(PageOpcodeCheck))) return false;
		if (memcmp(PageOpcodeCheck, PageOpcode, sizeof(PageOpcode))) return false;
		if (!Erectus::WPM(ErectusProcess::Exe + OFFSET_REDIRECTION_JMP, &PageJmpOff, sizeof(PageJmpOff))) return false;
		if (!Erectus::FreeEx(PageCheck)) return false;
	}

	if (!*TargetingPage)
	{
		DWORD64 Page = Erectus::AllocEx(sizeof(Opk));
		if (!Page) return false;
		*TargetingPage = Page;
	}

	if (!*TargetingPageValid)
	{
		TargetLocking TargetLockingData;
		TargetLockingData.TargetLockingPtr = TargetLockingPtr;
		DWORD64 OriginalFunction = ErectusProcess::Exe + OFFSET_REDIRECTION + sizeof(RedirectionOff);
		DWORD64 OriginalFunctionCheck;
		if (!Erectus::RPM(*TargetingPage + 0x30, &OriginalFunctionCheck, sizeof(OriginalFunctionCheck))) return false;
		if (OriginalFunctionCheck != OriginalFunction)
		{
			memcpy(&TargetLockingData.RedirectionASM[0x30], &OriginalFunction, sizeof(OriginalFunction));
		}
		if (!Erectus::WPM(*TargetingPage, &TargetLockingData, sizeof(TargetLockingData))) return false;
		*TargetingPageValid = true;
		return false;
	}
	else
	{
		TargetLocking TargetLockingData;
		if (!Erectus::RPM(*TargetingPage, &TargetLockingData, sizeof(TargetLockingData))) return false;
		if (TargetLockingData.TargetLockingPtr != TargetLockingPtr)
		{
			TargetLockingData.TargetLockingPtr = TargetLockingPtr;
			if (!Erectus::WPM(*TargetingPage, &TargetLockingData, sizeof(TargetLockingData))) return false;
			memcpy(&PageJmpOn[2], &*TargetingPage, sizeof(DWORD64));
		}
		memcpy(&PageJmpOn[2], &*TargetingPage, sizeof(DWORD64));
	}

	bool IsPlayer = false;
	bool TargetValid = LockedTargetValid(&IsPlayer);
	if (!ErectusIni::CustomTargetSettings.IndirectPlayers && IsPlayer)
	{
		TargetValid = false;
	}
	else if (!ErectusIni::CustomTargetSettings.IndirectNPCs && !IsPlayer)
	{
		TargetValid = false;
	}

	bool Redirection = Erectus::RPM(ErectusProcess::Exe + OFFSET_REDIRECTION, &RedirectionCheck, sizeof(RedirectionCheck));

	if (*TargetingPageValid && State && TargetValid)
	{
		if (Redirection && !memcmp(RedirectionCheck, RedirectionOff, sizeof(RedirectionOff)))
		{
			Erectus::WPM(ErectusProcess::Exe + OFFSET_REDIRECTION, &RedirectionOn, sizeof(RedirectionOn));
		}
	}
	else
	{
		if (Redirection && !memcmp(RedirectionCheck, RedirectionOn, sizeof(RedirectionOn)))
		{
			Erectus::WPM(ErectusProcess::Exe + OFFSET_REDIRECTION, &RedirectionOff, sizeof(RedirectionOff));
		}
	}

	if (*TargetingPageValid && !IsExiting && !memcmp(PageJmpCheck, PageJmpOff, sizeof(PageJmpOff)))
	{
		return Erectus::WPM(ErectusProcess::Exe + OFFSET_REDIRECTION_JMP, &PageJmpOn, sizeof(PageJmpOn));
	}
	else if (IsExiting && !memcmp(PageJmpCheck, PageJmpOn, sizeof(PageJmpOn)))
	{
		return Erectus::WPM(ErectusProcess::Exe + OFFSET_REDIRECTION_JMP, &PageJmpOff, sizeof(PageJmpOff));
	}
	else
	{
		return true;
	}
}

bool ErectusMemory::MovePlayer()
{
	DWORD64 CameraPtr = GetCameraPtr();
	if (!Erectus::Valid(CameraPtr)) return false;

	DWORD64 bhkCharProxyControllerPtr;
	if (!Erectus::RPM(ErectusProcess::Exe + OFFSET_CHAR_CONTROLLER, &bhkCharProxyControllerPtr, sizeof(bhkCharProxyControllerPtr))) return false;
	if (!Erectus::Valid(bhkCharProxyControllerPtr)) return false;

	bhkCharProxyController bhkCharProxyControllerData;
	if (!Erectus::RPM(bhkCharProxyControllerPtr, &bhkCharProxyControllerData, sizeof(bhkCharProxyControllerData))) return false;
	if (!Erectus::Valid(bhkCharProxyControllerData.hknpBSCharacterProxyPtr)) return false;

	hknpBSCharacterProxy hknpBSCharacterProxyData;
	if (!Erectus::RPM(bhkCharProxyControllerData.hknpBSCharacterProxyPtr, &hknpBSCharacterProxyData, sizeof(hknpBSCharacterProxyData))) return false;

	Camera CameraData;
	if (!Erectus::RPM(CameraPtr, &CameraData, sizeof(CameraData))) return false;

	float VelocityA[4];
	memset(VelocityA, 0x00, sizeof(VelocityA));

	float VelocityB[4];
	memset(VelocityB, 0x00, sizeof(VelocityB));

	float Speed = ErectusIni::CustomLocalPlayerSettings.NoclipSpeed;
	if (GetAsyncKeyState(VK_SHIFT))
	{
		Speed *= 1.5f;
	}

	bool WritePosition = false;

	if (GetAsyncKeyState('W'))
	{
		hknpBSCharacterProxyData.Position[0] += (CameraData.Forward[0] * Speed);
		hknpBSCharacterProxyData.Position[1] += (CameraData.Forward[1] * Speed);
		hknpBSCharacterProxyData.Position[2] += (CameraData.Forward[2] * Speed);
		WritePosition = true;
	}

	if (GetAsyncKeyState('A'))
	{
		hknpBSCharacterProxyData.Position[0] -= (CameraData.Forward[1] * Speed);
		hknpBSCharacterProxyData.Position[1] += (CameraData.Forward[0] * Speed);
		WritePosition = true;
	}

	if (GetAsyncKeyState('S'))
	{
		hknpBSCharacterProxyData.Position[0] -= (CameraData.Forward[0] * Speed);
		hknpBSCharacterProxyData.Position[1] -= (CameraData.Forward[1] * Speed);
		hknpBSCharacterProxyData.Position[2] -= (CameraData.Forward[2] * Speed);
		WritePosition = true;
	}

	if (GetAsyncKeyState('D'))
	{
		hknpBSCharacterProxyData.Position[0] += (CameraData.Forward[1] * Speed);
		hknpBSCharacterProxyData.Position[1] -= (CameraData.Forward[0] * Speed);
		WritePosition = true;
	}

	if (memcmp(hknpBSCharacterProxyData.VelocityA, VelocityA, sizeof(VelocityA)))
	{
		memcpy(hknpBSCharacterProxyData.VelocityA, VelocityA, sizeof(VelocityA));
		WritePosition = true;
	}

	if (memcmp(hknpBSCharacterProxyData.VelocityB, VelocityB, sizeof(VelocityB)))
	{
		memcpy(hknpBSCharacterProxyData.VelocityB, VelocityB, sizeof(VelocityB));
		WritePosition = true;
	}

	if (WritePosition)
	{
		return Erectus::WPM(bhkCharProxyControllerData.hknpBSCharacterProxyPtr, &hknpBSCharacterProxyData, sizeof(hknpBSCharacterProxyData));
	}

	return true;
}

void ErectusMemory::Noclip(bool State)
{
	BYTE NoclipOnA[] = { 0x0F, 0x1F, 0x44, 0x00, 0x00 };
	BYTE NoclipOffA[] = { 0xE8, 0xC3, 0xC5, 0xFE, 0xFF };
	BYTE NoclipCheckA[sizeof(NoclipOffA)];

	BYTE NoclipOnB[] = { 0x0F, 0x1F, 0x40, 0x00 };
	BYTE NoclipOffB[] = { 0x41, 0xFF, 0x50, 0x40 };
	BYTE NoclipCheckB[sizeof(NoclipOffB)];

	BYTE NoclipOnC[] = { 0x0F, 0x1F, 0x44, 0x00, 0x00 };
	BYTE NoclipOffC[] = { 0xE8, 0x9A, 0xA1, 0x34, 0x01 };
	BYTE NoclipCheckC[sizeof(NoclipOffC)];

	BYTE NoclipOnD[] = { 0x66, 0x0F, 0x1F, 0x44, 0x00, 0x00 };
	BYTE NoclipOffD[] = { 0xFF, 0x15, 0x59, 0xEC, 0xFF, 0x01 };
	BYTE NoclipCheckD[sizeof(NoclipOffD)];

	bool NoclipA = Erectus::RPM(ErectusProcess::Exe + OFFSET_NOCLIP_A, &NoclipCheckA, sizeof(NoclipCheckA));
	bool NoclipB = Erectus::RPM(ErectusProcess::Exe + OFFSET_NOCLIP_B, &NoclipCheckB, sizeof(NoclipCheckB));
	bool NoclipC = Erectus::RPM(ErectusProcess::Exe + OFFSET_NOCLIP_C, &NoclipCheckC, sizeof(NoclipCheckC));
	bool NoclipD = Erectus::RPM(ErectusProcess::Exe + OFFSET_NOCLIP_D, &NoclipCheckD, sizeof(NoclipCheckD));

	if (State)
	{
		if (NoclipA && !memcmp(NoclipCheckA, NoclipOffA, sizeof(NoclipOffA)))
		{
			Erectus::WPM(ErectusProcess::Exe + OFFSET_NOCLIP_A, &NoclipOnA, sizeof(NoclipOnA));
		}

		if (NoclipB && !memcmp(NoclipCheckB, NoclipOffB, sizeof(NoclipOffB)))
		{
			Erectus::WPM(ErectusProcess::Exe + OFFSET_NOCLIP_B, &NoclipOnB, sizeof(NoclipOnB));
		}

		if (NoclipC && !memcmp(NoclipCheckC, NoclipOffC, sizeof(NoclipOffC)))
		{
			Erectus::WPM(ErectusProcess::Exe + OFFSET_NOCLIP_C, &NoclipOnC, sizeof(NoclipOnC));
		}

		if (NoclipD && !memcmp(NoclipCheckD, NoclipOffD, sizeof(NoclipOffD)))
		{
			Erectus::WPM(ErectusProcess::Exe + OFFSET_NOCLIP_D, &NoclipOnD, sizeof(NoclipOnD));
		}

		MovePlayer();
	}
	else
	{
		if (NoclipA && !memcmp(NoclipCheckA, NoclipOnA, sizeof(NoclipOnA)))
		{
			Erectus::WPM(ErectusProcess::Exe + OFFSET_NOCLIP_A, &NoclipOffA, sizeof(NoclipOffA));
		}

		if (NoclipB && !memcmp(NoclipCheckB, NoclipOnB, sizeof(NoclipOnB)))
		{
			Erectus::WPM(ErectusProcess::Exe + OFFSET_NOCLIP_B, &NoclipOffB, sizeof(NoclipOffB));
		}

		if (NoclipC && !memcmp(NoclipCheckC, NoclipOnC, sizeof(NoclipOnC)))
		{
			Erectus::WPM(ErectusProcess::Exe + OFFSET_NOCLIP_C, &NoclipOffC, sizeof(NoclipOffC));
		}

		if (NoclipD && !memcmp(NoclipCheckD, NoclipOnD, sizeof(NoclipOnD)))
		{
			Erectus::WPM(ErectusProcess::Exe + OFFSET_NOCLIP_D, &NoclipOffD, sizeof(NoclipOffD));
		}
	}
}

bool ErectusMemory::ActorValue(DWORD64* ActorValuePage, bool* ActorValuePageValid, bool State)
{
	if (!*ActorValuePage)
	{
		DWORD64 Page = Erectus::AllocEx(sizeof(ActorValueHook));
		if (!Page) return false;
		*ActorValuePage = Page;
	}

	DWORD64 LocalPlayerPtr = GetLocalPlayerPtr(false);
	if (!Erectus::Valid(LocalPlayerPtr)) return false;

	Entity LocalPlayer;
	if (!Erectus::RPM(LocalPlayerPtr, &LocalPlayer, sizeof(LocalPlayer))) return false;
	if (!Erectus::Valid(LocalPlayer.vtable0050)) return false;

	DWORD64 ActorValueFunction;
	if (!Erectus::RPM(LocalPlayer.vtable0050 + 0x48, &ActorValueFunction, sizeof(ActorValueFunction))) return false;
	if (!Erectus::Valid(ActorValueFunction)) return false;

	ActorValueHook ActorValueHookData;
	ActorValueHookData.ActionPointsEnabled = int(ErectusIni::CustomLocalPlayerSettings.ActionPointsEnabled);
	ActorValueHookData.ActionPoints = float(ErectusIni::CustomLocalPlayerSettings.ActionPoints);
	ActorValueHookData.StrengthEnabled = int(ErectusIni::CustomLocalPlayerSettings.StrengthEnabled);
	ActorValueHookData.Strength = float(ErectusIni::CustomLocalPlayerSettings.Strength);
	ActorValueHookData.PerceptionEnabled = int(ErectusIni::CustomLocalPlayerSettings.PerceptionEnabled);
	ActorValueHookData.Perception = float(ErectusIni::CustomLocalPlayerSettings.Perception);
	ActorValueHookData.EnduranceEnabled = int(ErectusIni::CustomLocalPlayerSettings.EnduranceEnabled);
	ActorValueHookData.Endurance = float(ErectusIni::CustomLocalPlayerSettings.Endurance);
	ActorValueHookData.CharismaEnabled = int(ErectusIni::CustomLocalPlayerSettings.CharismaEnabled);
	ActorValueHookData.Charisma = float(ErectusIni::ErectusIni::CustomLocalPlayerSettings.Charisma);
	ActorValueHookData.IntelligenceEnabled = int(ErectusIni::CustomLocalPlayerSettings.IntelligenceEnabled);
	ActorValueHookData.Intelligence = float(ErectusIni::CustomLocalPlayerSettings.Intelligence);
	ActorValueHookData.AgilityEnabled = int(ErectusIni::CustomLocalPlayerSettings.AgilityEnabled);
	ActorValueHookData.Agility = float(ErectusIni::CustomLocalPlayerSettings.Agility);
	ActorValueHookData.LuckEnabled = int(ErectusIni::CustomLocalPlayerSettings.LuckEnabled);
	ActorValueHookData.Luck = float(ErectusIni::CustomLocalPlayerSettings.Luck);
	ActorValueHookData.OriginalFunction = ErectusProcess::Exe + OFFSET_ACTOR_VALUE;

	if (ActorValueFunction != (ErectusProcess::Exe + OFFSET_ACTOR_VALUE) && ActorValueFunction != *ActorValuePage)
	{
		if (Erectus::vtableSwap(LocalPlayer.vtable0050 + 0x48, ErectusProcess::Exe + OFFSET_ACTOR_VALUE))
		{
			Erectus::FreeEx(ActorValueFunction);
		}
	}

	if (State)
	{
		if (*ActorValuePageValid)
		{
			ActorValueHook ActorValueHookCheck;
			if (!Erectus::RPM(*ActorValuePage, &ActorValueHookCheck, sizeof(ActorValueHookCheck))) return false;
			if (memcmp(&ActorValueHookData, &ActorValueHookCheck, sizeof(ActorValueHookCheck)))
			{
				return Erectus::WPM(*ActorValuePage, &ActorValueHookData, sizeof(ActorValueHookData));
			}
		}
		else
		{
			if (!Erectus::WPM(*ActorValuePage, &ActorValueHookData, sizeof(ActorValueHookData))) return false;
			if (!Erectus::vtableSwap(LocalPlayer.vtable0050 + 0x48, *ActorValuePage)) return false;
			*ActorValuePageValid = true;
		}
	}
	else
	{
		if (ActorValueFunction != (ErectusProcess::Exe + OFFSET_ACTOR_VALUE))
		{
			if (Erectus::vtableSwap(LocalPlayer.vtable0050 + 0x48, ErectusProcess::Exe + OFFSET_ACTOR_VALUE))
			{
				Erectus::FreeEx(ActorValueFunction);
			}
		}

		if (*ActorValuePage)
		{
			if (Erectus::FreeEx(*ActorValuePage))
			{
				*ActorValuePage = 0;
				*ActorValuePageValid = false;
			}
		}
	}

	return true;
}

bool ErectusMemory::SetActorValueMaximum(DWORD Formid, float DefaultValue, float CustomValue, bool State)
{
	DWORD64 ActorValuePtr = GetPtr(Formid);
	if (!Erectus::Valid(ActorValuePtr)) return false;

	ActorValueInformation ActorValueData;
	if (!Erectus::RPM(ActorValuePtr, &ActorValueData, sizeof(ActorValueData))) return false;

	if (State)
	{
		if (ActorValueData.MaximumValue != CustomValue)
		{
			ActorValueData.MaximumValue = CustomValue;
			return Erectus::WPM(ActorValuePtr, &ActorValueData, sizeof(ActorValueData));
		}
	}
	else
	{
		if (ActorValueData.MaximumValue != DefaultValue)
		{
			ActorValueData.MaximumValue = DefaultValue;
			return Erectus::WPM(ActorValuePtr, &ActorValueData, sizeof(ActorValueData));
		}
	}

	return true;
}

bool ErectusMemory::OnePositionKill(DWORD64* OpkPage, bool* OpkPageValid, bool State)
{
	if (!*OpkPage)
	{
		DWORD64 Page = Erectus::AllocEx(sizeof(Opk));
		if (!Page) return false;
		*OpkPage = Page;
	}

	BYTE OpkOn[] = { 0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xE0, 0xCC, 0xCC, 0xCC };
	BYTE OpkOff[] = { 0x0F, 0x10, 0x87, 0x90, 0x04, 0x00, 0x00, 0x0F, 0x58, 0x45, 0xA7, 0x0F, 0x29, 0x45, 0xF7 };
	BYTE OpkCheck[sizeof(OpkOff)];

	if (!Erectus::RPM(ErectusProcess::Exe + OFFSET_OPK, &OpkCheck, sizeof(OpkCheck)))
	{
		return false;
	}

	DWORD64 OriginalFunction = ErectusProcess::Exe + OFFSET_OPK + sizeof(OpkOff);
	memcpy(&OpkOn[2], &*OpkPage, sizeof(DWORD64));

	DWORD64 PageCheck;
	memcpy(&PageCheck, &OpkCheck[2], sizeof(DWORD64));

	if (Erectus::Valid(PageCheck) && PageCheck != *OpkPage)
	{
		Opk Buffer;
		if (!Erectus::RPM(PageCheck, &Buffer, sizeof(Buffer))) return false;
		if (Buffer.OriginalFunction != OriginalFunction) return false;
		if (!Erectus::WPM(ErectusProcess::Exe + OFFSET_OPK, &OpkOff, sizeof(OpkOff))) return false;
		Erectus::FreeEx(PageCheck);
	}

	if (State)
	{
		if (*OpkPageValid)
		{
			return true;
		}
		else
		{
			Opk OpkData;
			OpkData.OpkPlayers = 0;
			OpkData.OpkNpcs = 0;
			OpkData.OriginalFunction = OriginalFunction;
			memset(OpkData.OpkPlayerPosition, 0x00, sizeof(OpkData.OpkPlayerPosition));
			memset(OpkData.OpkNpcPosition, 0x00, sizeof(OpkData.OpkNpcPosition));

			if (!Erectus::WPM(*OpkPage, &OpkData, sizeof(OpkData))) return false;
			if (!Erectus::WPM(ErectusProcess::Exe + OFFSET_OPK, &OpkOn, sizeof(OpkOn))) return false;
			*OpkPageValid = true;
		}
	}
	else
	{
		if (PageCheck == *OpkPage)
		{
			Erectus::WPM(ErectusProcess::Exe + OFFSET_OPK, &OpkOff, sizeof(OpkOff));
		}

		if (*OpkPage)
		{
			if (Erectus::FreeEx(*OpkPage))
			{
				*OpkPage = 0;
				*OpkPageValid = false;
			}
		}
	}

	return true;
}

bool ErectusMemory::CheckOpkDistance(DWORD64 OpkPage, bool Players)
{
	Opk OpkData;
	if (!Erectus::RPM(OpkPage, &OpkData, sizeof(OpkData))) return false;

	DWORD64 CameraPtr = GetCameraPtr();
	if (!Erectus::Valid(CameraPtr)) return false;

	Camera CameraData;
	if (!Erectus::RPM(CameraPtr, &CameraData, sizeof(CameraData))) return false;

	float EditedOrigin[3];
	EditedOrigin[0] = CameraData.Origin[0] / 70.0f;
	EditedOrigin[1] = CameraData.Origin[1] / 70.0f;
	EditedOrigin[2] = CameraData.Origin[2] / 70.0f;

	if (Players)
	{
		float Distance = Erectus::GetDistance(OpkData.OpkPlayerPosition, EditedOrigin);
		if (Distance > 20.0f) return false;
	}
	else
	{
		float Distance = Erectus::GetDistance(OpkData.OpkNpcPosition, EditedOrigin);
		if (Distance > 20.0f) return false;
	}

	return true;
}

bool ErectusMemory::SetOpkData(DWORD64 OpkPage, bool Players, bool State)
{
	Opk OpkData;
	if (!Erectus::RPM(OpkPage, &OpkData, sizeof(OpkData))) return false;

	if (!State)
	{
		bool WriteData = false;

		if (Players && OpkData.OpkPlayers)
		{
			OpkData.OpkPlayers = 0;
			memset(OpkData.OpkPlayerPosition, 0x00, sizeof(OpkData.OpkPlayerPosition));
			WriteData = true;
		}
		else if (!Players && OpkData.OpkNpcs)
		{
			OpkData.OpkNpcs = 0;
			memset(OpkData.OpkNpcPosition, 0x00, sizeof(OpkData.OpkNpcPosition));
			WriteData = true;
		}

		if (WriteData)
		{
			Erectus::WPM(OpkPage, &OpkData, sizeof(OpkData));
		}

		return true;
	}

	if (CheckOpkDistance(OpkPage, Players))
	{
		return true;
	}

	DWORD64 CameraPtr = GetCameraPtr();
	if (!Erectus::Valid(CameraPtr)) return false;

	Camera CameraData;
	if (!Erectus::RPM(CameraPtr, &CameraData, sizeof(CameraData))) return false;

	float EditedOrigin[3];
	EditedOrigin[0] = CameraData.Origin[0] / 70.0f;
	EditedOrigin[1] = CameraData.Origin[1] / 70.0f;
	EditedOrigin[2] = CameraData.Origin[2] / 70.0f;

	float OpkPosition[3];
	Erectus::ProjectView(OpkPosition, CameraData.Forward, EditedOrigin, 3.0f);

	if (Players)
	{
		OpkData.OpkPlayerPosition[0] = OpkPosition[0];
		OpkData.OpkPlayerPosition[1] = OpkPosition[1];
		OpkData.OpkPlayerPosition[2] = OpkPosition[2];
		OpkData.OpkPlayers = 1;
	}
	else
	{
		OpkData.OpkNpcPosition[0] = OpkPosition[0];
		OpkData.OpkNpcPosition[1] = OpkPosition[1];
		OpkData.OpkNpcPosition[2] = OpkPosition[2];
		OpkData.OpkNpcs = 1;
	}

	return Erectus::WPM(OpkPage, &OpkData, sizeof(OpkData));
}

bool ErectusMemory::InsideInteriorCell()
{
	DWORD64 EntityListTypePtr;
	if (!Erectus::RPM(ErectusProcess::Exe + OFFSET_ENTITY_LIST, &EntityListTypePtr, sizeof(EntityListTypePtr))) return false;
	if (!Erectus::Valid(EntityListTypePtr)) return false;

	EntityListType EntityListTypeData;
	if (!Erectus::RPM(EntityListTypePtr, &EntityListTypeData, sizeof(EntityListTypeData))) return false;
	if (!Erectus::Valid(EntityListTypeData.InteriorListPtr)) return false;
	if (!Erectus::Valid(EntityListTypeData.InteriorListCheck)) return false;
	if (!Erectus::Valid(EntityListTypeData.ExteriorListPtr)) return false;
	if (!Erectus::Valid(EntityListTypeData.ExteriorListCheck)) return false;

	if (EntityListTypeData.InteriorListPtr != EntityListTypeData.InteriorListCheck)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int ErectusMemory::RenderLocalPlayerData()
{
	DWORD64 LocalPlayerPtr = GetLocalPlayerPtr(false);
	if (!Erectus::Valid(LocalPlayerPtr)) return 0;

	Entity LocalPlayer;
	if (!Erectus::RPM(LocalPlayerPtr, &LocalPlayer, sizeof(LocalPlayer))) return 0;

	DWORD CellFormid = 0;
	if (Erectus::Valid(LocalPlayer.CellPtr))
	{
		if (!Erectus::RPM(LocalPlayer.CellPtr + 0x20, &CellFormid, sizeof(CellFormid)))
		{
			CellFormid = 0;
		}
	}

	float PlayerHealth = -1.0f;
	ActorSnapshotComponent ActorSnapshotComponentData;
	if (GetActorSnapshotComponentData(LocalPlayer, &ActorSnapshotComponentData))
	{
		PlayerHealth = ActorSnapshotComponentData.MaxHealth + ActorSnapshotComponentData.ModifiedHealth + ActorSnapshotComponentData.LostHealth;
	}

	char EntityFormidText[256];
	sprintf_s(EntityFormidText, "Entity Formid: %08lX", LocalPlayer.Formid);

	char StashFormidText[256];
	sprintf_s(StashFormidText, "STASH Formid: %08lX", LocalPlayer.Formid0C24);

	char CellFormidText[256];
	sprintf_s(CellFormidText, "Cell Formid: %08lX", CellFormid);

	char PositionTextX[256];
	sprintf_s(PositionTextX, "X: %f", LocalPlayer.Position[0]);

	char PositionTextY[256];
	sprintf_s(PositionTextY, "Y: %f", LocalPlayer.Position[1]);

	char PositionTextZ[256];
	sprintf_s(PositionTextZ, "Z: %f", LocalPlayer.Position[2]);

	char YawTest[256];
	sprintf_s(YawTest, "Yaw: %f", LocalPlayer.Yaw);

	char PitchText[256];
	sprintf_s(PitchText, "Pitch: %f", LocalPlayer.Pitch);

	char HealthText[256];
	sprintf_s(HealthText, "Health: %f", PlayerHealth);

	char* TextArray[]
	{
		EntityFormidText,
		StashFormidText,
		CellFormidText,
		PositionTextX,
		PositionTextY,
		PositionTextZ,
		YawTest,
		PitchText,
		HealthText,
	};

	float TextColor[3] = { 1.0f, 1.0f, 1.0f };
	float TextAlpha = 1.0f;

	int Index = 0;

	while (Index < sizeof(TextArray) / sizeof(char*))
	{
		float TextPosition[2] = { 0.0f, float(Index) * 16.0f };
		ErectusD3D9::D3D9DrawText(TextArray[Index], true, false, TextPosition, TextColor, TextAlpha);
		Index++;
	}

	return Index;
}

void ErectusMemory::RenderData()
{
	int Spacing = 0;
	if (ErectusIni::CustomUtilitySettings.DebugPlayer)
	{
		Spacing = RenderLocalPlayerData();
	}

	float EnabledTextColor[3] = { 0.0f, 1.0f, 0.0f };
	float DisabledTextColor[3] = { 1.0f, 0.0f, 0.0f };
	float TextAlpha = 1.0f;

	char ScrapLooterText[256];
	sprintf_s(ScrapLooterText, "Scrap Looter (Automatic): %d", int(ErectusIni::CustomScrapLooterSettings.ScrapAutomaticLootingEnabled));

	char ItemLooterText[256];
	sprintf_s(ItemLooterText, "Item Looter (Automatic): %d", int(ErectusIni::CustomItemLooterSettings.ItemAutomaticLootingEnabled));

	char NPCLooterText[256];
	sprintf_s(NPCLooterText, "NPC Looter (76m Distance Limit): %d", int(ErectusIni::NPCLooterSettings.EntityLooterEnabled));

	char ContainerLooterText[256];
	sprintf_s(ContainerLooterText, "Container Looter (6m Distance Limit): %d", int(ErectusIni::ContainerLooterSettings.EntityLooterEnabled));

	char HarvesterText[256];
	sprintf_s(HarvesterText, "Flora Harvester (6m Distance Limit): %d", int(ErectusIni::CustomHarvesterSettings.HarvesterEnabled));

	char PositionSpoofingText[256];
	sprintf_s(PositionSpoofingText, "Position Spoofing (Active): %d (Height: %d)", int(ErectusThread::PositionSpoofingToggle), ErectusIni::CustomLocalPlayerSettings.PositionSpoofingHeight);

	char DrawAlphaCodeText[256];
	sprintf_s(DrawAlphaCodeText, "%d %d %d %d %d %d %d %d - Alpha", ErectusImGui::AlphaCode[0], ErectusImGui::AlphaCode[1], ErectusImGui::AlphaCode[2], ErectusImGui::AlphaCode[3], ErectusImGui::AlphaCode[4], ErectusImGui::AlphaCode[5], ErectusImGui::AlphaCode[6], ErectusImGui::AlphaCode[7]);

	char DrawBravoCodeText[256];
	sprintf_s(DrawBravoCodeText, "%d %d %d %d %d %d %d %d - Bravo", ErectusImGui::BravoCode[0], ErectusImGui::BravoCode[1], ErectusImGui::BravoCode[2], ErectusImGui::BravoCode[3], ErectusImGui::BravoCode[4], ErectusImGui::BravoCode[5], ErectusImGui::BravoCode[6], ErectusImGui::BravoCode[7]);

	char DrawCharlieCodeText[256];
	sprintf_s(DrawCharlieCodeText, "%d %d %d %d %d %d %d %d - Charlie", ErectusImGui::CharlieCode[0], ErectusImGui::CharlieCode[1], ErectusImGui::CharlieCode[2], ErectusImGui::CharlieCode[3], ErectusImGui::CharlieCode[4], ErectusImGui::CharlieCode[5], ErectusImGui::CharlieCode[6], ErectusImGui::CharlieCode[7]);

	bool PositionSpoofingState = ErectusIni::CustomLocalPlayerSettings.PositionSpoofingEnabled;
	if (PositionSpoofingState)
	{
		if (InsideInteriorCell())
		{
			PositionSpoofingState = false;
		}
	}

	bool AlphaCodeState = false;
	for (int i = 0; i < 8; i++)
	{
		if (ErectusImGui::AlphaCode[i])
		{
			AlphaCodeState = true;
			break;
		}
	}

	bool BravoCodeState = false;
	for (int i = 0; i < 8; i++)
	{
		if (ErectusImGui::BravoCode[i])
		{
			BravoCodeState = true;
			break;
		}
	}

	bool CharlieCodeState = false;
	for (int i = 0; i < 8; i++)
	{
		if (ErectusImGui::CharlieCode[i])
		{
			CharlieCodeState = true;
			break;
		}
	}

	char* TextArray[]
	{
		ScrapLooterText,
		ItemLooterText,
		NPCLooterText,
		ContainerLooterText,
		HarvesterText,
		PositionSpoofingText,
		DrawAlphaCodeText,
		DrawBravoCodeText,
		DrawCharlieCodeText,
	};

	bool StateArray[]
	{
		ErectusIni::CustomScrapLooterSettings.ScrapAutomaticLootingEnabled,
		ErectusIni::CustomItemLooterSettings.ItemAutomaticLootingEnabled,
		ErectusIni::NPCLooterSettings.EntityLooterEnabled,
		ErectusIni::ContainerLooterSettings.EntityLooterEnabled,
		ErectusIni::CustomHarvesterSettings.HarvesterEnabled,
		PositionSpoofingState,
		AlphaCodeState,
		BravoCodeState,
		CharlieCodeState,
	};

	bool EnabledArray[]
	{
		ErectusIni::CustomScrapLooterSettings.ScrapAutomaticStatus,
		ErectusIni::CustomItemLooterSettings.ItemAutomaticStatus,
		ErectusIni::NPCLooterSettings.EntityLooterStatusEnabled,
		ErectusIni::ContainerLooterSettings.EntityLooterStatusEnabled,
		ErectusIni::CustomHarvesterSettings.HarvesterStatusEnabled,
		ErectusIni::CustomLocalPlayerSettings.DrawPositionSpoofingEnabled,
		ErectusIni::CustomNukeCodeSettings.DrawCodeAlpha,
		ErectusIni::CustomNukeCodeSettings.DrawCodeBravo,
		ErectusIni::CustomNukeCodeSettings.DrawCodeCharlie,
	};

	for (int i = 0; i < sizeof(TextArray) / sizeof(char*); i++)
	{
		if (!EnabledArray[i]) continue;
		float* TextColor = nullptr;
		if (StateArray[i]) TextColor = EnabledTextColor;
		else TextColor = DisabledTextColor;
		float TextPosition[2] = { 0.0f, float(Spacing) * 16.0f };
		ErectusD3D9::D3D9DrawText(TextArray[i], true, false, TextPosition, TextColor, TextAlpha);
		Spacing++;
	}
}

bool ErectusMemory::ReferenceSwap(DWORD* SourceFormid, DWORD* DestinationFormid)
{
	if (*SourceFormid == *DestinationFormid)
	{
		return true;
	}

	DWORD64 SourcePointer = GetPtr(*SourceFormid);
	if (!Erectus::Valid(SourcePointer))
	{
		*SourceFormid = 0x00000000;
		return false;
	}

	DWORD64 DestinationAddress = GetAddress(*DestinationFormid);
	if (!Erectus::Valid(DestinationAddress))
	{
		*DestinationFormid = 0x00000000;
		return false;
	}

	return Erectus::WPM(DestinationAddress, &SourcePointer, sizeof(SourcePointer));
}

bool ErectusMemory::CheckItemTransferList()
{
	for (int i = 0; i < 32; i++)
	{
		if (ErectusIni::CustomTransferSettings.Whitelist[i])
		{
			if (ErectusIni::CustomTransferSettings.Whitelisted[i])
			{
				return true;
			}
		}
	}

	return false;
}

bool ErectusMemory::TransferItems(DWORD SourceFormid, DWORD DestinationFormid)
{
	DWORD64 SourcePtr = GetPtr(SourceFormid);
	if (!Erectus::Valid(SourcePtr)) return false;

	DWORD64 DestinationPtr = GetPtr(DestinationFormid);
	if (!Erectus::Valid(DestinationPtr)) return false;

	Entity EntityData;
	if (!Erectus::RPM(SourcePtr, &EntityData, sizeof(EntityData))) return false;
	if (!Erectus::Valid(EntityData.InventoryPtr)) return false;

	Inventory InventoryData;
	if (!Erectus::RPM(EntityData.InventoryPtr, &InventoryData, sizeof(InventoryData))) return false;
	if (!Erectus::Valid(InventoryData.ItemArrayPtr) || InventoryData.ItemArrayEnd < InventoryData.ItemArrayPtr) return false;

	DWORD64 ItemArraySize = (InventoryData.ItemArrayEnd - InventoryData.ItemArrayPtr) / sizeof(Item);
	if (!ItemArraySize || ItemArraySize > 0x7FFF) return false;

	Item* ItemData = new Item[ItemArraySize];
	if (!Erectus::RPM(InventoryData.ItemArrayPtr, &*ItemData, ItemArraySize * sizeof(Item)))
	{
		delete[]ItemData;
		ItemData = nullptr;
		return false;
	}

	for (DWORD64 i = 0; i < ItemArraySize; i++)
	{
		if (!Erectus::Valid(ItemData[i].ReferencePtr)) continue;
		if (!Erectus::Valid(ItemData[i].DisplayPtr) || ItemData[i].Iterations < ItemData[i].DisplayPtr) continue;

		if (ErectusIni::CustomTransferSettings.UseWhitelist || ErectusIni::CustomTransferSettings.UseBlacklist)
		{
			Reference ReferenceData;
			if (!Erectus::RPM(ItemData[i].ReferencePtr, &ReferenceData, sizeof(ReferenceData))) continue;

			if (ErectusIni::CustomTransferSettings.UseWhitelist)
			{
				if (!CheckFormidArray(ReferenceData.Formid, ErectusIni::CustomTransferSettings.Whitelisted, ErectusIni::CustomTransferSettings.Whitelist, 32))
				{
					continue;
				}
			}

			if (ErectusIni::CustomTransferSettings.UseBlacklist)
			{
				if (CheckFormidArray(ReferenceData.Formid, ErectusIni::CustomTransferSettings.Blacklisted, ErectusIni::CustomTransferSettings.Blacklist, 32))
				{
					continue;
				}
			}
		}

		DWORD64 Iterations = (ItemData[i].Iterations - ItemData[i].DisplayPtr) / sizeof(ItemCount);
		if (!Iterations || Iterations > 0x7FFF) continue;

		ItemCount* ItemCountData = new ItemCount[Iterations];
		if (!Erectus::RPM(ItemData[i].DisplayPtr, &*ItemCountData, Iterations * sizeof(ItemCount)))
		{
			delete[]ItemCountData;
			ItemCountData = nullptr;
			continue;
		}

		int Count = 0;
		for (DWORD64 c = 0; c < Iterations; c++)
		{
			Count += ItemCountData[c].Count;
		}

		delete[]ItemCountData;
		ItemCountData = nullptr;

		if (Count == 0)
		{
			continue;
		}

		TransferMessage TransferMessageData;
		TransferMessageData.vtable = ErectusProcess::Exe + VTABLE_REQUESTTRANSFERITEMMSG;
		TransferMessageData.SrcFormid = SourceFormid;
		TransferMessageData.UnknownId = 0xE0001F7A;
		TransferMessageData.DstFormid = DestinationFormid;
		TransferMessageData.ItemId = ItemData[i].ItemId;
		TransferMessageData.Count = Count;
		TransferMessageData.UnknownA = 0x00000000;
		TransferMessageData.UnknownB = 0x00;
		TransferMessageData.UnknownC = 0x01;
		TransferMessageData.UnknownD = 0x00;
		TransferMessageData.UnknownE = 0x02;
		SendMessageToServer(&TransferMessageData, sizeof(TransferMessageData));
	}

	delete[]ItemData;
	ItemData = nullptr;
	return true;
}

bool ErectusMemory::GetTeleportPosition(int Index)
{
	DWORD64 LocalPlayerPtr = GetLocalPlayerPtr(true);
	if (!Erectus::Valid(LocalPlayerPtr)) return false;

	Entity LocalPlayer;
	if (!Erectus::RPM(LocalPlayerPtr, &LocalPlayer, sizeof(LocalPlayer))) return false;
	if (!Erectus::Valid(LocalPlayer.CellPtr)) return false;

	DWORD CellFormid;
	if (!Erectus::RPM(LocalPlayer.CellPtr + 0x20, &CellFormid, sizeof(CellFormid))) return false;

	ErectusIni::CustomTeleportSettings.TeleportEntryData[Index].Destination[0] = LocalPlayer.Position[0];
	ErectusIni::CustomTeleportSettings.TeleportEntryData[Index].Destination[1] = LocalPlayer.Position[1];
	ErectusIni::CustomTeleportSettings.TeleportEntryData[Index].Destination[2] = LocalPlayer.Position[2];
	ErectusIni::CustomTeleportSettings.TeleportEntryData[Index].Destination[3] = LocalPlayer.Yaw;
	ErectusIni::CustomTeleportSettings.TeleportEntryData[Index].CellFormid = CellFormid;

	return true;
}

bool ErectusMemory::RequestTeleport(int Index)
{
	DWORD64 CellPtr = GetPtr(ErectusIni::CustomTeleportSettings.TeleportEntryData[Index].CellFormid);
	if (!Erectus::Valid(CellPtr)) return false;

	RequestTeleportMessage RequestTeleportMessageData;
	RequestTeleportMessageData.vtable = ErectusProcess::Exe + VTABLE_REQUESTTELEPORTTOLOCATIONMSG;
	RequestTeleportMessageData.PositionX = ErectusIni::CustomTeleportSettings.TeleportEntryData[Index].Destination[0];
	RequestTeleportMessageData.PositionY = ErectusIni::CustomTeleportSettings.TeleportEntryData[Index].Destination[1];
	RequestTeleportMessageData.PositionZ = ErectusIni::CustomTeleportSettings.TeleportEntryData[Index].Destination[2];
	RequestTeleportMessageData.RotationX = 0.0f;
	RequestTeleportMessageData.RotationY = 0.0f;
	RequestTeleportMessageData.RotationZ = ErectusIni::CustomTeleportSettings.TeleportEntryData[Index].Destination[3];
	RequestTeleportMessageData.CellPtr = CellPtr;

	return SendMessageToServer(&RequestTeleportMessageData, sizeof(RequestTeleportMessageData));
}

DWORD ErectusMemory::GetLocalPlayerFormid()
{
	DWORD64 LocalPlayerPtr = GetLocalPlayerPtr(true);
	if (!Erectus::Valid(LocalPlayerPtr)) return 0;

	Entity LocalPlayer;
	if (!Erectus::RPM(LocalPlayerPtr, &LocalPlayer, sizeof(LocalPlayer))) return 0;

	return LocalPlayer.Formid;
}

DWORD ErectusMemory::GetStashFormid()
{
	DWORD64 LocalPlayerPtr = GetLocalPlayerPtr(true);
	if (!Erectus::Valid(LocalPlayerPtr)) return 0;

	Entity LocalPlayer;
	if (!Erectus::RPM(LocalPlayerPtr, &LocalPlayer, sizeof(LocalPlayer))) return 0;

	return LocalPlayer.Formid0C24;
}

bool ErectusMemory::FreezeActionPoints(DWORD64* FreezeApPage, bool* FreezeApPageValid, bool State)
{
	if (!*FreezeApPage)
	{
		DWORD64 Page = Erectus::AllocEx(sizeof(FreezeAp));
		if (!Page) return false;
		*FreezeApPage = Page;
	}

	BYTE FreezeApOn[]
	{
		0x0F, 0x1F, 0x40, 0x00,							//nop [rax+00]
		0x48, 0xBF,										//mov rdi (Page)
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	//Page (mov rdi)
		0xFF, 0xE7,										//jmp rdi
		0x0F, 0x1F, 0x40, 0x00,							//nop [rax+00]
	};

	BYTE FreezeApOff[]
	{
		0x8B, 0xD6,										//mov edx, esi
		0x48, 0x8B, 0xC8,								//mov rcx, rax
		0x48, 0x8B, 0x5C, 0x24, 0x30,					//mov rbx, [rsp+30]
		0x48, 0x8B, 0x74, 0x24, 0x38,					//mov rsi, [rsp+38]
		0x48, 0x83, 0xC4, 0x20,							//add rsp, 20
		0x5F,											//pop rdi
	};

	BYTE FreezeApCheck[sizeof(FreezeApOff)];

	if (!Erectus::RPM(ErectusProcess::Exe + OFFSET_AV_REGEN, &FreezeApCheck, sizeof(FreezeApCheck)))
	{
		return false;
	}

	DWORD64 PageCheck;
	memcpy(&PageCheck, &FreezeApCheck[0x6], sizeof(DWORD64));

	if (Erectus::Valid(PageCheck) && PageCheck != *FreezeApPage)
	{
		for (int i = 0; i < 0x6; i++) if (FreezeApCheck[i] != FreezeApOn[i]) return false;
		if (!Erectus::WPM(ErectusProcess::Exe + OFFSET_AV_REGEN, &FreezeApOff, sizeof(FreezeApOff))) return false;
		Erectus::FreeEx(PageCheck);
	}

	if (State)
	{
		FreezeAp FreezeApData;
		FreezeApData.FreezeApEnabled = ErectusIni::CustomLocalPlayerSettings.FreezeApEnabled;

		if (*FreezeApPageValid)
		{
			FreezeAp FreezeApPageCheck;
			if (!Erectus::RPM(*FreezeApPage, &FreezeApPageCheck, sizeof(FreezeApPageCheck))) return false;
			if (!memcmp(&FreezeApData, &FreezeApPageCheck, sizeof(FreezeApPageCheck))) return true;
			return Erectus::WPM(*FreezeApPage, &FreezeApData, sizeof(FreezeApData));
		}
		else
		{
			if (!Erectus::WPM(*FreezeApPage, &FreezeApData, sizeof(FreezeApData))) return false;
			memcpy(&FreezeApOn[0x6], &*FreezeApPage, sizeof(DWORD64));
			if (!Erectus::WPM(ErectusProcess::Exe + OFFSET_AV_REGEN, &FreezeApOn, sizeof(FreezeApOn))) return false;
			*FreezeApPageValid = true;
		}
	}
	else
	{
		if (PageCheck == *FreezeApPage)
		{
			Erectus::WPM(ErectusProcess::Exe + OFFSET_AV_REGEN, &FreezeApOff, sizeof(FreezeApOff));
		}

		if (*FreezeApPage)
		{
			if (Erectus::FreeEx(*FreezeApPage))
			{
				*FreezeApPage = 0;
				*FreezeApPageValid = false;
			}
		}
	}

	return true;
}

bool ErectusMemory::SetClientState(DWORD64 ClientState)
{
	DWORD64 LocalPlayerPtr = GetLocalPlayerPtr(true);
	if (!Erectus::Valid(LocalPlayerPtr)) return false;

	if (InsideInteriorCell())
	{
		return false;
	}

	ClientStateMsg ClientStateMsgData;
	ClientStateMsgData.vtable = ErectusProcess::Exe + VTABLE_CLIENTSTATEMSG;
	ClientStateMsgData.ClientState = ClientState;

	return SendMessageToServer(&ClientStateMsgData, sizeof(ClientStateMsgData));
}

bool ErectusMemory::PositionSpoofing(bool State)
{
	BYTE PositionSpoofingOn[] = { 0xBA, 0x00, 0x00, 0x00, 0x00, 0xEB, 0x11, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC };
	BYTE PositionSpoofingOff[] = { 0xBA, 0x01, 0x00, 0xF8, 0xFF, 0x3B, 0xC2, 0x7C, 0x0F, 0x8B, 0xD0, 0x41, 0xB8, 0xFF, 0xFF, 0x07, 0x00, 0x41, 0x3B, 0xC0, 0x41, 0x0F, 0x4F, 0xD0 };
	BYTE PositionSpoofingCheck[sizeof(PositionSpoofingOff)];

	if (!Erectus::RPM(ErectusProcess::Exe + OFFSET_SERVER_POSITION, &PositionSpoofingCheck, sizeof(PositionSpoofingCheck)))
	{
		return false;
	}

	if (!State)
	{
		PositionSpoofingCheck[1] = 0x00;
		PositionSpoofingCheck[2] = 0x00;
		PositionSpoofingCheck[3] = 0x00;
		PositionSpoofingCheck[4] = 0x00;

		if (!memcmp(PositionSpoofingCheck, PositionSpoofingOn, sizeof(PositionSpoofingOn)))
		{
			return Erectus::WPM(ErectusProcess::Exe + OFFSET_SERVER_POSITION, &PositionSpoofingOff, sizeof(PositionSpoofingOff));
		}
		else
		{
			return true;
		}
	}

	int SpoofingHeightCheck;
	memcpy(&SpoofingHeightCheck, &PositionSpoofingCheck[1], sizeof(SpoofingHeightCheck));
	memcpy(&PositionSpoofingOn[1], &ErectusIni::CustomLocalPlayerSettings.PositionSpoofingHeight, sizeof(ErectusIni::CustomLocalPlayerSettings.PositionSpoofingHeight));

	if (!memcmp(PositionSpoofingCheck, PositionSpoofingOn, sizeof(PositionSpoofingOn)))
	{
		return true;
	}
	else if (!memcmp(PositionSpoofingCheck, PositionSpoofingOff, sizeof(PositionSpoofingOff)))
	{
		return Erectus::WPM(ErectusProcess::Exe + OFFSET_SERVER_POSITION, &PositionSpoofingOn, sizeof(PositionSpoofingOn));
	}
	else if (SpoofingHeightCheck != ErectusIni::CustomLocalPlayerSettings.PositionSpoofingHeight)
	{
		if (PositionSpoofingCheck[0] != 0xBA || SpoofingHeightCheck < -524287 || SpoofingHeightCheck > 524287)
		{
			return false;
		}
		else
		{
			return Erectus::WPM(ErectusProcess::Exe + OFFSET_SERVER_POSITION, &PositionSpoofingOn, sizeof(PositionSpoofingOn));
		}
	}
	else
	{
		return false;
	}
}

DWORD ErectusMemory::GetEntityId(Entity EntityData)
{
	if (!(EntityData.IdValue[0] & 1)) return 0;

	DWORD v1;
	memcpy(&v1, EntityData.IdValue, sizeof(v1));

	DWORD v2 = v1 >> 1;
	DWORD v3 = v2 + v2;

	DWORD v4;
	if (!Erectus::RPM(ErectusProcess::Exe + OFFSET_ENTITY_ID + v3 * 0x8, &v4, sizeof(v4))) return 0;

	DWORD v5 = v4 & 0x7FF80000;
	DWORD v6 = v5 | v2;

	return v6;
}

bool ErectusMemory::SendHitsToServer(Hits* HitsData, size_t HitsDataSize)
{
	size_t AllocSize = sizeof(ExternalFunction) + sizeof(RequestHitsOnActors) + HitsDataSize;
	DWORD64 AllocAddress = Erectus::AllocEx(AllocSize);
	if (AllocAddress == 0) return false;

	ExternalFunction ExternalFunctionData;
	ExternalFunctionData.Address = ErectusProcess::Exe + OFFSET_MESSAGE_SENDER;
	ExternalFunctionData.RCX = AllocAddress + sizeof(ExternalFunction);
	ExternalFunctionData.RDX = 0;
	ExternalFunctionData.R8 = 0;
	ExternalFunctionData.R9 = 0;

	BYTE* PageData = new BYTE[AllocSize];
	memset(PageData, 0x00, AllocSize);

	RequestHitsOnActors RequestHitsOnActorsData;
	memset(&RequestHitsOnActorsData, 0x00, sizeof(RequestHitsOnActors));

	RequestHitsOnActorsData.vtable = ErectusProcess::Exe + VTABLE_REQUESTHITSONACTORS;
	RequestHitsOnActorsData.HitsArrayPtr = AllocAddress + sizeof(ExternalFunction) + sizeof(RequestHitsOnActors);
	RequestHitsOnActorsData.HitsArrayEnd = AllocAddress + sizeof(ExternalFunction) + sizeof(RequestHitsOnActors) + HitsDataSize;

	memcpy(PageData, &ExternalFunctionData, sizeof(ExternalFunctionData));
	memcpy(&PageData[sizeof(ExternalFunction)], &RequestHitsOnActorsData, sizeof(RequestHitsOnActorsData));
	memcpy(&PageData[sizeof(ExternalFunction) + sizeof(RequestHitsOnActors)], &*HitsData, HitsDataSize);

	bool PageWritten = Erectus::WPM(AllocAddress, &*PageData, AllocSize);

	delete[]PageData;
	PageData = nullptr;

	if (!PageWritten)
	{
		Erectus::FreeEx(AllocAddress);
		return false;
	}

	DWORD64 ParamAddress = AllocAddress + sizeof(ExternalFunction::ASM);
	HANDLE Thread = CreateRemoteThread(ErectusProcess::Handle, NULL, 0, LPTHREAD_START_ROUTINE(AllocAddress), LPVOID(ParamAddress), 0, 0);

	if (Thread == NULL)
	{
		Erectus::FreeEx(AllocAddress);
		return false;
	}

	DWORD ThreadResult = WaitForSingleObject(Thread, 3000);
	CloseHandle(Thread);

	if (ThreadResult == WAIT_TIMEOUT)
	{
		return false;
	}

	Erectus::FreeEx(AllocAddress);
	return true;
}

bool ErectusMemory::SendDamage(DWORD WeaponId, BYTE* ShotsHit, BYTE* ShotsFired, BYTE Count)
{
	if (!MessagePatcher(AllowMessages))
	{
		return false;
	}

	if (!AllowMessages)
	{
		return false;
	}

	if (!WeaponId)
	{
		return false;
	}

	bool IsPlayer = false;
	bool TargetValid = LockedTargetValid(&IsPlayer);
	if (!ErectusIni::CustomTargetSettings.DirectPlayers && IsPlayer)
	{
		TargetValid = false;
	}
	else if (!ErectusIni::CustomTargetSettings.DirectNPCs && !IsPlayer)
	{
		TargetValid = false;
	}

	if (!TargetValid)
	{
		return false;
	}

	DWORD64 LocalPlayerPtr = GetLocalPlayerPtr(true);
	if (!Erectus::Valid(LocalPlayerPtr)) return false;

	Entity LocalPlayer;
	if (!Erectus::RPM(LocalPlayerPtr, &LocalPlayer, sizeof(LocalPlayer))) return false;

	DWORD LocalPlayerId = GetEntityId(LocalPlayer);
	if (!LocalPlayerId) return false;

	Entity Target;
	if (!Erectus::RPM(TargetLockingPtr, &Target, sizeof(Target))) return false;

	DWORD TargetId = GetEntityId(Target);
	if (!TargetId) return false;

	Hits* HitsData = new Hits[Count];
	memset(HitsData, 0x00, Count * sizeof(Hits));

	if (*ShotsHit == 0 || *ShotsHit == 255)
	{
		*ShotsHit = 1;
	}

	if (*ShotsFired == 255)
	{
		*ShotsFired = 0;
	}

	for (BYTE i = 0; i < Count; i++)
	{
		HitsData[i].ValueA = LocalPlayerId;
		HitsData[i].ValueB = TargetId;
		HitsData[i].ValueC = 0;
		HitsData[i].InitializationType = 0x3;
		HitsData[i].uiWeaponServerID = WeaponId;
		HitsData[i].LIMB_ENUM = 0xFFFFFFFF;
		HitsData[i].HitEffectID = 0;
		HitsData[i].uEquipIndex = 0;
		HitsData[i].uAckIndex = *ShotsHit;
		HitsData[i].uFireID = *ShotsFired;
		HitsData[i].bPredictedKill = 0;
		HitsData[i].Padding0023 = 0;
		HitsData[i].ExplosionLocationX = 0.0f;
		HitsData[i].ExplosionLocationY = 0.0f;
		HitsData[i].ExplosionLocationZ = 0.0f;
		HitsData[i].fProjectilePower = 1.0f;
		HitsData[i].bVatsAttack = 0;
		HitsData[i].bVatsCritical = 0;
		HitsData[i].bTargetWasDead = 0;
		HitsData[i].Padding0037 = 0;

		if (ErectusIni::CustomTargetSettings.SendDamageMax < 10)
		{
			if (Erectus::GetRangedInt(1, 10) <= int(10 - ErectusIni::CustomTargetSettings.SendDamageMax))
			{
				if (*ShotsHit == 0 || *ShotsHit == 255)
				{
					*ShotsHit = 1;
				}
				else
				{
					*ShotsHit += 1;
				}
			}
			else
			{
				*ShotsHit = 1;
			}
		}
		else
		{
			*ShotsHit = 1;
		}

		for (int c = 0; c < Erectus::GetRangedInt(1, 6); c++)
		{
			if (*ShotsFired == 255)
			{
				*ShotsFired = 0;
			}
			else
			{
				*ShotsFired += 1;
			}
		}
	}

	bool Result = SendHitsToServer(HitsData, Count * sizeof(Hits));

	delete[]HitsData;

	return Result;
}

DWORD64 ErectusMemory::GetNukeCodePtr(DWORD Formid)
{
	ReferenceList QuestTextList;
	if (!Erectus::RPM(ErectusProcess::Exe + OFFSET_NUKE_CODE, &QuestTextList, sizeof(QuestTextList))) return 0;
	if (!Erectus::Valid(QuestTextList.ArrayPtr) || !QuestTextList.ArraySize || QuestTextList.ArraySize > 0x7FFF) return 0;

	DWORD64* QuestTextArray = new DWORD64[QuestTextList.ArraySize];
	if (!Erectus::RPM(QuestTextList.ArrayPtr, &*QuestTextArray, QuestTextList.ArraySize * sizeof(DWORD64)))
	{
		delete[]QuestTextArray;
		QuestTextArray = nullptr;
		return 0;
	}

	DWORD64 NukeCodePtr = 0;
	for (int i = 0; i < QuestTextList.ArraySize; i++)
	{
		if (!Erectus::Valid(QuestTextArray[i])) continue;

		BGSQuestText BGSQuestTextData;
		if (!Erectus::RPM(QuestTextArray[i], &BGSQuestTextData, sizeof(BGSQuestTextData))) continue;
		if (!Erectus::Valid(BGSQuestTextData.FormidPtr) || !Erectus::Valid(BGSQuestTextData.CodePtr)) continue;

		DWORD FormidCheck;
		if (!Erectus::RPM(BGSQuestTextData.FormidPtr + 0x4, &FormidCheck, sizeof(FormidCheck))) continue;
		if (FormidCheck != Formid) continue;

		NukeCodePtr = BGSQuestTextData.CodePtr;
		break;
	}

	delete[]QuestTextArray;
	QuestTextArray = nullptr;
	return NukeCodePtr;
}

bool ErectusMemory::GetNukeCode(DWORD Formid, int* NukeCode)
{
	DWORD64 NukeCodePtr = GetNukeCodePtr(Formid);
	if (!NukeCodePtr) return false;

	float NukeCodeArray[16];
	if (!Erectus::RPM(NukeCodePtr, &NukeCodeArray, sizeof(NukeCodeArray))) return false;

	for (int i = 0; i < 8; i++)
	{
		if (NukeCodeArray[i * 2 + 1] < 0.0f || NukeCodeArray[i * 2 + 1] > 9.0f)
		{
			memset(NukeCode, 0x00, sizeof(int) * 8);
			return false;
		}
		else
		{
			NukeCode[i] = int(NukeCodeArray[i * 2 + 1]);
		}
	}

	return true;
}

DWORD ErectusMemory::GetFavoritedWeaponId(BYTE Index)
{
	DWORD64 LocalPlayerPtr = GetLocalPlayerPtr(true);
	if (!Erectus::Valid(LocalPlayerPtr)) return 0;

	Entity LocalPlayer;
	if (!Erectus::RPM(LocalPlayerPtr, &LocalPlayer, sizeof(LocalPlayer))) return 0;
	if (!Erectus::Valid(LocalPlayer.InventoryPtr)) return 0;

	Inventory InventoryData;
	if (!Erectus::RPM(LocalPlayer.InventoryPtr, &InventoryData, sizeof(InventoryData))) return 0;
	if (!Erectus::Valid(InventoryData.ItemArrayPtr) || InventoryData.ItemArrayEnd < InventoryData.ItemArrayPtr) return 0;

	DWORD64 ItemArraySize = (InventoryData.ItemArrayEnd - InventoryData.ItemArrayPtr) / sizeof(Item);
	if (!ItemArraySize || ItemArraySize > 0x7FFF) return 0;

	Item* ItemData = new Item[ItemArraySize];
	if (!Erectus::RPM(InventoryData.ItemArrayPtr, &*ItemData, ItemArraySize * sizeof(Item)))
	{
		delete[]ItemData;
		ItemData = nullptr;
		return 0;
	}

	for (DWORD64 i = 0; i < ItemArraySize; i++)
	{
		if (!Erectus::Valid(ItemData[i].ReferencePtr)) continue;
		if (ItemData[i].FavoriteIndex != Index) continue;

		Reference ReferenceData;
		if (!Erectus::RPM(ItemData[i].ReferencePtr, &ReferenceData, sizeof(ReferenceData))) break;
		if (ReferenceData.vtable != ErectusProcess::Exe + VTABLE_TESOBJECTWEAP) break;

		DWORD WeaponId = ItemData[i].ItemId;
		delete[]ItemData;
		ItemData = nullptr;
		return WeaponId;
	}

	delete[]ItemData;
	ItemData = nullptr;
	return 0;
}

char ErectusMemory::GetFavoriteSlot(BYTE FavoriteIndex)
{
	switch (FavoriteIndex)
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

DWORD64 ErectusMemory::rttiGetNamePtr(DWORD64 vtable)
{
	DWORD64 Buffer;
	if (!Erectus::RPM(vtable - 0x8, &Buffer, sizeof(Buffer))) return 0;
	if (!Erectus::Valid(Buffer)) return 0;

	DWORD Offset;
	if (!Erectus::RPM(Buffer + 0xC, &Offset, sizeof(Offset))) return 0;
	if (Offset == 0 || Offset > 0x7FFFFFFF) return 0;

	return ErectusProcess::Exe + Offset + 0x10;
}

char* ErectusMemory::GetInstancedItemName(DWORD64 DisplayPtr)
{
	if (!Erectus::Valid(DisplayPtr)) return nullptr;

	DWORD64 InstancedArrayPtr;
	if (!Erectus::RPM(DisplayPtr, &InstancedArrayPtr, sizeof(InstancedArrayPtr))) return nullptr;
	if (!Erectus::Valid(InstancedArrayPtr)) return nullptr;

	ItemInstancedArray ItemInstancedArrayData;
	if (!Erectus::RPM(InstancedArrayPtr, &ItemInstancedArrayData, sizeof(ItemInstancedArrayData))) return nullptr;
	if (!Erectus::Valid(ItemInstancedArrayData.ArrayPtr) || ItemInstancedArrayData.ArrayEnd < ItemInstancedArrayData.ArrayPtr) return nullptr;

	DWORD64 InstancedArraySize = (ItemInstancedArrayData.ArrayEnd - ItemInstancedArrayData.ArrayPtr) / sizeof(DWORD64);
	if (!InstancedArraySize || InstancedArraySize > 0x7FFF) return nullptr;

	DWORD64* InstancedArray = new DWORD64[InstancedArraySize];
	if (!Erectus::RPM(ItemInstancedArrayData.ArrayPtr, &*InstancedArray, InstancedArraySize * sizeof(DWORD64)))
	{
		delete[]InstancedArray;
		InstancedArray = nullptr;
		return nullptr;
	}

	for (DWORD64 i = 0; i < InstancedArraySize; i++)
	{
		if (!Erectus::Valid(InstancedArray[i])) continue;

		ExtraTextDisplayData ExtraTextDisplayDataData;
		if (!Erectus::RPM(InstancedArray[i], &ExtraTextDisplayDataData, sizeof(ExtraTextDisplayDataData))) continue;

		DWORD64 rttiNamePtr = rttiGetNamePtr(ExtraTextDisplayDataData.vtable);
		if (!rttiNamePtr) continue;

		char rttiNameCheck[sizeof(".?AVExtraTextDisplayData@@")];
		if (!Erectus::RPM(rttiNamePtr, &rttiNameCheck, sizeof(rttiNameCheck))) continue;
		if (strcmp(rttiNameCheck, ".?AVExtraTextDisplayData@@")) continue;

		delete[]InstancedArray;
		InstancedArray = nullptr;
		return GetEntityName(ExtraTextDisplayDataData.InstancedNamePtr);
	}

	delete[]InstancedArray;
	InstancedArray = nullptr;
	return nullptr;
}

char** ErectusMemory::GetFavoritedWeapons()
{
	DWORD64 LocalPlayerPtr = GetLocalPlayerPtr(true);
	if (!Erectus::Valid(LocalPlayerPtr)) return nullptr;

	Entity LocalPlayer;
	if (!Erectus::RPM(LocalPlayerPtr, &LocalPlayer, sizeof(LocalPlayer))) return nullptr;
	if (!Erectus::Valid(LocalPlayer.InventoryPtr)) return nullptr;

	Inventory InventoryData;
	if (!Erectus::RPM(LocalPlayer.InventoryPtr, &InventoryData, sizeof(InventoryData))) return nullptr;
	if (!Erectus::Valid(InventoryData.ItemArrayPtr) || InventoryData.ItemArrayEnd < InventoryData.ItemArrayPtr) return nullptr;

	DWORD64 ItemArraySize = (InventoryData.ItemArrayEnd - InventoryData.ItemArrayPtr) / sizeof(Item);
	if (!ItemArraySize || ItemArraySize > 0x7FFF) return nullptr;

	Item* ItemData = new Item[ItemArraySize];
	if (!Erectus::RPM(InventoryData.ItemArrayPtr, &*ItemData, ItemArraySize * sizeof(Item)))
	{
		delete[]ItemData;
		ItemData = nullptr;
		return nullptr;
	}

	char** Array = new char* [13];
	Array[0] = new char[sizeof("[?] No Weapon Selected")];
	sprintf_s(Array[0], sizeof("[?] No Weapon Selected"), "[?] No Weapon Selected");
	for (int i = 1; i < 13; i++)
	{
		Array[i] = nullptr;
	}

	for (DWORD64 i = 0; i < ItemArraySize; i++)
	{
		if (!Erectus::Valid(ItemData[i].ReferencePtr)) continue;
		if (ItemData[i].FavoriteIndex > 12) continue;

		Reference ReferenceData;
		if (!Erectus::RPM(ItemData[i].ReferencePtr, &ReferenceData, sizeof(ReferenceData))) continue;
		if (ReferenceData.vtable != ErectusProcess::Exe + VTABLE_TESOBJECTWEAP) continue;

		char* TempWeaponName = ErectusMemory::GetInstancedItemName(ItemData[i].DisplayPtr);
		if (TempWeaponName == nullptr)
		{
			TempWeaponName = GetEntityName(ReferenceData.NamePtr0098);
			if (TempWeaponName == nullptr)
			{
				continue;
			}
		}

		int ArrayTextSize = sizeof("[?] ") + Erectus::GetTextLength(TempWeaponName);
		char* ArrayText = new char[ArrayTextSize];
		sprintf_s(ArrayText, ArrayTextSize, "[%c] %s", GetFavoriteSlot(ItemData[i].FavoriteIndex), TempWeaponName);

		Array[ItemData[i].FavoriteIndex + 1] = ArrayText;
		delete[]TempWeaponName;
		TempWeaponName = nullptr;
	}

	for (int i = 1; i < 13; i++)
	{
		if (Array[i] == nullptr)
		{
			Array[i] = new char[sizeof("[?] Favorited Item InValid")];
			sprintf_s(Array[i], sizeof("[?] Favorited Item InValid"), "[%c] Favorited Item InValid", GetFavoriteSlot(BYTE(i - 1)));
		}
	}

	delete[]ItemData;
	ItemData = nullptr;
	return Array;
}

char* ErectusMemory::GetFavoritedWeaponText(BYTE Index)
{
	DWORD64 LocalPlayerPtr = GetLocalPlayerPtr(true);
	if (!Erectus::Valid(LocalPlayerPtr)) return nullptr;

	Entity LocalPlayer;
	if (!Erectus::RPM(LocalPlayerPtr, &LocalPlayer, sizeof(LocalPlayer))) return nullptr;
	if (!Erectus::Valid(LocalPlayer.InventoryPtr)) return nullptr;

	Inventory InventoryData;
	if (!Erectus::RPM(LocalPlayer.InventoryPtr, &InventoryData, sizeof(InventoryData))) return nullptr;
	if (!Erectus::Valid(InventoryData.ItemArrayPtr) || InventoryData.ItemArrayEnd < InventoryData.ItemArrayPtr) return nullptr;

	DWORD64 ItemArraySize = (InventoryData.ItemArrayEnd - InventoryData.ItemArrayPtr) / sizeof(Item);
	if (!ItemArraySize || ItemArraySize > 0x7FFF) return nullptr;

	Item* ItemData = new Item[ItemArraySize];
	if (!Erectus::RPM(InventoryData.ItemArrayPtr, &*ItemData, ItemArraySize * sizeof(Item)))
	{
		delete[]ItemData;
		ItemData = nullptr;
		return nullptr;
	}

	for (DWORD64 i = 0; i < ItemArraySize; i++)
	{
		if (!Erectus::Valid(ItemData[i].ReferencePtr)) continue;
		if (ItemData[i].FavoriteIndex != Index) continue;

		Reference ReferenceData;
		if (!Erectus::RPM(ItemData[i].ReferencePtr, &ReferenceData, sizeof(ReferenceData))) break;
		if (ReferenceData.vtable != ErectusProcess::Exe + VTABLE_TESOBJECTWEAP) break;

		char* TempWeaponName = ErectusMemory::GetInstancedItemName(ItemData[i].DisplayPtr);
		if (TempWeaponName == nullptr)
		{
			TempWeaponName = GetEntityName(ReferenceData.NamePtr0098);
			if (TempWeaponName == nullptr)
			{
				continue;
			}
		}

		int WeaponTextSize = sizeof("[?] ") + Erectus::GetTextLength(TempWeaponName);
		char* WeaponText = new char[WeaponTextSize];
		sprintf_s(WeaponText, WeaponTextSize, "[%c] %s", GetFavoriteSlot(ItemData[i].FavoriteIndex), TempWeaponName);

		delete[]TempWeaponName;
		TempWeaponName = nullptr;

		delete[]ItemData;
		ItemData = nullptr;
		return WeaponText;
	}

	delete[]ItemData;
	ItemData = nullptr;
	return nullptr;
}

bool ErectusMemory::EntityInventoryValid(Entity EntityData)
{
	if (!Erectus::Valid(EntityData.InventoryPtr))
	{
		return false;
	}

	Inventory InventoryData;
	if (!Erectus::RPM(EntityData.InventoryPtr, &InventoryData, sizeof(InventoryData))) return false;
	if (!Erectus::Valid(InventoryData.ItemArrayPtr) || InventoryData.ItemArrayEnd < InventoryData.ItemArrayPtr) return false;

	DWORD64 ItemArraySize = (InventoryData.ItemArrayEnd - InventoryData.ItemArrayPtr) / sizeof(Item);
	if (!ItemArraySize || ItemArraySize > 0x7FFF) return false;

	Item* ItemData = new Item[ItemArraySize];
	if (!Erectus::RPM(InventoryData.ItemArrayPtr, &*ItemData, ItemArraySize * sizeof(Item)))
	{
		delete[]ItemData;
		ItemData = nullptr;
		return false;
	}

	for (DWORD64 i = 0; i < ItemArraySize; i++)
	{
		if (!Erectus::Valid(ItemData[i].ReferencePtr)) continue;

		Reference ReferenceData;
		if (!Erectus::RPM(ItemData[i].ReferencePtr, &ReferenceData, sizeof(ReferenceData))) continue;
		if ((ReferenceData.RecordFlagA >> 2) & 1) continue;

		delete[]ItemData;
		ItemData = nullptr;
		return true;
	}

	delete[]ItemData;
	ItemData = nullptr;
	return false;
}

bool ErectusMemory::AllowLegendaryWeapons(EntityLooterSettings* CustomEntityLooterSettings)
{
	if (!CustomEntityLooterSettings->EntityLooterAllWeaponsEnabled)
	{
		if (CustomEntityLooterSettings->EntityLooterOneStarWeaponsEnabled)
		{
			return true;
		}
		else if (CustomEntityLooterSettings->EntityLooterTwoStarWeaponsEnabled)
		{
			return true;
		}
		else if (CustomEntityLooterSettings->EntityLooterThreeStarWeaponsEnabled)
		{
			return true;
		}
	}

	return false;
}

bool ErectusMemory::AllowLegendaryArmor(EntityLooterSettings* CustomEntityLooterSettings)
{
	if (!CustomEntityLooterSettings->EntityLooterAllArmorEnabled)
	{
		if (CustomEntityLooterSettings->EntityLooterOneStarArmorEnabled)
		{
			return true;
		}
		else if (CustomEntityLooterSettings->EntityLooterTwoStarArmorEnabled)
		{
			return true;
		}
		else if (CustomEntityLooterSettings->EntityLooterThreeStarArmorEnabled)
		{
			return true;
		}
	}

	return false;
}

bool ErectusMemory::CheckEntityLooterItem(DWORD Formid, DWORD64 EntityFlag, EntityLooterSettings* CustomEntityLooterSettings, bool LegendaryWeaponsEnabled, bool LegendaryArmorEnabled)
{
	if (CustomEntityLooterSettings->EntityLooterCapsEnabled)
	{
		if (Formid == 0x0000000F)
		{
			return true;
		}
	}

	if (CustomEntityLooterSettings->EntityLooterListEnabled)
	{
		if (CheckFormidArray(Formid, CustomEntityLooterSettings->EntityLooterEnabledList, CustomEntityLooterSettings->EntityLooterFormidList, 100))
		{
			return true;
		}
	}

	if (EntityFlag & CUSTOM_ENTRY_WEAPON)
	{
		if (CustomEntityLooterSettings->EntityLooterAllWeaponsEnabled)
		{
			return true;
		}
		else
		{
			return LegendaryWeaponsEnabled;
		}
	}
	else if (EntityFlag & CUSTOM_ENTRY_ARMOR)
	{
		if (CustomEntityLooterSettings->EntityLooterAllArmorEnabled)
		{
			return true;
		}
		else
		{
			return LegendaryArmorEnabled;
		}
	}
	else if (EntityFlag & CUSTOM_ENTRY_AMMO)
	{
		return CustomEntityLooterSettings->EntityLooterAmmoEnabled;
	}
	else if (EntityFlag & CUSTOM_ENTRY_MOD)
	{
		return CustomEntityLooterSettings->EntityLooterModsEnabled;
	}
	else if (EntityFlag & CUSTOM_ENTRY_JUNK)
	{
		return CustomEntityLooterSettings->EntityLooterJunkEnabled;
	}
	else if (EntityFlag & CUSTOM_ENTRY_AID)
	{
		return CustomEntityLooterSettings->EntityLooterAidEnabled;
	}
	else if (EntityFlag & CUSTOM_ENTRY_TREASURE_MAP)
	{
		return CustomEntityLooterSettings->EntityLooterTreasureMapsEnabled;
	}
	else if (EntityFlag & CUSTOM_ENTRY_PLAN)
	{
		if (EntityFlag & CUSTOM_ENTRY_KNOWN_RECIPE)
		{
			return CustomEntityLooterSettings->EntityLooterKnownPlansEnabled;
		}
		else if (EntityFlag & CUSTOM_ENTRY_UNKNOWN_RECIPE)
		{
			return CustomEntityLooterSettings->EntityLooterUnknownPlansEnabled;
		}
		else if (EntityFlag & CUSTOM_ENTRY_FAILED_RECIPE)
		{
			return CustomEntityLooterSettings->EntityLooterUnknownPlansEnabled;
		}
	}
	else if (EntityFlag & CUSTOM_ENTRY_MISC)
	{
		return CustomEntityLooterSettings->EntityLooterMiscEnabled;
	}

	if (CustomEntityLooterSettings->EntityLooterUnlistedEnabled)
	{
		return true;
	}

	return false;
}

bool ErectusMemory::IsLegendaryFormid(DWORD Formid)
{
	for (size_t i = 0; i < sizeof(LegendaryFormidArray) / sizeof(DWORD); i++)
	{
		if (Formid == LegendaryFormidArray[i])
		{
			return true;
		}
	}

	return false;
}

BYTE ErectusMemory::GetLegendaryRank(DWORD64 DisplayPtr)
{
	if (!Erectus::Valid(DisplayPtr)) return 0;

	DWORD64 InstancedArrayPtr;
	if (!Erectus::RPM(DisplayPtr, &InstancedArrayPtr, sizeof(InstancedArrayPtr))) return 0;
	if (!Erectus::Valid(InstancedArrayPtr)) return 0;

	ItemInstancedArray ItemInstancedArrayData;
	if (!Erectus::RPM(InstancedArrayPtr, &ItemInstancedArrayData, sizeof(ItemInstancedArrayData))) return 0;
	if (!Erectus::Valid(ItemInstancedArrayData.ArrayPtr) || ItemInstancedArrayData.ArrayEnd < ItemInstancedArrayData.ArrayPtr) return 0;

	DWORD64 InstancedArraySize = (ItemInstancedArrayData.ArrayEnd - ItemInstancedArrayData.ArrayPtr) / sizeof(DWORD64);
	if (!InstancedArraySize || InstancedArraySize > 0x7FFF) return 0;

	DWORD64* InstancedArray = new DWORD64[InstancedArraySize];
	if (!Erectus::RPM(ItemInstancedArrayData.ArrayPtr, &*InstancedArray, InstancedArraySize * sizeof(DWORD64)))
	{
		delete[]InstancedArray;
		InstancedArray = nullptr;
		return 0;
	}

	DWORD64 ObjectInstanceExtraPtr = 0;

	for (DWORD64 i = 0; i < InstancedArraySize; i++)
	{
		if (!Erectus::Valid(InstancedArray[i])) continue;

		ExtraTextDisplayData ExtraTextDisplayDataData;
		if (!Erectus::RPM(InstancedArray[i], &ExtraTextDisplayDataData, sizeof(ExtraTextDisplayDataData))) continue;

		DWORD64 rttiNamePtr = rttiGetNamePtr(ExtraTextDisplayDataData.vtable);
		if (!rttiNamePtr) continue;

		char rttiNameCheck[sizeof(".?AVBGSObjectInstanceExtra@@")];
		if (!Erectus::RPM(rttiNamePtr, &rttiNameCheck, sizeof(rttiNameCheck))) continue;
		if (strcmp(rttiNameCheck, ".?AVBGSObjectInstanceExtra@@")) continue;

		ObjectInstanceExtraPtr = InstancedArray[i];
		break;
	}

	delete[]InstancedArray;
	InstancedArray = nullptr;

	if (!ObjectInstanceExtraPtr)
	{
		return 0;
	}

	ObjectInstanceExtra ObjectInstanceExtraData;
	if (!Erectus::RPM(ObjectInstanceExtraPtr, &ObjectInstanceExtraData, sizeof(ObjectInstanceExtraData))) return 0;
	if (!Erectus::Valid(ObjectInstanceExtraData.ModDataPtr)) return 0;

	ModInstance ModInstanceData;
	if (!Erectus::RPM(ObjectInstanceExtraData.ModDataPtr, &ModInstanceData, sizeof(ModInstanceData))) return 0;
	if (!Erectus::Valid(ModInstanceData.ModListPtr) || !ModInstanceData.ModListSize) return 0;

	DWORD64 ModArraySize = ModInstanceData.ModListSize / 0x8;
	if (!ModArraySize || ModArraySize > 0x7FFF) return 0;

	DWORD* ModArray = new DWORD[ModArraySize * 2];
	if (!Erectus::RPM(ModInstanceData.ModListPtr, &*ModArray, ModArraySize * 2 * sizeof(DWORD)))
	{
		delete[]ModArray;
		return 0;
	}

	BYTE LegendaryRank = 0;

	for (DWORD64 i = 0; i < ModArraySize; i++)
	{
		if (IsLegendaryFormid(ModArray[i * 2]))
		{
			LegendaryRank++;
		}
	}

	delete[]ModArray;
	return LegendaryRank;
}

bool ErectusMemory::ValidLegendary(BYTE LegendaryRank, DWORD64 EntityFlag, EntityLooterSettings* CustomEntityLooterSettings, bool LegendaryWeaponsEnabled, bool LegendaryArmorEnabled)
{
	if (EntityFlag & CUSTOM_ENTRY_WEAPON)
	{
		if (LegendaryWeaponsEnabled)
		{
			switch (LegendaryRank)
			{
			case 0x01:
				return CustomEntityLooterSettings->EntityLooterOneStarWeaponsEnabled;
			case 0x02:
				return CustomEntityLooterSettings->EntityLooterTwoStarWeaponsEnabled;
			case 0x03:
				return CustomEntityLooterSettings->EntityLooterThreeStarWeaponsEnabled;
			default:
				return CustomEntityLooterSettings->EntityLooterAllWeaponsEnabled;
			}
		}
	}
	else if (EntityFlag & CUSTOM_ENTRY_ARMOR)
	{
		if (LegendaryArmorEnabled)
		{
			switch (LegendaryRank)
			{
			case 0x01:
				return CustomEntityLooterSettings->EntityLooterOneStarArmorEnabled;
			case 0x02:
				return CustomEntityLooterSettings->EntityLooterTwoStarArmorEnabled;
			case 0x03:
				return CustomEntityLooterSettings->EntityLooterThreeStarArmorEnabled;
			default:
				return CustomEntityLooterSettings->EntityLooterAllArmorEnabled;
			}
		}
	}

	return false;
}

bool ErectusMemory::TransferEntityItems(Entity EntityData, Reference ReferenceData, Entity LocalPlayer, bool OnlyUseEntityLooterList, bool UseEntityLooterBlacklist)
{
	EntityLooterSettings* CurrentEntityLooterSettings = nullptr;
	switch (ReferenceData.vtable - ErectusProcess::Exe)
	{
	case VTABLE_TESNPC:
		CurrentEntityLooterSettings = &ErectusIni::NPCLooterSettings;
		break;
	case VTABLE_TESOBJECTCONT:
		CurrentEntityLooterSettings = &ErectusIni::ContainerLooterSettings;
		break;
	default:
		return false;
	}

	if (!Erectus::Valid(EntityData.InventoryPtr))
	{
		return false;
	}

	Inventory InventoryData;
	if (!Erectus::RPM(EntityData.InventoryPtr, &InventoryData, sizeof(InventoryData))) return false;
	if (!Erectus::Valid(InventoryData.ItemArrayPtr) || InventoryData.ItemArrayEnd < InventoryData.ItemArrayPtr) return false;

	DWORD64 ItemArraySize = (InventoryData.ItemArrayEnd - InventoryData.ItemArrayPtr) / sizeof(Item);
	if (!ItemArraySize || ItemArraySize > 0x7FFF) return false;

	Item* ItemData = new Item[ItemArraySize];
	if (!Erectus::RPM(InventoryData.ItemArrayPtr, &*ItemData, ItemArraySize * sizeof(Item)))
	{
		delete[]ItemData;
		ItemData = nullptr;
		return false;
	}

	bool LegendaryWeaponsEnabled = AllowLegendaryWeapons(CurrentEntityLooterSettings);
	bool LegendaryArmorEnabled = AllowLegendaryArmor(CurrentEntityLooterSettings);

	for (DWORD64 i = 0; i < ItemArraySize; i++)
	{
		if (!Erectus::Valid(ItemData[i].ReferencePtr)) continue;
		if (!Erectus::Valid(ItemData[i].DisplayPtr) || ItemData[i].Iterations < ItemData[i].DisplayPtr) continue;

		Reference ItemReferenceData;
		if (!Erectus::RPM(ItemData[i].ReferencePtr, &ItemReferenceData, sizeof(ItemReferenceData))) continue;
		if ((ItemReferenceData.RecordFlagA >> 2) & 1) continue;

		if (UseEntityLooterBlacklist)
		{
			if (CheckFormidArray(ItemReferenceData.Formid, CurrentEntityLooterSettings->EntityLooterBlacklistEnabled, CurrentEntityLooterSettings->EntityLooterBlacklist, 64))
			{
				continue;
			}
		}

		if (OnlyUseEntityLooterList)
		{
			if (!CheckFormidArray(ItemReferenceData.Formid, CurrentEntityLooterSettings->EntityLooterEnabledList, CurrentEntityLooterSettings->EntityLooterFormidList, 100))
			{
				continue;
			}
		}

		DWORD64 EntityFlag = CUSTOM_ENTRY_DEFAULT;
		DWORD64 EntityNamePtr = 0;
		int EnabledDistance = 0;

		GetCustomEntityData(ItemReferenceData, &EntityFlag, &EntityNamePtr, &EnabledDistance, false, false);
		if (!(EntityFlag & CUSTOM_ENTRY_VALID_ITEM)) continue;

		if (!OnlyUseEntityLooterList)
		{
			if (!CheckEntityLooterItem(ItemReferenceData.Formid, EntityFlag, CurrentEntityLooterSettings, LegendaryWeaponsEnabled, LegendaryArmorEnabled))
			{
				continue;
			}
		}

		DWORD64 Iterations = (ItemData[i].Iterations - ItemData[i].DisplayPtr) / sizeof(ItemCount);
		if (!Iterations || Iterations > 0x7FFF) continue;

		ItemCount* ItemCountData = new ItemCount[Iterations];
		if (!Erectus::RPM(ItemData[i].DisplayPtr, &*ItemCountData, Iterations * sizeof(ItemCount)))
		{
			delete[]ItemCountData;
			ItemCountData = nullptr;
			continue;
		}

		int Count = 0;
		for (DWORD64 c = 0; c < Iterations; c++)
		{
			Count += ItemCountData[c].Count;
		}

		delete[]ItemCountData;
		ItemCountData = nullptr;

		if (Count == 0)
		{
			continue;
		}

		if (EntityFlag & CUSTOM_ENTRY_WEAPON)
		{
			if (LegendaryWeaponsEnabled)
			{
				BYTE LegendaryRank = GetLegendaryRank(ItemData[i].DisplayPtr);
				if (!ValidLegendary(LegendaryRank, EntityFlag, CurrentEntityLooterSettings, LegendaryWeaponsEnabled, LegendaryArmorEnabled))
				{
					continue;
				}
			}
		}
		else if (EntityFlag & CUSTOM_ENTRY_ARMOR)
		{
			if (LegendaryArmorEnabled)
			{
				BYTE LegendaryRank = GetLegendaryRank(ItemData[i].DisplayPtr);
				if (!ValidLegendary(LegendaryRank, EntityFlag, CurrentEntityLooterSettings, LegendaryWeaponsEnabled, LegendaryArmorEnabled))
				{
					continue;
				}
			}
		}

		TransferMessage TransferMessageData;
		TransferMessageData.vtable = ErectusProcess::Exe + VTABLE_REQUESTTRANSFERITEMMSG;
		TransferMessageData.SrcFormid = EntityData.Formid;
		TransferMessageData.UnknownId = 0xE0001F7A;
		TransferMessageData.DstFormid = LocalPlayer.Formid;
		TransferMessageData.ItemId = ItemData[i].ItemId;
		TransferMessageData.Count = Count;
		TransferMessageData.UnknownA = 0x00000000;
		TransferMessageData.UnknownB = 0x00;
		TransferMessageData.UnknownC = 0x01;
		TransferMessageData.UnknownD = 0x00;
		TransferMessageData.UnknownE = 0x02;
		SendMessageToServer(&TransferMessageData, sizeof(TransferMessageData));
	}

	delete[]ItemData;
	ItemData = nullptr;
	return true;
}

bool ErectusMemory::ContainerValid(Reference ReferenceData)
{
	if (!Erectus::Valid(ReferenceData.KeywordArrayData00C0))
	{
		return false;
	}

	int nifTextLength;
	if (!Erectus::RPM(ReferenceData.KeywordArrayData00C0 + 0x10, &nifTextLength, sizeof(nifTextLength))) return false;
	if (nifTextLength == 41)
	{
		char ContainerMarkerCheck[sizeof("ContainerMarker")];
		if (!Erectus::RPM(ReferenceData.KeywordArrayData00C0 + 0x2E, &ContainerMarkerCheck, sizeof(ContainerMarkerCheck))) return false;

		ContainerMarkerCheck[15] = '\0';
		if (!strcmp(ContainerMarkerCheck, "ContainerMarker"))
		{
			return false;
		}
	}

	if (!Erectus::Valid(ReferenceData.NamePtr00B0))
	{
		return false;
	}

	DWORD64 NameBuffer;
	if (!Erectus::RPM(ReferenceData.NamePtr00B0 + 0x10, &NameBuffer, sizeof(NameBuffer))) return false;
	if (!NameBuffer) return false;

	if (!Erectus::Valid(NameBuffer))
	{
		NameBuffer = ReferenceData.NamePtr00B0;
	}

	int NameTextLength;
	if (!Erectus::RPM(NameBuffer + 0x10, &NameTextLength, sizeof(NameTextLength))) return false;
	if (!NameTextLength || NameTextLength > 0x7FFF) return false;

	return true;
}

bool ErectusMemory::LootEntity(Entity EntityData, Reference ReferenceData, Entity LocalPlayer, bool OnlyUseEntityLooterList, bool UseEntityLooterBlacklist)
{
	bool IsEntityNPC = false;
	bool IsEntityContainer = false;

	int MaxDistance = 0;
	switch (ReferenceData.vtable - ErectusProcess::Exe)
	{
	case VTABLE_TESNPC:
		IsEntityNPC = true;
		MaxDistance = 76;
		break;
	case VTABLE_TESOBJECTCONT:
		IsEntityContainer = true;
		MaxDistance = 6;
		break;
	default:
		return false;
	}

	if (IsEntityNPC)
	{
		if (ReferenceData.Formid == 0x00000007)
		{
			return false;
		}

		if (CheckHealthFlag(EntityData.HealthFlag) != 0x3)
		{
			return false;
		}
	}

	float Distance = Erectus::GetDistance(EntityData.Position, LocalPlayer.Position);
	int NormalDistance = int(Distance * 0.01f);

	if (NormalDistance > MaxDistance)
	{
		return false;
	}

	if (IsEntityContainer)
	{
		if (!ContainerValid(ReferenceData))
		{
			return false;
		}
	}

	if (!EntityInventoryValid(EntityData))
	{
		return false;
	}

	return TransferEntityItems(EntityData, ReferenceData, LocalPlayer, OnlyUseEntityLooterList, UseEntityLooterBlacklist);
}

bool ErectusMemory::CheckEntityLooterSettings(EntityLooterSettings* CustomEntityLooterSettings)
{
	if (CustomEntityLooterSettings->EntityLooterAllWeaponsEnabled)
	{
		return true;
	}
	else if (CustomEntityLooterSettings->EntityLooterAllArmorEnabled)
	{
		return true;
	}
	else if (CustomEntityLooterSettings->EntityLooterOneStarWeaponsEnabled)
	{
		return true;
	}
	else if (CustomEntityLooterSettings->EntityLooterOneStarArmorEnabled)
	{
		return true;
	}
	else if (CustomEntityLooterSettings->EntityLooterTwoStarWeaponsEnabled)
	{
		return true;
	}
	else if (CustomEntityLooterSettings->EntityLooterTwoStarArmorEnabled)
	{
		return true;
	}
	else if (CustomEntityLooterSettings->EntityLooterThreeStarWeaponsEnabled)
	{
		return true;
	}
	else if (CustomEntityLooterSettings->EntityLooterThreeStarArmorEnabled)
	{
		return true;
	}
	else if (CustomEntityLooterSettings->EntityLooterAmmoEnabled)
	{
		return true;
	}
	else if (CustomEntityLooterSettings->EntityLooterModsEnabled)
	{
		return true;
	}
	else if (CustomEntityLooterSettings->EntityLooterCapsEnabled)
	{
		return true;
	}
	else if (CustomEntityLooterSettings->EntityLooterJunkEnabled)
	{
		return true;
	}
	else if (CustomEntityLooterSettings->EntityLooterAidEnabled)
	{
		return true;
	}
	else if (CustomEntityLooterSettings->EntityLooterTreasureMapsEnabled)
	{
		return true;
	}
	else if (CustomEntityLooterSettings->EntityLooterKnownPlansEnabled)
	{
		return true;
	}
	else if (CustomEntityLooterSettings->EntityLooterUnknownPlansEnabled)
	{
		return true;
	}
	else if (CustomEntityLooterSettings->EntityLooterMiscEnabled)
	{
		return true;
	}
	else if (CustomEntityLooterSettings->EntityLooterUnlistedEnabled)
	{
		return true;
	}
	else if (CustomEntityLooterSettings->EntityLooterListEnabled)
	{
		return CheckEntityLooterList(CustomEntityLooterSettings);
	}
	else
	{
		return false;
	}
}

bool ErectusMemory::CheckOnlyUseEntityLooterList(EntityLooterSettings* CustomEntityLooterSettings)
{
	if (CustomEntityLooterSettings->EntityLooterAllWeaponsEnabled)
	{
		return false;
	}
	else if (CustomEntityLooterSettings->EntityLooterAllArmorEnabled)
	{
		return false;
	}
	else if (CustomEntityLooterSettings->EntityLooterOneStarWeaponsEnabled)
	{
		return false;
	}
	else if (CustomEntityLooterSettings->EntityLooterOneStarArmorEnabled)
	{
		return false;
	}
	else if (CustomEntityLooterSettings->EntityLooterTwoStarWeaponsEnabled)
	{
		return false;
	}
	else if (CustomEntityLooterSettings->EntityLooterTwoStarArmorEnabled)
	{
		return false;
	}
	else if (CustomEntityLooterSettings->EntityLooterThreeStarWeaponsEnabled)
	{
		return false;
	}
	else if (CustomEntityLooterSettings->EntityLooterThreeStarArmorEnabled)
	{
		return false;
	}
	else if (CustomEntityLooterSettings->EntityLooterAmmoEnabled)
	{
		return false;
	}
	else if (CustomEntityLooterSettings->EntityLooterModsEnabled)
	{
		return false;
	}
	else if (CustomEntityLooterSettings->EntityLooterCapsEnabled)
	{
		return false;
	}
	else if (CustomEntityLooterSettings->EntityLooterJunkEnabled)
	{
		return false;
	}
	else if (CustomEntityLooterSettings->EntityLooterAidEnabled)
	{
		return false;
	}
	else if (CustomEntityLooterSettings->EntityLooterTreasureMapsEnabled)
	{
		return false;
	}
	else if (CustomEntityLooterSettings->EntityLooterKnownPlansEnabled)
	{
		return false;
	}
	else if (CustomEntityLooterSettings->EntityLooterUnknownPlansEnabled)
	{
		return false;
	}
	else if (CustomEntityLooterSettings->EntityLooterMiscEnabled)
	{
		return false;
	}
	else if (CustomEntityLooterSettings->EntityLooterUnlistedEnabled)
	{
		return false;
	}
	else if (CustomEntityLooterSettings->EntityLooterListEnabled)
	{
		return CheckEntityLooterList(CustomEntityLooterSettings);
	}
	else
	{
		return false;
	}
}

bool ErectusMemory::HarvestFlora(Entity EntityData, Reference ReferenceData, Entity LocalPlayer)
{
	if (FloraHarvested(EntityData.HarvestFlagA, EntityData.HarvestFlagB))
	{
		return false;
	}

	float Distance = Erectus::GetDistance(EntityData.Position, LocalPlayer.Position);
	int NormalDistance = int(Distance * 0.01f);

	if (NormalDistance > 6)
	{
		return false;
	}

	if (!FloraValid(ReferenceData))
	{
		return false;
	}

	RequestActivateRefMessage RequestActivateRefMessageData;
	RequestActivateRefMessageData.vtable = ErectusProcess::Exe + VTABLE_REQUESTACTIVATEREFMSG;
	RequestActivateRefMessageData.Formid = EntityData.Formid;
	RequestActivateRefMessageData.Choice = 0xFF;
	RequestActivateRefMessageData.ForceActivate = 0;

	return SendMessageToServer(&RequestActivateRefMessageData, sizeof(RequestActivateRefMessageData));
}

bool ErectusMemory::Harvester()
{
	if (!MessagePatcher(AllowMessages))
	{
		return false;
	}

	if (!AllowMessages)
	{
		return false;
	}

	bool UseNPCLooter = false;
	if (ErectusIni::NPCLooterSettings.EntityLooterEnabled)
	{
		UseNPCLooter = CheckEntityLooterSettings(&ErectusIni::NPCLooterSettings);
	}

	bool UseContainerLooter = false;
	if (ErectusIni::ContainerLooterSettings.EntityLooterEnabled)
	{
		UseContainerLooter = CheckEntityLooterSettings(&ErectusIni::ContainerLooterSettings);
	}

	bool UseFloraHarvester = false;
	if (ErectusIni::CustomHarvesterSettings.HarvesterEnabled)
	{
		UseFloraHarvester = CheckIngredientList();
	}

	if (!UseNPCLooter && !UseContainerLooter && !UseFloraHarvester)
	{
		return false;
	}

	DWORD64 LocalPlayerPtr = GetLocalPlayerPtr(true);
	if (!Erectus::Valid(LocalPlayerPtr)) return false;

	Entity LocalPlayer;
	if (!Erectus::RPM(LocalPlayerPtr, &LocalPlayer, sizeof(LocalPlayer))) return false;

	bool OnlyUseNPCLooterList = false;
	bool UseNPCLooterBlacklist = false;
	if (UseNPCLooter)
	{
		OnlyUseNPCLooterList = CheckOnlyUseEntityLooterList(&ErectusIni::NPCLooterSettings);
		UseNPCLooterBlacklist = CheckEntityLooterBlacklist(&ErectusIni::NPCLooterSettings);
	}

	bool OnlyUseContainerLooterList = false;
	bool UseContainerLooterBlacklist = false;
	if (UseContainerLooter)
	{
		OnlyUseContainerLooterList = CheckOnlyUseEntityLooterList(&ErectusIni::ContainerLooterSettings);
		UseContainerLooterBlacklist = CheckEntityLooterBlacklist(&ErectusIni::ContainerLooterSettings);
	}

	if (UseNPCLooter)
	{
		int TemporaryNPCSize = 0;
		DWORD64* TemporaryNPCList = GetNPCList(&TemporaryNPCSize);
		if (TemporaryNPCList == nullptr) return false;

		for (int i = 0; i < TemporaryNPCSize; i++)
		{
			if (!Erectus::Valid(TemporaryNPCList[i])) continue;
			if (TemporaryNPCList[i] == LocalPlayerPtr) continue;

			Entity EntityData;
			if (!Erectus::RPM(TemporaryNPCList[i], &EntityData, sizeof(EntityData))) continue;
			if (!Erectus::Valid(EntityData.ReferencePtr)) continue;

			if (EntityData.SpawnFlag != 0x02)
			{
				continue;
			}

			Reference ReferenceData;
			if (!Erectus::RPM(EntityData.ReferencePtr, &ReferenceData, sizeof(ReferenceData))) continue;
			if (ReferenceData.vtable != ErectusProcess::Exe + VTABLE_TESNPC) continue;
			if (ReferenceData.Formid == 0x00000007) continue;

			LootEntity(EntityData, ReferenceData, LocalPlayer, OnlyUseNPCLooterList, UseNPCLooterBlacklist);
		}

		delete[]TemporaryNPCList;
		TemporaryNPCList = nullptr;
	}

	if (UseContainerLooter || UseFloraHarvester)
	{
		int TemporaryEntitySize = 0;
		DWORD64* TemporaryEntityList = GetEntityList(&TemporaryEntitySize);
		if (TemporaryEntityList == nullptr) return false;

		for (int i = 0; i < TemporaryEntitySize; i++)
		{
			if (!Erectus::Valid(TemporaryEntityList[i])) continue;
			if (TemporaryEntityList[i] == LocalPlayerPtr) continue;

			Entity EntityData;
			if (!Erectus::RPM(TemporaryEntityList[i], &EntityData, sizeof(EntityData))) continue;
			if (!Erectus::Valid(EntityData.ReferencePtr)) continue;

			if (EntityData.SpawnFlag != 0x02)
			{
				continue;
			}

			Reference ReferenceData;
			if (!Erectus::RPM(EntityData.ReferencePtr, &ReferenceData, sizeof(ReferenceData))) continue;

			if (ReferenceData.vtable == ErectusProcess::Exe + VTABLE_TESOBJECTCONT)
			{
				if (UseContainerLooter)
				{
					LootEntity(EntityData, ReferenceData, LocalPlayer, OnlyUseContainerLooterList, UseContainerLooterBlacklist);
				}
			}
			else if (ReferenceData.vtable == ErectusProcess::Exe + VTABLE_TESFLORA)
			{
				if (UseFloraHarvester)
				{
					HarvestFlora(EntityData, ReferenceData, LocalPlayer);
				}
			}
		}

		delete[]TemporaryEntityList;
		TemporaryEntityList = nullptr;
	}

	return true;
}

bool ErectusMemory::MeleeAttack()
{
	if (!MessagePatcher(AllowMessages))
	{
		return false;
	}

	if (!AllowMessages)
	{
		return false;
	}

	DWORD64 LocalPlayerPtr = GetLocalPlayerPtr(true);
	if (!Erectus::Valid(LocalPlayerPtr)) return false;

	DWORD64 AllocAddress = Erectus::AllocEx(sizeof(ExternalFunction));
	if (AllocAddress == 0) return false;

	ExternalFunction ExternalFunctionData;
	ExternalFunctionData.Address = ErectusProcess::Exe + OFFSET_MELEE_ATTACK;
	ExternalFunctionData.RCX = LocalPlayerPtr;
	ExternalFunctionData.RDX = 0;
	ExternalFunctionData.R8 = 1;
	ExternalFunctionData.R9 = 0;

	bool Written = Erectus::WPM(AllocAddress, &ExternalFunctionData, sizeof(ExternalFunction));

	if (!Written)
	{
		Erectus::FreeEx(AllocAddress);
		return false;
	}

	DWORD64 ParamAddress = AllocAddress + sizeof(ExternalFunction::ASM);
	HANDLE Thread = CreateRemoteThread(ErectusProcess::Handle, NULL, 0, LPTHREAD_START_ROUTINE(AllocAddress), LPVOID(ParamAddress), 0, 0);

	if (Thread == NULL)
	{
		Erectus::FreeEx(AllocAddress);
		return false;
	}

	DWORD ThreadResult = WaitForSingleObject(Thread, 3000);
	CloseHandle(Thread);

	if (ThreadResult == WAIT_TIMEOUT)
	{
		return false;
	}

	Erectus::FreeEx(AllocAddress);
	return true;
}

bool ErectusMemory::ChargenEditing()
{
	DWORD64 ChargenPtr;
	if (!Erectus::RPM(ErectusProcess::Exe + OFFSET_CHARGEN, &ChargenPtr, sizeof(ChargenPtr))) return false;
	if (!Erectus::Valid(ChargenPtr)) return false;

	Chargen ChargenData;
	if (!Erectus::RPM(ChargenPtr, &ChargenData, sizeof(ChargenData))) return false;

	bool EditChargenData = false;

	if (ChargenData.Thin != ErectusIni::CustomChargenSettings.Thin)
	{
		ChargenData.Thin = ErectusIni::CustomChargenSettings.Thin;
		EditChargenData = true;
	}

	if (ChargenData.Muscular != ErectusIni::CustomChargenSettings.Muscular)
	{
		ChargenData.Muscular = ErectusIni::CustomChargenSettings.Muscular;
		EditChargenData = true;
	}

	if (ChargenData.Large != ErectusIni::CustomChargenSettings.Large)
	{
		ChargenData.Large = ErectusIni::CustomChargenSettings.Large;
		EditChargenData = true;
	}

	if (EditChargenData)
	{
		return Erectus::WPM(ChargenPtr, &ChargenData, sizeof(ChargenData));
	}

	return true;
}

bool ErectusMemory::CreateProjectile(DWORD ItemId, float* Position, float* Rotation)
{
	if (!MessagePatcher(AllowMessages))
	{
		return false;
	}

	if (!AllowMessages)
	{
		return false;
	}

	DWORD64 AllocAddress = Erectus::AllocEx(sizeof(ExecutionProjectile));
	if (AllocAddress == 0) return false;

	CreateProjectileMessageClient CreateProjectileMessageClientData;
	CreateProjectileMessageClientData.vtable = ErectusProcess::Exe + VTABLE_CREATEPROJECTILEMESSAGECLIENT;
	CreateProjectileMessageClientData.PositionX = Position[0];
	CreateProjectileMessageClientData.PositionY = Position[1];
	CreateProjectileMessageClientData.PositionZ = Position[2];
	CreateProjectileMessageClientData.RotationArrayPtr = AllocAddress + 0xD0;
	CreateProjectileMessageClientData.RotationArrayEnd = AllocAddress + 0xD0 + sizeof(float[3]);
	CreateProjectileMessageClientData.RotationArrayPad = AllocAddress + 0xD0 + sizeof(float[3]);
	CreateProjectileMessageClientData.ItemId = ItemId;
	CreateProjectileMessageClientData.UnknownA = 0xFFFFFFFF;
	CreateProjectileMessageClientData.UnknownB = 0xFFFFFFFF;
	CreateProjectileMessageClientData.UnknownC = 0x00000000;
	CreateProjectileMessageClientData.UnknownD = 1.0f;
	CreateProjectileMessageClientData.UnknownE = 0x00000000;
	CreateProjectileMessageClientData.UnknownArrayPtrA = AllocAddress + 0xE0;
	CreateProjectileMessageClientData.UnknownArrayEndA = AllocAddress + 0xE0 + sizeof(WORD[1]);
	CreateProjectileMessageClientData.UnknownArrayPadA = AllocAddress + 0xE0 + sizeof(WORD[1]);
	CreateProjectileMessageClientData.UnknownF = 0xFF;
	CreateProjectileMessageClientData.UnknownArrayPtrB = AllocAddress + 0xF0;
	CreateProjectileMessageClientData.UnknownArrayEndB = AllocAddress + 0xF0 + sizeof(BYTE[1]);
	CreateProjectileMessageClientData.UnknownArrayPadB = AllocAddress + 0xF0 + sizeof(BYTE[1]);
	CreateProjectileMessageClientData.UnknownG = 0x00;

	ExecutionProjectile ExecutionProjectileData;
	ExecutionProjectileData.Address = ErectusProcess::Exe + OFFSET_MESSAGE_SENDER;
	ExecutionProjectileData.RCX = AllocAddress + 0x40;
	ExecutionProjectileData.RDX = 0;
	memcpy(ExecutionProjectileData.Message, &CreateProjectileMessageClientData, sizeof(CreateProjectileMessageClientData));
	ExecutionProjectileData.RotationX = Rotation[0];
	ExecutionProjectileData.RotationY = Rotation[1];
	ExecutionProjectileData.RotationZ = Rotation[2];
	ExecutionProjectileData.UnknownArrayValueA = WORD(Erectus::GetRangedInt(999, 9999));
	ExecutionProjectileData.UnknownArrayValueB = 0x01;

	if (!Erectus::WPM(AllocAddress, &ExecutionProjectileData, sizeof(ExecutionProjectileData)))
	{
		Erectus::FreeEx(AllocAddress);
		return false;
	}

	DWORD64 ParamAddress = AllocAddress + sizeof(ExecutionProjectile::ASM);
	HANDLE Thread = CreateRemoteThread(ErectusProcess::Handle, NULL, 0, LPTHREAD_START_ROUTINE(AllocAddress), LPVOID(ParamAddress), 0, 0);

	if (Thread == NULL)
	{
		Erectus::FreeEx(AllocAddress);
		return false;
	}

	DWORD ThreadResult = WaitForSingleObject(Thread, 3000);
	CloseHandle(Thread);

	if (ThreadResult == WAIT_TIMEOUT)
	{
		return false;
	}

	Erectus::FreeEx(AllocAddress);
	return true;
}

bool ErectusMemory::CreateForwardProjectile(DWORD ItemId)
{
	if (ItemId == 0)
	{
		return false;
	}

	DWORD64 CameraPtr = GetCameraPtr();
	if (!Erectus::Valid(CameraPtr)) return false;

	Camera CameraData;
	if (!Erectus::RPM(CameraPtr, &CameraData, sizeof(CameraData))) return false;

	float Rotation[3];
	Rotation[0] = -atan2f(CameraData.Forward[2], sqrtf(powf(CameraData.Forward[0], 2.0f) + powf(CameraData.Forward[1], 2.0f)));
	Rotation[1] = 0.0f;
	Rotation[2] = -atan2f(-CameraData.Forward[0], CameraData.Forward[1]);

	return CreateProjectile(ItemId, CameraData.Origin, Rotation);
}
