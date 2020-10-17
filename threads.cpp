#include <thread>

#include "threads.h"
#include "app.h"
#include "settings.h"
#include "utils.h"
#include "common.h"

#include "ErectusMemory.h"
#include "ErectusProcess.h"
#include "features/Looter.h"
#include "features/Opk.h"
#include "features/PlayerStatsEditor.h"
#include "features/WeaponEditor.h"
#include "game/Game.h"

DWORD WINAPI Threads::BufferEntityListThread([[maybe_unused]] LPVOID lpParameter)
{
	while (!threadDestructionState)
	{
		ErectusMemory::UpdateBufferEntityList();
		std::this_thread::sleep_for(std::chrono::milliseconds(30 * 16));
	}

	ErectusMemory::entityDataBuffer.clear();

	bufferEntityListThreadActive = false;

	return 0xBEEF;
}

DWORD WINAPI Threads::BufferPlayerListThread([[maybe_unused]] LPVOID lpParameter)
{
	while (!threadDestructionState)
	{
		ErectusMemory::UpdateBufferPlayerList();
		std::this_thread::sleep_for(std::chrono::milliseconds(60 * 16));
	}

	ErectusMemory::playerDataBuffer.clear();

	bufferPlayerListThreadActive = false;

	return 0xFEED;
}

DWORD WINAPI Threads::WeaponEditorThread([[maybe_unused]] LPVOID lpParameter)
{
	while (!threadDestructionState)
	{
		WeaponEditor::EditWeapons(true);

		WeaponEditor::InfiniteAmmo(Settings::weapons.infiniteAmmo);

		std::this_thread::sleep_for(std::chrono::milliseconds(60 * 16));
	}

	WeaponEditor::EditWeapons(false);
	WeaponEditor::InfiniteAmmo(false);

	weaponEditorThreadActive = false;

	return 0xDEAD;
}

DWORD WINAPI Threads::LockingThread([[maybe_unused]] LPVOID lpParameter)
{
	auto weaponIdRefreshCooldown = 0;

	DWORD weaponId = 0;
	BYTE shotsHit = 1;
	BYTE shotsFired = 0;

	DWORD64 targetingPage = 0;
	auto targetingPageValid = false;

	auto sendDamageCounter = 0;
	auto sendDamageThreshold = 0;

	bool targetLockingKeyPressed;
	auto targetLockingCooldown = 0;

	while (!threadDestructionState)
	{
		if (Settings::targetting.lockNpCs)
		{
			auto currentTargetValid = false;
			DWORD64 closestEntityPtr = 0;
			auto closestEntityDegrees = Settings::targetting.lockingFov;

			//refresh weaponId every 60 loops
			if (weaponIdRefreshCooldown == 0)
			{
				weaponIdRefreshCooldown = 60;
				weaponId = ErectusMemory::GetFavoritedWeaponId(static_cast<BYTE>(Settings::targetting.favoriteIndex));
			}
			weaponIdRefreshCooldown--;

			if (gApp->GetMode() == App::Mode::Overlay && GetAsyncKeyState('T'))
			{
				targetLockingKeyPressed = true;
				if (targetLockingCooldown > 0)
					targetLockingCooldown--;
			}
			else
			{
				targetLockingKeyPressed = false;
				targetLockingCooldown = 0;
				ErectusMemory::targetLockedEntityPtr = 0;
			}

			if (ErectusMemory::targetLockedEntityPtr || targetLockingKeyPressed)
			{
				auto camera = Game::GetPlayerCamera();
				auto entities = ErectusMemory::entityDataBuffer;
				for (const auto& entity : entities)
				{
					if (!(entity.flag & CUSTOM_ENTRY_NPC) || !Settings::targetting.lockNpCs)
						continue;

					TesObjectRefr entityData{};
					if (!ErectusProcess::Rpm(entity.entityPtr, &entityData, sizeof entityData))
						continue;

					if (!ErectusMemory::IsTargetValid(entityData))
						continue;

					if (entity.entityPtr == ErectusMemory::targetLockedEntityPtr)
					{
						currentTargetValid = true;
					}
					else if (targetLockingKeyPressed && !targetLockingCooldown)
					{
						const auto degrees = Utils::GetDegrees(entityData.position, camera.forward, camera.origin);
						if (degrees < closestEntityDegrees)
						{
							closestEntityDegrees = degrees;
							closestEntityPtr = entity.entityPtr;
						}
					}
				}
			}

			if (!currentTargetValid)
			{
				if (ErectusMemory::targetLockedEntityPtr)
				{
					targetLockingCooldown = Settings::targetting.retargeting ? Settings::targetting.cooldown : -1;
					ErectusMemory::targetLockedEntityPtr = 0;
				}
				else if (closestEntityDegrees < Settings::targetting.lockingFov)
				{
					targetLockingCooldown = 0;
					ErectusMemory::targetLockedEntityPtr = closestEntityPtr;
				}
			}

			if (ErectusMemory::targetLockedEntityPtr)
			{
				ErectusMemory::DamageRedirection(ErectusMemory::targetLockedEntityPtr, targetingPage, targetingPageValid, false, true);

				sendDamageCounter++;
				if (sendDamageCounter > sendDamageThreshold)
				{
					sendDamageCounter = 0;
					sendDamageThreshold = Utils::GetRangedInt(Settings::targetting.sendDamageMin, Settings::targetting.sendDamageMax);
					ErectusMemory::SendDamage(ErectusMemory::targetLockedEntityPtr, weaponId, &shotsHit, &shotsFired, 1);
				}
			}
			else
			{
				ErectusMemory::DamageRedirection(ErectusMemory::targetLockedEntityPtr, targetingPage, targetingPageValid, false, false);
				sendDamageThreshold = 0;
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

	ErectusMemory::DamageRedirection(ErectusMemory::targetLockedEntityPtr, targetingPage, targetingPageValid, true, false);

	if (targetingPage)
		ErectusProcess::FreeEx(targetingPage);

	lockingThreadActive = false;

	return 0xCAFE;
}

DWORD WINAPI Threads::MultihackThread([[maybe_unused]] LPVOID lpParameter)
{
	auto meleeCounter = 0;
	auto meleeThreshold = 0;

	std::uintptr_t freezeApPage = 0;
	auto freezeApPageValid = false;

	auto loopCount = -1;
	while (!threadDestructionState)
	{
		loopCount = (loopCount + 1) % 30000;

		if (positionSpoofingToggle && Settings::localPlayer.positionSpoofingEnabled && Settings::localPlayer.clientState)
			ErectusMemory::SetClientState(2);

		if (noclipToggle && Settings::localPlayer.noclipEnabled && Settings::localPlayer.clientState)
			ErectusMemory::SetClientState(2);

		ErectusMemory::PositionSpoofing(positionSpoofingToggle);

		ErectusMemory::Noclip(noclipToggle);

		PlayerStatsEditor::Edit(true);

		ErectusMemory::FreezeActionPoints(freezeApPage, freezeApPageValid, true);

		Opk::OnePositionKill(true);
		Opk::SetOpkData(opkNpcsToggle);

		if (Settings::customNukeCodeSettings.automaticNukeCodes)
		{
			if (loopCount % 300 == 0) { //every 300 loops
				ErectusMemory::UpdateNukeCodes();
			}
		}

		if (loopCount % 10 == 0) //every 10 loops
			ErectusMemory::ChargenEditing();

		if (Settings::melee.enabled)
		{
			if (gApp->GetMode() == App::Mode::Overlay && GetAsyncKeyState('U'))
			{
				meleeCounter++;
				if (meleeCounter > meleeThreshold)
				{
					meleeCounter = 0;
					meleeThreshold = Utils::GetRangedInt(Settings::melee.speedMin, Settings::melee.speedMax);
					ErectusMemory::MeleeAttack();
				}
			}
			else
				meleeThreshold = 0;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

	ErectusMemory::PositionSpoofing(false);
	ErectusMemory::Noclip(false);

	PlayerStatsEditor::Edit(false);

	Opk::OnePositionKill(false);

	ErectusMemory::FreezeActionPoints(freezeApPage, freezeApPageValid, false);

	if (freezeApPage)
		ErectusProcess::FreeEx(freezeApPage);

	multihackThreadActive = false;

	return 0xBEAD;
}

DWORD WINAPI Threads::Looter([[maybe_unused]] LPVOID lpParameter)
{
	while (!threadDestructionState)
	{
		Looter::Loot();
		std::this_thread::sleep_for(std::chrono::milliseconds(Utils::GetRangedInt(36, 72) * 16));
	}

	looterThreadActive = false;

	return 0xDEAF;
}

bool Threads::CreateProcessThreads()
{
	if (threadDestructionQueued)
		return false;

	if (threadDestructionState)
		return false;

	if (threadDestructionCounter)
		return false;

	if (!bufferEntityListThreadActive)
	{
		bufferEntityListThreadActive = CloseHandle(CreateThread(nullptr, 0, &BufferEntityListThread, nullptr, 0, nullptr));
		if (!bufferEntityListThreadActive)
		{
			threadDestructionQueued = true;
			return false;
		}
	}

	if (!bufferPlayerListThreadActive)
	{
		bufferPlayerListThreadActive = CloseHandle(CreateThread(nullptr, 0, &BufferPlayerListThread, nullptr, 0, nullptr));
		if (!bufferPlayerListThreadActive)
		{
			threadDestructionQueued = true;
			return false;
		}
	}

	if (!weaponEditorThreadActive)
	{
		weaponEditorThreadActive = CloseHandle(CreateThread(nullptr, 0, &WeaponEditorThread, nullptr, 0, nullptr));
		if (!weaponEditorThreadActive)
		{
			threadDestructionQueued = true;
			return false;
		}
	}

	if (!lockingThreadActive)
	{
		lockingThreadActive = CloseHandle(CreateThread(nullptr, 0, &LockingThread, nullptr, 0, nullptr));
		if (!lockingThreadActive)
		{
			threadDestructionQueued = true;
			return false;
		}
	}

	if (!multihackThreadActive)
	{
		multihackThreadActive = CloseHandle(CreateThread(nullptr, 0, &MultihackThread, nullptr, 0, nullptr));
		if (!multihackThreadActive)
		{
			threadDestructionQueued = true;
			return false;
		}
	}

	if (!looterThreadActive)
	{
		looterThreadActive = CloseHandle(CreateThread(nullptr, 0, &Looter, nullptr, 0, nullptr));
		if (!looterThreadActive)
		{
			threadDestructionQueued = true;
			return false;
		}
	}

	return true;
}

bool Threads::ThreadDestruction()
{
	threadDestructionState = true;

	if (bufferEntityListThreadActive)
		return false;

	if (bufferPlayerListThreadActive)
		return false;

	if (weaponEditorThreadActive)
		return false;

	if (lockingThreadActive)
		return false;

	if (multihackThreadActive)
		return false;

	if (looterThreadActive)
		return false;

	threadCreationState = false;
	threadDestructionQueued = false;
	threadDestructionState = false;
	threadDestructionCounter = 0;

	return true;
}
