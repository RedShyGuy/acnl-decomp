#include "nn/nex/nex__DDL_RVConnectionData.h"
#include "nn/nex/nex_RVConnectionData.h"

namespace nn {
namespace nex {
// 0x0039BFF4 slot 0x00
// 0x0039BFE4 (deleting dtor)
nn::nex::_DDL_RVConnectionData::~_DDL_RVConnectionData()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x0039BD24
void nn::nex::_DDL_RVConnectionData::Extract(nn::nex::Message* pMessage, nn::nex::_DDL_RVConnectionData* pData)
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
    _Type_stationurl::Extract(pMessage, &pData->m_urlRegularProtocols);
    if (pMessage->m_IsError) {
        return;
    }
    _Type_std_list<_Type_byte>::Extract(pMessage, &pData->m_lstSpecialProtocols);
    if (pMessage->m_IsError) {
        return;
    }
    _Type_stationurl::Extract(pMessage, &pData->m_urlSpecialProtocols);
    if (pMessage->m_IsError) {
        return;
    }
    if (pData->m_StructureVersion >= 1) {
        {
            u64 value;
            pMessage->ExtractPrimitive(&value);
            pData->m_currentUTCTime = DateTime(value);
        }
        if (pMessage->m_IsError) {
            return;
        }
    }
    if (!isWithoutHeader) {
        DDLStructure::EndExtract(pMessage, start, size);
    }
}

} // namespace nex
} // namespace nn
