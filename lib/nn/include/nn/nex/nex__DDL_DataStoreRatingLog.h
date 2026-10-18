#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_DateTime.h"

namespace nn {
namespace nex {
// The DDL part of DataStoreRatingLog (DataStore). The member names are from the DDL declarations in the binary.
// RTTI N2nn3nex23_DDL_DataStoreRatingLogE @ 0x008CED30
// vtable 0x008FE67C (vptr 0x008FE684), offset_to_top 0, 2 entries
class _DDL_DataStoreRatingLog : public ::nn::nex::RootObject
{
public:
    // (inline)
    _DDL_DataStoreRatingLog() : m_StructureVersion(0) {}
    virtual ~_DDL_DataStoreRatingLog(); // 0x003B1BCC slot 0x00
    // 0x003B1BBC slot 0x04 (deleting dtor)

    // (inline)
    static DECOMP_ALWAYS_INLINE void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_DataStoreRatingLog& data)
    {
        bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
        u32 start = 0;
        if (!isWithoutHeader) {
            start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion);
        }
        *pMessage << data.m_IsRated;
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_Pid), sizeof(u32));
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_RatingValue), sizeof(s32));
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_LockExpirationTime), sizeof(u64));
        if (!isWithoutHeader) {
            DDLStructure::EndAdd(pMessage, start);
        }
    }
    // (inline)
    static DECOMP_NOINLINE void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_DataStoreRatingLog* pData); // 0x003B1A68

    u8 m_StructureVersion; // 0x04
    bool m_IsRated; // 0x05
    u32 m_Pid; // 0x08
    s32 m_RatingValue; // 0x0C
    DateTime m_LockExpirationTime; // 0x10
};
ASSERT_OFFSET(_DDL_DataStoreRatingLog, m_LockExpirationTime, 0x10);
ASSERT_SIZE(_DDL_DataStoreRatingLog, 0x18);
} // namespace nex
} // namespace nn
