#include "nn/dsp/CTR/dsp_DSP.h"
#include "nn/os/os_ThreadLocalRegion.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace dsp {
namespace CTR {
namespace {
// command headers (3dbrew "DSP Services")
const bit32 COMMAND_RECV_DATA = 0x00010040;
const bit32 COMMAND_RECV_DATA_IS_READY = 0x00020040;
const bit32 COMMAND_SET_SEMAPHORE = 0x00070040;
const bit32 COMMAND_CONVERT_PROCESS_ADDRESS_FROM_DSP_DRAM = 0x000C0040;
const bit32 COMMAND_WRITE_PROCESS_PIPE = 0x000D0082;
const bit32 COMMAND_READ_PIPE_IF_POSSIBLE = 0x001000C0;
const bit32 COMMAND_LOAD_COMPONENT = 0x001100C2;
const bit32 COMMAND_UNLOAD_COMPONENT = 0x00120000;
const bit32 COMMAND_FLUSH_DATA_CACHE = 0x00130082;
const bit32 COMMAND_REGISTER_INTERRUPT_EVENTS = 0x00150082;
const bit32 COMMAND_GET_SEMAPHORE_EVENT_HANDLE = 0x00160000;
const bit32 COMMAND_SET_SEMAPHORE_MASK = 0x00170040;
const bit32 COMMAND_FORCE_HEADPHONE_OUT = 0x00200040;

// the translation descriptors (3dbrew "IPC")
const bit32 DESCRIPTOR_COPY_HANDLE = 0;

inline bit32 StaticBufferDescriptor(size_t size, s32 index)
{
    return (size << 14) | (index << 10) | 2;
}

inline bit32 ReadBufferDescriptor(size_t size)
{
    return (size << 4) | 0xA;
}

// the arguments go into the low byte / half word of their word
inline void SetByte(bit32* word, u8 value)
{
    *reinterpret_cast<u8*>(word) = value;
}

inline void SetHalf(bit32* word, u16 value)
{
    *reinterpret_cast<u16*>(word) = value;
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

// 0x00130B28 | fefates:bytes [tier B]
nn::Result nn::dsp::CTR::DSP::ForceHeadphoneOut(bool isForced)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_FORCE_HEADPHONE_OUT;
    SetByte(&command[1], isForced);
    return Send(m_Session, command);
}

// 0x00136864 | nintendogs:bytes [tier A]
nn::Result nn::dsp::CTR::DSP::LoadComponent(const unsigned char* component, unsigned size, unsigned short programMask, unsigned short dataMask,
                                            bool* pIsLoaded)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_LOAD_COMPONENT;
    command[1] = size;
    SetHalf(&command[2], programMask);
    SetHalf(&command[3], dataMask);
    command[4] = ReadBufferDescriptor(size);
    command[5] = reinterpret_cast<uptr>(component);
    nn::Result result = nn::svc::SendSyncRequest(m_Session);
    if (result.IsFailure()) {
        return result;
    }
    *pIsLoaded = *reinterpret_cast<bool*>(&command[2]);
    return nn::Result(command[1]);
}

// 0x001368C4 | nintendogs:bytes [tier A]
nn::Result nn::dsp::CTR::DSP::UnloadComponent()
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_UNLOAD_COMPONENT;
    return Send(m_Session, command);
}

// 0x001436D0 | nintendogs:bytes [tier A]
nn::Result nn::dsp::CTR::DSP::FlushDataCache(nn::Handle process, unsigned address, unsigned size)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_FLUSH_DATA_CACHE;
    command[1] = address;
    command[2] = size;
    command[3] = DESCRIPTOR_COPY_HANDLE;
    command[4] = process.GetPrintableBits();
    return Send(m_Session, command);
}

// 0x003517E8 | nintendogs:bytes [tier A]
nn::Result nn::dsp::CTR::DSP::SetSemaphore(unsigned short value)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SET_SEMAPHORE;
    SetHalf(&command[1], value);
    return Send(m_Session, command);
}

// 0x00351820 | nintendogs:bytes [tier A]
nn::Result nn::dsp::CTR::DSP::RecvDataIsReady(unsigned short registerNumber, bool* pIsReady)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_RECV_DATA_IS_READY;
    SetHalf(&command[1], registerNumber);
    nn::Result result = nn::svc::SendSyncRequest(m_Session);
    if (result.IsFailure()) {
        return result;
    }
    *pIsReady = *reinterpret_cast<bool*>(&command[2]);
    return nn::Result(command[1]);
}

// 0x00351864 | nintendogs:bytes [tier A]
nn::Result nn::dsp::CTR::DSP::SetSemaphoreMask(unsigned short mask)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SET_SEMAPHORE_MASK;
    SetHalf(&command[1], mask);
    return Send(m_Session, command);
}

// 0x0035189C | nintendogs:bytes [tier A]
nn::Result nn::dsp::CTR::DSP::WriteProcessPipe(int channel, const unsigned char* buffer, unsigned size)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_WRITE_PROCESS_PIPE;
    command[1] = channel;
    command[2] = size;
    command[3] = StaticBufferDescriptor(size, 1);
    command[4] = reinterpret_cast<uptr>(buffer);
    return Send(m_Session, command);
}

// 0x003518E4 | nintendogs:bytes [tier A]
nn::Result nn::dsp::CTR::DSP::ReadPipeIfPossible(int channel, int peer, unsigned char* buffer, unsigned short size, unsigned short* pReadSize)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_READ_PIPE_IF_POSSIBLE;
    command[1] = channel;
    command[2] = peer;
    SetHalf(&command[3], size);
    bit32* staticBuffers = nn::os::detail::GetIpcStaticBuffers();
    bit32 saved0 = staticBuffers[0];
    bit32 saved1 = staticBuffers[1];
    staticBuffers[0] = StaticBufferDescriptor(size, 0);
    staticBuffers[1] = reinterpret_cast<uptr>(buffer);
    nn::Result result = nn::svc::SendSyncRequest(m_Session);
    staticBuffers[0] = saved0;
    staticBuffers[1] = saved1;
    if (result.IsFailure()) {
        return result;
    }
    *pReadSize = *reinterpret_cast<u16*>(&command[2]);
    return nn::Result(command[1]);
}

// 0x0035194C | nintendogs:bytes [tier A]
nn::Result nn::dsp::CTR::DSP::GetSemaphoreEventHandle(nn::Handle* pEvent)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_SEMAPHORE_EVENT_HANDLE;
    nn::Result result = nn::svc::SendSyncRequest(m_Session);
    if (result.IsFailure()) {
        return result;
    }
    *pEvent = nn::Handle(command[3]);
    return nn::Result(command[1]);
}

// 0x00351980 | nintendogs:bytes [tier A]
nn::Result nn::dsp::CTR::DSP::RegisterInterruptEvents(nn::Handle event, int interrupt, int channel)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_REGISTER_INTERRUPT_EVENTS;
    command[1] = interrupt;
    command[2] = channel;
    command[3] = DESCRIPTOR_COPY_HANDLE;
    command[4] = event.GetPrintableBits();
    return Send(m_Session, command);
}

// 0x003519BC | nintendogs:bytes [tier A]
nn::Result nn::dsp::CTR::DSP::ConvertProcessAddressFromDspDram(unsigned address, unsigned* pAddress)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_CONVERT_PROCESS_ADDRESS_FROM_DSP_DRAM;
    command[1] = address;
    nn::Result result = nn::svc::SendSyncRequest(m_Session);
    if (result.IsFailure()) {
        return result;
    }
    *pAddress = command[2];
    return nn::Result(command[1]);
}

// 0x003519F8 | nintendogs:bytes [tier A]
nn::Result nn::dsp::CTR::DSP::RecvData(unsigned short registerNumber, unsigned short* pData)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_RECV_DATA;
    SetHalf(&command[1], registerNumber);
    nn::Result result = nn::svc::SendSyncRequest(m_Session);
    if (result.IsFailure()) {
        return result;
    }
    *pData = *reinterpret_cast<u16*>(&command[2]);
    return nn::Result(command[1]);
}

} // namespace CTR
} // namespace dsp
} // namespace nn
