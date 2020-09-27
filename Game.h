#pragma once
#include "LoadedAreaManager.h"
#include "LocalPlayer.h"

class Game final
{
public:
	static LocalPlayer GetLocalPlayer();
	static std::vector<TesObjectCell> GetLoadedCells();

private:
	static LoadedAreaManager GetLoadedAreaManager();
	static TesObjectCell GetSkyCell();
	
	virtual void Dummy() = 0;
};

