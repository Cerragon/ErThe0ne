#include "ErectusInclude.h"

void ErectusProcess::SetProcessMenu()
{
	if (ErectusMain::WindowSize[0] != 384 || ErectusMain::WindowSize[1] != 224)
	{
		ErectusMain::WindowSize[0] = 384;
		ErectusMain::WindowSize[1] = 224;

		if (ErectusMain::WindowHwnd != NULL)
		{
			ErectusD3D9::DeviceResetQueued = true;
			SetWindowPos(ErectusMain::WindowHwnd, HWND_NOTOPMOST, ErectusMain::WindowPosition[0], ErectusMain::WindowPosition[1], ErectusMain::WindowSize[0], ErectusMain::WindowSize[1], 0);
		}
	}

	int BufferPosition[2];
	BufferPosition[0] = (GetSystemMetrics(SM_CXSCREEN) / 2) - (ErectusMain::WindowSize[0] / 2);
	BufferPosition[1] = (GetSystemMetrics(SM_CYSCREEN) / 2) - (ErectusMain::WindowSize[1] / 2);

	if (ErectusMain::WindowPosition[0] != BufferPosition[0] || ErectusMain::WindowPosition[1] != BufferPosition[1])
	{
		ErectusMain::WindowPosition[0] = BufferPosition[0];
		ErectusMain::WindowPosition[1] = BufferPosition[1];

		if (ErectusMain::WindowHwnd != NULL)
		{
			MoveWindow(ErectusMain::WindowHwnd, ErectusMain::WindowPosition[0], ErectusMain::WindowPosition[1], ErectusMain::WindowSize[0], ErectusMain::WindowSize[1], FALSE);
			if (!ErectusD3D9::DeviceResetQueued)
			{
				SetWindowPos(ErectusMain::WindowHwnd, HWND_NOTOPMOST, ErectusMain::WindowPosition[0], ErectusMain::WindowPosition[1], ErectusMain::WindowSize[0], ErectusMain::WindowSize[1], 0);
			}
		}
	}

	if (ErectusMain::WindowHwnd != NULL)
	{
		LONG_PTR Style = GetWindowLongPtr(ErectusMain::WindowHwnd, GWL_EXSTYLE);

		if (Style & WS_EX_LAYERED)
		{
			Style &= ~WS_EX_LAYERED;
			SetWindowLongPtr(ErectusMain::WindowHwnd, GWL_EXSTYLE, Style);
		}

		if (Style & WS_EX_TOPMOST)
		{
			SetWindowPos(ErectusMain::WindowHwnd, HWND_NOTOPMOST, ErectusMain::WindowPosition[0], ErectusMain::WindowPosition[1], ErectusMain::WindowSize[0], ErectusMain::WindowSize[1], 0);
		}
	}

	ProcessMenuActive = true;
	ErectusMain::OverlayMenuActive = false;
	ErectusMain::OverlayActive = false;
}

void ErectusProcess::SetProcessError(int ErrorId, const char *Error, size_t Length)
{
	if (ProcessError != nullptr)
	{
		delete[]ProcessError;
		ProcessError = nullptr;
	}

	if (ErrorId != -1)
	{
		ProcessErrorId = ErrorId;
		ProcessError = new char[Length];
		sprintf_s(ProcessError, Length, Error);
	}
}

void ErectusProcess::ResetProcessData(bool ClearProcessError, int NewProcessListSize)
{
	if (ProcessList != nullptr)
	{
		if (ProcessListSize)
		{
			for (int i = 0; i < ProcessListSize; i++)
			{
				if (ProcessList[i] != nullptr)
				{
					delete[]ProcessList[i];
					ProcessList[i] = nullptr;
				}
			}
		}

		delete[]ProcessList;
		ProcessList = nullptr;
	}

	if (ProcessIdList != nullptr)
	{
		delete[]ProcessIdList;
		ProcessIdList = nullptr;
	}

	ProcessSelected = false;
	ProcessIndex = 0;

	if (NewProcessListSize)
	{
		ProcessListSize = NewProcessListSize;
		ProcessList = new char*[ProcessListSize];
		ProcessList[ProcessIndex] = new char[sizeof("No process selected")];
		sprintf_s(ProcessList[ProcessIndex], sizeof("No process selected"), "No process selected");
		ProcessIdList = new DWORD[ProcessListSize];
		ProcessIdList[ProcessIndex] = 0;

		if (!ProcessMenuActive)
		{
			SetProcessMenu();
		}

		if (ClearProcessError)
		{
			SetProcessError(0, "Process State: No process selected", sizeof("Process State: No process selected"));
		}
	}
	else
	{
		ProcessListSize = 0;
		SetProcessError(-1, "", sizeof(""));
	}

	if (ErectusThread::ThreadCreationState)
	{
		bool AreThreadsActive = false;

		while (!ErectusThread::ThreadDestruction())
		{
			ErectusThread::ThreadDestructionCounter++;
			if (ErectusThread::ThreadDestructionCounter > 14400)
			{
				AreThreadsActive = true;

				if (NewProcessListSize)
				{
					ErectusMain::Close();
				}

				break;
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}

		if (!AreThreadsActive)
		{
			ErectusMemory::MessagePatcher(false);
		}
	}

	if (ErectusMemory::CustomEntityListUpdated)
	{
		ErectusMemory::DeleteCustomEntityList();
	}

	if (ErectusMemory::CustomNPCListUpdated)
	{
		ErectusMemory::DeleteCustomNPCList();
	}

	if (ErectusMemory::CustomPlayerListUpdated)
	{
		ErectusMemory::DeleteCustomPlayerList();
	}

	Pid = 0;
	Hwnd = NULL;
	Exe = 0;

	if (Handle != NULL)
	{
		CloseHandle(Handle);
		Handle = NULL;
	}
}

int ErectusProcess::GetProcessCount()
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE) return 1;

	PROCESSENTRY32 lppe;
	lppe.dwSize = sizeof(lppe);

	int ProcessCount = 1;
	while (Process32Next(hSnapshot, &lppe))
	{
		if (!strcmp(lppe.szExeFile, "Fallout76.exe"))
		{
			ProcessCount++;
		}
	}

	CloseHandle(hSnapshot);
	return ProcessCount;
}

bool ErectusProcess::UpdateProcessList()
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE) return false;

	PROCESSENTRY32 lppe;
	lppe.dwSize = sizeof(lppe);

	int ProcessCount = GetProcessCount();
	if (ProcessCount == 1)
	{
		CloseHandle(hSnapshot);
		return false;
	}
	else
	{
		ResetProcessData(true, ProcessCount);
	}

	int CurrentProcess = 0;
	while (Process32Next(hSnapshot, &lppe))
	{
		if (!strcmp(lppe.szExeFile, "Fallout76.exe"))
		{
			CurrentProcess++;
			ProcessList[CurrentProcess] = new char[sizeof("Fallout76.exe - 4294967295")];
			sprintf_s(ProcessList[CurrentProcess], sizeof("Fallout76.exe - 4294967295"), "Fallout76.exe - %lu", lppe.th32ProcessID);
			ProcessIdList[CurrentProcess] = lppe.th32ProcessID;
		}
	}

	CloseHandle(hSnapshot);
	return true;
}

BOOL ErectusProcess::HwndEnumFunc(HWND hwnd, LPARAM lParam)
{
	DWORD lpdwProcessId;
	GetWindowThreadProcessId(hwnd, &lpdwProcessId);

	if (lpdwProcessId == lParam)
	{
		char Buffer[sizeof("Fallout76")] = { '\0' };
		if (GetClassName(hwnd, Buffer, sizeof(Buffer)))
		{
			if (!strcmp(Buffer, "Fallout76"))
			{
				Hwnd = hwnd;
				return FALSE;
			}
		}
	}

	Hwnd = NULL;
	return TRUE;
}

bool ErectusProcess::HwndValid(DWORD ProcessId)
{
	Pid = ProcessId;
	if (!Pid)
	{
		SetProcessError(2, "Process State: PID (Process Id) invalid", sizeof("Process State: PID (Process Id) invalid"));
		return false;
	}

	EnumWindows(WNDENUMPROC(HwndEnumFunc), Pid);
	if (Hwnd == NULL)
	{
		SetProcessError(2, "Process State: HWND (Window) invalid", sizeof("Process State: HWND (Window) invalid"));
		return false;
	}

	BOOL Minimized = IsIconic(Hwnd);
	if (Minimized == TRUE)
	{
		SetProcessError(2, "Process State: HWND (Window) minimized", sizeof("Process State: HWND (Window) minimized"));
		return false;
	}

	RECT Rect;
	if (GetClientRect(Hwnd, &Rect) == FALSE || Rect.right < 16 || Rect.bottom < 16)
	{
		SetProcessError(2, "Process State: HWND (Window) invalid/minimized", sizeof("Process State: HWND (Window) invalid/minimized"));
		return false;
	}

	SetProcessError(1, "Process State: Process selected", sizeof("Process State: Process selected"));
	return true;
}

DWORD64 ErectusProcess::GetModuleBaseAddress(DWORD ProcessId, const char *Module)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, ProcessId);
	if (hSnapshot == INVALID_HANDLE_VALUE) return 0;

	MODULEENTRY32 lpme;
	lpme.dwSize = sizeof(lpme);

	while (Module32Next(hSnapshot, &lpme))
	{
		if (!strcmp(lpme.szModule, Module))
		{
			CloseHandle(hSnapshot);
			return DWORD64(lpme.modBaseAddr);
		}
	}

	CloseHandle(hSnapshot);
	return 0;
}

bool ErectusProcess::ProcessValid(DWORD ProcessId)
{
	Pid = ProcessId;
	if (!Pid)
	{
		SetProcessError(2, "Process State: PID (Process Id) invalid", sizeof("Process State: PID (Process Id) invalid"));
		return false;
	}

	Exe = GetModuleBaseAddress(Pid, "Fallout76.exe");
	if (!Exe)
	{
		SetProcessError(2, "Process State: Base Address invalid", sizeof("Process State: Base Address invalid"));
		return false;
	}

	Handle = OpenProcess(PROCESS_ALL_ACCESS, false, Pid);
	if (Handle == NULL)
	{
		SetProcessError(2, "Process State: HANDLE invalid", sizeof("Process State: HANDLE invalid"));
		return false;
	}

	SetProcessError(1, "Process State: Process selected", sizeof("Process State: Process selected"));
	return true;
}
