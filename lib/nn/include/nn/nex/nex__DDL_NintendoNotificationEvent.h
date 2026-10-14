#pragma once

#include "decomp.h"
#include "nn/nex/nex_AnyObjectHolder.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_Data.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_String.h"

namespace nn {
namespace nex {
// The DDL part of NintendoNotificationEvent (NintendoProtocolFoundation). The member names are from
// the DDL declarations in the binary.
// RTTI N2nn3nex30_DDL_NintendoNotificationEventE @ 0x008CF218
// vtable 0x008FF4F8 (vptr 0x008FF500), offset_to_top 0, 2 entries
class _DDL_NintendoNotificationEvent : public ::nn::nex::RootObject
{
public:
    // (inline)
    _DDL_NintendoNotificationEvent() : m_StructureVersion(0) {}
    virtual ~_DDL_NintendoNotificationEvent(); // 0x003C4BDC slot 0x00
    // 0x003C4BA4 slot 0x04 (deleting dtor)

    // (inline)
    static DECOMP_ALWAYS_INLINE void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_NintendoNotificationEvent& data)
    {
        bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
        u32 start = 0;
        if (!isWithoutHeader) {
            start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion);
        }
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_uiType), sizeof(u32));
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_pidSender), sizeof(u32));
        data.m_dataholder.StreamIn(pMessage);
        if (!isWithoutHeader) {
            DDLStructure::EndAdd(pMessage, start);
        }
    }
    static void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_NintendoNotificationEvent* pData); // 0x003C4A7C | fefates:bytes [tier B]

    u8 m_StructureVersion;                       // 0x04
    u32 m_uiType;                                // 0x08
    u32 m_pidSender;                             // 0x0C
    AnyObjectHolder<Data, String> m_dataholder;  // 0x10
};
ASSERT_OFFSET(_DDL_NintendoNotificationEvent, m_dataholder, 0x10);
ASSERT_SIZE(_DDL_NintendoNotificationEvent, 0x18);
} // namespace nex
} // namespace nn
