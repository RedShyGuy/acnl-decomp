#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_DataStoreKeyValue.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/_Type_string/_Type_string_Api.h"

namespace nn {
namespace nex {
// The DDL part of DataStoreReqGetInfo (DataStore). The member names are from the DDL declarations in the binary.
// RTTI N2nn3nex24_DDL_DataStoreReqGetInfoE @ 0x008CEE50
// vtable 0x008FE978 (vptr 0x008FE980), offset_to_top 0, 2 entries
class _DDL_DataStoreReqGetInfo : public ::nn::nex::RootObject
{
public:
    // (inline)
    _DDL_DataStoreReqGetInfo() : m_StructureVersion(0), m_RequestHeaders(0), m_RootCaCert(0) {}
    virtual ~_DDL_DataStoreReqGetInfo(); // 0x003B5B90 slot 0x00
    // 0x003B5B58 slot 0x04 (deleting dtor)

    // (inline)
    static DECOMP_ALWAYS_INLINE void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_DataStoreReqGetInfo& data)
    {
        bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
        u32 start = 0;
        if (!isWithoutHeader) {
            start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion);
        }
        _Type_string::Add(pMessage, data.m_Url);
        _Type_qvector<DDLClassMetaType<DataStoreKeyValue>>::Add(pMessage, data.m_RequestHeaders);
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_Size), sizeof(u32));
        _Type_qvector<_Type_byte>::Add(pMessage, data.m_RootCaCert);
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_DataId), sizeof(u64));
        if (!isWithoutHeader) {
            DDLStructure::EndAdd(pMessage, start);
        }
    }
    static DECOMP_NOINLINE void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_DataStoreReqGetInfo* pData); // 0x003B5A44

    u8 m_StructureVersion; // 0x04
    String m_Url; // 0x08
    qVector<DataStoreKeyValue> m_RequestHeaders; // 0x10
    u32 m_Size; // 0x1C
    qVector<u8> m_RootCaCert; // 0x20
    u64 m_DataId; // 0x30
};
ASSERT_OFFSET(_DDL_DataStoreReqGetInfo, m_DataId, 0x30);
ASSERT_SIZE(_DDL_DataStoreReqGetInfo, 0x38);
} // namespace nex
} // namespace nn
