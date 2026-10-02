#include "nn/applet/CTR/detail/applet_APPLET.h"

namespace nn {
namespace applet {
namespace CTR {
namespace detail {
// 0x00120518 | nintendogs:bytes [tier A]
void nn::applet::CTR::detail::APPLET::GetLockHandle(nn::Handle*, unsigned, unsigned*, unsigned*)
{
}

// 0x00120574 | fefates:bytes [tier B]
void nn::applet::CTR::detail::APPLET::GetTargetPlatform(nn::ptm::CTR::TargetPlatform*)
{
}

// 0x004813A4 | nintendogs:callgraph [tier A]
void nn::applet::CTR::detail::APPLET::PrepareToJumpToHomeMenu()
{
}

// 0x004813D4 | fefates:bytes [tier B]
void nn::applet::CTR::detail::APPLET::GetApplicationRunningMode(nn::applet::CTR::ApplicationRunningMode*)
{
}

} // namespace detail
} // namespace CTR
} // namespace applet
} // namespace nn
