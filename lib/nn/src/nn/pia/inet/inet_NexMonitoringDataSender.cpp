#include "nn/pia/inet/inet_NexMonitoringDataSender.h"
#include "nn/nex/nex_NgsBridgeInterface.h"
#include "nn/pia/common/common_Api.h"
#include "nn/pia/common/common_Crypto.h"
#include "nn/pia/common/common_NewArray.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_SessionBeginMonitoringData.h"
#include "nn/pia/common/common_SessionEndMonitoringData.h"
#include "nn/pia/common/common_ZlibCompressor.h"
#include "nn/pia/inet/inet_NexFacade.h"
#include <string.h>

namespace nn {
namespace pia {
namespace inet {
namespace {
// the header of the blocks stays unencrypted
const u32 HEADER_SIZE = 16;
// the most the server takes
const u32 SEND_SIZE_MAX = 1232;
const u32 BUFFER_ALIGNMENT = 32;
// the zlib parameters
const int ZLIB_LEVEL = 9;
const int ZLIB_WINDOW_BITS = 9;
const int ZLIB_MEM_LEVEL = 2;

// the AES key of the monitoring data
// 0x008B12F2
const u8 MONITORING_KEY[16] = {0x90, 0x1E, 0xDF, 0x19, 0x3D, 0xC5, 0xEF, 0x3C, 0x52, 0x90, 0x64, 0x7B, 0xFF, 0x20, 0xC3, 0x85};
} // namespace

// (in .data; name is ours)
// 0x0097E410
common::Crypto::Setting g_MonitoringCryptoSetting = {common::Crypto::MODE_AES128, MONITORING_KEY, sizeof(MONITORING_KEY)};

// 0x00404AC4 slot 0x10
void nn::pia::inet::NexMonitoringDataSender::Send(u8 phase)
{
    SendResult result = sendCore(phase, false);
    if (result == SEND_RESULT_TOO_LARGE) {
        sendCore(phase, true);
    }
}

// 0x00404B00 (name is ours)
nn::pia::inet::NexMonitoringDataSender::SendResult nn::pia::inet::NexMonitoringDataSender::sendCore(u8 phase, bool isShort)
{
    u32 size = 0;
    u32 compressedSize = 0;
    memset(m_pDataBuffer, 0, DATA_BUFFER_SIZE);
    memset(m_pCompressBuffer, 0, COMPRESS_BUFFER_SIZE);
    memset(m_pZlibWorkBuffer, 0, ZLIB_WORK_BUFFER_SIZE);
    nn::Result result;
    u32 headerSize;
    switch (phase) {
    case 0: {
        common::SessionBeginMonitoringData& data = common::g_SessionBeginMonitoringData;
        data.Initialize();
        data.m_BeginContent = common::g_SessionBeginMonitoringContent;
        data.m_Header.m_Unknown0x2 &= ~3;
        result = data.Serialize(m_pDataBuffer, &size, DATA_BUFFER_SIZE);
        headerSize = HEADER_SIZE;
        m_Flag = true;
        break;
    }
    case 1:
    case 2: {
        common::SessionEndMonitoringData& data = common::g_SessionEndMonitoringData;
        data.Initialize(phase);
        // (a trace for the end; armlink removed the calls)
        data.m_BeginContent = common::g_SessionBeginMonitoringContent;
        if (isShort) {
            // without the state
            data.m_StateContent.Initialize();
            data.m_StateContent.m_Header.m_Unknown0x2 &= ~4;
        } else {
            data.m_StateContent = common::g_SessionStateMonitoringContent;
        }
        data.m_Header.m_Unknown0x2 &= ~3;
        result = common::g_SessionEndMonitoringData.Serialize(m_pDataBuffer, &size, DATA_BUFFER_SIZE);
        headerSize = HEADER_SIZE;
        break;
    }
    default:
        return SEND_RESULT_FAILURE;
    }
    if (result.IsFailure() || size == 0 || size <= HEADER_SIZE) {
        return SEND_RESULT_FAILURE;
    }
    // the data after the header is compressed
    common::ZlibCompressor compressor;
    if (compressor.Initialize(m_pZlibWorkBuffer, ZLIB_WORK_BUFFER_SIZE).IsFailure()) {
        return SEND_RESULT_FAILURE;
    }
    if (compressor.Startup(m_pCompressBuffer, COMPRESS_BUFFER_SIZE, ZLIB_LEVEL, ZLIB_WINDOW_BITS, ZLIB_MEM_LEVEL).IsSuccess() &&
        compressor.Deflate(m_pDataBuffer + HEADER_SIZE, size - HEADER_SIZE).IsSuccess()) {
        if (compressor.FinishDeflate(&compressedSize).IsFailure()) {
            compressedSize = 0;
        }
    }
    compressor.Cleanup();
    compressor.Finalize();
    if (compressedSize == 0) {
        return SEND_RESULT_FAILURE;
    }
    // padded to whole AES blocks with the inverted last byte
    u32 paddedSize = (compressedSize >> 4 << 4) + 16;
    if (paddedSize + HEADER_SIZE > SEND_SIZE_MAX) {
        return SEND_RESULT_TOO_LARGE;
    }
    common::serializeU16(m_pDataBuffer + 4, compressedSize);
    u8 padding = ~m_pCompressBuffer[compressedSize - 1];
    for (u32 i = compressedSize; i < paddedSize; i++) {
        m_pCompressBuffer[i] = padding;
    }
    memset(m_pDataBuffer + headerSize, 0, DATA_BUFFER_SIZE - headerSize);
    if (common::Crypto::Encrypt(m_pDataBuffer + headerSize, m_pCompressBuffer, paddedSize, g_MonitoringCryptoSetting).IsFailure()) {
        return SEND_RESULT_FAILURE;
    }
    nex::NgsBridgeInterface* pBridge = NexFacade::s_pInstance->m_pNgsBridge;
    if (common::IsValidPointer(pBridge)) {
        pBridge->SendReport(phase, m_pDataBuffer, paddedSize + headerSize);
    }
    return SEND_RESULT_SUCCESS;
}

// 0x00404E3C
nn::pia::inet::NexMonitoringDataSender::NexMonitoringDataSender()
{
    m_pDataBuffer = common::NewArray<u8>(DATA_BUFFER_SIZE, BUFFER_ALIGNMENT);
    m_pCompressBuffer = common::NewArray<u8>(COMPRESS_BUFFER_SIZE, BUFFER_ALIGNMENT);
    m_pZlibWorkBuffer = common::NewArray<u8>(ZLIB_WORK_BUFFER_SIZE, BUFFER_ALIGNMENT);
}

// 0x00404FB8
// 0x00404F24 (deleting dtor)
nn::pia::inet::NexMonitoringDataSender::~NexMonitoringDataSender()
{
    if (m_pDataBuffer != nullptr) {
        common::DeleteArray(m_pDataBuffer);
        m_pDataBuffer = nullptr;
    }
    if (m_pCompressBuffer != nullptr) {
        common::DeleteArray(m_pCompressBuffer);
        m_pCompressBuffer = nullptr;
    }
    if (m_pZlibWorkBuffer != nullptr) {
        common::DeleteArray(m_pZlibWorkBuffer);
        m_pZlibWorkBuffer = nullptr;
    }
}

// 0x0072F52C slot 0x14
void nn::pia::inet::NexMonitoringDataSender::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace inet
} // namespace pia
} // namespace nn
