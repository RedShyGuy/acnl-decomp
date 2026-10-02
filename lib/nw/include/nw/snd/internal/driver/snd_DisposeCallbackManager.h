#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
class DisposeCallbackManager
{
public:
    void GetInstance(); // 0x004D3A60 | fefates:bytes [tier B]
    void Dispose(const void*, unsigned long); // 0x004D3ACC | fefates:bytes [tier B]
};
} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
