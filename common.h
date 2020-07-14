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
	Loot,
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
	{ HotKeys::Loot, HotkeyCombination{ MOD_CONTROL | MOD_NOREPEAT, 'R' } },
	{ HotKeys::ToggleOverlay, HotkeyCombination{ MOD_CONTROL | MOD_NOREPEAT, VK_RETURN } }
};

const std::unordered_map<DWORD, std::string> JUNK_COMPONENT_NAMES = {
		{ 0x00017DE8, "Black Titanium" },
		{ 0x00017DE9, "Ultracite" },
		{ 0x0001FA8C, "Acid" },
		{ 0x0001FA91, "Aluminum" },
		{ 0x0001FA94, "Ballistic Fiber" },
		{ 0x0001FA96, "Antiseptic" },
		{ 0x0001FA97, "Asbestos" },
		{ 0x0001FA98, "Bone" },
		{ 0x0001FA9A, "Ceramic" },
		{ 0x0001FA9B, "Circuitry" },
		{ 0x0001FA9C, "Copper" },
		{ 0x0001FA9D, "Cork" },
		{ 0x0001FA9F, "Crystal" },
		{ 0x0001FAA0, "Fiber Optics" },
		{ 0x0001FAA1, "Fiberglass" },
		{ 0x0001FAA4, "Glass" },
		{ 0x0001FAA5, "Adhesive" },
		{ 0x0001FAA6, "Gold" },
		{ 0x0001FAAD, "Lead" },
		{ 0x0001FAAE, "Leather" },
		{ 0x0001FAB0, "Gear" },
		{ 0x0001FAB3, "Nuclear Material" },
		{ 0x0001FAB4, "Oil" },
		{ 0x0001FAB7, "Plastic" },
		{ 0x0001FAB9, "Rubber" },
		{ 0x0001FABB, "Silver" },
		{ 0x0001FABC, "Spring" },
		{ 0x0001FABD, "Steel" },
		{ 0x0001FAC2, "Wood" },
		{ 0x0003D294, "Screw" },
		{ 0x0005A0C4, "Concrete" },
		{ 0x0005A0C7, "Fertilizer" },
		{ 0x001223C7, "Cloth" },
		{ 0x002B9178, "Pure Cobalt Flux" },
		{ 0x002B917A, "Pure Fluorescent Flux" },
		{ 0x002B917C, "Pure Violet Flux" },
		{ 0x002B917E, "Pure Crimson Flux" },
		{ 0x002B9180, "Pure Yellowcake Flux" },
		{ 0x0031C09A, "Coal" },
		{ 0x0054E4FA, "Vault 94 Steel" },
};
const std::unordered_map<DWORD, std::string> FLORA_COMPONENT_NAMES = {
		{ 0x002DDD45, "Raw Crimson Flux" },
		{ 0x002DDD46, "Raw Cobalt Flux" },
		{ 0x002DDD49, "Raw Yellowcake Flux" },
		{ 0x002DDD4B, "Raw Fluorescent Flux" },
		{ 0x002DDD4D, "Raw Violet Flux" },
		{ 0x00017F81, "Black Titanium Ore" },
		{ 0x00019D29, "Aluminum Ore" },
		{ 0x00019E36, "Gold Ore" },
		{ 0x00019E51, "Copper Ore" },
		{ 0x00065304, "Iron Ore" },
		{ 0x00065324, "Lead Ore" },
		{ 0x00065328, "Silver Ore" },
		{ 0x00065338, "Uranium Ore" },
		{ 0x00139013, "Ultracite Ore" },
		{ 0x00058AA8, "Coal" },
		{ 0x0016FAD8, "Unrefined Coal" },
		{ 0x0056AAD2, "Irradiated Ore" },
		{ 0x0000BAEF, "Wild Blackberry" },
		{ 0x000F742E, "Carrot" },
		{ 0x000330F8, "Corn" },
		{ 0x000EF24D, "Gourd" },
		{ 0x000FAFEB, "Melon" },
		{ 0x00033102, "Mutfruit" },
		{ 0x000E0043, "Razorgrain" },
		{ 0x0009DCC4, "Tato" },
		{ 0x000183C6, "Junk Pile" },
		{ 0x000731A3, "Wood Scraps" },
		{ 0x00106D99, "Concrete Scrap" },
		{ 0x001BF732, "Waste Oil" },
		{ 0x001BF72D, "Waste Acid" },
		{ 0x00015C61, "Raw Crystal" },
		{ 0x001BF730, "Raw Fertilizer" },
		{ 0x0010D90E, "Cranberries" },
		{ 0x001FABC2, "Diseased Cranberries" },
		{ 0x000D981D, "Tarberry" },
		{ 0x0012C48E, "Firecracker Berry" },
		{ 0x001F24B6, "Melon Blossom" },
		{ 0x001C400D, "Tato Flower" },
		{ 0x001C4013, "Carrot Flower" },
		{ 0x001C4011, "Gourd Blossom" },
		{ 0x001C0FC2, "Brain Fungus" },
		{ 0x001C0FC3, "Glowing Fungus" },
		{ 0x001C25EE, "Bloodleaf" },
		{ 0x001BE2AB, "Silt Bean" },
		{ 0x001C3FDC, "Soot Flower" },
		{ 0x003C51E0, "Toxic Soot Flower" },
		{ 0x0009F24B, "Thistle" },
		{ 0x00525644, "Rhododendron Flower" },
		{ 0x00064AF0, "Firecap" },
		{ 0x003C51DF, "Snaptail Reed" },
		{ 0x00384D8C, "Starlight Berries" },
		{ 0x000E34E9, "Mutated Fern Flower" },
		{ 0x00111346, "Lure Weed" },
		{ 0x00110A12, "Aster" },
		{ 0x00111345, "Blight" },
		{ 0x00113106, "Fever Blossom" },
		{ 0x003FFADB, "Kaleidopore Flower" },
		{ 0x0011C0C2, "Ash Rose" },
		{ 0x00412186, "Gut Shroom" },
		{ 0x00088C3C, "Pumpkin" },
		{ 0x004EE5D2, "Ginseng Root" },
		{ 0x003B90A2, "Digested Goo" },
		{ 0x00081FDB, "Strangler Bloom" },
		{ 0x00050DC9, "Strangler Pod" },
		{ 0x003D7496, "Swamp Plant" },
		{ 0x003D202D, "Bleach Dogwood" },
		{ 0x00142950, "Megasloth Mushroom" },
		{ 0x000933EA, "Glowing Resin" },
		{ 0x0008E922, "Mothman Egg" },
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
