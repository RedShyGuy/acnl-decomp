#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// The DDL part of DataStoreNotificationV1 (DataStore). The member names are from the DDL declarations in the binary.
// RTTI N2nn3nex28_DDL_DataStoreNotificationV1E @ 0x008CF0CC
// vtable 0x008FF128 (vptr 0x008FF130), offset_to_top 0, 2 entries
class _DDL_DataStoreNotificationV1 : public ::nn::nex::RootObject
{
public:
    // (inline)
    _DDL_DataStoreNotificationV1() : m_StructureVersion(0) {}
    virtual ~_DDL_DataStoreNotificationV1(); // 0x003BF760 slot 0x00
    // 0x003BF75C slot 0x04 (deleting dtor)

    // (inline)
    static DECOMP_ALWAYS_INLINE void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_DataStoreNotificationV1& data)
    {
        bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
        u32 start = 0;
        if (!isWithoutHeader) {
            start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion);
        }
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_NotificationId), sizeof(u64));
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_DataId), sizeof(u32));
        if (!isWithoutHeader) {
            DDLStructure::EndAdd(pMessage, start);
        }
    }
    // (inline)
    static DECOMP_ALWAYS_INLINE void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_DataStoreNotificationV1* pData)
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
        pMessage->ExtractPrimitive(&pData->m_NotificationId);
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

    u8 m_StructureVersion; // 0x04
    u64 m_NotificationId; // 0x08
    u32 m_DataId; // 0x10
};
ASSERT_OFFSET(_DDL_DataStoreNotificationV1, m_DataId, 0x10);
ASSERT_SIZE(_DDL_DataStoreNotificationV1, 0x18);
} // namespace nex
} // namespace nn
