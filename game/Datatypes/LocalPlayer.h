#pragma once
#include "TesObjectCell.h"
#include "../../ErectusProcess.h"

class LocalPlayer : public TesObjectRefr
{
public:
	[[nodiscard]] bool IsIngame() const { return formId != 0x00000014; }
	[[nodiscard]] std::uint32_t GetStashFormId() const { return IsIngame() ? playerStashFormId : 0; }
	[[nodiscard]] TesObjectCell GetCurrentCell() const
	{
		TesObjectCell result = {};
		ErectusProcess::Rpm(cellPtr, &result, sizeof result);
		return result;
	}
};

