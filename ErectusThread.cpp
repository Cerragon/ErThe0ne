#include "ErectusInclude.h"

void ErectusThread::RequestLootItems()
{
	lootItemsRequested = true;
}

void ErectusThread::RequestLootScrap()
{
	lootScrapRequested = true;
}

DWORD WINAPI ErectusThread::BufferEntityListThread([[maybe_unused]] LPVOID lpParameter)
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

DWORD WINAPI ErectusThread::BufferNpcListThread([[maybe_unused]] LPVOID lpParameter)
{
	while (!threadDestructionState)
	{
		ErectusMemory::UpdateBufferNpcList();
		std::this_thread::sleep_for(std::chrono::milliseconds(60 * 16));
	}

	ErectusMemory::npcDataBuffer.clear();

	bufferNpcListThreadActive = false;

	return 0xFADE;
}

DWORD WINAPI ErectusThread::BufferPlayerListThread([[maybe_unused]] LPVOID lpParameter)
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

DWORD WINAPI ErectusThread::WeaponEditorThread([[maybe_unused]] LPVOID lpParameter)
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
			ErectusMemory::InfiniteAmmo(ErectusIni::customWeaponSettings.infiniteAmmo);
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

DWORD WINAPI ErectusThread::LockingThread([[maybe_unused]] LPVOID lpParameter)
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
		favoritedWeaponCounter++;
		if (favoritedWeaponCounter > 60)
		{
			favoritedWeaponCounter = 0;
			weaponId = ErectusMemory::GetFavoritedWeaponId(BYTE(ErectusIni::customTargetSettings.favoriteIndex));

		}

		if (ErectusMain::overlayForeground && GetAsyncKeyState('T'))
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
				sendDamageThreshold = Utils::GetRangedInt(ErectusIni::customTargetSettings.sendDamageMin, ErectusIni::customTargetSettings.sendDamageMax);
				ErectusMemory::SendDamage(weaponId, &shotsHit, &shotsFired, 1);
			}
		}
		else
		{
			ErectusMemory::DamageRedirection(&targetingPage, &targetingPageValid, false, false);
			sendDamageThreshold = 0;
		}

		if (ErectusIni::melee.enabled)
		{
			if (ErectusMain::overlayForeground && GetAsyncKeyState('U'))
			{
				meleeCounter++;
				if (meleeCounter > meleeThreshold)
				{
					meleeCounter = 0;
					meleeThreshold = Utils::GetRangedInt(ErectusIni::melee.speedMin, ErectusIni::melee.speedMax);
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
		Utils::FreeEx(targetingPage);

	lockingThreadActive = false;

	return 0xCAFE;
}

DWORD WINAPI ErectusThread::MultihackThread([[maybe_unused]] LPVOID lpParameter)
{
	DWORD64 actorValuePage = 0;
	auto actorValuePageValid = false;

	DWORD64 opkPage = 0;
	auto opkPageValid = false;

	DWORD64 freezeApPage = 0;
	auto freezeApPageValid = false;

	auto lootScrapCounter = 0;
	auto lootScrapThreshold = 0;

	auto lootItemsCounter = 0;
	auto lootItemsThreshold = 0;

	auto loopCount = -1;
	while (!threadDestructionState)
	{
		loopCount = (loopCount + 1) % 30000;

		if (positionSpoofingToggle && ErectusIni::customLocalPlayerSettings.positionSpoofingEnabled && ErectusIni::customLocalPlayerSettings.clientState)
			ErectusMemory::SetClientState(2);

		if (noclipToggle && ErectusIni::customLocalPlayerSettings.noclipEnabled && ErectusIni::customLocalPlayerSettings.clientState)
			ErectusMemory::SetClientState(2);

		if (lootScrapRequested) {
			ErectusMemory::LootScrap();
			lootScrapRequested = false;
		}

		if (lootItemsRequested) {
			ErectusMemory::LootItems();
			lootItemsRequested = false;
		}

		if ((positionSpoofingToggle || noclipToggle) && ErectusIni::customLocalPlayerSettings.automaticClientState)
		{
			if (loopCount % 60 == 0) //every 60 loops
				ErectusMemory::SetClientState(2);
		}

		ErectusMemory::PositionSpoofing(positionSpoofingToggle);
		ErectusMemory::Noclip(noclipToggle);

		ErectusMemory::ActorValue(&actorValuePage, &actorValuePageValid, true);
		if (loopCount % 60 == 0) { //every 60 loops
			ErectusMemory::SetActorValueMaximum(0x000002C2, 100.0f, static_cast<float>(ErectusIni::customLocalPlayerSettings.strength), ErectusIni::customLocalPlayerSettings.strengthEnabled);
			ErectusMemory::SetActorValueMaximum(0x000002C3, 100.0f, static_cast<float>(ErectusIni::customLocalPlayerSettings.perception), ErectusIni::customLocalPlayerSettings.perceptionEnabled);
			ErectusMemory::SetActorValueMaximum(0x000002C4, 100.0f, static_cast<float>(ErectusIni::customLocalPlayerSettings.endurance), ErectusIni::customLocalPlayerSettings.enduranceEnabled);
			ErectusMemory::SetActorValueMaximum(0x000002C5, 100.0f, static_cast<float>(ErectusIni::customLocalPlayerSettings.charisma), ErectusIni::customLocalPlayerSettings.charismaEnabled);
			ErectusMemory::SetActorValueMaximum(0x000002C6, 100.0f, static_cast<float>(ErectusIni::customLocalPlayerSettings.agility), ErectusIni::customLocalPlayerSettings.agilityEnabled);
			ErectusMemory::SetActorValueMaximum(0x000002C7, 100.0f, static_cast<float>(ErectusIni::customLocalPlayerSettings.intelligence), ErectusIni::customLocalPlayerSettings.intelligenceEnabled);
			ErectusMemory::SetActorValueMaximum(0x000002C8, 100.0f, static_cast<float>(ErectusIni::customLocalPlayerSettings.luck), ErectusIni::customLocalPlayerSettings.luckEnabled);
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

		if (ErectusIni::customNukeCodeSettings.automaticNukeCodes)
		{
			if (loopCount % 300 == 0) { //every 300 loops
				ErectusMemory::GetNukeCode(0x000921AE, ErectusImGui::alphaCode);
				ErectusMemory::GetNukeCode(0x00092213, ErectusImGui::bravoCode);
				ErectusMemory::GetNukeCode(0x00092214, ErectusImGui::charlieCode);
			}
		}

		if (ErectusIni::scrapLooter.autoLootingEnabled)
		{
			lootScrapCounter++;
			if (lootScrapCounter > lootScrapThreshold)
			{
				lootScrapCounter = 0;
				lootScrapThreshold = Utils::GetRangedInt(ErectusIni::scrapLooter.autoLootingSpeedMin, ErectusIni::scrapLooter.autoLootingSpeedMax);
				if (ErectusMemory::CheckScrapList())
				{
					ErectusMemory::LootScrap();
				}
			}
		}
		else
			lootScrapThreshold = 0;

		if (ErectusIni::itemLooter.autoLootingEnabled)
		{
			lootItemsCounter++;
			if (lootItemsCounter > lootItemsThreshold)
			{
				lootItemsCounter = 0;
				lootItemsThreshold = Utils::GetRangedInt(ErectusIni::itemLooter.autoLootingSpeedMin, ErectusIni::itemLooter.autoLootingSpeedMax);
				if (ErectusMemory::CheckItemLooterSettings())
					ErectusMemory::LootItems();
			}
		}
		else
			lootItemsThreshold = 0;


		if (loopCount % 10 == 0) //every 10 loops
			ErectusMemory::ChargenEditing();

		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

	ErectusMemory::PositionSpoofing(false);
	ErectusMemory::Noclip(false);
	ErectusMemory::ActorValue(&actorValuePage, &actorValuePageValid, false);

	if (actorValuePage)
		Utils::FreeEx(actorValuePage);

	ErectusMemory::SetActorValueMaximum(0x000002C2, 100.0f, static_cast<float>(ErectusIni::customLocalPlayerSettings.strength), false);
	ErectusMemory::SetActorValueMaximum(0x000002C3, 100.0f, static_cast<float>(ErectusIni::customLocalPlayerSettings.perception), false);
	ErectusMemory::SetActorValueMaximum(0x000002C4, 100.0f, static_cast<float>(ErectusIni::customLocalPlayerSettings.endurance), false);
	ErectusMemory::SetActorValueMaximum(0x000002C5, 100.0f, static_cast<float>(ErectusIni::customLocalPlayerSettings.charisma), false);
	ErectusMemory::SetActorValueMaximum(0x000002C6, 100.0f, static_cast<float>(ErectusIni::customLocalPlayerSettings.agility), false);
	ErectusMemory::SetActorValueMaximum(0x000002C7, 100.0f, static_cast<float>(ErectusIni::customLocalPlayerSettings.intelligence), false);
	ErectusMemory::SetActorValueMaximum(0x000002C8, 100.0f, static_cast<float>(ErectusIni::customLocalPlayerSettings.luck), false);

	ErectusMemory::OnePositionKill(&opkPage, &opkPageValid, false);

	if (opkPage)
		Utils::FreeEx(opkPage);

	ErectusMemory::FreezeActionPoints(&freezeApPage, &freezeApPageValid, false);

	if (freezeApPage)
		Utils::FreeEx(freezeApPage);

	multihackThreadActive = false;

	return 0xBEAD;
}

DWORD WINAPI ErectusThread::HarvesterThread([[maybe_unused]] LPVOID lpParameter)
{
	while (!threadDestructionState)
	{
		ErectusMemory::Harvester();
		std::this_thread::sleep_for(std::chrono::milliseconds(Utils::GetRangedInt(12, 36) * 16));
	}

	harvesterThreadActive = false;

	return 0xDEAF;
}

DWORD WINAPI ErectusThread::KnownRecipeThread([[maybe_unused]] LPVOID lpParameter)
{
	while (!threadDestructionState)
	{
		ErectusMemory::UpdateKnownRecipes();
		std::this_thread::sleep_for(std::chrono::milliseconds(300 * 16));
	}

	knownRecipeThreadActive = false;
	return 0xCADE;
}

bool ErectusThread::CreateProcessThreads()
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

	if (!bufferNpcListThreadActive)
	{
		bufferNpcListThreadActive = CloseHandle(CreateThread(nullptr, 0, &BufferNpcListThread, nullptr, 0, nullptr));
		if (!bufferNpcListThreadActive)
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
		harvesterThreadActive = CloseHandle(CreateThread(nullptr, 0, &HarvesterThread, nullptr, 0, nullptr));
		if (!harvesterThreadActive)
		{
			threadDestructionQueued = true;
			return false;
		}
	}

	if (!knownRecipeThreadActive)
	{
		knownRecipeThreadActive = CloseHandle(CreateThread(nullptr, 0, &KnownRecipeThread, nullptr, 0, nullptr));
		if (!knownRecipeThreadActive)
		{
			threadDestructionQueued = true;
			return false;
		}
	}

	return true;
}

bool ErectusThread::ThreadDestruction()
{
	threadDestructionState = true;

	if (bufferEntityListThreadActive)
		return false;

	if (bufferNpcListThreadActive)
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

	if (knownRecipeThreadActive)
		return false;

	threadCreationState = false;
	threadDestructionQueued = false;
	threadDestructionState = false;
	threadDestructionCounter = 0;

	return true;
}
