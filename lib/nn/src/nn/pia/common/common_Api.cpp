#include "nn/pia/common/common_Api.h"
#include "nn/pia/common/common_CachedPrint.h"
#include "nn/pia/common/common_HeapManager.h"
#include "nn/pia/common/common_Log.h"
#include "nn/pia/common/common_Md5Context.h"
#include "nn/pia/common/common_PayloadSizeManager.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_Scheduler.h"
#include "nn/pia/common/common_SessionBeginMonitoringContent.h"
#include "nn/pia/common/common_SessionStateMonitoringContent.h"
#include "nn/pia/common/common_SignatureManager.h"
#include "nn/pia/common/common_Trace.h"
#include "nn/pia/common/common_WatermarkManager.h"
#include "pead/peadPrintConfig.h"
#include <new>

namespace {
typedef void (*PrintCallback)(const char* str, s32 length);

// 0x00975A28
PrintCallback s_PrintCallback;

// the output of the text of pia: to the print callback of the application if there is one
// (the class is from the RTTI: (anonymous namespace)::Delegate of common_Api.cpp)
class Delegate : public pead::IDelegate1<const pead::PrintConfig::PrintEventArg&>
{
public:
    virtual void invoke(const pead::PrintConfig::PrintEventArg& arg); // 0x004DD2E4 slot 0x00
};

// 0x004DD2E4 (name of the slot is ours)
void Delegate::invoke(const pead::PrintConfig::PrintEventArg& arg)
{
    if (s_PrintCallback != nullptr) {
        s_PrintCallback(arg.mString, arg.mLength);
    } else {
        pead::PrintConfig::PrintDefault(arg.mString, arg.mLength);
    }
}
} // namespace

namespace nn {
namespace pia {
namespace common {
namespace {
// 0x00975A20
bool s_IsInitialized;
// 0x00975A21
bool s_IsInSetupMode;
// 0x00975A22
bool s_IsFirstInitialize = true;
// 0x00975A24
Log* s_pLog;
// the memory of the log
// 0x00AE0878
u64 s_LogBuffer[(sizeof(Log) + 7) / 8];
} // namespace

// the print callback of the application (not in the binary: armlink removed it as unused; the
// name is ours)
void SetPrintCallback(void (*callback)(const char* str, s32 length))
{
    s_PrintCallback = callback;
}

// 0x00425F88 | fefates:bytes [tier B]
nn::Result BeginSetup()
{
    if (!s_IsInitialized) {
        return RESULT_NOT_INITIALIZED;
    }
    if (s_IsInSetupMode) {
        return RESULT_INVALID_STATE;
    }
    HeapManager::SetCurrentHeap(MODULE_TYPE_COMMON);
    s_IsInSetupMode = true;
    PayloadSizeManager::CreateInstance();
    SignatureManager::CreateInstance();
    return nn::Result();
}

// 0x00425FDC (name is ours)
nn::Result Initialize(void* pMemory, unsigned int size)
{
    if (!IsValidPointer(pMemory) || size == 0 || (reinterpret_cast<uptr>(pMemory) & 3) != 0) {
        return RESULT_INVALID_ARGUMENT;
    }
    if (s_IsInitialized) {
        return RESULT_ALREADY_INITIALIZED;
    }
    SessionBeginMonitoringContent& beginContent = g_SessionBeginMonitoringContent;
    if (s_IsFirstInitialize) {
        beginContent.Initialize();
        g_SessionStateMonitoringContent.Initialize();
        s_IsFirstInitialize = false;
    } else {
        // these values survive a new initialization
        u8 value0x16E = beginContent.m_Unknown0x16E;
        u8 value0x2C = beginContent.m_NodeCountMax;
        u8 value0x38 = beginContent.m_SendOption;
        u8 value0x39 = beginContent.m_ReceiveOption;
        u8 value0x16C = beginContent.m_Unknown0x16C;
        u32 value0x28 = beginContent.m_LocalCommunicationId;
        u32 value0x30 = beginContent.m_ReceiveBufferSize;
        u32 value0x34 = beginContent.m_ScanBufferSize;
        u8 value0x16D = beginContent.m_Unknown0x16D;
        beginContent.Initialize();
        g_SessionStateMonitoringContent.Initialize();
        beginContent.m_LocalCommunicationId = value0x28;
        beginContent.m_NodeCountMax = value0x2C;
        beginContent.m_ReceiveBufferSize = value0x30;
        beginContent.m_ScanBufferSize = value0x34;
        beginContent.m_SendOption = value0x38;
        beginContent.m_ReceiveOption = value0x39;
        beginContent.m_Unknown0x16C = value0x16C;
        beginContent.m_Unknown0x16D = value0x16D;
        beginContent.m_Unknown0x16E = value0x16E;
    }
    HeapManager::Initialize(pMemory, size);
    beginContent.m_CommonHeapSize = size;
    HeapManager::Setup(MODULE_TYPE_COMMON, 0, pead::SafeStringBase<char>("pia common heap"));
    s_pLog = new (s_LogBuffer) Log();
    Log::s_pInstance = s_pLog;
    // 0x00975A30 (guard 0x00975A2C)
    static ::Delegate s_Delegate;
    pead::PrintConfig::SetPrintDelegate(&s_Delegate);
    s_IsInitialized = true;
    return nn::Result();
}

// 0x00426C68 | fefates:bytes [tier B]
u32 hashWithMd5(unsigned int value)
{
    u8 data[sizeof(u32)] = {};
    serializeU32(data, value);
    Md5Context md5;
    md5.Initialize();
    md5.Update(data, sizeof(data));
    u8 hash[Md5Context::HASH_SIZE];
    md5.GetHash(hash);
    return deserializeU32(hash);
}

// 0x00426CF4 | fefates:callgraph [tier C]
void serializeU16(unsigned char* pBuffer, unsigned short value)
{
    pBuffer[0] = value >> 8;
    pBuffer[1] = value;
}

// 0x00426D04 | fefates:bytes [tier B]
void serializeU32(unsigned char* pBuffer, unsigned int value)
{
    pBuffer[0] = value >> 24;
    pBuffer[1] = value >> 16;
    pBuffer[2] = value >> 8;
    pBuffer[3] = value;
}

// 0x00426D24 | fefates:bytes [tier B]
void serializeU64(unsigned char* pBuffer, unsigned long long value)
{
    pBuffer[0] = value >> 56;
    pBuffer[1] = value >> 48;
    pBuffer[2] = value >> 40;
    pBuffer[3] = value >> 32;
    pBuffer[4] = value >> 24;
    pBuffer[5] = value >> 16;
    pBuffer[6] = value >> 8;
    pBuffer[7] = value;
}

// 0x00426D90 | fefates:callgraph [tier C]
bool IsInSetupMode()
{
    return s_IsInSetupMode;
}

// 0x00426DA0 | fefates:callgraph [tier C]
bool IsInitialized()
{
    return s_IsInitialized;
}

// 0x004272BC | fefates:callgraph [tier C]
u16 deserializeU16(const unsigned char* pBuffer)
{
    return (pBuffer[0] << 8) | pBuffer[1];
}

// 0x004272C8 | fefates:bytes [tier B]
u32 deserializeU32(const unsigned char* pBuffer)
{
    return (pBuffer[0] << 24) | (pBuffer[1] << 16) | (pBuffer[2] << 8) | pBuffer[3];
}

// 0x004272E0 | fefates:bytes [tier B]
u64 deserializeU64(const unsigned char* pBuffer)
{
    return (static_cast<u64>(pBuffer[0]) << 56) | (static_cast<u64>(pBuffer[1]) << 48) |
           (static_cast<u64>(pBuffer[2]) << 40) | (static_cast<u64>(pBuffer[3]) << 32) |
           (static_cast<u64>(pBuffer[4]) << 24) | (static_cast<u64>(pBuffer[5]) << 16) |
           (static_cast<u64>(pBuffer[6]) << 8) | pBuffer[7];
}

// 0x004283DC | fefates:bytes [tier B]
bool isValidSourceStationIndex(nn::pia::StationIndex index)
{
    return index <= STATION_INDEX_MAX || index == STATION_INDEX_UNIDENTIFIED;
}

// 0x00429438 (name is ours)
nn::Result EndSetup()
{
    if (!s_IsInitialized) {
        return RESULT_NOT_INITIALIZED;
    }
    if (!s_IsInSetupMode) {
        return RESULT_INVALID_STATE;
    }
    HeapManager::GetHeap()->adjust();
    HeapManager::ClearCurrentHeap();
    s_IsInSetupMode = false;
    return nn::Result();
}

// 0x00429490 | fefates:bytes [tier B]
void Finalize()
{
    if (!s_IsInitialized) {
        return;
    }
    if (s_IsInSetupMode) {
        HeapManager::GetHeap()->adjust();
        HeapManager::ClearCurrentHeap();
        s_IsInSetupMode = false;
    }
    pead::PrintConfig::SetPrintDelegate(nullptr);
    Scheduler::DestroyInstance();
    WatermarkManager::DestroyInstance();
    Trace::DestroyInstance();
    CachedPrint::DestroyInstance();
    SignatureManager::DestroyInstance();
    PayloadSizeManager::DestroyInstance();
    if (Log::s_pInstance == s_pLog) {
        Log::s_pInstance = nullptr;
    }
    s_pLog->~Log();
    s_pLog = nullptr;
    HeapManager::Cleanup(MODULE_TYPE_COMMON);
    HeapManager::Finalize();
    s_IsInitialized = false;
}

} // namespace common
} // namespace pia
} // namespace nn
