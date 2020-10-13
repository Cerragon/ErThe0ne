#pragma once
#include <cstdint>

#include "../../common.h"

class TesItem
{
public:
	[[nodiscard]] FormType    GetFormType() const { return static_cast<FormType>(formType); }
	[[nodiscard]] std::string GetName() const;

	[[nodiscard]] bool IsPlan() const
	{
		return GetFormType() == FormType::TesObjectBook && planFlag >> 5 & 1;
	}
	[[nodiscard]] bool IsTreasureMap() const
	{
		return GetFormType() == FormType::TesObjectBook && TREASUREMAP_FORMIDS.contains(formId);
	}
	[[nodiscard]] bool IsMagazine() const;
	[[nodiscard]] bool IsJunkItem() const
	{
		return GetFormType() == FormType::TesObjectMisc && componentArraySize && !(recordFlagA >> 7 & 1);
	}
	[[nodiscard]] bool IsMod() const
	{
		return GetFormType() == FormType::TesObjectMisc && recordFlagA >> 7 & 1;
	}
	[[nodiscard]] bool IsBobblehead() const;

	std::uintptr_t vtable;//0x0
	char padding0008[0x10];
	char recordFlagA;//0x18
	char padding0019[0x7];
	std::uint32_t formId;//0x20
	char padding0024[0x2];//0x24
	std::uint8_t formType;//0x26
	char padding0027[0x71];//0x27
	std::uintptr_t namePtr0098;//0x98
	char padding00A0[0x10];
	std::uintptr_t namePtr00B0;//0xB0
	std::uintptr_t keywordArrayData00B8;//0xB8
	std::uintptr_t keywordArrayData00C0;//0xC0
	char padding00B8[0x19];
	char omodFlag;//0xE1
	char padding00E2[0xE];
	std::uintptr_t factionArrayPtr;//0xF0
	char padding00F8[0x8];
	int factionArraySize;//0x100
	char padding0104[0x74];
	std::uintptr_t namePtr0178;//0x178
	char padding0168[0x30];
	std::uintptr_t keywordArrayData01B0;//0x1B0
	std::uintptr_t keywordArrayData01B8;//0x1B8
	std::uintptr_t keywordArrayData01C0;//0x1C0
	char padding01C4[0x10];
	char planFlag;//0x1D8
	char padding01D9[0xF];
	std::uintptr_t componentArrayPtr;//0x1E8
	char padding01F0[0x8];
	int componentArraySize;//0x1F8
	char padding01Fc[0x14];
	std::uintptr_t harvestedPtr;//0x210
};
