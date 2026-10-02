#pragma once

#include "decomp.h"

namespace nn {
namespace nex {
class NetworkEmulator
{
public:
    NetworkEmulator(); // TODO: default ctor added so derived stubs compile - may not exist
    void Queue(nn::nex::Buffer*, nn::nex::InetAddress&); // 0x0037A6AC | fefates:bytes [tier B]
    void Dequeue(nn::nex::Buffer*); // 0x0037A7C4 | fefates:bytes [tier B]
    NetworkEmulator(nn::nex::EmulationDevice*); // 0x0037AD74 | mk7dlp:callseq [tier A]
};
} // namespace nex
} // namespace nn
