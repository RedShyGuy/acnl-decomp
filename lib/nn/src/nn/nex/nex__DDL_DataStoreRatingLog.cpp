#include "nn/nex/nex__DDL_DataStoreRatingLog.h"
#include "nn/nex/nex_DataStoreRatingLog.h"

namespace nn {
namespace nex {
// 0x003B1BCC slot 0x00
// 0x003B1BBC (deleting dtor)
nn::nex::_DDL_DataStoreRatingLog::~_DDL_DataStoreRatingLog()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x003B1A68
void nn::nex::_DDL_DataStoreRatingLog::Extract(nn::nex::Message* pMessage, nn::nex::_DDL_DataStoreRatingLog* pData)
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
    pMessage->ExtractPrimitive(reinterpret_cast<u8*>(&pData->m_IsRated));
    if (pMessage->m_IsError) {
        return;
    }
    pMessage->ExtractPrimitive(&pData->m_Pid);
    if (pMessage->m_IsError) {
        return;
    }
    pMessage->ExtractPrimitive(reinterpret_cast<u32*>(&pData->m_RatingValue));
    if (pMessage->m_IsError) {
        return;
    }
    {
        u64 value;
        pMessage->ExtractPrimitive(&value);
        pData->m_LockExpirationTime = DateTime(value);
    }
    if (pMessage->m_IsError) {
        return;
    }
    if (!isWithoutHeader) {
        DDLStructure::EndExtract(pMessage, start, size);
    }
}

} // namespace nex
} // namespace nn
