#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpShop.h"

// vtable +0x26E60 in ModuleShop.cro, offset_to_top 0, 100 entries
class AcNpcSpShopKinuyo : public ::AcNpcSpShop
{
public:
    class LoadHandleHook;
    class SaveHandleHook;
    class TalkRecept;
    AcNpcSpShopKinuyo(); // ctor address unknown
    virtual ~AcNpcSpShopKinuyo(); // ModuleShop.cro +0x00DFD0 slot 0x00
    virtual void Calc(); // ModuleShop.cro +0x00D6F8 slot 0x24
};
