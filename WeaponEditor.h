#pragma once
#include <unordered_map>
#include <Windows.h>

class WeaponEditor
{
public:
	static void EditWeapons(bool enabled);
	static bool InfiniteAmmo(bool enabled);
	
private:
	struct WeaponInfo
	{
		DWORD64 ptr;
		struct Weapon
		{
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
			float reachEngagementMult; //0x030C
			float minRange; //0x0310
			float maxRange; //0x0314
			float padding0318; //0x0318
			float attackDelay; //0x031C
			float padding0320; //0x0320
			float damage; //0x0324
			float padding0328; //0x0328
			float padding032C; //0x032C
			float weight; //0x0330
			float padding0334; //0x0334
			float actionPointCost;//0x338
			BYTE padding033C[0x24];
			BYTE flagA;//0x360
			BYTE flagB;//0x361
			BYTE flagC;//0x362
			BYTE flagD;//0x363
			BYTE padding0364[0x1C];
			short capacity;//0x380
			short baseDamage;//0x382

			bool operator==(const Weapon&) const = default;
		} weaponData;

		struct AimModel
		{
			DWORD64 vtable;//0x8
			BYTE padding0008[0x18];
			DWORD formId;//0x20
			BYTE padding0024[0x4];
			BYTE spreadData[0x18];//0x28
			BYTE recoilData[0x24];//0x40
			float sway;//0x64

			bool operator==(const AimModel&) const = default;
		} aimModelData;
	};
	static void ResetWeapons();

	inline static std::unordered_map<DWORD, WeaponInfo> originalWeaponValues = {};
};

