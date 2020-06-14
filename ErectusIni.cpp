#include "ErectusInclude.h"
using namespace SettingsClasses;

mINI::INIFile file("Erectus.ini");
mINI::INIStructure ini;


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
	{						//Whitelisted
		false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false,
	},
	{						//Whitelist
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
	},
};

ScrapLooterSettings defltScrapLooterSettings
{
	false,
	false,
	false,
	false,
	30,
	60,
	3000,
	{
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
	},
	{
		0x00017DE8,//Black Titanium
		0x00017DE9,//Ultracite
		0x0001FA8C,//Acid
		0x0001FA91,//Aluminum
		0x0001FA94,//Ballistic Fiber
		0x0001FA96,//Antiseptic
		0x0001FA97,//Asbestos
		0x0001FA98,//Bone
		0x0001FA9A,//Ceramic
		0x0001FA9B,//Circuitry
		0x0001FA9C,//Copper
		0x0001FA9D,//Cork
		0x0001FA9F,//Crystal
		0x0001FAA0,//Fiber Optics
		0x0001FAA1,//Fiberglass
		0x0001FAA4,//Glass
		0x0001FAA5,//Adhesive
		0x0001FAA6,//Gold
		0x0001FAAD,//Lead
		0x0001FAAE,//Leather
		0x0001FAB0,//Gear
		0x0001FAB3,//Nuclear Material
		0x0001FAB4,//Oil
		0x0001FAB7,//Plastic
		0x0001FAB9,//Rubber
		0x0001FABB,//Silver
		0x0001FABC,//Spring
		0x0001FABD,//Steel
		0x0001FAC2,//Wood
		0x0003D294,//Screw
		0x0005A0C4,//Concrete
		0x0005A0C7,//Fertilizer
		0x001223C7,//Cloth
		0x002B9178,//Pure Cobalt Flux
		0x002B917A,//Pure Fluorescent Flux
		0x002B917C,//Pure Violet Flux
		0x002B917E,//Pure Crimson Flux
		0x002B9180,//Pure Yellowcake Flux
		0x0031C09A,//Coal
		0x0054E4FA,//Vault 94 Steel
	},
	{
		"Black Titanium",//0x00017DE8
		"Ultracite",//0x00017DE9
		"Acid",//0x0001FA8C
		"Aluminum",//0x0001FA91
		"Ballistic Fiber",//0x0001FA94
		"Antiseptic",//0x0001FA96
		"Asbestos",//0x0001FA97
		"Bone",//0x0001FA98
		"Ceramic",//0x0001FA9A
		"Circuitry",//0x0001FA9B
		"Copper",//0x0001FA9C
		"Cork",//0x0001FA9D
		"Crystal",//0x0001FA9F
		"Fiber Optics",//0x0001FAA0
		"Fiberglass",//0x0001FAA1
		"Glass",//0x0001FAA4
		"Adhesive",//0x0001FAA5
		"Gold",//0x0001FAA6
		"Lead",//0x0001FAAD
		"Leather",//0x0001FAAE
		"Gear",//0x0001FAB0
		"Nuclear Material",//0x0001FAB3
		"Oil",//0x0001FAB4
		"Plastic",//0x0001FAB7
		"Rubber",//0x0001FAB9
		"Silver",//0x0001FABB
		"Spring",//0x0001FABC
		"Steel",//0x0001FABD
		"Wood",//0x0001FAC2
		"Screw",//0x0003D294
		"Concrete",//0x0005A0C4
		"Fertilizer",//0x0005A0C7
		"Cloth",//0x001223C7
		"Pure Cobalt Flux",//0x002B9178
		"Pure Fluorescent Flux",//0x002B917A
		"Pure Violet Flux",//0x002B917C
		"Pure Crimson Flux",//0x002B917E
		"Pure Yellowcake Flux",//0x002B9180
		"Coal",//0x0031C09A
		"Vault 94 Steel",//0x0054E4FA
	},
};

ItemLooterSettings defltItemLooterSettings
{
	false,
	false,
	false,
	30,
	60,
	false,
	3000,
	false,
	3000,
	false,
	3000,
	false,
	3000,
	false,
	3000,
	false,
	3000,
	false,
	3000,
	false,
	3000,
	false,
	3000,
	false,
	3000,
	false,
	3000,
	false,
	3000,
	false,
	{
		false, false, false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false, false, false,
	},
	{
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
	},
	{
		false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false,
	},
	{
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
	},
};

TargetSettings defltTargetSettings
{
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	20.0f,
	false,
	{ 1.0f, 0.0f, 1.0f },
	false,
	0,
	1,
	1,
	12,
};

LocalPlayerSettings defltLocalPlayerSettings
{
	false,
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

OpkSettings defltOpkSettings
{
	false,
	false,
};

UtilitySettings defltUtilitySettings
{
	false,
	false,
	0x00000000,
	0x00000000,
};

SwapperSettings defltSwapperSettings
{
	0x00000000,
	0x00000000,
};

TransferSettings defltTransferSettings
{
	0x00000000,
	0x00000000,
	true,
	true,
	{
		false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false,
	},
	{
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
	},
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

TeleportSettings defltTeleportSettings{};

NukeCodeSettings defltNukeCodeSettings
{
	false,
	false,
	false,
	false,
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

FluxSettings defltFluxSettings
{
	false,
	false,
	false,
	false,
	false,
};

EntityLooterSettings defltEntityLooterSettings{};

HarvesterSettings defltHarvesterSettings
{
	false,
	false,
	false,
	{
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
		false,
	},
	{
		0x002DDD45,//Raw Crimson Flux
		0x002DDD46,//Raw Cobalt Flux
		0x002DDD49,//Raw Yellowcake Flux
		0x002DDD4B,//Raw Fluorescent Flux
		0x002DDD4D,//Raw Violet Flux
		0x00017F81,//Black Titanium Ore
		0x00019D29,//Aluminum Ore
		0x00019E36,//Gold Ore
		0x00019E51,//Copper Ore
		0x00065304,//Iron Ore
		0x00065324,//Lead Ore
		0x00065328,//Silver Ore
		0x00065338,//Uranium Ore
		0x00139013,//Ultracite Ore
		0x00058AA8,//Coal
		0x0016FAD8,//Unrefined Coal
		0x0056AAD2,//Irradiated Ore
		0x0000BAEF,//Wild Blackberry
		0x000F742E,//Carrot
		0x000330F8,//Corn
		0x000EF24D,//Gourd
		0x000FAFEB,//Melon
		0x00033102,//Mutfruit
		0x000E0043,//Razorgrain
		0x0009DCC4,//Tato
		0x000183C6,//Junk Pile
		0x000731A3,//Wood Scraps
		0x00106D99,//Concrete Scrap
		0x001BF732,//Waste Oil
		0x001BF72D,//Waste Acid
		0x00015C61,//Raw Crystal
		0x001BF730,//Raw Fertilizer
		0x0010D90E,//Cranberries
		0x001FABC2,//Diseased Cranberries
		0x000D981D,//Tarberry
		0x0012C48E,//Firecracker Berry
		0x001F24B6,//Melon Blossom
		0x001C400D,//Tato Flower
		0x001C4013,//Carrot Flower
		0x001C4011,//Gourd Blossom
		0x001C0FC2,//Brain Fungus
		0x001C0FC3,//Glowing Fungus
		0x001C25EE,//Bloodleaf
		0x001BE2AB,//Silt Bean
		0x001C3FDC,//Soot Flower
		0x003C51E0,//Toxic Soot Flower
		0x0009F24B,//Thistle
		0x00525644,//Rhododendron Flower
		0x00064AF0,//Firecap
		0x003C51DF,//Snaptail Reed
		0x00384D8C,//Starlight Berries
		0x000E34E9,//Mutated Fern Flower
		0x00111346,//Lure Weed
		0x00110A12,//Aster
		0x00111345,//Blight
		0x00113106,//Fever Blossom
		0x003FFADB,//Kaleidopore Flower
		0x0011C0C2,//Ash Rose
		0x00412186,//Gut Shroom
		0x00088C3C,//Pumpkin
		0x004EE5D2,//Ginseng Root
		0x003B90A2,//Digested Goo
		0x00081FDB,//Strangler Bloom
		0x00050DC9,//Strangler Pod
		0x003D7496,//Swamp Plant
		0x003D202D,//Bleach Dogwood
		0x00142950,//Megasloth Mushroom
		0x000933EA,//Glowing Resin
		0x0008E922,//Mothman Egg
	},
	{
		"Raw Crimson Flux",
		"Raw Cobalt Flux",
		"Raw Yellowcake Flux",
		"Raw Fluorescent Flux",
		"Raw Violet Flux",
		"Black Titanium Ore",
		"Aluminum Ore",
		"Gold Ore",
		"Copper Ore",
		"Iron Ore",
		"Lead Ore",
		"Silver Ore",
		"Uranium Ore",
		"Ultracite Ore",
		"Coal",
		"Unrefined Coal",
		"Irradiated Ore",
		"Wild Blackberry",
		"Carrot",
		"Corn",
		"Gourd",
		"Melon",
		"Mutfruit",
		"Razorgrain",
		"Tato",
		"Junk Pile",
		"Wood Scraps",
		"Concrete Scrap",
		"Waste Oil",
		"Waste Acid",
		"Raw Crystal",
		"Raw Fertilizer",
		"Cranberries",
		"Diseased Cranberries",
		"Tarberry",
		"Firecracker Berry",
		"Melon Blossom",
		"Tato Flower",
		"Carrot Flower",
		"Gourd Blossom",
		"Brain Fungus",
		"Glowing Fungus",
		"Bloodleaf",
		"Silt Bean",
		"Soot Flower",
		"Toxic Soot Flower",
		"Thistle",
		"Rhododendron Flower",
		"Firecap",
		"Snaptail Reed",
		"Starlight Berries",
		"Mutated Fern Flower",
		"Lure Weed",
		"Aster",
		"Blight",
		"Fever Blossom",
		"Kaleidopore Flower",
		"Ash Rose",
		"Gut Shroom",
		"Pumpkin",
		"Ginseng Root",
		"Digested Goo",
		"Strangler Bloom",
		"Strangler Pod",
		"Swamp Plant",
		"Bleach Dogwood",
		"Megasloth Mushroom",
		"Glowing Resin",
		"Mothman Egg",
	},
};

KnownRecipeSettings defltKnownRecipeSettings
{
	true,
	true,
};

MeleeSettings defltMeleeSettings
{
	false,
	10,
	20,
};

ChargenSettings defltChargenSettings
{
	false,
	0.33f,
	0.33f,
	0.33f,
};

ExtraNpcSettings defltExtraNpcSettings
{
	false,
	false,
	false,
	false,
	false,
	{
		false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false,
	},
	{
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
	},
};

OverlaySettingsA ErectusIni::playerSettings = defltPlayerSettings;
OverlaySettingsA ErectusIni::npcSettings = defltNpcSettings;
OverlaySettingsB ErectusIni::containerSettings = defltOverlaySettingsB;
OverlaySettingsB ErectusIni::junkSettings = defltOverlaySettingsB;
OverlaySettingsB ErectusIni::planSettings = defltOverlaySettingsB;
OverlaySettingsB ErectusIni::magazineSettings = defltOverlaySettingsB;
OverlaySettingsB ErectusIni::bobbleheadSettings = defltOverlaySettingsB;
OverlaySettingsB ErectusIni::itemSettings = defltOverlaySettingsB;
OverlaySettingsB ErectusIni::floraSettings = defltOverlaySettingsB;
OverlaySettingsB ErectusIni::entitySettings = defltOverlaySettingsB;
ScrapLooterSettings ErectusIni::customScrapLooterSettings = defltScrapLooterSettings;
ItemLooterSettings ErectusIni::customItemLooterSettings = defltItemLooterSettings;
WeaponSettings ErectusIni::customWeaponSettings = defaultWeaponSettings;
TargetSettings ErectusIni::customTargetSettings = defltTargetSettings;
LocalPlayerSettings ErectusIni::customLocalPlayerSettings = defltLocalPlayerSettings;
OpkSettings ErectusIni::customOpkSettings = defltOpkSettings;
UtilitySettings ErectusIni::customUtilitySettings = defltUtilitySettings;
SwapperSettings ErectusIni::customSwapperSettings = defltSwapperSettings;
TransferSettings ErectusIni::customTransferSettings = defltTransferSettings;
TeleportSettings ErectusIni::customTeleportSettings = defltTeleportSettings;
NukeCodeSettings ErectusIni::customNukeCodeSettings = defltNukeCodeSettings;
LegendarySettings ErectusIni::customLegendarySettings = defltLegendarySettings;
FluxSettings ErectusIni::customFluxSettings = defltFluxSettings;
EntityLooterSettings ErectusIni::npcLooterSettings = defltEntityLooterSettings;
EntityLooterSettings ErectusIni::containerLooterSettings = defltEntityLooterSettings;
HarvesterSettings ErectusIni::customHarvesterSettings = defltHarvesterSettings;
KnownRecipeSettings ErectusIni::customKnownRecipeSettings = defltKnownRecipeSettings;
MeleeSettings ErectusIni::customMeleeSettings = defltMeleeSettings;
ChargenSettings ErectusIni::customChargenSettings = defltChargenSettings;
ExtraNpcSettings ErectusIni::customExtraNpcSettings = defltExtraNpcSettings;

void ErectusIni::GetDword(const char *section, const char *key, DWORD *value, const DWORD *deflt)
{
	if (!ini.has(section))
	{
		ini[section];
	}

	if (!ini[section].has(key))
	{
		char defltText[sizeof("00000000")];
		sprintf_s(defltText, "%08lX", *deflt);
		ini[section][key] = defltText;
	}

	try
	{
		*value = stoul(ini[section][key], nullptr, 16);
	}
	catch (...)
	{
		*value = *deflt;
	}
}

void ErectusIni::SetDword(const char *section, const char *key, const DWORD *value, const DWORD *deflt)
{
	if (!ini.has(section))
	{
		ini[section];
	}

	if (!ini[section].has(key))
	{
		char defltText[sizeof("00000000")];
		sprintf_s(defltText, "%08lX", *deflt);
		ini[section][key] = defltText;
	}

	char valueText[sizeof("00000000")];
	sprintf_s(valueText, "%08lX", *value);
	ini[section][key] = valueText;
}

void ErectusIni::GetInt(const char *section, const char *key, int *value, const int *deflt)
{
	if (!ini.has(section))
	{
		ini[section];
	}

	if (!ini[section].has(key))
	{
		ini[section][key] = std::to_string(*deflt);
	}

	try
	{
		*value = stoi(ini[section][key]);
	}
	catch (...)
	{
		*value = *deflt;
	}
}

void ErectusIni::SetInt(const char *section, const char *key, const int *value, const int *deflt)
{
	if (!ini.has(section))
	{
		ini[section];
	}

	if (!ini[section].has(key))
	{
		ini[section][key] = std::to_string(*deflt);
	}

	ini[section][key] = std::to_string(*value);
}

void ErectusIni::GetSliderInt(const char *section, const char *key, int *value, int *deflt, const int min, const int max)
{
	Utils::ValidateInt(deflt, min, max);
	GetInt(section, key, value, deflt);
	Utils::ValidateInt(value, min, max);
	SetInt(section, key, value, deflt);
}

void ErectusIni::SetSliderInt(const char *section, const char *key, int *value, int *deflt, const int min, const int max)
{
	Utils::ValidateInt(value, min, max);
	Utils::ValidateInt(deflt, min, max);
	SetInt(section, key, value, deflt);
}

void ErectusIni::GetFloat(const char *section, const char *key, float *value, const float *deflt)
{
	if (!ini.has(section))
	{
		ini[section];
	}

	if (!ini[section].has(key))
	{
		ini[section][key] = std::to_string(*deflt);
	}

	try
	{
		*value = stof(ini[section][key]);
	}
	catch (...)
	{
		*value = *deflt;
	}
}

void ErectusIni::SetFloat(const char *section, const char *key, const float *value, const float *deflt)
{
	if (!ini.has(section))
	{
		ini[section];
	}

	if (!ini[section].has(key))
	{
		ini[section][key] = std::to_string(*deflt);
	}

	ini[section][key] = std::to_string(*value);
}

void ErectusIni::GetSliderFloat(const char *section, const char *key, float *value, float *deflt, const float min, const float max)
{
	Utils::ValidateFloat(deflt, min, max);
	GetFloat(section, key, value, deflt);
	Utils::ValidateFloat(value, min, max);
	SetFloat(section, key, value, deflt);
}

void ErectusIni::ErectusIni::SetSliderFloat(const char *section, const char *key, float *value, float *deflt, const float min, const float max)
{
	Utils::ValidateFloat(value, min, max);
	Utils::ValidateFloat(deflt, min, max);
	SetFloat(section, key, value, deflt);
}

void ErectusIni::GetRgb(const char *section, const char *key, float *value, float *deflt)
{
	const auto keyLength = Utils::GetTextLength(key);
	const auto keySize = static_cast<int>(keyLength + sizeof"x");

	auto* keyR = new char[keySize];
	sprintf_s(keyR, keySize, "%sR", key);

	auto* keyG = new char[keySize];
	sprintf_s(keyG, keySize, "%sG", key);

	auto* keyB = new char[keySize];
	sprintf_s(keyB, keySize, "%sB", key);

	Utils::ValidateRgb(deflt);
	GetFloat(section, keyR, &value[0], &deflt[0]);
	GetFloat(section, keyG, &value[1], &deflt[1]);
	GetFloat(section, keyB, &value[2], &deflt[2]);
	Utils::ValidateRgb(value);

	delete[]keyR;
	keyR = nullptr;

	delete[]keyG;
	keyG = nullptr;

	delete[]keyB;
	keyB = nullptr;
}

void ErectusIni::SetRgb(const char *section, const char *key, float *value, float *deflt)
{
	const auto keyLength = Utils::GetTextLength(key);
	const auto keySize = static_cast<int>(keyLength + sizeof"x");

	auto* keyR = new char[keySize];
	sprintf_s(keyR, keySize, "%sR", key);

	auto* keyG = new char[keySize];
	sprintf_s(keyG, keySize, "%sG", key);

	auto* keyB = new char[keySize];
	sprintf_s(keyB, keySize, "%sB", key);

	Utils::ValidateRgb(value);
	Utils::ValidateRgb(deflt);
	SetFloat(section, keyR, &value[0], &deflt[0]);
	SetFloat(section, keyG, &value[1], &deflt[1]);
	SetFloat(section, keyB, &value[2], &deflt[2]);

	delete[]keyR;
	keyR = nullptr;

	delete[]keyG;
	keyG = nullptr;

	delete[]keyB;
	keyB = nullptr;
}

void ErectusIni::GetQuadFloat(const char *section, const char *key, float *value, float *deflt)
{
	const auto keyLength = Utils::GetTextLength(key);
	const auto keySize = static_cast<int>(keyLength + sizeof"x");

	auto* keyX = new char[keySize];
	sprintf_s(keyX, keySize, "%sX", key);

	auto* keyY = new char[keySize];
	sprintf_s(keyY, keySize, "%sY", key);

	auto* keyZ = new char[keySize];
	sprintf_s(keyZ, keySize, "%sZ", key);

	auto* keyW = new char[keySize];
	sprintf_s(keyW, keySize, "%sW", key);

	GetFloat(section, keyX, &value[0], &deflt[0]);
	GetFloat(section, keyY, &value[1], &deflt[1]);
	GetFloat(section, keyZ, &value[2], &deflt[2]);
	GetFloat(section, keyW, &value[3], &deflt[3]);

	delete[]keyX;
	keyX = nullptr;

	delete[]keyY;
	keyY = nullptr;

	delete[]keyZ;
	keyZ = nullptr;

	delete[]keyW;
	keyW = nullptr;
}

void ErectusIni::SetQuadFloat(const char *section, const char *key, float *value, float *deflt)
{
	const auto keyLength = Utils::GetTextLength(key);
	const auto keySize = static_cast<int>(keyLength + sizeof"x");

	auto* keyX = new char[keySize];
	sprintf_s(keyX, keySize, "%sX", key);

	auto* keyY = new char[keySize];
	sprintf_s(keyY, keySize, "%sY", key);

	auto* keyZ = new char[keySize];
	sprintf_s(keyZ, keySize, "%sZ", key);

	auto* keyW = new char[keySize];
	sprintf_s(keyW, keySize, "%sW", key);

	SetFloat(section, keyX, &value[0], &deflt[0]);
	SetFloat(section, keyY, &value[1], &deflt[1]);
	SetFloat(section, keyZ, &value[2], &deflt[2]);
	SetFloat(section, keyW, &value[3], &deflt[3]);

	delete[]keyX;
	keyX = nullptr;

	delete[]keyY;
	keyY = nullptr;

	delete[]keyZ;
	keyZ = nullptr;

	delete[]keyW;
	keyW = nullptr;
}

void ErectusIni::GetBool(const char *section, const char *key, bool *value, const bool *deflt)
{
	auto bufferValue = static_cast<int>(*value);
	auto bufferdeflt = static_cast<int>(*deflt);
	GetSliderInt(section, key, &bufferValue, &bufferdeflt, 0, 1);
	*value = static_cast<bool>(bufferValue);
}

void ErectusIni::SetBool(const char *section, const char *key, const bool *value, const bool *deflt)
{
	auto bufferValue = static_cast<int>(*value);
	auto bufferdeflt = static_cast<int>(*deflt);
	SetSliderInt(section, key, &bufferValue, &bufferdeflt, 0, 1);
}

void ErectusIni::GetOverlaySettingsA(const char *section, OverlaySettingsA* value, OverlaySettingsA* deflt)
{
	GetBool(section, "Enabled", &value->enabled, &deflt->enabled);
	GetSliderInt(section, "EnabledDistance", &value->enabledDistance, &deflt->enabledDistance, 0, 3000);
	GetBool(section, "DrawAlive", &value->drawAlive, &deflt->drawAlive);
	GetRgb(section, "AliveColor", value->aliveColor, deflt->aliveColor);
	GetBool(section, "DrawDowned", &value->drawDowned, &deflt->drawDowned);
	GetRgb(section, "DownedColor", value->downedColor, deflt->downedColor);
	GetBool(section, "DrawDead", &value->drawDead, &deflt->drawDead);
	GetRgb(section, "DeadColor", value->deadColor, deflt->deadColor);
	GetBool(section, "DrawUnknown", &value->drawUnknown, &deflt->drawUnknown);
	GetRgb(section, "UnknownColor", value->unknownColor, deflt->unknownColor);
	GetBool(section, "DrawEnabled", &value->drawEnabled, &deflt->drawEnabled);
	GetSliderFloat(section, "EnabledAlpha", &value->enabledAlpha, &deflt->enabledAlpha, 0.0f, 1.0f);
	GetBool(section, "DrawDisabled", &value->drawDisabled, &deflt->drawDisabled);
	GetSliderFloat(section, "DisabledAlpha", &value->disabledAlpha, &deflt->disabledAlpha, 0.0f, 1.0f);
	GetBool(section, "DrawNamed", &value->drawNamed, &deflt->drawNamed);
	GetBool(section, "DrawUnnamed", &value->drawUnnamed, &deflt->drawUnnamed);
	GetBool(section, "ShowName", &value->showName, &deflt->showName);
	GetBool(section, "ShowDistance", &value->showDistance, &deflt->showDistance);
	GetBool(section, "ShowHealth", &value->showHealth, &deflt->showHealth);
	GetBool(section, "ShowDeadHealth", &value->showDeadHealth, &deflt->showDeadHealth);
	GetBool(section, "TextShadowed", &value->textShadowed, &deflt->textShadowed);
	GetBool(section, "TextCentered", &value->textCentered, &deflt->textCentered);
}

void ErectusIni::SetOverlaySettingsA(const char *section, OverlaySettingsA* value, OverlaySettingsA* deflt)
{
	SetBool(section, "Enabled", &value->enabled, &deflt->enabled);
	SetSliderInt(section, "EnabledDistance", &value->enabledDistance, &deflt->enabledDistance, 0, 3000);
	SetBool(section, "DrawAlive", &value->drawAlive, &deflt->drawAlive);
	SetRgb(section, "AliveColor", value->aliveColor, deflt->aliveColor);
	SetBool(section, "DrawDowned", &value->drawDowned, &deflt->drawDowned);
	SetRgb(section, "DownedColor", value->downedColor, deflt->downedColor);
	SetBool(section, "DrawDead", &value->drawDead, &deflt->drawDead);
	SetRgb(section, "DeadColor", value->deadColor, deflt->deadColor);
	SetBool(section, "DrawUnknown", &value->drawUnknown, &deflt->drawUnknown);
	SetRgb(section, "UnknownColor", value->unknownColor, deflt->unknownColor);
	SetBool(section, "DrawEnabled", &value->drawEnabled, &deflt->drawEnabled);
	SetSliderFloat(section, "EnabledAlpha", &value->enabledAlpha, &deflt->enabledAlpha, 0.0f, 1.0f);
	SetBool(section, "DrawDisabled", &value->drawDisabled, &deflt->drawDisabled);
	SetSliderFloat(section, "DisabledAlpha", &value->disabledAlpha, &deflt->disabledAlpha, 0.0f, 1.0f);
	SetBool(section, "DrawNamed", &value->drawNamed, &deflt->drawNamed);
	SetBool(section, "DrawUnnamed", &value->drawUnnamed, &deflt->drawUnnamed);
	SetBool(section, "ShowName", &value->showName, &deflt->showName);
	SetBool(section, "ShowDistance", &value->showDistance, &deflt->showDistance);
	SetBool(section, "ShowHealth", &value->showHealth, &deflt->showHealth);
	SetBool(section, "ShowDeadHealth", &value->showDeadHealth, &deflt->showDeadHealth);
	SetBool(section, "TextShadowed", &value->textShadowed, &deflt->textShadowed);
	SetBool(section, "TextCentered", &value->textCentered, &deflt->textCentered);
}

void ErectusIni::GetOverlaySettingsB(const char *section, OverlaySettingsB* value, OverlaySettingsB* deflt)
{
	GetBool(section, "Enabled", &value->enabled, &deflt->enabled);
	GetSliderInt(section, "EnabledDistance", &value->enabledDistance, &deflt->enabledDistance, 0, 3000);
	GetRgb(section, "Color", value->color, deflt->color);
	GetBool(section, "DrawEnabled", &value->drawEnabled, &deflt->drawEnabled);
	GetSliderFloat(section, "EnabledAlpha", &value->enabledAlpha, &deflt->enabledAlpha, 0.0f, 1.0f);
	GetBool(section, "DrawDisabled", &value->drawDisabled, &deflt->drawDisabled);
	GetSliderFloat(section, "DisabledAlpha", &value->disabledAlpha, &deflt->disabledAlpha, 0.0f, 1.0f);
	GetBool(section, "DrawNamed", &value->drawNamed, &deflt->drawNamed);
	GetBool(section, "DrawUnnamed", &value->drawUnnamed, &deflt->drawUnnamed);
	GetBool(section, "ShowName", &value->showName, &deflt->showName);
	GetBool(section, "ShowDistance", &value->showDistance, &deflt->showDistance);
	GetBool(section, "TextShadowed", &value->textShadowed, &deflt->textShadowed);
	GetBool(section, "TextCentered", &value->textCentered, &deflt->textCentered);
	for (auto i = 0; i < 32; i++)
	{
		char whitelistedText[sizeof("Whitelisted31")];
		char whitelistText[sizeof("Whitelist31")];
		sprintf_s(whitelistedText, "Whitelisted%d", i);
		sprintf_s(whitelistText, "Whitelist%d", i);
		GetBool(section, whitelistedText, &value->whitelisted[i], &deflt->whitelisted[i]);
		GetDword(section, whitelistText, &value->whitelist[i], &deflt->whitelist[i]);
	}
}

void ErectusIni::SetOverlaySettingsB(const char *section, OverlaySettingsB* value, OverlaySettingsB* deflt)
{
	SetBool(section, "Enabled", &value->enabled, &deflt->enabled);
	SetSliderInt(section, "EnabledDistance", &value->enabledDistance, &deflt->enabledDistance, 0, 3000);
	SetRgb(section, "Color", value->color, deflt->color);
	SetBool(section, "DrawEnabled", &value->drawEnabled, &deflt->drawEnabled);
	SetSliderFloat(section, "EnabledAlpha", &value->enabledAlpha, &deflt->enabledAlpha, 0.0f, 1.0f);
	SetBool(section, "DrawDisabled", &value->drawDisabled, &deflt->drawDisabled);
	SetSliderFloat(section, "DisabledAlpha", &value->disabledAlpha, &deflt->disabledAlpha, 0.0f, 1.0f);
	SetBool(section, "DrawNamed", &value->drawNamed, &deflt->drawNamed);
	SetBool(section, "DrawUnnamed", &value->drawUnnamed, &deflt->drawUnnamed);
	SetBool(section, "ShowName", &value->showName, &deflt->showName);
	SetBool(section, "ShowDistance", &value->showDistance, &deflt->showDistance);
	SetBool(section, "TextShadowed", &value->textShadowed, &deflt->textShadowed);
	SetBool(section, "TextCentered", &value->textCentered, &deflt->textCentered);
	for (auto i = 0; i < 32; i++)
	{
		char whitelistedText[sizeof("Whitelisted31")];
		char whitelistText[sizeof("Whitelist31")];
		sprintf_s(whitelistedText, "Whitelisted%d", i);
		sprintf_s(whitelistText, "Whitelist%d", i);
		SetBool(section, whitelistedText, &value->whitelisted[i], &deflt->whitelisted[i]);
		SetDword(section, whitelistText, &value->whitelist[i], &deflt->whitelist[i]);
	}
}

void ErectusIni::GetScrapSettings()
{
	GetBool("ScrapSettings", "ScrapKeybindEnabled", &customScrapLooterSettings.scrapKeybindEnabled, &defltScrapLooterSettings.scrapKeybindEnabled);
	GetBool("ScrapSettings", "ScrapOverrideEnabled", &customScrapLooterSettings.scrapOverrideEnabled, &defltScrapLooterSettings.scrapOverrideEnabled);
	GetBool("ScrapSettings", "ScrapAutomaticLootingEnabled", &customScrapLooterSettings.scrapAutomaticLootingEnabled, &defltScrapLooterSettings.scrapAutomaticLootingEnabled);
	GetBool("ScrapSettings", "ScrapAutomaticStatus", &customScrapLooterSettings.scrapAutomaticStatus, &defltScrapLooterSettings.scrapAutomaticStatus);
	GetSliderInt("ScrapSettings", "ScrapAutomaticSpeedMin", &customScrapLooterSettings.scrapAutomaticSpeedMin, &defltScrapLooterSettings.scrapAutomaticSpeedMin, 10, 60);
	GetSliderInt("ScrapSettings", "ScrapAutomaticSpeedMax", &customScrapLooterSettings.scrapAutomaticSpeedMax, &defltScrapLooterSettings.scrapAutomaticSpeedMax, 10, 60);
	GetSliderInt("ScrapSettings", "ScrapLooterDistance", &customScrapLooterSettings.scrapLooterDistance, &defltScrapLooterSettings.scrapLooterDistance, 0, 3000);
	for (auto i = 0; i < 40; i++)
	{
		char scrapEnabledText[sizeof("ScrapEnabled39")];
		sprintf_s(scrapEnabledText, "ScrapEnabled%d", i);
		GetBool("ScrapSettings", scrapEnabledText, &customScrapLooterSettings.scrapEnabledList[i], &defltScrapLooterSettings.scrapEnabledList[i]);
	}
	if (customScrapLooterSettings.scrapAutomaticSpeedMax < customScrapLooterSettings.scrapAutomaticSpeedMin)
	{
		customScrapLooterSettings.scrapAutomaticSpeedMax = customScrapLooterSettings.scrapAutomaticSpeedMin;
	}
}

void ErectusIni::SetScrapSettings()
{
	if (customScrapLooterSettings.scrapAutomaticSpeedMax < customScrapLooterSettings.scrapAutomaticSpeedMin)
	{
		customScrapLooterSettings.scrapAutomaticSpeedMax = customScrapLooterSettings.scrapAutomaticSpeedMin;
	}
	SetBool("ScrapSettings", "ScrapKeybindEnabled", &customScrapLooterSettings.scrapKeybindEnabled, &defltScrapLooterSettings.scrapKeybindEnabled);
	SetBool("ScrapSettings", "ScrapOverrideEnabled", &customScrapLooterSettings.scrapOverrideEnabled, &defltScrapLooterSettings.scrapOverrideEnabled);
	SetBool("ScrapSettings", "ScrapAutomaticLootingEnabled", &customScrapLooterSettings.scrapAutomaticLootingEnabled, &defltScrapLooterSettings.scrapAutomaticLootingEnabled);
	SetBool("ScrapSettings", "ScrapAutomaticStatus", &customScrapLooterSettings.scrapAutomaticStatus, &defltScrapLooterSettings.scrapAutomaticStatus);
	SetSliderInt("ScrapSettings", "ScrapAutomaticSpeedMin", &customScrapLooterSettings.scrapAutomaticSpeedMin, &defltScrapLooterSettings.scrapAutomaticSpeedMin, 10, 60);
	SetSliderInt("ScrapSettings", "ScrapAutomaticSpeedMax", &customScrapLooterSettings.scrapAutomaticSpeedMax, &defltScrapLooterSettings.scrapAutomaticSpeedMax, 10, 60);
	SetSliderInt("ScrapSettings", "ScrapLooterDistance", &customScrapLooterSettings.scrapLooterDistance, &defltScrapLooterSettings.scrapLooterDistance, 0, 3000);
	for (auto i = 0; i < 40; i++)
	{
		char scrapEnabledText[sizeof("ScrapEnabled39")];
		sprintf_s(scrapEnabledText, "ScrapEnabled%d", i);
		SetBool("ScrapSettings", scrapEnabledText, &customScrapLooterSettings.scrapEnabledList[i], &defltScrapLooterSettings.scrapEnabledList[i]);
	}
}

void ErectusIni::GetItemLooterSettings()
{
	GetBool("ItemLooterSettings", "ItemKeybindEnabled", &customItemLooterSettings.itemKeybindEnabled, &defltItemLooterSettings.itemKeybindEnabled);
	GetBool("ItemLooterSettings", "ItemAutomaticLootingEnabled", &customItemLooterSettings.itemAutomaticLootingEnabled, &defltItemLooterSettings.itemAutomaticLootingEnabled);
	GetBool("ItemLooterSettings", "ItemAutomaticStatus", &customItemLooterSettings.itemAutomaticStatus, &defltItemLooterSettings.itemAutomaticStatus);
	GetSliderInt("ItemLooterSettings", "ItemAutomaticSpeedMin", &customItemLooterSettings.itemAutomaticSpeedMin, &defltItemLooterSettings.itemAutomaticSpeedMin, 10, 60);
	GetSliderInt("ItemLooterSettings", "ItemAutomaticSpeedMax", &customItemLooterSettings.itemAutomaticSpeedMax, &defltItemLooterSettings.itemAutomaticSpeedMax, 10, 60);
	GetBool("ItemLooterSettings", "ItemLooterWeaponsEnabled", &customItemLooterSettings.itemLooterWeaponsEnabled, &defltItemLooterSettings.itemLooterWeaponsEnabled);
	GetSliderInt("ItemLooterSettings", "ItemLooterWeaponsDistance", &customItemLooterSettings.itemLooterWeaponsDistance, &defltItemLooterSettings.itemLooterWeaponsDistance, 0, 3000);
	GetBool("ItemLooterSettings", "ItemLooterArmorEnabled", &customItemLooterSettings.itemLooterArmorEnabled, &defltItemLooterSettings.itemLooterArmorEnabled);
	GetSliderInt("ItemLooterSettings", "ItemLooterArmorDistance", &customItemLooterSettings.itemLooterArmorDistance, &defltItemLooterSettings.itemLooterArmorDistance, 0, 3000);
	GetBool("ItemLooterSettings", "ItemLooterAmmoEnabled", &customItemLooterSettings.itemLooterAmmoEnabled, &defltItemLooterSettings.itemLooterAmmoEnabled);
	GetSliderInt("ItemLooterSettings", "ItemLooterAmmoDistance", &customItemLooterSettings.itemLooterAmmoDistance, &defltItemLooterSettings.itemLooterAmmoDistance, 0, 3000);
	GetBool("ItemLooterSettings", "ItemLooterModsEnabled", &customItemLooterSettings.itemLooterModsEnabled, &defltItemLooterSettings.itemLooterModsEnabled);
	GetSliderInt("ItemLooterSettings", "ItemLooterModsDistance", &customItemLooterSettings.itemLooterModsDistance, &defltItemLooterSettings.itemLooterModsDistance, 0, 3000);
	GetBool("ItemLooterSettings", "ItemLooterMagazinesEnabled", &customItemLooterSettings.itemLooterMagazinesEnabled, &defltItemLooterSettings.itemLooterMagazinesEnabled);
	GetSliderInt("ItemLooterSettings", "ItemLooterMagazinesDistance", &customItemLooterSettings.itemLooterMagazinesDistance, &defltItemLooterSettings.itemLooterMagazinesDistance, 0, 3000);
	GetBool("ItemLooterSettings", "ItemLooterBobbleheadsEnabled", &customItemLooterSettings.itemLooterBobbleheadsEnabled, &defltItemLooterSettings.itemLooterBobbleheadsEnabled);
	GetSliderInt("ItemLooterSettings", "ItemLooterBobbleheadsDistance", &customItemLooterSettings.itemLooterBobbleheadsDistance, &defltItemLooterSettings.itemLooterBobbleheadsDistance, 0, 3000);
	GetBool("ItemLooterSettings", "ItemLooterAidEnabled", &customItemLooterSettings.itemLooterAidEnabled, &defltItemLooterSettings.itemLooterAidEnabled);
	GetSliderInt("ItemLooterSettings", "ItemLooterAidDistance", &customItemLooterSettings.itemLooterAidDistance, &defltItemLooterSettings.itemLooterAidDistance, 0, 3000);
	GetBool("ItemLooterSettings", "ItemLooterKnownPlansEnabled", &customItemLooterSettings.itemLooterKnownPlansEnabled, &defltItemLooterSettings.itemLooterKnownPlansEnabled);
	GetSliderInt("ItemLooterSettings", "ItemLooterKnownPlansDistance", &customItemLooterSettings.itemLooterKnownPlansDistance, &defltItemLooterSettings.itemLooterKnownPlansDistance, 0, 3000);
	GetBool("ItemLooterSettings", "ItemLooterUnknownPlansEnabled", &customItemLooterSettings.itemLooterUnknownPlansEnabled, &defltItemLooterSettings.itemLooterUnknownPlansEnabled);
	GetSliderInt("ItemLooterSettings", "ItemLooterUnknownPlansDistance", &customItemLooterSettings.itemLooterUnknownPlansDistance, &defltItemLooterSettings.itemLooterUnknownPlansDistance, 0, 3000);
	GetBool("ItemLooterSettings", "ItemLooterMiscEnabled", &customItemLooterSettings.itemLooterMiscEnabled, &defltItemLooterSettings.itemLooterMiscEnabled);
	GetSliderInt("ItemLooterSettings", "ItemLooterMiscDistance", &customItemLooterSettings.itemLooterMiscDistance, &defltItemLooterSettings.itemLooterMiscDistance, 0, 3000);
	GetBool("ItemLooterSettings", "ItemLooterUnlistedEnabled", &customItemLooterSettings.itemLooterUnlistedEnabled, &defltItemLooterSettings.itemLooterUnlistedEnabled);
	GetSliderInt("ItemLooterSettings", "ItemLooterUnlistedDistance", &customItemLooterSettings.itemLooterUnlistedDistance, &defltItemLooterSettings.itemLooterUnlistedDistance, 0, 3000);
	GetBool("ItemLooterSettings", "ItemLooterListEnabled", &customItemLooterSettings.itemLooterListEnabled, &defltItemLooterSettings.itemLooterListEnabled);
	GetSliderInt("ItemLooterSettings", "ItemLooterListDistance", &customItemLooterSettings.itemLooterListDistance, &defltItemLooterSettings.itemLooterListDistance, 0, 3000);
	GetBool("ItemLooterSettings", "ItemLooterBlacklistToggle", &customItemLooterSettings.itemLooterBlacklistToggle, &defltItemLooterSettings.itemLooterBlacklistToggle);
	for (auto i = 0; i < 100; i++)
	{
		char itemEnabledText[sizeof("ItemLooterEnabled99")];
		char itemFormIdText[sizeof("ItemLooterList99")];
		sprintf_s(itemEnabledText, "ItemLooterEnabled%d", i);
		sprintf_s(itemFormIdText, "ItemLooterList%d", i);
		GetBool("ItemLooterSettings", itemEnabledText, &customItemLooterSettings.itemLooterEnabledList[i], &defltItemLooterSettings.itemLooterEnabledList[i]);
		GetDword("ItemLooterSettings", itemFormIdText, &customItemLooterSettings.itemLooterFormIdList[i], &defltItemLooterSettings.itemLooterFormIdList[i]);
	}
	for (auto i = 0; i < 64; i++)
	{
		char itemBlacklistEnabledText[sizeof("ItemLooterBlacklistEnabled63")];
		char itemBlacklistFormIdText[sizeof("ItemLooterBlacklist63")];
		sprintf_s(itemBlacklistEnabledText, "ItemLooterBlacklistEnabled%d", i);
		sprintf_s(itemBlacklistFormIdText, "ItemLooterBlacklist%d", i);
		GetBool("ItemLooterSettings", itemBlacklistEnabledText, &customItemLooterSettings.itemLooterBlacklistEnabled[i], &defltItemLooterSettings.itemLooterBlacklistEnabled[i]);
		GetDword("ItemLooterSettings", itemBlacklistFormIdText, &customItemLooterSettings.itemLooterBlacklist[i], &defltItemLooterSettings.itemLooterBlacklist[i]);
	}
	if (customItemLooterSettings.itemAutomaticSpeedMax < customItemLooterSettings.itemAutomaticSpeedMin)
	{
		customItemLooterSettings.itemAutomaticSpeedMax = customItemLooterSettings.itemAutomaticSpeedMin;
	}
}

void ErectusIni::SetItemLooterSettings()
{
	if (customItemLooterSettings.itemAutomaticSpeedMax < customItemLooterSettings.itemAutomaticSpeedMin)
	{
		customItemLooterSettings.itemAutomaticSpeedMax = customItemLooterSettings.itemAutomaticSpeedMin;
	}
	SetBool("ItemLooterSettings", "ItemKeybindEnabled", &customItemLooterSettings.itemKeybindEnabled, &defltItemLooterSettings.itemKeybindEnabled);
	SetBool("ItemLooterSettings", "ItemAutomaticLootingEnabled", &customItemLooterSettings.itemAutomaticLootingEnabled, &defltItemLooterSettings.itemAutomaticLootingEnabled);
	SetBool("ItemLooterSettings", "ItemAutomaticStatus", &customItemLooterSettings.itemAutomaticStatus, &defltItemLooterSettings.itemAutomaticStatus);
	SetSliderInt("ItemLooterSettings", "ItemAutomaticSpeedMin", &customItemLooterSettings.itemAutomaticSpeedMin, &defltItemLooterSettings.itemAutomaticSpeedMin, 10, 60);
	SetSliderInt("ItemLooterSettings", "ItemAutomaticSpeedMax", &customItemLooterSettings.itemAutomaticSpeedMax, &defltItemLooterSettings.itemAutomaticSpeedMax, 10, 60);
	SetBool("ItemLooterSettings", "ItemLooterWeaponsEnabled", &customItemLooterSettings.itemLooterWeaponsEnabled, &defltItemLooterSettings.itemLooterWeaponsEnabled);
	SetSliderInt("ItemLooterSettings", "ItemLooterWeaponsDistance", &customItemLooterSettings.itemLooterWeaponsDistance, &defltItemLooterSettings.itemLooterWeaponsDistance, 0, 3000);
	SetBool("ItemLooterSettings", "ItemLooterArmorEnabled", &customItemLooterSettings.itemLooterArmorEnabled, &defltItemLooterSettings.itemLooterArmorEnabled);
	SetSliderInt("ItemLooterSettings", "ItemLooterArmorDistance", &customItemLooterSettings.itemLooterArmorDistance, &defltItemLooterSettings.itemLooterArmorDistance, 0, 3000);
	SetBool("ItemLooterSettings", "ItemLooterAmmoEnabled", &customItemLooterSettings.itemLooterAmmoEnabled, &defltItemLooterSettings.itemLooterAmmoEnabled);
	SetSliderInt("ItemLooterSettings", "ItemLooterAmmoDistance", &customItemLooterSettings.itemLooterAmmoDistance, &defltItemLooterSettings.itemLooterAmmoDistance, 0, 3000);
	SetBool("ItemLooterSettings", "ItemLooterModsEnabled", &customItemLooterSettings.itemLooterModsEnabled, &defltItemLooterSettings.itemLooterModsEnabled);
	SetSliderInt("ItemLooterSettings", "ItemLooterModsDistance", &customItemLooterSettings.itemLooterModsDistance, &defltItemLooterSettings.itemLooterModsDistance, 0, 3000);
	SetBool("ItemLooterSettings", "ItemLooterMagazinesEnabled", &customItemLooterSettings.itemLooterMagazinesEnabled, &defltItemLooterSettings.itemLooterMagazinesEnabled);
	SetSliderInt("ItemLooterSettings", "ItemLooterMagazinesDistance", &customItemLooterSettings.itemLooterMagazinesDistance, &defltItemLooterSettings.itemLooterMagazinesDistance, 0, 3000);
	SetBool("ItemLooterSettings", "ItemLooterBobbleheadsEnabled", &customItemLooterSettings.itemLooterBobbleheadsEnabled, &defltItemLooterSettings.itemLooterBobbleheadsEnabled);
	SetSliderInt("ItemLooterSettings", "ItemLooterBobbleheadsDistance", &customItemLooterSettings.itemLooterBobbleheadsDistance, &defltItemLooterSettings.itemLooterBobbleheadsDistance, 0, 3000);
	SetBool("ItemLooterSettings", "ItemLooterAidEnabled", &customItemLooterSettings.itemLooterAidEnabled, &defltItemLooterSettings.itemLooterAidEnabled);
	SetSliderInt("ItemLooterSettings", "ItemLooterAidDistance", &customItemLooterSettings.itemLooterAidDistance, &defltItemLooterSettings.itemLooterAidDistance, 0, 3000);
	SetBool("ItemLooterSettings", "ItemLooterKnownPlansEnabled", &customItemLooterSettings.itemLooterKnownPlansEnabled, &defltItemLooterSettings.itemLooterKnownPlansEnabled);
	SetSliderInt("ItemLooterSettings", "ItemLooterKnownPlansDistance", &customItemLooterSettings.itemLooterKnownPlansDistance, &defltItemLooterSettings.itemLooterKnownPlansDistance, 0, 3000);
	SetBool("ItemLooterSettings", "ItemLooterUnknownPlansEnabled", &customItemLooterSettings.itemLooterUnknownPlansEnabled, &defltItemLooterSettings.itemLooterUnknownPlansEnabled);
	SetSliderInt("ItemLooterSettings", "ItemLooterUnknownPlansDistance", &customItemLooterSettings.itemLooterUnknownPlansDistance, &defltItemLooterSettings.itemLooterUnknownPlansDistance, 0, 3000);
	SetBool("ItemLooterSettings", "ItemLooterMiscEnabled", &customItemLooterSettings.itemLooterMiscEnabled, &defltItemLooterSettings.itemLooterMiscEnabled);
	SetSliderInt("ItemLooterSettings", "ItemLooterMiscDistance", &customItemLooterSettings.itemLooterMiscDistance, &defltItemLooterSettings.itemLooterMiscDistance, 0, 3000);
	SetBool("ItemLooterSettings", "ItemLooterUnlistedEnabled", &customItemLooterSettings.itemLooterUnlistedEnabled, &defltItemLooterSettings.itemLooterUnlistedEnabled);
	SetSliderInt("ItemLooterSettings", "ItemLooterUnlistedDistance", &customItemLooterSettings.itemLooterUnlistedDistance, &defltItemLooterSettings.itemLooterUnlistedDistance, 0, 3000);
	SetBool("ItemLooterSettings", "ItemLooterListEnabled", &customItemLooterSettings.itemLooterListEnabled, &defltItemLooterSettings.itemLooterListEnabled);
	SetSliderInt("ItemLooterSettings", "ItemLooterListDistance", &customItemLooterSettings.itemLooterListDistance, &defltItemLooterSettings.itemLooterListDistance, 0, 3000);
	SetBool("ItemLooterSettings", "ItemLooterBlacklistToggle", &customItemLooterSettings.itemLooterBlacklistToggle, &defltItemLooterSettings.itemLooterBlacklistToggle);
	for (auto i = 0; i < 100; i++)
	{
		char itemEnabledText[sizeof("ItemLooterEnabled99")];
		char itemFormIdText[sizeof("ItemLooterList99")];
		sprintf_s(itemEnabledText, "ItemLooterEnabled%d", i);
		sprintf_s(itemFormIdText, "ItemLooterList%d", i);
		SetBool("ItemLooterSettings", itemEnabledText, &customItemLooterSettings.itemLooterEnabledList[i], &defltItemLooterSettings.itemLooterEnabledList[i]);
		SetDword("ItemLooterSettings", itemFormIdText, &customItemLooterSettings.itemLooterFormIdList[i], &defltItemLooterSettings.itemLooterFormIdList[i]);
	}
	for (auto i = 0; i < 64; i++)
	{
		char itemBlacklistEnabledText[sizeof("ItemLooterBlacklistEnabled63")];
		char itemBlacklistFormIdText[sizeof("ItemLooterBlacklist63")];
		sprintf_s(itemBlacklistEnabledText, "ItemLooterBlacklistEnabled%d", i);
		sprintf_s(itemBlacklistFormIdText, "ItemLooterBlacklist%d", i);
		SetBool("ItemLooterSettings", itemBlacklistEnabledText, &customItemLooterSettings.itemLooterBlacklistEnabled[i], &defltItemLooterSettings.itemLooterBlacklistEnabled[i]);
		SetDword("ItemLooterSettings", itemBlacklistFormIdText, &customItemLooterSettings.itemLooterBlacklist[i], &defltItemLooterSettings.itemLooterBlacklist[i]);
	}
}

void ErectusIni::GetWeaponSettings()
{
	GetBool("WeaponSettings", "WeaponNoRecoil", &customWeaponSettings.noRecoil, &defaultWeaponSettings.noRecoil);
	GetBool("WeaponSettings", "WeaponNoSpread", &customWeaponSettings.noSpread, &defaultWeaponSettings.noSpread);
	GetBool("WeaponSettings", "WeaponNoSway", &customWeaponSettings.noSway, &defaultWeaponSettings.noSway);
	GetBool("WeaponSettings", "WeaponInfiniteAmmo", &customWeaponSettings.infiniteAmmo, &defaultWeaponSettings.infiniteAmmo);
	GetBool("WeaponSettings", "WeaponInstantReload", &customWeaponSettings.instantReload, &defaultWeaponSettings.instantReload);
	GetBool("WeaponSettings", "WeaponAutomaticflag", &customWeaponSettings.automaticflag, &defaultWeaponSettings.automaticflag);
	GetBool("WeaponSettings", "WeaponCapacityEnabled", &customWeaponSettings.capacityEnabled, &defaultWeaponSettings.capacityEnabled);
	GetSliderInt("WeaponSettings", "WeaponCapacity", &customWeaponSettings.capacity, &defaultWeaponSettings.capacity, 0, 999);
	GetBool("WeaponSettings", "WeaponSpeedEnabled", &customWeaponSettings.speedEnabled, &defaultWeaponSettings.speedEnabled);
	GetSliderFloat("WeaponSettings", "WeaponSpeed", &customWeaponSettings.speed, &defaultWeaponSettings.speed, 0.0f, 100.0f);
	GetBool("WeaponSettings", "WeaponReachEnabled", &customWeaponSettings.reachEnabled, &defaultWeaponSettings.reachEnabled);
	GetSliderFloat("WeaponSettings", "WeaponReach", &customWeaponSettings.reach, &defaultWeaponSettings.reach, 0.0f, 999.0f);
}

void ErectusIni::SetWeaponSettings()
{
	SetBool("WeaponSettings", "WeaponNoRecoil", &customWeaponSettings.noRecoil, &defaultWeaponSettings.noRecoil);
	SetBool("WeaponSettings", "WeaponNoSpread", &customWeaponSettings.noSpread, &defaultWeaponSettings.noSpread);
	SetBool("WeaponSettings", "WeaponNoSway", &customWeaponSettings.noSway, &defaultWeaponSettings.noSway);
	SetBool("WeaponSettings", "WeaponInfiniteAmmo", &customWeaponSettings.infiniteAmmo, &defaultWeaponSettings.infiniteAmmo);
	SetBool("WeaponSettings", "WeaponInstantReload", &customWeaponSettings.instantReload, &defaultWeaponSettings.instantReload);
	SetBool("WeaponSettings", "WeaponAutomaticflag", &customWeaponSettings.automaticflag, &defaultWeaponSettings.automaticflag);
	SetBool("WeaponSettings", "WeaponCapacityEnabled", &customWeaponSettings.capacityEnabled, &defaultWeaponSettings.capacityEnabled);
	SetSliderInt("WeaponSettings", "WeaponCapacity", &customWeaponSettings.capacity, &defaultWeaponSettings.capacity, 0, 999);
	SetBool("WeaponSettings", "WeaponSpeedEnabled", &customWeaponSettings.speedEnabled, &defaultWeaponSettings.speedEnabled);
	SetSliderFloat("WeaponSettings", "WeaponSpeed", &customWeaponSettings.speed, &defaultWeaponSettings.speed, 0.0f, 100.0f);
	SetBool("WeaponSettings", "WeaponReachEnabled", &customWeaponSettings.reachEnabled, &defaultWeaponSettings.reachEnabled);
	SetSliderFloat("WeaponSettings", "WeaponReach", &customWeaponSettings.reach, &defaultWeaponSettings.reach, 0.0f, 999.0f);
}

void ErectusIni::GetTargetSettings()
{
	GetBool("TargetSettings", "LockPlayers", &customTargetSettings.lockPlayers, &defltTargetSettings.lockPlayers);
	GetBool("TargetSettings", "LockNPCs", &customTargetSettings.lockNpCs, &defltTargetSettings.lockNpCs);
	GetBool("TargetSettings", "IndirectPlayers", &customTargetSettings.indirectPlayers, &defltTargetSettings.indirectPlayers);
	GetBool("TargetSettings", "IndirectNPCs", &customTargetSettings.indirectNpCs, &defltTargetSettings.indirectNpCs);
	GetBool("TargetSettings", "DirectPlayers", &customTargetSettings.directPlayers, &defltTargetSettings.directPlayers);
	GetBool("TargetSettings", "DirectNPCs", &customTargetSettings.directNpCs, &defltTargetSettings.directNpCs);
	GetBool("TargetSettings", "TargetLiving", &customTargetSettings.targetLiving, &defltTargetSettings.targetLiving);
	GetBool("TargetSettings", "TargetDowned", &customTargetSettings.targetDowned, &defltTargetSettings.targetDowned);
	GetBool("TargetSettings", "TargetDead", &customTargetSettings.targetDead, &defltTargetSettings.targetDead);
	GetBool("TargetSettings", "TargetUnknown", &customTargetSettings.targetUnknown, &defltTargetSettings.targetUnknown);
	GetBool("TargetSettings", "IgnoreRenderDistance", &customTargetSettings.ignoreRenderDistance, &defltTargetSettings.ignoreRenderDistance);
	GetSliderFloat("TargetSettings", "LockingFOV", &customTargetSettings.lockingFov, &defltTargetSettings.lockingFov, 5.0f, 40.0f);
	GetBool("TargetSettings", "IgnoreEssentialNPCs", &customTargetSettings.ignoreEssentialNpCs, &defltTargetSettings.ignoreEssentialNpCs);
	GetRgb("TargetSettings", "LockingColor", customTargetSettings.lockingColor, defltTargetSettings.lockingColor);
	GetBool("TargetSettings", "Retargeting", &customTargetSettings.retargeting, &defltTargetSettings.retargeting);
	GetSliderInt("TargetSettings", "Cooldown", &customTargetSettings.cooldown, &defltTargetSettings.cooldown, 0, 120);
	GetSliderInt("TargetSettings", "SendDamageMin", &customTargetSettings.sendDamageMin, &defltTargetSettings.sendDamageMin, 1, 60);
	GetSliderInt("TargetSettings", "SendDamageMax", &customTargetSettings.sendDamageMax, &defltTargetSettings.sendDamageMax, 1, 60);
	GetSliderInt("TargetSettings", "FavoriteIndex", &customTargetSettings.favoriteIndex, &defltTargetSettings.favoriteIndex, 0, 12);
	if (customTargetSettings.sendDamageMax < customTargetSettings.sendDamageMin)
	{
		customTargetSettings.sendDamageMax = customTargetSettings.sendDamageMin;
	}
}

void ErectusIni::SetTargetSettings()
{
	if (customTargetSettings.sendDamageMax < customTargetSettings.sendDamageMin)
	{
		customTargetSettings.sendDamageMax = customTargetSettings.sendDamageMin;
	}
	SetBool("TargetSettings", "LockPlayers", &customTargetSettings.lockPlayers, &defltTargetSettings.lockPlayers);
	SetBool("TargetSettings", "LockNPCs", &customTargetSettings.lockNpCs, &defltTargetSettings.lockNpCs);
	SetBool("TargetSettings", "IndirectPlayers", &customTargetSettings.indirectPlayers, &defltTargetSettings.indirectPlayers);
	SetBool("TargetSettings", "IndirectNPCs", &customTargetSettings.indirectNpCs, &defltTargetSettings.indirectNpCs);
	SetBool("TargetSettings", "DirectPlayers", &customTargetSettings.directPlayers, &defltTargetSettings.directPlayers);
	SetBool("TargetSettings", "DirectNPCs", &customTargetSettings.directNpCs, &defltTargetSettings.directNpCs);
	SetBool("TargetSettings", "TargetLiving", &customTargetSettings.targetLiving, &defltTargetSettings.targetLiving);
	SetBool("TargetSettings", "TargetDowned", &customTargetSettings.targetDowned, &defltTargetSettings.targetDowned);
	SetBool("TargetSettings", "TargetDead", &customTargetSettings.targetDead, &defltTargetSettings.targetDead);
	SetBool("TargetSettings", "TargetUnknown", &customTargetSettings.targetUnknown, &defltTargetSettings.targetUnknown);
	SetBool("TargetSettings", "IgnoreRenderDistance", &customTargetSettings.ignoreRenderDistance, &defltTargetSettings.ignoreRenderDistance);
	SetSliderFloat("TargetSettings", "LockingFOV", &customTargetSettings.lockingFov, &defltTargetSettings.lockingFov, 5.0f, 40.0f);
	SetBool("TargetSettings", "IgnoreEssentialNPCs", &customTargetSettings.ignoreEssentialNpCs, &defltTargetSettings.ignoreEssentialNpCs);
	SetRgb("TargetSettings", "LockingColor", customTargetSettings.lockingColor, defltTargetSettings.lockingColor);
	SetBool("TargetSettings", "Retargeting", &customTargetSettings.retargeting, &defltTargetSettings.retargeting);
	SetSliderInt("TargetSettings", "Cooldown", &customTargetSettings.cooldown, &defltTargetSettings.cooldown, 0, 120);
	SetSliderInt("TargetSettings", "SendDamageMin", &customTargetSettings.sendDamageMin, &defltTargetSettings.sendDamageMin, 1, 60);
	SetSliderInt("TargetSettings", "SendDamageMax", &customTargetSettings.sendDamageMax, &defltTargetSettings.sendDamageMax, 1, 60);
	SetSliderInt("TargetSettings", "FavoriteIndex", &customTargetSettings.favoriteIndex, &defltTargetSettings.favoriteIndex, 0, 12);
}

void ErectusIni::GetLocalPlayerSettings()
{
	GetBool("LocalPlayerSettings", "PositionSpoofingEnabled", &customLocalPlayerSettings.positionSpoofingEnabled, &defltLocalPlayerSettings.positionSpoofingEnabled);
	GetBool("LocalPlayerSettings", "DrawPositionSpoofingEnabled", &customLocalPlayerSettings.drawPositionSpoofingEnabled, &defltLocalPlayerSettings.drawPositionSpoofingEnabled);
	GetSliderInt("LocalPlayerSettings", "PositionSpoofingHeight", &customLocalPlayerSettings.positionSpoofingHeight, &defltLocalPlayerSettings.positionSpoofingHeight, -524287, 524287);
	GetBool("LocalPlayerSettings", "NoclipEnabled", &customLocalPlayerSettings.noclipEnabled, &defltLocalPlayerSettings.noclipEnabled);
	GetSliderFloat("LocalPlayerSettings", "NoclipSpeed", &customLocalPlayerSettings.noclipSpeed, &defltLocalPlayerSettings.noclipSpeed, 0.0f, 2.0f);
	GetBool("LocalPlayerSettings", "ClientState", &customLocalPlayerSettings.clientState, &defltLocalPlayerSettings.clientState);
	GetBool("LocalPlayerSettings", "AutomaticClientState", &customLocalPlayerSettings.automaticClientState, &defltLocalPlayerSettings.automaticClientState);
	GetBool("LocalPlayerSettings", "FreezeApEnabled", &customLocalPlayerSettings.freezeApEnabled, &defltLocalPlayerSettings.freezeApEnabled);
	GetBool("LocalPlayerSettings", "ActionPointsEnabled", &customLocalPlayerSettings.actionPointsEnabled, &defltLocalPlayerSettings.actionPointsEnabled);
	GetSliderInt("LocalPlayerSettings", "ActionPoints", &customLocalPlayerSettings.actionPoints, &defltLocalPlayerSettings.actionPoints, 0, 99999);
	GetBool("LocalPlayerSettings", "StrengthEnabled", &customLocalPlayerSettings.strengthEnabled, &defltLocalPlayerSettings.strengthEnabled);
	GetSliderInt("LocalPlayerSettings", "Strength", &customLocalPlayerSettings.strength, &defltLocalPlayerSettings.strength, 0, 99999);
	GetBool("LocalPlayerSettings", "PerceptionEnabled", &customLocalPlayerSettings.perceptionEnabled, &defltLocalPlayerSettings.perceptionEnabled);
	GetSliderInt("LocalPlayerSettings", "Perception", &customLocalPlayerSettings.perception, &defltLocalPlayerSettings.perception, 0, 99999);
	GetBool("LocalPlayerSettings", "EnduranceEnabled", &customLocalPlayerSettings.enduranceEnabled, &defltLocalPlayerSettings.enduranceEnabled);
	GetSliderInt("LocalPlayerSettings", "Endurance", &customLocalPlayerSettings.endurance, &defltLocalPlayerSettings.endurance, 0, 99999);
	GetBool("LocalPlayerSettings", "CharismaEnabled", &customLocalPlayerSettings.charismaEnabled, &defltLocalPlayerSettings.charismaEnabled);
	GetSliderInt("LocalPlayerSettings", "Charisma", &customLocalPlayerSettings.charisma, &defltLocalPlayerSettings.charisma, 0, 99999);
	GetBool("LocalPlayerSettings", "IntelligenceEnabled", &customLocalPlayerSettings.intelligenceEnabled, &defltLocalPlayerSettings.intelligenceEnabled);
	GetSliderInt("LocalPlayerSettings", "Intelligence", &customLocalPlayerSettings.intelligence, &defltLocalPlayerSettings.intelligence, 0, 99999);
	GetBool("LocalPlayerSettings", "AgilityEnabled", &customLocalPlayerSettings.agilityEnabled, &defltLocalPlayerSettings.agilityEnabled);
	GetSliderInt("LocalPlayerSettings", "Agility", &customLocalPlayerSettings.agility, &defltLocalPlayerSettings.agility, 0, 99999);
	GetBool("LocalPlayerSettings", "LuckEnabled", &customLocalPlayerSettings.luckEnabled, &defltLocalPlayerSettings.luckEnabled);
	GetSliderInt("LocalPlayerSettings", "Luck", &customLocalPlayerSettings.luck, &defltLocalPlayerSettings.luck, 0, 99999);
}

void ErectusIni::SetLocalPlayerSettings()
{
	SetBool("LocalPlayerSettings", "PositionSpoofingEnabled", &customLocalPlayerSettings.positionSpoofingEnabled, &defltLocalPlayerSettings.positionSpoofingEnabled);
	SetBool("LocalPlayerSettings", "DrawPositionSpoofingEnabled", &customLocalPlayerSettings.drawPositionSpoofingEnabled, &defltLocalPlayerSettings.drawPositionSpoofingEnabled);
	SetSliderInt("LocalPlayerSettings", "PositionSpoofingHeight", &customLocalPlayerSettings.positionSpoofingHeight, &defltLocalPlayerSettings.positionSpoofingHeight, -524287, 524287);
	SetBool("LocalPlayerSettings", "NoclipEnabled", &customLocalPlayerSettings.noclipEnabled, &defltLocalPlayerSettings.noclipEnabled);
	SetSliderFloat("LocalPlayerSettings", "NoclipSpeed", &customLocalPlayerSettings.noclipSpeed, &defltLocalPlayerSettings.noclipSpeed, 0.0f, 2.0f);
	SetBool("LocalPlayerSettings", "ClientState", &customLocalPlayerSettings.clientState, &defltLocalPlayerSettings.clientState);
	SetBool("LocalPlayerSettings", "AutomaticClientState", &customLocalPlayerSettings.automaticClientState, &defltLocalPlayerSettings.automaticClientState);
	SetBool("LocalPlayerSettings", "FreezeApEnabled", &customLocalPlayerSettings.freezeApEnabled, &defltLocalPlayerSettings.freezeApEnabled);
	SetBool("LocalPlayerSettings", "ActionPointsEnabled", &customLocalPlayerSettings.actionPointsEnabled, &defltLocalPlayerSettings.actionPointsEnabled);
	SetSliderInt("LocalPlayerSettings", "ActionPoints", &customLocalPlayerSettings.actionPoints, &defltLocalPlayerSettings.actionPoints, 0, 99999);
	SetBool("LocalPlayerSettings", "StrengthEnabled", &customLocalPlayerSettings.strengthEnabled, &defltLocalPlayerSettings.strengthEnabled);
	SetSliderInt("LocalPlayerSettings", "Strength", &customLocalPlayerSettings.strength, &defltLocalPlayerSettings.strength, 0, 99999);
	SetBool("LocalPlayerSettings", "PerceptionEnabled", &customLocalPlayerSettings.perceptionEnabled, &defltLocalPlayerSettings.perceptionEnabled);
	SetSliderInt("LocalPlayerSettings", "Perception", &customLocalPlayerSettings.perception, &defltLocalPlayerSettings.perception, 0, 99999);
	SetBool("LocalPlayerSettings", "EnduranceEnabled", &customLocalPlayerSettings.enduranceEnabled, &defltLocalPlayerSettings.enduranceEnabled);
	SetSliderInt("LocalPlayerSettings", "Endurance", &customLocalPlayerSettings.endurance, &defltLocalPlayerSettings.endurance, 0, 99999);
	SetBool("LocalPlayerSettings", "CharismaEnabled", &customLocalPlayerSettings.charismaEnabled, &defltLocalPlayerSettings.charismaEnabled);
	SetSliderInt("LocalPlayerSettings", "Charisma", &customLocalPlayerSettings.charisma, &defltLocalPlayerSettings.charisma, 0, 99999);
	SetBool("LocalPlayerSettings", "IntelligenceEnabled", &customLocalPlayerSettings.intelligenceEnabled, &defltLocalPlayerSettings.intelligenceEnabled);
	SetSliderInt("LocalPlayerSettings", "Intelligence", &customLocalPlayerSettings.intelligence, &defltLocalPlayerSettings.intelligence, 0, 99999);
	SetBool("LocalPlayerSettings", "AgilityEnabled", &customLocalPlayerSettings.agilityEnabled, &defltLocalPlayerSettings.agilityEnabled);
	SetSliderInt("LocalPlayerSettings", "Agility", &customLocalPlayerSettings.agility, &defltLocalPlayerSettings.agility, 0, 99999);
	SetBool("LocalPlayerSettings", "LuckEnabled", &customLocalPlayerSettings.luckEnabled, &defltLocalPlayerSettings.luckEnabled);
	SetSliderInt("LocalPlayerSettings", "Luck", &customLocalPlayerSettings.luck, &defltLocalPlayerSettings.luck, 0, 99999);
}

void ErectusIni::GetOpkSettings()
{
	GetBool("OpkSettings", "OpkPlayersEnabled", &customOpkSettings.playersEnabled, &defltOpkSettings.playersEnabled);
	GetBool("OpkSettings", "OpkNpcsEnabled", &customOpkSettings.npcsEnabled, &defltOpkSettings.npcsEnabled);
}

void ErectusIni::SetOpkSettings()
{
	SetBool("OpkSettings", "OpkPlayersEnabled", &customOpkSettings.playersEnabled, &defltOpkSettings.playersEnabled);
	SetBool("OpkSettings", "OpkNpcsEnabled", &customOpkSettings.npcsEnabled, &defltOpkSettings.npcsEnabled);
}

void ErectusIni::GetUtilitySettings()
{
	GetBool("UtilitySettings", "DebugPlayer", &customUtilitySettings.debugPlayer, &defltUtilitySettings.debugPlayer);
	GetBool("UtilitySettings", "DebugEsp", &customUtilitySettings.debugEsp, &defltUtilitySettings.debugEsp);
	GetDword("UtilitySettings", "PtrFormId", &customUtilitySettings.ptrFormId, &defltUtilitySettings.ptrFormId);
	GetDword("UtilitySettings", "AddressFormId", &customUtilitySettings.addressFormId, &defltUtilitySettings.addressFormId);
}

void ErectusIni::SetUtilitySettings()
{
	SetBool("UtilitySettings", "DebugPlayer", &customUtilitySettings.debugPlayer, &defltUtilitySettings.debugPlayer);
	SetBool("UtilitySettings", "DebugEsp", &customUtilitySettings.debugEsp, &defltUtilitySettings.debugEsp);
	SetDword("UtilitySettings", "PtrFormId", &customUtilitySettings.ptrFormId, &defltUtilitySettings.ptrFormId);
	SetDword("UtilitySettings", "AddressFormId", &customUtilitySettings.addressFormId, &defltUtilitySettings.addressFormId);
}

void ErectusIni::GetSwapperSettings()
{
	GetDword("SwapperSettings", "SourceFormId", &customSwapperSettings.sourceFormId, &defltSwapperSettings.sourceFormId);
	GetDword("SwapperSettings", "DestinationFormId", &customSwapperSettings.destinationFormId, &defltSwapperSettings.destinationFormId);
}

void ErectusIni::SetSwapperSettings()
{
	SetDword("SwapperSettings", "SourceFormId", &customSwapperSettings.sourceFormId, &defltSwapperSettings.sourceFormId);
	SetDword("SwapperSettings", "DestinationFormId", &customSwapperSettings.destinationFormId, &defltSwapperSettings.destinationFormId);
}

void ErectusIni::GetTransferSettings()
{
	GetDword("TransferSettings", "SourceFormId", &customTransferSettings.sourceFormId, &defltTransferSettings.sourceFormId);
	GetDword("TransferSettings", "DestinationFormId", &customTransferSettings.destinationFormId, &defltTransferSettings.destinationFormId);
	GetBool("TransferSettings", "UseWhitelist", &customTransferSettings.useWhitelist, &defltTransferSettings.useWhitelist);
	GetBool("TransferSettings", "UseBlacklist", &customTransferSettings.useBlacklist, &defltTransferSettings.useBlacklist);
	for (auto i = 0; i < 32; i++)
	{
		char whitelistedText[sizeof("Whitelisted31")];
		char whitelistText[sizeof("Whitelist31")];
		sprintf_s(whitelistedText, "Whitelisted%d", i);
		sprintf_s(whitelistText, "Whitelist%d", i);
		GetBool("TransferSettings", whitelistedText, &customTransferSettings.whitelisted[i], &defltTransferSettings.whitelisted[i]);
		GetDword("TransferSettings", whitelistText, &customTransferSettings.whitelist[i], &defltTransferSettings.whitelist[i]);
	}
	for (auto i = 0; i < 32; i++)
	{
		char blacklistedText[sizeof("Blacklisted31")];
		char blacklistText[sizeof("Blacklist31")];
		sprintf_s(blacklistedText, "Blacklisted%d", i);
		sprintf_s(blacklistText, "Blacklist%d", i);
		GetBool("TransferSettings", blacklistedText, &customTransferSettings.blacklisted[i], &defltTransferSettings.blacklisted[i]);
		GetDword("TransferSettings", blacklistText, &customTransferSettings.blacklist[i], &defltTransferSettings.blacklist[i]);
	}
}

void ErectusIni::SetTransferSettings()
{
	SetDword("TransferSettings", "SourceFormId", &customTransferSettings.sourceFormId, &defltTransferSettings.sourceFormId);
	SetDword("TransferSettings", "DestinationFormId", &customTransferSettings.destinationFormId, &defltTransferSettings.destinationFormId);
	SetBool("TransferSettings", "UseWhitelist", &customTransferSettings.useWhitelist, &defltTransferSettings.useWhitelist);
	SetBool("TransferSettings", "UseBlacklist", &customTransferSettings.useBlacklist, &defltTransferSettings.useBlacklist);
	for (auto i = 0; i < 32; i++)
	{
		char whitelistedText[sizeof("Whitelisted31")];
		char whitelistText[sizeof("Whitelist31")];
		sprintf_s(whitelistedText, "Whitelisted%d", i);
		sprintf_s(whitelistText, "Whitelist%d", i);
		SetBool("TransferSettings", whitelistedText, &customTransferSettings.whitelisted[i], &defltTransferSettings.whitelisted[i]);
		SetDword("TransferSettings", whitelistText, &customTransferSettings.whitelist[i], &defltTransferSettings.whitelist[i]);
	}
	for (auto i = 0; i < 32; i++)
	{
		char blacklistedText[sizeof("Blacklisted31")];
		char blacklistText[sizeof("Blacklist31")];
		sprintf_s(blacklistedText, "Blacklisted%d", i);
		sprintf_s(blacklistText, "Blacklist%d", i);
		SetBool("TransferSettings", blacklistedText, &customTransferSettings.blacklisted[i], &defltTransferSettings.blacklisted[i]);
		SetDword("TransferSettings", blacklistText, &customTransferSettings.blacklist[i], &defltTransferSettings.blacklist[i]);
	}
}

void ErectusIni::GetTeleportSettings()
{
	for (auto i = 0; i < 16; i++)
	{
		char destinationText[sizeof("Destination15")];
		char cellFormIdText[sizeof("CellFormId15")];
		char disableSavingText[sizeof("DisableSaving15")];
		sprintf_s(destinationText, "Destination%d", i);
		sprintf_s(cellFormIdText, "CellFormId%d", i);
		sprintf_s(disableSavingText, "DisableSaving%d", i);
		GetQuadFloat("TeleportSettings", destinationText, customTeleportSettings.teleportEntryData[i].destination, defltTeleportSettings.teleportEntryData[i].destination);
		GetDword("TeleportSettings", cellFormIdText, &customTeleportSettings.teleportEntryData[i].cellFormId, &defltTeleportSettings.teleportEntryData[i].cellFormId);
		GetBool("TeleportSettings", disableSavingText, &customTeleportSettings.teleportEntryData[i].disableSaving, &defltTeleportSettings.teleportEntryData[i].disableSaving);
	}
}

void ErectusIni::SetTeleportSettings()
{
	for (auto i = 0; i < 16; i++)
	{
		char destinationText[sizeof("Destination15")];
		char cellFormIdText[sizeof("CellFormId15")];
		char disableSavingText[sizeof("DisableSaving15")];
		sprintf_s(destinationText, "Destination%d", i);
		sprintf_s(cellFormIdText, "CellFormId%d", i);
		sprintf_s(disableSavingText, "DisableSaving%d", i);
		SetQuadFloat("TeleportSettings", destinationText, customTeleportSettings.teleportEntryData[i].destination, defltTeleportSettings.teleportEntryData[i].destination);
		SetDword("TeleportSettings", cellFormIdText, &customTeleportSettings.teleportEntryData[i].cellFormId, &defltTeleportSettings.teleportEntryData[i].cellFormId);
		SetBool("TeleportSettings", disableSavingText, &customTeleportSettings.teleportEntryData[i].disableSaving, &defltTeleportSettings.teleportEntryData[i].disableSaving);
	}
}

void ErectusIni::GetNukeCodeSettings()
{
	GetBool("NukeCodeSettings", "AutomaticNukeCodes", &customNukeCodeSettings.automaticNukeCodes, &defltNukeCodeSettings.automaticNukeCodes);
	GetBool("NukeCodeSettings", "DrawCodeAlpha", &customNukeCodeSettings.drawCodeAlpha, &defltNukeCodeSettings.drawCodeAlpha);
	GetBool("NukeCodeSettings", "DrawCodeBravo", &customNukeCodeSettings.drawCodeBravo, &defltNukeCodeSettings.drawCodeBravo);
	GetBool("NukeCodeSettings", "DrawCodeCharlie", &customNukeCodeSettings.drawCodeCharlie, &defltNukeCodeSettings.drawCodeCharlie);
}

void ErectusIni::SetNukeCodeSettings()
{
	SetBool("NukeCodeSettings", "AutomaticNukeCodes", &customNukeCodeSettings.automaticNukeCodes, &defltNukeCodeSettings.automaticNukeCodes);
	SetBool("NukeCodeSettings", "DrawCodeAlpha", &customNukeCodeSettings.drawCodeAlpha, &defltNukeCodeSettings.drawCodeAlpha);
	SetBool("NukeCodeSettings", "DrawCodeBravo", &customNukeCodeSettings.drawCodeBravo, &defltNukeCodeSettings.drawCodeBravo);
	SetBool("NukeCodeSettings", "DrawCodeCharlie", &customNukeCodeSettings.drawCodeCharlie, &defltNukeCodeSettings.drawCodeCharlie);
}

void ErectusIni::GetLegendarySettings()
{
	GetBool("LegendarySettings", "OverrideLivingOneStar", &customLegendarySettings.overrideLivingOneStar, &defltLegendarySettings.overrideLivingOneStar);
	GetRgb("LegendarySettings", "LivingOneStarColor", customLegendarySettings.livingOneStarColor, defltLegendarySettings.livingOneStarColor);
	GetBool("LegendarySettings", "OverrideDeadOneStar", &customLegendarySettings.overrideDeadOneStar, &defltLegendarySettings.overrideDeadOneStar);
	GetRgb("LegendarySettings", "DeadOneStarColor", customLegendarySettings.deadOneStarColor, defltLegendarySettings.deadOneStarColor);
	GetBool("LegendarySettings", "OverrideLivingTwoStar", &customLegendarySettings.overrideLivingTwoStar, &defltLegendarySettings.overrideLivingTwoStar);
	GetRgb("LegendarySettings", "LivingTwoStarColor", customLegendarySettings.livingTwoStarColor, defltLegendarySettings.livingTwoStarColor);
	GetBool("LegendarySettings", "OverrideDeadTwoStar", &customLegendarySettings.overrideDeadTwoStar, &defltLegendarySettings.overrideDeadTwoStar);
	GetRgb("LegendarySettings", "DeadTwoStarColor", customLegendarySettings.deadTwoStarColor, defltLegendarySettings.deadTwoStarColor);
	GetBool("LegendarySettings", "OverrideLivingThreeStar", &customLegendarySettings.overrideLivingThreeStar, &defltLegendarySettings.overrideLivingThreeStar);
	GetRgb("LegendarySettings", "LivingThreeStarColor", customLegendarySettings.livingThreeStarColor, defltLegendarySettings.livingThreeStarColor);
	GetBool("LegendarySettings", "OverrideDeadThreeStar", &customLegendarySettings.overrideDeadThreeStar, &defltLegendarySettings.overrideDeadThreeStar);
	GetRgb("LegendarySettings", "DeadThreeStarColor", customLegendarySettings.deadThreeStarColor, defltLegendarySettings.deadThreeStarColor);
}

void ErectusIni::SetLegendarySettings()
{
	SetBool("LegendarySettings", "OverrideLivingOneStar", &customLegendarySettings.overrideLivingOneStar, &defltLegendarySettings.overrideLivingOneStar);
	SetRgb("LegendarySettings", "LivingOneStarColor", customLegendarySettings.livingOneStarColor, defltLegendarySettings.livingOneStarColor);
	SetBool("LegendarySettings", "OverrideDeadOneStar", &customLegendarySettings.overrideDeadOneStar, &defltLegendarySettings.overrideDeadOneStar);
	SetRgb("LegendarySettings", "DeadOneStarColor", customLegendarySettings.deadOneStarColor, defltLegendarySettings.deadOneStarColor);
	SetBool("LegendarySettings", "OverrideLivingTwoStar", &customLegendarySettings.overrideLivingTwoStar, &defltLegendarySettings.overrideLivingTwoStar);
	SetRgb("LegendarySettings", "LivingTwoStarColor", customLegendarySettings.livingTwoStarColor, defltLegendarySettings.livingTwoStarColor);
	SetBool("LegendarySettings", "OverrideDeadTwoStar", &customLegendarySettings.overrideDeadTwoStar, &defltLegendarySettings.overrideDeadTwoStar);
	SetRgb("LegendarySettings", "DeadTwoStarColor", customLegendarySettings.deadTwoStarColor, defltLegendarySettings.deadTwoStarColor);
	SetBool("LegendarySettings", "OverrideLivingThreeStar", &customLegendarySettings.overrideLivingThreeStar, &defltLegendarySettings.overrideLivingThreeStar);
	SetRgb("LegendarySettings", "LivingThreeStarColor", customLegendarySettings.livingThreeStarColor, defltLegendarySettings.livingThreeStarColor);
	SetBool("LegendarySettings", "OverrideDeadThreeStar", &customLegendarySettings.overrideDeadThreeStar, &defltLegendarySettings.overrideDeadThreeStar);
	SetRgb("LegendarySettings", "DeadThreeStarColor", customLegendarySettings.deadThreeStarColor, defltLegendarySettings.deadThreeStarColor);
}

void ErectusIni::GetFluxSettings()
{
	GetBool("FluxSettings", "CrimsonFluxEnabled", &customFluxSettings.crimsonFluxEnabled, &defltFluxSettings.crimsonFluxEnabled);
	GetBool("FluxSettings", "CobaltFluxEnabled", &customFluxSettings.cobaltFluxEnabled, &defltFluxSettings.cobaltFluxEnabled);
	GetBool("FluxSettings", "YellowcakeFluxEnabled", &customFluxSettings.yellowcakeFluxEnabled, &defltFluxSettings.yellowcakeFluxEnabled);
	GetBool("FluxSettings", "FluorescentFluxEnabled", &customFluxSettings.fluorescentFluxEnabled, &defltFluxSettings.fluorescentFluxEnabled);
	GetBool("FluxSettings", "VioletFluxEnabled", &customFluxSettings.violetFluxEnabled, &defltFluxSettings.violetFluxEnabled);
}

void ErectusIni::SetFluxSettings()
{
	SetBool("FluxSettings", "CrimsonFluxEnabled", &customFluxSettings.crimsonFluxEnabled, &defltFluxSettings.crimsonFluxEnabled);
	SetBool("FluxSettings", "CobaltFluxEnabled", &customFluxSettings.cobaltFluxEnabled, &defltFluxSettings.cobaltFluxEnabled);
	SetBool("FluxSettings", "YellowcakeFluxEnabled", &customFluxSettings.yellowcakeFluxEnabled, &defltFluxSettings.yellowcakeFluxEnabled);
	SetBool("FluxSettings", "FluorescentFluxEnabled", &customFluxSettings.fluorescentFluxEnabled, &defltFluxSettings.fluorescentFluxEnabled);
	SetBool("FluxSettings", "VioletFluxEnabled", &customFluxSettings.violetFluxEnabled, &defltFluxSettings.violetFluxEnabled);
}

void ErectusIni::GetEntityLooterSettings(const char *section, EntityLooterSettings* value, EntityLooterSettings* deflt)
{
	GetBool(section, "EntityLooterEnabled", &value->entityLooterEnabled, &deflt->entityLooterEnabled);
	GetBool(section, "EntityLooterStatusEnabled", &value->entityLooterStatusEnabled, &deflt->entityLooterStatusEnabled);
	GetBool(section, "EntityLooterAllWeaponsEnabled", &value->entityLooterAllWeaponsEnabled, &deflt->entityLooterAllWeaponsEnabled);
	GetBool(section, "EntityLooterAllArmorEnabled", &value->entityLooterAllArmorEnabled, &deflt->entityLooterAllArmorEnabled);
	GetBool(section, "EntityLooterOneStarWeaponsEnabled", &value->entityLooterOneStarWeaponsEnabled, &deflt->entityLooterOneStarWeaponsEnabled);
	GetBool(section, "EntityLooterOneStarArmorEnabled", &value->entityLooterOneStarArmorEnabled, &deflt->entityLooterOneStarArmorEnabled);
	GetBool(section, "EntityLooterTwoStarWeaponsEnabled", &value->entityLooterTwoStarWeaponsEnabled, &deflt->entityLooterTwoStarWeaponsEnabled);
	GetBool(section, "EntityLooterTwoStarArmorEnabled", &value->entityLooterTwoStarArmorEnabled, &deflt->entityLooterTwoStarArmorEnabled);
	GetBool(section, "EntityLooterThreeStarWeaponsEnabled", &value->entityLooterThreeStarWeaponsEnabled, &deflt->entityLooterThreeStarWeaponsEnabled);
	GetBool(section, "EntityLooterThreeStarArmorEnabled", &value->entityLooterThreeStarArmorEnabled, &deflt->entityLooterThreeStarArmorEnabled);
	GetBool(section, "EntityLooterAmmoEnabled", &value->entityLooterAmmoEnabled, &deflt->entityLooterAmmoEnabled);
	GetBool(section, "EntityLooterModsEnabled", &value->entityLooterModsEnabled, &deflt->entityLooterModsEnabled);
	GetBool(section, "EntityLooterCapsEnabled", &value->entityLooterCapsEnabled, &deflt->entityLooterCapsEnabled);
	GetBool(section, "EntityLooterJunkEnabled", &value->entityLooterJunkEnabled, &deflt->entityLooterJunkEnabled);
	GetBool(section, "EntityLooterAidEnabled", &value->entityLooterAidEnabled, &deflt->entityLooterAidEnabled);
	GetBool(section, "EntityLooterTreasureMapsEnabled", &value->entityLooterTreasureMapsEnabled, &deflt->entityLooterTreasureMapsEnabled);
	GetBool(section, "EntityLooterKnownPlansEnabled", &value->entityLooterKnownPlansEnabled, &deflt->entityLooterKnownPlansEnabled);
	GetBool(section, "EntityLooterUnknownPlansEnabled", &value->entityLooterUnknownPlansEnabled, &deflt->entityLooterUnknownPlansEnabled);
	GetBool(section, "EntityLooterMiscEnabled", &value->entityLooterMiscEnabled, &deflt->entityLooterMiscEnabled);
	GetBool(section, "EntityLooterUnlistedEnabled", &value->entityLooterUnlistedEnabled, &deflt->entityLooterUnlistedEnabled);
	GetBool(section, "EntityLooterListEnabled", &value->entityLooterListEnabled, &deflt->entityLooterListEnabled);
	GetBool(section, "EntityLooterBlacklistToggle", &value->entityLooterBlacklistToggle, &deflt->entityLooterBlacklistToggle);
	for (auto i = 0; i < 100; i++)
	{
		char entityEnabledText[sizeof("EntityLooterEnabled99")];
		char entityFormIdText[sizeof("EntityLooterList99")];
		sprintf_s(entityEnabledText, "EntityLooterEnabled%d", i);
		sprintf_s(entityFormIdText, "EntityLooterList%d", i);
		GetBool(section, entityEnabledText, &value->entityLooterEnabledList[i], &deflt->entityLooterEnabledList[i]);
		GetDword(section, entityFormIdText, &value->entityLooterFormIdList[i], &deflt->entityLooterFormIdList[i]);
	}
	for (auto i = 0; i < 64; i++)
	{
		char entityBlacklistEnabledText[sizeof("EntityLooterBlacklistEnabled63")];
		char entityBlacklistFormIdText[sizeof("EntityLooterBlacklist63")];
		sprintf_s(entityBlacklistEnabledText, "EntityLooterBlacklistEnabled%d", i);
		sprintf_s(entityBlacklistFormIdText, "EntityLooterBlacklist%d", i);
		GetBool(section, entityBlacklistEnabledText, &value->entityLooterBlacklistEnabled[i], &deflt->entityLooterBlacklistEnabled[i]);
		GetDword(section, entityBlacklistFormIdText, &value->entityLooterBlacklist[i], &deflt->entityLooterBlacklist[i]);
	}
}

void ErectusIni::SetEntityLooterSettings(const char *section, EntityLooterSettings* value, EntityLooterSettings* deflt)
{
	SetBool(section, "EntityLooterEnabled", &value->entityLooterEnabled, &deflt->entityLooterEnabled);
	SetBool(section, "EntityLooterStatusEnabled", &value->entityLooterStatusEnabled, &deflt->entityLooterStatusEnabled);
	SetBool(section, "EntityLooterAllWeaponsEnabled", &value->entityLooterAllWeaponsEnabled, &deflt->entityLooterAllWeaponsEnabled);
	SetBool(section, "EntityLooterAllArmorEnabled", &value->entityLooterAllArmorEnabled, &deflt->entityLooterAllArmorEnabled);
	SetBool(section, "EntityLooterOneStarWeaponsEnabled", &value->entityLooterOneStarWeaponsEnabled, &deflt->entityLooterOneStarWeaponsEnabled);
	SetBool(section, "EntityLooterOneStarArmorEnabled", &value->entityLooterOneStarArmorEnabled, &deflt->entityLooterOneStarArmorEnabled);
	SetBool(section, "EntityLooterTwoStarWeaponsEnabled", &value->entityLooterTwoStarWeaponsEnabled, &deflt->entityLooterTwoStarWeaponsEnabled);
	SetBool(section, "EntityLooterTwoStarArmorEnabled", &value->entityLooterTwoStarArmorEnabled, &deflt->entityLooterTwoStarArmorEnabled);
	SetBool(section, "EntityLooterThreeStarWeaponsEnabled", &value->entityLooterThreeStarWeaponsEnabled, &deflt->entityLooterThreeStarWeaponsEnabled);
	SetBool(section, "EntityLooterThreeStarArmorEnabled", &value->entityLooterThreeStarArmorEnabled, &deflt->entityLooterThreeStarArmorEnabled);
	SetBool(section, "EntityLooterAmmoEnabled", &value->entityLooterAmmoEnabled, &deflt->entityLooterAmmoEnabled);
	SetBool(section, "EntityLooterModsEnabled", &value->entityLooterModsEnabled, &deflt->entityLooterModsEnabled);
	SetBool(section, "EntityLooterCapsEnabled", &value->entityLooterCapsEnabled, &deflt->entityLooterCapsEnabled);
	SetBool(section, "EntityLooterJunkEnabled", &value->entityLooterJunkEnabled, &deflt->entityLooterJunkEnabled);
	SetBool(section, "EntityLooterAidEnabled", &value->entityLooterAidEnabled, &deflt->entityLooterAidEnabled);
	SetBool(section, "EntityLooterTreasureMapsEnabled", &value->entityLooterTreasureMapsEnabled, &deflt->entityLooterTreasureMapsEnabled);
	SetBool(section, "EntityLooterKnownPlansEnabled", &value->entityLooterKnownPlansEnabled, &deflt->entityLooterKnownPlansEnabled);
	SetBool(section, "EntityLooterUnknownPlansEnabled", &value->entityLooterUnknownPlansEnabled, &deflt->entityLooterUnknownPlansEnabled);
	SetBool(section, "EntityLooterMiscEnabled", &value->entityLooterMiscEnabled, &deflt->entityLooterMiscEnabled);
	SetBool(section, "EntityLooterUnlistedEnabled", &value->entityLooterUnlistedEnabled, &deflt->entityLooterUnlistedEnabled);
	SetBool(section, "EntityLooterListEnabled", &value->entityLooterListEnabled, &deflt->entityLooterListEnabled);
	SetBool(section, "EntityLooterBlacklistToggle", &value->entityLooterBlacklistToggle, &deflt->entityLooterBlacklistToggle);
	for (auto i = 0; i < 100; i++)
	{
		char entityEnabledText[sizeof("EntityLooterEnabled99")];
		char entityFormIdText[sizeof("EntityLooterList99")];
		sprintf_s(entityEnabledText, "EntityLooterEnabled%d", i);
		sprintf_s(entityFormIdText, "EntityLooterList%d", i);
		SetBool(section, entityEnabledText, &value->entityLooterEnabledList[i], &deflt->entityLooterEnabledList[i]);
		SetDword(section, entityFormIdText, &value->entityLooterFormIdList[i], &deflt->entityLooterFormIdList[i]);
	}
	for (auto i = 0; i < 64; i++)
	{
		char entityBlacklistEnabledText[sizeof("EntityLooterBlacklistEnabled63")];
		char entityBlacklistFormIdText[sizeof("EntityLooterBlacklist63")];
		sprintf_s(entityBlacklistEnabledText, "EntityLooterBlacklistEnabled%d", i);
		sprintf_s(entityBlacklistFormIdText, "EntityLooterBlacklist%d", i);
		SetBool(section, entityBlacklistEnabledText, &value->entityLooterBlacklistEnabled[i], &deflt->entityLooterBlacklistEnabled[i]);
		SetDword(section, entityBlacklistFormIdText, &value->entityLooterBlacklist[i], &deflt->entityLooterBlacklist[i]);
	}
}

void ErectusIni::GetHarvesterSettings()
{
	GetBool("HarvesterSettings", "HarvesterEnabled", &customHarvesterSettings.harvesterEnabled, &defltHarvesterSettings.harvesterEnabled);
	GetBool("HarvesterSettings", "HarvesterStatusEnabled", &customHarvesterSettings.harvesterStatusEnabled, &defltHarvesterSettings.harvesterStatusEnabled);
	GetBool("HarvesterSettings", "HarvesterOverrideEnabled", &customHarvesterSettings.harvesterOverrideEnabled, &defltHarvesterSettings.harvesterOverrideEnabled);
	for (auto i = 0; i < 69; i++)
	{
		char harvesterEnabledText[sizeof("HarvesterEnabled68")];
		sprintf_s(harvesterEnabledText, "HarvesterEnabled%d", i);
		GetBool("HarvesterSettings", harvesterEnabledText, &customHarvesterSettings.harvesterEnabledList[i], &defltHarvesterSettings.harvesterEnabledList[i]);
	}
}

void ErectusIni::SetHarvesterSettings()
{
	SetBool("HarvesterSettings", "HarvesterEnabled", &customHarvesterSettings.harvesterEnabled, &defltHarvesterSettings.harvesterEnabled);
	SetBool("HarvesterSettings", "HarvesterStatusEnabled", &customHarvesterSettings.harvesterStatusEnabled, &defltHarvesterSettings.harvesterStatusEnabled);
	SetBool("HarvesterSettings", "HarvesterOverrideEnabled", &customHarvesterSettings.harvesterOverrideEnabled, &defltHarvesterSettings.harvesterOverrideEnabled);
	for (auto i = 0; i < 69; i++)
	{
		char harvesterEnabledText[sizeof("HarvesterEnabled68")];
		sprintf_s(harvesterEnabledText, "HarvesterEnabled%d", i);
		SetBool("HarvesterSettings", harvesterEnabledText, &customHarvesterSettings.harvesterEnabledList[i], &defltHarvesterSettings.harvesterEnabledList[i]);
	}
}

void ErectusIni::GetKnownRecipeSettings()
{
	GetBool("KnownRecipeSettings", "KnownRecipesEnabled", &customKnownRecipeSettings.knownRecipesEnabled, &defltKnownRecipeSettings.knownRecipesEnabled);
	GetBool("KnownRecipeSettings", "UnknownRecipesEnabled", &customKnownRecipeSettings.unknownRecipesEnabled, &defltKnownRecipeSettings.unknownRecipesEnabled);
}

void ErectusIni::SetKnownRecipeSettings()
{
	SetBool("KnownRecipeSettings", "KnownRecipesEnabled", &customKnownRecipeSettings.knownRecipesEnabled, &defltKnownRecipeSettings.knownRecipesEnabled);
	SetBool("KnownRecipeSettings", "UnknownRecipesEnabled", &customKnownRecipeSettings.unknownRecipesEnabled, &defltKnownRecipeSettings.unknownRecipesEnabled);
}

void ErectusIni::GetMeleeSettings()
{
	GetBool("MeleeSettings", "MeleeEnabled", &customMeleeSettings.meleeEnabled, &defltMeleeSettings.meleeEnabled);
	GetSliderInt("MeleeSettings", "MeleeSpeedMin", &customMeleeSettings.meleeSpeedMin, &defltMeleeSettings.meleeSpeedMin, 1, 60);
	GetSliderInt("MeleeSettings", "MeleeSpeedMax", &customMeleeSettings.meleeSpeedMax, &defltMeleeSettings.meleeSpeedMax, 1, 60);

	if (customMeleeSettings.meleeSpeedMax < customMeleeSettings.meleeSpeedMin)
	{
		customMeleeSettings.meleeSpeedMax = customMeleeSettings.meleeSpeedMin;
	}
}

void ErectusIni::SetMeleeSettings()
{
	if (customMeleeSettings.meleeSpeedMax < customMeleeSettings.meleeSpeedMin)
	{
		customMeleeSettings.meleeSpeedMax = customMeleeSettings.meleeSpeedMin;
	}

	SetBool("MeleeSettings", "MeleeEnabled", &customMeleeSettings.meleeEnabled, &defltMeleeSettings.meleeEnabled);
	SetSliderInt("MeleeSettings", "MeleeSpeedMin", &customMeleeSettings.meleeSpeedMin, &defltMeleeSettings.meleeSpeedMin, 1, 60);
	SetSliderInt("MeleeSettings", "MeleeSpeedMax", &customMeleeSettings.meleeSpeedMax, &defltMeleeSettings.meleeSpeedMax, 1, 60);
}

void ErectusIni::GetChargenSettings()
{
	GetBool("ChargenSettings", "ChargenEditingEnabled", &customChargenSettings.chargenEditingEnabled, &defltChargenSettings.chargenEditingEnabled);
	GetSliderFloat("ChargenSettings", "Thin", &customChargenSettings.thin, &defltChargenSettings.thin, 0.0f, 1.0f);
	GetSliderFloat("ChargenSettings", "Muscular", &customChargenSettings.muscular, &defltChargenSettings.muscular, 0.0f, 1.0f);
	GetSliderFloat("ChargenSettings", "Large", &customChargenSettings.large, &defltChargenSettings.large, 0.0f, 1.0f);
}

void ErectusIni::SetChargenSettings()
{
	SetBool("ChargenSettings", "ChargenEditingEnabled", &customChargenSettings.chargenEditingEnabled, &defltChargenSettings.chargenEditingEnabled);
	SetSliderFloat("ChargenSettings", "Thin", &customChargenSettings.thin, &defltChargenSettings.thin, 0.0f, 1.0f);
	SetSliderFloat("ChargenSettings", "Muscular", &customChargenSettings.muscular, &defltChargenSettings.muscular, 0.0f, 1.0f);
	SetSliderFloat("ChargenSettings", "Large", &customChargenSettings.large, &defltChargenSettings.large, 0.0f, 1.0f);
}

void ErectusIni::GetExtraNpcSettings()
{
	GetBool("ExtraNPCSettings", "HideSettlerFaction", &customExtraNpcSettings.hideSettlerFaction, &defltExtraNpcSettings.hideSettlerFaction);
	GetBool("ExtraNPCSettings", "HideCraterRaiderFaction", &customExtraNpcSettings.hideCraterRaiderFaction, &defltExtraNpcSettings.hideCraterRaiderFaction);
	GetBool("ExtraNPCSettings", "HideDieHardFaction", &customExtraNpcSettings.hideDieHardFaction, &defltExtraNpcSettings.hideDieHardFaction);
	GetBool("ExtraNPCSettings", "HideSecretServiceFaction", &customExtraNpcSettings.hideSecretServiceFaction, &defltExtraNpcSettings.hideSecretServiceFaction);
	GetBool("ExtraNPCSettings", "UseNPCBlacklist", &customExtraNpcSettings.useNpcBlacklist, &defltExtraNpcSettings.useNpcBlacklist);
	for (auto i = 0; i < 64; i++)
	{
		char npcBlacklistEnabledText[sizeof("NPCBlacklistEnabled63")];
		char npcBlacklistFormIdText[sizeof("NPCBlacklist63")];
		sprintf_s(npcBlacklistEnabledText, "NPCBlacklistEnabled%d", i);
		sprintf_s(npcBlacklistFormIdText, "NPCBlacklist%d", i);
		GetBool("ExtraNPCSettings", npcBlacklistEnabledText, &customExtraNpcSettings.npcBlacklistEnabled[i], &defltExtraNpcSettings.npcBlacklistEnabled[i]);
		GetDword("ExtraNPCSettings", npcBlacklistFormIdText, &customExtraNpcSettings.npcBlacklist[i], &defltExtraNpcSettings.npcBlacklist[i]);
	}
}

void ErectusIni::SetExtraNpcSettings()
{
	SetBool("ExtraNPCSettings", "HideSettlerFaction", &customExtraNpcSettings.hideSettlerFaction, &defltExtraNpcSettings.hideSettlerFaction);
	SetBool("ExtraNPCSettings", "HideCraterRaiderFaction", &customExtraNpcSettings.hideCraterRaiderFaction, &defltExtraNpcSettings.hideCraterRaiderFaction);
	SetBool("ExtraNPCSettings", "HideDieHardFaction", &customExtraNpcSettings.hideDieHardFaction, &defltExtraNpcSettings.hideDieHardFaction);
	SetBool("ExtraNPCSettings", "HideSecretServiceFaction", &customExtraNpcSettings.hideSecretServiceFaction, &defltExtraNpcSettings.hideSecretServiceFaction);
	SetBool("ExtraNPCSettings", "UseNPCBlacklist", &customExtraNpcSettings.useNpcBlacklist, &defltExtraNpcSettings.useNpcBlacklist);
	for (auto i = 0; i < 64; i++)
	{
		char npcBlacklistEnabledText[sizeof("NPCBlacklistEnabled63")];
		char npcBlacklistFormIdText[sizeof("NPCBlacklist63")];
		sprintf_s(npcBlacklistEnabledText, "NPCBlacklistEnabled%d", i);
		sprintf_s(npcBlacklistFormIdText, "NPCBlacklist%d", i);
		SetBool("ExtraNPCSettings", npcBlacklistEnabledText, &customExtraNpcSettings.npcBlacklistEnabled[i], &defltExtraNpcSettings.npcBlacklistEnabled[i]);
		SetDword("ExtraNPCSettings", npcBlacklistFormIdText, &customExtraNpcSettings.npcBlacklist[i], &defltExtraNpcSettings.npcBlacklist[i]);
	}
}

void ErectusIni::GetBitMsgWriterSettings()
{
	auto allowMessagesdeflt = false;
	GetBool("BitMsgWriter", "AllowMessages", &ErectusMemory::allowMessages, &allowMessagesdeflt);
}

void ErectusIni::SetBitMsgWriterSettings()
{
	auto allowMessagesdeflt = false;
	SetBool("BitMsgWriter", "AllowMessages", &ErectusMemory::allowMessages, &allowMessagesdeflt);
}

void ErectusIni::GetExperimentalSettings()
{
	auto experimentalOverlayFixdeflt = false;
	GetBool("ExperimentalOverlayFix", "ExperimentalOverlayFix", &ErectusMain::experimentalOverlayFix, &experimentalOverlayFixdeflt);
}

void ErectusIni::SetExperimentalSettings()
{
	auto experimentalOverlayFixdeflt = false;
	SetBool("ExperimentalOverlayFix", "ExperimentalOverlayFix", &ErectusMain::experimentalOverlayFix, &experimentalOverlayFixdeflt);
}

void ErectusIni::ReadIniSettings()
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
	GetScrapSettings();
	GetItemLooterSettings();
	GetEntityLooterSettings("NPCLooterSettings", &npcLooterSettings, &defltEntityLooterSettings);
	GetEntityLooterSettings("ContainerLooterSettings", &containerLooterSettings, &defltEntityLooterSettings);
	GetHarvesterSettings();
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
	GetExperimentalSettings();
	file.write(ini, true);
}

void ErectusIni::WriteIniSettings()
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
	SetScrapSettings();
	SetItemLooterSettings();
	SetEntityLooterSettings("NPCLooterSettings", &npcLooterSettings, &defltEntityLooterSettings);
	SetEntityLooterSettings("ContainerLooterSettings", &containerLooterSettings, &defltEntityLooterSettings);
	SetHarvesterSettings();
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
	SetExperimentalSettings();
	file.write(ini, true);
}
