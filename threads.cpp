#include "threads.h"
#include "app.h"
#include "settings.h"
#include "utils.h"
#include "common.h"
#include <thread>

#include "ErectusMemory.h"
#include "ErectusProcess.h"
#include "Looter.h"

DWORD WINAPI Threads::BufferEntityListThread([[maybe_unused]] LPVOID lpParameter)
{
	while (!threadDestructionState)
	{
		ErectusMemory::UpdateBufferEntityList();
		std::this_thread::sleep_for(std::chrono::milliseconds(60 * 16));
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
		ErectusMemory::oldWeaponListCounter++;
		if (ErectusMemory::oldWeaponListCounter > 60)
		{
			ErectusMemory::oldWeaponListCounter = 0;
			if (!ErectusMemory::oldWeaponListUpdated)
				ErectusMemory::oldWeaponListUpdated = ErectusMemory::UpdateOldWeaponData();

			if (ErectusMemory::oldWeaponListUpdated)
			{
				auto revertWeapons = true;
				if (ErectusMemory::WeaponEditingEnabled())
					revertWeapons = false;

				for (auto i = 0; i < ErectusMemory::oldWeaponListSize; i++)
				{
					if (threadDestructionState)
						break;
					ErectusMemory::EditWeapon(i, revertWeapons);
				}
			}
			ErectusMemory::InfiniteAmmo(Settings::weapons.infiniteAmmo);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

	if (ErectusMemory::oldWeaponListUpdated)
	{
		for (auto i = 0; i < ErectusMemory::oldWeaponListSize; i++)
		{
			ErectusMemory::EditWeapon(i, true);
		}
	}

	ErectusMemory::InfiniteAmmo(false);

	ErectusMemory::DeleteOldWeaponList();

	weaponEditorThreadActive = false;

	return 0xDEAD;
}

DWORD WINAPI Threads::LockingThread([[maybe_unused]] LPVOID lpParameter)
{
	srand(static_cast<unsigned int>(time(nullptr)));

	auto favoritedWeaponCounter = 0;

	DWORD weaponId = 0;
	BYTE shotsHit = 1;
	BYTE shotsFired = 0;

	DWORD64 targetingPage = 0;
	auto targetingPageValid = false;

	auto sendDamageCounter = 0;
	auto sendDamageThreshold = 0;

	auto meleeCounter = 0;
	auto meleeThreshold = 0;

	while (!threadDestructionState)
	{
		//no clue if this works properly
		//begin targetting

		auto currentTargetValid = false;
		DWORD64 closestEntityPtr = 0;
		auto closestEntityDegrees = Settings::targetting.lockingFov;


		auto cameraData = ErectusMemory::GetCameraInfo();
		auto entities = ErectusMemory::entityDataBuffer;
		for (const auto& entity : entities)
		{
			if ((!(entity.flag & CUSTOM_ENTRY_PLAYER) || !Settings::targetting.lockPlayers) && (!(entity.flag & CUSTOM_ENTRY_NPC) || !Settings::targetting.lockNpCs))
				continue;

			TesObjectRefr entityData{};
			if (!ErectusProcess::Rpm(entity.entityPtr, &entityData, sizeof entityData))
				continue;

			if (!ErectusMemory::TargetValid(entityData))
				continue;

			if (entity.entityPtr == ErectusMemory::targetLockingPtr)
			{
				currentTargetValid = true;
			}
			else if (ErectusMemory::targetLockingKeyPressed && !ErectusMemory::targetLockingCooldown)
			{
				auto degrees = Utils::GetDegrees(entityData.position, cameraData.forward, cameraData.origin);
				if (degrees < closestEntityDegrees)
				{
					closestEntityDegrees = degrees;
					closestEntityPtr = entity.entityPtr;
				}
			}
		}

		if (!currentTargetValid)
		{
			if (ErectusMemory::targetLockingPtr)
			{
				ErectusMemory::targetLockingCooldown = Settings::targetting.retargeting ? Settings::targetting.cooldown : -1;
				ErectusMemory::targetLockingPtr = 0;
			}
			else if (closestEntityDegrees < Settings::targetting.lockingFov)
			{
				ErectusMemory::targetLockingCooldown = 0;
				ErectusMemory::targetLockingPtr = closestEntityPtr;
			}
		}
		//end targetting

		favoritedWeaponCounter++;
		if (favoritedWeaponCounter > 60)
		{
			favoritedWeaponCounter = 0;
			weaponId = ErectusMemory::GetFavoritedWeaponId(BYTE(Settings::targetting.favoriteIndex));
		}

		if (App::overlayForeground && GetAsyncKeyState('T'))
		{
			ErectusMemory::targetLockingKeyPressed = true;
			if (ErectusMemory::targetLockingCooldown > 0)
				ErectusMemory::targetLockingCooldown--;
		}
		else
		{
			ErectusMemory::targetLockingKeyPressed = false;
			ErectusMemory::targetLockingCooldown = 0;
			ErectusMemory::targetLockingPtr = 0;
		}

		if (ErectusMemory::targetLockingPtr)
		{
			ErectusMemory::DamageRedirection(&targetingPage, &targetingPageValid, false, true);

			sendDamageCounter++;
			if (sendDamageCounter > sendDamageThreshold)
			{
				sendDamageCounter = 0;
				sendDamageThreshold = Utils::GetRangedInt(Settings::targetting.sendDamageMin, Settings::targetting.sendDamageMax);
				ErectusMemory::SendDamage(weaponId, &shotsHit, &shotsFired, 1);
			}
		}
		else
		{
			ErectusMemory::DamageRedirection(&targetingPage, &targetingPageValid, false, false);
			sendDamageThreshold = 0;
		}

		if (Settings::melee.enabled)
		{
			if (App::overlayForeground && GetAsyncKeyState('U'))
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

	ErectusMemory::DamageRedirection(&targetingPage, &targetingPageValid, true, false);

	if (targetingPage)
		ErectusProcess::FreeEx(targetingPage);

	lockingThreadActive = false;

	return 0xCAFE;
}

DWORD WINAPI Threads::MultihackThread([[maybe_unused]] LPVOID lpParameter)
{
	DWORD64 actorValuePage = 0;
	auto actorValuePageValid = false;

	DWORD64 opkPage = 0;
	auto opkPageValid = false;

	DWORD64 freezeApPage = 0;
	auto freezeApPageValid = false;

	auto loopCount = -1;
	while (!threadDestructionState)
	{
		loopCount = (loopCount + 1) % 30000;

		if (positionSpoofingToggle && Settings::customLocalPlayerSettings.positionSpoofingEnabled && Settings::customLocalPlayerSettings.clientState)
			ErectusMemory::SetClientState(2);

		if (noclipToggle && Settings::customLocalPlayerSettings.noclipEnabled && Settings::customLocalPlayerSettings.clientState)
			ErectusMemory::SetClientState(2);

		if ((positionSpoofingToggle || noclipToggle) && Settings::customLocalPlayerSettings.automaticClientState)
		{
			if (loopCount % 60 == 0) //every 60 loops
				ErectusMemory::SetClientState(2);
		}

		ErectusMemory::PositionSpoofing(positionSpoofingToggle);
		ErectusMemory::Noclip(noclipToggle);

		ErectusMemory::ActorValue(&actorValuePage, &actorValuePageValid, true);
		if (loopCount % 60 == 0) { //every 60 loops
			ErectusMemory::SetActorValueMaximum(0x000002C2, 100.0f, static_cast<float>(Settings::customLocalPlayerSettings.strength), Settings::customLocalPlayerSettings.strengthEnabled);
			ErectusMemory::SetActorValueMaximum(0x000002C3, 100.0f, static_cast<float>(Settings::customLocalPlayerSettings.perception), Settings::customLocalPlayerSettings.perceptionEnabled);
			ErectusMemory::SetActorValueMaximum(0x000002C4, 100.0f, static_cast<float>(Settings::customLocalPlayerSettings.endurance), Settings::customLocalPlayerSettings.enduranceEnabled);
			ErectusMemory::SetActorValueMaximum(0x000002C5, 100.0f, static_cast<float>(Settings::customLocalPlayerSettings.charisma), Settings::customLocalPlayerSettings.charismaEnabled);
			ErectusMemory::SetActorValueMaximum(0x000002C6, 100.0f, static_cast<float>(Settings::customLocalPlayerSettings.agility), Settings::customLocalPlayerSettings.agilityEnabled);
			ErectusMemory::SetActorValueMaximum(0x000002C7, 100.0f, static_cast<float>(Settings::customLocalPlayerSettings.intelligence), Settings::customLocalPlayerSettings.intelligenceEnabled);
			ErectusMemory::SetActorValueMaximum(0x000002C8, 100.0f, static_cast<float>(Settings::customLocalPlayerSettings.luck), Settings::customLocalPlayerSettings.luckEnabled);
		}

		ErectusMemory::FreezeActionPoints(&freezeApPage, &freezeApPageValid, true);
		ErectusMemory::OnePositionKill(&opkPage, &opkPageValid, true);

		if (opkPageValid)
		{
			if (opkPlayersToggle)
				ErectusMemory::SetOpkData(opkPage, true, true);
			else
				ErectusMemory::SetOpkData(opkPage, true, false);

			if (opkNpcsToggle)
				ErectusMemory::SetOpkData(opkPage, false, true);
			else
				ErectusMemory::SetOpkData(opkPage, false, false);
		}

		if (Settings::customNukeCodeSettings.automaticNukeCodes)
		{
			if (loopCount % 300 == 0) { //every 300 loops
				ErectusMemory::UpdateNukeCodes();
			}
		}

		if (loopCount % 10 == 0) //every 10 loops
			ErectusMemory::ChargenEditing();

		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

	ErectusMemory::PositionSpoofing(false);
	ErectusMemory::Noclip(false);
	ErectusMemory::ActorValue(&actorValuePage, &actorValuePageValid, false);

	if (actorValuePage)
		ErectusProcess::FreeEx(actorValuePage);

	ErectusMemory::SetActorValueMaximum(0x000002C2, 100.0f, static_cast<float>(Settings::customLocalPlayerSettings.strength), false);
	ErectusMemory::SetActorValueMaximum(0x000002C3, 100.0f, static_cast<float>(Settings::customLocalPlayerSettings.perception), false);
	ErectusMemory::SetActorValueMaximum(0x000002C4, 100.0f, static_cast<float>(Settings::customLocalPlayerSettings.endurance), false);
	ErectusMemory::SetActorValueMaximum(0x000002C5, 100.0f, static_cast<float>(Settings::customLocalPlayerSettings.charisma), false);
	ErectusMemory::SetActorValueMaximum(0x000002C6, 100.0f, static_cast<float>(Settings::customLocalPlayerSettings.agility), false);
	ErectusMemory::SetActorValueMaximum(0x000002C7, 100.0f, static_cast<float>(Settings::customLocalPlayerSettings.intelligence), false);
	ErectusMemory::SetActorValueMaximum(0x000002C8, 100.0f, static_cast<float>(Settings::customLocalPlayerSettings.luck), false);

	ErectusMemory::OnePositionKill(&opkPage, &opkPageValid, false);

	if (opkPage)
		ErectusProcess::FreeEx(opkPage);

	ErectusMemory::FreezeActionPoints(&freezeApPage, &freezeApPageValid, false);

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
		std::this_thread::sleep_for(std::chrono::milliseconds(Utils::GetRangedInt(12, 36) * 16));
	}

	harvesterThreadActive = false;

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

	if (!harvesterThreadActive)
	{
		harvesterThreadActive = CloseHandle(CreateThread(nullptr, 0, &Looter, nullptr, 0, nullptr));
		if (!harvesterThreadActive)
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

	if (harvesterThreadActive)
		return false;

	threadCreationState = false;
	threadDestructionQueued = false;
	threadDestructionState = false;
	threadDestructionCounter = 0;

	return true;
}
