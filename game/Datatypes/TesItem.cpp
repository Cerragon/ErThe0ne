#include "TesItem.h"

#include "../../ErectusMemory.h"

std::string TesItem::GetName() const
{
	std::uintptr_t entityNamePtr = 0;

	switch (GetFormType())
	{
	case FormType::AlchemyItem:
	case FormType::TesObjectArmo:
	case FormType::TesObjectBook:
	case FormType::TesObjectMisc:
	case FormType::CurrencyObject:
	case FormType::TesFlora:
	case FormType::TesObjectWeap:
	case FormType::TesAmmo:
	case FormType::TesUtilityItem:
	case FormType::BgsNote:
	case FormType::TesKey:
		entityNamePtr = namePtr0098;
		break;
	case FormType::TesObjectCont:
		entityNamePtr = namePtr00B0;
		break;
	case FormType::TesNpc:
		entityNamePtr = namePtr0178;
		break;
	default:
		break;
	}

	return ErectusMemory::GetEntityName(entityNamePtr);
}

bool TesItem::IsMagazine() const
{
	return GetFormType() == FormType::TesObjectBook && ErectusMemory::CheckReferenceKeywordBook(*this, 0x001D4A70);
}

bool TesItem::IsBobblehead() const
{
	return GetFormType() == FormType::TesObjectMisc && ErectusMemory::CheckReferenceKeywordMisc(*this, 0x00135E6C);
}
