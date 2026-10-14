#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// The DDL part of DataStoreGetNewArrivedNotificationsParam (DataStore). The member names are from the DDL declarations in the binary.
// RTTI N2nn3nex45_DDL_DataStoreGetNewArrivedNotificationsParamE @ 0x008CF4FC
class _DDL_DataStoreGetNewArrivedNotificationsParam : public ::nn::nex::RootObject
{
public:
    // (inline)
    _DDL_DataStoreGetNewArrivedNotificationsParam() : m_StructureVersion(0) {}
    // (inline: the original has no vtable of its own)
    virtual ~_DDL_DataStoreGetNewArrivedNotificationsParam() {}

    // (inline)
    static DECOMP_ALWAYS_INLINE void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_DataStoreGetNewArrivedNotificationsParam& data)
    {
        bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
        u32 start = 0;
        if (!isWithoutHeader) {
            start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion);
        }
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_LastNotificationId), sizeof(u64));
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_Limit), sizeof(u16));
        if (!isWithoutHeader) {
            DDLStructure::EndAdd(pMessage, start);
        }
    }
    // (inline)
    static DECOMP_ALWAYS_INLINE void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_DataStoreGetNewArrivedNotificationsParam* pData)
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
        pMessage->ExtractPrimitive(&pData->m_LastNotificationId);
        if (pMessage->m_IsError) {
            return;
        }
        pMessage->ExtractPrimitive(&pData->m_Limit);
        if (pMessage->m_IsError) {
            return;
        }
        if (!isWithoutHeader) {
            DDLStructure::EndExtract(pMessage, start, size);
        }
    }

    u8 m_StructureVersion; // 0x04
    u64 m_LastNotificationId; // 0x08
    u16 m_Limit; // 0x10
};
ASSERT_OFFSET(_DDL_DataStoreGetNewArrivedNotificationsParam, m_Limit, 0x10);
ASSERT_SIZE(_DDL_DataStoreGetNewArrivedNotificationsParam, 0x18);
} // namespace nex
} // namespace nn
