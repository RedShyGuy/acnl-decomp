#include "nn/nex/nex__DDL_NintendoNotificationEvent.h"

namespace nn {
namespace nex {
// 0x003C4BDC slot 0x00
// 0x003C4BA4 (deleting dtor)
nn::nex::_DDL_NintendoNotificationEvent::~_DDL_NintendoNotificationEvent()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x003C4A7C | fefates:bytes [tier B]
void nn::nex::_DDL_NintendoNotificationEvent::Extract(nn::nex::Message* pMessage, nn::nex::_DDL_NintendoNotificationEvent* pData)
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
    pMessage->ExtractPrimitive(&pData->m_uiType);
    if (pMessage->m_IsError) {
        return;
    }
    pMessage->ExtractPrimitive(&pData->m_pidSender);
    if (pMessage->m_IsError) {
        return;
    }
    pData->m_dataholder.StreamOut(pMessage);
    if (pMessage->m_IsError) {
        return;
    }
    if (!isWithoutHeader) {
        DDLStructure::EndExtract(pMessage, start, size);
    }
}

} // namespace nex
} // namespace nn
