#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x27F98 in ModuleShop.cro, offset_to_top 0, 89 entries
class AcNpcSpTakumiRealEstate : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpTakumiRealEstate(); // ctor address unknown
    virtual ~AcNpcSpTakumiRealEstate(); // ModuleShop.cro +0x022558 slot 0x00
};
