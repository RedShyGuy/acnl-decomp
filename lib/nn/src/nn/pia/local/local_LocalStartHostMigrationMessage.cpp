#include "nn/pia/local/local_LocalStartHostMigrationMessage.h"
#include "nn/pia/local/local_LocalNetworkManager.h"
#include "pead/peadHashCrc16.h"
#include <cstring>

namespace nn {
namespace pia {
namespace local {
// 0x004238C0
bool nn::pia::local::LocalStartHostMigrationMessage::ParseMessageHeader()
{
    Header* pHeader = reinterpret_cast<Header*>(m_pBuffer);
    if (pHeader->m_Checksum != pead::HashCrc16::calcHash(pHeader, CHECKSUM_SIZE)) {
        return false;
    }
    m_Type = pHeader->m_Type;
    m_DataSize = pHeader->m_DataSize;
    return true;
}

// 0x00423900
void nn::pia::local::LocalStartHostMigrationMessage::UpdateMessageHeader()
{
    Header* pHeader = reinterpret_cast<Header*>(m_pBuffer);
    pHeader->m_Version = VERSION;
    pHeader->m_Type = m_Type;
    pHeader->m_DataSize = m_DataSize;
    std::memset(pHeader->m_Reserved, 0, sizeof(pHeader->m_Reserved));
    pHeader->m_Checksum = pead::HashCrc16::calcHash(m_pBuffer, CHECKSUM_SIZE);
    *reinterpret_cast<u32*>(m_pBuffer + HEADER_SIZE) = 0;
}

// 0x0042394C
nn::pia::local::LocalStartHostMigrationMessage::LocalStartHostMigrationMessage(u8* pBuffer, u32 bufferSize) : LocalMessage(pBuffer, bufferSize)
{
    m_Type = LocalNetworkManager::MESSAGE_TYPE_START_HOST_MIGRATION;
    m_HeaderSize = START_HOST_MIGRATION_HEADER_SIZE;
}

// 0x00423980
// 0x0042397C (deleting dtor)
nn::pia::local::LocalStartHostMigrationMessage::~LocalStartHostMigrationMessage()
{
    // empty (in the original too)
}

} // namespace local
} // namespace pia
} // namespace nn
