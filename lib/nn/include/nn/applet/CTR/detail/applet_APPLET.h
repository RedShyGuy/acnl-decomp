#pragma once

#include "decomp.h"

namespace nn {
namespace applet {
namespace CTR {
namespace detail {
class APPLET
{
public:
    void GetLockHandle(nn::Handle*, unsigned, unsigned*, unsigned*); // 0x00120518 | nintendogs:bytes [tier A]
    void GetTargetPlatform(nn::ptm::CTR::TargetPlatform*); // 0x00120574 | fefates:bytes [tier B]
    void PrepareToJumpToHomeMenu(); // 0x004813A4 | nintendogs:callgraph [tier A]
    void GetApplicationRunningMode(nn::applet::CTR::ApplicationRunningMode*); // 0x004813D4 | fefates:bytes [tier B]
};
} // namespace detail
} // namespace CTR
} // namespace applet
} // namespace nn
