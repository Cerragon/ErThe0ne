#pragma once

#include <Windows.h>
#include <unordered_map>

//overlay
constexpr LPCSTR OVERLAY_WINDOW_NAME = "Er";
constexpr LPCSTR OVERLAY_WINDOW_CLASS = "ErCLS";

//Offsets

//just rpm
constexpr auto OFFSET_LOCAL_PLAYER = 0x05B1D518UL;//1.3.2.10
constexpr auto OFFSET_ENTITY_LIST = 0x05902898UL;//1.3.2.10
constexpr auto OFFSET_MAIN = 0x05B7B4F8UL;//1.3.2.10
constexpr auto OFFSET_CAMERA = 0x06B1F760UL;//1.3.2.10
constexpr auto OFFSET_GET_PTR_A1 = 0x05908C68UL;//1.3.2.10
constexpr auto OFFSET_GET_PTR_A2 = 0x03C92020UL;//1.3.2.10
constexpr auto OFFSET_NUKE_CODE = 0x05A3F078UL;//1.3.2.10

//wpm
constexpr auto OFFSET_CHARGEN = 0x05B1F9A0UL;//1.3.2.10 + 0x05B0CD60UL?

//remote threads and/or function patching
constexpr auto OFFSET_MESSAGE_SENDER = 0x0147D8D0UL;//1.3.2.10
constexpr auto OFFSET_FAKE_MESSAGE = 0x013F9017UL;//1.3.2.10
constexpr auto OFFSET_REDIRECTION = 0x017F52FBUL;//1.3.2.10 + 0x017FAD46UL?
constexpr auto OFFSET_REDIRECTION_JMP = 0x017F4F4DUL;//1.3.2.10 + 0x017F515DUL?
constexpr auto OFFSET_ENTITY_ID = 0x050F9C80UL;//1.3.2.10 +
constexpr auto OFFSET_ACTOR_VALUE = 0x0167C230UL;//1.3.2.10 +
constexpr auto OFFSET_MELEE_ATTACK = 0x01685BE0UL;//1.3.2.10 +
constexpr auto OFFSET_OPK = 0x0298374DUL;//1.3.2.10 +
constexpr auto OFFSET_AV_REGEN = 0x0167C5EFUL;//1.3.2.10 +
constexpr auto OFFSET_SERVER_POSITION = 0x01469D25UL;//1.3.2.10 + 0x0146B121UL?
constexpr auto OFFSET_CHAR_CONTROLLER = 0x05D6E120UL;//1.3.2.10
constexpr auto OFFSET_NOCLIP_A = 0x02975798UL;//1.3.2.10 +
constexpr auto OFFSET_NOCLIP_B = 0x02975750UL;//1.3.2.10 + 0x02965510UL?
constexpr auto OFFSET_NOCLIP_C = 0x015F9441UL;//1.3.2.10 + 0x01602D91UL?
constexpr auto OFFSET_NOCLIP_D = 0x015F9471UL;//1.3.2.10 + 0x01602DC1UL?
constexpr auto OFFSET_INFINITE_AMMO = 0x00CB4316UL;//1.3.2.10 +

constexpr auto OFFSET_DATA_HANDLER = 0x058FEFE8UL;//1.3.2.10

//vtables
constexpr auto VTABLE_REQUESTACTIVATEREFMSG = 0x03AD5C70UL;//1.3.2.10
constexpr auto VTABLE_REQUESTTRANSFERITEMMSG = 0x03AEA2B8UL;//1.3.2.10
constexpr auto VTABLE_REQUESTTELEPORTTOLOCATIONMSG = 0x03ADE020UL;//1.3.2.10
constexpr auto VTABLE_CLIENTSTATEMSG = 0x03ADDC00UL;//1.3.2.10
constexpr auto VTABLE_REQUESTHITSONACTORS = 0x03AD58A0UL;//1.3.2.10



enum class HotKeys : int
{
	NpcLooterToggle = 1,
	ContainerLooterToggle,
	HarvesterToggle,
	PositionSpoofingToggle,
	NoclipToggle,
	OpkPlayersToggle,
	OpkNpcsToggle,
	LootItems,
	LootScrap,
	ToggleOverlay,
};

struct HotkeyCombination
{
	UINT modifiers;
	UINT vk;
};

const std::unordered_map<HotKeys, HotkeyCombination> HOTKEYS{
	{ HotKeys::NpcLooterToggle, HotkeyCombination{MOD_CONTROL | MOD_NOREPEAT, VK_OEM_COMMA } },
	{ HotKeys::ContainerLooterToggle, HotkeyCombination{MOD_CONTROL | MOD_NOREPEAT, VK_OEM_PERIOD } },
	{ HotKeys::HarvesterToggle, HotkeyCombination{MOD_CONTROL | MOD_NOREPEAT, 'P' } },
	{ HotKeys::PositionSpoofingToggle, HotkeyCombination{MOD_CONTROL | MOD_NOREPEAT, 'L' } },
	{ HotKeys::NoclipToggle, HotkeyCombination{ MOD_CONTROL | MOD_NOREPEAT, 'Y' } },
	{ HotKeys::OpkPlayersToggle, HotkeyCombination{ MOD_CONTROL | MOD_NOREPEAT, 'B' } },
	{ HotKeys::OpkNpcsToggle, HotkeyCombination{ MOD_CONTROL | MOD_NOREPEAT, 'N' } },
	{ HotKeys::LootItems, HotkeyCombination{ MOD_CONTROL | MOD_NOREPEAT, 'R' } },
	{ HotKeys::LootScrap, HotkeyCombination{ MOD_CONTROL | MOD_NOREPEAT, 'E' } },
	{ HotKeys::ToggleOverlay, HotkeyCombination{ MOD_CONTROL | MOD_NOREPEAT, VK_RETURN } }
};


constexpr auto CUSTOM_ENTRY_DEFAULT = 0x0000000000000000ULL;
constexpr auto CUSTOM_ENTRY_UNNAMED = 0x0000000000000001ULL;
constexpr auto CUSTOM_ENTRY_PLAYER = 0x0000000000000002ULL;
constexpr auto CUSTOM_ENTRY_NPC = 0x0000000000000004ULL;
constexpr auto CUSTOM_ENTRY_CONTAINER = 0x0000000000000008ULL;
constexpr auto CUSTOM_ENTRY_JUNK = 0x0000000000000010ULL;
constexpr auto CUSTOM_ENTRY_PLAN = 0x0000000000000020ULL;
constexpr auto CUSTOM_ENTRY_ITEM = 0x0000000000000040ULL;
constexpr auto CUSTOM_ENTRY_ENTITY = 0x0000000000000080ULL;
constexpr auto CUSTOM_ENTRY_VALID_SCRAP = 0x0000000000000100ULL;
constexpr auto CUSTOM_ENTRY_VALID_ITEM = 0x0000000000000200ULL;
constexpr auto CUSTOM_ENTRY_MAGAZINE = 0x0000000000000400ULL;
constexpr auto CUSTOM_ENTRY_BOBBLEHEAD = 0x0000000000000800ULL;
constexpr auto CUSTOM_ENTRY_FLORA = 0x0000000000001000ULL;
constexpr auto CUSTOM_ENTRY_MISC = 0x0000000000002000ULL;
constexpr auto CUSTOM_ENTRY_MOD = 0x0000000000004000ULL;
constexpr auto CUSTOM_ENTRY_WEAPON = 0x0000000000008000ULL;
constexpr auto CUSTOM_ENTRY_ARMOR = 0x0000000000010000ULL;
constexpr auto CUSTOM_ENTRY_AMMO = 0x0000000000020000ULL;
constexpr auto CUSTOM_ENTRY_AID = 0x0000000000040000ULL;
constexpr auto CUSTOM_ENTRY_VALID_INGREDIENT = 0x0000000000080000ULL;
constexpr auto CUSTOM_ENTRY_KNOWN_RECIPE = 0x0000000000100000ULL;
constexpr auto CUSTOM_ENTRY_UNKNOWN_RECIPE = 0x0000000000200000ULL;
constexpr auto CUSTOM_ENTRY_TREASURE_MAP = 0x0000000000800000ULL;
constexpr auto CUSTOM_ENTRY_WHITELISTED = 0x4000000000000000ULL;
constexpr auto CUSTOM_ENTRY_INVALID = 0x8000000000000000ULL;
