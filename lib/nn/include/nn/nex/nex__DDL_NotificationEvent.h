#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/_Type_string/_Type_string_Api.h"

namespace nn {
namespace nex {
// The DDL part of NotificationEvent (ProtocolFoundation). The member names are from the DDL declarations in the binary.
// RTTI N2nn3nex22_DDL_NotificationEventE @ 0x008CECA4
// vtable 0x008FE21C (vptr 0x008FE224), offset_to_top 0, 2 entries
class _DDL_NotificationEvent : public ::nn::nex::RootObject
{
public:
    // (inline)
    _DDL_NotificationEvent() : m_StructureVersion(0) {}
    virtual ~_DDL_NotificationEvent(); // 0x0039F438 slot 0x00
    // 0x0039F418 slot 0x04 (deleting dtor)

    // (inline)
    static DECOMP_ALWAYS_INLINE void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_NotificationEvent& data)
    {
        bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
        u32 start = 0;
        if (!isWithoutHeader) {
            start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion);
        }
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_pidSource), sizeof(u32));
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_uiType), sizeof(u32));
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_uiParam1), sizeof(u32));
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_uiParam2), sizeof(u32));
        _Type_string::Add(pMessage, data.m_strParam);
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_uiParam3), sizeof(u32));
        if (!isWithoutHeader) {
            DDLStructure::EndAdd(pMessage, start);
        }
    }
    static DECOMP_NOINLINE void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_NotificationEvent* pData); // 0x0039F2EC

    u8 m_StructureVersion; // 0x04
    u32 m_pidSource; // 0x08
    u32 m_uiType; // 0x0C
    u32 m_uiParam1; // 0x10
    u32 m_uiParam2; // 0x14
    String m_strParam; // 0x18
    u32 m_uiParam3; // 0x20
};
ASSERT_OFFSET(_DDL_NotificationEvent, m_uiParam3, 0x20);
ASSERT_SIZE(_DDL_NotificationEvent, 0x24);
} // namespace nex
} // namespace nn
