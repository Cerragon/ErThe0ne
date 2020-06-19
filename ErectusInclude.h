#pragma once

//Main
#include <thread>
#include <Windows.h>
#include <TlHelp32.h>


//Icon

//Overlay
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

//D3D9
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

//ImGui
#include "imgui.h"
#include "imgui_internal.h"

//mINI
#define MINI_CASE_SENSITIVE
#include "ini.h"

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
constexpr auto VTABLE_TESNPC = 0x038E0A20UL;//1.3.1.26;
constexpr auto VTABLE_TESOBJECTCONT = 0x03862F50UL;//1.3.1.26;
constexpr auto VTABLE_TESOBJECTMISC = 0x03865380UL;//1.3.1.26;
constexpr auto VTABLE_TESOBJECTBOOK = 0x038621E0UL;//1.3.1.26;
constexpr auto VTABLE_ALCHEMYITEM = 0x037ABC10UL;//1.3.1.26;
constexpr auto VTABLE_TESAMMO = 0x03855BB8UL;//1.3.1.26;
constexpr auto VTABLE_TESOBJECTWEAP = 0x0386D958UL;//1.3.1.26;
constexpr auto VTABLE_TESOBJECTARMO = 0x03861358UL;//1.3.1.26;
constexpr auto VTABLE_TESUTILITYITEM = 0x0386F898UL;//1.3.1.26;
constexpr auto VTABLE_BGSNOTE = 0x038D6120UL;//1.3.1.26;
constexpr auto VTABLE_TESKEY = 0x0385A750UL;//1.3.1.26;
constexpr auto VTABLE_TESFLORA = 0x038578C0UL;//1.3.1.26;
constexpr auto VTABLE_TESLEVITEM = 0x0385BB10UL; // 1.3.1.26;
constexpr auto VTABLE_CURRENCYOBJECT = 0x038545A0UL;//1.3.1.26;
constexpr auto VTABLE_REQUESTACTIVATEREFMSG = 0x03B1BAD0UL;//1.3.1.26;
constexpr auto VTABLE_REQUESTTRANSFERITEMMSG = 0x03B2FD28UL;//1.3.1.26;
constexpr auto VTABLE_REQUESTTELEPORTTOLOCATIONMSG = 0x03B23D60UL;//1.3.1.26;
constexpr auto VTABLE_CLIENTSTATEMSG = 0x03B23940UL;//1.3.1.26;
constexpr auto VTABLE_REQUESTHITSONACTORS = 0x03B1B700UL;//1.3.1.26;
constexpr auto VTABLE_CREATEPROJECTILEMESSAGECLIENT = 0x03AE8A38UL;//1.3.1.26;

//CustomEntry Flags
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
constexpr auto CUSTOM_ENTRY_FAILED_RECIPE = 0x0000000000400000ULL;
constexpr auto CUSTOM_ENTRY_TREASURE_MAP = 0x0000000000800000ULL;
constexpr auto CUSTOM_ENTRY_WHITELISTED = 0x4000000000000000ULL;
constexpr auto CUSTOM_ENTRY_INVALID = 0x8000000000000000ULL;

//Game Classes
class TesObjectRefr
{
public:
	DWORD64 vtable;//0x0
	BYTE padding0008[0x8];
	BYTE harvestFlagA;//0x10
	BYTE padding0011[0x8];
	BYTE harvestFlagB;//0x19
	BYTE padding001A[0x6];
	DWORD formId;//0x20
	BYTE padding0024[0x2];//0x24
	BYTE formType;//0x26
	BYTE padding0027[0x11];//0x27
	BYTE idValue[4];//0x38
	BYTE padding003C[0x14];
	DWORD64 vtable0050;//0x50
	BYTE padding0058[0x8];
	float pitch;//0x60
	BYTE padding0064[0x4];
	float yaw;//0x68
	BYTE padding006C[0x4];
	float position[3];//0x70
	BYTE padding007C[0x4];
	DWORD64 inventoryPtr;//0x80
	BYTE padding0088[0x8];
	DWORD64 actorCorePtr;//0x90
	BYTE padding0098[0x10];
	DWORD64 cellPtr;//0xA8
	DWORD64 skeletonPtr;//0xB0
	DWORD64 referencedItemPtr;//0xB8
	BYTE padding00C0[0xE];
	BYTE spawnFlag;//0xCE
	BYTE padding00Cf[0xC9];
	BYTE movementFlag;//0x198
	BYTE sprintFlag;//0x199
	BYTE healthFlag;//0x19A
	BYTE padding019B[0xA89];
	DWORD formId0C24;//0xC24
};
class TesItem
{
public:
	DWORD64 vtable;//0x0
	BYTE padding0008[0x10];
	BYTE recordFlagA;//0x18
	BYTE padding0019[0x7];
	DWORD formId;//0x20
	BYTE padding0024[0x2];//0x24
	BYTE formType;//0x26
	BYTE padding0027[0x71];//0x27
	DWORD64 namePtr0098;//0x98
	BYTE padding00A0[0x10];
	DWORD64 namePtr00B0;//0xB0
	DWORD64 keywordArrayData00B8;//0xB8
	DWORD64 keywordArrayData00C0;//0xC0
	BYTE padding00B8[0x19];
	BYTE omodFlag;//0xE1
	BYTE padding00E2[0xE];
	DWORD64 factionArrayPtr;//0xF0
	BYTE padding00F8[0x8];
	int factionArraySize;//0x100
	BYTE padding0104[0x5C];
	DWORD64 namePtr0160;//0x160
	BYTE padding0168[0x48];
	DWORD64 keywordArrayData01B0;//0x1B0
	DWORD64 keywordArrayData01B8;//0x1B8
	DWORD64 keywordArrayData01C0;//0x1C0
	BYTE padding01C4[0x10];
	BYTE planFlag;//0x1D8
	BYTE padding01D9[0xF];
	DWORD64 componentArrayPtr;//0x1E8
	BYTE padding01F0[0x8];
	int componentArraySize;//0x1F8
	BYTE padding01Fc[0x14];
	DWORD64 harvestedPtr;//0x210
};

namespace MemoryClasses {
	class LeveledList
	{
	public:
		DWORD64 vtable;//0x0
		BYTE padding0008[0x18];
		DWORD formId;//0x20
		BYTE padding0024[0x8C];
		DWORD64 listEntryArrayPtr;//0xB0
		BYTE padding00B8[0x9];
		BYTE listEntryArraySize;//0xC1
	};
	class RequestActivateRefMessage
	{
	public:
		DWORD64 vtable;//0x0
		DWORD formId;//0x8
		BYTE choice;//0xC
		BYTE forceActivate;//0xB
	};

	class TransferMessage
	{
	public:
		DWORD64 vtable;//0x0
		DWORD srcFormId;//0x8
		DWORD unknownId;//0xC
		DWORD dstFormId;//0x10
		DWORD itemId;//0x14
		int count;//0x18
		DWORD unknownA;//0x1C
		BYTE unknownB;//0x20
		BYTE unknownC;//0x21
		BYTE unknownD;//0x22
		BYTE unknownE;//0x23
	};

	class RequestTeleportMessage
	{
	public:
		DWORD64 vtable;//0x0
		float positionX;//0x8
		float positionY;//0xC
		float positionZ;//0x10
		float rotationX;//0x14
		float rotationY;//0x18
		float rotationZ;//0x1C
		DWORD64 cellPtr;//0x20
	};

	class ClientStateMsg
	{
	public:
		DWORD64 vtable;//0x0
		DWORD64 clientState;//0x8
	};

	class Camera
	{
	public:
		DWORD64 vtable;//0x0
		BYTE padding0008[0x68];
		float forward[3];//0x70
		BYTE padding007C[0x24];
		float origin[3];//0xA0
		BYTE padding00Ac[0x84];
		float view[16];//0x130
	};

	class FalloutMain
	{
	public:
		DWORD64 vtable;//0x0
		BYTE padding0008[0x338];
		DWORD64 platformSessionManagerPtr;//0x340
	};

	class PlatformSessionManager
	{
	public:
		DWORD64 vtable;//0x0
		BYTE padding0008[0x78];
		DWORD64 clientAccountManagerPtr;//0x80
	};

	class ClientAccountManager
	{
	public:
		DWORD64 vtable;//0x0
		BYTE padding0008[0x38];
		DWORD64 clientAccountArrayPtr;//0x40
		BYTE padding0048[0x8];
		int clientAccountArraySizeA;//0x50
		BYTE padding0054[0x4];
		int clientAccountArraySizeB;//0x58
	};

	class ClientAccountBuffer
	{
	public:
		DWORD64 namePtr;//0x0
		DWORD64 clientAccountPtr;//0x8
	};

	class ClientAccount
	{
	public:
		DWORD64 vtable;//0x0
		BYTE padding0008[0x8];
		BYTE nameData[0x10];//0x10
		int nameLength;//0x20
		BYTE padding0024[0x58];
		DWORD formId;//0x7C
	};

	class EntityListType
	{
	public:
		BYTE padding0000[0x80];
		DWORD64 interiorListPtr;//0x80
		DWORD64 interiorListCheck;//0x88
		BYTE padding0090[0x18];
		DWORD64 exteriorListPtr;//0xA8
		DWORD64 exteriorListCheck;//0xB0
	};

	class EntityList
	{
	public:
		BYTE padding0000[0x88];
		DWORD64 listPtr;//0x88
		BYTE padding0088[0x8];
		int listSize;//0x98
	};

	class Component
	{
	public:
		DWORD64 componentReferencePtr;//0x0
		DWORD64 componentCountReferencePtr;//0x8
	};

	class ReferenceList
	{
	public:
		DWORD64 arrayPtr;//0x0
		BYTE padding0008[0x8];
		int arraySize;//0x10
	};

	class Weapon
	{
	public:
		DWORD64 vtable;//0x08
		BYTE padding0008[0x18];
		DWORD formId;//0x20
		BYTE padding0024[0x19C];
		DWORD64 keywordArrayPtr;//0x1C0
		int keywordArraySize;//0x1C8
		BYTE padding01D0[0xE0];
		DWORD64 aimModelPtr;//0x2B0
		BYTE padding02B8[0x44];
		float reloadSpeed;//0x2FC
		BYTE padding0300[0x4];
		float speed;//0x304
		float reach;//0x308
		BYTE padding030C[0x2C];
		float actionPointCost;//0x338
		BYTE padding033C[0x24];
		BYTE flagA;//0x360
		BYTE flagB;//0x361
		BYTE flagC;//0x362
		BYTE flagD;//0x363
		BYTE padding0364[0x1C];
		short capacity;//0x380
	};

	class AimModel
	{
	public:
		DWORD64 vtable;//0x8
		BYTE padding0008[0x18];
		DWORD formId;//0x20
		BYTE padding0024[0x4];
		BYTE spreadData[0x18];//0x28
		BYTE recoilData[0x24];//0x40
		float sway;//0x64
	};

	class BhkCharProxyController
	{
	public:
		DWORD64 vtable;//0x0
		BYTE padding0008[0x4A8];
		DWORD64 hknpBsCharacterProxyPtr;//0x4B0
	};

	class HknpBsCharacterProxy
	{
	public:
		DWORD64 vtable;//0x0
		BYTE padding0008[0x48];
		float rotation[2];//0x50
		BYTE padding0058[0x18];
		float position[3];//0x70
		BYTE padding007C[0x24];
		float velocityA[4];//0xA0
		float velocityB[4];//0xB0
	};

	class Inventory
	{
	public:
		DWORD64 vtable;//0x0
		BYTE padding0008[0x58];
		DWORD64 itemArrayPtr;//0x60
		DWORD64 itemArrayEnd;//0x68
	};

	class Item
	{
	public:
		DWORD64 referencePtr;//0x0
		BYTE padding0008[0x8];
		DWORD64 displayPtr;//0x10
		BYTE padding0018[0x8];
		DWORD64 iterations;//0x20
		BYTE equipFlag;//0x28
		BYTE padding0025[0x3];
		DWORD itemId;//0x2C
		BYTE favoriteIndex;//0x30
		BYTE padding0031[0x7];
	};

	class ItemCount
	{
	public:
		BYTE padding0000[0x8];
		int count;//0x8
		BYTE padding000C[0x4];
	};

	class ItemInstancedArray
	{
	public:
		BYTE padding0000[0x10];
		DWORD64 arrayPtr;//0x10
		DWORD64 arrayEnd;//0x18
	};

	class ExtraTextDisplayData
	{
	public:
		DWORD64 vtable;//0x0
		BYTE padding0008[0x8];
		DWORD64 instancedNamePtr;//0x10
	};

	class ActorValueInformation
	{
	public:
		DWORD64 vtable;//0x8
		BYTE padding0008[0x18];
		DWORD formId;//0x20
		BYTE padding0024[0x1DC];
		float maximumValue;//0x200
	};

	class RequestHitsOnActors
	{
	public:
		DWORD64 vtable;//0x0
		DWORD64 hitsArrayPtr;//0x8
		DWORD64 hitsArrayEnd;//0x10
		BYTE padding0018[0x48];
	};

	class BgsQuestText
	{
	public:
		DWORD64 vtable;//0x0
		DWORD64 formIdPtr;//0x8
		BYTE padding0010[0x28];
		DWORD64 codePtr;//0x38
	};

	class ActorSnapshotComponent
	{
	public:
		//ActorCoreSnapshotComponent
		DWORD64 actorCorevtable;//0x0
		BYTE actorCorePadding0008[0x98];
		//ActorServerAuthSnapshotData
		DWORD64 vtable;//0x0
		BYTE padding0008[0x33];
		BYTE isEssential;//0x3B
		BYTE padding003C[0x34];
		float maxHealth;//0x70
		float modifiedHealth;//0x74
		BYTE padding0078[0x4];
		float lostHealth;//0x7C
		BYTE padding0080[0xA0];
		BYTE epicRank;//0x120
		BYTE padding0121[0x7];
	};

	class ListEntry
	{
	public:
		DWORD64 referencePtr;//0x0
		DWORD64 extraData;//0x8
		DWORD64 conditions;//0x10
		float quantity;//0x18
		DWORD quantityPadding;//0x1C
		DWORD64 quantityGlobal;//0x20
		DWORD64 quantityGlobalPadding;//0x28;
		float chanceNoneValue;//0x30
		DWORD chanceNoneValuePadding;//0x34
		DWORD64 chanceNoneGlobal;//0x38
		DWORD64 chanceNoneCurveTable;//0x40
		float minimumLevel;//0x48
		DWORD minimumLevelPadding;//0x4C
		DWORD64 minimumLevelGlobal;//0x50
		DWORD64 minimumLevelCurveTable;//0x58
		DWORD64 minimumLevelCurveTablePadding;//0x68
	};

	class ProcessLists
	{
	public:
		DWORD64 vtable;//0x0
		BYTE padding0008[0x48];
		DWORD64 npcIdArrayPtrA;//0x50
		BYTE padding0058[0x8];
		int npcIdArraySizeA;//0x60
		BYTE padding0064[0x1C];
		DWORD64 npcIdArrayPtrB;//0x80
		BYTE padding0088[0x8];
		int npcIdArraySizeB;//0x90
	};

	class Chargen
	{
	public:
		BYTE padding[0x28];
		float thin;//0x28
		float muscular;//0x2C
		float large;//0x30
	};

	class ModInstance
	{
	public:
		DWORD64 modListPtr;//0x0
		int modListSize;//0x8
	};

	class ObjectInstanceExtra
	{
	public:
		BYTE padding0000[0x10];
		DWORD64 modDataPtr;//0x10
	};

	class CreateProjectileMessageClient
	{
	public:
		DWORD64 vtable;//0x0
		float positionX;//0x8
		float positionY;//0xC
		float positionZ;//0x10
		BYTE padding0014[0x4];
		DWORD64 rotationArrayPtr;//0x18 (float, XYZ)
		DWORD64 rotationArrayEnd;//0x20
		DWORD64 rotationArrayPad;//0x28
		DWORD itemId;//0x30
		DWORD unknownA;//0x34 (0xFFFFFFFF)
		DWORD unknownB;//0x38 (0xFFFFFFFF)
		DWORD unknownC;//0x3C (0x00000000)
		float unknownD;//0x40 (1.0f)
		DWORD unknownE;//0x44 (0x00000000)
		DWORD64 unknownArrayPtrA;//0x48 (WORD, 0xFFFF)
		DWORD64 unknownArrayEndA;//0x50
		DWORD64 unknownArrayPadA;//0x58
		BYTE unknownF;//0x60 (0xFF)
		BYTE padding0061[0x7];
		DWORD64 unknownArrayPtrB;//0x68 (BYTE, 0x01)
		DWORD64 unknownArrayEndB;//0x70
		DWORD64 unknownArrayPadB;//0x78
		BYTE unknownG;//0x80 (0x00)
		BYTE padding0081[0xF];
	};
	class OldWeapon
	{
	public:
		Weapon* weaponData;
		AimModel* aimModelData;
	};

	class Hits
	{
	public:
		DWORD valueA;//0x0 (Local Player)
		DWORD valueB;//0x4 (Entity)
		DWORD valueC;//0x8 (Projectile == 0)
		DWORD initializationType;//0xC (3 == Default/Gun, 4 == Explosive)
		DWORD uiWeaponServerId;//0x10
		DWORD limbEnum;//0x20 (0xFFFFFFFF == Default/Body)
		DWORD hitEffectId;//0x18 (0 == Default/Gun)
		DWORD uEquipIndex;//0x1C (0 == Default/Gun)
		BYTE uAckIndex;//0x20 (Shots Hit, Always > 0)
		BYTE uFireId;//0x21 (Shots Fired)
		BYTE bPredictedKill;//0x22
		BYTE padding0023;//0x23 (0)
		float explosionLocationX;//0x24
		float explosionLocationY;//0x28
		float explosionLocationZ;//0x2C
		float fProjectilePower;//0x30 (1.0f == Default)
		BYTE bVatsAttack;//0x34
		BYTE bVatsCritical;//0x35
		BYTE bTargetWasDead;//0x36
		BYTE padding0037;//0x37 (0)
	};

	class CustomEntry
	{
	public:
		DWORD64 entityPtr;
		DWORD64 referencePtr;
		DWORD entityFormId;
		DWORD referenceFormId;
		DWORD64 flag;
		char* name;
	};

	class ExecutionList
	{
	public:
		BYTE ASM[0x40]	            //0x0
		{
			0x55,                   //push rbp
			0x56,                   //push rsi
			0x57,                   //push rdi
			0x48, 0x83, 0xEC, 0x28, //sub rsp, 0x28
			0x48, 0x8B, 0xF1,       //mov rsi, rcx
			0x48, 0x8B, 0x2E,       //mov rbp, [rsi]
			0x31, 0xFF,             //xor edi, edi
			0x48, 0x8B, 0x46, 0x10, //mov rax, [rsi + 0x10]
			0x48, 0x8B, 0x0C, 0xF8, //mov rcx, [rax + rdi * 0x8]
			0x48, 0x8B, 0x46, 0x18, //mov rax, [rsi + 0x18]
			0x48, 0x8D, 0x14, 0xF8, //lea rdx, [rax + rdi * 0x8]
			0x4C, 0x8B, 0xC7,       //mov r8, rdi
			0xFF, 0xD5,             //call rbp
			0xFF, 0xC7,             //inc edi
			0x3B, 0x7E, 0x08,       //cmp edi, [rsi + 0x08]
			0x72, 0xE4,             //jb ID_TO_PTR_LOOP
			0x48, 0x83, 0xC4, 0x28, //add rsp, 0x28
			0x5F,                   //pop rdi
			0x5E,                   //pop rsi
			0x5D,                   //pop rbp
			0xC3,                   //ret
			0xCC,                   //Padding
			0xCC, 0xCC, 0xCC, 0xCC, //Padding
			0xCC, 0xCC, 0xCC, 0xCC, //Padding
			0xCC, 0xCC, 0xCC, 0xCC, //Padding
		};
		DWORD64 function{};           //0x40 (0x0)
		DWORD64 idArraySize{};        //0x48 (0x8)
		DWORD64 idArray{};            //0x50 (0x10)
		DWORD64 ptrArray{};           //0x58 (0x18)
	};

	class ExecutionPlan
	{
	public:
		BYTE ASM[0x40]              //0x0
		{
			0x55,                   //push rbp
			0x56,                   //push rsi
			0x57,                   //push rdi
			0x48, 0x83, 0xEC, 0x28, //sub rsp, 0x28
			0x48, 0x8B, 0xF1,       //mov rsi, rcx
			0x48, 0x8B, 0x2E,       //mov rbp, [rsi]
			0x31, 0xFF,             //xor edi, edi
			0x48, 0x8B, 0x4E, 0x08, //mov rcx, [rsi + 0x08]
			0x48, 0x8B, 0x46, 0x18, //mov rax, [rsi + 0x18]
			0x48, 0x8B, 0x14, 0xF8, //mov rdx, [rax + rdi * 0x8]
			0x45, 0x31, 0xC0,       //xor r8d, r8d
			0xFF, 0xD5,             //call rbp
			0x48, 0x8B, 0x4E, 0x20, //mov rcx, [rsi+ 0x20]
			0x88, 0x04, 0x39,       //mov [rcx + rdi], al
			0xFF, 0xC7,             //inc edi
			0x3B, 0x7E, 0x10,       //cmp edi, [rsi + 0x10]
			0x72, 0xE1,             //jb GET_RECIPE_LOOP
			0x48, 0x83, 0xC4, 0x28, //add rsp, 0x28
			0x5F,                   //pop rdi
			0x5E,                   //pop rsi
			0x5D,                   //pop rbp
			0xC3,                   //ret 
			0xCC, 0xCC,             //Padding
			0xCC, 0xCC, 0xCC, 0xCC, //Padding
			0xCC, 0xCC, 0xCC, 0xCC, //Padding
		};
		BYTE rbp[0xF0]
		{
			0x48, 0x89, 0x54, 0x24, 0x10, 0x55, 0x56, 0x57, 0x48, 0x81, 0xEC, 0x40, 0x01, 0x00, 0x00, 0x48,
			0xC7, 0x44, 0x24, 0x20, 0xFE, 0xFF, 0xFF, 0xFF, 0x48, 0x89, 0x9C, 0x24, 0x60, 0x01, 0x00, 0x00,
			0x41, 0x0F, 0xB6, 0xF0, 0x4C, 0x8B, 0xDA, 0x48, 0x85, 0xD2, 0x0F, 0x84, 0xA3, 0x01, 0x00, 0x00,
			0x33, 0xDB, 0x44, 0x8B, 0xD3, 0x48, 0x8D, 0xB9, 0x88, 0x0C, 0x00, 0x00, 0x48, 0x8B, 0x47, 0x28,
			0x4C, 0x8B, 0x4F, 0x20, 0x4C, 0x8B, 0x47, 0x10, 0x49, 0x3B, 0xC1, 0x75, 0x23, 0x49, 0x8B, 0xC9,
			0x4D, 0x85, 0xC0, 0x74, 0x16, 0x49, 0x8D, 0x80, 0x00, 0x01, 0x00, 0x00, 0x48, 0x3B, 0xC8, 0x72,
			0x0A, 0x48, 0x8B, 0x00, 0x48, 0x85, 0xC0, 0x48, 0x0F, 0x45, 0xC8, 0x48, 0x8B, 0xD3, 0xEB, 0x0D,
			0x48, 0x8B, 0x17, 0x48, 0x8B, 0xCB, 0x48, 0x85, 0xD2, 0x48, 0x0F, 0x45, 0xC8, 0x4D, 0x85, 0xC0,
			0x74, 0x16, 0x49, 0x8D, 0x80, 0x00, 0x01, 0x00, 0x00, 0x4C, 0x3B, 0xC8, 0x72, 0x0A, 0x48, 0x8B,
			0x00, 0x48, 0x85, 0xC0, 0x4C, 0x0F, 0x45, 0xC8, 0x49, 0x3B, 0xC9, 0x74, 0x36, 0x0F, 0x1F, 0x00,
			0x4C, 0x39, 0x19, 0x74, 0x24, 0x41, 0xFF, 0xC2, 0x48, 0x83, 0xC1, 0x08, 0x48, 0x8D, 0x82, 0x00,
			0x01, 0x00, 0x00, 0x48, 0x3B, 0xC8, 0x72, 0x0A, 0x48, 0x8B, 0x10, 0x48, 0x85, 0xD2, 0x48, 0x0F,
			0x45, 0xCA, 0x49, 0x3B, 0xC9, 0x75, 0xD9, 0xEB, 0x0A, 0x41, 0x83, 0xFA, 0xFF, 0x74, 0x04, 0xB0,
			0x01, 0xEB, 0x02, 0x32, 0xC0, 0x48, 0x8B, 0x9C, 0x24, 0x60, 0x01, 0x00, 0x00, 0x48, 0x81, 0xC4,
			0x40, 0x01, 0x00, 0x00, 0x5F, 0x5E, 0x5D, 0xC3, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC,
		};
		DWORD64 function{};           //0x130 (0x00)
		DWORD64 localPlayerPtr{};		//0x138 (0x08)
		DWORD64 recipeArraySize{};	//0x140 (0x10)
		DWORD64 recipeArray{};		//0x148 (0x18)
		DWORD64 learnedRecipeArray{};	//0x150 (0x20)
	};

	class ExecutionProjectile
	{
	public:
		BYTE ASM[0x20]              //0x0
		{
			0x48, 0x8B, 0xC1,       //mov rax, rcx
			0x48, 0x8B, 0x48, 0x08, //mov rcx, [rax + 0x08]
			0x48, 0x8B, 0x50, 0x10, //mov rdx, [rax + 0x10]
			0x48, 0x8B, 0x00,       //mov rax, [rax]
			0xFF, 0xE0,             //jmp rax
			0xCC, 0xCC, 0xCC, 0xCC, //Padding
			0xCC, 0xCC, 0xCC, 0xCC, //Padding
			0xCC, 0xCC, 0xCC, 0xCC, //Padding
		};
		DWORD64 address{};            //0x20 (0x0)
		DWORD64 rcx{};                //0x28 (0x8)
		DWORD64 rdx{};                //0x30 (0x10)
		BYTE padding0038[0x8]{};      //0x38
		BYTE message[0x90]{};         //0x40
		float rotationX{};            //0xD0
		float rotationY{};            //0xD4
		float rotationZ{};            //0xD8
		BYTE padding00Cc[0x4]{};      //0xDC
		WORD unknownArrayValueA{};    //0xE0
		BYTE padding00D2[0xE]{};      //0xE2
		BYTE unknownArrayValueB{};    //0xF0
		BYTE padding00E2[0xF]{};      //0xF1
	};

	class ExternalFunction
	{
	public:
		BYTE ASM[0x20]				//0x0
		{
			0x48, 0x8B, 0xC1,		//mov rax, rcx
			0x48, 0x8B, 0x48, 0x08,	//mov rcx, [rax + 0x08]
			0x48, 0x8B, 0x50, 0x10,	//mov rdx, [rax + 0x10]
			0x4C, 0x8B, 0x40, 0x18,	//mov r8, [rax + 0x18]
			0x4C, 0x8B, 0x48, 0x20,	//mov r9, [rax + 0x20]
			0x48, 0x8B, 0x00,		//mov rax, [rax]
			0xFF, 0xE0,				//jmp rax
			0xCC, 0xCC, 0xCC, 0xCC,	//Padding
			0xCC, 0xCC, 0xCC, 0xCC,	//Padding
		};
		DWORD64 address{};			//0x20 (0x0)
		DWORD64 rcx{};				//0x28 (0x8)
		DWORD64 rdx{};				//0x30 (0x10)
		DWORD64 r8{};					//0x38 (0x18)
		DWORD64 r9{};					//0x40 (0x20)
	};

	class ActorValueHook
	{
	public:
		BYTE actorValueAsm[0x118]
		{
			0x8B, 0x42, 0x20,								//mov eax, [rdx+20]
			0x0F, 0x1F, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00,	//nop (multi)
			0x0F, 0x1F, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00,	//nop (multi)
			0x0F, 0x1F, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00,	//nop (multi)
			0x0F, 0x1F, 0x00,								//nop (multi)
			0x3D, 0xD5, 0x02, 0x00, 0x00,					//cmp eax, 000002D5
			0x75, 0x14,										//jne StrengthCheck
			0x31, 0xC0,										//xor eax, eax
			0x3B, 0x05, 0xEB, 0x00, 0x00, 0x00,				//cmp eax, [ActionPointsEnabled]
			0x74, 0x0A,										//je StrengthCheck
			0xF3, 0x0F, 0x10, 0x15, 0xE5, 0x00, 0x00, 0x00,	//movss xmm2, [ActionPoints]
			0xEB, 0x19,										//jmp OriginalFunction
			0x3D, 0xC2, 0x02, 0x00, 0x00,					//cmp eax, 000002C2
			0x75, 0x14,										//jne PerceptionCheck
			0x31, 0xC0,										//xor eax, eax
			0x3B, 0x05, 0xD8, 0x00, 0x00, 0x00,				//cmp eax, [StrengthEnabled]
			0x74, 0x0A,										//je PerceptionCheck
			0xF3, 0x0F, 0x10, 0x15, 0xD2, 0x00, 0x00, 0x00,	//movss xmm2, [Strength]
			0xEB, 0x19,										//jmp OriginalFunction
			0x3D, 0xC3, 0x02, 0x00, 0x00,					//cmp eax, 000002C3
			0x75, 0x14,										//jne EnduranceCheck
			0x31, 0xC0,										//xor eax, eax
			0x3B, 0x05, 0xC5, 0x00, 0x00, 0x00,				//cmp eax, [PerceptionEnabled]
			0x74, 0x0A,										//je EnduranceCheck
			0xF3, 0x0F, 0x10, 0x15, 0xBF, 0x00, 0x00, 0x00,	//movss xmm2, [Perception]
			0xEB, 0x19,										//jmp OriginalFunction
			0x3D, 0xC4, 0x02, 0x00, 0x00,					//cmp eax, 000002C4
			0x75, 0x14,										//jne CharismaCheck
			0x31, 0xC0,										//xor eax, eax
			0x3B, 0x05, 0xB2, 0x00, 0x00, 0x00,				//cmp eax, [EnduranceEnabled]
			0x74, 0x0A,										//je CharismaCheck
			0xF3, 0x0F, 0x10, 0x15, 0xAC, 0x00, 0x00, 0x00,	//movss xmm2, [Endurance]
			0xEB, 0x19,										//jmp OriginalFunction
			0x3D, 0xC5, 0x02, 0x00, 0x00,					//cmp eax, 000002C5
			0x75, 0x14,										//jne IntelligenceCheck
			0x31, 0xC0,										//xor eax, eax
			0x3B, 0x05, 0x9F, 0x00, 0x00, 0x00,				//cmp eax, [CharismaEnabled]
			0x74, 0x0A,										//je IntelligenceCheck
			0xF3, 0x0F, 0x10, 0x15, 0x99, 0x00, 0x00, 0x00,	//movss xmm2, [Charisma]
			0xEB, 0x19,										//jmp OriginalFunction
			0x3D, 0xC6, 0x02, 0x00, 0x00,					//cmp eax, 000002C6
			0x75, 0x14,										//jne AgilityCheck
			0x31, 0xC0,										//xor eax, eax
			0x3B, 0x05, 0x8C, 0x00, 0x00, 0x00,				//cmp eax, [IntelligenceEnabled]
			0x74, 0x0A,										//je AgilityCheck
			0xF3, 0x0F, 0x10, 0x15, 0x86, 0x00, 0x00, 0x00,	//movss xmm2, [Intelligence]
			0xEB, 0x19,										//jmp OriginalFunction
			0x3D, 0xC7, 0x02, 0x00, 0x00,					//cmp eax, 000002C7
			0x75, 0x14,										//jne LuckCheck
			0x31, 0xC0,										//xor eax, eax
			0x3B, 0x05, 0x79, 0x00, 0x00, 0x00,				//cmp eax, [AgilityEnabled]
			0x74, 0x0A,										//je LuckCheck
			0xF3, 0x0F, 0x10, 0x15, 0x73, 0x00, 0x00, 0x00,	//movss xmm2, [Agility]
			0xEB, 0x19,										//jmp OriginalFunction
			0x3D, 0xC8, 0x02, 0x00, 0x00,					//cmp eax, 000002C8
			0x75, 0x14,										//jne OriginalFunction
			0x31, 0xC0,										//xor eax, eax
			0x3B, 0x05, 0x66, 0x00, 0x00, 0x00,				//cmp eax, [LuckEnabled]
			0x74, 0x0A,										//je OriginalFunction
			0xF3, 0x0F, 0x10, 0x15, 0x60, 0x00, 0x00, 0x00,	//movss xmm2, [Luck]
			0xEB, 0x00,										//jmp OriginalFunction
			0x48, 0x8D, 0x05, 0x5B, 0x00, 0x00, 0x00,		//lea rax, [OriginalFunction]
			0x48, 0x8B, 0x00,								//mov rax, [rax]
			0xFF, 0xE0,										//jmp rax
			0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC,	//Padding
			0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC,	//Padding
			0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC,				//Padding
		};
		int actionPointsEnabled{};
		float actionPoints{};
		int strengthEnabled{};
		float strength{};
		int perceptionEnabled{};
		float perception{};
		int enduranceEnabled{};
		float endurance{};
		int charismaEnabled{};
		float charisma{};
		int intelligenceEnabled{};
		float intelligence{};
		int agilityEnabled{};
		float agility{};
		int luckEnabled{};
		float luck{};
		DWORD64 originalFunction{};
	};

	class Opk
	{
	public:
		BYTE opkAsm[0x70]
		{
			0x31, 0xC0,                                     //xor eax, eax
			0x3B, 0x05, 0x68, 0x00, 0x00, 0x00,             //cmp eax, [OpkPlayers]
			0x74, 0x14,                                     //je OpkNpcsCheck
			0x8B, 0x87, 0xC0, 0x03, 0x00, 0x00,             //mov eax,[rdi+000003C0]
			0x83, 0xF8, 0x02,                               //cmp eax, 02
			0x75, 0x09,                                     //jne OpkNpcsCheck
			0x0F, 0x28, 0x05, 0x64, 0x00, 0x00, 0x00,       //movaps xmm0, [OpkPlayerPosition]
			0xEB, 0x2A,                                     //jmp OriginalFunction
			0x31, 0xC0,                                     //xor eax, eax
			0x3B, 0x05, 0x4E, 0x00, 0x00, 0x00,             //cmp eax, [OpkNpcs]
			0x74, 0x14,                                     //je OriginalFunction
			0x8B, 0x87, 0xC0, 0x03, 0x00, 0x00,             //mov eax,[rdi+000003C0]
			0x83, 0xF8, 0x03,                               //cmp eax, 03
			0x75, 0x09,                                     //jne OriginalFunction
			0x0F, 0x28, 0x05, 0x56, 0x00, 0x00, 0x00,       //movaps xmm0, [OpkNpcPosition]
			0xEB, 0x0C,                                     //jmp OriginalFunction
			0x0F, 0x10, 0x87, 0x90, 0x04, 0x00, 0x00,       //movups xmm0,[rdi+00000490]
			0x0F, 0x58, 0x45, 0xA7,                         //addps xmm0,[rbp-59]
			0x90,                                           //nop 
			0x0F, 0x29, 0x45, 0xF7,                         //movaps [rbp-09],xmm0
			0x48, 0x8D, 0x05, 0x25, 0x00, 0x00, 0x00,       //lea rax,[FunctionReturn]
			0x48, 0x8B, 0x00,                               //mov rax,[rax]
			0xFF, 0xE0,                                     //jmp rax
			0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, //Padding
			0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, //Padding
			0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, //Padding
		};
		int opkPlayers{};
		int opkNpcs{};
		DWORD64 originalFunction{};
		float opkPlayerPosition[4]{};
		float opkNpcPosition[4]{};
	};

	class FreezeAp
	{
	public:
		BYTE freezeApAsm[0x70]
		{
			0x8B, 0xD6,                                 //mov edx, esi
			0x48, 0x8B, 0xC8,                           //mov rcx, rax
			0x48, 0x8B, 0x5C, 0x24, 0x30,               //mov rbx, [rsp+30]
			0x48, 0x8B, 0x74, 0x24, 0x38,               //mov rsi, [rsp+38]
			0x48, 0x83, 0xC4, 0x20,                     //add rsp, 20
			0x5F,                                       //pop rdi
			0x49, 0x81, 0xF8, 0x00, 0x00, 0x01, 0x00,   //cmp r8, 00010000
			0x7C, 0x19,                                 //jl OriginalFunction
			0x41, 0x8B, 0x40, 0x20,                     //mov eax, [r8+20]
			0x3D, 0xD5, 0x02, 0x00, 0x00,               //cmp eax, 000002D5
			0x75, 0x0E,                                 //jne OriginalFunction
			0x31, 0xC0,                                 //xor eax, eax
			0x39, 0x05, 0x40, 0x00, 0x00, 0x00,         //cmp [FreezeApEnabled], eax
			0x74, 0x04,                                 //je OriginalFunction
			0x48, 0x63, 0xC2,                           //movsxd rax, edx
			0xC3,                                       //ret 
			0x80, 0x79, 0x0C, 0x00,                     //cmp byte ptr [rcx+0C],00
			0x48, 0x63, 0xC2,                           //movsxd  rax,edx
			0xF3, 0x0F, 0x10, 0x04, 0x81,               //movss xmm0,[rcx+rax*4]
			0x48, 0x8D, 0x14, 0x81,                     //lea rdx,[rcx+rax*4]
			0x74, 0x1F,                                 //je ret
			0xF3, 0x0F, 0x11, 0x44, 0x24, 0x08,         //movss [rsp+08],xmm0
			0x80, 0xCA, 0x20,                           //or dl,20
			0x30, 0x54, 0x24, 0x08,                     //xor [rsp+08],dl
			0x30, 0x54, 0x24, 0x09,                     //xor [rsp+09],dl
			0x30, 0x54, 0x24, 0x0A,                     //xor [rsp+0A],dl
			0x30, 0x54, 0x24, 0x0B,                     //xor [rsp+0B],dl
			0xF3, 0x0F, 0x10, 0x44, 0x24, 0x08,         //movss xmm0,[rsp+08]
			0xC3,                                       //ret 
			0xCC, 0xCC, 0xCC, 0xCC,                     //Padding
			0xCC, 0xCC, 0xCC, 0xCC,                     //Padding
		};
		int freezeApEnabled{};
	};

	class TargetLocking
	{
	public:
		BYTE redirectionAsm[0x40]
		{
			0x48, 0xBB, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x00,     //mov rbx, 0xF000000000000
			0x48, 0x39, 0x1D, 0x2F, 0x00, 0x00, 0x00,                       //cmp [TargetLockingPtr], rbx
			0x7D, 0x12,                                                     //jnl OriginalPtr
			0x48, 0x8B, 0x1D, 0x26, 0x00, 0x00, 0x00,                       //mov rbx, [TargetLockingPtr]
			0x48, 0x81, 0xFB, 0x00, 0x00, 0x01, 0x00,                       //cmp rbx, 0x10000
			0x7E, 0x02,                                                     //jle OriginalPtr
			0xEB, 0x05,                                                     //jmp OriginalFunction
			0x48, 0x8B, 0x5C, 0x24, 0x50,                                   //mov rbx, [rsp+50]
			0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,                             //jmp (Below)
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,                 //OriginalFunction
			0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC,                 //Padding
		};
		DWORD64 targetLockingPtr{};
	};
}

namespace SettingsClasses {
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
		bool scrapKeybindEnabled;
		bool scrapOverrideEnabled;
		bool scrapAutomaticLootingEnabled;
		bool scrapAutomaticStatus;
		int scrapAutomaticSpeedMin;
		int scrapAutomaticSpeedMax;
		int scrapLooterDistance;
		bool scrapEnabledList[40];
		DWORD scrapFormIdList[40];
		const char* scrapNameList[40];
	};

	class ItemLooterSettings
	{
	public:
		bool itemKeybindEnabled;
		bool itemAutomaticLootingEnabled;
		bool itemAutomaticStatus;
		int itemAutomaticSpeedMin;
		int itemAutomaticSpeedMax;
		bool itemLooterWeaponsEnabled;
		int itemLooterWeaponsDistance;
		bool itemLooterArmorEnabled;
		int itemLooterArmorDistance;
		bool itemLooterAmmoEnabled;
		int itemLooterAmmoDistance;
		bool itemLooterModsEnabled;
		int itemLooterModsDistance;
		bool itemLooterMagazinesEnabled;
		int itemLooterMagazinesDistance;
		bool itemLooterBobbleheadsEnabled;
		int itemLooterBobbleheadsDistance;
		bool itemLooterAidEnabled;
		int itemLooterAidDistance;
		bool itemLooterKnownPlansEnabled;
		int itemLooterKnownPlansDistance;
		bool itemLooterUnknownPlansEnabled;
		int itemLooterUnknownPlansDistance;
		bool itemLooterMiscEnabled;
		int itemLooterMiscDistance;
		bool itemLooterUnlistedEnabled;
		int itemLooterUnlistedDistance;
		bool itemLooterListEnabled;
		int itemLooterListDistance;
		bool itemLooterBlacklistToggle;
		bool itemLooterEnabledList[100];
		DWORD itemLooterFormIdList[100];
		bool itemLooterBlacklistEnabled[64];
		DWORD itemLooterBlacklist[64];
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
		bool drawPositionSpoofingEnabled;
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
		TeleportEntry teleportEntryData[16];
	};

	class NukeCodeSettings
	{
	public:
		bool automaticNukeCodes;
		bool drawCodeAlpha;
		bool drawCodeBravo;
		bool drawCodeCharlie;
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
		bool entityLooterEnabled;
		bool entityLooterStatusEnabled;
		bool entityLooterOneStarWeaponsEnabled;
		bool entityLooterOneStarArmorEnabled;
		bool entityLooterTwoStarWeaponsEnabled;
		bool entityLooterTwoStarArmorEnabled;
		bool entityLooterThreeStarWeaponsEnabled;
		bool entityLooterThreeStarArmorEnabled;
		bool entityLooterAllWeaponsEnabled;
		bool entityLooterAllArmorEnabled;
		bool entityLooterAmmoEnabled;
		bool entityLooterModsEnabled;
		bool entityLooterCapsEnabled;
		bool entityLooterJunkEnabled;
		bool entityLooterAidEnabled;
		bool entityLooterTreasureMapsEnabled;
		bool entityLooterKnownPlansEnabled;
		bool entityLooterUnknownPlansEnabled;
		bool entityLooterMiscEnabled;
		bool entityLooterUnlistedEnabled;
		bool entityLooterListEnabled;
		bool entityLooterBlacklistToggle;
		bool entityLooterEnabledList[100];
		DWORD entityLooterFormIdList[100];
		bool entityLooterBlacklistEnabled[64];
		DWORD entityLooterBlacklist[64];
	};

	class HarvesterSettings
	{
	public:
		bool harvesterEnabled;
		bool harvesterStatusEnabled;
		bool harvesterOverrideEnabled;
		bool harvesterEnabledList[69];
		DWORD harvesterFormIdList[69];
		const char* harvesterNameList[69];
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
		bool meleeEnabled;
		int meleeSpeedMin;
		int meleeSpeedMax;
	};

	class ChargenSettings
	{
	public:
		bool chargenEditingEnabled;
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

}


class ErectusMain final {
public:
	static int CreateWnd(HINSTANCE hInstance);
	static void CloseWnd();
	static LRESULT CALLBACK WndCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	static bool SetOverlayPosition(bool topmost, bool layered);
	static void SetOverlayMenu();

	inline static int windowSize[2] = { 0, 0 };
	inline static int windowPosition[2] = { 0, 0 };
	inline static bool overlayMenuActive = false;
	inline static bool overlayActive = false;
	inline static bool overlayForeground = false;
	inline static bool experimentalOverlayFix = false;

	inline static HWND windowHwnd = nullptr;

private:
	static void Render();

	static void RenderProcessMenu();
	static void RenderOverlayMenu();
	static void RenderOverlay();

	inline static HINSTANCE mHInstance = nullptr;
	inline static DWORD* keybindHandlerKey = nullptr;
	inline static DWORD* keybindHandlerBit = nullptr;
	inline static DWORD oldKeybindHandlerKey = 0;
	inline static DWORD oldKeybindHandlerBit = 0;
	inline static int windowTopmostCounter = 0;

	virtual void __dummy() = 0;
};
class Utils final {
public:
	static void ValidateFloat(float* value, float min, float max);
	static void ValidateRgb(float* value);
	static void ValidateInt(int* value, int min, int max);
	static int GetRangedInt(int min, int max);

	static int GetTextLength(const char* text);
	static int GetTextSize(const char* text);

	static bool Valid(DWORD64 ptr);
	static bool Rpm(DWORD64 src, void* dst, size_t size);
	static bool Wpm(DWORD64 dst, void* src, size_t size);
	static DWORD64 AllocEx(size_t size);
	static bool FreeEx(DWORD64 src);
	static bool VtableSwap(DWORD64 dst, DWORD64 src);

	static float GetDistance(float* a1, float* a2);
	static float GetDegrees(float* src, float* forward, float* origin);
	static bool Wts(const float* view, const float* position, float* screen);
	static void ProjectView(float* dst, const float* forward, const float* origin, float distance);

	static bool DoubleKeyPress(int keyCodeA, int keyCodeB);

private:
	static float RadiansToDegrees(float radians);
	static void ValidateDword(DWORD* value, DWORD min, DWORD max);
	static void ValidateRgba(float* value);

	static bool SingleKeyPress(int keyCode);

	virtual void __dummy() = 0;
};
class ErectusProcess final {
public:
	static void SetProcessMenu();
	static void ResetProcessData();
	static bool AttachToProcess(DWORD processId);
	static void SetProcessError(int errorId, const char* error);

	static std::vector<DWORD> GetProcesses();
	
	static bool HwndValid(DWORD processId);

	inline static std::string processError;

	inline static int processValidCounter = 0;
	inline static bool processMenuActive = false;
	inline static bool processSelected = false;

	inline static int processErrorId = 0;

	inline static HANDLE handle = nullptr;
	inline static HWND hWnd = nullptr;
	inline static DWORD64 exe = 0;
	inline static DWORD pid = 0;

private:
	static BOOL HwndEnumFunc(HWND hwnd, LPARAM lParam);
	static DWORD64 GetModuleBaseAddress(DWORD pid, const char* module);


	virtual void __dummy() = 0;
};
class ErectusImGui final {
public:
	static void ProcessMenu();
	static void OverlayMenu();

	static bool ImGuiInitialize();
	static void ImGuiCleanup();

	inline static bool imGuiInitialized = false;

	inline static int alphaCode[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	inline static int bravoCode[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	inline static int charlieCode[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
private:
	static bool DragMenu();
	static void ButtonToggle(const char* label, bool* state);
	static void LargeButtonToggle(const char* label, bool* state);
	static void SmallButtonToggle(const char* label, bool* state);
	inline static bool imGuiContextCreated = false;
	inline static bool imGuiD3D9Initialized = false;
	inline static bool imGuiWin32Initialized = false;

	inline static POINT pointerPosition = { 0, 0 };
	inline static ImVec2 pointerOrigin = { 0.0f, 0.0f };
	inline static bool pointerDrag = false;

	inline static bool swapperSourceToggle = false;
	inline static bool swapperDestinationToggle = false;
	inline static bool transferSourceToggle = false;
	inline static bool transferDestinationToggle = false;

	inline static DWORD64 getPtrResult = 0;
	inline static DWORD64 getAddressResult = 0;

	inline static char** favoritedWeaponsArray = nullptr;

	virtual void __dummy() = 0;
};
class Renderer final {
public:
	static int BeginScene();
	static void EndScene();

	static bool Init();
	static void Cleanup();
	static bool DrawText(const char* text, bool shadowed, bool centered, float* position, const float* color, float alpha);

	inline static LPD3DXSPRITE d3DxSprite = nullptr;

	inline static bool d3D9Initialized = false;
	inline static LPDIRECT3DDEVICE9 d3D9Device = nullptr;
	inline static bool deviceResetQueued = false;

private:
	static bool Reset(HRESULT deviceState);

	inline static D3DPRESENT_PARAMETERS d3D9Parameters = { };

	inline static LPDIRECT3D9 d3D9Interface = nullptr;
	inline static LPD3DXFONT d3DxFont = nullptr;

	inline static bool deviceResetState = false;
	inline static int deviceResetCounter = 0;

	virtual void __dummy() = 0;
};
class ErectusThread final {
public:
	static bool CreateProcessThreads();
	static bool ThreadDestruction();

	inline static bool threadCreationState = false;
	inline static bool threadDestructionQueued = false;
	inline static int threadDestructionCounter = 0;

	inline static bool positionSpoofingToggle = false;

private:
	static DWORD __stdcall BufferEntityListThread(LPVOID lpParameter);
	static DWORD __stdcall BufferNpcListThread(LPVOID lpParameter);
	static DWORD __stdcall BufferPlayerListThread(LPVOID lpParameter);
	static DWORD __stdcall WeaponEditorThread(LPVOID lpParameter);
	static DWORD __stdcall LockingThread(LPVOID lpParameter);
	static DWORD __stdcall MultihackThread(LPVOID lpParameter);
	static DWORD __stdcall HarvesterThread(LPVOID lpParameter);
	static DWORD __stdcall KnownRecipeThread(LPVOID lpParameter);

	inline static bool bufferEntityListThreadActive = false;
	inline static bool bufferNpcListThreadActive = false;
	inline static bool bufferPlayerListThreadActive = false;
	inline static bool weaponEditorThreadActive = false;
	inline static bool lockingThreadActive = false;
	inline static bool multihackThreadActive = false;
	inline static bool harvesterThreadActive = false;
	inline static bool knownRecipeThreadActive = false;

	inline static bool threadDestructionState = false;

	inline static bool noclipToggle = false;

	virtual void __dummy();
};
class ErectusMemory final {
public:
	static DWORD64 GetAddress(DWORD formId);
	static DWORD64 GetPtr(DWORD formId);

	static bool RenderCustomNpcList();
	static bool RenderCustomEntityList();
	static bool RenderCustomPlayerList();
	static void RenderData();

	static char** GetFavoritedWeapons();
	static char* GetFavoritedWeaponText(BYTE index);
	static DWORD GetFavoritedWeaponId(BYTE index);
	static char GetFavoriteSlot(BYTE favoriteIndex);

	static DWORD GetLocalPlayerFormId();
	static DWORD GetStashFormId();

	static bool UpdateBufferEntityList();
	static void DeleteBufferEntityList();
	static bool UpdateBufferNpcList();
	static void DeleteBufferNpcList();
	static bool UpdateBufferPlayerList();
	static void DeleteBufferPlayerList();

	static void DeleteCustomEntityList();
	static void DeleteCustomNpcList();
	static void DeleteCustomPlayerList();
	static void DeleteOldWeaponList();

	static bool UpdateOldWeaponData();
	static bool UpdateKnownRecipes();

	static bool CheckScrapList();
	static bool LootScrap();

	static bool CheckItemLooterSettings();
	static bool LootItems();

	static bool CheckItemTransferList();
	static bool TransferItems(DWORD sourceFormId, DWORD destinationFormId);

	static bool ReferenceSwap(DWORD* sourceFormId, DWORD* destinationFormId);

	static bool GetNukeCode(DWORD formId, int* nukeCode);

	static bool GetTeleportPosition(int index);
	static bool RequestTeleport(int index);

	static bool WeaponEditingEnabled();
	static bool EditWeapon(int index, bool revertWeaponData);

	static bool InfiniteAmmo(bool state);

	static bool SetClientState(DWORD64 clientState);

	static bool DamageRedirection(DWORD64* targetingPage, bool* targetingPageValid, bool isExiting, bool state);

	static bool FreezeActionPoints(DWORD64* freezeApPage, bool* freezeApPageValid, bool state);
	static void Noclip(bool state);
	static bool PositionSpoofing(bool state);
	static bool SendDamage(DWORD weaponId, BYTE* shotsHit, BYTE* shotsFired, BYTE count);

	static bool ActorValue(DWORD64* actorValuePage, bool* actorValuePageValid, bool state);
	static bool SetActorValueMaximum(DWORD formId, float defaultValue, float customValue, bool state);

	static bool SetOpkData(DWORD64 opkPage, bool players, bool state);
	static bool OnePositionKill(DWORD64* opkPage, bool* opkPageValid, bool state);

	static bool MeleeAttack();
	static bool ChargenEditing();

	static bool Harvester();

	static bool MessagePatcher(bool state);

	inline static bool customEntityListUpdated = false;
	inline static bool customNpcListUpdated = false;
	inline static bool customPlayerListUpdated = false;

	inline static bool bufferEntityListUpdated = false;
	inline static bool bufferNpcListUpdated = false;
	inline static bool bufferPlayerListUpdated = false;
	inline static bool oldWeaponListUpdated = false;

	inline static bool customEntityListDestructionQueued = false;
	inline static bool customNpcListDestructionQueued = false;
	inline static bool customPlayerListDestructionQueued = false;
	inline static bool bufferEntityListDestructionQueued = false;
	inline static bool bufferPlayerListDestructionQueued = false;
	inline static bool bufferNpcListDestructionQueued = false;

	inline static int bufferEntityListCounter = 0;
	inline static int bufferNpcListCounter = 0;
	inline static int bufferPlayerListCounter = 0;
	inline static int oldWeaponListCounter = 0;

	inline static int oldWeaponListSize = 0;

	inline static bool targetLockingValid = false;
	inline static float targetLockingClosestDegrees = 0.0f;
	inline static DWORD64 targetLockingClosestPtr = 0;
	inline static int targetLockingCooldown = 0;
	inline static DWORD64 targetLockingPtr = 0;

	inline static bool targetLockingKeyPressed = false;

	inline static bool allowMessages = false;

private:
	static bool RenderCustomEntryA(MemoryClasses::CustomEntry* customEntryData, SettingsClasses::OverlaySettingsA customSettingsA);
	static bool RenderCustomEntryB(MemoryClasses::CustomEntry* customEntryData, SettingsClasses::OverlaySettingsB customSettingsB);
	bool RenderCustomEntityList2();

	static char* GetPlayerName(MemoryClasses::ClientAccount* clientAccountData);
	static bool TargetValid(TesObjectRefr entityData, TesItem referenceData);
	static bool FloraHarvested(BYTE harvestFlagA, BYTE harvestFlagB);

	static bool CheckItemLooterList();
	static bool CheckItemLooterBlacklist();
	static bool CheckEntityLooterList(SettingsClasses::EntityLooterSettings* customEntityLooterSettings);
	static bool CheckEntityLooterBlacklist(SettingsClasses::EntityLooterSettings* customEntityLooterSettings);
	static bool CheckIngredientList();
	static bool CheckJunkPileEnabled();
	static bool CheckComponentArray(TesItem referenceData);
	static bool CheckReferenceKeywordBook(TesItem referenceData, DWORD formId);
	static bool CheckReferenceKeywordMisc(TesItem referenceData, DWORD formId);
	static bool CheckOnlyUseItemLooterList();
	static bool CheckEnabledItem(DWORD formId, DWORD64 entityFlag, int normalDistance);
	static bool CheckFormIdArray(DWORD formId, bool* enabledArray, const DWORD* formIdArray, int size);
	static bool CheckReferenceJunk(TesItem referenceData);
	static bool CheckReferenceMod(TesItem referenceData);
	static bool CheckReferencePlan(TesItem referenceData);

	static int GetOldWeaponIndex(DWORD formId);

	static BYTE IsKnownRecipe(DWORD formId);

	static bool LockedTargetValid(bool* isPlayer);

	static bool MovePlayer();


	static bool CheckOpkDistance(DWORD64 opkPage, bool players);
	static bool InsideInteriorCell();
	static int RenderLocalPlayerData();


	static DWORD GetEntityId(TesObjectRefr entityData);
	static bool SendHitsToServer(MemoryClasses::Hits* hitsData, size_t hitsDataSize);
	static DWORD64 GetNukeCodePtr(DWORD formId);
	static DWORD64 RttiGetNamePtr(DWORD64 vtable);
	static char* GetInstancedItemName(DWORD64 displayPtr);
	static bool EntityInventoryValid(TesObjectRefr entityData);
	static bool AllowLegendaryWeapons(SettingsClasses::EntityLooterSettings* customEntityLooterSettings);
	static bool AllowLegendaryArmor(SettingsClasses::EntityLooterSettings* customEntityLooterSettings);
	static bool CheckEntityLooterItem(DWORD formId, DWORD64 entityFlag, SettingsClasses::EntityLooterSettings* customEntityLooterSettings, bool legendaryWeaponsEnabled, bool legendaryArmorEnabled);
	static bool IsLegendaryFormId(DWORD formId);
	static BYTE GetLegendaryRank(DWORD64 displayPtr);
	static bool ValidLegendary(BYTE legendaryRank, DWORD64 entityFlag, SettingsClasses::EntityLooterSettings* customEntityLooterSettings, bool legendaryWeaponsEnabled, bool legendaryArmorEnabled);
	static bool TransferEntityItems(TesObjectRefr entityData, TesItem referenceData, TesObjectRefr localPlayer, bool onlyUseEntityLooterList, bool useEntityLooterBlacklist);
	static bool ContainerValid(TesItem referenceData);
	static bool LootEntity(TesObjectRefr entityData, TesItem referenceData, TesObjectRefr localPlayer, bool onlyUseEntityLooterList, bool useEntityLooterBlacklist);
	static bool CheckEntityLooterSettings(SettingsClasses::EntityLooterSettings* customEntityLooterSettings);
	static bool CheckOnlyUseEntityLooterList(SettingsClasses::EntityLooterSettings* customEntityLooterSettings);
	static bool HarvestFlora(TesObjectRefr entityData, TesItem referenceData, TesObjectRefr localPlayer);

	static bool CreateProjectile(DWORD itemId, const float* position, const float* rotation);

	static bool SendMessageToServer(void* message, size_t size);
	static bool CreateForwardProjectile(DWORD itemId);

	static DWORD64 GetCameraPtr();
	static BYTE CheckHealthFlag(BYTE healthFlag);
	static DWORD64 GetLocalPlayerPtr(bool checkMainMenu);
	static DWORD64* GetEntityList(int* size);
	static DWORD64* GetNpcList(int* size);
	static DWORD64* GetRecipeArray(int* size);
	static bool CheckWhitelistedFlux(TesItem referenceData);
	static bool FloraLeveledListValid(MemoryClasses::LeveledList leveledListData);
	static bool FloraValid(TesItem referenceData);
	static bool IsTreasureMap(DWORD formId);
	static bool CheckFactionFormId(DWORD formId);
	static bool BlacklistedNpcFaction(TesItem referenceData);
	static bool CheckReferenceItem(TesItem referenceData);
	static void GetCustomEntityData(TesItem referenceData, DWORD64* entityFlag, DWORD64* entityNamePtr, int* enabledDistance, bool checkScrap, bool checkIngredient);
	static bool GetActorSnapshotComponentData(TesObjectRefr entityData, MemoryClasses::ActorSnapshotComponent* actorSnapshotComponentData);
	static char* GetEntityName(DWORD64 ptr);

	inline static MemoryClasses::CustomEntry* customEntityList = nullptr;
	inline static int customEntityListSize = 0;
	inline static int customEntityListCounter = 0;

	inline static MemoryClasses::CustomEntry* bufferEntityList = nullptr;
	inline static int bufferEntityListSize = 0;

	inline static MemoryClasses::CustomEntry* customNpcList = nullptr;
	inline static int customNpcListSize = 0;
	inline static int customNpcListCounter = 0;

	inline static MemoryClasses::CustomEntry* bufferNpcList = nullptr;
	inline static int bufferNpcListSize = 0;

	inline static MemoryClasses::CustomEntry* customPlayerList = nullptr;
	inline static int customPlayerListSize = 0;
	inline static int customPlayerListCounter = 0;

	inline static MemoryClasses::CustomEntry* bufferPlayerList = nullptr;
	inline static int bufferPlayerListSize = 0;

	inline static MemoryClasses::OldWeapon* oldWeaponList = nullptr;

	inline static int knownRecipeArraySize = 0;

	virtual void __dummy() = 0;
};
class ErectusIni final {
public:
	static void ReadIniSettings();
	static void WriteIniSettings();

	static SettingsClasses::OverlaySettingsA playerSettings;
	static SettingsClasses::OverlaySettingsB junkSettings;
	static SettingsClasses::OverlaySettingsA npcSettings;
	static SettingsClasses::OverlaySettingsB containerSettings;
	static SettingsClasses::OverlaySettingsB planSettings;
	static SettingsClasses::OverlaySettingsB magazineSettings;
	static SettingsClasses::OverlaySettingsB bobbleheadSettings;
	static SettingsClasses::OverlaySettingsB itemSettings;
	static SettingsClasses::OverlaySettingsB floraSettings;
	static SettingsClasses::OverlaySettingsB entitySettings;

	static SettingsClasses::ScrapLooterSettings customScrapLooterSettings;
	static SettingsClasses::ItemLooterSettings customItemLooterSettings;
	static SettingsClasses::WeaponSettings customWeaponSettings;
	static SettingsClasses::TargetSettings customTargetSettings;
	static SettingsClasses::LocalPlayerSettings customLocalPlayerSettings;
	static SettingsClasses::OpkSettings customOpkSettings;
	static SettingsClasses::UtilitySettings customUtilitySettings;
	static SettingsClasses::SwapperSettings customSwapperSettings;
	static SettingsClasses::TransferSettings customTransferSettings;
	static SettingsClasses::TeleportSettings customTeleportSettings;
	static SettingsClasses::NukeCodeSettings customNukeCodeSettings;
	static SettingsClasses::LegendarySettings customLegendarySettings;
	static SettingsClasses::FluxSettings customFluxSettings;
	static SettingsClasses::EntityLooterSettings npcLooterSettings;
	static SettingsClasses::EntityLooterSettings containerLooterSettings;
	static SettingsClasses::HarvesterSettings customHarvesterSettings;
	static SettingsClasses::KnownRecipeSettings customKnownRecipeSettings;
	static SettingsClasses::MeleeSettings customMeleeSettings;
	static SettingsClasses::ChargenSettings customChargenSettings;
	static SettingsClasses::ExtraNpcSettings customExtraNpcSettings;
	

	inline static SettingsClasses::WeaponSettings defaultWeaponSettings{
		false, false, false, false, false, false, false, 250, false, 2.0f, false, 500.0f
	};

private:
	static void GetOverlaySettingsB(const char* section, SettingsClasses::OverlaySettingsB* value,
	                         SettingsClasses::OverlaySettingsB* deflt);
	static void SetOverlaySettingsB(const char* section, SettingsClasses::OverlaySettingsB* value,
	                                SettingsClasses::OverlaySettingsB* deflt);
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
	static void GetEntityLooterSettings(const char* section, SettingsClasses::EntityLooterSettings* value, SettingsClasses::EntityLooterSettings* deflt);
	static void SetEntityLooterSettings(const char* section, SettingsClasses::EntityLooterSettings* value, SettingsClasses::EntityLooterSettings* deflt);
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
	static void GetExperimentalSettings();
	static void SetExperimentalSettings();

	static void GetDword(const char* section, const char* key, DWORD* value, const DWORD* deflt);
	static void SetDword(const char* section, const char* key, const DWORD* value, const DWORD* deflt);
	static void GetInt(const char* section, const char* key, int* value, const int* deflt);
	static void SetInt(const char* section, const char* key, const int* value, const int* deflt);
	static void GetSliderInt(const char* section, const char* key, int* value, int* deflt, int min, int max);
	static void SetSliderInt(const char* section, const char* key, int* value, int* deflt, int min, int max);
	static void GetFloat(const char* section, const char* key, float* value, const float* deflt);
	static void SetFloat(const char* section, const char* key, const float* value, const float* deflt);
	static void GetSliderFloat(const char* section, const char* key, float* value, float* deflt, float min, float max);
	static void SetSliderFloat(const char* section, const char* key, float* value, float* deflt, float min, float max);
	static void GetRgb(const char* section, const char* key, float* value, float* deflt);
	static void SetRgb(const char* section, const char* key, float* value, float* deflt);
	static void GetQuadFloat(const char* section, const char* key, float* value, float* deflt);
	static void SetQuadFloat(const char* section, const char* key, float* value, float* deflt);
	static void GetBool(const char* section, const char* key, bool* value, const bool* deflt);
	static void SetBool(const char* section, const char* key, const bool* value, const bool* deflt);
	static void GetOverlaySettingsA(const char* section, SettingsClasses::OverlaySettingsA* value, SettingsClasses::OverlaySettingsA* deflt);
	static void SetOverlaySettingsA(const char* section, SettingsClasses::OverlaySettingsA* value, SettingsClasses::OverlaySettingsA* deflt);

	virtual void __dummy() = 0;
};

