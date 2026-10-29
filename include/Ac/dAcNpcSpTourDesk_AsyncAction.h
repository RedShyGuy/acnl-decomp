#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpTourDesk.h"
#include "sead/seadIDelegateR.h"

// RTTI N15AcNpcSpTourDesk11AsyncActionE @ 0x008CDA60
// vtable 0x008FB9C8 (vptr 0x008FB9D0), offset_to_top 0, 2 entries
class AcNpcSpTourDesk::AsyncAction : public ::sead::IDelegateR<bool>
{
public:
    AsyncAction(); // ctor address unknown
    virtual bool invoke(); // 0x00282F8C slot 0x00
};
