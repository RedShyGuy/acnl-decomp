#include "nn/nex/nex__DDL_DataStorePersistenceInfo.h"
#include "nn/nex/nex_DataStorePersistenceInfo.h"

namespace nn {
namespace nex {
// 0x003C1808 slot 0x00
// 0x003C1804 (deleting dtor)
nn::nex::_DDL_DataStorePersistenceInfo::~_DDL_DataStorePersistenceInfo()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x003C1720
void nn::nex::_DDL_DataStorePersistenceInfo::Extract(nn::nex::Message* pMessage, nn::nex::_DDL_DataStorePersistenceInfo* pData)
{
    pData->m_StructureVersion = 0;
    bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
    u32 size = 0;
    if (!isWithoutHeader && !DDLStructure::BeginExtract(pMessage, &pData->m_StructureVersion, &size)) {
        return;
    }
    if (pMessage->m_IsError) {
        return;
    }
    u32 start = pMessage->m_Position;
    pMessage->ExtractPrimitive(&pData->m_OwnerId);
    if (pMessage->m_IsError) {
        return;
    }
    pMessage->ExtractPrimitive(&pData->m_PersistenceSlotId);
    if (pMessage->m_IsError) {
        return;
    }
    pMessage->ExtractPrimitive(&pData->m_DataId);
    if (pMessage->m_IsError) {
        return;
    }
    if (!isWithoutHeader) {
        DDLStructure::EndExtract(pMessage, start, size);
    }
}

} // namespace nex
} // namespace nn
