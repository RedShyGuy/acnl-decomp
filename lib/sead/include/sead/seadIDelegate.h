#pragma once

#include "decomp.h"

namespace sead {
class Heap;

// RTTI N4sead9IDelegateE @ 0x008D2388
//
// A callback object without arguments: slot 0x00 calls it, slot 0x04 copies it onto a heap (NULL
// for the interface). The names are ours.
class IDelegate
{
public:
    virtual void invoke() = 0;
    DECOMP_EMIT_COPY virtual IDelegate* clone(Heap*) const // 0x00750A50
    {
        return nullptr;
    }
};
} // namespace sead
