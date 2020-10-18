#pragma once

#include <Windows.h>
#include <unordered_map>
#include <array>

#include "utils.h"
#include "game/Datatypes/TesObjectRefr.h"

class FalloutMain
{
public:
	std::uintptr_t vtable;//0x0
	char padding0008[0x338];
	std::uintptr_t platformSessionManagerPtr;//0x340
};

class PlatformSessionManager
{
public:
	std::uintptr_t vtable;//0x0
	char padding0008[0x78];
	std::uintptr_t clientAccountManagerPtr;//0x80
};

class BhkCharProxyController
{
public:
	std::uintptr_t vtable;//0x0
	BYTE padding0008[0x4A8];
	std::uintptr_t hknpBsCharacterProxyPtr;//0x4B0
};

class HknpBsCharacterProxy
{
public:
	std::uintptr_t vtable;//0x0
	BYTE padding0008[0x48];
	float rotation[2];//0x50
	BYTE padding0058[0x18];
	Vector3 position;//0x70
	BYTE padding007C[0x24];
	float velocityA[4];//0xA0
	float velocityB[4];//0xB0
};

class TransferMessage
{
public:
	std::uintptr_t vtable;//0x0
	std::uint32_t sourceEntityId;//0x8
	std::uint32_t playerEntityId;//0xC
	BYTE bShouldSendResult;//0x10
	char padding0011[3];
	std::uint32_t destEntityId;//0x14
	std::uint32_t itemServerHandleId;//0x18
	std::uint32_t stashAccessEntityId;//0x1C
	BYTE bCreateIfMissing;//0x20
	BYTE bIsExpectingResult;//0x21
	char padding0022[2];//0x22
	int count;//0x24
};

class ClientAccountManager
{
public:
	std::uintptr_t vtable;//0x0
	BYTE padding0008[0x38];
	std::uintptr_t clientAccountArrayPtr;//0x40
	BYTE padding0048[0x8];
	int clientAccountArraySizeA;//0x50
	BYTE padding0054[0x4];
	int clientAccountArraySizeB;//0x58
};

class LeveledList
{
public:
	std::uintptr_t vtable;//0x0
	char padding0008[0x18];
	std::uint32_t formId;//0x20
	char padding0024[0x8C];
	std::uintptr_t listEntryArrayPtr;//0xB0
	char padding00B8[0x9];
	BYTE listEntryArraySize;//0xC1
};

class RequestActivateRefMessage
{
public:
	std::uintptr_t vtable;//0x0
	std::uint32_t formId;//0x8
	BYTE choice;//0xC
	BYTE forceActivate;//0xB
};

class ClientAccountBuffer
{
public:
	std::uintptr_t namePtr;//0x0
	std::uintptr_t clientAccountPtr;//0x8
};

class ClientAccount
{
public:
	std::uintptr_t vtable;//0x0
	char padding0008[0x8];
	BYTE nameData[0x10];//0x10
	int nameLength;//0x20
	char padding0024[0x58];
	std::uint32_t formId;//0x7C
};

class Component
{
public:
	std::uintptr_t componentReferencePtr;//0x0
	std::uintptr_t componentCountReferencePtr;//0x8
};

class ReferenceList
{
public:
	std::uintptr_t arrayPtr;//0x0
	char padding0008[0x8];
	int arraySize;//0x10
};

class ItemCount
{
public:
	char padding0000[0x8];
	int count;//0x8
	char padding000C[0x4];
};

class ItemInstancedArray
{
public:
	char padding0000[0x10];
	std::uintptr_t arrayPtr;//0x10
	std::uintptr_t arrayEnd;//0x18
};

class ExtraTextDisplayData
{
public:
	std::uintptr_t vtable;//0x0
	char padding0008[0x8];
	std::uintptr_t instancedNamePtr;//0x10
};

class RequestHitsOnActors
{
public:
	std::uintptr_t vtable;//0x0
	std::uintptr_t hitsArrayPtr;//0x8
	std::uintptr_t hitsArrayEnd;//0x10
	char padding0018[0x48];
};

class BgsQuestText
{
public:
	std::uintptr_t vtable;//0x0
	std::uintptr_t formIdPtr;//0x8
	char padding0010[0x28];
	std::uintptr_t codePtr;//0x38
};

class ListEntry
{
public:
	std::uintptr_t referencePtr;//0x0
	std::uintptr_t extraData;//0x8
	std::uintptr_t conditions;//0x10
	float quantity;//0x18
	std::uint32_t quantityPadding;//0x1C
	std::uintptr_t quantityGlobal;//0x20
	std::uintptr_t quantityGlobalPadding;//0x28;
	float chanceNoneValue;//0x30
	std::uint32_t chanceNoneValuePadding;//0x34
	std::uintptr_t chanceNoneGlobal;//0x38
	std::uintptr_t chanceNoneCurveTable;//0x40
	float minimumLevel;//0x48
	std::uint32_t minimumLevelPadding;//0x4C
	std::uintptr_t minimumLevelGlobal;//0x50
	std::uintptr_t minimumLevelCurveTable;//0x58
	std::uintptr_t minimumLevelCurveTablePadding;//0x68
};

class ModInstance
{
public:
	std::uintptr_t modListPtr;//0x0
	int modListSize;//0x8
};

class ObjectInstanceExtra
{
public:
	char padding0000[0x10];
	std::uintptr_t modDataPtr;//0x10
};

class Hits
{
public:
	std::uint32_t valueA;//0x0 (Local Player)
	std::uint32_t valueB;//0x4 (Entity)
	std::uint32_t valueC;//0x8 (Projectile == 0)
	std::uint32_t initializationType;//0xC (3 == Default/Gun, 4 == Explosive)
	std::uint32_t uiWeaponServerId;//0x10
	std::uint32_t limbEnum;//0x20 (0xFFFFFFFF == Default/Body)
	std::uint32_t hitEffectId;//0x18 (0 == Default/Gun)
	std::uint32_t uEquipIndex;//0x1C (0 == Default/Gun)
	BYTE uAckIndex;//0x20 (Shots Hit, Always > 0)
	BYTE uFireId;//0x21 (Shots Fired)
	BYTE bPredictedKill;//0x22
	char padding0023;//0x23 (0)
	float explosionLocationX;//0x24
	float explosionLocationY;//0x28
	float explosionLocationZ;//0x2C
	float fProjectilePower;//0x30 (1.0f == Default)
	BYTE bVatsAttack;//0x34
	BYTE bVatsCritical;//0x35
	BYTE bTargetWasDead;//0x36
	char padding0037;//0x37 (0)
};

class RequestTeleportMessage
{
public:
	std::uintptr_t vtable;//0x0
	Vector3 position;//0x8
	Vector3 rotation;//0x14
	std::uintptr_t cellPtr;//0x20
};

class ClientStateMsg
{
public:
	std::uintptr_t vtable;//0x0
	std::uintptr_t clientState;//0x8
};

class Chargen
{
public:
	char padding[0x28];
	float thin;//0x28
	float muscular;//0x2C
	float large;//0x30
};

class CustomEntry
{
public:
	std::uintptr_t entityPtr;
	std::uintptr_t baseObjectPtr;
	std::uint32_t entityFormId;
	std::uint32_t baseObjectFormId;
	std::uintptr_t flag;
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
	std::uintptr_t address{};			//0x20 (0x0)
	std::uintptr_t rcx{};				//0x28 (0x8)
	std::uintptr_t rdx{};				//0x30 (0x10)
	std::uintptr_t r8{};				//0x38 (0x18)
	std::uintptr_t r9{};				//0x40 (0x20)
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
	std::uintptr_t targetLockingPtr{};
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

	TesObjectRefr refr;
	TesItem base;
};

class ErectusMemory final {
public:
	static std::uintptr_t GetAddress(std::uint32_t formId);
	static std::uintptr_t GetPtr(std::uint32_t formId);

	//send damage
	static std::unordered_map<int, std::string> GetFavoritedWeapons();
	static std::uint32_t GetFavoritedWeaponId(BYTE favouriteIndex);
	static char FavoriteIndex2Slot(BYTE favoriteIndex);
	static bool SendDamage(std::uintptr_t targetPtr, std::uint32_t weaponId, BYTE* shotsHit, BYTE* shotsFired, BYTE count);

	//nuke codes
	static void UpdateNukeCodes();
	inline static std::array<int, 8> alphaCode = { };
	inline static std::array<int, 8> bravoCode = { };
	inline static std::array<int, 8> charlieCode = { };

	//teleporter
	static bool SaveTeleportPosition(int index);
	static bool RequestTeleport(int index);

	//freezeAP
	static bool FreezeActionPoints(std::uintptr_t& freezeApPage, bool& freezeApPageValid, bool enabled);

	//noclip
	static bool SetClientState(std::uintptr_t clientState);
	static void Noclip(bool enabled);

	//item transfer
	static bool CheckItemTransferList();
	static bool TransferItems(std::uint32_t sourceFormId, std::uint32_t destinationFormId);

	//esp
	static bool UpdateBufferEntityList();
	static bool UpdateBufferPlayerList();
	inline static std::vector<CustomEntry> entityDataBuffer{};
	inline static std::vector<CustomEntry> playerDataBuffer{};

	static bool ReferenceSwap(std::uint32_t& sourceFormId, std::uint32_t& destinationFormId);

	static bool DamageRedirection(std::uintptr_t targetPtr, std::uintptr_t& targetingPage, bool& targetingPageValid, bool isExiting, bool enabled);

	static bool PositionSpoofing(bool enabled);

	static bool MeleeAttack();
	static bool ChargenEditing();

	static bool IsFloraHarvested(char harvestFlagA, char harvestFlagB);

	static bool IsTargetValid(std::uintptr_t targetPtr);
	static bool IsTargetValid(const TesObjectRefr& targetData);

	inline static std::uintptr_t targetLockedEntityPtr = 0;

	static void    GetCustomEntityData(const TesItem& baseObject, std::uintptr_t& entityFlag, int& enabledDistance);
	static bool    CheckFormIdArray(std::uint32_t formId, const bool* enabledArray, const std::uint32_t* formIdArray, int size);
	static std::uintptr_t RttiGetNamePtr(std::uintptr_t vtable);
	static bool    VtableSwap(std::uintptr_t dst, std::uintptr_t src);

	static ItemInfo GetItemInfo(const TesObjectRefr& entity);
	static std::string GetEntityName(std::uintptr_t ptr);

	static bool CheckReferenceKeywordMisc(const TesItem& referenceData, std::uint32_t formId);
	static bool CheckReferenceKeywordBook(const TesItem& referenceData, std::uint32_t formId);

private:
	[[nodiscard]] static std::uint32_t GenerateCrc32(std::uint32_t formId) noexcept;

	static std::array<int, 8> GetNukeCode(std::uint32_t formId);
	static std::string GetPlayerName(const ClientAccount& clientAccountData);

	static bool CheckWhitelistedFlux(const TesItem& referenceData);
	
	static bool IsItem(const TesItem& referenceData);
	static bool IsRecipeKnown(std::uint32_t formId);

	static bool MovePlayer();

	static std::uint32_t GetEntityId(const TesObjectRefr& entityData);
	static bool SendHitsToServer(Hits* hitsData, size_t hitsDataSize);
	static std::uintptr_t GetNukeCodePtr(std::uint32_t formId);
	static std::string GetInstancedItemName(std::uintptr_t displayPtr);

	virtual void Dummy() = 0;
};
