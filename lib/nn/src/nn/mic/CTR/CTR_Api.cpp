#include "nn/mic/CTR/CTR_Api.h"
#include <string.h>
#include "nn/dbg/dbg_Api.h"
#include "nn/mic/CTR/detail/mic_Mic.h"
#include "nn/os/os_TransferMemoryBlock.h"
#include "nn/srv/srv_Api.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace mic {
namespace CTR {
namespace {
// module 35 mic (names are ours, after the level, summary and description of 3dbrew)
// status, invalid state, already initialized / not initialized
const bit32 RESULT_ALREADY_INITIALIZED = 0xD8208FF9;
const bit32 RESULT_NOT_INITIALIZED = 0xD8208FF8;
// status, invalid state, the service is used by another process
const bit32 RESULT_BUSY = 0xC8A08FF9;
// usage, invalid argument, misaligned size / address
const bit32 RESULT_MISALIGNED_SIZE = 0xE0E08FF2;
const bit32 RESULT_MISALIGNED_ADDRESS = 0xE0E08FF1;
// status, invalid state, sampling
const bit32 RESULT_SAMPLING = 0xC8208FF0;
// usage, out of range
const bit32 RESULT_OUT_OF_RANGE = 0xE1008FF3;
// what srv returns when another process has the session
const bit32 RESULT_SRV_SESSION_BUSY = 0xD0401834;
const bit32 DESCRIPTION_ALREADY_INITIALIZED = 1017;

const char SERVICE_NAME[] = "mic:u";
// the version this library reports (value from the binary)
const u32 CLIENT_VERSION = 0x0B0500C8;
// the buffer is shared in pages; its last word is the position of the newest sample
const uptr PAGE_MASK = 0xFFF;
const size_t POSITION_SIZE = sizeof(u32);
// the permissions of the transfer memory (read and write)
const u32 TRANSFER_PERMISSION = 3;
// the size of an IIR filter of a sample rate
const size_t IIR_FILTER_SIZE = 50;
} // namespace

// the state of the library (names are ours)
// 0x00975F50
bool s_IsInitialized;
// 0x00975F51
bool s_IsBufferSet;
// (never set in this program)
// 0x00975F52
bool s_IsIirFilterEnabled;
// 0x00975F53
SamplingRate s_SamplingRate;
// 0x00975F54
size_t s_SamplingBufferSize;
// 0x00AE1F18
nn::os::TransferMemoryBlock s_TransferMemory;
// the IIR filters of the sample rates, in .rodata, not in the source (name is ours)
// 0x008A33D6
extern const u8 s_IirFilters[];

// 0x0012A348 | nintendogs:callseq [tier A]
nn::Result Initialize()
{
    if (s_IsInitialized) {
        return nn::Result(RESULT_ALREADY_INITIALIZED);
    }
    nn::Result result = nn::srv::Initialize();
    if (result.GetDescription() != DESCRIPTION_ALREADY_INITIALIZED && result.IsFailure()) {
        nndbgPanic();
    }
    result = nn::srv::GetServiceHandle(&detail::s_Session, SERVICE_NAME, strlen(SERVICE_NAME), 1);
    if (result.IsFailure()) {
        if (result == nn::Result(RESULT_SRV_SESSION_BUSY)) {
            return nn::Result(RESULT_BUSY);
        }
        nndbgPanic();
    }
    detail::Mic::SetClientVersion(CLIENT_VERSION);
    s_IsInitialized = true;
    return result;
}

// 0x0012A3F4 | nintendogs:bytes [tier B]
nn::Result GetSamplingBufferSize(size_t* pSize)
{
    if (!s_IsBufferSet) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    *pSize = s_SamplingBufferSize;
    return nn::Result();
}

// 0x0012A41C | nintendogs:bytes [tier A]
nn::Result SetBuffer(void* pBuffer, size_t size)
{
    if (s_IsBufferSet) {
        return nn::Result(RESULT_ALREADY_INITIALIZED);
    }
    if ((size & PAGE_MASK) != 0) {
        return nn::Result(RESULT_MISALIGNED_SIZE);
    }
    if ((reinterpret_cast<uptr>(pBuffer) & PAGE_MASK) != 0) {
        return nn::Result(RESULT_MISALIGNED_ADDRESS);
    }
    s_TransferMemory.Initialize(pBuffer, size, TRANSFER_PERMISSION, TRANSFER_PERMISSION);
    nn::Result result = detail::Mic::MapSharedMem(s_TransferMemory.GetHandle(), size);
    s_SamplingBufferSize = size - POSITION_SIZE;
    if (result.IsSuccess()) {
        s_IsBufferSet = true;
    }
    return result;
}

// 0x00140A88 | nintendogs:bytes [tier A]
nn::Result ResetBuffer()
{
    if (!s_IsBufferSet) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    bool isSampling;
    nn::Result result = detail::Mic::IsSampling(&isSampling);
    if (result.IsFailure()) {
        return result;
    }
    if (isSampling) {
        return nn::Result(RESULT_SAMPLING);
    }
    result = detail::Mic::FreeBuffer();
    s_TransferMemory.Finalize();
    s_IsBufferSet = false;
    return result;
}

// 0x00140BD0 | nintendogs:bytes [tier A]
nn::Result Finalize()
{
    if (!s_IsInitialized) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    detail::Mic::StopSampling();
    if (s_IsBufferSet) {
        bool isSampling;
        if (detail::Mic::IsSampling(&isSampling).IsSuccess() && !isSampling) {
            detail::Mic::FreeBuffer();
            s_TransferMemory.Finalize();
            s_IsBufferSet = false;
        }
    }
    s_IsInitialized = false;
    return nn::svc::CloseHandle(detail::s_Session);
}

// 0x0035486C | nintendogs:callseq [tier A]
nn::Result StartSampling(SamplingType type, SamplingRate rate, s32 offset, size_t size, bool loop)
{
    if (!s_IsBufferSet) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    if (((size | offset) & 1) != 0) {
        return nn::Result(RESULT_MISALIGNED_SIZE);
    }
    if (offset + size > s_SamplingBufferSize) {
        return nn::Result(RESULT_OUT_OF_RANGE);
    }
    if (s_IsIirFilterEnabled) {
        nn::Result result = detail::Mic::SetIirFilterMic(&s_IirFilters[rate * IIR_FILTER_SIZE], IIR_FILTER_SIZE);
        if (result.IsFailure()) {
            return result;
        }
    }
    nn::Result result = detail::Mic::StartSampling(type, rate, offset, size, loop);
    if (result.IsSuccess()) {
        s_SamplingRate = rate;
    }
    return result;
}

// 0x00354924 (name after the command)
nn::Result AdjustSampling(SamplingRate rate)
{
    nn::Result result = detail::Mic::AdjustSampling(rate);
    if (result.IsFailure()) {
        return result;
    }
    s_SamplingRate = rate;
    if (s_IsIirFilterEnabled) {
        return detail::Mic::SetIirFilterMic(&s_IirFilters[rate * IIR_FILTER_SIZE], IIR_FILTER_SIZE);
    }
    return result;
}

// 0x00354974 | nintendogs:bytes [tier B]
nn::Result GetLastSamplingAddress(uptr* pAddress)
{
    if (!s_IsBufferSet) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    uptr address = s_TransferMemory.GetAddress();
    *pAddress = *reinterpret_cast<const u32*>(address + s_SamplingBufferSize) + address;
    return nn::Result();
}

} // namespace CTR
} // namespace mic
} // namespace nn
