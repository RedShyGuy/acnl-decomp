#pragma once

#include "decomp.h"
#include "Npc/dNpcTalkRecept.h"
#include "sead/seadIDelegateR.h"

// RTTI N13NpcTalkRecept19AsyncSaveHandleHookE @ 0x008CD9AC
// vtable 0x008FB3F4 (vptr 0x008FB3FC), offset_to_top 0, 2 entries
class NpcTalkRecept::AsyncSaveHandleHook : public ::sead::IDelegateR<bool>
{
public:
    AsyncSaveHandleHook(); // ctor address unknown
    virtual bool invoke(); // 0x00237928 slot 0x00
};
