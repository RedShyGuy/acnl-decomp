#pragma once

#include "decomp.h"
#include "Other/dBase.h"

// vtable +0x15618 in ModuleAmiiboCamera.cro, offset_to_top 0, 16 entries
class BsMenuAmiiboShutter : public ::Base
{
public:
    BsMenuAmiiboShutter(); // ctor address unknown
    virtual ~BsMenuAmiiboShutter(); // ModuleAmiiboCamera.cro +0x00DCA4 slot 0x00
    virtual void Initialize(); // ModuleAmiiboCamera.cro +0x00DA1C slot 0x0C
    virtual void Finalize(); // ModuleAmiiboCamera.cro +0x00DBE0 slot 0x18
    virtual void Calc(); // ModuleAmiiboCamera.cro +0x00DB98 slot 0x24
    virtual void Draw(); // ModuleAmiiboCamera.cro +0x00D9F0 slot 0x30
};
