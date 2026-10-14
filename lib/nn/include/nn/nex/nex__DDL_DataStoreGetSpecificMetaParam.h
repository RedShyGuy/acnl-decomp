#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// The DDL part of DataStoreGetSpecificMetaParam (DataStore). The member names are from the DDL declarations in the binary.
// RTTI N2nn3nex34_DDL_DataStoreGetSpecificMetaParamE @ 0x008CF358
// vtable 0x008FF6F4 (vptr 0x008FF6FC), offset_to_top 0, 2 entries
class _DDL_DataStoreGetSpecificMetaParam : public ::nn::nex::RootObject
{
public:
    // (inline)
    _DDL_DataStoreGetSpecificMetaParam() : m_StructureVersion(0), m_DataIds(0) {}
    virtual ~_DDL_DataStoreGetSpecificMetaParam(); // 0x003CB908 slot 0x00
    // 0x003CB8E8 slot 0x04 (deleting dtor)

    // (inline)
    static DECOMP_ALWAYS_INLINE void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_DataStoreGetSpecificMetaParam& data)
    {
        bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
        u32 start = 0;
        if (!isWithoutHeader) {
            start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion);
        }
        _Type_qvector<_Type_uint64>::Add(pMessage, data.m_DataIds);
        if (!isWithoutHeader) {
            DDLStructure::EndAdd(pMessage, start);
        }
    }
    // (inline)
    static DECOMP_ALWAYS_INLINE void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_DataStoreGetSpecificMetaParam* pData)
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
        _Type_qvector<_Type_uint64>::Extract(pMessage, &pData->m_DataIds);
        if (pMessage->m_IsError) {
            return;
        }
        if (!isWithoutHeader) {
            DDLStructure::EndExtract(pMessage, start, size);
        }
    }

    u8 m_StructureVersion; // 0x04
    qVector<u64> m_DataIds; // 0x08
};
ASSERT_OFFSET(_DDL_DataStoreGetSpecificMetaParam, m_DataIds, 0x8);
ASSERT_SIZE(_DDL_DataStoreGetSpecificMetaParam, 0x14);
} // namespace nex
} // namespace nn
