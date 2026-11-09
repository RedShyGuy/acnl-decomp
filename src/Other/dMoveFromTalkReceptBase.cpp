#include "script/dITalkRecept.h"
#include "script/dIFlowRecept.h"
#include "Other/dMoveFromTalkReceptBase.h"

// ctor address unknown
MoveFromTalkReceptBase::MoveFromTalkReceptBase() : IFlowRecept(this)
{
}

// 0x00330CC0 slot 0x00 | slot vf_0x00 of script::ITalkRecept
MoveFromTalkReceptBase::~MoveFromTalkReceptBase()
{
}

// 0x00330B38 slot 0x94
void MoveFromTalkReceptBase::Slot94()
{
}

// 0x00330430 slot 0x9C
void MoveFromTalkReceptBase::OnMessageStart(script::TalkLabel*)
{
}

// 0x00330828 slot 0xAC
void MoveFromTalkReceptBase::OnEnd()
{
}

// 0x00330AA8 slot 0xFC | virtual slot, introduced by MoveFromTalkReceptBase
void MoveFromTalkReceptBase::vf_0xFC()
{
}

