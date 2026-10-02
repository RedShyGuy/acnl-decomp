#pragma once

#include "decomp.h"

namespace nn {
namespace pia {
namespace common {
class CallContext
{
public:
    void InitiateCall(); // 0x004267CC | fefates:bytes [tier B]
    void SignalCancel(); // 0x004267E4 | fefates:bytes [tier B]
    void SignalFailure(nn::Result); // 0x00426814 | fefates:bytes [tier B]
    void SignalSuccess(nn::Result); // 0x00426860 | fefates:bytes [tier B]
};
} // namespace common
} // namespace pia
} // namespace nn
