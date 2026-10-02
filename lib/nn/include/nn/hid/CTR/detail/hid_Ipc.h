#pragma once

#include "decomp.h"

namespace nn {
namespace hid {
namespace CTR {
namespace detail {
class Ipc
{
public:
    void GetIPCHandles(nn::Handle*, nn::Handle*, nn::Handle*, nn::Handle*, nn::Handle*, nn::Handle*); // 0x00354578 | nintendogs:bytes [tier A]
    void EnableAccelerometer(); // 0x00354620 | nintendogs:bytes [tier B]
};
} // namespace detail
} // namespace CTR
} // namespace hid
} // namespace nn
