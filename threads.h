#pragma once
#include <Windows.h>

class Threads final {
public:
	static bool CreateProcessThreads();
	static bool ThreadDestruction();

	inline static bool threadCreationState = false;
	inline static bool threadDestructionQueued = false;
	inline static int threadDestructionCounter = 0;

	inline static bool positionSpoofingToggle = false;
	inline static bool noclipToggle = false;
	inline static bool opkNpcsToggle = false;

private:
	static DWORD __stdcall BufferEntityListThread(LPVOID lpParameter);
	static DWORD __stdcall BufferPlayerListThread(LPVOID lpParameter);
	static DWORD __stdcall WeaponEditorThread(LPVOID lpParameter);
	static DWORD __stdcall LockingThread(LPVOID lpParameter);
	static DWORD __stdcall MultihackThread(LPVOID lpParameter);
	static DWORD __stdcall Looter(LPVOID lpParameter);

	inline static bool bufferEntityListThreadActive = false;
	inline static bool bufferPlayerListThreadActive = false;
	inline static bool weaponEditorThreadActive = false;
	inline static bool lockingThreadActive = false;
	inline static bool multihackThreadActive = false;
	inline static bool looterThreadActive = false;

	inline static bool threadDestructionState = false;

	virtual void Dummy() = 0;
};
