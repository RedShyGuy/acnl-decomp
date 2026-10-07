#include "nn/pia/transport/transport_ProtocolMessageFilteringManager.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/transport/transport_Api.h"

namespace nn {
namespace pia {
namespace transport {
// 0x0097E45C
nn::pia::transport::ProtocolMessageFilteringManager* nn::pia::transport::ProtocolMessageFilteringManager::s_pInstance;

// 0x0045F1C0 | fefates:bytes [tier B]
nn::Result nn::pia::transport::ProtocolMessageFilteringManager::CreateInstance()
{
    if (!IsInitialized()) {
        return common::RESULT_NOT_INITIALIZED;
    }
    if (!IsInSetupMode()) {
        return common::RESULT_INVALID_STATE;
    }
    if (s_pInstance != nullptr) {
        return common::RESULT_ALREADY_EXISTS;
    }
    s_pInstance = new ProtocolMessageFilteringManager();
    return nn::Result();
}

// 0x0045F26C | fefates:callseq [tier C]
void nn::pia::transport::ProtocolMessageFilteringManager::DestroyInstance()
{
    if (s_pInstance != nullptr) {
        delete s_pInstance;
        s_pInstance = nullptr;
    }
}

// 0x0045F294 | fefates:bytes [tier B]
bool nn::pia::transport::ProtocolMessageFilteringManager::AddNoFilteringProtocolType(unsigned short protocolType)
{
    if (m_ProtocolTypeNum >= PROTOCOL_TYPE_NUM_MAX) {
        return false;
    }
    m_ProtocolTypes[m_ProtocolTypeNum++] = protocolType;
    return true;
}

// 0x0045F2BC | fefates:bytes [tier B]
bool nn::pia::transport::ProtocolMessageFilteringManager::RemoveNoFilteringProtocolType(unsigned short protocolType)
{
    if (m_ProtocolTypeNum == 0) {
        return false;
    }
    u32 i;
    for (i = 0; i < m_ProtocolTypeNum; i++) {
        if (m_ProtocolTypes[i] == protocolType) {
            break;
        }
    }
    if (i == m_ProtocolTypeNum) {
        return false;
    }
    for (; i < m_ProtocolTypeNum - 1; i++) {
        m_ProtocolTypes[i] = m_ProtocolTypes[i + 1];
    }
    m_ProtocolTypeNum--;
    return true;
}

// 0x00736D10 | fefates:bytes [tier B]
bool nn::pia::transport::ProtocolMessageFilteringManager::IsFilteringEnabled(unsigned short protocolType) const
{
    for (u32 i = 0; i < m_ProtocolTypeNum; i++) {
        if (m_ProtocolTypes[i] == protocolType) {
            return false;
        }
    }
    return true;
}

} // namespace transport
} // namespace pia
} // namespace nn
