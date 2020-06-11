#pragma once

//Main
#include <thread>
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

//Icon
#include "resource.h"

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
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

//mINI
#define MINI_CASE_SENSITIVE
#include "ini.h"

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
#define CUSTOM_ENTRY_DEFAULT                            0x0000000000000000ULL
#define CUSTOM_ENTRY_UNNAMED                            0x0000000000000001ULL
#define CUSTOM_ENTRY_PLAYER                             0x0000000000000002ULL
#define CUSTOM_ENTRY_NPC                                0x0000000000000004ULL
#define CUSTOM_ENTRY_CONTAINER                          0x0000000000000008ULL
#define CUSTOM_ENTRY_JUNK                               0x0000000000000010ULL
#define CUSTOM_ENTRY_PLAN                               0x0000000000000020ULL
#define CUSTOM_ENTRY_ITEM                               0x0000000000000040ULL
#define CUSTOM_ENTRY_ENTITY                             0x0000000000000080ULL
#define CUSTOM_ENTRY_VALID_SCRAP                        0x0000000000000100ULL
#define CUSTOM_ENTRY_VALID_ITEM                         0x0000000000000200ULL
#define CUSTOM_ENTRY_MAGAZINE                           0x0000000000000400ULL
#define CUSTOM_ENTRY_BOBBLEHEAD                         0x0000000000000800ULL
#define	CUSTOM_ENTRY_FLORA                              0x0000000000001000ULL
#define CUSTOM_ENTRY_MISC                               0x0000000000002000ULL
#define CUSTOM_ENTRY_MOD                                0x0000000000004000ULL
#define CUSTOM_ENTRY_WEAPON                             0x0000000000008000ULL
#define CUSTOM_ENTRY_ARMOR                              0x0000000000010000ULL
#define CUSTOM_ENTRY_AMMO                               0x0000000000020000ULL
#define CUSTOM_ENTRY_AID                                0x0000000000040000ULL
#define CUSTOM_ENTRY_VALID_INGREDIENT                   0x0000000000080000ULL
#define CUSTOM_ENTRY_KNOWN_RECIPE                       0x0000000000100000ULL
#define CUSTOM_ENTRY_UNKNOWN_RECIPE                     0x0000000000200000ULL
#define CUSTOM_ENTRY_FAILED_RECIPE                      0x0000000000400000ULL
#define CUSTOM_ENTRY_TREASURE_MAP                       0x0000000000800000ULL
#define CUSTOM_ENTRY_WHITELISTED                        0x4000000000000000ULL
#define CUSTOM_ENTRY_INVALID                            0x8000000000000000ULL

extern Settings gSettings;

//Game Classes
class Entity
{
public:
	DWORD64 vtable;//0x0
	BYTE Padding0008[0x8];
	BYTE HarvestFlagA;//0x10
	BYTE Padding0011[0x8];
	BYTE HarvestFlagB;//0x19
	BYTE Padding001A[0x6];
	DWORD Formid;//0x20
	BYTE Padding0024[0x14];
	BYTE IdValue[4];//0x38
	BYTE Padding003C[0x14];
	DWORD64 vtable0050;//0x50
	BYTE Padding0058[0x8];
	float Pitch;//0x60
	BYTE Padding0064[0x4];
	float Yaw;//0x68
	BYTE Padding006C[0x4];
	float Position[3];//0x70
	BYTE Padding007C[0x4];
	DWORD64 InventoryPtr;//0x80
	BYTE Padding0088[0x8];
	DWORD64 ActorCorePtr;//0x90
	BYTE Padding0098[0x10];
	DWORD64 CellPtr;//0xA8
	DWORD64 SkeletonPtr;//0xB0
	DWORD64 ReferencePtr;//0xB8
	BYTE Padding00C0[0xE];
	BYTE SpawnFlag;//0xCE
	BYTE Padding00CF[0xC9];
	BYTE MovementFlag;//0x198
	BYTE SprintFlag;//0x199
	BYTE HealthFlag;//0x19A
	BYTE Padding019B[0xA89];
	DWORD Formid0C24;//0xC24
};
class Reference
{
public:
	DWORD64 vtable;//0x0
	BYTE Padding0008[0x10];
	BYTE RecordFlagA;//0x18
	BYTE Padding0019[0x7];
	DWORD Formid;//0x20
	BYTE Padding0024[0x74];
	DWORD64 NamePtr0098;//0x98
	BYTE Padding00A0[0x10];
	DWORD64 NamePtr00B0;//0xB0
	DWORD64 KeywordArrayData00B8;//0xB8
	DWORD64 KeywordArrayData00C0;//0xC0
	BYTE Padding00B8[0x19];
	BYTE OMODFlag;//0xE1
	BYTE Padding00E2[0xE];
	DWORD64 FactionArrayPtr;//0xF0
	BYTE Padding00F8[0x8];
	int FactionArraySize;//0x100
	BYTE Padding0104[0x5C];
	DWORD64 NamePtr0160;//0x160
	BYTE Padding0168[0x48];
	DWORD64 KeywordArrayData01B0;//0x1B0
	DWORD64 KeywordArrayData01B8;//0x1B8
	DWORD64 KeywordArrayData01C0;//0x1C0
	BYTE Padding01C4[0x10];
	BYTE PlanFlag;//0x1D8
	BYTE Padding01D9[0xF];
	DWORD64 ComponentArrayPtr;//0x1E8
	BYTE Padding01F0[0x8];
	int ComponentArraySize;//0x1F8
	BYTE Padding01FC[0x14];
	DWORD64 HarvestedPtr;//0x210
};

namespace memoryClasses {
	class LeveledList
	{
	public:
		DWORD64 vtable;//0x0
		BYTE Padding0008[0x18];
		DWORD Formid;//0x20
		BYTE Padding0024[0x8C];
		DWORD64 ListEntryArrayPtr;//0xB0
		BYTE Padding00B8[0x9];
		BYTE ListEntryArraySize;//0xC1
	};
	class RequestActivateRefMessage
	{
	public:
		DWORD64 vtable;//0x0
		DWORD Formid;//0x8
		BYTE Choice;//0xC
		BYTE ForceActivate;//0xB
	};

	class TransferMessage
	{
	public:
		DWORD64 vtable;//0x0
		DWORD SrcFormid;//0x8
		DWORD UnknownId;//0xC
		DWORD DstFormid;//0x10
		DWORD ItemId;//0x14
		int Count;//0x18
		DWORD UnknownA;//0x1C
		BYTE UnknownB;//0x20
		BYTE UnknownC;//0x21
		BYTE UnknownD;//0x22
		BYTE UnknownE;//0x23
	};

	class RequestTeleportMessage
	{
	public:
		DWORD64 vtable;//0x0
		float PositionX;//0x8
		float PositionY;//0xC
		float PositionZ;//0x10
		float RotationX;//0x14
		float RotationY;//0x18
		float RotationZ;//0x1C
		DWORD64 CellPtr;//0x20
	};

	class ClientStateMsg
	{
	public:
		DWORD64 vtable;//0x0
		DWORD64 ClientState;//0x8
	};

	class Camera
	{
	public:
		DWORD64 vtable;//0x0
		BYTE Padding0008[0x68];
		float Forward[3];//0x70
		BYTE Padding007C[0x24];
		float Origin[3];//0xA0
		BYTE Padding00AC[0x84];
		float View[16];//0x130
	};

	class FalloutMain
	{
	public:
		DWORD64 vtable;//0x0
		BYTE Padding0008[0x338];
		DWORD64 PlatformSessionManagerPtr;//0x340
	};

	class PlatformSessionManager
	{
	public:
		DWORD64 vtable;//0x0
		BYTE Padding0008[0x78];
		DWORD64 ClientAccountManagerPtr;//0x80
	};

	class ClientAccountManager
	{
	public:
		DWORD64 vtable;//0x0
		BYTE Padding0008[0x38];
		DWORD64 ClientAccountArrayPtr;//0x40
		BYTE Padding0048[0x8];
		int ClientAccountArraySizeA;//0x50
		BYTE Padding0054[0x4];
		int ClientAccountArraySizeB;//0x58
	};

	class ClientAccountBuffer
	{
	public:
		DWORD64 NamePtr;//0x0
		DWORD64 ClientAccountPtr;//0x8
	};

	class ClientAccount
	{
	public:
		DWORD64 vtable;//0x0
		BYTE Padding0008[0x8];
		BYTE NameData[0x10];//0x10
		int NameLength;//0x20
		BYTE Padding0024[0x58];
		DWORD Formid;//0x7C
	};

	class EntityListType
	{
	public:
		BYTE Padding0000[0x80];
		DWORD64 InteriorListPtr;//0x80
		DWORD64 InteriorListCheck;//0x88
		BYTE Padding0090[0x18];
		DWORD64 ExteriorListPtr;//0xA8
		DWORD64 ExteriorListCheck;//0xB0
	};

	class EntityList
	{
	public:
		BYTE Padding0000[0x88];
		DWORD64 ListPtr;//0x88
		BYTE Padding0088[0x8];
		int ListSize;//0x98
	};

	class Component
	{
	public:
		DWORD64 ComponentReferencePtr;//0x0
		DWORD64 ComponentCountReferencePtr;//0x8
	};

	class ReferenceList
	{
	public:
		DWORD64 ArrayPtr;//0x0
		BYTE Padding0008[0x8];
		int ArraySize;//0x10
	};

	class Weapon
	{
	public:
		DWORD64 vtable;//0x08
		BYTE Padding0008[0x18];
		DWORD Formid;//0x20
		BYTE Padding0024[0x19C];
		DWORD64 KeywordArrayPtr;//0x1C0
		int KeywordArraySize;//0x1C8
		BYTE Padding01D0[0xE0];
		DWORD64 AimModelPtr;//0x2B0
		BYTE Padding02B8[0x44];
		float ReloadSpeed;//0x2FC
		BYTE Padding0300[0x4];
		float Speed;//0x304
		float Reach;//0x308
		BYTE Padding030C[0x2C];
		float ActionPointCost;//0x338
		BYTE Padding033C[0x24];
		BYTE FlagA;//0x360
		BYTE FlagB;//0x361
		BYTE FlagC;//0x362
		BYTE FlagD;//0x363
		BYTE Padding0364[0x1C];
		short Capacity;//0x380
	};

	class AimModel
	{
	public:
		DWORD64 vtable;//0x8
		BYTE Padding0008[0x18];
		DWORD Formid;//0x20
		BYTE Padding0024[0x4];
		BYTE SpreadData[0x18];//0x28
		BYTE RecoilData[0x24];//0x40
		float Sway;//0x64
	};

	class bhkCharProxyController
	{
	public:
		DWORD64 vtable;//0x0
		BYTE Padding0008[0x4A8];
		DWORD64 hknpBSCharacterProxyPtr;//0x4B0
	};

	class hknpBSCharacterProxy
	{
	public:
		DWORD64 vtable;//0x0
		BYTE Padding0008[0x48];
		float Rotation[2];//0x50
		BYTE Padding0058[0x18];
		float Position[3];//0x70
		BYTE Padding007C[0x24];
		float VelocityA[4];//0xA0
		float VelocityB[4];//0xB0
	};

	class Inventory
	{
	public:
		DWORD64 vtable;//0x0
		BYTE Padding0008[0x58];
		DWORD64 ItemArrayPtr;//0x60
		DWORD64 ItemArrayEnd;//0x68
	};

	class Item
	{
	public:
		DWORD64 ReferencePtr;//0x0
		BYTE Padding0008[0x8];
		DWORD64 DisplayPtr;//0x10
		BYTE Padding0018[0x8];
		DWORD64 Iterations;//0x20
		BYTE EquipFlag;//0x28
		BYTE Padding0025[0x3];
		DWORD ItemId;//0x2C
		BYTE FavoriteIndex;//0x30
		BYTE Padding0031[0x7];
	};

	class ItemCount
	{
	public:
		BYTE Padding0000[0x8];
		int Count;//0x8
		BYTE Padding000C[0x4];
	};

	class ItemInstancedArray
	{
	public:
		BYTE Padding0000[0x10];
		DWORD64 ArrayPtr;//0x10
		DWORD64 ArrayEnd;//0x18
	};

	class ExtraTextDisplayData
	{
	public:
		DWORD64 vtable;//0x0
		BYTE Padding0008[0x8];
		DWORD64 InstancedNamePtr;//0x10
	};

	class ActorValueInformation
	{
	public:
		DWORD64 vtable;//0x8
		BYTE Padding0008[0x18];
		DWORD Formid;//0x20
		BYTE Padding0024[0x1DC];
		float MaximumValue;//0x200
	};

	class RequestHitsOnActors
	{
	public:
		DWORD64 vtable;//0x0
		DWORD64 HitsArrayPtr;//0x8
		DWORD64 HitsArrayEnd;//0x10
		BYTE Padding0018[0x48];
	};

	class BGSQuestText
	{
	public:
		DWORD64 vtable;//0x0
		DWORD64 FormidPtr;//0x8
		BYTE Padding0010[0x28];
		DWORD64 CodePtr;//0x38
	};

	class ActorSnapshotComponent
	{
	public:
		//ActorCoreSnapshotComponent
		DWORD64 ActorCorevtable;//0x0
		BYTE ActorCorePadding0008[0x98];
		//ActorServerAuthSnapshotData
		DWORD64 vtable;//0x0
		BYTE Padding0008[0x33];
		BYTE IsEssential;//0x3B
		BYTE Padding003C[0x34];
		float MaxHealth;//0x70
		float ModifiedHealth;//0x74
		BYTE Padding0078[0x4];
		float LostHealth;//0x7C
		BYTE Padding0080[0xA0];
		BYTE EpicRank;//0x120
		BYTE Padding0121[0x7];
	};

	class ListEntry
	{
	public:
		DWORD64 ReferencePtr;//0x0
		DWORD64 ExtraData;//0x8
		DWORD64 Conditions;//0x10
		float Quantity;//0x18
		DWORD QuantityPadding;//0x1C
		DWORD64 QuantityGlobal;//0x20
		DWORD64 QuantityGlobalPadding;//0x28;
		float ChanceNoneValue;//0x30
		DWORD ChanceNoneValuePadding;//0x34
		DWORD64 ChanceNoneGlobal;//0x38
		DWORD64 ChanceNoneCurveTable;//0x40
		float MinimumLevel;//0x48
		DWORD MinimumLevelPadding;//0x4C
		DWORD64 MinimumLevelGlobal;//0x50
		DWORD64 MinimumLevelCurveTable;//0x58
		DWORD64 MinimumLevelCurveTablePadding;//0x68
	};

	class ProcessLists
	{
	public:
		DWORD64 vtable;//0x0
		BYTE Padding0008[0x48];
		DWORD64 NPCIdArrayPtrA;//0x50
		BYTE Padding0058[0x8];
		int NPCIdArraySizeA;//0x60
		BYTE Padding0064[0x1C];
		DWORD64 NPCIdArrayPtrB;//0x80
		BYTE Padding0088[0x8];
		int NPCIdArraySizeB;//0x90
	};

	class Chargen
	{
	public:
		BYTE Padding[0x28];
		float Thin;//0x28
		float Muscular;//0x2C
		float Large;//0x30
	};

	class ModInstance
	{
	public:
		DWORD64 ModListPtr;//0x0
		int ModListSize;//0x8
	};

	class ObjectInstanceExtra
	{
	public:
		BYTE Padding0000[0x10];
		DWORD64 ModDataPtr;//0x10
	};

	class CreateProjectileMessageClient
	{
	public:
		DWORD64 vtable;//0x0
		float PositionX;//0x8
		float PositionY;//0xC
		float PositionZ;//0x10
		BYTE Padding0014[0x4];
		DWORD64 RotationArrayPtr;//0x18 (float, XYZ)
		DWORD64 RotationArrayEnd;//0x20
		DWORD64 RotationArrayPad;//0x28
		DWORD ItemId;//0x30
		DWORD UnknownA;//0x34 (0xFFFFFFFF)
		DWORD UnknownB;//0x38 (0xFFFFFFFF)
		DWORD UnknownC;//0x3C (0x00000000)
		float UnknownD;//0x40 (1.0f)
		DWORD UnknownE;//0x44 (0x00000000)
		DWORD64 UnknownArrayPtrA;//0x48 (WORD, 0xFFFF)
		DWORD64 UnknownArrayEndA;//0x50
		DWORD64 UnknownArrayPadA;//0x58
		BYTE UnknownF;//0x60 (0xFF)
		BYTE Padding0061[0x7];
		DWORD64 UnknownArrayPtrB;//0x68 (BYTE, 0x01)
		DWORD64 UnknownArrayEndB;//0x70
		DWORD64 UnknownArrayPadB;//0x78
		BYTE UnknownG;//0x80 (0x00)
		BYTE Padding0081[0xF];
	};
	class OldWeapon
	{
	public:
		Weapon* WeaponData;
		AimModel* AimModelData;
	};

	class Hits
	{
	public:
		DWORD ValueA;//0x0 (Local Player)
		DWORD ValueB;//0x4 (Entity)
		DWORD ValueC;//0x8 (Projectile == 0)
		DWORD InitializationType;//0xC (3 == Default/Gun, 4 == Explosive)
		DWORD uiWeaponServerID;//0x10
		DWORD LIMB_ENUM;//0x20 (0xFFFFFFFF == Default/Body)
		DWORD HitEffectID;//0x18 (0 == Default/Gun)
		DWORD uEquipIndex;//0x1C (0 == Default/Gun)
		BYTE uAckIndex;//0x20 (Shots Hit, Always > 0)
		BYTE uFireID;//0x21 (Shots Fired)
		BYTE bPredictedKill;//0x22
		BYTE Padding0023;//0x23 (0)
		float ExplosionLocationX;//0x24
		float ExplosionLocationY;//0x28
		float ExplosionLocationZ;//0x2C
		float fProjectilePower;//0x30 (1.0f == Default)
		BYTE bVatsAttack;//0x34
		BYTE bVatsCritical;//0x35
		BYTE bTargetWasDead;//0x36
		BYTE Padding0037;//0x37 (0)
	};

	class CustomEntry
	{
	public:
		DWORD64 EntityPtr;
		DWORD64 ReferencePtr;
		DWORD EntityFormid;
		DWORD ReferenceFormid;
		DWORD64 Flag;
		char* Name;
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
		DWORD64 Function;           //0x40 (0x0)
		DWORD64 IdArraySize;        //0x48 (0x8)
		DWORD64 IdArray;            //0x50 (0x10)
		DWORD64 PtrArray;           //0x58 (0x18)
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
		BYTE RBP[0xF0]
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
		DWORD64 Function;           //0x130 (0x00)
		DWORD64 LocalPlayerPtr;		//0x138 (0x08)
		DWORD64 RecipeArraySize;	//0x140 (0x10)
		DWORD64 RecipeArray;		//0x148 (0x18)
		DWORD64 LearnedRecipeArray;	//0x150 (0x20)
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
		DWORD64 Address;            //0x20 (0x0)
		DWORD64 RCX;                //0x28 (0x8)
		DWORD64 RDX;                //0x30 (0x10)
		BYTE Padding0038[0x8];      //0x38
		BYTE Message[0x90];         //0x40
		float RotationX;            //0xD0
		float RotationY;            //0xD4
		float RotationZ;            //0xD8
		BYTE Padding00CC[0x4];      //0xDC
		WORD UnknownArrayValueA;    //0xE0
		BYTE Padding00D2[0xE];      //0xE2
		BYTE UnknownArrayValueB;    //0xF0
		BYTE Padding00E2[0xF];      //0xF1
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
		DWORD64 Address;			//0x20 (0x0)
		DWORD64 RCX;				//0x28 (0x8)
		DWORD64 RDX;				//0x30 (0x10)
		DWORD64 R8;					//0x38 (0x18)
		DWORD64 R9;					//0x40 (0x20)
	};

	class ActorValueHook
	{
	public:
		BYTE ActorValueASM[0x118]
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
		int ActionPointsEnabled;
		float ActionPoints;
		int StrengthEnabled;
		float Strength;
		int PerceptionEnabled;
		float Perception;
		int EnduranceEnabled;
		float Endurance;
		int CharismaEnabled;
		float Charisma;
		int IntelligenceEnabled;
		float Intelligence;
		int AgilityEnabled;
		float Agility;
		int LuckEnabled;
		float Luck;
		DWORD64 OriginalFunction;
	};

	class Opk
	{
	public:
		BYTE OpkASM[0x70]
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
		int OpkPlayers;
		int OpkNpcs;
		DWORD64 OriginalFunction;
		float OpkPlayerPosition[4];
		float OpkNpcPosition[4];
	};

	class FreezeAp
	{
	public:
		BYTE FreezeApASM[0x70]
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
		int FreezeApEnabled;
	};

	class TargetLocking
	{
	public:
		BYTE RedirectionASM[0x40]
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
		DWORD64 TargetLockingPtr;
	};
}

namespace settingsClasses {
	class OverlaySettingsA
	{
	public:
		bool Enabled;
		int EnabledDistance;
		bool DrawAlive;
		float AliveColor[3];
		bool DrawDowned;
		float DownedColor[3];
		bool DrawDead;
		float DeadColor[3];
		bool DrawUnknown;
		float UnknownColor[3];
		bool DrawEnabled;
		float EnabledAlpha;
		bool DrawDisabled;
		float DisabledAlpha;
		bool DrawNamed;
		bool DrawUnnamed;
		bool ShowName;
		bool ShowDistance;
		bool ShowHealth;
		bool ShowDeadHealth;
		bool TextCentered;
		bool TextShadowed;
	};

	class OverlaySettingsB
	{
	public:
		bool Enabled;
		int EnabledDistance;
		float Color[3];
		bool DrawEnabled;
		float EnabledAlpha;
		bool DrawDisabled;
		float DisabledAlpha;
		bool DrawNamed;
		bool DrawUnnamed;
		bool ShowName;
		bool ShowDistance;
		bool TextCentered;
		bool TextShadowed;
		bool Whitelisted[32];
		DWORD Whitelist[32];
	};

	class ScrapLooterSettings
	{
	public:
		bool ScrapKeybindEnabled;
		bool ScrapOverrideEnabled;
		bool ScrapAutomaticLootingEnabled;
		bool ScrapAutomaticStatus;
		int ScrapAutomaticSpeedMin;
		int ScrapAutomaticSpeedMax;
		int ScrapLooterDistance;
		bool ScrapEnabledList[40];
		DWORD ScrapFormidList[40];
		const char* ScrapNameList[40];
	};

	class ItemLooterSettings
	{
	public:
		bool ItemKeybindEnabled;
		bool ItemAutomaticLootingEnabled;
		bool ItemAutomaticStatus;
		int ItemAutomaticSpeedMin;
		int ItemAutomaticSpeedMax;
		bool ItemLooterWeaponsEnabled;
		int ItemLooterWeaponsDistance;
		bool ItemLooterArmorEnabled;
		int ItemLooterArmorDistance;
		bool ItemLooterAmmoEnabled;
		int ItemLooterAmmoDistance;
		bool ItemLooterModsEnabled;
		int ItemLooterModsDistance;
		bool ItemLooterMagazinesEnabled;
		int ItemLooterMagazinesDistance;
		bool ItemLooterBobbleheadsEnabled;
		int ItemLooterBobbleheadsDistance;
		bool ItemLooterAidEnabled;
		int ItemLooterAidDistance;
		bool ItemLooterKnownPlansEnabled;
		int ItemLooterKnownPlansDistance;
		bool ItemLooterUnknownPlansEnabled;
		int ItemLooterUnknownPlansDistance;
		bool ItemLooterMiscEnabled;
		int ItemLooterMiscDistance;
		bool ItemLooterUnlistedEnabled;
		int ItemLooterUnlistedDistance;
		bool ItemLooterListEnabled;
		int ItemLooterListDistance;
		bool ItemLooterBlacklistToggle;
		bool ItemLooterEnabledList[100];
		DWORD ItemLooterFormidList[100];
		bool ItemLooterBlacklistEnabled[64];
		DWORD ItemLooterBlacklist[64];
	};

	class WeaponSettings
	{
	public:
		bool NoRecoil;
		bool NoSpread;
		bool NoSway;
		bool InfiniteAmmo;
		bool InstantReload;
		bool Automaticflag;
		bool CapacityEnabled;
		int Capacity;
		bool SpeedEnabled;
		float Speed;
		bool ReachEnabled;
		float Reach;
	};

	class TargetSettings
	{
	public:
		bool LockPlayers;
		bool LockNPCs;
		bool IndirectPlayers;
		bool IndirectNPCs;
		bool DirectPlayers;
		bool DirectNPCs;
		bool TargetLiving;
		bool TargetDowned;
		bool TargetDead;
		bool TargetUnknown;
		bool IgnoreRenderDistance;
		float LockingFOV;
		bool IgnoreEssentialNPCs;
		float LockingColor[3];
		bool Retargeting;
		int Cooldown;
		int SendDamageMin;
		int SendDamageMax;
		int FavoriteIndex;
	};

	class LocalPlayerSettings
	{
	public:
		bool PositionSpoofingEnabled;
		bool DrawPositionSpoofingEnabled;
		int PositionSpoofingHeight;
		bool NoclipEnabled;
		float NoclipSpeed;
		bool ClientState;
		bool AutomaticClientState;
		bool FreezeApEnabled;
		bool ActionPointsEnabled;
		int ActionPoints;
		bool StrengthEnabled;
		int Strength;
		bool PerceptionEnabled;
		int Perception;
		bool EnduranceEnabled;
		int Endurance;
		bool CharismaEnabled;
		int Charisma;
		bool IntelligenceEnabled;
		int Intelligence;
		bool AgilityEnabled;
		int Agility;
		bool LuckEnabled;
		int Luck;
	};

	class OpkSettings
	{
	public:
		bool PlayersEnabled;
		bool NpcsEnabled;
	};

	class UtilitySettings
	{
	public:
		bool DebugPlayer;
		bool DebugEsp;
		DWORD PtrFormid;
		DWORD AddressFormid;
	};

	class SwapperSettings
	{
	public:
		DWORD SourceFormid;
		DWORD DestinationFormid;
	};

	class TransferSettings
	{
	public:
		DWORD SourceFormid;
		DWORD DestinationFormid;
		bool UseWhitelist;
		bool UseBlacklist;
		bool Whitelisted[32];
		DWORD Whitelist[32];
		bool Blacklisted[32];
		DWORD Blacklist[32];
	};

	class TeleportEntry
	{
	public:
		float Destination[4];
		DWORD CellFormid;
		bool DisableSaving;
	};

	class TeleportSettings
	{
	public:
		TeleportEntry TeleportEntryData[16];
	};

	class NukeCodeSettings
	{
	public:
		bool AutomaticNukeCodes;
		bool DrawCodeAlpha;
		bool DrawCodeBravo;
		bool DrawCodeCharlie;
	};

	class LegendarySettings
	{
	public:
		bool OverrideLivingOneStar;
		float LivingOneStarColor[3];
		bool OverrideDeadOneStar;
		float DeadOneStarColor[3];
		bool OverrideLivingTwoStar;
		float LivingTwoStarColor[3];
		bool OverrideDeadTwoStar;
		float DeadTwoStarColor[3];
		bool OverrideLivingThreeStar;
		float LivingThreeStarColor[3];
		bool OverrideDeadThreeStar;
		float DeadThreeStarColor[3];
	};

	class FluxSettings
	{
	public:
		bool CrimsonFluxEnabled;
		bool CobaltFluxEnabled;
		bool YellowcakeFluxEnabled;
		bool FluorescentFluxEnabled;
		bool VioletFluxEnabled;
	};

	class EntityLooterSettings
	{
	public:
		bool EntityLooterEnabled;
		bool EntityLooterStatusEnabled;
		bool EntityLooterOneStarWeaponsEnabled;
		bool EntityLooterOneStarArmorEnabled;
		bool EntityLooterTwoStarWeaponsEnabled;
		bool EntityLooterTwoStarArmorEnabled;
		bool EntityLooterThreeStarWeaponsEnabled;
		bool EntityLooterThreeStarArmorEnabled;
		bool EntityLooterAllWeaponsEnabled;
		bool EntityLooterAllArmorEnabled;
		bool EntityLooterAmmoEnabled;
		bool EntityLooterModsEnabled;
		bool EntityLooterCapsEnabled;
		bool EntityLooterJunkEnabled;
		bool EntityLooterAidEnabled;
		bool EntityLooterTreasureMapsEnabled;
		bool EntityLooterKnownPlansEnabled;
		bool EntityLooterUnknownPlansEnabled;
		bool EntityLooterMiscEnabled;
		bool EntityLooterUnlistedEnabled;
		bool EntityLooterListEnabled;
		bool EntityLooterBlacklistToggle;
		bool EntityLooterEnabledList[100];
		DWORD EntityLooterFormidList[100];
		bool EntityLooterBlacklistEnabled[64];
		DWORD EntityLooterBlacklist[64];
	};

	class HarvesterSettings
	{
	public:
		bool HarvesterEnabled;
		bool HarvesterStatusEnabled;
		bool HarvesterOverrideEnabled;
		bool HarvesterEnabledList[69];
		DWORD HarvesterFormidList[69];
		const char* HarvesterNameList[69];
	};

	class KnownRecipeSettings
	{
	public:
		bool KnownRecipesEnabled;
		bool UnknownRecipesEnabled;
	};

	class MeleeSettings
	{
	public:
		bool MeleeEnabled;
		int MeleeSpeedMin;
		int MeleeSpeedMax;
	};

	class ChargenSettings
	{
	public:
		bool ChargenEditingEnabled;
		float Thin;
		float Muscular;
		float Large;
	};

	class ExtraNPCSettings
	{
	public:
		bool HideSettlerFaction;
		bool HideCraterRaiderFaction;
		bool HideDieHardFaction;
		bool HideSecretServiceFaction;
		bool UseNPCBlacklist;
		bool NPCBlacklistEnabled[64];
		DWORD NPCBlacklist[64];
	};

}


class ErectusMain final {
public:
	static void Close();

	static bool SetOverlayPosition(bool Topmost, bool Layered);
	static void SetOverlayMenu();

	static bool KeybindHandler(WPARAM wParam, LPARAM lParam);
	static void CancelKeybindInput();

	inline static int WindowSize[2] = { 0, 0 };
	inline static int WindowPosition[2] = { 0, 0 };
	inline static bool OverlayMenuActive = false;
	inline static bool OverlayActive = false;
	inline static bool OverlayForeground = false;
	inline static bool OverlayMenuPress = false;
	inline static bool ExperimentalOverlayFix = false;

	inline static HWND WindowHwnd = NULL;

private:
	static void KeybindInput(DWORD* KeybindKey, DWORD* KeybindBit);
	static void ClearKeybind(DWORD* KeybindKey, DWORD* KeybindBit);

	inline static DWORD* KeybindHandlerKey = nullptr;
	inline static DWORD* KeybindHandlerBit = nullptr;
	inline static DWORD OldKeybindHandlerKey = 0;
	inline static DWORD OldKeybindHandlerBit = 0;
	inline static int WindowTopmostCounter = 0;

	virtual void __dummy() = 0;
};
class Erectus final {
public:
	static void ValidateFloat(float* Value, float Min, float Max);
	static void ValidateRGB(float* Value);
	static void ValidateInt(int* Value, int Min, int Max);
	static int GetRangedInt(int Min, int Max);

	static int GetTextLength(const char* Text);
	static int GetTextSize(const char* Text);

	static bool Valid(DWORD64 ptr);
	static bool RPM(DWORD64 src, void* dst, size_t Size);
	static bool WPM(DWORD64 dst, void* src, size_t Size);
	static DWORD64 AllocEx(size_t Size);
	static bool FreeEx(DWORD64 src);
	static bool vtableSwap(DWORD64 dst, DWORD64 src);

	static float GetDistance(float* a1, float* a2);
	static float GetDegrees(float* src, float* Forward, float* Origin);
	static bool WTS(float* View, float* Position, float* Out);
	static void ProjectView(float* dst, float* Forward, float* Origin, float Distance);

	static bool DoubleKeyPress(int KeyCodeA, int KeyCodeB, bool* KeyPress);

private:
	static void ValidateDWORD(DWORD* Value, DWORD Min, DWORD Max);
	static void ValidateRGBA(float* Value);

	static bool SingleKeyPress(int KeyCode, bool* KeyPress);

	virtual void __dummy() = 0;
};
class ErectusProcess final {
public:
	static void SetProcessMenu();
	static void ResetProcessData(bool ClearProcessError, int NewProcessListSize);
	static bool ProcessValid(DWORD ProcessId);
	static bool UpdateProcessList();
	static void SetProcessError(int ErrorId, const char* Error, size_t Length);

	static bool HwndValid(DWORD ProcessId);

	inline static char* ProcessError = nullptr;


	inline static int ProcessValidCounter = 0;
	inline static bool ProcessMenuActive = false;
	inline static bool ProcessSelected = false;
	inline static bool ProcessListUpdated = false;

	inline static char** ProcessList = nullptr;
	inline static int ProcessListSize = 0;
	inline static int ProcessIndex = 0;
	inline static DWORD* ProcessIdList = nullptr;
	inline static int ProcessErrorId = 0;


	inline static HANDLE Handle = NULL;
	inline static HWND Hwnd = NULL;
	inline static DWORD64 Exe = 0;
	inline static DWORD Pid = 0;

private:
	static int GetProcessCount();
	static BOOL HwndEnumFunc(HWND hwnd, LPARAM lParam);
	static DWORD64 GetModuleBaseAddress(DWORD Pid, const char* Module);


	virtual void __dummy() = 0;
};
class ErectusImGui final {
public:
	static void ProcessMenu();
	static void OverlayMenu();

	static bool ImGuiInitialize();
	static void ImGuiCleanup();

	inline static bool ImGuiInitialized = false;

	inline static int AlphaCode[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	inline static int BravoCode[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	inline static int CharlieCode[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
private:
	static bool DragMenu();

	inline static bool ImGuiContextCreated = false;
	inline static bool ImGuiD3D9Initialized = false;
	inline static bool ImGuiWin32Initialized = false;

	inline static POINT PointerPosition = { 0, 0 };
	inline static ImVec2 PointerOrigin = { 0.0f, 0.0f };
	inline static bool PointerDrag = false;

	inline static bool SwapperSourceToggle = false;
	inline static bool SwapperDestinationToggle = false;
	inline static bool TransferSourceToggle = false;
	inline static bool TransferDestinationToggle = false;

	inline static DWORD64 GetPtrResult = 0;
	inline static DWORD64 GetAddressResult = 0;

	inline static char** FavoritedWeaponsArray = nullptr;

	virtual void __dummy() = 0;
};
class ErectusD3D9 final {
public:
	static void D3D9Render();
	static bool D3D9Initialize();
	static void D3D9Cleanup();
	static bool D3D9DrawText(const char* Text, bool Shadowed, bool Centered, float* Position, float* Color, float Alpha);

	inline static bool D3D9Initialized = false;
	inline static LPDIRECT3DDEVICE9 D3D9Device = NULL;
	inline static bool DeviceResetQueued = false;

private:
	static void D3D9Reset(HRESULT DeviceState);

	inline static D3DPRESENT_PARAMETERS D3D9Parameters = { };

	inline static LPDIRECT3D9 D3D9Interface = NULL;
	inline static LPD3DXSPRITE D3DXSprite = NULL;
	inline static LPD3DXFONT D3DXFont = NULL;

	inline static bool D3D9InterfaceCreated = false;
	inline static bool D3D9DeviceCreated = false;
	inline static bool D3DXSpriteCreated = false;
	inline static bool D3DXFontCreated = false;

	inline static bool DeviceResetState = false;
	inline static int DeviceResetCounter = 0;

	virtual void __dummy() = 0;
};
class ErectusThread final {
public:
	static bool CreateProcessThreads();
	static bool ThreadDestruction();

	inline static bool ThreadCreationState = false;
	inline static bool ThreadDestructionQueued = false;
	inline static int ThreadDestructionCounter = 0;

	inline static bool PositionSpoofingToggle = false;

private:
	static DWORD __stdcall BufferEntityListThread(LPVOID lpParameter);
	static DWORD __stdcall BufferNPCListThread(LPVOID lpParameter);
	static DWORD __stdcall BufferPlayerListThread(LPVOID lpParameter);
	static DWORD __stdcall WeaponEditorThread(LPVOID lpParameter);
	static DWORD __stdcall LockingThread(LPVOID lpParameter);
	static DWORD __stdcall MultihackThread(LPVOID lpParameter);
	static DWORD __stdcall HarvesterThread(LPVOID lpParameter);
	static DWORD __stdcall KnownRecipeThread(LPVOID lpParameter);

	inline static bool BufferEntityListThreadActive = false;
	inline static bool BufferNPCListThreadActive = false;
	inline static bool BufferPlayerListThreadActive = false;
	inline static bool WeaponEditorThreadActive = false;
	inline static bool LockingThreadActive = false;
	inline static bool MultihackThreadActive = false;
	inline static bool HarvesterThreadActive = false;
	inline static bool KnownRecipeThreadActive = false;

	inline static bool ThreadDestructionState = false;

	inline static bool NoclipToggle = false;

	virtual void __dummy() = 0;
};
class ErectusMemory final {
public:
	static DWORD64 GetAddress(DWORD Formid);
	static DWORD64 GetPtr(DWORD Formid);

	static bool RenderCustomNPCList();
	static bool RenderCustomEntityList();
	static bool RenderCustomPlayerList();
	static void RenderData();

	static char** GetFavoritedWeapons();
	static char* GetFavoritedWeaponText(BYTE Index);
	static DWORD GetFavoritedWeaponId(BYTE Index);
	static char GetFavoriteSlot(BYTE FavoriteIndex);

	static DWORD GetLocalPlayerFormid();
	static DWORD GetStashFormid();

	static bool UpdateBufferEntityList();
	static void DeleteBufferEntityList();
	static bool UpdateBufferNPCList();
	static void DeleteBufferNPCList();
	static bool UpdateBufferPlayerList();
	static void DeleteBufferPlayerList();

	static void DeleteCustomEntityList();
	static void DeleteCustomNPCList();
	static void DeleteCustomPlayerList();
	static void DeleteOldWeaponList();

	static bool UpdateOldWeaponData();
	static bool UpdateKnownRecipes();

	static bool CheckScrapList();
	static bool LootScrap();

	static bool CheckItemLooterSettings();
	static bool LootItems();

	static bool CheckItemTransferList();
	static bool TransferItems(DWORD SourceFormid, DWORD DestinationFormid);

	static bool ReferenceSwap(DWORD* SourceFormid, DWORD* DestinationFormid);

	static bool GetNukeCode(DWORD Formid, int* NukeCode);

	static bool GetTeleportPosition(int Index);
	static bool RequestTeleport(int Index);

	static bool WeaponEditingEnabled();
	static bool EditWeapon(int Index, bool RevertWeaponData);

	static bool InfiniteAmmo(bool State);

	static bool SetClientState(DWORD64 ClientState);

	static bool DamageRedirection(DWORD64* TargetingPage, bool* TargetingPageValid, bool IsExiting, bool State);

	static bool FreezeActionPoints(DWORD64* FreezeApPage, bool* FreezeApPageValid, bool State);
	static void Noclip(bool State);
	static bool PositionSpoofing(bool State);
	static bool SendDamage(DWORD WeaponId, BYTE* ShotsHit, BYTE* ShotsFired, BYTE Count);

	static bool ActorValue(DWORD64* ActorValuePage, bool* ActorValuePageValid, bool State);
	static bool SetActorValueMaximum(DWORD Formid, float DefaultValue, float CustomValue, bool State);

	static bool SetOpkData(DWORD64 OpkPage, bool Players, bool State);
	static bool OnePositionKill(DWORD64* OpkPage, bool* OpkValid, bool State);

	static bool MeleeAttack();
	static bool ChargenEditing();

	static bool Harvester();

	static bool MessagePatcher(bool State);

	inline static bool CustomEntityListUpdated = false;
	inline static bool CustomNPCListUpdated = false;
	inline static bool CustomPlayerListUpdated = false;

	inline static bool BufferEntityListUpdated = false;
	inline static bool BufferNPCListUpdated = false;
	inline static bool BufferPlayerListUpdated = false;
	inline static bool OldWeaponListUpdated = false;

	inline static bool CustomEntityListDestructionQueued = false;
	inline static bool CustomNPCListDestructionQueued = false;
	inline static bool CustomPlayerListDestructionQueued = false;
	inline static bool BufferEntityListDestructionQueued = false;
	inline static bool BufferPlayerListDestructionQueued = false;
	inline static bool BufferNPCListDestructionQueued = false;

	inline static int BufferEntityListCounter = 0;
	inline static int BufferNPCListCounter = 0;
	inline static int BufferPlayerListCounter = 0;
	inline static int OldWeaponListCounter = 0;

	inline static int OldWeaponListSize = 0;

	inline static bool TargetLockingValid = false;
	inline static float TargetLockingClosestDegrees = 0.0f;
	inline static DWORD64 TargetLockingClosestPtr = 0;
	inline static int TargetLockingCooldown = 0;
	inline static DWORD64 TargetLockingPtr = 0;

	inline static bool TargetLockingKeyPressed = false;

	inline static bool AllowMessages = false;

private:
	static bool RenderCustomEntryA(memoryClasses::CustomEntry* CustomEntryData, settingsClasses::OverlaySettingsA CustomSettingsA);
	static bool RenderCustomEntryB(memoryClasses::CustomEntry* CustomEntryData, settingsClasses::OverlaySettingsB CustomSettingsB);

	static char* GetPlayerName(memoryClasses::ClientAccount* ClientAccountData);
	static bool TargetValid(Entity EntityData, Reference ReferenceData);
	static bool FloraHarvested(BYTE HarvestFlagA, BYTE HarvestFlagB);

	static bool CheckItemLooterList();
	static bool CheckItemLooterBlacklist();
	static bool CheckEntityLooterList(settingsClasses::EntityLooterSettings* CustomEntityLooterSettings);
	static bool CheckEntityLooterBlacklist(settingsClasses::EntityLooterSettings* CustomEntityLooterSettings);
	static bool CheckIngredientList();
	static bool CheckJunkPileEnabled();
	static bool CheckComponentArray(Reference ReferenceData);
	static bool CheckReferenceKeywordBook(Reference ReferenceData, DWORD Formid);
	static bool CheckReferenceKeywordMisc(Reference ReferenceData, DWORD Formid);
	static bool CheckOnlyUseItemLooterList();
	static bool CheckEnabledItem(DWORD Formid, DWORD64 EntityFlag, int NormalDistance);
	static bool CheckFormidArray(DWORD Formid, bool* EnabledArray, DWORD* FormidArray, int Size);
	static bool CheckReferenceJunk(Reference ReferenceData);
	static bool CheckReferenceMod(Reference ReferenceData);
	static bool CheckReferencePlan(Reference ReferenceData);

	static int GetOldWeaponIndex(DWORD Formid);

	static BYTE IsKnownRecipe(DWORD Formid);

	static bool LockedTargetValid(bool* IsPlayer);

	static bool MovePlayer();


	static bool CheckOpkDistance(DWORD64 OpkPage, bool Players);
	static bool InsideInteriorCell();
	static int RenderLocalPlayerData();


	static DWORD GetEntityId(Entity EntityData);
	static bool SendHitsToServer(memoryClasses::Hits* HitsData, size_t HitsDataSize);
	static DWORD64 GetNukeCodePtr(DWORD Formid);
	static DWORD64 rttiGetNamePtr(DWORD64 vtable);
	static char* GetInstancedItemName(DWORD64 DisplayPtr);
	static bool EntityInventoryValid(Entity EntityData);
	static bool AllowLegendaryWeapons(settingsClasses::EntityLooterSettings* CustomEntityLooterSettings);
	static bool AllowLegendaryArmor(settingsClasses::EntityLooterSettings* CustomEntityLooterSettings);
	static bool CheckEntityLooterItem(DWORD Formid, DWORD64 EntityFlag, settingsClasses::EntityLooterSettings* CustomEntityLooterSettings, bool LegendaryWeaponsEnabled, bool LegendaryArmorEnabled);
	static bool IsLegendaryFormid(DWORD Formid);
	static BYTE GetLegendaryRank(DWORD64 DisplayPtr);
	static bool ValidLegendary(BYTE LegendaryRank, DWORD64 EntityFlag, settingsClasses::EntityLooterSettings* CustomEntityLooterSettings, bool LegendaryWeaponsEnabled, bool LegendaryArmorEnabled);
	static bool TransferEntityItems(Entity EntityData, Reference ReferenceData, Entity LocalPlayer, bool OnlyUseEntityLooterList, bool UseEntityLooterBlacklist);
	static bool ContainerValid(Reference ReferenceData);
	static bool LootEntity(Entity EntityData, Reference ReferenceData, Entity LocalPlayer, bool OnlyUseEntityLooterList, bool UseEntityLooterBlacklist);
	static bool CheckEntityLooterSettings(settingsClasses::EntityLooterSettings* CustomEntityLooterSettings);
	static bool CheckOnlyUseEntityLooterList(settingsClasses::EntityLooterSettings* CustomEntityLooterSettings);
	static bool HarvestFlora(Entity EntityData, Reference ReferenceData, Entity LocalPlayer);

	static bool CreateProjectile(DWORD ItemId, float* Position, float* Rotation);

	static bool SendMessageToServer(void* Message, size_t Size);
	static bool CreateForwardProjectile(DWORD ItemId);

	static DWORD64 GetCameraPtr();
	static BYTE CheckHealthFlag(BYTE HealthFlag);
	static DWORD64 GetLocalPlayerPtr(bool CheckMainMenu);
	static DWORD64* GetEntityList(int* Size);
	static DWORD64* GetNPCList(int* Size);
	static DWORD64* GetRecipeArray(int* Size);
	static bool CheckWhitelistedFlux(Reference ReferenceData);
	static bool FloraLeveledListValid(memoryClasses::LeveledList LeveledListData);
	static bool FloraValid(Reference ReferenceData);
	static bool IsTreasureMap(DWORD Formid);
	static bool CheckFactionFormid(DWORD Formid);
	static bool BlacklistedNPCFaction(Reference ReferenceData);
	static bool CheckReferenceItem(Reference ReferenceData);
	static void GetCustomEntityData(Reference ReferenceData, DWORD64* EntityFlag, DWORD64* EntityNamePtr, int* EnabledDistance, bool CheckScrap, bool CheckIngredient);
	static bool GetActorSnapshotComponentData(Entity EntityData, memoryClasses::ActorSnapshotComponent* ActorSnapshotComponentData);
	static char* GetEntityName(DWORD64 Ptr);

	inline static memoryClasses::CustomEntry* CustomEntityList = nullptr;
	inline static int CustomEntityListSize = 0;
	inline static int CustomEntityListCounter = 0;

	inline static memoryClasses::CustomEntry* BufferEntityList = nullptr;
	inline static int BufferEntityListSize = 0;

	inline static memoryClasses::CustomEntry* CustomNPCList = nullptr;
	inline static int CustomNPCListSize = 0;
	inline static int CustomNPCListCounter = 0;

	inline static memoryClasses::CustomEntry* BufferNPCList = nullptr;
	inline static int BufferNPCListSize = 0;

	inline static memoryClasses::CustomEntry* CustomPlayerList = nullptr;
	inline static int CustomPlayerListSize = 0;
	inline static int CustomPlayerListCounter = 0;

	inline static memoryClasses::CustomEntry* BufferPlayerList = nullptr;
	inline static int BufferPlayerListSize = 0;

	inline static memoryClasses::OldWeapon* OldWeaponList = nullptr;

	inline static int KnownRecipeArraySize = 0;

	virtual void __dummy() = 0;
};
class ErectusIni final {
public:
	static void ReadIniSettings();
	static void WriteIniSettings();

	static settingsClasses::OverlaySettingsA PlayerSettings;
	static settingsClasses::OverlaySettingsB JunkSettings;
	static settingsClasses::OverlaySettingsA NpcSettings;
	static settingsClasses::OverlaySettingsB ContainerSettings;
	static settingsClasses::OverlaySettingsB PlanSettings;
	static settingsClasses::OverlaySettingsB MagazineSettings;
	static settingsClasses::OverlaySettingsB BobbleheadSettings;
	static settingsClasses::OverlaySettingsB ItemSettings;
	static settingsClasses::OverlaySettingsB FloraSettings;
	static settingsClasses::OverlaySettingsB EntitySettings;

	static settingsClasses::ScrapLooterSettings CustomScrapLooterSettings;
	static settingsClasses::ItemLooterSettings CustomItemLooterSettings;
	static settingsClasses::WeaponSettings CustomWeaponSettings;
	static settingsClasses::TargetSettings CustomTargetSettings;
	static settingsClasses::LocalPlayerSettings CustomLocalPlayerSettings;
	static settingsClasses::OpkSettings CustomOpkSettings;
	static settingsClasses::UtilitySettings CustomUtilitySettings;
	static settingsClasses::SwapperSettings CustomSwapperSettings;
	static settingsClasses::TransferSettings CustomTransferSettings;
	static settingsClasses::TeleportSettings CustomTeleportSettings;
	static settingsClasses::NukeCodeSettings CustomNukeCodeSettings;
	static settingsClasses::LegendarySettings CustomLegendarySettings;
	static settingsClasses::FluxSettings CustomFluxSettings;
	static settingsClasses::EntityLooterSettings NPCLooterSettings;
	static settingsClasses::EntityLooterSettings ContainerLooterSettings;
	static settingsClasses::HarvesterSettings CustomHarvesterSettings;
	static settingsClasses::KnownRecipeSettings CustomKnownRecipeSettings;
	static settingsClasses::MeleeSettings CustomMeleeSettings;
	static settingsClasses::ChargenSettings CustomChargenSettings;
	static settingsClasses::ExtraNPCSettings CustomExtraNPCSettings;

	inline static settingsClasses::WeaponSettings DefaultWeaponSettings{ false,	false, false, false, false,	false, false, 250, false, 2.0f,	false, 500.0f };

private:
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
	static void GetEntityLooterSettings(const char* Section, settingsClasses::EntityLooterSettings* Value, settingsClasses::EntityLooterSettings* Default);
	static void SetEntityLooterSettings(const char* Section, settingsClasses::EntityLooterSettings* Value, settingsClasses::EntityLooterSettings* Default);
	static void GetHarvesterSettings();
	static void SetHarvesterSettings();
	static void GetKnownRecipeSettings();
	static void SetKnownRecipeSettings();
	static void GetMeleeSettings();
	static void SetMeleeSettings();
	static void GetChargenSettings();
	static void SetChargenSettings();
	static void GetExtraNPCSettings();
	static void SetExtraNPCSettings();
	static void GetBitMsgWriterSettings();
	static void SetBitMsgWriterSettings();
	static void GetExperimentalSettings();
	static void SetExperimentalSettings();

	virtual void __dummy() = 0;
};