#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpKappei.h"
#include "sead/seadIDelegateR.h"

// vtable +0x3578 in ModuleKappei.cro, offset_to_top 0, 2 entries
class AcNpcSpKappei::AsyncAction : public ::sead::IDelegateR<bool>
{
public:
    AsyncAction(); // ctor address unknown
};
