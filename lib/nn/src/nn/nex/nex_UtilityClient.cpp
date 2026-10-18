#include "nn/nex/nex_UtilityClient.h"

namespace nn {
namespace nex {
// 0x0036F2E4 (name is ours)
bool nn::nex::UtilityClient::GetIntegerSettings(nn::nex::ProtocolCallContext* pContext, unsigned int index, nn::nex::qMap<u16, u32>* pSettings)
{
    return m_Protocol.CallGetIntegerSettings(pContext, index, pSettings);
}

// 0x0036F2FC slot 0x0C
bool nn::nex::UtilityClient::Bind(nn::nex::Credentials* pCredentials)
{
    if (!ServiceClient::Bind(pCredentials)) {
        return false;
    }
    if (!RegisterProtocol(&m_Protocol)) {
        return false;
    }
    m_IsProtocolRegistered = true;
    return true;
}

// 0x0036F330 slot 0x10
bool nn::nex::UtilityClient::Unbind()
{
    if (!ServiceClient::Unbind()) {
        return false;
    }
    if (!UnregisterProtocol(&m_Protocol)) {
        return false;
    }
    m_IsProtocolRegistered = false;
    return true;
}

// 0x0036F368
nn::nex::UtilityClient::UtilityClient() : ServiceClient(PROTOCOL_ID), m_Protocol(0)
{
    m_Protocol.m_ProtocolId = PROTOCOL_ID;
    m_IsProtocolRegistered = false;
}

// 0x0036F41C slot 0x00
// 0x0036F3B8 (deleting dtor)
nn::nex::UtilityClient::~UtilityClient()
{
    if (m_IsProtocolRegistered) {
        UtilityClient::Unbind();
    }
}

// 0x00370DCC slot 0x20
void nn::nex::UtilityClient::UpdateProtocolsDefaultCredentials(nn::nex::Credentials* pCredentials)
{
    m_Protocol.SetDefaultCredentials(pCredentials);
}

} // namespace nex
} // namespace nn
