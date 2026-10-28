#pragma once

#include "decomp.h"

namespace sead {
// RTTI N4sead11PrintOutputE @ 0x008D141C
//
// The target of a PrintFormatter: slot 0x08 takes the characters (the name of the slot is ours).
class PrintOutput
{
public:
    virtual ~PrintOutput() {}
    // writes length characters of str (length < 0: up to the null character)
    virtual void write(const char* str, s32 length) = 0;
};
} // namespace sead
