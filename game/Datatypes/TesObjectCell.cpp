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

	const auto objectCount = (objectListEndPtr - objectListBeginPtr) / sizeof(std::uintptr_t);
	auto objectPtrs = std::vector<std::uintptr_t>(objectCount);
	if (!ErectusProcess::Rpm(objectListBeginPtr, objectPtrs.data(), objectPtrs.size() * sizeof std::uintptr_t))
		return result;

	result.reserve(objectPtrs.size());
	for(const auto objectPtr : objectPtrs)
	{
		if (!Utils::Valid(objectPtr))
			continue;

		TesObjectRefr entityData = {};
		if (!ErectusProcess::Rpm(objectPtr, &entityData, sizeof entityData))
			continue;
		if (!Utils::Valid(entityData.baseObjectPtr))
			continue;
		entityData.ptr = objectPtr;
		
		result.push_back(entityData);
	}
	return result;
}
