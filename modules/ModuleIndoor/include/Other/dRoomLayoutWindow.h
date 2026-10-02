#pragma once

#include "decomp.h"
#include "Other/dInOutWindow.h"

// vtable +0x666C4 in ModuleIndoor.cro, offset_to_top 0, 13 entries
class RoomLayoutWindow : public ::InOutWindow
{
public:
    RoomLayoutWindow(); // ctor address unknown
    virtual ~RoomLayoutWindow(); // ModuleIndoor.cro +0x0223F8 slot 0x00
};
