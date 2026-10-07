#include "nn/pia/local/local_LocalUpdateSessionMessage.h"
#include "nn/pia/local/local_LocalNetwork.h"
#include "nn/pia/local/local_LocalNetworkManager.h"
#include "pead/peadHashCrc16.h"
#include <cstring>

namespace nn {
namespace pia {
namespace local {
// 0x00420A50 | fefates:bytes
bool nn::pia::local::LocalUpdateSessionMessage::ParseMessageHeader()
{
    Header* pHeader = reinterpret_cast<Header*>(m_pBuffer);
    if (pHeader->m_Checksum != pead::HashCrc16::calcHash(pHeader, CHECKSUM_SIZE)) {
        return false;
    }
    m_Type = pHeader->m_Type;
    m_DataSize = pHeader->m_DataSize;
    UpdateSessionHeader* pUpdateSessionHeader = reinterpret_cast<UpdateSessionHeader*>(m_pBuffer);
    m_Version = pUpdateSessionHeader->m_Version;
    m_Unknown0x18 = pUpdateSessionHeader->m_Unknown0xC;
    m_ParticipationState = pUpdateSessionHeader->m_ParticipationState;
    return true;
}

// 0x00420AAC | fefates:callseq
void nn::pia::local::LocalUpdateSessionMessage::UpdateMessageHeader()
{
    Header* pHeader = reinterpret_cast<Header*>(m_pBuffer);
    pHeader->m_Version = VERSION;
    pHeader->m_Type = m_Type;
    pHeader->m_DataSize = m_DataSize;
    std::memset(pHeader->m_Reserved, 0, sizeof(pHeader->m_Reserved));
    pHeader->m_Checksum = pead::HashCrc16::calcHash(m_pBuffer, CHECKSUM_SIZE);
    UpdateSessionHeader* pUpdateSessionHeader = reinterpret_cast<UpdateSessionHeader*>(m_pBuffer);
    pUpdateSessionHeader->m_Version = m_Version;
    pUpdateSessionHeader->m_Unknown0xC = LocalNetwork::s_pInstance->m_pNetworkManager->m_Unknown0x123C;
    pUpdateSessionHeader->m_ParticipationState = LocalNetwork::s_pInstance->GetParticipationState();
    std::memset(pUpdateSessionHeader->m_Reserved, 0, sizeof(pUpdateSessionHeader->m_Reserved));
}

// 0x00420B28
nn::pia::local::LocalUpdateSessionMessage::LocalUpdateSessionMessage(u8* pBuffer, u32 bufferSize, u32 version) : LocalMessage(pBuffer, bufferSize)
{
    m_Type = LocalNetworkManager::MESSAGE_TYPE_UPDATE_SESSION;
    m_HeaderSize = UPDATE_SESSION_HEADER_SIZE;
    m_Version = version;
}

// 0x00420B5C
// 0x00420B58 (deleting dtor)
nn::pia::local::LocalUpdateSessionMessage::~LocalUpdateSessionMessage()
{
    // empty (in the original too)
}

} // namespace local
} // namespace pia
} // namespace nn
