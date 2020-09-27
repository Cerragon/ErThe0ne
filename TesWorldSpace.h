#pragma once
#include <cstdint>

class TesWorldSpace
{
public:
	char pad0000[32]; //0x0000
	std::uint32_t formId; //0x0020
	char pad0024[2]; //0x0024
	std::uint8_t formType; //0x0026
	char pad0027[353]; //0x0027
	std::uintptr_t skyCellPtr; //0x0188
	char pad0190[120]; //0x0190
};
