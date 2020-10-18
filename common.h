#pragma once
#include <unordered_map>
#include <unordered_set>

#include <Windows.h>

//overlay
constexpr LPCSTR OVERLAY_WINDOW_NAME = "Er";

//offsets

//just rpm
constexpr auto OFFSET_LOCAL_PLAYER = 0x05485AD8UL;//1.4.1.6
constexpr auto OFFSET_ENTITY_LIST = 0x052628A8UL;//1.4.1.6
constexpr auto OFFSET_MAIN = 0x054E7628UL;//1.4.1.6
constexpr auto OFFSET_CAMERA = 0x0552A630UL;//1.4.1.6
constexpr auto OFFSET_GET_PTR_A1 = 0x05269AD8UL;//1.4.1.6
constexpr auto OFFSET_NUKE_CODE = 0x0539F7E8UL;//1.4.1.6
constexpr auto OFFSET_MAIN_WORLDSPACE = 0x0539E320UL;//1.4.1.6

//wpm

//chargen
constexpr auto OFFSET_CHARGEN = 0x05488380UL;//1.4.1.6

//dmg redirection
constexpr auto OFFSET_REDIRECTION = 0x0187BE4BUL;//1.4.1.6
constexpr auto OFFSET_REDIRECTION_JMP = 0x0187BCDBUL;//1.4.1.6

//stats editor
constexpr auto OFFSET_ACTOR_VALUE = 0x01701BB0UL;//1.4.1.6
constexpr auto OFFSET_AV_REGEN = 0x01701F6FUL;//1.4.1.6
constexpr auto OFFSET_SERVER_POSITION = 0x014EA9A5UL;//1.4.1.6

//opk
constexpr auto OFFSET_OPK = 0x02A2ED6DUL;//1.4.1.6

//noclip
constexpr auto OFFSET_CHAR_CONTROLLER = 0x056E7160UL;//1.4.1.6
constexpr auto OFFSET_NOCLIP_A = 0x02A20C78UL;//1.4.1.6
constexpr auto OFFSET_NOCLIP_B = 0x02A20C30UL;//1.4.1.6
constexpr auto OFFSET_NOCLIP_C = 0x0167EA61UL;//1.4.1.6
constexpr auto OFFSET_NOCLIP_D = 0x0167EA91UL;//1.4.1.6

//weapon editor
constexpr auto OFFSET_DATA_HANDLER = 0x0525EE70UL;//1.4.1.6
constexpr auto OFFSET_INFINITE_AMMO = 0x00D1C726UL;//1.4.1.6

//remote threads / message sending

constexpr auto OFFSET_MELEE_ATTACK = 0x0170BA10UL;//1.4.1.6
constexpr auto OFFSET_MESSAGE_SENDER = 0x014FF310UL;//1.4.1.6
constexpr auto OFFSET_FAKE_MESSAGE = 0x01479AF0UL;//1.4.1.6

//vtables
constexpr auto VTABLE_REQUESTACTIVATEREFMSG = 0x03BC6540UL;//1.4.1.6
constexpr auto VTABLE_REQUESTTRANSFERITEMMSG = 0x03BDAEF8UL;//1.4.1.6
constexpr auto VTABLE_REQUESTTELEPORTTOLOCATIONMSG = 0x03BCE9E0UL;//1.4.1.6
constexpr auto VTABLE_CLIENTSTATEMSG = 0x03BCE5C0UL;//1.4.1.6
constexpr auto VTABLE_REQUESTHITSONACTORS = 0x03BC6170UL;//1.4.1.6

enum class HotKey : int
{
	PositionSpoofingToggle,
	NoclipToggle,
	OpkNpcsToggle,
	Loot,
	ToggleOverlay,
};

struct HotkeyCombination
{
	unsigned modifiers;
	unsigned vk;
};

const std::unordered_map<HotKey, HotkeyCombination> HOTKEYS{
	{ HotKey::PositionSpoofingToggle, HotkeyCombination{MOD_CONTROL | MOD_NOREPEAT, 'L' } },
	{ HotKey::NoclipToggle, HotkeyCombination{ MOD_CONTROL | MOD_NOREPEAT, 'Y' } },
	{ HotKey::OpkNpcsToggle, HotkeyCombination{ MOD_CONTROL | MOD_NOREPEAT, 'N' } },
	{ HotKey::Loot, HotkeyCombination{ MOD_CONTROL | MOD_NOREPEAT, 'R' } },
	{ HotKey::ToggleOverlay, HotkeyCombination{ MOD_CONTROL | MOD_NOREPEAT, VK_RETURN } }
};

const std::unordered_map<std::uint32_t, std::string> JUNK_COMPONENT_NAMES = {
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
const std::unordered_map<std::uint32_t, std::string> FLORA_COMPONENT_NAMES = {
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
const std::unordered_set<std::uint32_t> LEGENDARYEFFECT_FORMIDS = {
	0x00425E28, 0x004392CD, 0x0037F7D9, 0x001A7B80, 0x001A7AF6, 0x001A7BE2, 0x001A7BD3, 0x001A7AB2, 0x001A7B88,
	0x001A7BDA, 0x001A7C39, 0x0052BDC7, 0x0052BDC5, 0x0052BDC2, 0x0052BDC8, 0x0052BDB4, 0x0052BDB5, 0x0052BDB6,
	0x0052BDB7, 0x0052BDBA, 0x0052BDBC, 0x0052BDBF, 0x005299F5, 0x005299ED, 0x00529A14, 0x005299FE, 0x00529A0F,
	0x00529A0C, 0x00529A09, 0x005299F9, 0x005299FA, 0x005299FC, 0x00529A05, 0x00529A04, 0x005299FB, 0x00529A03,
	0x005299FD, 0x00529A02, 0x005281B8, 0x005281B4, 0x00527F6F, 0x00527F72, 0x00527F6E, 0x00527F7D, 0x00527F75,
	0x00527F6C, 0x00527F6D, 0x00527F74, 0x00527F84, 0x00527F82, 0x00527F8B, 0x00527F81, 0x00527F78, 0x00527F76,
	0x00527F7F, 0x00527F77, 0x00527F79, 0x00527F7A, 0x00527F7B, 0x00525400, 0x00525401, 0x005253FB, 0x0052414C,
	0x00524143, 0x0052414E, 0x0052414F, 0x00524150, 0x00524152, 0x00524153, 0x00524154, 0x00524146, 0x00524147,
	0x0052414A, 0x0052414B, 0x00521914, 0x00521915, 0x004F6D77, 0x004F6D7C, 0x004F6D86, 0x004F6D76, 0x004F6D85,
	0x004F6D84, 0x004F6D82, 0x004F6D83, 0x004F6D81, 0x004F6D80, 0x004F6D7F, 0x004F6D78, 0x004F6D7E, 0x004F6D7D,
	0x004F6AAE, 0x004F6AAB, 0x004F6AA1, 0x004F6AA0, 0x004F6AA7, 0x004F6AA5, 0x004F6AB1, 0x004F5772, 0x004F5778,
	0x004F5770, 0x004F5773, 0x004F577C, 0x004F5771, 0x004F5777, 0x004F5776, 0x004F577D, 0x004F577B, 0x004F577A,
	0x004F5779, 0x004EE548, 0x004EE54B, 0x004EE54C, 0x004EE54E, 0x004ED02B, 0x004ED02E, 0x004ED02C, 0x004ED02F,
	0x004E89B3, 0x004E89B2, 0x004E89AC, 0x004E89B4, 0x004E89B0, 0x004E89AF, 0x004E89AE, 0x004E89B6, 0x004E89AD,
	0x004E89B5, 0x003C4E27, 0x003C3458, 0x00357FBF, 0x001142A8, 0x0011410E, 0x0011410D, 0x0011410C, 0x0011410B,
	0x0011410A, 0x00114109, 0x00114108, 0x00114107, 0x00114106, 0x00114105, 0x00114104, 0x00114103, 0x00114101,
	0x001140FF, 0x001140FD, 0x001140FC, 0x001140FB, 0x001140FA, 0x001140F8, 0x001140F2, 0x001140F1, 0x001140F0,
	0x001140EF, 0x001140EE, 0x001140ED, 0x001140EC, 0x001140EB, 0x001140EA, 0x00113FC0, 0x001138DD, 0x0011384A,
	0x0011374F, 0x0011371F, 0x0010F599, 0x0010F598, 0x0010F596, 0x00226436, 0x001F81EB, 0x001F7A75, 0x001F1E47,
	0x001F1E0C, 0x001F1E0B, 0x001E73BD,
};

const std::unordered_set<std::uint32_t> TREASUREMAP_FORMIDS = {
	0x0051B8A8, //Ash Heap Treasure Map #01
	0x0051B8AC, //Ash Heap Treasure Map #02
	
	0x0051B8CD, //Cranberry Bog Treasure Map #01
	0x0051B8D6, //Cranberry Bog Treasure Map #02
	0x0051B8D9, //Cranberry Bog Treasure Map #03
	0x0051B8DE, //Cranberry Bog Treasure Map #04
	
	0x0051B7A2, //Forest Treasure Map #01
	0x0051B8A6, //Forest Treasure Map #02
	0x0051B8A7, //Forest Treasure Map #03
	0x0051B8A9, //Forest Treasure Map #04
	0x0051B8AA, //Forest Treasure Map #05
	0x0051B8AE, //Forest Treasure Map #06
	0x0051B8B0, //Forest Treasure Map #07
	0x0051B8B2, //Forest Treasure Map #08
	0x0051B8B6, //Forest Treasure Map #09
	0x0051B8B9, //Forest Treasure Map #10
	
	0x0051B8CE, //Mire Treasure Map #01
	0x0051B8D2, //Mire Treasure Map #02
	0x0051B8D7, //Mire Treasure Map #03
	0x0051B8D8, //Mire Treasure Map #04
	0x0051B8DB, //Mire Treasure Map #05
	
	0x0051B8BA, //Savage Divide Treasure Map #01
	0x0051B8C0, //Savage Divide Treasure Map #02
	0x0051B8C2, //Savage Divide Treasure Map #03
	0x0051B8C4, //Savage Divide Treasure Map #04
	0x0051B8C6, //Savage Divide Treasure Map #05
	0x0051B8C7, //Savage Divide Treasure Map #06
	0x0051B8C8, //Savage Divide Treasure Map #07
	0x0051B8CA, //Savage Divide Treasure Map #08
	0x0051B8CC, //Savage Divide Treasure Map #09
	0x0051B8D4, //Savage Divide Treasure Map #10
	
	0x0051B8B1, //Toxic Valley Treasure Map #01
	0x0051B8B8, //Toxic Valley Treasure Map #02
	0x0051B8BC, //Toxic Valley Treasure Map #03
	0x0051B8C1, //Toxic Valley Treasure Map #04
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

enum class FormType : std::uint8_t
{
	BgsTextureSet = 0x10,
	TesSound = 0x19,
	BgsAcousticSpace = 0x1B,
	TesObjectArmo = 0x26,
	TesObjectBook = 0x27,
	TesObjectCont = 0x28,
	TesObjectLigh = 0x2B,
	TesObjectMisc = 0x2C,
	CurrencyObject = 0x2F,
	TesObjectStat = 0x30,
	BgsStaticCollection = 0x31,
	BgsMovableStatic = 0x32,
	TesFlora = 0x35,
	TesObjectWeap = 0x37,
	TesAmmo = 0x38,
	TesNpc = 0x39,
	TesKey = 0x3C,
	AlchemyItem = 0x3D,
	TesUtilityItem = 0x3E,
	BgsIdleMarker = 0x3F,
	BgsNote = 0x40,
	BgsBendableSpline = 0x43,
	TesLevItem = 0x48,
	TesObjectRefr = 0x50,  //used in REFR objects, ref to item
	TesActor = 0x51, //used in REFR objects, ref to npc
	PlayerCharacter = 0xB5, //also used in REFR objects, ref to player

	Undefined = UINT8_MAX,
};
