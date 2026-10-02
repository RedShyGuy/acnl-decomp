#pragma once

#include "decomp.h"
#include "Button/dButtonActionControl.h"

// RTTI 21MyButtonActionControl @ 0x008CCD18
// vtable 0x008F6390 (vptr 0x008F6398), offset_to_top 0, 15 entries
class MyButtonActionControl : public ::ButtonActionControl
{
public:
    MyButtonActionControl(); // ctor address unknown
    virtual ~MyButtonActionControl(); // 0x00328F90 slot 0x00 | slot vf_0x00 of ssys::st::List
    // 0x00328F80 slot 0x04 | slot vf_0x04 of ssys::st::List (deleting dtor)
};
