#include "ErectusInclude.h"

DWORD WINAPI ErectusThread::BufferEntityListThread(LPVOID lpParameter)
{
	UNREFERENCED_PARAMETER(lpParameter);

	while (!threadDestructionState)
	{
		ErectusMemory::bufferEntityListCounter++;
		if (ErectusMemory::bufferEntityListCounter > 60)
		{
			ErectusMemory::bufferEntityListCounter = 0;
			if (!ErectusMemory::bufferEntityListUpdated)
			{
				ErectusMemory::bufferEntityListUpdated = ErectusMemory::UpdateBufferEntityList();
				if (!ErectusMemory::bufferEntityListUpdated)
				{
					ErectusMemory::customEntityListDestructionQueued = true;
				}
			}
		}

		if (ErectusMemory::bufferEntityListDestructionQueued)
		{
			ErectusMemory::bufferEntityListDestructionQueued = false;
			ErectusMemory::DeleteBufferEntityList();
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

	ErectusMemory::DeleteBufferEntityList();

	bufferEntityListThreadActive = false;

	return 0xBEEF;
}

DWORD WINAPI ErectusThread::BufferNpcListThread(LPVOID lpParameter)
{
	UNREFERENCED_PARAMETER(lpParameter);

	while (!threadDestructionState)
	{
		ErectusMemory::bufferNpcListCounter++;
		if (ErectusMemory::bufferNpcListCounter > 60)
		{
			ErectusMemory::bufferNpcListCounter = 0;
			if (!ErectusMemory::bufferNpcListUpdated)
			{
				ErectusMemory::bufferNpcListUpdated = ErectusMemory::UpdateBufferNpcList();
				if (!ErectusMemory::bufferNpcListUpdated)
				{
					ErectusMemory::customNpcListDestructionQueued = true;
				}
			}
		}

		if (ErectusMemory::bufferNpcListDestructionQueued)
		{
			ErectusMemory::bufferNpcListDestructionQueued = false;
			ErectusMemory::DeleteBufferNpcList();
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

	ErectusMemory::DeleteBufferNpcList();

	bufferNpcListThreadActive = false;

	return 0xFADE;
}

DWORD WINAPI ErectusThread::BufferPlayerListThread([[maybe_unused]] LPVOID lpParameter)
{
	while (!threadDestructionState)
	{
		ErectusMemory::bufferPlayerListCounter++;
		if (ErectusMemory::bufferPlayerListCounter > 60)
		{
			ErectusMemory::bufferPlayerListCounter = 0;
			if (!ErectusMemory::bufferPlayerListUpdated)
			{
				ErectusMemory::bufferPlayerListUpdated = ErectusMemory::UpdateBufferPlayerList();
				if (!ErectusMemory::bufferPlayerListUpdated)
				{
					ErectusMemory::customPlayerListDestructionQueued = true;
				}
			}
		}

		if (ErectusMemory::bufferPlayerListDestructionQueued)
		{
			ErectusMemory::bufferPlayerListDestructionQueued = false;
			ErectusMemory::DeleteBufferPlayerList();
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

	ErectusMemory::DeleteBufferPlayerList();

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
			{
				ErectusMemory::oldWeaponListUpdated = ErectusMemory::UpdateOldWeaponData();
			}

			if (ErectusMemory::oldWeaponListUpdated)
			{
				auto revertWeapons = true;
				if (ErectusMemory::WeaponEditingEnabled())
				{
					revertWeapons = false;
				}

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
			if (ErectusIni::customTargetSettings.favoriteIndex < 12)
			{
				weaponId = ErectusMemory::GetFavoritedWeaponId(BYTE(ErectusIni::customTargetSettings.favoriteIndex));
			}
			else
			{
				weaponId = 0;
			}
		}

		if (ErectusMain::overlayForeground && GetAsyncKeyState('T'))
		{
			if (!ErectusMemory::targetLockingKeyPressed)
			{
				ErectusMemory::targetLockingKeyPressed = true;
			}

			if (ErectusMemory::targetLockingCooldown > 0)
			{
				ErectusMemory::targetLockingCooldown--;
			}
		}
		else
		{
			if (ErectusMemory::targetLockingKeyPressed)
			{
				ErectusMemory::targetLockingKeyPressed = false;
			}

			if (ErectusMemory::targetLockingCooldown)
			{
				ErectusMemory::targetLockingCooldown = 0;
			}

			if (ErectusMemory::targetLockingPtr)
			{
				ErectusMemory::targetLockingPtr = 0;
			}
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

		if (ErectusIni::customMeleeSettings.meleeEnabled)
		{
			if (ErectusMain::overlayForeground && GetAsyncKeyState('U'))
			{
				meleeCounter++;
				if (meleeCounter > meleeThreshold)
				{
					meleeCounter = 0;
					meleeThreshold = Utils::GetRangedInt(ErectusIni::customMeleeSettings.meleeSpeedMin, ErectusIni::customMeleeSettings.meleeSpeedMax);
					ErectusMemory::MeleeAttack();
				}
			}
			else
			{
				meleeThreshold = 0;
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

	ErectusMemory::DamageRedirection(&targetingPage, &targetingPageValid, true, false);

	if (targetingPage)
	{
		Utils::FreeEx(targetingPage);
	}

	lockingThreadActive = false;

	return 0xCAFE;
}

DWORD WINAPI ErectusThread::MultihackThread([[maybe_unused]] LPVOID lpParameter)
{
	auto positionSpoofingPress = false;
	auto noclipPress = false;

	auto clientStateCounter = 0;

	DWORD64 actorValuePage = 0;
	auto actorValuePageValid = false;

	auto actorValueDefaultCounter = 0;

	auto opkPlayersPress = false;
	auto opkPlayersToggle = false;

	auto opkNpcsPress = false;
	auto opkNpcsToggle = false;

	DWORD64 opkPage = 0;
	auto opkPageValid = false;

	DWORD64 freezeApPage = 0;
	auto freezeApPageValid = false;

	auto lootScrapKeyPress = false;
	auto lootItemsKeyPress = false;

	auto lootScrapCounter = 0;
	auto lootScrapThreshold = 0;

	auto lootItemsCounter = 0;
	auto lootItemsThreshold = 0;

	auto nukeCodeCounter = 0;

	auto chargenCounter = 0;

	while (!threadDestructionState)
	{
		if (Utils::DoubleKeyPress(VK_CONTROL, 'L', &positionSpoofingPress))
		{
			if (positionSpoofingToggle)
			{
				positionSpoofingToggle = false;
			}
			else
			{
				positionSpoofingToggle = true;
				if (ErectusIni::customLocalPlayerSettings.positionSpoofingEnabled && ErectusIni::customLocalPlayerSettings.clientState)
				{
					ErectusMemory::SetClientState(2);
				}
			}
		}

		if (!ErectusIni::customLocalPlayerSettings.positionSpoofingEnabled)
		{
			positionSpoofingToggle = false;
		}

		if (Utils::DoubleKeyPress(VK_CONTROL, 'Y', &noclipPress))
		{
			if (noclipToggle)
			{
				noclipToggle = false;
			}
			else
			{
				noclipToggle = true;
				if (ErectusIni::customLocalPlayerSettings.noclipEnabled && ErectusIni::customLocalPlayerSettings.clientState)
				{
					ErectusMemory::SetClientState(2);
				}

			}
		}

		if (!ErectusIni::customLocalPlayerSettings.noclipEnabled)
			noclipToggle = false;

		if (Utils::DoubleKeyPress(VK_CONTROL, 'B', &opkPlayersPress))
			opkPlayersToggle = !opkPlayersToggle;

		if (Utils::DoubleKeyPress(VK_CONTROL, 'N', &opkNpcsPress))
			opkNpcsToggle = !opkNpcsToggle;

		if (Utils::DoubleKeyPress(VK_CONTROL, 'E', &lootScrapKeyPress))
		{
			if (ErectusIni::customScrapLooterSettings.scrapKeybindEnabled && ErectusMemory::CheckScrapList())
				ErectusMemory::LootScrap();
		}

		if (Utils::DoubleKeyPress(VK_CONTROL, 'R', &lootItemsKeyPress))
		{
			if (ErectusIni::customItemLooterSettings.itemKeybindEnabled && ErectusMemory::CheckItemLooterSettings())
				ErectusMemory::LootItems();
		}

		if (positionSpoofingToggle || noclipToggle)
		{
			if (ErectusIni::customLocalPlayerSettings.automaticClientState)
			{
				clientStateCounter++;
				if (clientStateCounter > 60)
				{
					clientStateCounter = 0;
					ErectusMemory::SetClientState(2);
				}
			}
		}

		ErectusMemory::PositionSpoofing(positionSpoofingToggle);
		ErectusMemory::Noclip(noclipToggle);

		ErectusMemory::ActorValue(&actorValuePage, &actorValuePageValid, true);

		actorValueDefaultCounter++;
		if (actorValueDefaultCounter > 60)
		{
			actorValueDefaultCounter = 0;
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
			{
				if (!ErectusIni::customOpkSettings.playersEnabled)
				{
					opkPlayersToggle = false;
				}
				else
				{
					ErectusMemory::SetOpkData(opkPage, true, true);
				}
			}
			else
			{
				ErectusMemory::SetOpkData(opkPage, true, false);
			}

			if (opkNpcsToggle)
			{
				if (!ErectusIni::customOpkSettings.npcsEnabled)
				{
					opkNpcsToggle = false;
				}
				else
				{
					ErectusMemory::SetOpkData(opkPage, false, true);
				}
			}
			else
			{
				ErectusMemory::SetOpkData(opkPage, false, false);
			}
		}

		if (ErectusIni::customNukeCodeSettings.automaticNukeCodes)
		{
			nukeCodeCounter++;
			if (nukeCodeCounter > 300)
			{
				nukeCodeCounter = 0;
				ErectusMemory::GetNukeCode(0x000921AE, ErectusImGui::alphaCode);
				ErectusMemory::GetNukeCode(0x00092213, ErectusImGui::bravoCode);
				ErectusMemory::GetNukeCode(0x00092214, ErectusImGui::charlieCode);
			}
		}

		if (ErectusIni::customScrapLooterSettings.scrapAutomaticLootingEnabled)
		{
			lootScrapCounter++;
			if (lootScrapCounter > lootScrapThreshold)
			{
				lootScrapCounter = 0;
				lootScrapThreshold = Utils::GetRangedInt(ErectusIni::customScrapLooterSettings.scrapAutomaticSpeedMin, ErectusIni::customScrapLooterSettings.scrapAutomaticSpeedMax);
				if (ErectusMemory::CheckScrapList())
				{
					ErectusMemory::LootScrap();
				}
			}
		}
		else
		{
			lootScrapThreshold = 0;
		}

		if (ErectusIni::customItemLooterSettings.itemAutomaticLootingEnabled)
		{
			lootItemsCounter++;
			if (lootItemsCounter > lootItemsThreshold)
			{
				lootItemsCounter = 0;
				lootItemsThreshold = Utils::GetRangedInt(ErectusIni::customItemLooterSettings.itemAutomaticSpeedMin, ErectusIni::customItemLooterSettings.itemAutomaticSpeedMax);
				if (ErectusMemory::CheckItemLooterSettings())
				{
					ErectusMemory::LootItems();
				}
			}
		}
		else
		{
			lootItemsThreshold = 0;
		}

		if (ErectusIni::customChargenSettings.chargenEditingEnabled)
		{
			chargenCounter++;
			if (chargenCounter > 10)
			{
				chargenCounter = 0;
				ErectusMemory::ChargenEditing();
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

	ErectusMemory::PositionSpoofing(false);
	ErectusMemory::Noclip(false);
	ErectusMemory::ActorValue(&actorValuePage, &actorValuePageValid, false);

	if (actorValuePage)
	{
		Utils::FreeEx(actorValuePage);
	}

	ErectusMemory::SetActorValueMaximum(0x000002C2, 100.0f, static_cast<float>(ErectusIni::customLocalPlayerSettings.strength), false);
	ErectusMemory::SetActorValueMaximum(0x000002C3, 100.0f, static_cast<float>(ErectusIni::customLocalPlayerSettings.perception), false);
	ErectusMemory::SetActorValueMaximum(0x000002C4, 100.0f, static_cast<float>(ErectusIni::customLocalPlayerSettings.endurance), false);
	ErectusMemory::SetActorValueMaximum(0x000002C5, 100.0f, static_cast<float>(ErectusIni::customLocalPlayerSettings.charisma), false);
	ErectusMemory::SetActorValueMaximum(0x000002C6, 100.0f, static_cast<float>(ErectusIni::customLocalPlayerSettings.agility), false);
	ErectusMemory::SetActorValueMaximum(0x000002C7, 100.0f, static_cast<float>(ErectusIni::customLocalPlayerSettings.intelligence), false);
	ErectusMemory::SetActorValueMaximum(0x000002C8, 100.0f, static_cast<float>(ErectusIni::customLocalPlayerSettings.luck), false);

	ErectusMemory::OnePositionKill(&opkPage, &opkPageValid, false);

	if (opkPage)
	{
		Utils::FreeEx(opkPage);
	}

	ErectusMemory::FreezeActionPoints(&freezeApPage, &freezeApPageValid, false);

	if (freezeApPage)
	{
		Utils::FreeEx(freezeApPage);
	}

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
