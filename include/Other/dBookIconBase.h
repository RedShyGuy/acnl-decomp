#pragma once

#include "decomp.h"
#include "state/dMode.h"

// RTTI 12BookIconBase @ 0x008CB508
// vtable 0x008EDFC4 (vptr 0x008EDFCC), offset_to_top 0, 3 entries
class BookIconBase : public ::state::Mode<BookIconBase>
{
public:
    BookIconBase(); // ctor candidate(s) 0x001F9040 (unverified)
    virtual void vf_0x00(); // 0x001F912C slot 0x00 | virtual slot, introduced by BookIconBase
    virtual void vf_0x04(); // 0x001F90EC slot 0x04 | virtual slot, introduced by BookIconBase
    virtual void vf_0x08(); // 0x0082AAB8 slot 0x08 | virtual slot, introduced by BookIconBase
};
