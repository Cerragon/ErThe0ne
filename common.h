#pragma once

#include <Windows.h>
#include <unordered_map>

//overlay
constexpr LPCSTR OVERLAY_WINDOW_NAME = "Er";
constexpr LPCSTR OVERLAY_WINDOW_CLASS = "ErCLS";

//Offsets
constexpr auto OFFSET_LOCAL_PLAYER = 0x05B49F78UL;//1.3.1.26;
constexpr auto OFFSET_ENTITY_LIST = 0x05923C48UL;//1.3.1.26;
constexpr auto OFFSET_MAIN = 0x05BAA290UL;//1.3.1.26;
constexpr auto OFFSET_CAMERA = 0x06B54E68UL;//1.3.1.26;
constexpr auto OFFSET_CHAR_CONTROLLER = 0x05DA26F0UL;//1.3.1.26;
constexpr auto OFFSET_GET_PTR_A1 = 0x0592A7A8UL;//1.3.1.26;
constexpr auto OFFSET_GET_PTR_A2 = 0x03CE0F90UL;//1.3.1.26;
constexpr auto OFFSET_MESSAGE_SENDER = 0x014AA5E0UL;//1.3.1.26;
constexpr auto OFFSET_FAKE_MESSAGE = 0x0142696EUL;//1.3.1.26;
constexpr auto OFFSET_DATA_HANDLER = 0x0591F750UL;//1.3.1.26;
constexpr auto OFFSET_INFINITE_AMMO = 0x00D19A26UL;//1.3.1.26;
constexpr auto OFFSET_REDIRECTION = 0x0181A826UL;//1.3.1.26;
constexpr auto OFFSET_REDIRECTION_JMP = 0x0181A694UL;//1.3.1.26;
constexpr auto OFFSET_NOCLIP_A = 0x02985978UL;//1.3.1.26;
constexpr auto OFFSET_NOCLIP_B = 0x02985930UL;//1.3.1.26;
constexpr auto OFFSET_NOCLIP_C = 0x01622771UL;//1.3.1.26;
constexpr auto OFFSET_NOCLIP_D = 0x016227A1UL;//1.3.1.26;
constexpr auto OFFSET_ACTOR_VALUE = 0x016A2D60UL;//1.3.1.26;
constexpr auto OFFSET_OPK = 0x0299381DUL;//1.3.1.26;
constexpr auto OFFSET_AV_REGEN = 0x016A311FUL;//1.3.1.26;
constexpr auto OFFSET_SERVER_POSITION = 0x01496B85UL; // 1.3.1.26;
constexpr auto OFFSET_ENTITY_ID = 0x0511A000UL;//1.3.1.26;
constexpr auto OFFSET_NUKE_CODE = 0x05A64078UL;//1.3.1.26;
constexpr auto OFFSET_NPC_LIST = 0x0591EFC0UL;//1.3.1.26;
constexpr auto OFFSET_NPC_PTR_FUNCTION = 0x0046EDB0UL;//1.3.1.26;
constexpr auto OFFSET_MELEE_ATTACK = 0x016AC800UL;//1.3.1.26;
constexpr auto OFFSET_CHARGEN = 0x05B4C900UL;//1.3.1.26;

//vtables
constexpr auto VTABLE_REQUESTACTIVATEREFMSG = 0x03B1BAD0UL;//1.3.1.26;
constexpr auto VTABLE_REQUESTTRANSFERITEMMSG = 0x03B2FD28UL;//1.3.1.26;
constexpr auto VTABLE_REQUESTTELEPORTTOLOCATIONMSG = 0x03B23D60UL;//1.3.1.26;
constexpr auto VTABLE_CLIENTSTATEMSG = 0x03B23940UL;//1.3.1.26;
constexpr auto VTABLE_REQUESTHITSONACTORS = 0x03B1B700UL;//1.3.1.26;
constexpr auto VTABLE_CREATEPROJECTILEMESSAGECLIENT = 0x03AE8A38UL;//1.3.1.26;



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

