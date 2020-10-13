#pragma once
#include <map>
#include <string>
#include <unordered_map>

#include "common.h"
#include "utils.h"

struct LooterSettings
{
	enum class Mode
	{
		Disabled,
		Auto,
		Keybind,
	} mode{};

	struct Looters {
		bool npcs;
		bool groundItems;
		bool containers;
		bool flora;
	} looters{};

	struct Selection {
		struct WeaponsAndArmor
		{
			bool all;
			bool oneStar;
			bool twoStar;
			bool threeStar;

			[[nodiscard]] bool IsEnabled() const { return all || oneStar || twoStar || threeStar; }
		} weapons{}, apparel{};

		struct Aid
		{
			bool all;
			bool magazines;
			bool bobbleheads;

			[[nodiscard]] bool IsEnabled() const { return all || magazines || bobbleheads; }
		} aid{};

		struct Misc
		{
			bool all;

			[[nodiscard]] bool IsEnabled() const { return all; }
		} misc{};

		struct Holo
		{
			bool all;

			[[nodiscard]] bool IsEnabled() const { return all; }
		} holo{};

		struct Notes
		{
			bool all;
			bool treasureMaps;
			bool plansKnown;
			bool plansUnknown;

			[[nodiscard]] bool IsEnabled() const { return all || treasureMaps || plansKnown || plansUnknown; }
		} notes{};

		struct Junk
		{
			bool all;

			std::unordered_map<std::uint32_t, bool>	components = []() {
				std::unordered_map<std::uint32_t, bool> result(JUNK_COMPONENT_NAMES.size());
				for (const auto& [formId, isEnabled] : JUNK_COMPONENT_NAMES)
					result.emplace(formId, false);
				return result;
			}();

			[[nodiscard]] bool IsEnabled() const { return all || std::find_if(components.begin(), components.end(), [](const auto& element) { return element.second == true; }) != components.end(); }
		} junk{};

		struct Flora
		{
			bool all;

			std::unordered_map<std::uint32_t, bool>	components = []() {
				std::unordered_map<std::uint32_t, bool> result(FLORA_COMPONENT_NAMES.size());
				for (const auto& [formId, isEnabled] : FLORA_COMPONENT_NAMES)
					result.emplace(formId, false);
				return result;
			}();

			[[nodiscard]] bool IsEnabled() const { return all || std::find_if(components.begin(), components.end(), [](const auto& element) { return element.second == true; }) != components.end(); }
		} flora{};

		struct Mods
		{
			bool all;

			[[nodiscard]] bool IsEnabled() const { return all; }
		} mods{};

		struct Ammo
		{
			bool all;

			[[nodiscard]] bool IsEnabled() const { return all; }
		} ammo{};

		struct Other
		{
			bool caps;

			[[nodiscard]] bool IsEnabled() const { return caps; }
		} other{};

		std::map<std::uint32_t, bool> whitelist, blacklist;

		[[nodiscard]] bool IsEnabled() const {
			return weapons.IsEnabled()
				|| apparel.IsEnabled()
				|| aid.IsEnabled()
				|| misc.IsEnabled()
				|| holo.IsEnabled()
				|| notes.IsEnabled()
				|| junk.IsEnabled()
				|| flora.IsEnabled()
				|| mods.IsEnabled()
				|| ammo.IsEnabled()
				|| other.IsEnabled()
				|| std::find_if(whitelist.begin(), whitelist.end(), [](const auto& element) { return element.second == true; }) != whitelist.end()
				|| std::find_if(blacklist.begin(), blacklist.end(), [](const auto& element) { return element.second == true; }) != blacklist.end();
		}
	} selection;
};
struct EspSettings
{
	struct Actors
	{
		bool enabled = true;
		int enabledDistance = 1500;
		bool drawAlive = true;
		float aliveColor[3] = { 0.0f, 1.0f, 0.0f };
		bool drawDowned = true;
		float downedColor[3] = { 1.0f, 1.0f, 0.0f };
		bool drawDead = false;
		float deadColor[3] = { 1.0f, 0.0f, 0.0f };
		bool drawUnknown = false;
		float unknownColor[3] = { 1.0f, 0.5f, 0.0f };
		bool drawEnabled = true;
		float enabledAlpha = 1.f;
		bool drawDisabled = false;
		float disabledAlpha = 0.5f;
		bool drawNamed = true;
		bool drawUnnamed = false;
		bool showName = true;
		bool showDistance = true;
		bool showHealth = true;
		bool showDeadHealth = false;
	} players, npcs;

	struct Items
	{
		bool enabled = false;
		int enabledDistance = 1500;
		float color[3] = { 1.0f, 1.0f, 1.0f };
		bool drawEnabled = true;
		float enabledAlpha = 1.f;
		bool drawDisabled = false;
		float disabledAlpha = 0.5f;
		bool drawNamed = true;
		bool drawUnnamed = false;
		bool showName = true;
		bool showDistance = true;
	} junk, containers, plans, magazines, bobbleheads, items, flora, entities;

	struct NpcsExt
	{
		bool overrideLivingOneStar = true;
		float livingOneStarColor[3] = { 0.7f, 1.0f, 0.7f };
		bool overrideDeadOneStar = true;
		float deadOneStarColor[3] = { 1.0f, 0.5f, 0.5f };
		bool overrideLivingTwoStar = true;
		float livingTwoStarColor[3] = { 0.7f, 1.0f, 0.7f };
		bool overrideDeadTwoStar = true;
		float deadTwoStarColor[3] = { 1.0f, 0.5f, 0.5f };
		bool overrideLivingThreeStar = true;
		float livingThreeStarColor[3] = { 0.7f, 1.0f, 0.7f };
		bool overrideDeadThreeStar = true;
		float deadThreeStarColor[3] = { 1.0f, 0.5f, 0.5f };
	} npcsExt;

	struct FloraExt
	{
		bool crimsonFluxEnabled = true;
		bool cobaltFluxEnabled = true;
		bool yellowcakeFluxEnabled = true;
		bool fluorescentFluxEnabled = true;
		bool violetFluxEnabled = true;
	} floraExt;

	struct PlansExt
	{
		bool knownRecipesEnabled = true;
		bool unknownRecipesEnabled = true;
	} plansExt;

	struct InfoBoxSettings
	{
		bool drawPlayerInfo = false;
		bool drawPositionSpoofingStatus = false;
		bool drawNukeCodes = false;
		bool drawFps = false;
	} infobox;

	std::map<DWORD, bool> whitelist, blacklist;
};

//utils
class WeaponEditorSettings
{
public:
	bool infiniteAmmo = false;

	bool noRecoil = false;
	bool noSpread = false;
	bool noSway = false;
	bool instantReload = false;
	bool automaticflag = false;
	bool capacityEnabled = false;
	int capacity = 250;
	bool speedEnabled = false;
	float speed = 1.f;
	bool reachEnabled = false;
	float reach = 1.f;
	bool rangeEnabled = false;
	float range = 256;

	[[nodiscard]] bool IsEnabled() const { return noRecoil || noSpread || noSway || instantReload || automaticflag || capacityEnabled || speedEnabled || reachEnabled || rangeEnabled; }//ignoring infiniteAmmo on purpose!!!
};
class TargetingSettings
{
public:
	bool lockNpCs = true;
	bool dmgRedirect = true;
	bool dmgSend = true;
	bool targetUnknown = false;
	float lockingFov = 20.0f;
	bool ignoreEssentialNpcs = true;
	bool ignoreNonHostileNpcs = true;
	float lockedColor[3] = { 1.0f, 0.0f, 1.0f };
	bool retargeting = false;
	int cooldown = 1;
	int sendDamageMin = 1;
	int sendDamageMax = 1;
	int favoriteIndex = 12;
};
class PlayerEditorSettings
{
public:
	bool positionSpoofingEnabled = false;
	int positionSpoofingHeight = 524287;
	bool noclipEnabled = false;
	float noclipSpeed = 0.1f;
	bool clientState = false;
	bool automaticClientState = false;
	bool freezeApEnabled = false;
	bool actionPointsEnabled = false;
	int actionPoints = 9001;
	bool strengthEnabled = false;
	int strength = 9001;
	bool perceptionEnabled = false;
	int perception = 9001;
	bool enduranceEnabled = false;
	int endurance = 9001;
	bool charismaEnabled = false;
	int charisma = 9001;
	bool intelligenceEnabled = false;
	int intelligence = 9001;
	bool agilityEnabled = false;
	int agility = 9001;
	bool luckEnabled = false;
	int luck = 9001;

	[[nodiscard]] bool IsEnabled() const { return actionPointsEnabled || strengthEnabled || perceptionEnabled || enduranceEnabled || charismaEnabled || intelligenceEnabled || agilityEnabled || luckEnabled; }
};
class OpkSettings
{
public:
	bool enabled;
};
class UtilitySettings
{
public:
	bool debugEsp;
	std::uint32_t ptrFormId;
	std::uint32_t addressFormId;
};
class SwapperSettings
{
public:
	std::uint32_t sourceFormId;
	std::uint32_t destinationFormId;
};
class TransferSettings
{
public:
	std::uint32_t sourceFormId = 0;
	std::uint32_t destinationFormId = 0;
	bool useWhitelist = true;
	bool useBlacklist = true;
	bool whitelisted[32] = {};
	std::uint32_t whitelist[32] = {};
	bool blacklisted[32] = {
		true, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false,
	};
	std::uint32_t blacklist[32] = {
		0x00021B3B, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
	};
};

class TeleporterSettings
{
public:
	struct Entry {
		Vector3 position;
		Vector3 rotation;
		std::uint32_t cellFormId;
		bool disableSaving;
	} entries[16];
};

class NukeCodeSettings
{
public:
	bool automaticNukeCodes;
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
class MessageWriterSettings
{
public:
	bool enabled;
};

class Settings final {
public:
	static void Read();
	static void Write();

	inline static EspSettings esp = {};
	inline static LooterSettings looter = {};
	inline static MessageWriterSettings msgWriter = {};

	//utils
	inline static WeaponEditorSettings weapons = {};
	inline static TargetingSettings targetting = {};
	inline static PlayerEditorSettings localPlayer = {};
	inline static OpkSettings opk = {};
	inline static UtilitySettings utilities = {};
	inline static SwapperSettings swapper = {};
	inline static TransferSettings customTransferSettings = {};
	inline static TeleporterSettings teleporter = {};
	inline static NukeCodeSettings customNukeCodeSettings = {};
	inline static MeleeSettings melee = { false,10,20 };
	inline static ChargenSettings characterEditor = { false,0.33f,0.33f,0.33f };

private:
	static void GetItemSettings(const std::string& section, EspSettings::Items& value, const EspSettings::Items& deflt);
	static void SetItemSettings(const std::string& section, EspSettings::Items& value, const EspSettings::Items& deflt);

	static void GetEspSettings();
	static void SetEspSettings();
	static void GetInfoBoxSettings();
	static void SetInfoBoxSettings();
	static void GetLooterSettings();
	static void SetLooterSettings();

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
	static void GetKnownRecipeSettings();
	static void SetKnownRecipeSettings();
	static void GetMeleeSettings();
	static void SetMeleeSettings();
	static void GetChargenSettings();
	static void SetChargenSettings();
	static void GetBitMsgWriterSettings();
	static void SetBitMsgWriterSettings();

	static void GetDword(const std::string& section, const std::string& key, std::uint32_t& value, std::uint32_t deflt);
	static void SetDword(const std::string& section, const std::string& key, std::uint32_t value, std::uint32_t deflt);
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
	static void GetBool(const std::string& section, const std::string& key, bool& value, bool deflt);
	static void SetBool(const std::string& section, const std::string& key, const bool& value, bool deflt);
	static void GetActorSettings(const std::string& section, EspSettings::Actors& value, const EspSettings::Actors& deflt);
	static void SetActorSettings(const std::string& section, EspSettings::Actors& value, const EspSettings::Actors& deflt);

	virtual void Dummy() = 0;
};
