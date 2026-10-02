#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpPostOffice.h"
#include "sead/seadIDelegateR.h"

// vtable +0x689C4 in ModuleIndoor.cro, offset_to_top 0, 2 entries
class AcNpcSpPostOffice::AddSaveHook : public ::sead::IDelegateR<bool>
{
public:
    AddSaveHook(); // ctor address unknown
};
