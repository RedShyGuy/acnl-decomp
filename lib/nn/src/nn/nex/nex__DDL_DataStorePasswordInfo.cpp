#include "nn/nex/nex__DDL_DataStorePasswordInfo.h"
#include "nn/nex/nex_DataStorePasswordInfo.h"

namespace nn {
namespace nex {
// 0x003BBCE8 slot 0x00
// 0x003BBCE4 (deleting dtor)
nn::nex::_DDL_DataStorePasswordInfo::~_DDL_DataStorePasswordInfo()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x003BBC00
void nn::nex::_DDL_DataStorePasswordInfo::Extract(nn::nex::Message* pMessage, nn::nex::_DDL_DataStorePasswordInfo* pData)
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
    pMessage->ExtractPrimitive(&pData->m_DataId);
    if (pMessage->m_IsError) {
        return;
    }
    pMessage->ExtractPrimitive(&pData->m_AccessPassword);
    if (pMessage->m_IsError) {
        return;
    }
    pMessage->ExtractPrimitive(&pData->m_UpdatePassword);
    if (pMessage->m_IsError) {
        return;
    }
    if (!isWithoutHeader) {
        DDLStructure::EndExtract(pMessage, start, size);
    }
}

} // namespace nex
} // namespace nn
