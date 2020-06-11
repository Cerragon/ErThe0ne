#include "ErectusInclude.h"

DWORD WINAPI ErectusThread::BufferEntityListThread(LPVOID lpParameter)
{
	UNREFERENCED_PARAMETER(lpParameter);

	while (!ThreadDestructionState)
	{
		ErectusMemory::BufferEntityListCounter++;
		if (ErectusMemory::BufferEntityListCounter > 60)
		{
			ErectusMemory::BufferEntityListCounter = 0;
			if (!ErectusMemory::BufferEntityListUpdated)
			{
				ErectusMemory::BufferEntityListUpdated = ErectusMemory::UpdateBufferEntityList();
				if (!ErectusMemory::BufferEntityListUpdated)
				{
					ErectusMemory::CustomEntityListDestructionQueued = true;
				}
			}
		}

		if (ErectusMemory::BufferEntityListDestructionQueued)
		{
			ErectusMemory::BufferEntityListDestructionQueued = false;
			ErectusMemory::DeleteBufferEntityList();
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

	ErectusMemory::DeleteBufferEntityList();

	BufferEntityListThreadActive = false;

	return 0xBEEF;
}

DWORD WINAPI ErectusThread::BufferNPCListThread(LPVOID lpParameter)
{
	UNREFERENCED_PARAMETER(lpParameter);

	while (!ThreadDestructionState)
	{
		ErectusMemory::BufferNPCListCounter++;
		if (ErectusMemory::BufferNPCListCounter > 60)
		{
			ErectusMemory::BufferNPCListCounter = 0;
			if (!ErectusMemory::BufferNPCListUpdated)
			{
				ErectusMemory::BufferNPCListUpdated = ErectusMemory::UpdateBufferNPCList();
				if (!ErectusMemory::BufferNPCListUpdated)
				{
					ErectusMemory::CustomNPCListDestructionQueued = true;
				}
			}
		}

		if (ErectusMemory::BufferNPCListDestructionQueued)
		{
			ErectusMemory::BufferNPCListDestructionQueued = false;
			ErectusMemory::DeleteBufferNPCList();
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

	ErectusMemory::DeleteBufferNPCList();

	BufferNPCListThreadActive = false;

	return 0xFADE;
}

DWORD WINAPI ErectusThread::BufferPlayerListThread(LPVOID lpParameter)
{
	UNREFERENCED_PARAMETER(lpParameter);

	while (!ErectusThread::ThreadDestructionState)
	{
		ErectusMemory::BufferPlayerListCounter++;
		if (ErectusMemory::BufferPlayerListCounter > 60)
		{
			ErectusMemory::BufferPlayerListCounter = 0;
			if (!ErectusMemory::BufferPlayerListUpdated)
			{
				ErectusMemory::BufferPlayerListUpdated = ErectusMemory::UpdateBufferPlayerList();
				if (!ErectusMemory::BufferPlayerListUpdated)
				{
					ErectusMemory::CustomPlayerListDestructionQueued = true;
				}
			}
		}

		if (ErectusMemory::BufferPlayerListDestructionQueued)
		{
			ErectusMemory::BufferPlayerListDestructionQueued = false;
			ErectusMemory::DeleteBufferPlayerList();
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

	ErectusMemory::DeleteBufferPlayerList();

	BufferPlayerListThreadActive = false;

	return 0xFEED;
}

DWORD WINAPI ErectusThread::WeaponEditorThread(LPVOID lpParameter)
{
	UNREFERENCED_PARAMETER(lpParameter);

	while (!ErectusThread::ThreadDestructionState)
	{
		ErectusMemory::OldWeaponListCounter++;
		if (ErectusMemory::OldWeaponListCounter > 60)
		{
			ErectusMemory::OldWeaponListCounter = 0;
			if (!ErectusMemory::OldWeaponListUpdated)
			{
				ErectusMemory::OldWeaponListUpdated = ErectusMemory::UpdateOldWeaponData();
			}

			if (ErectusMemory::OldWeaponListUpdated)
			{
				bool RevertWeapons = true;
				if (ErectusMemory::WeaponEditingEnabled())
				{
					RevertWeapons = false;
				}

				for (int i = 0; i < ErectusMemory::OldWeaponListSize; i++)
				{
					if (ErectusThread::ThreadDestructionState) break;
					else ErectusMemory::EditWeapon(i, RevertWeapons);
				}
			}

			ErectusMemory::InfiniteAmmo(ErectusIni::CustomWeaponSettings.InfiniteAmmo);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

	if (ErectusMemory::OldWeaponListUpdated)
	{
		for (int i = 0; i < ErectusMemory::OldWeaponListSize; i++)
		{
			ErectusMemory::EditWeapon(i, true);
		}
	}

	ErectusMemory::InfiniteAmmo(false);

	ErectusMemory::DeleteOldWeaponList();

	WeaponEditorThreadActive = false;

	return 0xDEAD;
}

DWORD WINAPI ErectusThread::LockingThread(LPVOID lpParameter)
{
	UNREFERENCED_PARAMETER(lpParameter);

	srand(unsigned int(time(NULL)));

	int FavoritedWeaponCounter = 0;

	DWORD WeaponId = 0;
	BYTE ShotsHit = 1;
	BYTE ShotsFired = 0;

	DWORD64 TargetingPage = 0;
	bool TargetingPageValid = false;

	int SendDamageCounter = 0;
	int SendDamageThreshold = 0;

	int MeleeCounter = 0;
	int MeleeThreshold = 0;

	while (!ErectusThread::ThreadDestructionState)
	{
		FavoritedWeaponCounter++;
		if (FavoritedWeaponCounter > 60)
		{
			FavoritedWeaponCounter = 0;
			if (ErectusIni::CustomTargetSettings.FavoriteIndex < 12)
			{
				WeaponId = ErectusMemory::GetFavoritedWeaponId(BYTE(ErectusIni::CustomTargetSettings.FavoriteIndex));
			}
			else
			{
				WeaponId = 0;
			}
		}

		if (ErectusMain::OverlayForeground && GetAsyncKeyState('T'))
		{
			if (!ErectusMemory::TargetLockingKeyPressed)
			{
				ErectusMemory::TargetLockingKeyPressed = true;
			}

			if (ErectusMemory::TargetLockingCooldown > 0)
			{
				ErectusMemory::TargetLockingCooldown--;
			}
		}
		else
		{
			if (ErectusMemory::TargetLockingKeyPressed)
			{
				ErectusMemory::TargetLockingKeyPressed = false;
			}

			if (ErectusMemory::TargetLockingCooldown)
			{
				ErectusMemory::TargetLockingCooldown = 0;
			}

			if (ErectusMemory::TargetLockingPtr)
			{
				ErectusMemory::TargetLockingPtr = 0;
			}
		}

		if (ErectusMemory::TargetLockingPtr)
		{
			ErectusMemory::DamageRedirection(&TargetingPage, &TargetingPageValid, false, true);

			SendDamageCounter++;
			if (SendDamageCounter > SendDamageThreshold)
			{
				SendDamageCounter = 0;
				SendDamageThreshold = Erectus::GetRangedInt(ErectusIni::CustomTargetSettings.SendDamageMin, ErectusIni::CustomTargetSettings.SendDamageMax);
				ErectusMemory::SendDamage(WeaponId, &ShotsHit, &ShotsFired, 1);
			}
		}
		else
		{
			ErectusMemory::DamageRedirection(&TargetingPage, &TargetingPageValid, false, false);
			SendDamageThreshold = 0;
		}

		if (ErectusIni::CustomMeleeSettings.MeleeEnabled)
		{
			if (ErectusMain::OverlayForeground && GetAsyncKeyState('U'))
			{
				MeleeCounter++;
				if (MeleeCounter > MeleeThreshold)
				{
					MeleeCounter = 0;
					MeleeThreshold = Erectus::GetRangedInt(ErectusIni::CustomMeleeSettings.MeleeSpeedMin, ErectusIni::CustomMeleeSettings.MeleeSpeedMax);
					ErectusMemory::MeleeAttack();
				}
			}
			else
			{
				MeleeThreshold = 0;
			}
		}
		
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

	ErectusMemory::DamageRedirection(&TargetingPage, &TargetingPageValid, true, false);

	if (TargetingPage)
	{
		Erectus::FreeEx(TargetingPage);
	}

	LockingThreadActive = false;

	return 0xCAFE;
}

DWORD WINAPI ErectusThread::MultihackThread(LPVOID lpParameter)
{
	UNREFERENCED_PARAMETER(lpParameter);

	bool PositionSpoofingPress = false;
	bool NoclipPress = false;

	int ClientStateCounter = 0;

	DWORD64 ActorValuePage = 0;
	bool ActorValuePageValid = false;

	int ActorValueDefaultCounter = 0;

	bool OpkPlayersPress = false;
	bool OpkPlayersToggle = false;

	bool OpkNpcsPress = false;
	bool OpkNpcsToggle = false;

	DWORD64 OpkPage = 0;
	bool OpkPageValid = false;

	DWORD64 FreezeApPage = 0;
	bool FreezeApPageValid = false;

	bool LootScrapKeyPress = false;
	bool LootItemsKeyPress = false;

	int LootScrapCounter = 0;
	int LootScrapThreshold = 0;

	int LootItemsCounter = 0;
	int LootItemsThreshold = 0;

	int NukeCodeCounter = 0;
	
	int ChargenCounter = 0;

	while (!ErectusThread::ThreadDestructionState)
	{
		if (Erectus::DoubleKeyPress(VK_CONTROL, 'L', &PositionSpoofingPress))
		{
			if (ErectusThread::PositionSpoofingToggle)
			{
				ErectusThread::PositionSpoofingToggle = false;
			}
			else
			{
				ErectusThread::PositionSpoofingToggle = true;
				if (ErectusIni::CustomLocalPlayerSettings.PositionSpoofingEnabled)
				{
					if (ErectusIni::CustomLocalPlayerSettings.ClientState)
					{
						ErectusMemory::SetClientState(2);
					}
				}
			}
		}

		if (!ErectusIni::CustomLocalPlayerSettings.PositionSpoofingEnabled)
		{
			ErectusThread::PositionSpoofingToggle = false;
		}

		if (Erectus::DoubleKeyPress(VK_CONTROL, 'Y', &NoclipPress))
		{
			if (ErectusThread::NoclipToggle)
			{
				ErectusThread::NoclipToggle = false;
			}
			else
			{
				ErectusThread::NoclipToggle = true;
				if (ErectusIni::CustomLocalPlayerSettings.NoclipEnabled)
				{
					if (ErectusIni::CustomLocalPlayerSettings.ClientState)
					{
						ErectusMemory::SetClientState(2);
					}
				}
			}
		}

		if (!ErectusIni::CustomLocalPlayerSettings.NoclipEnabled)
		{
			ErectusThread::NoclipToggle = false;
		}

		if (Erectus::DoubleKeyPress(VK_CONTROL, 'B', &OpkPlayersPress))
		{
			if (OpkPlayersToggle)
			{
				OpkPlayersToggle = false;
			}
			else
			{
				OpkPlayersToggle = true;
			}
		}

		if (Erectus::DoubleKeyPress(VK_CONTROL, 'N', &OpkNpcsPress))
		{
			if (OpkNpcsToggle)
			{
				OpkNpcsToggle = false;
			}
			else
			{
				OpkNpcsToggle = true;
			}
		}

		if (Erectus::DoubleKeyPress(VK_CONTROL, 'E', &LootScrapKeyPress))
		{
			if (ErectusIni::CustomScrapLooterSettings.ScrapKeybindEnabled)
			{
				if (ErectusMemory::CheckScrapList())
				{
					ErectusMemory::LootScrap();
				}
			}
		}

		if (Erectus::DoubleKeyPress(VK_CONTROL, 'R', &LootItemsKeyPress))
		{
			if (ErectusIni::CustomItemLooterSettings.ItemKeybindEnabled)
			{
				if (ErectusMemory::CheckItemLooterSettings())
				{
					ErectusMemory::LootItems();
				}
			}
		}

		if (ErectusThread::PositionSpoofingToggle || ErectusThread::NoclipToggle)
		{
			if (ErectusIni::CustomLocalPlayerSettings.AutomaticClientState)
			{
				ClientStateCounter++;
				if (ClientStateCounter > 60)
				{
					ClientStateCounter = 0;
					ErectusMemory::SetClientState(2);
				}
			}
		}

		ErectusMemory::PositionSpoofing(ErectusThread::PositionSpoofingToggle);

		ErectusMemory::Noclip(ErectusThread::NoclipToggle);

		ErectusMemory::ActorValue(&ActorValuePage, &ActorValuePageValid, true);

		ActorValueDefaultCounter++;
		if (ActorValueDefaultCounter > 60)
		{
			ActorValueDefaultCounter = 0;
			ErectusMemory::SetActorValueMaximum(0x000002C2, 100.0f, float(ErectusIni::CustomLocalPlayerSettings.Strength), ErectusIni::CustomLocalPlayerSettings.StrengthEnabled);
			ErectusMemory::SetActorValueMaximum(0x000002C3, 100.0f, float(ErectusIni::CustomLocalPlayerSettings.Perception), ErectusIni::CustomLocalPlayerSettings.PerceptionEnabled);
			ErectusMemory::SetActorValueMaximum(0x000002C4, 100.0f, float(ErectusIni::CustomLocalPlayerSettings.Endurance), ErectusIni::CustomLocalPlayerSettings.EnduranceEnabled);
			ErectusMemory::SetActorValueMaximum(0x000002C5, 100.0f, float(ErectusIni::CustomLocalPlayerSettings.Charisma), ErectusIni::CustomLocalPlayerSettings.CharismaEnabled);
			ErectusMemory::SetActorValueMaximum(0x000002C6, 100.0f, float(ErectusIni::CustomLocalPlayerSettings.Agility), ErectusIni::CustomLocalPlayerSettings.AgilityEnabled);
			ErectusMemory::SetActorValueMaximum(0x000002C7, 100.0f, float(ErectusIni::CustomLocalPlayerSettings.Intelligence), ErectusIni::CustomLocalPlayerSettings.IntelligenceEnabled);
			ErectusMemory::SetActorValueMaximum(0x000002C8, 100.0f, float(ErectusIni::CustomLocalPlayerSettings.Luck), ErectusIni::CustomLocalPlayerSettings.LuckEnabled);
		}
		
		ErectusMemory::FreezeActionPoints(&FreezeApPage, &FreezeApPageValid, true);
		
		ErectusMemory::OnePositionKill(&OpkPage, &OpkPageValid, true);

		if (OpkPageValid)
		{
			if (OpkPlayersToggle)
			{
				if (!ErectusIni::CustomOpkSettings.PlayersEnabled)
				{
					OpkPlayersToggle = false;
				}
				else
				{
					ErectusMemory::SetOpkData(OpkPage, true, true);
				}
			}
			else
			{
				ErectusMemory::SetOpkData(OpkPage, true, false);
			}

			if (OpkNpcsToggle)
			{
				if (!ErectusIni::CustomOpkSettings.NpcsEnabled)
				{
					OpkNpcsToggle = false;
				}
				else
				{
					ErectusMemory::SetOpkData(OpkPage, false, true);
				}
			}
			else
			{
				ErectusMemory::SetOpkData(OpkPage, false, false);
			}
		}

		if (ErectusIni::CustomNukeCodeSettings.AutomaticNukeCodes)
		{
			NukeCodeCounter++;
			if (NukeCodeCounter > 300)
			{
				NukeCodeCounter = 0;
				ErectusMemory::GetNukeCode(0x000921AE, ErectusImGui::AlphaCode);
				ErectusMemory::GetNukeCode(0x00092213, ErectusImGui::BravoCode);
				ErectusMemory::GetNukeCode(0x00092214, ErectusImGui::CharlieCode);
			}
		}

		if (ErectusIni::CustomScrapLooterSettings.ScrapAutomaticLootingEnabled)
		{
			LootScrapCounter++;
			if (LootScrapCounter > LootScrapThreshold)
			{
				LootScrapCounter = 0;
				LootScrapThreshold = Erectus::GetRangedInt(ErectusIni::CustomScrapLooterSettings.ScrapAutomaticSpeedMin, ErectusIni::CustomScrapLooterSettings.ScrapAutomaticSpeedMax);
				if (ErectusMemory::CheckScrapList())
				{
					ErectusMemory::LootScrap();
				}
			}
		}
		else
		{
			LootScrapThreshold = 0;
		}

		if (ErectusIni::CustomItemLooterSettings.ItemAutomaticLootingEnabled)
		{
			LootItemsCounter++;
			if (LootItemsCounter > LootItemsThreshold)
			{
				LootItemsCounter = 0;
				LootItemsThreshold = Erectus::GetRangedInt(ErectusIni::CustomItemLooterSettings.ItemAutomaticSpeedMin, ErectusIni::CustomItemLooterSettings.ItemAutomaticSpeedMax);
				if (ErectusMemory::CheckItemLooterSettings())
				{
					ErectusMemory::LootItems();
				}
			}
		}
		else
		{
			LootItemsThreshold = 0;
		}
		
		if (ErectusIni::CustomChargenSettings.ChargenEditingEnabled)
		{
			ChargenCounter++;
			if (ChargenCounter > 10)
			{
				ChargenCounter = 0;
				ErectusMemory::ChargenEditing();
			}
		}
		
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

	ErectusMemory::PositionSpoofing(false);

	ErectusMemory::Noclip(false);

	ErectusMemory::ActorValue(&ActorValuePage, &ActorValuePageValid, false);

	if (ActorValuePage)
	{
		Erectus::FreeEx(ActorValuePage);
	}

	ErectusMemory::SetActorValueMaximum(0x000002C2, 100.0f, float(ErectusIni::CustomLocalPlayerSettings.Strength), false);
	ErectusMemory::SetActorValueMaximum(0x000002C3, 100.0f, float(ErectusIni::CustomLocalPlayerSettings.Perception), false);
	ErectusMemory::SetActorValueMaximum(0x000002C4, 100.0f, float(ErectusIni::CustomLocalPlayerSettings.Endurance), false);
	ErectusMemory::SetActorValueMaximum(0x000002C5, 100.0f, float(ErectusIni::CustomLocalPlayerSettings.Charisma), false);
	ErectusMemory::SetActorValueMaximum(0x000002C6, 100.0f, float(ErectusIni::CustomLocalPlayerSettings.Agility), false);
	ErectusMemory::SetActorValueMaximum(0x000002C7, 100.0f, float(ErectusIni::CustomLocalPlayerSettings.Intelligence), false);
	ErectusMemory::SetActorValueMaximum(0x000002C8, 100.0f, float(ErectusIni::CustomLocalPlayerSettings.Luck), false);

	ErectusMemory::OnePositionKill(&OpkPage, &OpkPageValid, false);

	if (OpkPage)
	{
		Erectus::FreeEx(OpkPage);
	}

	ErectusMemory::FreezeActionPoints(&FreezeApPage, &FreezeApPageValid, false);

	if (FreezeApPage)
	{
		Erectus::FreeEx(FreezeApPage);
	}

	MultihackThreadActive = false;

	return 0xBEAD;
}

DWORD WINAPI ErectusThread::HarvesterThread(LPVOID lpParameter)
{
	UNREFERENCED_PARAMETER(lpParameter);

	bool NPCKeyPress = false;
	bool ContainerKeyPress = false;
	bool FloraKeyPress = false;

	int HarvesterCounter = 0;
	int HarvesterThreshold = 0;

	while (!ErectusThread::ThreadDestructionState)
	{
		if (Erectus::DoubleKeyPress(VK_CONTROL, VK_OEM_COMMA, &NPCKeyPress))
		{
			if (ErectusIni::NPCLooterSettings.EntityLooterEnabled)
			{
				ErectusIni::NPCLooterSettings.EntityLooterEnabled = false;
			}
			else
			{
				ErectusIni::NPCLooterSettings.EntityLooterEnabled = true;
			}
		}

		if (Erectus::DoubleKeyPress(VK_CONTROL, VK_OEM_PERIOD, &ContainerKeyPress))
		{
			if (ErectusIni::ContainerLooterSettings.EntityLooterEnabled)
			{
				ErectusIni::ContainerLooterSettings.EntityLooterEnabled = false;
			}
			else
			{
				ErectusIni::ContainerLooterSettings.EntityLooterEnabled = true;
			}
		}

		if (Erectus::DoubleKeyPress(VK_CONTROL, 'P', &FloraKeyPress))
		{
			if (ErectusIni::CustomHarvesterSettings.HarvesterEnabled)
			{
				ErectusIni::CustomHarvesterSettings.HarvesterEnabled = false;
			}
			else
			{
				ErectusIni::CustomHarvesterSettings.HarvesterEnabled = true;
			}
		}

		HarvesterCounter++;
		if (HarvesterCounter > HarvesterThreshold)
		{
			HarvesterCounter = 0;
			HarvesterThreshold = Erectus::GetRangedInt(12, 36);
			ErectusMemory::Harvester();
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

	HarvesterThreadActive = false;

	return 0xDEAF;
}

DWORD WINAPI ErectusThread::KnownRecipeThread(LPVOID lpParameter)
{
	UNREFERENCED_PARAMETER(lpParameter);

	int KnownRecipeCounter = 0;
	bool KnownRecipeListUpdated = false;

	while (!ErectusThread::ThreadDestructionState)
	{
		if (KnownRecipeListUpdated)
		{
			KnownRecipeCounter++;
			if (KnownRecipeCounter > 300)
			{
				KnownRecipeCounter = 0;
				KnownRecipeListUpdated = ErectusMemory::UpdateKnownRecipes();
			}
		}
		else
		{
			KnownRecipeCounter++;
			if (KnownRecipeCounter > 60)
			{
				KnownRecipeCounter = 0;
				KnownRecipeListUpdated = ErectusMemory::UpdateKnownRecipes();
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

	KnownRecipeThreadActive = false;

	return 0xCADE;
}

bool ErectusThread::CreateProcessThreads()
{
	if (ThreadDestructionQueued)
	{
		return false;
	}

	if (ThreadDestructionState)
	{
		return false;
	}

	if (ThreadDestructionCounter)
	{
		return false;
	}

	if (!BufferEntityListThreadActive)
	{
		BufferEntityListThreadActive = CloseHandle(CreateThread(NULL, 0, &BufferEntityListThread, NULL, 0, NULL));
		if (!BufferEntityListThreadActive)
		{
			ThreadDestructionQueued = true;
			return false;
		}
	}

	if (!BufferNPCListThreadActive)
	{
		BufferNPCListThreadActive = CloseHandle(CreateThread(NULL, 0, &BufferNPCListThread, NULL, 0, NULL));
		if (!BufferNPCListThreadActive)
		{
			ThreadDestructionQueued = true;
			return false;
		}
	}

	if (!BufferPlayerListThreadActive)
	{
		BufferPlayerListThreadActive = CloseHandle(CreateThread(NULL, 0, &BufferPlayerListThread, NULL, 0, NULL));
		if (!BufferPlayerListThreadActive)
		{
			ThreadDestructionQueued = true;
			return false;
		}
	}

	if (!WeaponEditorThreadActive)
	{
		WeaponEditorThreadActive = CloseHandle(CreateThread(NULL, 0, &WeaponEditorThread, NULL, 0, NULL));
		if (!WeaponEditorThreadActive)
		{
			ThreadDestructionQueued = true;
			return false;
		}
	}

	if (!LockingThreadActive)
	{
		LockingThreadActive = CloseHandle(CreateThread(NULL, 0, &LockingThread, NULL, 0, NULL));
		if (!LockingThreadActive)
		{
			ThreadDestructionQueued = true;
			return false;
		}
	}

	if (!MultihackThreadActive)
	{
		MultihackThreadActive = CloseHandle(CreateThread(NULL, 0, &MultihackThread, NULL, 0, NULL));
		if (!MultihackThreadActive)
		{
			ThreadDestructionQueued = true;
			return false;
		}
	}

	if (!HarvesterThreadActive)
	{
		HarvesterThreadActive = CloseHandle(CreateThread(NULL, 0, &HarvesterThread, NULL, 0, NULL));
		if (!HarvesterThreadActive)
		{
			ThreadDestructionQueued = true;
			return false;
		}
	}

	if (!KnownRecipeThreadActive)
	{
		KnownRecipeThreadActive = CloseHandle(CreateThread(NULL, 0, &KnownRecipeThread, NULL, 0, NULL));
		if (!KnownRecipeThreadActive)
		{
			ThreadDestructionQueued = true;
			return false;
		}
	}

	return true;
}

bool ErectusThread::ThreadDestruction()
{
	ThreadDestructionState = true;

	if (BufferEntityListThreadActive)
	{
		return false;
	}

	if (BufferNPCListThreadActive)
	{
		return false;
	}

	if (BufferPlayerListThreadActive)
	{
		return false;
	}

	if (WeaponEditorThreadActive)
	{
		return false;
	}

	if (LockingThreadActive)
	{
		return false;
	}

	if (MultihackThreadActive)
	{
		return false;
	}

	if (HarvesterThreadActive)
	{
		return false;
	}

	if (KnownRecipeThreadActive)
	{
		return false;
	}

	ThreadCreationState = false;
	ThreadDestructionQueued = false;
	ThreadDestructionState = false;
	ThreadDestructionCounter = 0;

	return true;
}
