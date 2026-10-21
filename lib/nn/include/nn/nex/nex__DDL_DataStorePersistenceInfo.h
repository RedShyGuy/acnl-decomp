#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// The DDL part of DataStorePersistenceInfo (DataStore). The member names are from the DDL declarations in the binary.
// RTTI N2nn3nex29_DDL_DataStorePersistenceInfoE @ 0x008CF160
// vtable 0x008FF344 (vptr 0x008FF34C), offset_to_top 0, 2 entries
class _DDL_DataStorePersistenceInfo : public ::nn::nex::RootObject
{
public:
    // (inline)
    _DDL_DataStorePersistenceInfo() : m_StructureVersion(0) {}
    virtual ~_DDL_DataStorePersistenceInfo(); // 0x003C1808 slot 0x00
    // 0x003C1804 slot 0x04 (deleting dtor)

    // (inline)
    static DECOMP_ALWAYS_INLINE void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_DataStorePersistenceInfo& data)
    {
        bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
        u32 start = 0;
        if (!isWithoutHeader) {
            start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion);
        }
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_OwnerId), sizeof(u32));
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_PersistenceSlotId), sizeof(u16));
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_DataId), sizeof(u64));
        if (!isWithoutHeader) {
            DDLStructure::EndAdd(pMessage, start);
        }
    }
    static DECOMP_NOINLINE void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_DataStorePersistenceInfo* pData); // 0x003C1720

    u8 m_StructureVersion; // 0x04
    u32 m_OwnerId; // 0x08
    u16 m_PersistenceSlotId; // 0x0C
    u64 m_DataId; // 0x10
};
ASSERT_OFFSET(_DDL_DataStorePersistenceInfo, m_DataId, 0x10);
ASSERT_SIZE(_DDL_DataStorePersistenceInfo, 0x18);
} // namespace nex
} // namespace nn
