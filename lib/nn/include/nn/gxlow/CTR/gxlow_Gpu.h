#pragma once

#include "decomp.h"
#include "nn/Handle.h"
#include "nn/Result.h"

namespace nn {
namespace gxlow {
namespace CTR {
struct DisplayCaptureInfo;

// The commands of the service gsp::Gpu (3dbrew "GSPGPU"); the object holds the session. The
// function names are from the symbols (or after 3dbrew), the member name is ours.
class Gpu
{
public:
    nn::Result WriteHWRegs(unsigned regAddress, const unsigned char* data, unsigned size); // 0x001314B0 | nintendogs:bytes [tier A]
    nn::Result WriteHWRegsWithMask(unsigned regAddress, const unsigned char* data, const unsigned char* mask, unsigned size); // 0x001315C4 | nintendogs:bytes [tier A]
    nn::Result ReadHWRegs(unsigned regAddress, unsigned char* data, unsigned size); // 0x00137218 | nintendogs:bytes [tier A]
    nn::Result FlushDataCache(nn::Handle process, unsigned address, unsigned size); // 0x0013B6B8 | nintendogs:bytes [tier A]
    nn::Result SetLcdForceBlack(bool isBlack); // 0x00131564 | nintendogs:bytes [tier A]
    nn::Result TriggerCmdReqQueue(); // 0x0013EE58 | nintendogs:bytes [tier A]
    // the queue of the interrupts for this process: its event, flags (bit 0/1 application/applet,
    // bit 2 fatal error mode); returns the shared memory and the index of the thread in it
    nn::Result RegisterInterruptRelayQueue(nn::Handle event, unsigned flags, nn::Handle* pSharedMemory, int* pThreadIndex); // 0x0013726C | nintendogs:bytes [tier A]
    nn::Result AcquireRight(nn::Handle process, bool flags); // 0x001314F8 | nintendogs:bytes [tier A]
    nn::Result ReleaseRight(); // 0x0013153C | nintendogs:bytes [tier A]
    nn::Result ImportDisplayCaptureInfo(nn::gxlow::CTR::DisplayCaptureInfo* pInfo); // 0x0047F874 | nintendogs:bytes [tier A]
    nn::Result SaveVramSysArea(); // 0x0047F84C (name after 3dbrew)
    nn::Result RestoreVramSysArea(); // 0x0013159C (name after 3dbrew)
    nn::Result StoreDataCache(nn::Handle process, unsigned int address, unsigned int size); // 0x00144C58 | fefates:bytes [tier B]

    nn::Handle m_Session; // 0x0
};
ASSERT_SIZE(Gpu, 4);
} // namespace CTR
} // namespace gxlow
} // namespace nn
