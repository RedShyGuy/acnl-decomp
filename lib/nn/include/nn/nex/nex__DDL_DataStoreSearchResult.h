#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_DataStoreMetaInfo.h"

namespace nn {
namespace nex {
// The DDL part of DataStoreSearchResult (DataStore). The member names are from the DDL declarations in the binary.
// RTTI N2nn3nex26_DDL_DataStoreSearchResultE @ 0x008CEFDC
// vtable 0x008FEEA0 (vptr 0x008FEEA8), offset_to_top 0, 2 entries
class _DDL_DataStoreSearchResult : public ::nn::nex::RootObject
{
public:
    // (inline)
    _DDL_DataStoreSearchResult() : m_StructureVersion(0), m_Result(0) {}
    virtual ~_DDL_DataStoreSearchResult(); // 0x003BC24C slot 0x00
    // 0x003BC1FC slot 0x04 (deleting dtor)

    // (inline)
    static DECOMP_ALWAYS_INLINE void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_DataStoreSearchResult& data)
    {
        bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
        u32 start = 0;
        if (!isWithoutHeader) {
            start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion);
        }
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_TotalCount), sizeof(u32));
        _Type_qvector<DDLClassMetaType<DataStoreMetaInfo>>::Add(pMessage, data.m_Result);
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_TotalCountType), sizeof(u8));
        if (!isWithoutHeader) {
            DDLStructure::EndAdd(pMessage, start);
        }
    }
    static DECOMP_NOINLINE void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_DataStoreSearchResult* pData); // 0x003BBE30

    u8 m_StructureVersion; // 0x04
    u32 m_TotalCount; // 0x08
    qVector<DataStoreMetaInfo> m_Result; // 0x0C
    u8 m_TotalCountType; // 0x18
};
ASSERT_OFFSET(_DDL_DataStoreSearchResult, m_TotalCountType, 0x18);
ASSERT_SIZE(_DDL_DataStoreSearchResult, 0x1C);
} // namespace nex
} // namespace nn
