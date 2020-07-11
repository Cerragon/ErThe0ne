#pragma once
#include <Windows.h>

#include <string>
#include <vector>


class ErectusProcess final {
public:
	static void SetProcessMenu();
	static void ResetProcessData();
	static bool AttachToProcess(DWORD processId);
	static void SetProcessError(int errorId, const char* error);

	static std::vector<DWORD> GetProcesses();

	static bool HwndValid(DWORD processId);

	inline static std::string processError;

	inline static int processValidCounter = 0;
	inline static bool processMenuActive = false;
	inline static bool processSelected = false;

	inline static int processErrorId = 0;

	inline static HANDLE handle = nullptr;
	inline static HWND hWnd = nullptr;
	inline static DWORD64 exe = 0;
	inline static DWORD pid = 0;

	static bool Rpm(DWORD64 src, void* dst, size_t size);
	static bool Wpm(DWORD64 dst, void* src, size_t size);
	static DWORD64 AllocEx(size_t size);
	static bool FreeEx(DWORD64 src);

private:
	static BOOL HwndEnumFunc(HWND hwnd, LPARAM lParam);
	static DWORD64 GetModuleBaseAddress(DWORD pid, const char* module);

	virtual void __dummy() = 0;
};