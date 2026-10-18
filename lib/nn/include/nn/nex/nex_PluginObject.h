#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
class Plugin;

// The base of the algorithms (encryption, checksum, compression). The member name is ours.
// RTTI N2nn3nex12PluginObjectE @ 0x008CE15C
class PluginObject : public ::nn::nex::RootObject
{
public:
    // (inline)
    PluginObject() : m_pPlugin(nullptr) {}

    Plugin* m_pPlugin; // 0x04 (that created it; after the vptr of the derived class)
};
} // namespace nex
} // namespace nn
