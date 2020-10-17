#include "Opk.h"

#include "../ErectusProcess.h"
#include "../settings.h"
#include "../game/Game.h"

bool Opk::OnePositionKill(const bool enabled)
{

	if (!opkPage && !Settings::opk.enabled)
		return false;

	if (!opkPage)
	{
		const auto page = ErectusProcess::AllocEx(sizeof(Opk));
		if (!page)
			return false;
		opkPage = page;
	}

	std::array<unsigned char, 15> opkOn = { 0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xE0, 0xCC, 0xCC, 0xCC };

	std::array<unsigned char, 15> opkCheck = {};
	if (!ErectusProcess::Rpm(ErectusProcess::exe + OFFSET_OPK, opkCheck.data(), opkCheck.size()))
		return false;

	const auto originalFunction = ErectusProcess::exe + OFFSET_OPK + opk_off.size();
	memcpy(&opkOn[2], &opkPage, sizeof(std::uintptr_t));

	std::uintptr_t pageCheck;
	memcpy(&pageCheck, &opkCheck[2], sizeof(std::uintptr_t));

	if (Utils::Valid(pageCheck) && pageCheck != opkPage)
	{
		OpkData buffer;
		if (!ErectusProcess::Rpm(pageCheck, &buffer, sizeof buffer))
			return false;
		if (buffer.originalFunction != originalFunction)
			return false;
		if (!ErectusProcess::Wpm(ErectusProcess::exe + OFFSET_OPK, opk_off.data(), opk_off.size()))
			return false;
		ErectusProcess::FreeEx(pageCheck);
	}

	if (enabled)
	{
		if (opkPageValid)
			return true;

		OpkData opkData = {
			.originalFunction = originalFunction,
		};
		if (!ErectusProcess::Wpm(opkPage, &opkData, sizeof opkData))
			return false;
		if (!ErectusProcess::Wpm(ErectusProcess::exe + OFFSET_OPK, &opkOn, sizeof opkOn))
			return false;
		opkPageValid = true;
	}
	else
	{
		if (pageCheck == opkPage)
			ErectusProcess::Wpm(ErectusProcess::exe + OFFSET_OPK, opk_off.data(), opk_off.size());

		if (opkPage && ErectusProcess::FreeEx(opkPage))
		{
			opkPage = 0;
			opkPageValid = false;
		}
	}
	return true;
}

bool Opk::SetOpkData(const bool enabled)
{
	OpkData opkData;
	if (!ErectusProcess::Rpm(opkPage, &opkData, sizeof opkData))
		return false;

	if (!enabled)
	{
		opkData.opkNpcs = 0;
		memset(opkData.opkNpcPosition, 0x00, sizeof opkData.opkNpcPosition);
		ErectusProcess::Wpm(opkPage, &opkData, sizeof opkData);

		return true;
	}

	const auto camera = Game::GetPlayerCamera();
	const Vector3 opkNpcPositionCurrent = { opkData.opkNpcPosition[0], opkData.opkNpcPosition[1], opkData.opkNpcPosition[2] };
	if (opkNpcPositionCurrent.DistanceTo(camera.origin / 70.f) <= 20.0f)
		return true;

	const auto opkPosition = Utils::ProjectView(camera.forward, camera.origin / 70.f, 3.0f);

	opkData.opkNpcPosition[0] = opkPosition.x;
	opkData.opkNpcPosition[1] = opkPosition.y;
	opkData.opkNpcPosition[2] = opkPosition.z;
	opkData.opkNpcs = 1;

	return ErectusProcess::Wpm(opkPage, &opkData, sizeof opkData);
}
