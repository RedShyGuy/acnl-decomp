#include "nn/pia/common/common_PayloadSizeManager.h"
#include "nn/pia/common/common_Api.h"
#include "nn/pia/common/common_ChunkOld.h"
#include "nn/pia/common/common_PacketOld.h"
#include "nn/pia/common/common_Result.h"

namespace nn {
namespace pia {
namespace common {
namespace {
// the IP and UDP headers in the MTU, the header of a chunk in the packet
const unsigned int IP_UDP_HEADER_SIZE = 24;
const unsigned int CHUNK_HEADER_SIZE = 14;
} // namespace

// 0x0097E3D0
PayloadSizeManager* PayloadSizeManager::s_pInstance;

// 0x00427FD4 | fefates:callgraph [tier C]
nn::Result nn::pia::common::PayloadSizeManager::CreateInstance()
{
    if (!IsInitialized()) {
        return RESULT_NOT_INITIALIZED;
    }
    if (!IsInSetupMode()) {
        return RESULT_INVALID_STATE;
    }
    if (s_pInstance != nullptr) {
        return RESULT_ALREADY_EXISTS;
    }
    s_pInstance = new PayloadSizeManager();
    return nn::Result();
}

// 0x00428058 | fefates:callgraph [tier C]
void nn::pia::common::PayloadSizeManager::DestroyInstance()
{
    if (s_pInstance != nullptr) {
        delete s_pInstance;
        s_pInstance = nullptr;
    }
}

// 0x00428080 | fefates:callgraph [tier C]
nn::Result nn::pia::common::PayloadSizeManager::SetSignatureSize(unsigned int signatureSize)
{
    return SetSize(m_MtuSize, signatureSize);
}

// 0x0042808C | fefates:callgraph [tier C]
nn::Result nn::pia::common::PayloadSizeManager::SetMtuSize(unsigned int mtuSize)
{
    return SetSize(mtuSize, m_SignatureSize);
}

// 0x00428094 (name is ours)
nn::Result nn::pia::common::PayloadSizeManager::SetSize(unsigned int mtuSize, unsigned int signatureSize)
{
    if (mtuSize < MTU_SIZE_MIN || MTU_SIZE_MAX < mtuSize) {
        return RESULT_INVALID_ARGUMENT;
    }
    unsigned int packetSize = mtuSize - IP_UDP_HEADER_SIZE;
    if (!PacketOld::IsValidDefaultPayloadSize(packetSize)) {
        return RESULT_INVALID_ARGUMENT;
    }
    if (!PacketOld::IsValidSignatureSize(signatureSize)) {
        return RESULT_INVALID_ARGUMENT;
    }
    unsigned int chunkDataSize = packetSize - signatureSize - CHUNK_HEADER_SIZE;
    if (!ChunkOld::IsValidDataSizeLimit(chunkDataSize)) {
        return RESULT_INVALID_ARGUMENT;
    }
    PacketOld::SetDefaultPayloadSize(packetSize);
    PacketOld::SetSignatureSize(signatureSize);
    ChunkOld::SetDataSizeLimit(chunkDataSize);
    m_MtuSize = mtuSize;
    m_SignatureSize = signatureSize;
    return nn::Result();
}

// 0x00731B00 (name after StepSequenceJob::Trace)
void nn::pia::common::PayloadSizeManager::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace common
} // namespace pia
} // namespace nn
