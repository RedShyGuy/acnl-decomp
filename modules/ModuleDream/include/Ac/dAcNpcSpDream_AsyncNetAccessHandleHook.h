#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpDream.h"
#include "sead/seadIDelegateR.h"

// vtable +0x13A60 in ModuleDream.cro, offset_to_top 0, 2 entries
class AcNpcSpDream::AsyncNetAccessHandleHook : public ::sead::IDelegateR<bool>
{
public:
    AsyncNetAccessHandleHook(); // ctor address unknown
};
