#include "settings.h"
#include "utils.h"

#include "ErectusMemory.h"

#define MINI_CASE_SENSITIVE
#include "dependencies/fmt/fmt/format.h"
#include "dependencies/mIni/ini.h"

mINI::INIFile file("Erectus.ini");
mINI::INIStructure ini;

void Settings::GetDword(const std::string& section, const std::string& key, std::uint32_t& value, const std::uint32_t deflt)
{
	if (!ini.has(section))
		ini[section];

	if (!ini[section].has(key))
	{
		ini[section][key] = format(FMT_STRING("{:08X}"), deflt);
	}

	try
	{
		value = stoul(ini[section][key], nullptr, 16);
	}
	catch (...)
	{
		value = deflt;
	}
}
void Settings::SetDword(const std::string& section, const std::string& key, const std::uint32_t value, const std::uint32_t deflt)
{
	if (!ini.has(section))
		ini[section];

	if (!ini[section].has(key))
	{
		ini[section][key] = format(FMT_STRING("{:08x}"), deflt);
	}
	ini[section][key] = format(FMT_STRING("{:08x}"), value);
}

void Settings::GetInt(const std::string& section, const std::string& key, int& value, const int deflt)
{
	if (!ini.has(section))
		ini[section];

	if (!ini[section].has(key))
		ini[section][key] = std::to_string(deflt);

	try
	{
		value = stoi(ini[section][key]);
	}
	catch (...)
	{
		value = deflt;
	}
}
void Settings::SetInt(const std::string& section, const std::string& key, const int value, const int deflt)
{
	if (!ini.has(section))
		ini[section];

	if (!ini[section].has(key))
		ini[section][key] = std::to_string(deflt);

	ini[section][key] = std::to_string(value);
}

void Settings::GetSliderInt(const std::string& section, const std::string& key, int& value, const int deflt, const int min, const int max)
{
	GetInt(section, key, value, deflt);
	value = std::clamp(value, min, max);
	SetInt(section, key, value, deflt);
}
void Settings::SetSliderInt(const std::string& section, const std::string& key, int& value, const int deflt, const int min, const int max)
{
	value = std::clamp(value, min, max);
	SetInt(section, key, value, deflt);
}

void Settings::GetFloat(const std::string& section, const std::string& key, float& value, const float deflt)
{
	if (!ini.has(section))
		ini[section];

	if (!ini[section].has(key))
		ini[section][key] = std::to_string(deflt);

	try
	{
		value = stof(ini[section][key]);
	}
	catch (...)
	{
		value = deflt;
	}
}
void Settings::SetFloat(const std::string& section, const std::string& key, const float value, const float deflt)
{
	if (!ini.has(section))
		ini[section];

	if (!ini[section].has(key))
		ini[section][key] = std::to_string(deflt);

	ini[section][key] = std::to_string(value);
}

void Settings::GetSliderFloat(const std::string& section, const std::string& key, float& value, const float deflt, const float min, const float max)
{
	GetFloat(section, key, value, deflt);
	value = std::clamp(value, min, max);
	SetFloat(section, key, value, deflt);
}
void Settings::SetSliderFloat(const std::string& section, const std::string& key, float& value, const float deflt, const float min, const float max)
{
	value = std::clamp(value, min, max);
	SetFloat(section, key, value, deflt);
}

void Settings::GetRgb(const std::string& section, const std::string& key, float* value, const float* deflt)
{
	const auto keyR = format(FMT_STRING("{}R"), key);
	const auto keyG = format(FMT_STRING("{}G"), key);
	const auto keyB = format(FMT_STRING("{}B"), key);

	GetFloat(section, keyR, value[0], deflt[0]);
	GetFloat(section, keyG, value[1], deflt[1]);
	GetFloat(section, keyB, value[2], deflt[2]);
	Utils::ValidateRgb(value);
}
void Settings::SetRgb(const std::string& section, const std::string& key, float* value, const float* deflt)
{
	Utils::ValidateRgb(value);

	const auto keyR = format(FMT_STRING("{}R"), key);
	const auto keyG = format(FMT_STRING("{}G"), key);
	const auto keyB = format(FMT_STRING("{}B"), key);

	SetFloat(section, keyR, value[0], deflt[0]);
	SetFloat(section, keyG, value[1], deflt[1]);
	SetFloat(section, keyB, value[2], deflt[2]);
}

void Settings::GetBool(const std::string& section, const std::string& key, bool& value, const bool deflt)
{
	auto bufferValue = static_cast<int>(value);
	const auto bufferdeflt = static_cast<int>(deflt);
	GetSliderInt(section, key, bufferValue, bufferdeflt, 0, 1);
	value = static_cast<bool>(bufferValue);
}
void Settings::SetBool(const std::string& section, const std::string& key, const bool& value, const bool deflt)
{
	auto bufferValue = static_cast<int>(value);
	const auto bufferdeflt = static_cast<int>(deflt);
	SetSliderInt(section, key, bufferValue, bufferdeflt, 0, 1);
}

void Settings::GetActorSettings(const std::string& section, EspSettings::Actors& value, const EspSettings::Actors& deflt)
{
	GetBool(section, "Enabled", value.enabled, deflt.enabled);
	GetSliderInt(section, "EnabledDistance", value.enabledDistance, deflt.enabledDistance, 0, 3000);
	GetBool(section, "DrawAlive", value.drawAlive, deflt.drawAlive);
	GetRgb(section, "AliveColor", value.aliveColor, deflt.aliveColor);
	GetBool(section, "DrawDowned", value.drawDowned, deflt.drawDowned);
	GetRgb(section, "DownedColor", value.downedColor, deflt.downedColor);
	GetBool(section, "DrawDead", value.drawDead, deflt.drawDead);
	GetRgb(section, "DeadColor", value.deadColor, deflt.deadColor);
	GetBool(section, "DrawUnknown", value.drawUnknown, deflt.drawUnknown);
	GetRgb(section, "UnknownColor", value.unknownColor, deflt.unknownColor);
	GetBool(section, "DrawEnabled", value.drawEnabled, deflt.drawEnabled);
	GetSliderFloat(section, "EnabledAlpha", value.enabledAlpha, deflt.enabledAlpha, 0.0f, 1.0f);
	GetBool(section, "DrawDisabled", value.drawDisabled, deflt.drawDisabled);
	GetSliderFloat(section, "DisabledAlpha", value.disabledAlpha, deflt.disabledAlpha, 0.0f, 1.0f);
	GetBool(section, "DrawNamed", value.drawNamed, deflt.drawNamed);
	GetBool(section, "DrawUnnamed", value.drawUnnamed, deflt.drawUnnamed);
	GetBool(section, "ShowName", value.showName, deflt.showName);
	GetBool(section, "ShowDistance", value.showDistance, deflt.showDistance);
	GetBool(section, "ShowHealth", value.showHealth, deflt.showHealth);
	GetBool(section, "ShowDeadHealth", value.showDeadHealth, deflt.showDeadHealth);
}
void Settings::SetActorSettings(const std::string& section, EspSettings::Actors& value, const EspSettings::Actors& deflt)
{
	SetBool(section, "Enabled", value.enabled, deflt.enabled);
	SetSliderInt(section, "EnabledDistance", value.enabledDistance, deflt.enabledDistance, 0, 3000);
	SetBool(section, "DrawAlive", value.drawAlive, deflt.drawAlive);
	SetRgb(section, "AliveColor", value.aliveColor, deflt.aliveColor);
	SetBool(section, "DrawDowned", value.drawDowned, deflt.drawDowned);
	SetRgb(section, "DownedColor", value.downedColor, deflt.downedColor);
	SetBool(section, "DrawDead", value.drawDead, deflt.drawDead);
	SetRgb(section, "DeadColor", value.deadColor, deflt.deadColor);
	SetBool(section, "DrawUnknown", value.drawUnknown, deflt.drawUnknown);
	SetRgb(section, "UnknownColor", value.unknownColor, deflt.unknownColor);
	SetBool(section, "DrawEnabled", value.drawEnabled, deflt.drawEnabled);
	SetSliderFloat(section, "EnabledAlpha", value.enabledAlpha, deflt.enabledAlpha, 0.0f, 1.0f);
	SetBool(section, "DrawDisabled", value.drawDisabled, deflt.drawDisabled);
	SetSliderFloat(section, "DisabledAlpha", value.disabledAlpha, deflt.disabledAlpha, 0.0f, 1.0f);
	SetBool(section, "DrawNamed", value.drawNamed, deflt.drawNamed);
	SetBool(section, "DrawUnnamed", value.drawUnnamed, deflt.drawUnnamed);
	SetBool(section, "ShowName", value.showName, deflt.showName);
	SetBool(section, "ShowDistance", value.showDistance, deflt.showDistance);
	SetBool(section, "ShowHealth", value.showHealth, deflt.showHealth);
	SetBool(section, "ShowDeadHealth", value.showDeadHealth, deflt.showDeadHealth);
}

void Settings::GetItemSettings(const std::string& section, EspSettings::Items& value, const EspSettings::Items& deflt)
{
	GetBool(section, "Enabled", value.enabled, deflt.enabled);
	GetSliderInt(section, "EnabledDistance", value.enabledDistance, deflt.enabledDistance, 0, 3000);
	GetRgb(section, "Color", value.color, deflt.color);
	GetBool(section, "DrawEnabled", value.drawEnabled, deflt.drawEnabled);
	GetSliderFloat(section, "EnabledAlpha", value.enabledAlpha, deflt.enabledAlpha, 0.0f, 1.0f);
	GetBool(section, "DrawDisabled", value.drawDisabled, deflt.drawDisabled);
	GetSliderFloat(section, "DisabledAlpha", value.disabledAlpha, deflt.disabledAlpha, 0.0f, 1.0f);
	GetBool(section, "DrawNamed", value.drawNamed, deflt.drawNamed);
	GetBool(section, "DrawUnnamed", value.drawUnnamed, deflt.drawUnnamed);
	GetBool(section, "ShowName", value.showName, deflt.showName);
	GetBool(section, "ShowDistance", value.showDistance, deflt.showDistance);
}
void Settings::SetItemSettings(const std::string& section, EspSettings::Items& value, const EspSettings::Items& deflt)
{
	SetBool(section, "Enabled", value.enabled, deflt.enabled);
	SetSliderInt(section, "EnabledDistance", value.enabledDistance, deflt.enabledDistance, 0, 3000);
	SetRgb(section, "Color", value.color, deflt.color);
	SetBool(section, "DrawEnabled", value.drawEnabled, deflt.drawEnabled);
	SetSliderFloat(section, "EnabledAlpha", value.enabledAlpha, deflt.enabledAlpha, 0.0f, 1.0f);
	SetBool(section, "DrawDisabled", value.drawDisabled, deflt.drawDisabled);
	SetSliderFloat(section, "DisabledAlpha", value.disabledAlpha, deflt.disabledAlpha, 0.0f, 1.0f);
	SetBool(section, "DrawNamed", value.drawNamed, deflt.drawNamed);
	SetBool(section, "DrawUnnamed", value.drawUnnamed, deflt.drawUnnamed);
	SetBool(section, "ShowName", value.showName, deflt.showName);
	SetBool(section, "ShowDistance", value.showDistance, deflt.showDistance);
}

void Settings::GetLooterSettings()
{
	auto modeInt = 0;
	GetInt("Looter", "mode", modeInt, 0);
	looter.mode = static_cast<LooterSettings::Mode>(modeInt);

	GetBool("Looter", "lootersNpc", looter.looters.npcs, false);
	GetBool("Looter", "lootersGroundItems", looter.looters.groundItems, false);
	GetBool("Looter", "lootersContainers", looter.looters.containers, false);
	GetBool("Looter", "lootersFlora", looter.looters.flora, false);

	GetBool("Looter", "weaponsAll", looter.selection.weapons.all, false);
	GetBool("Looter", "weaponsOneStar", looter.selection.weapons.oneStar, false);
	GetBool("Looter", "weaponsTwoStar", looter.selection.weapons.twoStar, false);
	GetBool("Looter", "weaponsThreeStar", looter.selection.weapons.threeStar, false);

	GetBool("Looter", "apparelAll", looter.selection.apparel.all, false);
	GetBool("Looter", "apparelOneStar", looter.selection.apparel.oneStar, false);
	GetBool("Looter", "apparelTwoStar", looter.selection.apparel.twoStar, false);
	GetBool("Looter", "apparelThreeStar", looter.selection.apparel.threeStar, false);

	GetBool("Looter", "aidAll", looter.selection.aid.all, false);
	GetBool("Looter", "aidMagazines", looter.selection.aid.magazines, false);
	GetBool("Looter", "aidBobbleheads", looter.selection.aid.bobbleheads, false);

	GetBool("Looter", "miscAll", looter.selection.misc.all, false);

	GetBool("Looter", "holoAll", looter.selection.holo.all, false);

	GetBool("Looter", "notesAll", looter.selection.notes.all, false);
	GetBool("Looter", "notesTreasureMaps", looter.selection.notes.treasureMaps, false);
	GetBool("Looter", "notesPlansKnown", looter.selection.notes.plansKnown, false);
	GetBool("Looter", "notesPlansUnknown", looter.selection.notes.plansUnknown, false);

	GetBool("Looter", "junkAll", looter.selection.junk.all, false);
	for (auto& [formId, isEnabled] : looter.selection.junk.components)
	{
		GetBool("Looter", format(FMT_STRING("junk{:x}"), formId), isEnabled, false);
	}

	GetBool("Looter", "floraAll", looter.selection.flora.all, false);
	for (auto& [formId, isEnabled] : looter.selection.flora.components)
	{
		GetBool("Looter", format(FMT_STRING("flora{:x}"), formId), isEnabled, false);
	}

	GetBool("Looter", "modsAll", looter.selection.mods.all, false);

	GetBool("Looter", "ammoAll", looter.selection.ammo.all, false);

	GetBool("Looter", "otherCaps", looter.selection.other.caps, false);

	for (const auto& [formIdString, isEnabledString] : ini["LooterWhitelist"])
	{
		looter.selection.whitelist.emplace(stoul(formIdString, nullptr, 16), static_cast<bool>(stoi(isEnabledString)));
	}

	for (const auto& [formIdString, isEnabledString] : ini["LooterBlacklist"])
	{
		looter.selection.blacklist.emplace(stoul(formIdString, nullptr, 16), static_cast<bool>(stoi(isEnabledString)));
	}
}
void Settings::SetLooterSettings()
{
	SetInt("Looter", "mode", static_cast<int>(looter.mode), 0);

	SetBool("Looter", "lootersNpc", looter.looters.npcs, false);
	SetBool("Looter", "lootersGroundItems", looter.looters.groundItems, false);
	SetBool("Looter", "lootersContainers", looter.looters.containers, false);
	SetBool("Looter", "lootersFlora", looter.looters.flora, false);

	SetBool("Looter", "weaponsAll", looter.selection.weapons.all, false);
	SetBool("Looter", "weaponsOneStar", looter.selection.weapons.oneStar, false);
	SetBool("Looter", "weaponsTwoStar", looter.selection.weapons.twoStar, false);
	SetBool("Looter", "weaponsThreeStar", looter.selection.weapons.threeStar, false);

	SetBool("Looter", "apparelAll", looter.selection.apparel.all, false);
	SetBool("Looter", "apparelOneStar", looter.selection.apparel.oneStar, false);
	SetBool("Looter", "apparelTwoStar", looter.selection.apparel.twoStar, false);
	SetBool("Looter", "apparelThreeStar", looter.selection.apparel.threeStar, false);

	SetBool("Looter", "aidAll", looter.selection.aid.all, false);
	SetBool("Looter", "aidMagazines", looter.selection.aid.magazines, false);
	SetBool("Looter", "aidBobbleheads", looter.selection.aid.bobbleheads, false);

	SetBool("Looter", "miscAll", looter.selection.misc.all, false);

	SetBool("Looter", "holoAll", looter.selection.holo.all, false);

	SetBool("Looter", "notesAll", looter.selection.notes.all, false);
	SetBool("Looter", "notesTreasureMaps", looter.selection.notes.treasureMaps, false);
	SetBool("Looter", "notesPlansKnown", looter.selection.notes.plansKnown, false);
	SetBool("Looter", "notesPlansUnknown", looter.selection.notes.plansUnknown, false);

	SetBool("Looter", "junkAll", looter.selection.junk.all, false);
	for (const auto& [formId, isEnabled] : looter.selection.junk.components)
	{
		SetBool("Looter", format(FMT_STRING("junk{:x}"), formId), isEnabled, false);
	}

	SetBool("Looter", "floraAll", looter.selection.flora.all, false);
	for (const auto& [formId, isEnabled] : looter.selection.flora.components)
	{
		SetBool("Looter", format(FMT_STRING("flora{:x}"), formId), isEnabled, false);
	}

	SetBool("Looter", "modsAll", looter.selection.mods.all, false);

	SetBool("Looter", "ammoAll", looter.selection.ammo.all, false);

	SetBool("Looter", "otherCaps", looter.selection.other.caps, false);

	ini.remove("LooterWhitelist");
	for (const auto& [formId, isEnabled] : looter.selection.whitelist)
	{
		SetBool("LooterWhitelist", format(FMT_STRING("{:x}"), formId), isEnabled, false);
	}

	ini.remove("LooterBlacklist");
	for (const auto& [formId, isEnabled] : looter.selection.blacklist)
	{
		SetBool("LooterBlacklist", format(FMT_STRING("{:x}"), formId), isEnabled, false);
	}
}

void Settings::GetWeaponSettings()
{
	GetBool("WeaponEditorSettings", "WeaponNoRecoil", weapons.noRecoil, weapons.noRecoil);
	GetBool("WeaponEditorSettings", "WeaponNoSpread", weapons.noSpread, weapons.noSpread);
	GetBool("WeaponEditorSettings", "WeaponNoSway", weapons.noSway, weapons.noSway);
	GetBool("WeaponEditorSettings", "WeaponInfiniteAmmo", weapons.infiniteAmmo, weapons.infiniteAmmo);
	GetBool("WeaponEditorSettings", "WeaponInstantReload", weapons.instantReload, weapons.instantReload);
	GetBool("WeaponEditorSettings", "WeaponAutomaticflag", weapons.automaticflag, weapons.automaticflag);
	GetBool("WeaponEditorSettings", "WeaponCapacityEnabled", weapons.capacityEnabled, weapons.capacityEnabled);
	GetSliderInt("WeaponEditorSettings", "WeaponCapacity", weapons.capacity, weapons.capacity, 0, 999);
	GetBool("WeaponEditorSettings", "WeaponSpeedEnabled", weapons.speedEnabled, weapons.speedEnabled);
	GetSliderFloat("WeaponEditorSettings", "WeaponSpeed", weapons.speed, weapons.speed, 1.0f, 100.0f);
	GetBool("WeaponEditorSettings", "WeaponReachEnabled", weapons.reachEnabled, weapons.reachEnabled);
	GetSliderFloat("WeaponEditorSettings", "WeaponReach", weapons.reach, weapons.reach, 1.0f, 999.0f);
	GetBool("WeaponEditorSettings", "WeaponRangeEnabled", weapons.rangeEnabled, weapons.rangeEnabled);
	GetSliderFloat("WeaponEditorSettings", "WeaponRange", weapons.range, weapons.range, 0.f, 5000.0f);
}
void Settings::SetWeaponSettings()
{
	SetBool("WeaponEditorSettings", "WeaponNoRecoil", weapons.noRecoil, weapons.noRecoil);
	SetBool("WeaponEditorSettings", "WeaponNoSpread", weapons.noSpread, weapons.noSpread);
	SetBool("WeaponEditorSettings", "WeaponNoSway", weapons.noSway, weapons.noSway);
	SetBool("WeaponEditorSettings", "WeaponInfiniteAmmo", weapons.infiniteAmmo, weapons.infiniteAmmo);
	SetBool("WeaponEditorSettings", "WeaponInstantReload", weapons.instantReload, weapons.instantReload);
	SetBool("WeaponEditorSettings", "WeaponAutomaticflag", weapons.automaticflag, weapons.automaticflag);
	SetBool("WeaponEditorSettings", "WeaponCapacityEnabled", weapons.capacityEnabled, weapons.capacityEnabled);
	SetSliderInt("WeaponEditorSettings", "WeaponCapacity", weapons.capacity, weapons.capacity, 0, 999);
	SetBool("WeaponEditorSettings", "WeaponSpeedEnabled", weapons.speedEnabled, weapons.speedEnabled);
	SetSliderFloat("WeaponEditorSettings", "WeaponSpeed", weapons.speed, weapons.speed, 1.0f, 100.0f);
	SetBool("WeaponEditorSettings", "WeaponReachEnabled", weapons.reachEnabled, weapons.reachEnabled);
	SetSliderFloat("WeaponEditorSettings", "WeaponReach", weapons.reach, weapons.reach, 1.0f, 999.0f);
	SetBool("WeaponEditorSettings", "WeaponRangeEnabled", weapons.rangeEnabled, weapons.rangeEnabled);
	SetSliderFloat("WeaponEditorSettings", "WeaponRange", weapons.range, weapons.range, 0.f, 5000.0f);
}

void Settings::GetTargetSettings()
{
	GetBool("TargetSettings", "LockNPCs", targetting.lockNpCs, targetting.lockNpCs);
	GetBool("TargetSettings", "IndirectNPCs", targetting.dmgRedirect, targetting.dmgRedirect);
	GetBool("TargetSettings", "DirectNPCs", targetting.dmgSend, targetting.dmgSend);
	GetBool("TargetSettings", "TargetUnknown", targetting.targetUnknown, targetting.targetUnknown);
	GetSliderFloat("TargetSettings", "LockingFOV", targetting.lockingFov, targetting.lockingFov, 5.0f, 40.0f);
	GetBool("TargetSettings", "IgnoreEssentialNPCs", targetting.ignoreEssentialNpcs, targetting.ignoreEssentialNpcs);
	GetBool("TargetSettings", "IgnoreNonHostileNPCs", targetting.ignoreNonHostileNpcs, targetting.ignoreNonHostileNpcs);
	GetRgb("TargetSettings", "LockingColor", targetting.lockedColor, targetting.lockedColor);
	GetBool("TargetSettings", "Retargeting", targetting.retargeting, targetting.retargeting);
	GetSliderInt("TargetSettings", "Cooldown", targetting.cooldown, targetting.cooldown, 0, 120);
	GetSliderInt("TargetSettings", "SendDamageMin", targetting.sendDamageMin, targetting.sendDamageMin, 1, 60);
	GetSliderInt("TargetSettings", "SendDamageMax", targetting.sendDamageMax, targetting.sendDamageMax, 1, 60);
	GetSliderInt("TargetSettings", "FavoriteIndex", targetting.favoriteIndex, targetting.favoriteIndex, 0, 12);
	if (targetting.sendDamageMax < targetting.sendDamageMin)
		targetting.sendDamageMax = targetting.sendDamageMin;
}
void Settings::SetTargetSettings()
{
	if (targetting.sendDamageMax < targetting.sendDamageMin)
		targetting.sendDamageMax = targetting.sendDamageMin;

	SetBool("TargetSettings", "LockNPCs", targetting.lockNpCs, targetting.lockNpCs);
	SetBool("TargetSettings", "IndirectNPCs", targetting.dmgRedirect, targetting.dmgRedirect);
	SetBool("TargetSettings", "DirectNPCs", targetting.dmgSend, targetting.dmgSend);
	SetBool("TargetSettings", "TargetUnknown", targetting.targetUnknown, targetting.targetUnknown);
	SetSliderFloat("TargetSettings", "LockingFOV", targetting.lockingFov, targetting.lockingFov, 5.0f, 40.0f);
	SetBool("TargetSettings", "IgnoreEssentialNPCs", targetting.ignoreEssentialNpcs, targetting.ignoreEssentialNpcs);
	SetBool("TargetSettings", "IgnoreNonHostileNPCs", targetting.ignoreNonHostileNpcs, targetting.ignoreNonHostileNpcs);
	SetRgb("TargetSettings", "LockingColor", targetting.lockedColor, targetting.lockedColor);
	SetBool("TargetSettings", "Retargeting", targetting.retargeting, targetting.retargeting);
	SetSliderInt("TargetSettings", "Cooldown", targetting.cooldown, targetting.cooldown, 0, 120);
	SetSliderInt("TargetSettings", "SendDamageMin", targetting.sendDamageMin, targetting.sendDamageMin, 1, 60);
	SetSliderInt("TargetSettings", "SendDamageMax", targetting.sendDamageMax, targetting.sendDamageMax, 1, 60);
	SetSliderInt("TargetSettings", "FavoriteIndex", targetting.favoriteIndex, targetting.favoriteIndex, 0, 12);
}

void Settings::GetLocalPlayerSettings()
{
	GetBool("LocalPlayerSettings", "PositionSpoofingEnabled", localPlayer.positionSpoofingEnabled, localPlayer.positionSpoofingEnabled);
	GetSliderInt("LocalPlayerSettings", "PositionSpoofingHeight", localPlayer.positionSpoofingHeight, localPlayer.positionSpoofingHeight, -524287, 524287);
	GetBool("LocalPlayerSettings", "NoclipEnabled", localPlayer.noclipEnabled, localPlayer.noclipEnabled);
	GetSliderFloat("LocalPlayerSettings", "NoclipSpeed", localPlayer.noclipSpeed, localPlayer.noclipSpeed, 0.0f, 2.0f);
	GetBool("LocalPlayerSettings", "ClientState", localPlayer.clientState, localPlayer.clientState);
	GetBool("LocalPlayerSettings", "AutomaticClientState", localPlayer.automaticClientState, localPlayer.automaticClientState);
	GetBool("LocalPlayerSettings", "FreezeApEnabled", localPlayer.freezeApEnabled, localPlayer.freezeApEnabled);
	GetBool("LocalPlayerSettings", "ActionPointsEnabled", localPlayer.actionPointsEnabled, localPlayer.actionPointsEnabled);
	GetSliderInt("LocalPlayerSettings", "ActionPoints", localPlayer.actionPoints, localPlayer.actionPoints, 0, 99999);
	GetBool("LocalPlayerSettings", "StrengthEnabled", localPlayer.strengthEnabled, localPlayer.strengthEnabled);
	GetSliderInt("LocalPlayerSettings", "Strength", localPlayer.strength, localPlayer.strength, 0, 99999);
	GetBool("LocalPlayerSettings", "PerceptionEnabled", localPlayer.perceptionEnabled, localPlayer.perceptionEnabled);
	GetSliderInt("LocalPlayerSettings", "Perception", localPlayer.perception, localPlayer.perception, 0, 99999);
	GetBool("LocalPlayerSettings", "EnduranceEnabled", localPlayer.enduranceEnabled, localPlayer.enduranceEnabled);
	GetSliderInt("LocalPlayerSettings", "Endurance", localPlayer.endurance, localPlayer.endurance, 0, 99999);
	GetBool("LocalPlayerSettings", "CharismaEnabled", localPlayer.charismaEnabled, localPlayer.charismaEnabled);
	GetSliderInt("LocalPlayerSettings", "Charisma", localPlayer.charisma, localPlayer.charisma, 0, 99999);
	GetBool("LocalPlayerSettings", "IntelligenceEnabled", localPlayer.intelligenceEnabled, localPlayer.intelligenceEnabled);
	GetSliderInt("LocalPlayerSettings", "Intelligence", localPlayer.intelligence, localPlayer.intelligence, 0, 99999);
	GetBool("LocalPlayerSettings", "AgilityEnabled", localPlayer.agilityEnabled, localPlayer.agilityEnabled);
	GetSliderInt("LocalPlayerSettings", "Agility", localPlayer.agility, localPlayer.agility, 0, 99999);
	GetBool("LocalPlayerSettings", "LuckEnabled", localPlayer.luckEnabled, localPlayer.luckEnabled);
	GetSliderInt("LocalPlayerSettings", "Luck", localPlayer.luck, localPlayer.luck, 0, 99999);
}
void Settings::SetLocalPlayerSettings()
{
	SetBool("LocalPlayerSettings", "PositionSpoofingEnabled", localPlayer.positionSpoofingEnabled, localPlayer.positionSpoofingEnabled);
	SetSliderInt("LocalPlayerSettings", "PositionSpoofingHeight", localPlayer.positionSpoofingHeight, localPlayer.positionSpoofingHeight, -524287, 524287);
	SetBool("LocalPlayerSettings", "NoclipEnabled", localPlayer.noclipEnabled, localPlayer.noclipEnabled);
	SetSliderFloat("LocalPlayerSettings", "NoclipSpeed", localPlayer.noclipSpeed, localPlayer.noclipSpeed, 0.0f, 2.0f);
	SetBool("LocalPlayerSettings", "ClientState", localPlayer.clientState, localPlayer.clientState);
	SetBool("LocalPlayerSettings", "AutomaticClientState", localPlayer.automaticClientState, localPlayer.automaticClientState);
	SetBool("LocalPlayerSettings", "FreezeApEnabled", localPlayer.freezeApEnabled, localPlayer.freezeApEnabled);
	SetBool("LocalPlayerSettings", "ActionPointsEnabled", localPlayer.actionPointsEnabled, localPlayer.actionPointsEnabled);
	SetSliderInt("LocalPlayerSettings", "ActionPoints", localPlayer.actionPoints, localPlayer.actionPoints, 0, 99999);
	SetBool("LocalPlayerSettings", "StrengthEnabled", localPlayer.strengthEnabled, localPlayer.strengthEnabled);
	SetSliderInt("LocalPlayerSettings", "Strength", localPlayer.strength, localPlayer.strength, 0, 99999);
	SetBool("LocalPlayerSettings", "PerceptionEnabled", localPlayer.perceptionEnabled, localPlayer.perceptionEnabled);
	SetSliderInt("LocalPlayerSettings", "Perception", localPlayer.perception, localPlayer.perception, 0, 99999);
	SetBool("LocalPlayerSettings", "EnduranceEnabled", localPlayer.enduranceEnabled, localPlayer.enduranceEnabled);
	SetSliderInt("LocalPlayerSettings", "Endurance", localPlayer.endurance, localPlayer.endurance, 0, 99999);
	SetBool("LocalPlayerSettings", "CharismaEnabled", localPlayer.charismaEnabled, localPlayer.charismaEnabled);
	SetSliderInt("LocalPlayerSettings", "Charisma", localPlayer.charisma, localPlayer.charisma, 0, 99999);
	SetBool("LocalPlayerSettings", "IntelligenceEnabled", localPlayer.intelligenceEnabled, localPlayer.intelligenceEnabled);
	SetSliderInt("LocalPlayerSettings", "Intelligence", localPlayer.intelligence, localPlayer.intelligence, 0, 99999);
	SetBool("LocalPlayerSettings", "AgilityEnabled", localPlayer.agilityEnabled, localPlayer.agilityEnabled);
	SetSliderInt("LocalPlayerSettings", "Agility", localPlayer.agility, localPlayer.agility, 0, 99999);
	SetBool("LocalPlayerSettings", "LuckEnabled", localPlayer.luckEnabled, localPlayer.luckEnabled);
	SetSliderInt("LocalPlayerSettings", "Luck", localPlayer.luck, localPlayer.luck, 0, 99999);
}

void Settings::GetOpkSettings()
{
	GetBool("OpkSettings", "OpkNpcsEnabled", opk.enabled, false);
}
void Settings::SetOpkSettings()
{
	SetBool("OpkSettings", "OpkNpcsEnabled", opk.enabled, false);
}

void Settings::GetUtilitySettings()
{
	GetBool("UtilitySettings", "DebugEsp", utilities.debugEsp, false);
	GetDword("UtilitySettings", "PtrFormId", utilities.ptrFormId, 0);
	GetDword("UtilitySettings", "AddressFormId", utilities.addressFormId, 0);
}
void Settings::SetUtilitySettings()
{
	SetBool("UtilitySettings", "DebugEsp", utilities.debugEsp, false);
	SetDword("UtilitySettings", "PtrFormId", utilities.ptrFormId, 0);
	SetDword("UtilitySettings", "AddressFormId", utilities.addressFormId, 0);
}

void Settings::GetSwapperSettings()
{
	GetDword("SwapperSettings", "SourceFormId", swapper.sourceFormId, 0);
	GetDword("SwapperSettings", "DestinationFormId", swapper.destinationFormId, 0);
}
void Settings::SetSwapperSettings()
{
	SetDword("SwapperSettings", "SourceFormId", swapper.sourceFormId, 0);
	SetDword("SwapperSettings", "DestinationFormId", swapper.destinationFormId, 0);
}

void Settings::GetTransferSettings()
{
	GetDword("TransferSettings", "SourceFormId", customTransferSettings.sourceFormId, customTransferSettings.sourceFormId);
	GetDword("TransferSettings", "DestinationFormId", customTransferSettings.destinationFormId, customTransferSettings.destinationFormId);
	GetBool("TransferSettings", "UseWhitelist", customTransferSettings.useWhitelist, customTransferSettings.useWhitelist);
	GetBool("TransferSettings", "UseBlacklist", customTransferSettings.useBlacklist, customTransferSettings.useBlacklist);
	for (auto i = 0; i < 32; i++)
	{
		GetBool("TransferSettings", format(FMT_STRING("Whitelisted{:d}"), i), customTransferSettings.whitelisted[i], customTransferSettings.whitelisted[i]);
		GetDword("TransferSettings", format(FMT_STRING("Whitelist{:d}"), i), customTransferSettings.whitelist[i], customTransferSettings.whitelist[i]);
	}
	for (auto i = 0; i < 32; i++)
	{
		GetBool("TransferSettings", format(FMT_STRING("Blacklisted{:d}"), i), customTransferSettings.blacklisted[i], customTransferSettings.blacklisted[i]);
		GetDword("TransferSettings", format(FMT_STRING("Blacklist{:d}"), i), customTransferSettings.blacklist[i], customTransferSettings.blacklist[i]);
	}
}
void Settings::SetTransferSettings()
{
	SetDword("TransferSettings", "SourceFormId", customTransferSettings.sourceFormId, customTransferSettings.sourceFormId);
	SetDword("TransferSettings", "DestinationFormId", customTransferSettings.destinationFormId, customTransferSettings.destinationFormId);
	SetBool("TransferSettings", "UseWhitelist", customTransferSettings.useWhitelist, customTransferSettings.useWhitelist);
	SetBool("TransferSettings", "UseBlacklist", customTransferSettings.useBlacklist, customTransferSettings.useBlacklist);
	for (auto i = 0; i < 32; i++)
	{
		SetBool("TransferSettings", format(FMT_STRING("Whitelisted{:d}"), i), customTransferSettings.whitelisted[i], customTransferSettings.whitelisted[i]);
		SetDword("TransferSettings", format(FMT_STRING("Whitelist{:d}"), i), customTransferSettings.whitelist[i], customTransferSettings.whitelist[i]);
	}
	for (auto i = 0; i < 32; i++)
	{
		SetBool("TransferSettings", format(FMT_STRING("Blacklisted{:d}"), i), customTransferSettings.blacklisted[i], customTransferSettings.blacklisted[i]);
		SetDword("TransferSettings", format(FMT_STRING("Blacklist{:d}"), i), customTransferSettings.blacklist[i], customTransferSettings.blacklist[i]);
	}
}

void Settings::GetTeleportSettings()
{
	for (auto i = 0; i < 16; i++)
	{
		GetFloat("TeleportSettings", format(FMT_STRING("Destination{:d}X"), i), teleporter.entries[i].position.x, 0.f);
		GetFloat("TeleportSettings", format(FMT_STRING("Destination{:d}Y"), i), teleporter.entries[i].position.y, 0.f);
		GetFloat("TeleportSettings", format(FMT_STRING("Destination{:d}Z"), i), teleporter.entries[i].position.z, 0.f);
		GetFloat("TeleportSettings", format(FMT_STRING("Destination{:d}W"), i), teleporter.entries[i].rotation.z, 0.f);
		GetDword("TeleportSettings", format(FMT_STRING("CellFormId{:d}"), i), teleporter.entries[i].cellFormId, 0);
		GetBool("TeleportSettings", format(FMT_STRING("DisableSaving{:d}"), i), teleporter.entries[i].disableSaving, false);
	}
}
void Settings::SetTeleportSettings()
{
	for (auto i = 0; i < 16; i++)
	{
		SetFloat("TeleportSettings", format(FMT_STRING("Destination{:d}X"), i), teleporter.entries[i].position.x, 0.f);
		SetFloat("TeleportSettings", format(FMT_STRING("Destination{:d}Y"), i), teleporter.entries[i].position.y, 0.f);
		SetFloat("TeleportSettings", format(FMT_STRING("Destination{:d}Z"), i), teleporter.entries[i].position.z, 0.f);
		SetFloat("TeleportSettings", format(FMT_STRING("Destination{:d}W"), i), teleporter.entries[i].rotation.z, 0.f);
		SetDword("TeleportSettings", format(FMT_STRING("CellFormId{:d}"), i), teleporter.entries[i].cellFormId, 0);
		SetBool("TeleportSettings", format(FMT_STRING("DisableSaving{:d}"), i), teleporter.entries[i].disableSaving, false);
	}
}

void Settings::GetNukeCodeSettings()
{
	GetBool("NukeCodeSettings", "AutomaticNukeCodes", customNukeCodeSettings.automaticNukeCodes, false);
}
void Settings::SetNukeCodeSettings()
{
	SetBool("NukeCodeSettings", "AutomaticNukeCodes", customNukeCodeSettings.automaticNukeCodes, false);
}

void Settings::GetLegendarySettings()
{
	GetBool("LegendarySettings", "OverrideLivingOneStar", esp.npcsExt.overrideLivingOneStar, esp.npcsExt.overrideLivingOneStar);
	GetRgb("LegendarySettings", "LivingOneStarColor", esp.npcsExt.livingOneStarColor, esp.npcsExt.livingOneStarColor);
	GetBool("LegendarySettings", "OverrideDeadOneStar", esp.npcsExt.overrideDeadOneStar, esp.npcsExt.overrideDeadOneStar);
	GetRgb("LegendarySettings", "DeadOneStarColor", esp.npcsExt.deadOneStarColor, esp.npcsExt.deadOneStarColor);
	GetBool("LegendarySettings", "OverrideLivingTwoStar", esp.npcsExt.overrideLivingTwoStar, esp.npcsExt.overrideLivingTwoStar);
	GetRgb("LegendarySettings", "LivingTwoStarColor", esp.npcsExt.livingTwoStarColor, esp.npcsExt.livingTwoStarColor);
	GetBool("LegendarySettings", "OverrideDeadTwoStar", esp.npcsExt.overrideDeadTwoStar, esp.npcsExt.overrideDeadTwoStar);
	GetRgb("LegendarySettings", "DeadTwoStarColor", esp.npcsExt.deadTwoStarColor, esp.npcsExt.deadTwoStarColor);
	GetBool("LegendarySettings", "OverrideLivingThreeStar", esp.npcsExt.overrideLivingThreeStar, esp.npcsExt.overrideLivingThreeStar);
	GetRgb("LegendarySettings", "LivingThreeStarColor", esp.npcsExt.livingThreeStarColor, esp.npcsExt.livingThreeStarColor);
	GetBool("LegendarySettings", "OverrideDeadThreeStar", esp.npcsExt.overrideDeadThreeStar, esp.npcsExt.overrideDeadThreeStar);
	GetRgb("LegendarySettings", "DeadThreeStarColor", esp.npcsExt.deadThreeStarColor, esp.npcsExt.deadThreeStarColor);
}
void Settings::SetLegendarySettings()
{
	SetBool("LegendarySettings", "OverrideLivingOneStar", esp.npcsExt.overrideLivingOneStar, esp.npcsExt.overrideLivingOneStar);
	SetRgb("LegendarySettings", "LivingOneStarColor", esp.npcsExt.livingOneStarColor, esp.npcsExt.livingOneStarColor);
	SetBool("LegendarySettings", "OverrideDeadOneStar", esp.npcsExt.overrideDeadOneStar, esp.npcsExt.overrideDeadOneStar);
	SetRgb("LegendarySettings", "DeadOneStarColor", esp.npcsExt.deadOneStarColor, esp.npcsExt.deadOneStarColor);
	SetBool("LegendarySettings", "OverrideLivingTwoStar", esp.npcsExt.overrideLivingTwoStar, esp.npcsExt.overrideLivingTwoStar);
	SetRgb("LegendarySettings", "LivingTwoStarColor", esp.npcsExt.livingTwoStarColor, esp.npcsExt.livingTwoStarColor);
	SetBool("LegendarySettings", "OverrideDeadTwoStar", esp.npcsExt.overrideDeadTwoStar, esp.npcsExt.overrideDeadTwoStar);
	SetRgb("LegendarySettings", "DeadTwoStarColor", esp.npcsExt.deadTwoStarColor, esp.npcsExt.deadTwoStarColor);
	SetBool("LegendarySettings", "OverrideLivingThreeStar", esp.npcsExt.overrideLivingThreeStar, esp.npcsExt.overrideLivingThreeStar);
	SetRgb("LegendarySettings", "LivingThreeStarColor", esp.npcsExt.livingThreeStarColor, esp.npcsExt.livingThreeStarColor);
	SetBool("LegendarySettings", "OverrideDeadThreeStar", esp.npcsExt.overrideDeadThreeStar, esp.npcsExt.overrideDeadThreeStar);
	SetRgb("LegendarySettings", "DeadThreeStarColor", esp.npcsExt.deadThreeStarColor, esp.npcsExt.deadThreeStarColor);
}

void Settings::GetFluxSettings()
{
	GetBool("FluxSettings", "CrimsonFluxEnabled", esp.floraExt.crimsonFluxEnabled, false);
	GetBool("FluxSettings", "CobaltFluxEnabled", esp.floraExt.cobaltFluxEnabled, false);
	GetBool("FluxSettings", "YellowcakeFluxEnabled", esp.floraExt.yellowcakeFluxEnabled, false);
	GetBool("FluxSettings", "FluorescentFluxEnabled", esp.floraExt.fluorescentFluxEnabled, false);
	GetBool("FluxSettings", "VioletFluxEnabled", esp.floraExt.violetFluxEnabled, false);
}
void Settings::SetFluxSettings()
{
	SetBool("FluxSettings", "CrimsonFluxEnabled", esp.floraExt.crimsonFluxEnabled, false);
	SetBool("FluxSettings", "CobaltFluxEnabled", esp.floraExt.cobaltFluxEnabled, false);
	SetBool("FluxSettings", "YellowcakeFluxEnabled", esp.floraExt.yellowcakeFluxEnabled, false);
	SetBool("FluxSettings", "FluorescentFluxEnabled", esp.floraExt.fluorescentFluxEnabled, false);
	SetBool("FluxSettings", "VioletFluxEnabled", esp.floraExt.violetFluxEnabled, false);
}

void Settings::GetKnownRecipeSettings()
{
	GetBool("KnownRecipeSettings", "KnownRecipesEnabled", esp.plansExt.knownRecipesEnabled, false);
	GetBool("KnownRecipeSettings", "UnknownRecipesEnabled", esp.plansExt.unknownRecipesEnabled, false);
}
void Settings::SetKnownRecipeSettings()
{
	SetBool("KnownRecipeSettings", "KnownRecipesEnabled", esp.plansExt.knownRecipesEnabled, false);
	SetBool("KnownRecipeSettings", "UnknownRecipesEnabled", esp.plansExt.unknownRecipesEnabled, false);
}

void Settings::GetMeleeSettings()
{
	GetBool("MeleeSettings", "MeleeEnabled", melee.enabled, false);
	GetSliderInt("MeleeSettings", "MeleeSpeedMin", melee.speedMin, 60, 1, 60);
	GetSliderInt("MeleeSettings", "MeleeSpeedMax", melee.speedMax, 60, 1, 60);

	if (melee.speedMax < melee.speedMin)
		melee.speedMax = melee.speedMin;
}
void Settings::SetMeleeSettings()
{
	if (melee.speedMax < melee.speedMin)
		melee.speedMax = melee.speedMin;

	SetBool("MeleeSettings", "MeleeEnabled", melee.enabled, false);
	SetSliderInt("MeleeSettings", "MeleeSpeedMin", melee.speedMin, 60, 1, 60);
	SetSliderInt("MeleeSettings", "MeleeSpeedMax", melee.speedMax, 60, 1, 60);
}

void Settings::GetChargenSettings()
{
	GetBool("ChargenSettings", "ChargenEditingEnabled", characterEditor.enabled, false);
	GetSliderFloat("ChargenSettings", "Thin", characterEditor.thin, 0, 0.0f, 1.0f);
	GetSliderFloat("ChargenSettings", "Muscular", characterEditor.muscular, 0, 0.0f, 1.0f);
	GetSliderFloat("ChargenSettings", "Large", characterEditor.large, 0, 0.0f, 1.0f);
}
void Settings::SetChargenSettings()
{
	SetBool("ChargenSettings", "ChargenEditingEnabled", characterEditor.enabled, false);
	SetSliderFloat("ChargenSettings", "Thin", characterEditor.thin, 0, 0.0f, 1.0f);
	SetSliderFloat("ChargenSettings", "Muscular", characterEditor.muscular, 0, 0.0f, 1.0f);
	SetSliderFloat("ChargenSettings", "Large", characterEditor.large, 0, 0.0f, 1.0f);
}

void Settings::GetBitMsgWriterSettings()
{
	GetBool("BitMsgWriter", "AllowMessages", msgWriter.enabled, false);
}

void Settings::SetBitMsgWriterSettings()
{
	SetBool("BitMsgWriter", "AllowMessages", msgWriter.enabled, false);
}

void Settings::GetInfoBoxSettings()
{
	GetBool("esp.infoBox", "drawPlayerInfo", esp.infobox.drawPlayerInfo, false);
	GetBool("esp.infoBox", "drawPositionSpoofingStatus", esp.infobox.drawPositionSpoofingStatus, false);
	GetBool("esp.infoBox", "drawNukeCodes", esp.infobox.drawNukeCodes, false);
	GetBool("esp.infoBox", "drawFPS", esp.infobox.drawFps, false);
}

void Settings::SetInfoBoxSettings()
{
	SetBool("esp.infoBox", "drawPlayerInfo", esp.infobox.drawPlayerInfo, false);
	SetBool("esp.infoBox", "drawPositionSpoofingStatus", esp.infobox.drawPositionSpoofingStatus, false);
	SetBool("esp.infoBox", "drawNukeCodes", esp.infobox.drawNukeCodes, false);
	SetBool("esp.infoBox", "drawFPS", esp.infobox.drawFps, false);
}

void Settings::GetEspSettings()
{
	GetActorSettings("esp.players", esp.players, EspSettings::Actors());
	GetActorSettings("esp.npcs", esp.npcs, EspSettings::Actors());

	GetItemSettings("esp.containers", esp.containers, EspSettings::Items());
	GetItemSettings("esp.junk", esp.junk, EspSettings::Items());
	GetItemSettings("esp.plans", esp.plans, EspSettings::Items());
	GetItemSettings("esp.magazines", esp.magazines, EspSettings::Items());
	GetItemSettings("esp.bobbleheads", esp.bobbleheads, EspSettings::Items());
	GetItemSettings("esp.items", esp.items, EspSettings::Items());
	GetItemSettings("esp.flora", esp.flora, EspSettings::Items());
	GetItemSettings("esp.entities", esp.entities, EspSettings::Items());

	GetLegendarySettings();
	GetKnownRecipeSettings();
	GetFluxSettings();
	GetInfoBoxSettings();

	for (const auto& [formIdString, isEnabledString] : ini["EspWhitelist"])
	{
		esp.whitelist.emplace(stoul(formIdString, nullptr, 16), static_cast<bool>(stoi(isEnabledString)));
	}

	for (const auto& [formIdString, isEnabledString] : ini["EspBlacklist"])
	{
		esp.blacklist.emplace(stoul(formIdString, nullptr, 16), static_cast<bool>(stoi(isEnabledString)));
	}
}
void Settings::SetEspSettings()
{
	SetActorSettings("esp.players", esp.players, EspSettings::Actors());
	SetActorSettings("esp.npcs", esp.npcs, EspSettings::Actors());

	SetItemSettings("esp.containers", esp.containers, EspSettings::Items());
	SetItemSettings("esp.junk", esp.junk, EspSettings::Items());
	SetItemSettings("esp.plans", esp.plans, EspSettings::Items());
	SetItemSettings("esp.magazines", esp.magazines, EspSettings::Items());
	SetItemSettings("esp.bobbleheads", esp.bobbleheads, EspSettings::Items());
	SetItemSettings("esp.items", esp.items, EspSettings::Items());
	SetItemSettings("esp.flora", esp.flora, EspSettings::Items());
	SetItemSettings("esp.entities", esp.entities, EspSettings::Items());

	SetLegendarySettings();
	SetKnownRecipeSettings();
	SetFluxSettings();
	SetInfoBoxSettings();

	ini.remove("EspWhitelist");
	for (const auto& [formId, isEnabled] : esp.whitelist)
	{
		SetBool("EspWhitelist", format(FMT_STRING("{:x}"), formId), isEnabled, false);
	}

	ini.remove("EspBlacklist");
	for (const auto& [formId, isEnabled] : esp.blacklist)
	{
		SetBool("EspBlacklist", format(FMT_STRING("{:x}"), formId), isEnabled, false);
	}
}

void Settings::Read()
{
	file.read(ini);

	GetLooterSettings();
	GetEspSettings();

	GetWeaponSettings();
	GetTargetSettings();
	GetLocalPlayerSettings();
	GetOpkSettings();
	GetUtilitySettings();
	GetTransferSettings();
	GetTeleportSettings();
	GetNukeCodeSettings();
	GetMeleeSettings();
	GetChargenSettings();
	GetBitMsgWriterSettings();

	file.write(ini, true);
}

void Settings::Write()
{
	file.read(ini);

	SetLooterSettings();
	SetEspSettings();

	SetWeaponSettings();
	SetTargetSettings();
	SetLocalPlayerSettings();
	SetOpkSettings();
	SetUtilitySettings();
	SetTransferSettings();
	SetTeleportSettings();
	SetNukeCodeSettings();
	SetMeleeSettings();
	SetChargenSettings();
	SetBitMsgWriterSettings();

	file.write(ini, true);
}
