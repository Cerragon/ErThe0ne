#pragma once
#include "LoadedAreaManager.h"
#include "LocalPlayer.h"

class Game final
{
public:
	static LoadedAreaManager GetLoadedAreaManager();
	static LocalPlayer GetLocalPlayer();

private:
	virtual void Dummy() = 0;
};

