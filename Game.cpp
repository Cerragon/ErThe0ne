#include "Game.h"

#include "common.h"
#include "ErectusProcess.h"
#include "utils.h"

LoadedAreaManager Game::GetLoadedAreaManager()
{
	LoadedAreaManager result = {};

	std::uintptr_t loadedAreaManagerPtr;
	if (!ErectusProcess::Rpm(ErectusProcess::exe + OFFSET_ENTITY_LIST, &loadedAreaManagerPtr, sizeof loadedAreaManagerPtr))
		return result;
	if (!Utils::Valid(loadedAreaManagerPtr))
		return result;

	ErectusProcess::Rpm(loadedAreaManagerPtr, &result, sizeof result);

	return result;
}
