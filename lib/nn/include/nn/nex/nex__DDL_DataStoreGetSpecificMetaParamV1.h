#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// The DDL part of DataStoreGetSpecificMetaParamV1 (DataStore). The member names are from the DDL declarations in the binary.
// RTTI N2nn3nex36_DDL_DataStoreGetSpecificMetaParamV1E @ 0x008CF3DC
// vtable 0x008FF7CC (vptr 0x008FF7D4), offset_to_top 0, 2 entries
class _DDL_DataStoreGetSpecificMetaParamV1 : public ::nn::nex::RootObject
{
public:
    // (inline)
    _DDL_DataStoreGetSpecificMetaParamV1() : m_StructureVersion(0), m_DataIds(0) {}
    virtual ~_DDL_DataStoreGetSpecificMetaParamV1(); // 0x003CC224 slot 0x00
    // 0x003CC1FC slot 0x04 (deleting dtor)

    // (inline)
    static DECOMP_ALWAYS_INLINE void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_DataStoreGetSpecificMetaParamV1& data)
    {
        bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
        u32 start = 0;
        if (!isWithoutHeader) {
            start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion);
        }
        _Type_qvector<_Type_uint32>::Add(pMessage, data.m_DataIds);
        if (!isWithoutHeader) {
            DDLStructure::EndAdd(pMessage, start);
        }
    }
    // (inline)
    static DECOMP_ALWAYS_INLINE void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_DataStoreGetSpecificMetaParamV1* pData)
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
        _Type_qvector<_Type_uint32>::Extract(pMessage, &pData->m_DataIds);
        if (pMessage->m_IsError) {
            return;
        }
        if (!isWithoutHeader) {
            DDLStructure::EndExtract(pMessage, start, size);
        }
    }

    u8 m_StructureVersion; // 0x04
    qVector<u32> m_DataIds; // 0x08
};
ASSERT_OFFSET(_DDL_DataStoreGetSpecificMetaParamV1, m_DataIds, 0x8);
ASSERT_SIZE(_DDL_DataStoreGetSpecificMetaParamV1, 0x14);
} // namespace nex
} // namespace nn
