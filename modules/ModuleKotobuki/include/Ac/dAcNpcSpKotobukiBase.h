#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x43D0 in ModuleKotobuki.cro, offset_to_top 0, 91 entries
class AcNpcSpKotobukiBase : public ::AcNpcSp
{
public:
    class KotobukiTalkRecept;
    AcNpcSpKotobukiBase(); // ctor address unknown
    virtual ~AcNpcSpKotobukiBase(); // ModuleKotobuki.cro +0x002450 slot 0x00
};
