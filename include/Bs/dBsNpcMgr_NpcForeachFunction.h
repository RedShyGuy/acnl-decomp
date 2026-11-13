#pragma once

#include "decomp.h"
#include "Bs/dBsNpcMgr.h"

// RTTI N8BsNpcMgr18NpcForeachFunctionE @ 0x008D3FF4
class BsNpcMgr::NpcForeachFunction
{
public:
    NpcForeachFunction(); // ctor address unknown
    // (the derived functions get each NPC of the list; name is ours)
    virtual void Invoke(AcNpc* npc);
};
