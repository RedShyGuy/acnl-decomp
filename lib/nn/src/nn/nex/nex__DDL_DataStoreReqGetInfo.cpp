#include "nn/nex/nex__DDL_DataStoreReqGetInfo.h"
#include "nn/nex/nex_DataStoreReqGetInfo.h"

namespace nn {
namespace nex {
// 0x003B5B90 slot 0x00
// 0x003B5B58 (deleting dtor)
nn::nex::_DDL_DataStoreReqGetInfo::~_DDL_DataStoreReqGetInfo()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x003B5A44
void nn::nex::_DDL_DataStoreReqGetInfo::Extract(nn::nex::Message* pMessage, nn::nex::_DDL_DataStoreReqGetInfo* pData)
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
    _Type_string::Extract(pMessage, &pData->m_Url);
    if (pMessage->m_IsError) {
        return;
    }
    _Type_qvector<DDLClassMetaType<DataStoreKeyValue>>::Extract(pMessage, &pData->m_RequestHeaders);
    if (pMessage->m_IsError) {
        return;
    }
    pMessage->ExtractPrimitive(&pData->m_Size);
    if (pMessage->m_IsError) {
        return;
    }
    _Type_qvector<_Type_byte>::Extract(pMessage, &pData->m_RootCaCert);
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
