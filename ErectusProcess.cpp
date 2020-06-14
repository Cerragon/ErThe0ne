#include "ErectusInclude.h"

void ErectusProcess::SetProcessMenu()
{
	if (ErectusMain::windowSize[0] != 384 || ErectusMain::windowSize[1] != 224)
	{
		ErectusMain::windowSize[0] = 384;
		ErectusMain::windowSize[1] = 224;

		if (ErectusMain::windowHwnd != nullptr)
		{
			Renderer::deviceResetQueued = true;
			SetWindowPos(ErectusMain::windowHwnd, HWND_NOTOPMOST, ErectusMain::windowPosition[0], ErectusMain::windowPosition[1], ErectusMain::windowSize[0], ErectusMain::windowSize[1], 0);
		}
	}

	int bufferPosition[2];
	bufferPosition[0] = (GetSystemMetrics(SM_CXSCREEN) / 2) - (ErectusMain::windowSize[0] / 2);
	bufferPosition[1] = (GetSystemMetrics(SM_CYSCREEN) / 2) - (ErectusMain::windowSize[1] / 2);

	if (ErectusMain::windowPosition[0] != bufferPosition[0] || ErectusMain::windowPosition[1] != bufferPosition[1])
	{
		ErectusMain::windowPosition[0] = bufferPosition[0];
		ErectusMain::windowPosition[1] = bufferPosition[1];

		if (ErectusMain::windowHwnd != nullptr)
		{
			MoveWindow(ErectusMain::windowHwnd, ErectusMain::windowPosition[0], ErectusMain::windowPosition[1], ErectusMain::windowSize[0], ErectusMain::windowSize[1], FALSE);
			if (!Renderer::deviceResetQueued)
			{
				SetWindowPos(ErectusMain::windowHwnd, HWND_NOTOPMOST, ErectusMain::windowPosition[0], ErectusMain::windowPosition[1], ErectusMain::windowSize[0], ErectusMain::windowSize[1], 0);
			}
		}
	}

	if (ErectusMain::windowHwnd != nullptr)
	{
		auto style = GetWindowLongPtr(ErectusMain::windowHwnd, GWL_EXSTYLE);

		if (style & WS_EX_LAYERED)
		{
			style &= ~WS_EX_LAYERED;
			SetWindowLongPtr(ErectusMain::windowHwnd, GWL_EXSTYLE, style);
		}

		if (style & WS_EX_TOPMOST)
		{
			SetWindowPos(ErectusMain::windowHwnd, HWND_NOTOPMOST, ErectusMain::windowPosition[0], ErectusMain::windowPosition[1], ErectusMain::windowSize[0], ErectusMain::windowSize[1], 0);
		}
	}

	processMenuActive = true;
	ErectusMain::overlayMenuActive = false;
	ErectusMain::overlayActive = false;
}

void ErectusProcess::SetProcessError(const int errorId, const char* error)
{
	if (errorId == -1)
		return;
	processErrorId = errorId;
	processError = error;
}

void ErectusProcess::ResetProcessData(const bool clearProcessError, const int newProcessListSize)
{
	if (processList != nullptr)
	{
		if (processListSize)
		{
			for (auto i = 0; i < processListSize; i++)
			{
				if (processList[i] != nullptr)
				{
					delete[]processList[i];
					processList[i] = nullptr;
				}
			}
		}

		delete[]processList;
		processList = nullptr;
	}

	if (processIdList != nullptr)
	{
		delete[]processIdList;
		processIdList = nullptr;
	}

	processSelected = false;
	processIndex = 0;

	if (newProcessListSize)
	{
		processListSize = newProcessListSize;
		processList = new char* [processListSize];
		processList[processIndex] = new char[sizeof("No process selected")];
		sprintf_s(processList[processIndex], sizeof("No process selected"), "No process selected");
		processIdList = new DWORD[processListSize];
		processIdList[processIndex] = 0;

		if (!processMenuActive)
		{
			SetProcessMenu();
		}

		if (clearProcessError)
		{
			SetProcessError(0, "Process State: No process selected");
		}
	}
	else
	{
		processListSize = 0;
		SetProcessError(-1, "");
	}

	if (ErectusThread::threadCreationState)
	{
		auto areThreadsActive = false;

		while (!ErectusThread::ThreadDestruction())
		{
			ErectusThread::threadDestructionCounter++;
			if (ErectusThread::threadDestructionCounter > 14400)
			{
				areThreadsActive = true;

				if (newProcessListSize)
				{
					ErectusMain::CloseWnd();
				}

				break;
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}

		if (!areThreadsActive)
		{
			ErectusMemory::MessagePatcher(false);
		}
	}

	if (ErectusMemory::customEntityListUpdated)
	{
		ErectusMemory::DeleteCustomEntityList();
	}

	if (ErectusMemory::customNpcListUpdated)
	{
		ErectusMemory::DeleteCustomNpcList();
	}

	if (ErectusMemory::customPlayerListUpdated)
	{
		ErectusMemory::DeleteCustomPlayerList();
	}

	pid = 0;
	hWnd = nullptr;
	exe = 0;

	if (handle != nullptr)
	{
		CloseHandle(handle);
		handle = nullptr;
	}
}

int ErectusProcess::GetProcessCount()
{
	auto* hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE) return 1;

	PROCESSENTRY32 lppe;
	lppe.dwSize = sizeof(lppe);

	auto processCount = 1;
	while (Process32Next(hSnapshot, &lppe))
	{
		if (!strcmp(lppe.szExeFile, "Fallout76.exe"))
		{
			processCount++;
		}
	}

	CloseHandle(hSnapshot);
	return processCount;
}

bool ErectusProcess::UpdateProcessList()
{
	auto* const hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE) return false;

	PROCESSENTRY32 lppe;
	lppe.dwSize = sizeof(lppe);

	const auto processCount = GetProcessCount();
	if (processCount == 1)
	{
		CloseHandle(hSnapshot);
		return false;
	}
	
	ResetProcessData(true, processCount);

	auto currentProcess = 0;
	while (Process32Next(hSnapshot, &lppe))
	{
		if (!strcmp(lppe.szExeFile, "Fallout76.exe"))
		{
			currentProcess++;
			processList[currentProcess] = new char[sizeof("Fallout76.exe - 4294967295")];
			sprintf_s(processList[currentProcess], sizeof("Fallout76.exe - 4294967295"), "Fallout76.exe - %lu", lppe.th32ProcessID);
			processIdList[currentProcess] = lppe.th32ProcessID;
		}
	}

	CloseHandle(hSnapshot);
	return true;
}

BOOL ErectusProcess::HwndEnumFunc(const HWND hwnd, const LPARAM lParam)
{
	DWORD lpdwProcessId;
	GetWindowThreadProcessId(hwnd, &lpdwProcessId);

	if (lpdwProcessId == lParam)
	{
		char buffer[sizeof("Fallout76")] = { '\0' };
		if (GetClassName(hwnd, buffer, sizeof(buffer)))
		{
			if (!strcmp(buffer, "Fallout76"))
			{
				hWnd = hwnd;
				return FALSE;
			}
		}
	}

	hWnd = nullptr;
	return TRUE;
}

bool ErectusProcess::HwndValid(const DWORD processId)
{
	pid = processId;
	if (!pid)
	{
		SetProcessError(2, "Process State: PID (Process Id) invalid");
		return false;
	}

	EnumWindows(WNDENUMPROC(HwndEnumFunc), pid);
	if (hWnd == nullptr)
	{
		SetProcessError(2, "Process State: HWND (Window) invalid");
		return false;
	}

	if (IsIconic(hWnd))
	{
		SetProcessError(2, "Process State: HWND (Window) minimized");
		return false;
	}

	RECT rect;
	if (GetClientRect(hWnd, &rect) == FALSE || rect.right < 16 || rect.bottom < 16)
	{
		SetProcessError(2, "Process State: HWND (Window) invalid/minimized");
		return false;
	}

	SetProcessError(1, "Process State: Process selected");
	return true;
}

DWORD64 ErectusProcess::GetModuleBaseAddress(const DWORD pid, const char* module)
{
	auto* const hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
	if (hSnapshot == INVALID_HANDLE_VALUE) return 0;

	MODULEENTRY32 lpme;
	lpme.dwSize = sizeof(lpme);

	while (Module32Next(hSnapshot, &lpme))
	{
		if (!strcmp(lpme.szModule, module))
		{
			CloseHandle(hSnapshot);
			return DWORD64(lpme.modBaseAddr);
		}
	}

	CloseHandle(hSnapshot);
	return 0;
}

bool ErectusProcess::ProcessValid(const DWORD processId)
{
	pid = processId;
	if (!pid)
	{
		SetProcessError(2, "Process State: PID (Process Id) invalid");
		return false;
	}

	exe = GetModuleBaseAddress(pid, "Fallout76.exe");
	if (!exe)
	{
		SetProcessError(2, "Process State: Base Address invalid");
		return false;
	}

	handle = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
	if (handle == nullptr)
	{
		SetProcessError(2, "Process State: HANDLE invalid");
		return false;
	}

	SetProcessError(1, "Process State: Process selected");
	return true;
}
