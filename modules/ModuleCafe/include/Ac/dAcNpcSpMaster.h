#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0xD6A0 in ModuleCafe.cro, offset_to_top 0, 89 entries
class AcNpcSpMaster : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpMaster(); // ctor address unknown
    virtual ~AcNpcSpMaster(); // ModuleCafe.cro +0x0055E8 slot 0x00
};
