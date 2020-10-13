#pragma once
#include <cstdint>
#include <vector>

#include "TesObjectRefr.h"

class TesObjectCell
{
public:
	std::uintptr_t vtable;
	char padding0008[0x18];
	std::uint32_t formId;
	char padding0024[0x40];
	std::uint64_t isInterior;
	char loadedState;
	char padding0071[0x2F];
	std::uintptr_t objectListBeginPtr;//0xA0
	std::uintptr_t objectListEndPtr;//0xA8

	[[nodiscard]] std::vector<TesObjectRefr> GetObjectRefs() const;
};

