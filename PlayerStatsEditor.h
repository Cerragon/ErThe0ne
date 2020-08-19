#pragma once
#include <Windows.h>

class PlayerStatsEditor
{
public:
	static bool Edit(bool enabled);

private:
	static bool SetActorValueMaximum(DWORD formId, float defaultValue, float customValue, bool enabled);

	inline static DWORD64 actorValuePage = 0;
	inline static bool actorValuePageValid = false;
};
