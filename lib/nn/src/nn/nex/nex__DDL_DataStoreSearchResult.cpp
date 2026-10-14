#include "nn/nex/nex__DDL_DataStoreSearchResult.h"
#include "nn/nex/nex_DataStoreSearchResult.h"

namespace nn {
namespace nex {
// 0x003BC24C slot 0x00
// 0x003BC1FC (deleting dtor)
nn::nex::_DDL_DataStoreSearchResult::~_DDL_DataStoreSearchResult()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x003BBE30
void nn::nex::_DDL_DataStoreSearchResult::Extract(nn::nex::Message* pMessage, nn::nex::_DDL_DataStoreSearchResult* pData)
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
    pMessage->ExtractPrimitive(&pData->m_TotalCount);
    if (pMessage->m_IsError) {
        return;
    }
    _Type_qvector<DDLClassMetaType<DataStoreMetaInfo>>::Extract(pMessage, &pData->m_Result);
    if (pMessage->m_IsError) {
        return;
    }
    pMessage->ExtractPrimitive(&pData->m_TotalCountType);
    if (pMessage->m_IsError) {
        return;
    }
    if (!isWithoutHeader) {
        DDLStructure::EndExtract(pMessage, start, size);
    }
}

} // namespace nex
} // namespace nn
