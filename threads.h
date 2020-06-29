#pragma once
#include <Windows.h>

class Threads final {
public:
	static bool CreateProcessThreads();
	static bool ThreadDestruction();

	static void RequestLootItems();
	static void RequestLootScrap();

	inline static bool threadCreationState = false;
	inline static bool threadDestructionQueued = false;
	inline static int threadDestructionCounter = 0;

	inline static bool positionSpoofingToggle = false;
	inline static bool noclipToggle = false;
	inline static bool opkPlayersToggle = false;
	inline static bool opkNpcsToggle = false;

private:
	static DWORD __stdcall BufferEntityListThread(LPVOID lpParameter);
	static DWORD __stdcall BufferNpcListThread(LPVOID lpParameter);
	static DWORD __stdcall BufferPlayerListThread(LPVOID lpParameter);
	static DWORD __stdcall WeaponEditorThread(LPVOID lpParameter);
	static DWORD __stdcall LockingThread(LPVOID lpParameter);
	static DWORD __stdcall MultihackThread(LPVOID lpParameter);
	static DWORD __stdcall HarvesterThread(LPVOID lpParameter);
	static DWORD __stdcall KnownRecipeThread(LPVOID lpParameter);

	inline static bool bufferEntityListThreadActive = false;
	inline static bool bufferNpcListThreadActive = false;
	inline static bool bufferPlayerListThreadActive = false;
	inline static bool weaponEditorThreadActive = false;
	inline static bool lockingThreadActive = false;
	inline static bool multihackThreadActive = false;
	inline static bool harvesterThreadActive = false;
	inline static bool knownRecipeThreadActive = false;

	inline static bool threadDestructionState = false;

	inline static bool lootItemsRequested = false;
	inline static bool lootScrapRequested = false;

	virtual void __dummy();
};
