#pragma once

#include <Windows.h>

#include <unordered_map>
#include <unordered_set>
#include <array>

enum class FormTypes : BYTE
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
	PlayerCharacter = 0xB5 //also used in REFR objects, ref to player
};

//no change in 1.32.10
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
	DWORD64 baseObjectPtr;//0xB8
	BYTE padding00C0[0xE];
	BYTE spawnFlag;//0xCE
	BYTE padding00Cf[0xC9];
	BYTE movementFlag;//0x198
	BYTE sprintFlag;//0x199
	BYTE healthFlag;//0x19A
	BYTE padding019B[0xA89];
	DWORD formId0C24;//0xC24
};

class LoadedAreaManager
{
public:
	BYTE padding0000[0x80];
	DWORD64 interiorCellArrayBegin;//0x80
	DWORD64 interiorCellArrayEnd;//0x88
	BYTE padding0090[0x18];
	DWORD64 exteriorCellArrayBegin;//0xA8
	DWORD64 exteriorCellArrayEnd;//0xB0
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

class ActorSnapshotComponent
{
public:
	//ActorCoreSnapshotComponent
	DWORD64 actorCorevtable;//0x0
	BYTE actorCorePadding0008[0x98];
	//ActorServerAuthSnapshotData
	DWORD64 vtable;//0xA0
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

//changed + fixed
class TesObjectCell
{
public:
	BYTE padding0000[0x68];
	DWORD64 isInterior;
	BYTE loadedState;
	BYTE padding0071[0x2F];
	DWORD64 objectListBeginPtr;//0xA0
	DWORD64 objectListEndPtr;//0xA8
};

class TransferMessage
{
public:
	DWORD64 vtable;//0x0
	DWORD sourceEntityId;//0x8
	DWORD playerEntityId;//0xC
	BYTE bShouldSendResult;//0x10
	BYTE padding0X11[3];
	DWORD destEntityId;//0x14
	DWORD itemServerHandleId;//0x18
	DWORD stashAccessEntityId;//0x1C
	BYTE bCreateIfMissing;//0x20
	BYTE bIsExpectingResult;//0x21
	BYTE padding0X22[2];//0x22
	int count;//0x24
};

//changed

//unknown
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

class Inventory
{
public:
	DWORD64 vtable;//0x0
	BYTE padding0008[0x58];
	DWORD64 entryArrayBegin;//0x60
	DWORD64 entryArrayEnd;//0x68
};

class InventoryEntry
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
	std::string name;
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

struct LocalPlayerInfo
{
	DWORD formId;
	DWORD stashFormId;
	DWORD cellFormId;
	float position[3];
	float yaw;
	float pitch;
	float currentHealth;
};

enum class ItemTypes
{
	Invalid,
	Other,

	Npc,
	Container,
	Flora,
	
	Weapons,
	Apparel,
	
	Misc,

	Aid,
	AidBobblehead,
	AidMagazine,
	
	NotesKnownPlan,
	NotesUnknownPlan,
	NotesTreasureMap,
	Notes,

	Mod,
	Junk,
	Ammo,
	Currency,
};

struct ItemInfo
{
	ItemTypes type;
	
	DWORD64 namePtr;
	TesObjectRefr refr;
	TesItem base;

	std::string GetName();
};

class ErectusMemory final {
public:
	static DWORD64 GetAddress(DWORD formId);
	static DWORD64 GetPtr(DWORD formId);

	static std::unordered_map<int, std::string> GetFavoritedWeapons();
	static std::string GetFavoritedWeaponText(BYTE index);
	static DWORD GetFavoritedWeaponId(BYTE favouriteIndex);
	static char FavoriteIndex2Slot(BYTE favoriteIndex);

	static DWORD GetLocalPlayerFormId();
	static DWORD GetStashFormId();
	static void UpdateNukeCodes();
	static bool UpdateBufferEntityList();
	static bool UpdateBufferPlayerList();

	static void DeleteOldWeaponList();
	static bool UpdateOldWeaponData();

	static bool CheckItemTransferList();

	static bool TransferItems(DWORD sourceFormId, DWORD destinationFormId);
	static bool ReferenceSwap(DWORD& sourceFormId, DWORD& destinationFormId);

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

	static bool InsideInteriorCell();
	static LocalPlayerInfo GetLocalPlayerInfo();
	static Camera GetCameraInfo();
	static bool IsFloraHarvested(BYTE harvestFlagA, BYTE harvestFlagB);
	static BYTE CheckHealthFlag(BYTE healthFlag);
	static bool GetActorSnapshotComponentData(const TesObjectRefr& entityData, ActorSnapshotComponent* actorSnapshotComponentData);
	static bool TargetValid(const TesObjectRefr& entityData);

	
	inline static bool oldWeaponListUpdated = false;
	inline static int oldWeaponListCounter = 0;
	inline static int oldWeaponListSize = 0;

	inline static int targetLockingCooldown = 0;
	inline static DWORD64 targetLockingPtr = 0;

	inline static bool targetLockingKeyPressed = false;

	inline static std::vector<CustomEntry> entityDataBuffer{};
	inline static std::vector<CustomEntry> playerDataBuffer{};


	inline static std::array<int, 8> alphaCode = { };
	inline static std::array<int, 8> bravoCode = { };
	inline static std::array<int, 8> charlieCode = { };

	static DWORD64 GetLocalPlayerPtr(bool checkMainMenu);
	static std::vector<DWORD64> GetEntityPtrList();
	static void GetCustomEntityData(const TesItem& referenceData, DWORD64* entityFlag, DWORD64* entityNamePtr, int* enabledDistance, bool checkScrap, bool checkIngredient);
	static bool CheckFormIdArray(DWORD formId, const bool* enabledArray, const DWORD* formIdArray, int size);
	static DWORD64 RttiGetNamePtr(DWORD64 vtable);
	static bool IsBobblehead(const TesItem& tesItem);
	static bool IsMagazine(const TesItem& tesItem);
	static ItemInfo GetItemInfo(const TesObjectRefr& entity, const TesItem& base);

private:
	static bool GetNukeCode(DWORD formId, std::array<int, 8>& nukeCode);
	static std::string GetPlayerName(const ClientAccount& clientAccountData);

	static bool CheckReferenceKeywordBook(const TesItem& referenceData, DWORD formId);
	static bool CheckReferenceKeywordMisc(const TesItem& referenceData, DWORD formId);

	static bool IsJunk(const TesItem& referenceData);
	static bool IsMod(const TesItem& referenceData);
	static bool IsPlan(const TesItem& referenceData);

	static int GetOldWeaponIndex(DWORD formId);

	static bool IsRecipeKnown(DWORD formId);

	static bool LockedTargetValid(bool* isPlayer);

	static bool MovePlayer();

	static bool CheckOpkDistance(DWORD64 opkPage, bool players);

	static DWORD GetEntityId(const TesObjectRefr& entityData);
	static bool SendHitsToServer(Hits* hitsData, size_t hitsDataSize);
	static DWORD64 GetNukeCodePtr(DWORD formId);
	static std::string GetInstancedItemName(DWORD64 displayPtr);
	
	static DWORD64 GetCameraPtr();

	static bool CheckWhitelistedFlux(const TesItem& referenceData);
	static bool IsTreasureMap(const TesItem& referenceData);
	static bool CheckFactionFormId(DWORD formId);
	static bool BlacklistedNpcFaction(const TesItem& referenceData);
	static bool CheckReferenceItem(const TesItem& referenceData);

	static std::string GetEntityName(DWORD64 ptr);

	static bool VtableSwap(DWORD64 dst, DWORD64 src);
	
	inline static OldWeapon* oldWeaponList = nullptr;

	virtual void __dummy();
};
