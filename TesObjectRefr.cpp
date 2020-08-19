#include "TesObjectRefr.h"

#include "ErectusProcess.h"

TesItem TesObjectRefr::GetBaseObject() const
{
	TesItem result = {};
	ErectusProcess::Rpm(baseObjectPtr, &result, sizeof result);

    return result;
}
