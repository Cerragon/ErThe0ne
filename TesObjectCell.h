#pragma once
#include <cstdint>
#include <vector>

#include "TesObjectRefr.h"


class TesObjectCell
{
public:
	char padding0000[0x68];
	std::uintptr_t isInterior;
	char loadedState;
	char padding0071[0x2F];
	std::uintptr_t objectListBeginPtr;//0xA0
	std::uintptr_t objectListEndPtr;//0xA8

	[[nodiscard]] std::vector<TesObjectRefr> GetObjectRefs() const;
};

