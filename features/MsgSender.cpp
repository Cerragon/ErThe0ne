#include "MsgSender.h"

#include <memory>

#include "../ErectusMemory.h"
#include "../ErectusProcess.h"
#include "../settings.h"

bool MsgSender::IsEnabled()
{
	if (!Settings::msgWriter.enabled)
		return false;

	return Patcher(Settings::msgWriter.enabled);
}

bool MsgSender::Send(void* message, const size_t size)
{
	if (!IsEnabled())
		return false;

	const auto allocSize = size + sizeof(ExternalFunction);
	const auto allocAddress = ErectusProcess::AllocEx(allocSize);
	if (allocAddress == 0)
		return false;

	ExternalFunction externalFunctionData = {
		.address = ErectusProcess::exe + OFFSET_MESSAGE_SENDER,
		.rcx = allocAddress + sizeof(ExternalFunction),
		.rdx = 0,
		.r8 = 0,
		.r9 = 0
	};

	const auto pageData = std::make_unique<BYTE[]>(allocSize);
	memset(pageData.get(), 0x00, allocSize);
	memcpy(pageData.get(), &externalFunctionData, sizeof externalFunctionData);
	memcpy(&pageData.get()[sizeof(ExternalFunction)], message, size);
	const auto written = ErectusProcess::Wpm(allocAddress, pageData.get(), allocSize);

	if (!written)
	{
		ErectusProcess::FreeEx(allocAddress);
		return false;
	}

	const auto paramAddress = allocAddress + sizeof ExternalFunction::ASM;
	auto* const thread = CreateRemoteThread(ErectusProcess::handle, nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(allocAddress),
		reinterpret_cast<LPVOID>(paramAddress), 0, nullptr);

	if (thread == nullptr)
	{
		ErectusProcess::FreeEx(allocAddress);
		return false;
	}

	const auto threadResult = WaitForSingleObject(thread, 3000);
	CloseHandle(thread);

	if (threadResult == WAIT_TIMEOUT)
		return false;

	ErectusProcess::FreeEx(allocAddress);

	return true;
}

bool MsgSender::Patcher(const bool enabled)
{
	BYTE fakeMessagesCheck[4];
	if (!ErectusProcess::Rpm(ErectusProcess::exe + OFFSET_FAKE_MESSAGE, &fakeMessagesCheck, sizeof fakeMessagesCheck))
		return false;

	BYTE fakeMessagesEnabled[] = { 0x48, 0x31, 0xC0, 0xC3 };
	BYTE fakeMessagesDisabled[] = { 0x40, 0x53, 0x56, 0x57 };

	if (!memcmp(fakeMessagesCheck, fakeMessagesEnabled, sizeof fakeMessagesEnabled))
	{
		if (enabled)
			return true;

		return ErectusProcess::Wpm(ErectusProcess::exe + OFFSET_FAKE_MESSAGE, &fakeMessagesDisabled, sizeof fakeMessagesDisabled);
	}

	if (!memcmp(fakeMessagesCheck, fakeMessagesDisabled, sizeof fakeMessagesDisabled))
	{
		if (enabled)
			return ErectusProcess::Wpm(ErectusProcess::exe + OFFSET_FAKE_MESSAGE, &fakeMessagesEnabled, sizeof fakeMessagesEnabled);
		return true;
	}

	return false;
}
