#pragma once

#include "decomp.h"

namespace nn {
namespace nex {
class PacketDispatchQueue
{
public:
    void GetNextToDispatch(); // 0x003941B8 | fefates:bytes [tier B]
    void Purge(); // 0x00394244 | fefates:bytes [tier B]
    void Queue(nn::nex::PacketIn*); // 0x00394324 | fefates:bytes [tier B]
    PacketDispatchQueue(); // 0x00394534 | mk7dlp:bytes [tier A]
};
} // namespace nex
} // namespace nn
