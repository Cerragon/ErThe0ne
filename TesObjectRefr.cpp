#include "TesObjectRefr.h"

#include "ErectusProcess.h"

TesItem TesObjectRefr::GetBaseObject() const
{
	TesItem result = {};
	ErectusProcess::Rpm(baseObjectPtr, &result, sizeof result);

	return result;
}

ActorState TesObjectRefr::GetActorState() const
{
	if (GetFormType() != FormType::PlayerCharacter && GetFormType() != FormType::TesActor)
		return ActorState::Unknown;

	auto flag = healthFlag;
	flag &= ~(1 << 7);
	flag &= ~(1 << 6);
	flag &= ~(1 << 5);
	switch (flag)
	{
	case 0x00:
		return ActorState::Alive;
	case 0x02:
	case 0x04:
		return ActorState::Dead;
	case 0x10:
	case 0x12:
		return ActorState::Downed;
	default:
		return ActorState::Unknown;
	}
}

ActorSnapshotComponent TesObjectRefr::GetActorSnapshot() const
{
	ActorSnapshotComponent result = {};
	if (!Utils::Valid(actorCorePtr))
		return result;

	DWORD64 actorCoreBufferA;
	if (!ErectusProcess::Rpm(actorCorePtr + 0x70, &actorCoreBufferA, sizeof actorCoreBufferA))
		return result;
	if (!Utils::Valid(actorCoreBufferA))
		return result;

	DWORD64 actorCoreBufferB;
	if (!ErectusProcess::Rpm(actorCoreBufferA + 0x8, &actorCoreBufferB, sizeof actorCoreBufferB))
		return result;
	if (!Utils::Valid(actorCoreBufferB))
		return result;

	ErectusProcess::Rpm(actorCoreBufferB, &result, sizeof result);

	return result;
}
