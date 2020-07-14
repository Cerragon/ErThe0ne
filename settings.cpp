#include "settings.h"
#include "utils.h"

#include "ErectusMemory.h"

#define MINI_CASE_SENSITIVE
#include "ini.h"
#include "fmt/format.h"

mINI::INIFile file("Erectus.ini");
mINI::INIStructure ini;

//esp
OverlaySettingsA defltPlayerSettings
{
	true,					//Enabled
	1500,					//EnabledDistance
	true,					//DrawAlive
	{ 0.0f, 1.0f, 0.0f },	//AliveColor
	true,					//DrawDowned
	{ 1.0f, 1.0f, 0.0f },	//DownedColor
	false,					//DrawDead
	{ 1.0f, 0.0f, 0.0f },	//DeadColor
	false,					//DrawUnknown
	{ 1.0f, 0.5f, 0.0f },	//UnknownColor
	true,					//DrawEnabled;
	1.0f,					//EnabledAlpha;
	false,					//DrawDisabled;
	0.5f,					//DisabledAlpha;
	true,					//DrawNamed;
	false,					//DrawUnnamed;
	true,					//ShowName;
	true,					//ShowDistance;
	true,					//ShowHealth
	false,					//ShowDeadHealth
	true,					//TextShadowed;
	true,					//TextCentered;
};
OverlaySettingsA defltNpcSettings
{
	true,					//Enabled
	1500,					//EnabledDistance
	true,					//DrawAlive
	{ 0.5f, 1.0f, 1.0f },	//AliveColor
	false,					//DrawDowned
	{ 0.7f, 0.7f, 1.0f },	//DownedColor
	false,					//DrawDead
	{ 1.0f, 0.5f, 0.5f },	//DeadColor
	false,					//DrawUnknown
	{ 1.0f, 0.5f, 1.0f },	//UnknownColor
	true,					//DrawEnabled;
	1.0f,					//EnabledAlpha;
	false,					//DrawDisabled;
	0.5f,					//DisabledAlpha;
	true,					//DrawNamed;
	false,					//DrawUnnamed;
	true,					//ShowName;
	true,					//ShowDistance;
	true,					//ShowHealth
	false,					//ShowDeadHealth
	true,					//TextShadowed;
	true,					//TextCentered;
};
OverlaySettingsB defltOverlaySettingsB
{
	false,					//Enabled
	1500,					//EnabledDistance
	{ 1.0f, 1.0f, 1.0f },	//Color
	true,					//DrawEnabled
	1.0f,					//EnabledAlpha
	false,					//DrawDisabled
	0.5f,					//DisabledAlpha
	true,					//DrawNamed
	false,					//DrawUnnamed
	true,					//ShowName
	true,					//ShowDistance
	true,					//TextCentered
	true,					//TextShadowed
	{},						//Whitelisted
	{},				//Whitelist
};
LegendarySettings defltLegendarySettings
{
	true,
	{ 0.7f, 1.0f, 0.7f },
	true,
	{ 1.0f, 0.5f, 0.5f },
	true,
	{ 0.7f, 1.0f, 0.7f },
	true,
	{ 1.0f, 0.5f, 0.5f },
	true,
	{ 0.7f, 1.0f, 0.7f },
	true,
	{ 1.0f, 0.5f, 0.5f },
};

OverlaySettingsA Settings::playerSettings = defltPlayerSettings;
OverlaySettingsA Settings::npcSettings = defltNpcSettings;
OverlaySettingsB Settings::containerSettings = defltOverlaySettingsB;
OverlaySettingsB Settings::junkSettings = defltOverlaySettingsB;
OverlaySettingsB Settings::planSettings = defltOverlaySettingsB;
OverlaySettingsB Settings::magazineSettings = defltOverlaySettingsB;
OverlaySettingsB Settings::bobbleheadSettings = defltOverlaySettingsB;
OverlaySettingsB Settings::itemSettings = defltOverlaySettingsB;
OverlaySettingsB Settings::floraSettings = defltOverlaySettingsB;
OverlaySettingsB Settings::entitySettings = defltOverlaySettingsB;
LegendarySettings Settings::customLegendarySettings = defltLegendarySettings;
FluxSettings Settings::customFluxSettings = {};

//utils
TargetSettings defltTargetSettings
{
	.lockingFov = 20.0f,
	.lockingColor = { 1.0f, 0.0f, 1.0f },
	.sendDamageMin = 1,
	.sendDamageMax = 1,
	.favoriteIndex = 12,
};
LocalPlayerSettings defltLocalPlayerSettings
{
	false,
	524287,
	false,
	0.1f,
	false,
	false,
	false,
	false,
	90001,
	false,
	9001,
	false,
	9001,
	false,
	9001,
	false,
	9001,
	false,
	9001,
	false,
	9001,
	false,
	9001,
};
TransferSettings defltTransferSettings
{
	0x00000000,
	0x00000000,
	true,
	true,
	{},
	{},
	{
		true, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false,
	},
	{
		0x00021B3B, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
	},
};

WeaponSettings Settings::weapons = defaultWeaponSettings;
TargetSettings Settings::targetting = defltTargetSettings;
LocalPlayerSettings Settings::customLocalPlayerSettings = defltLocalPlayerSettings;
OpkSettings Settings::opk = {};
UtilitySettings Settings::utilities = {};
SwapperSettings Settings::swapper = {};
TransferSettings Settings::customTransferSettings = defltTransferSettings;
TeleportSettings Settings::teleporter = {};
NukeCodeSettings Settings::customNukeCodeSettings = {};
MeleeSettings Settings::melee = {false,10,20 };
ChargenSettings Settings::characterEditor = { false,0.33f,0.33f,0.33f };
ExtraNpcSettings Settings::customExtraNpcSettings = {};
KnownRecipeSettings Settings::recipes = { true, true };


void Settings::GetDword(const std::string& section, const std::string& key, DWORD& value, const DWORD deflt)
{
	if (!ini.has(section))
		ini[section];

	if (!ini[section].has(key))
	{
		ini[section][key] = fmt::format("{:08X}", deflt);
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

void Settings::SetDword(const std::string& section, const std::string& key, const DWORD value, const DWORD deflt)
{
	if (!ini.has(section))
		ini[section];

	if (!ini[section].has(key))
	{
		ini[section][key] = fmt::format("{:08x}", deflt);
	}
	ini[section][key] = fmt::format("{:08x}", value);
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
	Utils::ValidateInt(value, min, max);
	SetInt(section, key, value, deflt);
}

void Settings::SetSliderInt(const std::string& section, const std::string& key, int& value, const int deflt, const int min, const int max)
{
	Utils::ValidateInt(value, min, max);
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
	Utils::ValidateFloat(value, min, max);
	SetFloat(section, key, value, deflt);
}

void Settings::SetSliderFloat(const std::string& section, const std::string& key, float& value, const float deflt, const float min, const float max)
{
	Utils::ValidateFloat(value, min, max);
	SetFloat(section, key, value, deflt);
}

void Settings::GetRgb(const std::string& section, const std::string& key, float* value, const float* deflt)
{
	auto keyR = fmt::format("{}R", key);
	auto keyG = fmt::format("{}G", key);
	auto keyB = fmt::format("{}B", key);

	GetFloat(section, keyR, value[0], deflt[0]);
	GetFloat(section, keyG, value[1], deflt[1]);
	GetFloat(section, keyB, value[2], deflt[2]);
	Utils::ValidateRgb(value);
}

void Settings::SetRgb(const std::string& section, const std::string& key, float* value, const float* deflt)
{
	Utils::ValidateRgb(value);

	auto keyR = fmt::format("{}R", key);
	auto keyG = fmt::format("{}G", key);
	auto keyB = fmt::format("{}B", key);

	SetFloat(section, keyR, value[0], deflt[0]);
	SetFloat(section, keyG, value[1], deflt[1]);
	SetFloat(section, keyB, value[2], deflt[2]);
}

void Settings::GetQuadFloat(const std::string& section, const std::string& key, float* value, const float* deflt)
{
	auto keyX = fmt::format("{}X", key);
	auto keyY = fmt::format("{}Y", key);
	auto keyZ = fmt::format("{}Z", key);
	auto keyW = fmt::format("{}W", key);

	GetFloat(section, keyX, value[0], deflt == nullptr ? 0 : deflt[0]);
	GetFloat(section, keyY, value[1], deflt == nullptr ? 0 : deflt[1]);
	GetFloat(section, keyZ, value[2], deflt == nullptr ? 0 : deflt[2]);
	GetFloat(section, keyW, value[3], deflt == nullptr ? 0 : deflt[3]);
}

void Settings::SetQuadFloat(const std::string& section, const std::string& key, float* value, const float* deflt)
{
	auto keyX = fmt::format("{}X", key);
	auto keyY = fmt::format("{}Y", key);
	auto keyZ = fmt::format("{}Z", key);
	auto keyW = fmt::format("{}W", key);

	SetFloat(section, keyX, value[0], deflt == nullptr ? 0 : deflt[0]);
	SetFloat(section, keyY, value[1], deflt == nullptr ? 0 : deflt[1]);
	SetFloat(section, keyZ, value[2], deflt == nullptr ? 0 : deflt[2]);
	SetFloat(section, keyW, value[3], deflt == nullptr ? 0 : deflt[3]);
}

void Settings::GetBool(const std::string& section, const std::string& key, bool& value, const bool deflt)
{
	auto bufferValue = static_cast<int>(value);
	auto bufferdeflt = static_cast<int>(deflt);
	GetSliderInt(section, key, bufferValue, bufferdeflt, 0, 1);
	value = static_cast<bool>(bufferValue);
}

void Settings::SetBool(const std::string& section, const std::string& key, const bool& value, const bool deflt)
{
	auto bufferValue = static_cast<int>(value);
	auto bufferdeflt = static_cast<int>(deflt);
	SetSliderInt(section, key, bufferValue, bufferdeflt, 0, 1);
}

void Settings::GetOverlaySettingsA(const std::string& section, OverlaySettingsA* value, const OverlaySettingsA* deflt)
{
	GetBool(section, "Enabled", value->enabled, deflt->enabled);
	GetSliderInt(section, "EnabledDistance", value->enabledDistance, deflt->enabledDistance, 0, 3000);
	GetBool(section, "DrawAlive", value->drawAlive, deflt->drawAlive);
	GetRgb(section, "AliveColor", value->aliveColor, deflt->aliveColor);
	GetBool(section, "DrawDowned", value->drawDowned, deflt->drawDowned);
	GetRgb(section, "DownedColor", value->downedColor, deflt->downedColor);
	GetBool(section, "DrawDead", value->drawDead, deflt->drawDead);
	GetRgb(section, "DeadColor", value->deadColor, deflt->deadColor);
	GetBool(section, "DrawUnknown", value->drawUnknown, deflt->drawUnknown);
	GetRgb(section, "UnknownColor", value->unknownColor, deflt->unknownColor);
	GetBool(section, "DrawEnabled", value->drawEnabled, deflt->drawEnabled);
	GetSliderFloat(section, "EnabledAlpha", value->enabledAlpha, deflt->enabledAlpha, 0.0f, 1.0f);
	GetBool(section, "DrawDisabled", value->drawDisabled, deflt->drawDisabled);
	GetSliderFloat(section, "DisabledAlpha", value->disabledAlpha, deflt->disabledAlpha, 0.0f, 1.0f);
	GetBool(section, "DrawNamed", value->drawNamed, deflt->drawNamed);
	GetBool(section, "DrawUnnamed", value->drawUnnamed, deflt->drawUnnamed);
	GetBool(section, "ShowName", value->showName, deflt->showName);
	GetBool(section, "ShowDistance", value->showDistance, deflt->showDistance);
	GetBool(section, "ShowHealth", value->showHealth, deflt->showHealth);
	GetBool(section, "ShowDeadHealth", value->showDeadHealth, deflt->showDeadHealth);
	GetBool(section, "TextShadowed", value->textShadowed, deflt->textShadowed);
	GetBool(section, "TextCentered", value->textCentered, deflt->textCentered);
}

void Settings::SetOverlaySettingsA(const std::string& section, OverlaySettingsA* value, const OverlaySettingsA* deflt)
{
	SetBool(section, "Enabled", value->enabled, deflt->enabled);
	SetSliderInt(section, "EnabledDistance", value->enabledDistance, deflt->enabledDistance, 0, 3000);
	SetBool(section, "DrawAlive", value->drawAlive, deflt->drawAlive);
	SetRgb(section, "AliveColor", value->aliveColor, deflt->aliveColor);
	SetBool(section, "DrawDowned", value->drawDowned, deflt->drawDowned);
	SetRgb(section, "DownedColor", value->downedColor, deflt->downedColor);
	SetBool(section, "DrawDead", value->drawDead, deflt->drawDead);
	SetRgb(section, "DeadColor", value->deadColor, deflt->deadColor);
	SetBool(section, "DrawUnknown", value->drawUnknown, deflt->drawUnknown);
	SetRgb(section, "UnknownColor", value->unknownColor, deflt->unknownColor);
	SetBool(section, "DrawEnabled", value->drawEnabled, deflt->drawEnabled);
	SetSliderFloat(section, "EnabledAlpha", value->enabledAlpha, deflt->enabledAlpha, 0.0f, 1.0f);
	SetBool(section, "DrawDisabled", value->drawDisabled, deflt->drawDisabled);
	SetSliderFloat(section, "DisabledAlpha", value->disabledAlpha, deflt->disabledAlpha, 0.0f, 1.0f);
	SetBool(section, "DrawNamed", value->drawNamed, deflt->drawNamed);
	SetBool(section, "DrawUnnamed", value->drawUnnamed, deflt->drawUnnamed);
	SetBool(section, "ShowName", value->showName, deflt->showName);
	SetBool(section, "ShowDistance", value->showDistance, deflt->showDistance);
	SetBool(section, "ShowHealth", value->showHealth, deflt->showHealth);
	SetBool(section, "ShowDeadHealth", value->showDeadHealth, deflt->showDeadHealth);
	SetBool(section, "TextShadowed", value->textShadowed, deflt->textShadowed);
	SetBool(section, "TextCentered", value->textCentered, deflt->textCentered);
}

void Settings::GetOverlaySettingsB(const std::string& section, OverlaySettingsB* value, const OverlaySettingsB* deflt)
{
	GetBool(section, "Enabled", value->enabled, deflt->enabled);
	GetSliderInt(section, "EnabledDistance", value->enabledDistance, deflt->enabledDistance, 0, 3000);
	GetRgb(section, "Color", value->color, deflt->color);
	GetBool(section, "DrawEnabled", value->drawEnabled, deflt->drawEnabled);
	GetSliderFloat(section, "EnabledAlpha", value->enabledAlpha, deflt->enabledAlpha, 0.0f, 1.0f);
	GetBool(section, "DrawDisabled", value->drawDisabled, deflt->drawDisabled);
	GetSliderFloat(section, "DisabledAlpha", value->disabledAlpha, deflt->disabledAlpha, 0.0f, 1.0f);
	GetBool(section, "DrawNamed", value->drawNamed, deflt->drawNamed);
	GetBool(section, "DrawUnnamed", value->drawUnnamed, deflt->drawUnnamed);
	GetBool(section, "ShowName", value->showName, deflt->showName);
	GetBool(section, "ShowDistance", value->showDistance, deflt->showDistance);
	GetBool(section, "TextShadowed", value->textShadowed, deflt->textShadowed);
	GetBool(section, "TextCentered", value->textCentered, deflt->textCentered);
	for (auto i = 0; i < 32; i++)
	{
		GetBool(section, fmt::format("Whitelisted{:d}", i), value->whitelisted[i], deflt->whitelisted[i]);
		GetDword(section, fmt::format("Whitelist{:d}", i), value->whitelist[i], deflt->whitelist[i]);
	}
}

void Settings::SetOverlaySettingsB(const std::string& section, OverlaySettingsB* value, const OverlaySettingsB* deflt)
{
	SetBool(section, "Enabled", value->enabled, deflt->enabled);
	SetSliderInt(section, "EnabledDistance", value->enabledDistance, deflt->enabledDistance, 0, 3000);
	SetRgb(section, "Color", value->color, deflt->color);
	SetBool(section, "DrawEnabled", value->drawEnabled, deflt->drawEnabled);
	SetSliderFloat(section, "EnabledAlpha", value->enabledAlpha, deflt->enabledAlpha, 0.0f, 1.0f);
	SetBool(section, "DrawDisabled", value->drawDisabled, deflt->drawDisabled);
	SetSliderFloat(section, "DisabledAlpha", value->disabledAlpha, deflt->disabledAlpha, 0.0f, 1.0f);
	SetBool(section, "DrawNamed", value->drawNamed, deflt->drawNamed);
	SetBool(section, "DrawUnnamed", value->drawUnnamed, deflt->drawUnnamed);
	SetBool(section, "ShowName", value->showName, deflt->showName);
	SetBool(section, "ShowDistance", value->showDistance, deflt->showDistance);
	SetBool(section, "TextShadowed", value->textShadowed, deflt->textShadowed);
	SetBool(section, "TextCentered", value->textCentered, deflt->textCentered);
	for (auto i = 0; i < 32; i++)
	{
		SetBool(section, fmt::format("Whitelisted{:d}", i), value->whitelisted[i], deflt->whitelisted[i]);
		SetDword(section, fmt::format("Whitelist{:d}", i), value->whitelist[i], deflt->whitelist[i]);
	}
}

void Settings::GetInfoBoxSettings()
{
	GetBool("InfoBox", "drawScrapLooterStatus", infobox.drawScrapLooterStatus, false);
	GetBool("InfoBox", "drawItemLooterStatus", infobox.drawItemLooterStatus, false);
	GetBool("InfoBox", "drawNpcLooterStatus", infobox.drawNpcLooterStatus, false);
	GetBool("InfoBox", "drawContainerLooterStatus", infobox.drawContainerLooterStatus, false);
	GetBool("InfoBox", "drawHarvesterStatus", infobox.drawHarvesterStatus, false);

	GetBool("InfoBox", "drawPositionSpoofingStatus", infobox.drawPositionSpoofingStatus, false);
	GetBool("InfoBox", "drawNukeCodes", infobox.drawNukeCodes, false);
}

void Settings::SetInfoBoxSettings()
{
	SetBool("InfoBox", "drawScrapLooterStatus", infobox.drawScrapLooterStatus, false);
	SetBool("InfoBox", "drawItemLooterStatus", infobox.drawItemLooterStatus, false);
	SetBool("InfoBox", "drawNpcLooterStatus", infobox.drawNpcLooterStatus, false);
	SetBool("InfoBox", "drawContainerLooterStatus", infobox.drawContainerLooterStatus, false);
	SetBool("InfoBox", "drawHarvesterStatus", infobox.drawHarvesterStatus, false);

	SetBool("InfoBox", "drawPositionSpoofingStatus", infobox.drawPositionSpoofingStatus, false);
	SetBool("InfoBox", "drawNukeCodes", infobox.drawNukeCodes, false);
}

void Settings::GetItemLooterSettings()
{
	/*
	GetBool("ItemLooterSettings", "ItemKeybindEnabled", itemLooter.keybindEnabled, defltItemLooterSettings.keybindEnabled);
	GetBool("ItemLooterSettings", "ItemAutomaticLootingEnabled", itemLooter.autoLootingEnabled, defltItemLooterSettings.autoLootingEnabled);

	GetSliderInt("ItemLooterSettings", "ItemAutomaticSpeedMin", itemLooter.autoLootingSpeedMin, defltItemLooterSettings.autoLootingSpeedMin, 10, 60);
	GetSliderInt("ItemLooterSettings", "ItemAutomaticSpeedMax", itemLooter.autoLootingSpeedMax, defltItemLooterSettings.autoLootingSpeedMax, 10, 60);
	GetBool("ItemLooterSettings", "ItemLooterWeaponsEnabled", itemLooter.lootWeaponsEnabled, defltItemLooterSettings.lootWeaponsEnabled);
	GetSliderInt("ItemLooterSettings", "ItemLooterWeaponsDistance", itemLooter.lootWeaponsDistance, defltItemLooterSettings.lootWeaponsDistance, 0, 3000);
	GetBool("ItemLooterSettings", "ItemLooterArmorEnabled", itemLooter.lootArmorEnabled, defltItemLooterSettings.lootArmorEnabled);
	GetSliderInt("ItemLooterSettings", "ItemLooterArmorDistance", itemLooter.lootArmorDistance, defltItemLooterSettings.lootArmorDistance, 0, 3000);
	GetBool("ItemLooterSettings", "ItemLooterAmmoEnabled", itemLooter.lootAmmoEnabled, defltItemLooterSettings.lootAmmoEnabled);
	GetSliderInt("ItemLooterSettings", "ItemLooterAmmoDistance", itemLooter.lootAmmoDistance, defltItemLooterSettings.lootAmmoDistance, 0, 3000);
	GetBool("ItemLooterSettings", "ItemLooterModsEnabled", itemLooter.lootModsEnabled, defltItemLooterSettings.lootModsEnabled);
	GetSliderInt("ItemLooterSettings", "ItemLooterModsDistance", itemLooter.lootModsDistance, defltItemLooterSettings.lootModsDistance, 0, 3000);
	GetBool("ItemLooterSettings", "ItemLooterMagazinesEnabled", itemLooter.lootMagazinesEnabled, defltItemLooterSettings.lootMagazinesEnabled);
	GetSliderInt("ItemLooterSettings", "ItemLooterMagazinesDistance", itemLooter.lootMagazinesDistance, defltItemLooterSettings.lootMagazinesDistance, 0, 3000);
	GetBool("ItemLooterSettings", "ItemLooterBobbleheadsEnabled", itemLooter.lootBobbleheadsEnabled, defltItemLooterSettings.lootBobbleheadsEnabled);
	GetSliderInt("ItemLooterSettings", "ItemLooterBobbleheadsDistance", itemLooter.lootBobbleheadsDistance, defltItemLooterSettings.lootBobbleheadsDistance, 0, 3000);
	GetBool("ItemLooterSettings", "ItemLooterAidEnabled", itemLooter.lootAidEnabled, defltItemLooterSettings.lootAidEnabled);
	GetSliderInt("ItemLooterSettings", "ItemLooterAidDistance", itemLooter.lootAidDistance, defltItemLooterSettings.lootAidDistance, 0, 3000);
	GetBool("ItemLooterSettings", "ItemLooterKnownPlansEnabled", itemLooter.lootKnownPlansEnabled, defltItemLooterSettings.lootKnownPlansEnabled);
	GetSliderInt("ItemLooterSettings", "ItemLooterKnownPlansDistance", itemLooter.lootKnownPlansDistance, defltItemLooterSettings.lootKnownPlansDistance, 0, 3000);
	GetBool("ItemLooterSettings", "ItemLooterUnknownPlansEnabled", itemLooter.lootUnknownPlansEnabled, defltItemLooterSettings.lootUnknownPlansEnabled);
	GetSliderInt("ItemLooterSettings", "ItemLooterUnknownPlansDistance", itemLooter.lootUnknownPlansDistance, defltItemLooterSettings.lootUnknownPlansDistance, 0, 3000);
	GetBool("ItemLooterSettings", "ItemLooterMiscEnabled", itemLooter.lootMiscEnabled, defltItemLooterSettings.lootMiscEnabled);
	GetSliderInt("ItemLooterSettings", "ItemLooterMiscDistance", itemLooter.lootMiscDistance, defltItemLooterSettings.lootMiscDistance, 0, 3000);
	GetBool("ItemLooterSettings", "ItemLooterUnlistedEnabled", itemLooter.lootUnlistedEnabled, defltItemLooterSettings.lootUnlistedEnabled);
	GetSliderInt("ItemLooterSettings", "ItemLooterUnlistedDistance", itemLooter.lootUnlistedDistance, defltItemLooterSettings.lootUnlistedDistance, 0, 3000);
	GetBool("ItemLooterSettings", "ItemLooterListEnabled", itemLooter.lootListEnabled, defltItemLooterSettings.lootListEnabled);
	GetSliderInt("ItemLooterSettings", "ItemLooterListDistance", itemLooter.lootListDistance, defltItemLooterSettings.lootListDistance, 0, 3000);
	GetBool("ItemLooterSettings", "ItemLooterBlacklistToggle", itemLooter.blacklistToggle, defltItemLooterSettings.blacklistToggle);
	for (auto i = 0; i < 100; i++)
	{
		GetBool("ItemLooterSettings", fmt::format("ItemLooterEnabled{:d}", i), itemLooter.enabledList[i], defltItemLooterSettings.enabledList[i]);
		GetDword("ItemLooterSettings", fmt::format("ItemLooterList{:d}", i), itemLooter.formIdList[i], defltItemLooterSettings.formIdList[i]);
	}
	for (auto i = 0; i < 64; i++)
	{
		GetBool("ItemLooterSettings", fmt::format("ItemLooterBlacklistEnabled{:d}", i), itemLooter.blacklistEnabled[i], defltItemLooterSettings.blacklistEnabled[i]);
		GetDword("ItemLooterSettings", fmt::format("ItemLooterBlacklist{:d}", i), itemLooter.blacklist[i], defltItemLooterSettings.blacklist[i]);
	}

*/
}

void Settings::SetItemLooterSettings()
{
	/*
		SetBool("ItemLooterSettings", "ItemKeybindEnabled", itemLooter.keybindEnabled, defltItemLooterSettings.keybindEnabled);
		SetBool("ItemLooterSettings", "ItemAutomaticLootingEnabled", itemLooter.autoLootingEnabled, defltItemLooterSettings.autoLootingEnabled);
		SetSliderInt("ItemLooterSettings", "ItemAutomaticSpeedMin", itemLooter.autoLootingSpeedMin, defltItemLooterSettings.autoLootingSpeedMin, 10, 60);
		SetSliderInt("ItemLooterSettings", "ItemAutomaticSpeedMax", itemLooter.autoLootingSpeedMax, defltItemLooterSettings.autoLootingSpeedMax, 10, 60);
		SetBool("ItemLooterSettings", "ItemLooterWeaponsEnabled", itemLooter.lootWeaponsEnabled, defltItemLooterSettings.lootWeaponsEnabled);
		SetSliderInt("ItemLooterSettings", "ItemLooterWeaponsDistance", itemLooter.lootWeaponsDistance, defltItemLooterSettings.lootWeaponsDistance, 0, 3000);
		SetBool("ItemLooterSettings", "ItemLooterArmorEnabled", itemLooter.lootArmorEnabled, defltItemLooterSettings.lootArmorEnabled);
		SetSliderInt("ItemLooterSettings", "ItemLooterArmorDistance", itemLooter.lootArmorDistance, defltItemLooterSettings.lootArmorDistance, 0, 3000);
		SetBool("ItemLooterSettings", "ItemLooterAmmoEnabled", itemLooter.lootAmmoEnabled, defltItemLooterSettings.lootAmmoEnabled);
		SetSliderInt("ItemLooterSettings", "ItemLooterAmmoDistance", itemLooter.lootAmmoDistance, defltItemLooterSettings.lootAmmoDistance, 0, 3000);
		SetBool("ItemLooterSettings", "ItemLooterModsEnabled", itemLooter.lootModsEnabled, defltItemLooterSettings.lootModsEnabled);
		SetSliderInt("ItemLooterSettings", "ItemLooterModsDistance", itemLooter.lootModsDistance, defltItemLooterSettings.lootModsDistance, 0, 3000);
		SetBool("ItemLooterSettings", "ItemLooterMagazinesEnabled", itemLooter.lootMagazinesEnabled, defltItemLooterSettings.lootMagazinesEnabled);
		SetSliderInt("ItemLooterSettings", "ItemLooterMagazinesDistance", itemLooter.lootMagazinesDistance, defltItemLooterSettings.lootMagazinesDistance, 0, 3000);
		SetBool("ItemLooterSettings", "ItemLooterBobbleheadsEnabled", itemLooter.lootBobbleheadsEnabled, defltItemLooterSettings.lootBobbleheadsEnabled);
		SetSliderInt("ItemLooterSettings", "ItemLooterBobbleheadsDistance", itemLooter.lootBobbleheadsDistance, defltItemLooterSettings.lootBobbleheadsDistance, 0, 3000);
		SetBool("ItemLooterSettings", "ItemLooterAidEnabled", itemLooter.lootAidEnabled, defltItemLooterSettings.lootAidEnabled);
		SetSliderInt("ItemLooterSettings", "ItemLooterAidDistance", itemLooter.lootAidDistance, defltItemLooterSettings.lootAidDistance, 0, 3000);
		SetBool("ItemLooterSettings", "ItemLooterKnownPlansEnabled", itemLooter.lootKnownPlansEnabled, defltItemLooterSettings.lootKnownPlansEnabled);
		SetSliderInt("ItemLooterSettings", "ItemLooterKnownPlansDistance", itemLooter.lootKnownPlansDistance, defltItemLooterSettings.lootKnownPlansDistance, 0, 3000);
		SetBool("ItemLooterSettings", "ItemLooterUnknownPlansEnabled", itemLooter.lootUnknownPlansEnabled, defltItemLooterSettings.lootUnknownPlansEnabled);
		SetSliderInt("ItemLooterSettings", "ItemLooterUnknownPlansDistance", itemLooter.lootUnknownPlansDistance, defltItemLooterSettings.lootUnknownPlansDistance, 0, 3000);
		SetBool("ItemLooterSettings", "ItemLooterMiscEnabled", itemLooter.lootMiscEnabled, defltItemLooterSettings.lootMiscEnabled);
		SetSliderInt("ItemLooterSettings", "ItemLooterMiscDistance", itemLooter.lootMiscDistance, defltItemLooterSettings.lootMiscDistance, 0, 3000);
		SetBool("ItemLooterSettings", "ItemLooterUnlistedEnabled", itemLooter.lootUnlistedEnabled, defltItemLooterSettings.lootUnlistedEnabled);
		SetSliderInt("ItemLooterSettings", "ItemLooterUnlistedDistance", itemLooter.lootUnlistedDistance, defltItemLooterSettings.lootUnlistedDistance, 0, 3000);
		SetBool("ItemLooterSettings", "ItemLooterListEnabled", itemLooter.lootListEnabled, defltItemLooterSettings.lootListEnabled);
		SetSliderInt("ItemLooterSettings", "ItemLooterListDistance", itemLooter.lootListDistance, defltItemLooterSettings.lootListDistance, 0, 3000);
		SetBool("ItemLooterSettings", "ItemLooterBlacklistToggle", itemLooter.blacklistToggle, defltItemLooterSettings.blacklistToggle);
		for (auto i = 0; i < 100; i++)
		{
			SetBool("ItemLooterSettings", fmt::format("ItemLooterEnabled{:d}", i), itemLooter.enabledList[i], defltItemLooterSettings.enabledList[i]);
			SetDword("ItemLooterSettings", fmt::format("ItemLooterList{:d}", i), itemLooter.formIdList[i], defltItemLooterSettings.formIdList[i]);
		}
		for (auto i = 0; i < 64; i++)
		{
			SetBool("ItemLooterSettings", fmt::format("ItemLooterBlacklistEnabled{:d}", i), itemLooter.blacklistEnabled[i], defltItemLooterSettings.blacklistEnabled[i]);
			SetDword("ItemLooterSettings", fmt::format("ItemLooterBlacklist{:d}", i), itemLooter.blacklist[i], defltItemLooterSettings.blacklist[i]);
		}
		*/
}

void Settings::GetWeaponSettings()
{
	GetBool("WeaponSettings", "WeaponNoRecoil", weapons.noRecoil, defaultWeaponSettings.noRecoil);
	GetBool("WeaponSettings", "WeaponNoSpread", weapons.noSpread, defaultWeaponSettings.noSpread);
	GetBool("WeaponSettings", "WeaponNoSway", weapons.noSway, defaultWeaponSettings.noSway);
	GetBool("WeaponSettings", "WeaponInfiniteAmmo", weapons.infiniteAmmo, defaultWeaponSettings.infiniteAmmo);
	GetBool("WeaponSettings", "WeaponInstantReload", weapons.instantReload, defaultWeaponSettings.instantReload);
	GetBool("WeaponSettings", "WeaponAutomaticflag", weapons.automaticflag, defaultWeaponSettings.automaticflag);
	GetBool("WeaponSettings", "WeaponCapacityEnabled", weapons.capacityEnabled, defaultWeaponSettings.capacityEnabled);
	GetSliderInt("WeaponSettings", "WeaponCapacity", weapons.capacity, defaultWeaponSettings.capacity, 0, 999);
	GetBool("WeaponSettings", "WeaponSpeedEnabled", weapons.speedEnabled, defaultWeaponSettings.speedEnabled);
	GetSliderFloat("WeaponSettings", "WeaponSpeed", weapons.speed, defaultWeaponSettings.speed, 0.0f, 100.0f);
	GetBool("WeaponSettings", "WeaponReachEnabled", weapons.reachEnabled, defaultWeaponSettings.reachEnabled);
	GetSliderFloat("WeaponSettings", "WeaponReach", weapons.reach, defaultWeaponSettings.reach, 0.0f, 999.0f);
}

void Settings::SetWeaponSettings()
{
	SetBool("WeaponSettings", "WeaponNoRecoil", weapons.noRecoil, defaultWeaponSettings.noRecoil);
	SetBool("WeaponSettings", "WeaponNoSpread", weapons.noSpread, defaultWeaponSettings.noSpread);
	SetBool("WeaponSettings", "WeaponNoSway", weapons.noSway, defaultWeaponSettings.noSway);
	SetBool("WeaponSettings", "WeaponInfiniteAmmo", weapons.infiniteAmmo, defaultWeaponSettings.infiniteAmmo);
	SetBool("WeaponSettings", "WeaponInstantReload", weapons.instantReload, defaultWeaponSettings.instantReload);
	SetBool("WeaponSettings", "WeaponAutomaticflag", weapons.automaticflag, defaultWeaponSettings.automaticflag);
	SetBool("WeaponSettings", "WeaponCapacityEnabled", weapons.capacityEnabled, defaultWeaponSettings.capacityEnabled);
	SetSliderInt("WeaponSettings", "WeaponCapacity", weapons.capacity, defaultWeaponSettings.capacity, 0, 999);
	SetBool("WeaponSettings", "WeaponSpeedEnabled", weapons.speedEnabled, defaultWeaponSettings.speedEnabled);
	SetSliderFloat("WeaponSettings", "WeaponSpeed", weapons.speed, defaultWeaponSettings.speed, 0.0f, 100.0f);
	SetBool("WeaponSettings", "WeaponReachEnabled", weapons.reachEnabled, defaultWeaponSettings.reachEnabled);
	SetSliderFloat("WeaponSettings", "WeaponReach", weapons.reach, defaultWeaponSettings.reach, 0.0f, 999.0f);
}

void Settings::GetTargetSettings()
{
	GetBool("TargetSettings", "LockPlayers", targetting.lockPlayers, defltTargetSettings.lockPlayers);
	GetBool("TargetSettings", "LockNPCs", targetting.lockNpCs, defltTargetSettings.lockNpCs);
	GetBool("TargetSettings", "IndirectPlayers", targetting.indirectPlayers, defltTargetSettings.indirectPlayers);
	GetBool("TargetSettings", "IndirectNPCs", targetting.indirectNpCs, defltTargetSettings.indirectNpCs);
	GetBool("TargetSettings", "DirectPlayers", targetting.directPlayers, defltTargetSettings.directPlayers);
	GetBool("TargetSettings", "DirectNPCs", targetting.directNpCs, defltTargetSettings.directNpCs);
	GetBool("TargetSettings", "TargetLiving", targetting.targetLiving, defltTargetSettings.targetLiving);
	GetBool("TargetSettings", "TargetDowned", targetting.targetDowned, defltTargetSettings.targetDowned);
	GetBool("TargetSettings", "TargetDead", targetting.targetDead, defltTargetSettings.targetDead);
	GetBool("TargetSettings", "TargetUnknown", targetting.targetUnknown, defltTargetSettings.targetUnknown);
	GetBool("TargetSettings", "IgnoreRenderDistance", targetting.ignoreRenderDistance, defltTargetSettings.ignoreRenderDistance);
	GetSliderFloat("TargetSettings", "LockingFOV", targetting.lockingFov, defltTargetSettings.lockingFov, 5.0f, 40.0f);
	GetBool("TargetSettings", "IgnoreEssentialNPCs", targetting.ignoreEssentialNpCs, defltTargetSettings.ignoreEssentialNpCs);
	GetRgb("TargetSettings", "LockingColor", targetting.lockingColor, defltTargetSettings.lockingColor);
	GetBool("TargetSettings", "Retargeting", targetting.retargeting, defltTargetSettings.retargeting);
	GetSliderInt("TargetSettings", "Cooldown", targetting.cooldown, defltTargetSettings.cooldown, 0, 120);
	GetSliderInt("TargetSettings", "SendDamageMin", targetting.sendDamageMin, defltTargetSettings.sendDamageMin, 1, 60);
	GetSliderInt("TargetSettings", "SendDamageMax", targetting.sendDamageMax, defltTargetSettings.sendDamageMax, 1, 60);
	GetSliderInt("TargetSettings", "FavoriteIndex", targetting.favoriteIndex, defltTargetSettings.favoriteIndex, 0, 12);
	if (targetting.sendDamageMax < targetting.sendDamageMin)
		targetting.sendDamageMax = targetting.sendDamageMin;
}

void Settings::SetTargetSettings()
{
	if (targetting.sendDamageMax < targetting.sendDamageMin)
		targetting.sendDamageMax = targetting.sendDamageMin;

	SetBool("TargetSettings", "LockPlayers", targetting.lockPlayers, defltTargetSettings.lockPlayers);
	SetBool("TargetSettings", "LockNPCs", targetting.lockNpCs, defltTargetSettings.lockNpCs);
	SetBool("TargetSettings", "IndirectPlayers", targetting.indirectPlayers, defltTargetSettings.indirectPlayers);
	SetBool("TargetSettings", "IndirectNPCs", targetting.indirectNpCs, defltTargetSettings.indirectNpCs);
	SetBool("TargetSettings", "DirectPlayers", targetting.directPlayers, defltTargetSettings.directPlayers);
	SetBool("TargetSettings", "DirectNPCs", targetting.directNpCs, defltTargetSettings.directNpCs);
	SetBool("TargetSettings", "TargetLiving", targetting.targetLiving, defltTargetSettings.targetLiving);
	SetBool("TargetSettings", "TargetDowned", targetting.targetDowned, defltTargetSettings.targetDowned);
	SetBool("TargetSettings", "TargetDead", targetting.targetDead, defltTargetSettings.targetDead);
	SetBool("TargetSettings", "TargetUnknown", targetting.targetUnknown, defltTargetSettings.targetUnknown);
	SetBool("TargetSettings", "IgnoreRenderDistance", targetting.ignoreRenderDistance, defltTargetSettings.ignoreRenderDistance);
	SetSliderFloat("TargetSettings", "LockingFOV", targetting.lockingFov, defltTargetSettings.lockingFov, 5.0f, 40.0f);
	SetBool("TargetSettings", "IgnoreEssentialNPCs", targetting.ignoreEssentialNpCs, defltTargetSettings.ignoreEssentialNpCs);
	SetRgb("TargetSettings", "LockingColor", targetting.lockingColor, defltTargetSettings.lockingColor);
	SetBool("TargetSettings", "Retargeting", targetting.retargeting, defltTargetSettings.retargeting);
	SetSliderInt("TargetSettings", "Cooldown", targetting.cooldown, defltTargetSettings.cooldown, 0, 120);
	SetSliderInt("TargetSettings", "SendDamageMin", targetting.sendDamageMin, defltTargetSettings.sendDamageMin, 1, 60);
	SetSliderInt("TargetSettings", "SendDamageMax", targetting.sendDamageMax, defltTargetSettings.sendDamageMax, 1, 60);
	SetSliderInt("TargetSettings", "FavoriteIndex", targetting.favoriteIndex, defltTargetSettings.favoriteIndex, 0, 12);
}

void Settings::GetLocalPlayerSettings()
{
	GetBool("LocalPlayerSettings", "PositionSpoofingEnabled", customLocalPlayerSettings.positionSpoofingEnabled, defltLocalPlayerSettings.positionSpoofingEnabled);
	GetSliderInt("LocalPlayerSettings", "PositionSpoofingHeight", customLocalPlayerSettings.positionSpoofingHeight, defltLocalPlayerSettings.positionSpoofingHeight, -524287, 524287);
	GetBool("LocalPlayerSettings", "NoclipEnabled", customLocalPlayerSettings.noclipEnabled, defltLocalPlayerSettings.noclipEnabled);
	GetSliderFloat("LocalPlayerSettings", "NoclipSpeed", customLocalPlayerSettings.noclipSpeed, defltLocalPlayerSettings.noclipSpeed, 0.0f, 2.0f);
	GetBool("LocalPlayerSettings", "ClientState", customLocalPlayerSettings.clientState, defltLocalPlayerSettings.clientState);
	GetBool("LocalPlayerSettings", "AutomaticClientState", customLocalPlayerSettings.automaticClientState, defltLocalPlayerSettings.automaticClientState);
	GetBool("LocalPlayerSettings", "FreezeApEnabled", customLocalPlayerSettings.freezeApEnabled, defltLocalPlayerSettings.freezeApEnabled);
	GetBool("LocalPlayerSettings", "ActionPointsEnabled", customLocalPlayerSettings.actionPointsEnabled, defltLocalPlayerSettings.actionPointsEnabled);
	GetSliderInt("LocalPlayerSettings", "ActionPoints", customLocalPlayerSettings.actionPoints, defltLocalPlayerSettings.actionPoints, 0, 99999);
	GetBool("LocalPlayerSettings", "StrengthEnabled", customLocalPlayerSettings.strengthEnabled, defltLocalPlayerSettings.strengthEnabled);
	GetSliderInt("LocalPlayerSettings", "Strength", customLocalPlayerSettings.strength, defltLocalPlayerSettings.strength, 0, 99999);
	GetBool("LocalPlayerSettings", "PerceptionEnabled", customLocalPlayerSettings.perceptionEnabled, defltLocalPlayerSettings.perceptionEnabled);
	GetSliderInt("LocalPlayerSettings", "Perception", customLocalPlayerSettings.perception, defltLocalPlayerSettings.perception, 0, 99999);
	GetBool("LocalPlayerSettings", "EnduranceEnabled", customLocalPlayerSettings.enduranceEnabled, defltLocalPlayerSettings.enduranceEnabled);
	GetSliderInt("LocalPlayerSettings", "Endurance", customLocalPlayerSettings.endurance, defltLocalPlayerSettings.endurance, 0, 99999);
	GetBool("LocalPlayerSettings", "CharismaEnabled", customLocalPlayerSettings.charismaEnabled, defltLocalPlayerSettings.charismaEnabled);
	GetSliderInt("LocalPlayerSettings", "Charisma", customLocalPlayerSettings.charisma, defltLocalPlayerSettings.charisma, 0, 99999);
	GetBool("LocalPlayerSettings", "IntelligenceEnabled", customLocalPlayerSettings.intelligenceEnabled, defltLocalPlayerSettings.intelligenceEnabled);
	GetSliderInt("LocalPlayerSettings", "Intelligence", customLocalPlayerSettings.intelligence, defltLocalPlayerSettings.intelligence, 0, 99999);
	GetBool("LocalPlayerSettings", "AgilityEnabled", customLocalPlayerSettings.agilityEnabled, defltLocalPlayerSettings.agilityEnabled);
	GetSliderInt("LocalPlayerSettings", "Agility", customLocalPlayerSettings.agility, defltLocalPlayerSettings.agility, 0, 99999);
	GetBool("LocalPlayerSettings", "LuckEnabled", customLocalPlayerSettings.luckEnabled, defltLocalPlayerSettings.luckEnabled);
	GetSliderInt("LocalPlayerSettings", "Luck", customLocalPlayerSettings.luck, defltLocalPlayerSettings.luck, 0, 99999);
}

void Settings::SetLocalPlayerSettings()
{
	SetBool("LocalPlayerSettings", "PositionSpoofingEnabled", customLocalPlayerSettings.positionSpoofingEnabled, defltLocalPlayerSettings.positionSpoofingEnabled);
	SetSliderInt("LocalPlayerSettings", "PositionSpoofingHeight", customLocalPlayerSettings.positionSpoofingHeight, defltLocalPlayerSettings.positionSpoofingHeight, -524287, 524287);
	SetBool("LocalPlayerSettings", "NoclipEnabled", customLocalPlayerSettings.noclipEnabled, defltLocalPlayerSettings.noclipEnabled);
	SetSliderFloat("LocalPlayerSettings", "NoclipSpeed", customLocalPlayerSettings.noclipSpeed, defltLocalPlayerSettings.noclipSpeed, 0.0f, 2.0f);
	SetBool("LocalPlayerSettings", "ClientState", customLocalPlayerSettings.clientState, defltLocalPlayerSettings.clientState);
	SetBool("LocalPlayerSettings", "AutomaticClientState", customLocalPlayerSettings.automaticClientState, defltLocalPlayerSettings.automaticClientState);
	SetBool("LocalPlayerSettings", "FreezeApEnabled", customLocalPlayerSettings.freezeApEnabled, defltLocalPlayerSettings.freezeApEnabled);
	SetBool("LocalPlayerSettings", "ActionPointsEnabled", customLocalPlayerSettings.actionPointsEnabled, defltLocalPlayerSettings.actionPointsEnabled);
	SetSliderInt("LocalPlayerSettings", "ActionPoints", customLocalPlayerSettings.actionPoints, defltLocalPlayerSettings.actionPoints, 0, 99999);
	SetBool("LocalPlayerSettings", "StrengthEnabled", customLocalPlayerSettings.strengthEnabled, defltLocalPlayerSettings.strengthEnabled);
	SetSliderInt("LocalPlayerSettings", "Strength", customLocalPlayerSettings.strength, defltLocalPlayerSettings.strength, 0, 99999);
	SetBool("LocalPlayerSettings", "PerceptionEnabled", customLocalPlayerSettings.perceptionEnabled, defltLocalPlayerSettings.perceptionEnabled);
	SetSliderInt("LocalPlayerSettings", "Perception", customLocalPlayerSettings.perception, defltLocalPlayerSettings.perception, 0, 99999);
	SetBool("LocalPlayerSettings", "EnduranceEnabled", customLocalPlayerSettings.enduranceEnabled, defltLocalPlayerSettings.enduranceEnabled);
	SetSliderInt("LocalPlayerSettings", "Endurance", customLocalPlayerSettings.endurance, defltLocalPlayerSettings.endurance, 0, 99999);
	SetBool("LocalPlayerSettings", "CharismaEnabled", customLocalPlayerSettings.charismaEnabled, defltLocalPlayerSettings.charismaEnabled);
	SetSliderInt("LocalPlayerSettings", "Charisma", customLocalPlayerSettings.charisma, defltLocalPlayerSettings.charisma, 0, 99999);
	SetBool("LocalPlayerSettings", "IntelligenceEnabled", customLocalPlayerSettings.intelligenceEnabled, defltLocalPlayerSettings.intelligenceEnabled);
	SetSliderInt("LocalPlayerSettings", "Intelligence", customLocalPlayerSettings.intelligence, defltLocalPlayerSettings.intelligence, 0, 99999);
	SetBool("LocalPlayerSettings", "AgilityEnabled", customLocalPlayerSettings.agilityEnabled, defltLocalPlayerSettings.agilityEnabled);
	SetSliderInt("LocalPlayerSettings", "Agility", customLocalPlayerSettings.agility, defltLocalPlayerSettings.agility, 0, 99999);
	SetBool("LocalPlayerSettings", "LuckEnabled", customLocalPlayerSettings.luckEnabled, defltLocalPlayerSettings.luckEnabled);
	SetSliderInt("LocalPlayerSettings", "Luck", customLocalPlayerSettings.luck, defltLocalPlayerSettings.luck, 0, 99999);
}

void Settings::GetOpkSettings()
{
	GetBool("OpkSettings", "OpkPlayersEnabled", opk.playersEnabled, false);
	GetBool("OpkSettings", "OpkNpcsEnabled", opk.npcsEnabled, false);
}

void Settings::SetOpkSettings()
{
	SetBool("OpkSettings", "OpkPlayersEnabled", opk.playersEnabled, false);
	SetBool("OpkSettings", "OpkNpcsEnabled", opk.npcsEnabled, false);
}

void Settings::GetUtilitySettings()
{
	GetBool("UtilitySettings", "DebugPlayer", utilities.debugPlayer, false);
	GetBool("UtilitySettings", "DebugEsp", utilities.debugEsp, false);
	GetDword("UtilitySettings", "PtrFormId", utilities.ptrFormId, 0);
	GetDword("UtilitySettings", "AddressFormId", utilities.addressFormId, 0);
}

void Settings::SetUtilitySettings()
{
	SetBool("UtilitySettings", "DebugPlayer", utilities.debugPlayer, false);
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
	GetDword("TransferSettings", "SourceFormId", customTransferSettings.sourceFormId, defltTransferSettings.sourceFormId);
	GetDword("TransferSettings", "DestinationFormId", customTransferSettings.destinationFormId, defltTransferSettings.destinationFormId);
	GetBool("TransferSettings", "UseWhitelist", customTransferSettings.useWhitelist, defltTransferSettings.useWhitelist);
	GetBool("TransferSettings", "UseBlacklist", customTransferSettings.useBlacklist, defltTransferSettings.useBlacklist);
	for (auto i = 0; i < 32; i++)
	{
		GetBool("TransferSettings", fmt::format("Whitelisted{:d}", i), customTransferSettings.whitelisted[i], defltTransferSettings.whitelisted[i]);
		GetDword("TransferSettings", fmt::format("Whitelist{:d}", i), customTransferSettings.whitelist[i], defltTransferSettings.whitelist[i]);
	}
	for (auto i = 0; i < 32; i++)
	{
		GetBool("TransferSettings", fmt::format("Blacklisted{:d}", i), customTransferSettings.blacklisted[i], defltTransferSettings.blacklisted[i]);
		GetDword("TransferSettings", fmt::format("Blacklist{:d}", i), customTransferSettings.blacklist[i], defltTransferSettings.blacklist[i]);
	}
}

void Settings::SetTransferSettings()
{
	SetDword("TransferSettings", "SourceFormId", customTransferSettings.sourceFormId, defltTransferSettings.sourceFormId);
	SetDword("TransferSettings", "DestinationFormId", customTransferSettings.destinationFormId, defltTransferSettings.destinationFormId);
	SetBool("TransferSettings", "UseWhitelist", customTransferSettings.useWhitelist, defltTransferSettings.useWhitelist);
	SetBool("TransferSettings", "UseBlacklist", customTransferSettings.useBlacklist, defltTransferSettings.useBlacklist);
	for (auto i = 0; i < 32; i++)
	{
		SetBool("TransferSettings", fmt::format("Whitelisted{:d}", i), customTransferSettings.whitelisted[i], defltTransferSettings.whitelisted[i]);
		SetDword("TransferSettings", fmt::format("Whitelist{:d}", i), customTransferSettings.whitelist[i], defltTransferSettings.whitelist[i]);
	}
	for (auto i = 0; i < 32; i++)
	{
		SetBool("TransferSettings", fmt::format("Blacklisted{:d}", i), customTransferSettings.blacklisted[i], defltTransferSettings.blacklisted[i]);
		SetDword("TransferSettings", fmt::format("Blacklist{:d}", i), customTransferSettings.blacklist[i], defltTransferSettings.blacklist[i]);
	}
}

void Settings::GetTeleportSettings()
{
	for (auto i = 0; i < 16; i++)
	{
		GetQuadFloat("TeleportSettings", fmt::format("Destination{:d}", i), teleporter.entries[i].destination, nullptr);
		GetDword("TeleportSettings", fmt::format("CellFormId{:d}", i), teleporter.entries[i].cellFormId, 0);
		GetBool("TeleportSettings", fmt::format("DisableSaving{:d}", i), teleporter.entries[i].disableSaving, false);
	}
}

void Settings::SetTeleportSettings()
{
	for (auto i = 0; i < 16; i++)
	{
		SetQuadFloat("TeleportSettings", fmt::format("Destination{:d}", i), teleporter.entries[i].destination, nullptr);
		SetDword("TeleportSettings", fmt::format("CellFormId{:d}", i), teleporter.entries[i].cellFormId, 0);
		SetBool("TeleportSettings", fmt::format("DisableSaving{:d}", i), teleporter.entries[i].disableSaving, false);
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
	GetBool("LegendarySettings", "OverrideLivingOneStar", customLegendarySettings.overrideLivingOneStar, defltLegendarySettings.overrideLivingOneStar);
	GetRgb("LegendarySettings", "LivingOneStarColor", customLegendarySettings.livingOneStarColor, defltLegendarySettings.livingOneStarColor);
	GetBool("LegendarySettings", "OverrideDeadOneStar", customLegendarySettings.overrideDeadOneStar, defltLegendarySettings.overrideDeadOneStar);
	GetRgb("LegendarySettings", "DeadOneStarColor", customLegendarySettings.deadOneStarColor, defltLegendarySettings.deadOneStarColor);
	GetBool("LegendarySettings", "OverrideLivingTwoStar", customLegendarySettings.overrideLivingTwoStar, defltLegendarySettings.overrideLivingTwoStar);
	GetRgb("LegendarySettings", "LivingTwoStarColor", customLegendarySettings.livingTwoStarColor, defltLegendarySettings.livingTwoStarColor);
	GetBool("LegendarySettings", "OverrideDeadTwoStar", customLegendarySettings.overrideDeadTwoStar, defltLegendarySettings.overrideDeadTwoStar);
	GetRgb("LegendarySettings", "DeadTwoStarColor", customLegendarySettings.deadTwoStarColor, defltLegendarySettings.deadTwoStarColor);
	GetBool("LegendarySettings", "OverrideLivingThreeStar", customLegendarySettings.overrideLivingThreeStar, defltLegendarySettings.overrideLivingThreeStar);
	GetRgb("LegendarySettings", "LivingThreeStarColor", customLegendarySettings.livingThreeStarColor, defltLegendarySettings.livingThreeStarColor);
	GetBool("LegendarySettings", "OverrideDeadThreeStar", customLegendarySettings.overrideDeadThreeStar, defltLegendarySettings.overrideDeadThreeStar);
	GetRgb("LegendarySettings", "DeadThreeStarColor", customLegendarySettings.deadThreeStarColor, defltLegendarySettings.deadThreeStarColor);
}

void Settings::SetLegendarySettings()
{
	SetBool("LegendarySettings", "OverrideLivingOneStar", customLegendarySettings.overrideLivingOneStar, defltLegendarySettings.overrideLivingOneStar);
	SetRgb("LegendarySettings", "LivingOneStarColor", customLegendarySettings.livingOneStarColor, defltLegendarySettings.livingOneStarColor);
	SetBool("LegendarySettings", "OverrideDeadOneStar", customLegendarySettings.overrideDeadOneStar, defltLegendarySettings.overrideDeadOneStar);
	SetRgb("LegendarySettings", "DeadOneStarColor", customLegendarySettings.deadOneStarColor, defltLegendarySettings.deadOneStarColor);
	SetBool("LegendarySettings", "OverrideLivingTwoStar", customLegendarySettings.overrideLivingTwoStar, defltLegendarySettings.overrideLivingTwoStar);
	SetRgb("LegendarySettings", "LivingTwoStarColor", customLegendarySettings.livingTwoStarColor, defltLegendarySettings.livingTwoStarColor);
	SetBool("LegendarySettings", "OverrideDeadTwoStar", customLegendarySettings.overrideDeadTwoStar, defltLegendarySettings.overrideDeadTwoStar);
	SetRgb("LegendarySettings", "DeadTwoStarColor", customLegendarySettings.deadTwoStarColor, defltLegendarySettings.deadTwoStarColor);
	SetBool("LegendarySettings", "OverrideLivingThreeStar", customLegendarySettings.overrideLivingThreeStar, defltLegendarySettings.overrideLivingThreeStar);
	SetRgb("LegendarySettings", "LivingThreeStarColor", customLegendarySettings.livingThreeStarColor, defltLegendarySettings.livingThreeStarColor);
	SetBool("LegendarySettings", "OverrideDeadThreeStar", customLegendarySettings.overrideDeadThreeStar, defltLegendarySettings.overrideDeadThreeStar);
	SetRgb("LegendarySettings", "DeadThreeStarColor", customLegendarySettings.deadThreeStarColor, defltLegendarySettings.deadThreeStarColor);
}

void Settings::GetFluxSettings()
{
	GetBool("FluxSettings", "CrimsonFluxEnabled", customFluxSettings.crimsonFluxEnabled, false);
	GetBool("FluxSettings", "CobaltFluxEnabled", customFluxSettings.cobaltFluxEnabled, false);
	GetBool("FluxSettings", "YellowcakeFluxEnabled", customFluxSettings.yellowcakeFluxEnabled, false);
	GetBool("FluxSettings", "FluorescentFluxEnabled", customFluxSettings.fluorescentFluxEnabled, false);
	GetBool("FluxSettings", "VioletFluxEnabled", customFluxSettings.violetFluxEnabled, false);
}

void Settings::SetFluxSettings()
{
	SetBool("FluxSettings", "CrimsonFluxEnabled", customFluxSettings.crimsonFluxEnabled, false);
	SetBool("FluxSettings", "CobaltFluxEnabled", customFluxSettings.cobaltFluxEnabled, false);
	SetBool("FluxSettings", "YellowcakeFluxEnabled", customFluxSettings.yellowcakeFluxEnabled, false);
	SetBool("FluxSettings", "FluorescentFluxEnabled", customFluxSettings.fluorescentFluxEnabled, false);
	SetBool("FluxSettings", "VioletFluxEnabled", customFluxSettings.violetFluxEnabled, false);
}

void Settings::GetKnownRecipeSettings()
{
	GetBool("KnownRecipeSettings", "KnownRecipesEnabled", recipes.knownRecipesEnabled, false);
	GetBool("KnownRecipeSettings", "UnknownRecipesEnabled", recipes.unknownRecipesEnabled, false);
}

void Settings::SetKnownRecipeSettings()
{
	SetBool("KnownRecipeSettings", "KnownRecipesEnabled", recipes.knownRecipesEnabled, false);
	SetBool("KnownRecipeSettings", "UnknownRecipesEnabled", recipes.unknownRecipesEnabled, false);
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

void Settings::GetExtraNpcSettings()
{
	for (auto i = 0; i < 64; i++)
	{
		GetBool("ExtraNPCSettings", fmt::format("NPCBlacklistEnabled{:d}", i), customExtraNpcSettings.npcBlacklistEnabled[i], false);
		GetDword("ExtraNPCSettings", fmt::format("NPCBlacklist{:d}", i), customExtraNpcSettings.npcBlacklist[i], 0);
	}
}

void Settings::SetExtraNpcSettings()
{
	for (auto i = 0; i < 64; i++)
	{
		SetBool("ExtraNPCSettings", fmt::format("NPCBlacklistEnabled{:d}", i), customExtraNpcSettings.npcBlacklistEnabled[i], false);
		SetDword("ExtraNPCSettings", fmt::format("NPCBlacklist{:d}", i), customExtraNpcSettings.npcBlacklist[i], 0);
	}
}

void Settings::GetBitMsgWriterSettings()
{
	GetBool("BitMsgWriter", "AllowMessages", msgWriter.enabled, false);
}

void Settings::SetBitMsgWriterSettings()
{
	SetBool("BitMsgWriter", "AllowMessages", msgWriter.enabled, false);
}

void Settings::ReadIniSettings()
{
	file.read(ini);
	GetOverlaySettingsA("PlayerSettings", &playerSettings, &defltPlayerSettings);
	GetOverlaySettingsA("NpcSettings", &npcSettings, &defltNpcSettings);
	GetLegendarySettings();
	GetExtraNpcSettings();
	GetOverlaySettingsB("ContainerSettings", &containerSettings, &defltOverlaySettingsB);
	GetOverlaySettingsB("JunkSettings", &junkSettings, &defltOverlaySettingsB);
	GetOverlaySettingsB("PlanSettings", &planSettings, &defltOverlaySettingsB);
	GetKnownRecipeSettings();
	GetOverlaySettingsB("MagazineSettings", &magazineSettings, &defltOverlaySettingsB);
	GetOverlaySettingsB("BobbleheadSettings", &bobbleheadSettings, &defltOverlaySettingsB);
	GetOverlaySettingsB("ItemSettings", &itemSettings, &defltOverlaySettingsB);
	GetOverlaySettingsB("FloraSettings", &floraSettings, &floraSettings);
	GetFluxSettings();
	GetOverlaySettingsB("EntitySettings", &entitySettings, &defltOverlaySettingsB);
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

	GetInfoBoxSettings();

	file.write(ini, true);
}

void Settings::WriteIniSettings()
{
	file.read(ini);
	SetOverlaySettingsA("PlayerSettings", &playerSettings, &defltPlayerSettings);
	SetOverlaySettingsA("NpcSettings", &npcSettings, &defltNpcSettings);
	SetLegendarySettings();
	SetExtraNpcSettings();
	SetOverlaySettingsB("ContainerSettings", &containerSettings, &defltOverlaySettingsB);
	SetOverlaySettingsB("JunkSettings", &junkSettings, &defltOverlaySettingsB);
	SetOverlaySettingsB("PlanSettings", &planSettings, &defltOverlaySettingsB);
	SetKnownRecipeSettings();
	SetOverlaySettingsB("MagazineSettings", &magazineSettings, &defltOverlaySettingsB);
	SetOverlaySettingsB("BobbleheadSettings", &bobbleheadSettings, &defltOverlaySettingsB);
	SetOverlaySettingsB("ItemSettings", &itemSettings, &defltOverlaySettingsB);
	SetOverlaySettingsB("FloraSettings", &floraSettings, &floraSettings);
	SetFluxSettings();
	SetOverlaySettingsB("EntitySettings", &entitySettings, &defltOverlaySettingsB);
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

	SetInfoBoxSettings();

	file.write(ini, true);
}
