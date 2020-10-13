#include "WeaponEditor.h"

#include <memory>

#include "../ErectusMemory.h"
#include "../ErectusProcess.h"
#include "../settings.h"

void WeaponEditor::EditWeapons(bool enabled)
{
	if (!enabled || !Settings::weapons.IsEnabled())
	{
		ResetWeapons();
		return;
	}

	DWORD64 dataHandlerPtr;
	if (!ErectusProcess::Rpm(ErectusProcess::exe + OFFSET_DATA_HANDLER, &dataHandlerPtr, sizeof dataHandlerPtr))
		return;
	if (!Utils::Valid(dataHandlerPtr))
		return;

	ReferenceList weaponList{};
	if (!ErectusProcess::Rpm(dataHandlerPtr + 0x598, &weaponList, sizeof weaponList))
		return;
	if (!Utils::Valid(weaponList.arrayPtr) || !weaponList.arraySize || weaponList.arraySize > 0x7FFF)
		return;

	auto weaponPtrArray = std::make_unique<DWORD64[]>(weaponList.arraySize);
	if (!ErectusProcess::Rpm(weaponList.arrayPtr, weaponPtrArray.get(), weaponList.arraySize * sizeof(DWORD64)))
		return;

	for (size_t i = 0; i < weaponList.arraySize; i++)
	{
		if (!Utils::Valid(weaponPtrArray[i]))
			continue;

		WeaponInfo originalValues = {};

		originalValues.ptr = weaponPtrArray[i];
		if (!ErectusProcess::Rpm(originalValues.ptr, &originalValues.weaponData, sizeof originalValues.weaponData))
			continue;

		if (Utils::Valid(originalValues.weaponData.aimModelPtr))
		{
			if (!ErectusProcess::Rpm(originalValues.weaponData.aimModelPtr, &originalValues.aimModelData, sizeof originalValues.aimModelData))
				originalValues.aimModelData = {};
		}

		//if there's a backup it means we've already edited the weapon, let's take original values as the base for editing
		if (originalWeaponValues.contains(originalValues.weaponData.formId))
			originalValues = originalWeaponValues.at(originalValues.weaponData.formId);

		//check if we want to edit stuff, if so -> backup the original + set new
		auto newValues = originalValues;

		if (Settings::weapons.instantReload)
			newValues.weaponData.reloadSpeed = 100.f; //this is basically reload ANIMATION speed multiplier
		if (Settings::weapons.automaticflag)
			newValues.weaponData.flagB |= 1 << 7;
		if (Settings::weapons.capacityEnabled && originalValues.weaponData.capacity != 0) // capacity == 0 => melee weapon
			newValues.weaponData.capacity = static_cast<short>(Settings::weapons.capacity);
		if (Settings::weapons.speedEnabled)
			newValues.weaponData.speed = Settings::weapons.speed;
		if (Settings::weapons.reachEnabled)
			newValues.weaponData.reach = Settings::weapons.reach; //this is probably just for melee weapons/bashing?
		if (Settings::weapons.rangeEnabled) {
			newValues.weaponData.minRange = Settings::weapons.range;
			newValues.weaponData.maxRange = Settings::weapons.range;
		}

		if (Settings::weapons.noRecoil)
			std::fill(std::begin(newValues.aimModelData.recoilData), std::end(newValues.aimModelData.recoilData), 0x00);
		if (Settings::weapons.noSpread)
			std::fill(std::begin(newValues.aimModelData.spreadData), std::end(newValues.aimModelData.spreadData), 0x00);
		if (Settings::weapons.noSway)
			newValues.aimModelData.sway = 100.f;

		//backup the original value if we're going to edit sthg; if a backup already exists it will fail silently
		if (!(newValues.weaponData == originalValues.weaponData) || !(newValues.aimModelData == originalValues.aimModelData))
			originalWeaponValues.try_emplace(originalValues.weaponData.formId, originalValues);

		if (!(newValues.weaponData == originalValues.weaponData))
			ErectusProcess::Wpm(newValues.ptr, &newValues.weaponData, sizeof newValues.weaponData);

		if (!(newValues.aimModelData == originalValues.aimModelData) && Utils::Valid(newValues.weaponData.aimModelPtr))
			ErectusProcess::Wpm(newValues.weaponData.aimModelPtr, &newValues.aimModelData, sizeof newValues.aimModelData);
	}
}

void WeaponEditor::ResetWeapons()
{
	const WeaponInfo::AimModel emptyAimModel = {};
	for (const auto& [formid, originalValue] : originalWeaponValues)
	{
		ErectusProcess::Wpm(originalValue.ptr, &originalValue.weaponData, sizeof originalValue.weaponData);

		if (!(originalValue.aimModelData == emptyAimModel))
			ErectusProcess::Wpm(originalValue.weaponData.aimModelPtr, &originalValue.aimModelData, sizeof originalValue.aimModelData);
	}
	originalWeaponValues.clear();
}


bool WeaponEditor::InfiniteAmmo(const bool enabled)
{
	BYTE infiniteAmmoOn[] = { 0x66, 0xB8, 0xE7, 0x03 };
	BYTE infiniteAmmoOff[] = { 0x8B, 0x44, 0x24, 0x50 };
	BYTE infiniteAmmoCheck[sizeof infiniteAmmoOff];

	if (!ErectusProcess::Rpm(ErectusProcess::exe + OFFSET_INFINITE_AMMO, &infiniteAmmoCheck, sizeof infiniteAmmoCheck))
		return false;

	if (enabled && !memcmp(infiniteAmmoCheck, infiniteAmmoOff, sizeof infiniteAmmoOff))
		return ErectusProcess::Wpm(ErectusProcess::exe + OFFSET_INFINITE_AMMO, &infiniteAmmoOn, sizeof infiniteAmmoOn);
	if (!enabled && !memcmp(infiniteAmmoCheck, infiniteAmmoOn, sizeof infiniteAmmoOn))
		return ErectusProcess::Wpm(ErectusProcess::exe + OFFSET_INFINITE_AMMO, &infiniteAmmoOff, sizeof infiniteAmmoOff);
	return false;
}
