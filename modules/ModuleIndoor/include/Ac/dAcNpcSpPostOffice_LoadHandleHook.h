#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpPostOffice.h"
#include "sead/seadIDelegateR.h"

// vtable +0x689E4 in ModuleIndoor.cro, offset_to_top 0, 2 entries
class AcNpcSpPostOffice::LoadHandleHook : public ::sead::IDelegateR<bool>
{
public:
    LoadHandleHook(); // ctor address unknown
};
