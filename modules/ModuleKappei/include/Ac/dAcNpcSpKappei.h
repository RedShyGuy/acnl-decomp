#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x3248 in ModuleKappei.cro, offset_to_top 0, 89 entries
class AcNpcSpKappei : public ::AcNpcSp
{
public:
    class AsyncAction;
    class TalkRecept;
    AcNpcSpKappei(); // ctor address unknown
    virtual ~AcNpcSpKappei(); // ModuleKappei.cro +0x0020A4 slot 0x00
    virtual void Initialize(); // ModuleKappei.cro +0x0028CC slot 0x0C
    virtual void Finalize(); // ModuleKappei.cro +0x002968 slot 0x18
    virtual void Unk0(); // ModuleKappei.cro +0x0024C4 slot 0x3C
};
