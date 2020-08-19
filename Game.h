#pragma once
#include "LoadedAreaManager.h"

class Game final
{
public:
	static LoadedAreaManager GetLoadedAreaManager();
	
private:
	virtual void Dummy() = 0;
};

