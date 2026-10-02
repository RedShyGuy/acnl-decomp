#pragma once

#include "decomp.h"
#include "Other/dInOutWindow.h"

// vtable +0x152BC in ModuleAmiiboCamera.cro, offset_to_top 0, 13 entries
class AmiiboPhotoBG : public ::InOutWindow
{
public:
    AmiiboPhotoBG(); // ctor address unknown
    virtual ~AmiiboPhotoBG(); // ModuleAmiiboCamera.cro +0x001878 slot 0x00
};
