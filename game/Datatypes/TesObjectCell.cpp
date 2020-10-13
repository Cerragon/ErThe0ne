#include "TesObjectCell.h"

#include <memory>


#include "../../ErectusProcess.h"
#include "../../utils.h"

std::vector<TesObjectRefr> TesObjectCell::GetObjectRefs() const
{
	std::vector<TesObjectRefr> result = {};

	if (loadedState != 7) // attached == 7
		return result;

	if (!Utils::Valid(objectListBeginPtr) || !Utils::Valid(objectListEndPtr))
		return result;

	const auto itemArraySize = (objectListEndPtr - objectListBeginPtr) / sizeof(DWORD64);
	const auto objectPtrArray = std::make_unique<DWORD64[]>(itemArraySize);
	if (!ErectusProcess::Rpm(objectListBeginPtr, objectPtrArray.get(), itemArraySize * sizeof DWORD64))
		return result;

	result.reserve(itemArraySize);
	for(size_t i = 0; i < itemArraySize; i++)
	{
		if (!Utils::Valid(objectPtrArray[i]))
			continue;

		TesObjectRefr entityData = {};
		if (!ErectusProcess::Rpm(objectPtrArray[i], &entityData, sizeof entityData))
			continue;
		if (!Utils::Valid(entityData.baseObjectPtr))
			continue;

		entityData.ptr = objectPtrArray[i];
		result.push_back(entityData);
	}
	return result;
}
