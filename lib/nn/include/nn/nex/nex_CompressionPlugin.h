#pragma once

#include "decomp.h"
#include "nn/nex/nex_Plugin.h"

namespace nn {
namespace nex {
class CompressionAlgorithm;

// A plugin that creates compression algorithms. The names of the functions are ours.
// RTTI N2nn3nex17CompressionPluginE @ 0x008CE680
class CompressionPlugin : public ::nn::nex::Plugin
{
public:
    // (inline)
    CompressionPlugin() {}
    virtual u32 GetType(); // 0x00386254 slot 0x1C
    virtual nn::nex::CompressionAlgorithm* CreateAlgorithm() = 0;                    // slot 0x38
    virtual void DeleteAlgorithm(nn::nex::CompressionAlgorithm* pAlgorithm) = 0;     // slot 0x3C
};
} // namespace nex
} // namespace nn
