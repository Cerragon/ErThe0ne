#include "LoadedAreaManager.h"

#include <memory>

#include "../../ErectusProcess.h"
#include "../../utils.h"

std::vector<TesObjectCell> LoadedAreaManager::GetLoadedCells() const
{
	std::vector<TesObjectCell> result = {};

	if (!Utils::Valid(interiorCellArrayBegin) || !Utils::Valid(interiorCellArrayEnd) || !Utils::Valid(exteriorCellArrayBegin) || !Utils::Valid(exteriorCellArrayEnd))
		return result;

	std::uintptr_t cellPtrArrayPtr;
	std::size_t cellPtrArraySize;

	//1) Select  interior or exterior objectlist
	if (interiorCellArrayBegin != interiorCellArrayEnd)
	{
		cellPtrArrayPtr = interiorCellArrayBegin;
		cellPtrArraySize = 2;
	}
	else if (exteriorCellArrayBegin != exteriorCellArrayEnd)
	{
		cellPtrArrayPtr = exteriorCellArrayBegin;
		cellPtrArraySize = 50;
	}
	else return result; // sthg went wrong

	//2) Read the array of pointers to cells
	const auto cellPtrArray = std::make_unique<std::uintptr_t[]>(cellPtrArraySize);
	if (!ErectusProcess::Rpm(cellPtrArrayPtr, cellPtrArray.get(), cellPtrArraySize * sizeof std::uintptr_t))
		return result;

	//3) Read each cell
	//this is actually a linked list presenting as an array, odd entries are just pointers to 'next' element, so we skip them
	for (std::size_t i = 0; i < cellPtrArraySize; i++)
	{
		if (i % 2 != 0)
			continue;

		TesObjectCell cell{};
		if (!ErectusProcess::Rpm(cellPtrArray[i], &cell, sizeof TesObjectCell))
			continue;

		result.emplace_back(cell);
	}

	return result;
}

