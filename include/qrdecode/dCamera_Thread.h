#pragma once

#include "decomp.h"
#include "qrdecode/dCamera.h"
#include "sead/seadThread.h"

// RTTI N8qrdecode6Camera6ThreadE @ 0x008D4078
// vtable 0x0090C13C (vptr 0x0090C144), offset_to_top 0, 18 entries
// vtable 0x0090C18C (vptr 0x0090C194), offset_to_top -24, 1 entries
class qrdecode::Camera::Thread : public ::sead::Thread
{
public:
    Thread(); // ctor address unknown
    virtual ~Thread(); // 0x006CA7C0 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x006CA7B0 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
    virtual void vf_0x40(); // 0x006CA694 slot 0x40 | virtual slot, introduced by sead::Thread
};
