#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// The base of the algorithms (encryption, checksum, compression). The member name is ours.
// RTTI N2nn3nex12PluginObjectE @ 0x008CE15C
class PluginObject : public ::nn::nex::RootObject
{
public:
    // (inline)
    PluginObject() : m_Unknown0x04(0) {}

    u32 m_Unknown0x04; // 0x04 (after the vptr of the derived class)
};
} // namespace nex
} // namespace nn
