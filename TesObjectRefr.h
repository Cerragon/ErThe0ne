#pragma once
#include <cstdint>

#include "TesItem.h"

class TesObjectRefr
{
public:
	std::uintptr_t vtable;//0x0
	char padding0008[0x8];
	char harvestFlagA;//0x10
	char padding0011[0x8];
	char harvestFlagB;//0x19
	char padding001A[0x6];
	std::uint32_t formId;//0x20
	char padding0024[0x2];//0x24
	std::uint8_t formType;//0x26
	char padding0027[0x11];//0x27
	char idValue[4];//0x38
	char padding003C[0x14];
	std::uintptr_t vtable0050;//0x50
	char padding0058[0x8];
	float pitch;//0x60
	char padding0064[0x4];
	float yaw;//0x68
	char padding006C[0x4];
	float position[3];//0x70
	char padding007C[0x4];
	std::uintptr_t inventoryPtr;//0x80
	char padding0088[0x8];
	std::uintptr_t actorCorePtr;//0x90
	char padding0098[0x10];
	std::uintptr_t cellPtr;//0xA8
	std::uintptr_t skeletonPtr;//0xB0
	std::uintptr_t baseObjectPtr;//0xB8
	char padding00C0[0xE];
	char spawnFlag;//0xCE
	char padding00Cf[0xC9];
	char movementFlag;//0x198
	char sprintFlag;//0x199
	char healthFlag;//0x19A
	char padding019B[0xA89];
	std::uint32_t formId0C24;//0xC24

	std::uintptr_t ptr; //not really there, fill it manually

	[[nodiscard]] TesItem GetBaseObject() const;
};

