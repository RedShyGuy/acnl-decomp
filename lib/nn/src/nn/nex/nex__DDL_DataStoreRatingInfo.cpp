#include "nn/nex/nex__DDL_DataStoreRatingInfo.h"
#include "nn/nex/nex_DataStoreRatingInfo.h"

namespace nn {
namespace nex {
// 0x003B5A40 slot 0x00
// 0x003B5A3C (deleting dtor)
nn::nex::_DDL_DataStoreRatingInfo::~_DDL_DataStoreRatingInfo()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x003B57F8
void nn::nex::_DDL_DataStoreRatingInfo::Extract(nn::nex::Message* pMessage, nn::nex::_DDL_DataStoreRatingInfo* pData)
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
    pMessage->ExtractPrimitive(reinterpret_cast<u64*>(&pData->m_TotalValue));
    if (pMessage->m_IsError) {
        return;
    }
    pMessage->ExtractPrimitive(&pData->m_Count);
    if (pMessage->m_IsError) {
        return;
    }
    pMessage->ExtractPrimitive(reinterpret_cast<u64*>(&pData->m_InitialValue));
    if (pMessage->m_IsError) {
        return;
    }
    if (!isWithoutHeader) {
        DDLStructure::EndExtract(pMessage, start, size);
    }
}

} // namespace nex
} // namespace nn
