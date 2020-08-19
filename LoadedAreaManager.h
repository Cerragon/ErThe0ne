#pragma once
#include <cstdint>
#include <vector>

#include "TesObjectCell.h"

class LoadedAreaManager
{
public:
	char padding0000[0x80];
	std::uintptr_t interiorCellArrayBegin;//0x80
	std::uintptr_t interiorCellArrayEnd;//0x88
	char padding0090[0x18];
	std::uintptr_t exteriorCellArrayBegin;//0xA8
	std::uintptr_t exteriorCellArrayEnd;//0xB0

	[[nodiscard]] std::vector<TesObjectCell> GetLoadedCells() const;
};


