#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// The DDL part of DataStoreRatingInfo (DataStore). The member names are from the DDL declarations in the binary.
// RTTI N2nn3nex24_DDL_DataStoreRatingInfoE @ 0x008CEE44
// vtable 0x008FE968 (vptr 0x008FE970), offset_to_top 0, 2 entries
class _DDL_DataStoreRatingInfo : public ::nn::nex::RootObject
{
public:
    // (inline)
    _DDL_DataStoreRatingInfo() : m_StructureVersion(0) {}
    virtual ~_DDL_DataStoreRatingInfo(); // 0x003B5A40 slot 0x00
    // 0x003B5A3C slot 0x04 (deleting dtor)

    // (inline)
    static DECOMP_ALWAYS_INLINE void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_DataStoreRatingInfo& data)
    {
        bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
        u32 start = 0;
        if (!isWithoutHeader) {
            start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion);
        }
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_TotalValue), sizeof(s64));
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_Count), sizeof(u32));
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_InitialValue), sizeof(s64));
        if (!isWithoutHeader) {
            DDLStructure::EndAdd(pMessage, start);
        }
    }
    static DECOMP_NOINLINE void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_DataStoreRatingInfo* pData); // 0x003B57F8

    u8 m_StructureVersion; // 0x04
    s64 m_TotalValue; // 0x08
    u32 m_Count; // 0x10
    s64 m_InitialValue; // 0x18
};
ASSERT_OFFSET(_DDL_DataStoreRatingInfo, m_InitialValue, 0x18);
ASSERT_SIZE(_DDL_DataStoreRatingInfo, 0x20);
} // namespace nex
} // namespace nn
