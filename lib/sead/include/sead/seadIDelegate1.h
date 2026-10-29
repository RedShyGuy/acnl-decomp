#pragma once

#include "decomp.h"

namespace sead {
class Heap;

// Instantiations found in the binary:
//   sead::IDelegate1<HumanModel*>  typeinfo 0x008D1398
//   sead::IDelegate1<script::Phrase*>  typeinfo 0x008D13A0
//   sead::IDelegate1<sead::TaskBase*>  typeinfo 0x008D13A8
//
// A callback object with one argument; the one slot calls it (as pead; the slot name is ours).
//
// A callback object with one argument (slots as IDelegate; names are ours).
template <typename T0>
class IDelegate1
{
public:
    virtual void invoke(T0 arg) = 0;
    virtual IDelegate1* clone(Heap*) const { return nullptr; }
};
} // namespace sead
