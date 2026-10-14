#include "nn/nex/nex__DDL_NotificationEvent.h"
#include "nn/nex/nex_NotificationEvent.h"

namespace nn {
namespace nex {
// 0x0039F438 slot 0x00
// 0x0039F418 (deleting dtor)
nn::nex::_DDL_NotificationEvent::~_DDL_NotificationEvent()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x0039F2EC
void nn::nex::_DDL_NotificationEvent::Extract(nn::nex::Message* pMessage, nn::nex::_DDL_NotificationEvent* pData)
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
    pMessage->ExtractPrimitive(&pData->m_pidSource);
    if (pMessage->m_IsError) {
        return;
    }
    pMessage->ExtractPrimitive(&pData->m_uiType);
    if (pMessage->m_IsError) {
        return;
    }
    pMessage->ExtractPrimitive(&pData->m_uiParam1);
    if (pMessage->m_IsError) {
        return;
    }
    pMessage->ExtractPrimitive(&pData->m_uiParam2);
    if (pMessage->m_IsError) {
        return;
    }
    _Type_string::Extract(pMessage, &pData->m_strParam);
    if (pMessage->m_IsError) {
        return;
    }
    pMessage->ExtractPrimitive(&pData->m_uiParam3);
    if (pMessage->m_IsError) {
        return;
    }
    if (!isWithoutHeader) {
        DDLStructure::EndExtract(pMessage, start, size);
    }
}

} // namespace nex
} // namespace nn
