#pragma once
#include <Windows.h>
#include <string>

class InfoBoxSettings
{
public:
	bool drawScrapLooterStatus;
	bool drawItemLooterStatus;
	bool drawNpcLooterStatus;
	bool drawContainerLooterStatus;
	bool drawHarvesterStatus;
	bool drawPositionSpoofingStatus;

	bool drawNukeCodes;
};
class OverlaySettingsA
{
public:
	bool enabled;
	int enabledDistance;
	bool drawAlive;
	float aliveColor[3];
	bool drawDowned;
	float downedColor[3];
	bool drawDead;
	float deadColor[3];
	bool drawUnknown;
	float unknownColor[3];
	bool drawEnabled;
	float enabledAlpha;
	bool drawDisabled;
	float disabledAlpha;
	bool drawNamed;
	bool drawUnnamed;
	bool showName;
	bool showDistance;
	bool showHealth;
	bool showDeadHealth;
	bool textCentered;
	bool textShadowed;
};

class OverlaySettingsB
{
public:
	bool enabled;
	int enabledDistance;
	float color[3];
	bool drawEnabled;
	float enabledAlpha;
	bool drawDisabled;
	float disabledAlpha;
	bool drawNamed;
	bool drawUnnamed;
	bool showName;
	bool showDistance;
	bool textCentered;
	bool textShadowed;
	bool whitelisted[32];
	DWORD whitelist[32];
};

class ScrapLooterSettings
{
public:
	bool keybindEnabled;
	bool scrapOverrideEnabled;
	bool autoLootingEnabled;
	int autoLootingSpeedMin;
	int autoLootingSpeedMax;
	int maxDistance;
	bool enabledList[40];
	DWORD formIdList[40];
	const char* nameList[40];
};

class ItemLooterSettings
{
public:
	bool keybindEnabled;
	bool autoLootingEnabled;
	int autoLootingSpeedMin;
	int autoLootingSpeedMax;
	bool lootWeaponsEnabled;
	int lootWeaponsDistance;
	bool lootArmorEnabled;
	int lootArmorDistance;
	bool lootAmmoEnabled;
	int lootAmmoDistance;
	bool lootModsEnabled;
	int lootModsDistance;
	bool lootMagazinesEnabled;
	int lootMagazinesDistance;
	bool lootBobbleheadsEnabled;
	int lootBobbleheadsDistance;
	bool lootAidEnabled;
	int lootAidDistance;
	bool lootKnownPlansEnabled;
	int lootKnownPlansDistance;
	bool lootUnknownPlansEnabled;
	int lootUnknownPlansDistance;
	bool lootMiscEnabled;
	int lootMiscDistance;
	bool lootUnlistedEnabled;
	int lootUnlistedDistance;
	bool lootListEnabled;
	int lootListDistance;
	bool blacklistToggle;
	bool enabledList[100];
	DWORD formIdList[100];
	bool blacklistEnabled[64];
	DWORD blacklist[64];
};

class WeaponSettings
{
public:
	bool noRecoil;
	bool noSpread;
	bool noSway;
	bool infiniteAmmo;
	bool instantReload;
	bool automaticflag;
	bool capacityEnabled;
	int capacity;
	bool speedEnabled;
	float speed;
	bool reachEnabled;
	float reach;
};

class TargetSettings
{
public:
	bool lockPlayers;
	bool lockNpCs;
	bool indirectPlayers;
	bool indirectNpCs;
	bool directPlayers;
	bool directNpCs;
	bool targetLiving;
	bool targetDowned;
	bool targetDead;
	bool targetUnknown;
	bool ignoreRenderDistance;
	float lockingFov;
	bool ignoreEssentialNpCs;
	float lockingColor[3];
	bool retargeting;
	int cooldown;
	int sendDamageMin;
	int sendDamageMax;
	int favoriteIndex;
};

class LocalPlayerSettings
{
public:
	bool positionSpoofingEnabled;
	int positionSpoofingHeight;
	bool noclipEnabled;
	float noclipSpeed;
	bool clientState;
	bool automaticClientState;
	bool freezeApEnabled;
	bool actionPointsEnabled;
	int actionPoints;
	bool strengthEnabled;
	int strength;
	bool perceptionEnabled;
	int perception;
	bool enduranceEnabled;
	int endurance;
	bool charismaEnabled;
	int charisma;
	bool intelligenceEnabled;
	int intelligence;
	bool agilityEnabled;
	int agility;
	bool luckEnabled;
	int luck;
};

class OpkSettings
{
public:
	bool playersEnabled;
	bool npcsEnabled;
};

class UtilitySettings
{
public:
	bool debugPlayer;
	bool debugEsp;
	DWORD ptrFormId;
	DWORD addressFormId;
};

class SwapperSettings
{
public:
	DWORD sourceFormId;
	DWORD destinationFormId;
};

class TransferSettings
{
public:
	DWORD sourceFormId;
	DWORD destinationFormId;
	bool useWhitelist;
	bool useBlacklist;
	bool whitelisted[32];
	DWORD whitelist[32];
	bool blacklisted[32];
	DWORD blacklist[32];
};

class TeleportEntry
{
public:
	float destination[4];
	DWORD cellFormId;
	bool disableSaving;
};

class TeleportSettings
{
public:
	TeleportEntry entries[16];
};

class NukeCodeSettings
{
public:
	bool automaticNukeCodes;
};

class LegendarySettings
{
public:
	bool overrideLivingOneStar;
	float livingOneStarColor[3];
	bool overrideDeadOneStar;
	float deadOneStarColor[3];
	bool overrideLivingTwoStar;
	float livingTwoStarColor[3];
	bool overrideDeadTwoStar;
	float deadTwoStarColor[3];
	bool overrideLivingThreeStar;
	float livingThreeStarColor[3];
	bool overrideDeadThreeStar;
	float deadThreeStarColor[3];
};

class FluxSettings
{
public:
	bool crimsonFluxEnabled;
	bool cobaltFluxEnabled;
	bool yellowcakeFluxEnabled;
	bool fluorescentFluxEnabled;
	bool violetFluxEnabled;
};

class EntityLooterSettings
{
public:
	bool enabled;
	bool oneStarWeaponsEnabled;
	bool oneStarArmorEnabled;
	bool twoStarWeaponsEnabled;
	bool twoStarArmorEnabled;
	bool threeStarWeaponsEnabled;
	bool threeStarArmorEnabled;
	bool allWeaponsEnabled;
	bool allArmorEnabled;
	bool ammoEnabled;
	bool modsEnabled;
	bool capsEnabled;
	bool junkEnabled;
	bool aidEnabled;
	bool treasureMapsEnabled;
	bool knownPlansEnabled;
	bool unknownPlansEnabled;
	bool miscEnabled;
	bool unlistedEnabled;
	bool listEnabled;
	bool blacklistToggle;
	bool enabledList[100];
	DWORD formIdList[100];
	bool blacklistEnabled[64];
	DWORD blacklist[64];
};

class HarvesterSettings
{
public:
	bool enabled;
	bool overrideEnabled;
	bool enabledList[69];
	DWORD formIdList[69];
	const char* nameList[69];
};

class KnownRecipeSettings
{
public:
	bool knownRecipesEnabled;
	bool unknownRecipesEnabled;
};

class MeleeSettings
{
public:
	bool enabled;
	int speedMin;
	int speedMax;
};

class ChargenSettings
{
public:
	bool enabled;
	float thin;
	float muscular;
	float large;
};

class ExtraNpcSettings
{
public:
	bool hideSettlerFaction;
	bool hideCraterRaiderFaction;
	bool hideDieHardFaction;
	bool hideSecretServiceFaction;
	bool useNpcBlacklist;
	bool npcBlacklistEnabled[64];
	DWORD npcBlacklist[64];
};

class MessageWriterSettings
{
public:
	bool enabled;
};

class Settings final {
public:
	static void ReadIniSettings();
	static void WriteIniSettings();

	static OverlaySettingsA playerSettings;
	static OverlaySettingsB junkSettings;
	static OverlaySettingsA npcSettings;
	static OverlaySettingsB containerSettings;
	static OverlaySettingsB planSettings;
	static OverlaySettingsB magazineSettings;
	static OverlaySettingsB bobbleheadSettings;
	static OverlaySettingsB itemSettings;
	static OverlaySettingsB floraSettings;
	static OverlaySettingsB entitySettings;

	static ScrapLooterSettings scrapLooter;
	static ItemLooterSettings itemLooter;
	static WeaponSettings weapons;
	static TargetSettings targetting;
	static LocalPlayerSettings customLocalPlayerSettings;
	static OpkSettings opk;
	static UtilitySettings utilities;
	static SwapperSettings swapper;
	static TransferSettings customTransferSettings;
	static TeleportSettings teleporter;
	static NukeCodeSettings customNukeCodeSettings;
	static LegendarySettings customLegendarySettings;
	static FluxSettings customFluxSettings;
	static EntityLooterSettings npcLooter;
	static EntityLooterSettings containerLooter;
	static HarvesterSettings harvester;
	static KnownRecipeSettings recipes;
	static MeleeSettings melee;
	static ChargenSettings characterEditor;
	static ExtraNpcSettings customExtraNpcSettings;

	inline static InfoBoxSettings infobox = {};
	inline static MessageWriterSettings msgWriter = {};

	inline static WeaponSettings defaultWeaponSettings{
		false, false, false, false, false, false, false, 250, false, 2.0f, false, 500.0f
	};

private:
	static void GetOverlaySettingsB(const std::string& section, OverlaySettingsB* value, const OverlaySettingsB* deflt);
	static void SetOverlaySettingsB(const std::string& section, OverlaySettingsB* value, const OverlaySettingsB* deflt);
	static void GetInfoBoxSettings();
	static void SetInfoBoxSettings();
	static void GetScrapSettings();
	static void SetScrapSettings();
	static void GetItemLooterSettings();
	static void SetItemLooterSettings();
	static void GetWeaponSettings();
	static void SetWeaponSettings();
	static void GetTargetSettings();
	static void SetTargetSettings();
	static void GetLocalPlayerSettings();
	static void SetLocalPlayerSettings();
	static void GetOpkSettings();
	static void SetOpkSettings();
	static void GetUtilitySettings();
	static void SetUtilitySettings();
	static void GetSwapperSettings();
	static void SetSwapperSettings();
	static void GetTransferSettings();
	static void SetTransferSettings();
	static void GetTeleportSettings();
	static void SetTeleportSettings();
	static void GetNukeCodeSettings();
	static void SetNukeCodeSettings();
	static void GetLegendarySettings();
	static void SetLegendarySettings();
	static void GetFluxSettings();
	static void SetFluxSettings();
	static void GetEntityLooterSettings(const std::string& section, EntityLooterSettings* value, const EntityLooterSettings* deflt);
	static void SetEntityLooterSettings(const std::string& section, EntityLooterSettings* value, const EntityLooterSettings* deflt);
	static void GetHarvesterSettings();
	static void SetHarvesterSettings();
	static void GetKnownRecipeSettings();
	static void SetKnownRecipeSettings();
	static void GetMeleeSettings();
	static void SetMeleeSettings();
	static void GetChargenSettings();
	static void SetChargenSettings();
	static void GetExtraNpcSettings();
	static void SetExtraNpcSettings();
	static void GetBitMsgWriterSettings();
	static void SetBitMsgWriterSettings();

	static void GetDword(const std::string& section, const std::string& key, DWORD& value, DWORD deflt);
	static void SetDword(const std::string& section, const std::string& key, DWORD value, DWORD deflt);
	static void GetInt(const std::string& section, const std::string& key, int& value, int deflt);
	static void SetInt(const std::string& section, const std::string& key, int value, int deflt);
	static void GetSliderInt(const std::string& section, const std::string& key, int& value, int deflt, int min, int max);
	static void SetSliderInt(const std::string& section, const std::string& key, int& value, int deflt, int min, int max);
	static void GetFloat(const std::string& section, const std::string& key, float& value, float deflt);
	static void SetFloat(const std::string& section, const std::string& key, float value, float deflt);
	static void GetSliderFloat(const std::string& section, const std::string& key, float& value, float deflt, float min, float max);
	static void SetSliderFloat(const std::string& section, const std::string& key, float& value, float deflt, float min, float max);
	static void GetRgb(const std::string& section, const std::string& key, float* value, const float* deflt);
	static void SetRgb(const std::string& section, const std::string& key, float* value, const float* deflt);
	static void GetQuadFloat(const std::string& section, const std::string& key, float* value, const float* deflt);
	static void SetQuadFloat(const std::string& section, const std::string& key, float* value, const float* deflt);
	static void GetBool(const std::string& section, const std::string& key, bool& value, bool deflt);
	static void SetBool(const std::string& section, const std::string& key, const bool& value, bool deflt);
	static void GetOverlaySettingsA(const std::string& section, OverlaySettingsA* value, const OverlaySettingsA* deflt);
	static void SetOverlaySettingsA(const std::string& section, OverlaySettingsA* value, const OverlaySettingsA* deflt);

	virtual void __dummy();
};