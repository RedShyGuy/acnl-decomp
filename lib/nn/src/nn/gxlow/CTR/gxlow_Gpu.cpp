#include "nn/gxlow/CTR/gxlow_Gpu.h"
#include "nn/gxlow/CTR/CTR_Api.h"
#include "nn/os/os_ThreadLocalRegion.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace gxlow {
namespace CTR {
namespace {
// command headers (3dbrew "GSPGPU")
const bit32 COMMAND_WRITE_HW_REGS = 0x00010082;
const bit32 COMMAND_WRITE_HW_REGS_WITH_MASK = 0x00020084;
const bit32 COMMAND_READ_HW_REGS = 0x00040080;
const bit32 COMMAND_FLUSH_DATA_CACHE = 0x00080082;
const bit32 COMMAND_SET_LCD_FORCE_BLACK = 0x000B0040;
const bit32 COMMAND_TRIGGER_CMD_REQ_QUEUE = 0x000C0000;
const bit32 COMMAND_REGISTER_INTERRUPT_RELAY_QUEUE = 0x00130042;
const bit32 COMMAND_ACQUIRE_RIGHT = 0x00160042;
const bit32 COMMAND_RELEASE_RIGHT = 0x00170000;
const bit32 COMMAND_IMPORT_DISPLAY_CAPTURE_INFO = 0x00180000;
const bit32 COMMAND_SAVE_VRAM_SYS_AREA = 0x00190000;
const bit32 COMMAND_RESTORE_VRAM_SYS_AREA = 0x001A0000;
const bit32 COMMAND_STORE_DATA_CACHE = 0x001F0082;

// the translation descriptors (3dbrew "IPC")
const bit32 DESCRIPTOR_COPY_HANDLE = 0;

inline bit32 StaticBufferDescriptor(size_t size, s32 index)
{
    return (size << 14) | (index << 10) | 2;
}

inline void SetByte(bit32* word, u8 value)
{
    *reinterpret_cast<u8*>(word) = value;
}

inline nn::Result Send(nn::Handle session, bit32* command)
{
    nn::Result result = nn::svc::SendSyncRequest(session);
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result(command[1]);
}
} // namespace

// 0x001314B0 | nintendogs:bytes [tier A]
nn::Result nn::gxlow::CTR::Gpu::WriteHWRegs(unsigned regAddress, const unsigned char* data, unsigned size)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_WRITE_HW_REGS;
    command[1] = regAddress;
    command[2] = size;
    command[3] = StaticBufferDescriptor(size, 0);
    command[4] = reinterpret_cast<uptr>(data);
    return Send(m_Session, command);
}

// 0x001314F8 | nintendogs:bytes [tier A]
nn::Result nn::gxlow::CTR::Gpu::AcquireRight(nn::Handle process, bool flags)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_ACQUIRE_RIGHT;
    SetByte(&command[1], flags);
    command[3] = process.GetPrintableBits();
    command[2] = DESCRIPTOR_COPY_HANDLE;
    return Send(m_Session, command);
}

// 0x0013153C | nintendogs:bytes [tier A]
nn::Result nn::gxlow::CTR::Gpu::ReleaseRight()
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_RELEASE_RIGHT;
    return Send(m_Session, command);
}

// 0x00131564 | nintendogs:bytes [tier A]
nn::Result nn::gxlow::CTR::Gpu::SetLcdForceBlack(bool isBlack)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SET_LCD_FORCE_BLACK;
    SetByte(&command[1], isBlack);
    return Send(m_Session, command);
}

// 0x0013159C (name after 3dbrew)
nn::Result nn::gxlow::CTR::Gpu::RestoreVramSysArea()
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_RESTORE_VRAM_SYS_AREA;
    return Send(m_Session, command);
}

// 0x001315C4 | nintendogs:bytes [tier A]
nn::Result nn::gxlow::CTR::Gpu::WriteHWRegsWithMask(unsigned regAddress, const unsigned char* data, const unsigned char* mask, unsigned size)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_WRITE_HW_REGS_WITH_MASK;
    command[1] = regAddress;
    command[2] = size;
    command[3] = StaticBufferDescriptor(size, 0);
    command[4] = reinterpret_cast<uptr>(data);
    command[5] = StaticBufferDescriptor(size, 1);
    command[6] = reinterpret_cast<uptr>(mask);
    return Send(m_Session, command);
}

// 0x00137218 | nintendogs:bytes [tier A]
nn::Result nn::gxlow::CTR::Gpu::ReadHWRegs(unsigned regAddress, unsigned char* data, unsigned size)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_READ_HW_REGS;
    command[1] = regAddress;
    command[2] = size;
    bit32* staticBuffers = nn::os::detail::GetIpcStaticBuffers();
    bit32 saved0 = staticBuffers[0];
    bit32 saved1 = staticBuffers[1];
    staticBuffers[0] = StaticBufferDescriptor(size, 0);
    staticBuffers[1] = reinterpret_cast<uptr>(data);
    nn::Result result = nn::svc::SendSyncRequest(m_Session);
    staticBuffers[0] = saved0;
    staticBuffers[1] = saved1;
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result(command[1]);
}

// 0x0013726C | nintendogs:bytes [tier A]
nn::Result nn::gxlow::CTR::Gpu::RegisterInterruptRelayQueue(nn::Handle event, unsigned flags, nn::Handle* pSharedMemory, int* pThreadIndex)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_REGISTER_INTERRUPT_RELAY_QUEUE;
    command[3] = event.GetPrintableBits();
    command[1] = flags;
    command[2] = DESCRIPTOR_COPY_HANDLE;
    nn::Result result = nn::svc::SendSyncRequest(m_Session);
    if (result.IsFailure()) {
        return result;
    }
    *pThreadIndex = command[2];
    *pSharedMemory = nn::Handle(command[4]);
    return nn::Result(command[1]);
}

// 0x0013B6B8 | nintendogs:bytes [tier A]
nn::Result nn::gxlow::CTR::Gpu::FlushDataCache(nn::Handle process, unsigned address, unsigned size)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_FLUSH_DATA_CACHE;
    command[1] = address;
    command[2] = size;
    command[3] = DESCRIPTOR_COPY_HANDLE;
    command[4] = process.GetPrintableBits();
    return Send(m_Session, command);
}

// 0x0013EE58 | nintendogs:bytes [tier A]
nn::Result nn::gxlow::CTR::Gpu::TriggerCmdReqQueue()
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_TRIGGER_CMD_REQ_QUEUE;
    return Send(m_Session, command);
}

// 0x00144C58 | fefates:bytes [tier B]
nn::Result nn::gxlow::CTR::Gpu::StoreDataCache(nn::Handle process, unsigned int address, unsigned int size)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_STORE_DATA_CACHE;
    command[1] = address;
    command[2] = size;
    command[3] = DESCRIPTOR_COPY_HANDLE;
    command[4] = process.GetPrintableBits();
    return Send(m_Session, command);
}

// 0x0047F84C (name after 3dbrew)
nn::Result nn::gxlow::CTR::Gpu::SaveVramSysArea()
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SAVE_VRAM_SYS_AREA;
    return Send(m_Session, command);
}

// 0x0047F874 | nintendogs:bytes [tier A]
nn::Result nn::gxlow::CTR::Gpu::ImportDisplayCaptureInfo(nn::gxlow::CTR::DisplayCaptureInfo* pInfo)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_IMPORT_DISPLAY_CAPTURE_INFO;
    nn::Result result = nn::svc::SendSyncRequest(m_Session);
    if (result.IsFailure()) {
        return result;
    }
    *pInfo = *reinterpret_cast<DisplayCaptureInfo*>(&command[2]);
    return nn::Result(command[1]);
}

} // namespace CTR
} // namespace gxlow
} // namespace nn
