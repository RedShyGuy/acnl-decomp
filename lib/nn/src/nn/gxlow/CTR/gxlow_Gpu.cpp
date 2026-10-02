#include "nn/gxlow/CTR/gxlow_Gpu.h"

namespace nn {
namespace gxlow {
namespace CTR {
// 0x001314B0 | nintendogs:bytes [tier A]
void nn::gxlow::CTR::Gpu::WriteHWRegs(unsigned, const unsigned char*, unsigned)
{
}

// 0x001314F8 | nintendogs:bytes [tier A]
void nn::gxlow::CTR::Gpu::AcquireRight(nn::Handle, bool)
{
}

// 0x0013153C | nintendogs:bytes [tier A]
void nn::gxlow::CTR::Gpu::ReleaseRight()
{
}

// 0x00131564 | nintendogs:bytes [tier A]
void nn::gxlow::CTR::Gpu::SetLcdForceBlack(bool)
{
}

// 0x001315C4 | nintendogs:bytes [tier A]
void nn::gxlow::CTR::Gpu::WriteHWRegsWithMask(unsigned, const unsigned char*, const unsigned char*, unsigned)
{
}

// 0x00137218 | nintendogs:bytes [tier A]
void nn::gxlow::CTR::Gpu::ReadHWRegs(unsigned, unsigned char*, unsigned)
{
}

// 0x0013726C | nintendogs:bytes [tier A]
void nn::gxlow::CTR::Gpu::RegisterInterruptRelayQueue(nn::Handle, unsigned, nn::Handle*, int*)
{
}

// 0x0013B6B8 | nintendogs:bytes [tier A]
void nn::gxlow::CTR::Gpu::FlushDataCache(nn::Handle, unsigned, unsigned)
{
}

// 0x0013EE58 | nintendogs:bytes [tier A]
void nn::gxlow::CTR::Gpu::TriggerCmdReqQueue()
{
}

// 0x00144C58 | fefates:bytes [tier B]
void nn::gxlow::CTR::Gpu::StoreDataCache(nn::Handle, unsigned int, unsigned int)
{
}

// 0x0047F874 | nintendogs:bytes [tier A]
void nn::gxlow::CTR::Gpu::ImportDisplayCaptureInfo(nn::gxlow::CTR::DisplayCaptureInfo*)
{
}

} // namespace CTR
} // namespace gxlow
} // namespace nn
