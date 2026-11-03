#pragma once

#include "decomp.h"
#include "Other/dBase.h"

// vtable +0x15618 in ModuleAmiiboCamera.cro, offset_to_top 0, 16 entries
class BsMenuAmiiboShutter : public ::Base
{
public:
    BsMenuAmiiboShutter(); // ctor address unknown
    virtual ~BsMenuAmiiboShutter(); // ModuleAmiiboCamera.cro +0x00DCA4 slot 0x00
    virtual oml::framework::Result Initialize(); // ModuleAmiiboCamera.cro +0x00DA1C slot 0x0C
    virtual oml::framework::Result Finalize(); // ModuleAmiiboCamera.cro +0x00DBE0 slot 0x18
    virtual oml::framework::Result Calc(); // ModuleAmiiboCamera.cro +0x00DB98 slot 0x24
    virtual oml::framework::Result Draw(); // ModuleAmiiboCamera.cro +0x00D9F0 slot 0x30
};
