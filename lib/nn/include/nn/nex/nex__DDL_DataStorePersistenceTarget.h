#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// The DDL part of DataStorePersistenceTarget (DataStore). The member names are from the DDL declarations in the binary.
// RTTI N2nn3nex31_DDL_DataStorePersistenceTargetE @ 0x008CF260
// vtable 0x008FF54C (vptr 0x008FF554), offset_to_top 0, 2 entries
class _DDL_DataStorePersistenceTarget : public ::nn::nex::RootObject
{
public:
    // (inline)
    _DDL_DataStorePersistenceTarget() : m_StructureVersion(0) {}
    virtual ~_DDL_DataStorePersistenceTarget(); // 0x003C6DE4 slot 0x00
    // 0x003C6DE0 slot 0x04 (deleting dtor)
    _DDL_DataStorePersistenceTarget& operator=(const _DDL_DataStorePersistenceTarget& other); // 0x003C6DE8

    // (inline)
    static DECOMP_ALWAYS_INLINE void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_DataStorePersistenceTarget& data)
    {
        bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
        u32 start = 0;
        if (!isWithoutHeader) {
            start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion);
        }
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_OwnerId), sizeof(u32));
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_PersistenceSlotId), sizeof(u16));
        if (!isWithoutHeader) {
            DDLStructure::EndAdd(pMessage, start);
        }
    }
    // (inline)
    static DECOMP_ALWAYS_INLINE void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_DataStorePersistenceTarget* pData)
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
        pMessage->ExtractPrimitive(&pData->m_OwnerId);
        if (pMessage->m_IsError) {
            return;
        }
        pMessage->ExtractPrimitive(&pData->m_PersistenceSlotId);
        if (pMessage->m_IsError) {
            return;
        }
        if (!isWithoutHeader) {
            DDLStructure::EndExtract(pMessage, start, size);
        }
    }

    u8 m_StructureVersion; // 0x04
    u32 m_OwnerId; // 0x08
    u16 m_PersistenceSlotId; // 0x0C
};
ASSERT_OFFSET(_DDL_DataStorePersistenceTarget, m_PersistenceSlotId, 0xC);
ASSERT_SIZE(_DDL_DataStorePersistenceTarget, 0x10);
} // namespace nex
} // namespace nn
