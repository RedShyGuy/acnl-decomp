#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_DataStoreKeyValue.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/_Type_string/_Type_string_Api.h"

namespace nn {
namespace nex {
// The DDL part of DataStoreReqUpdateInfo (DataStore). The member names are from the DDL declarations in the binary.
// RTTI N2nn3nex27_DDL_DataStoreReqUpdateInfoE @ 0x008CF06C
// vtable 0x008FEFDC (vptr 0x008FEFE4), offset_to_top 0, 2 entries
class _DDL_DataStoreReqUpdateInfo : public ::nn::nex::RootObject
{
public:
    // (inline)
    _DDL_DataStoreReqUpdateInfo() : m_StructureVersion(0), m_RequestHeaders(0), m_FormFields(0), m_RootCaCert(0) {}
    virtual ~_DDL_DataStoreReqUpdateInfo(); // 0x003BDA48 slot 0x00
    // 0x003BDA08 slot 0x04 (deleting dtor)

    // (inline)
    static DECOMP_ALWAYS_INLINE void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_DataStoreReqUpdateInfo& data)
    {
        bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
        u32 start = 0;
        if (!isWithoutHeader) {
            start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion);
        }
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_Version), sizeof(u32));
        _Type_string::Add(pMessage, data.m_Url);
        _Type_qvector<DDLClassMetaType<DataStoreKeyValue>>::Add(pMessage, data.m_RequestHeaders);
        _Type_qvector<DDLClassMetaType<DataStoreKeyValue>>::Add(pMessage, data.m_FormFields);
        _Type_qvector<_Type_byte>::Add(pMessage, data.m_RootCaCert);
        if (!isWithoutHeader) {
            DDLStructure::EndAdd(pMessage, start);
        }
    }
    // (inline)
    static DECOMP_ALWAYS_INLINE void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_DataStoreReqUpdateInfo* pData)
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
        pMessage->ExtractPrimitive(&pData->m_Version);
        if (pMessage->m_IsError) {
            return;
        }
        _Type_string::Extract(pMessage, &pData->m_Url);
        if (pMessage->m_IsError) {
            return;
        }
        _Type_qvector<DDLClassMetaType<DataStoreKeyValue>>::Extract(pMessage, &pData->m_RequestHeaders);
        if (pMessage->m_IsError) {
            return;
        }
        _Type_qvector<DDLClassMetaType<DataStoreKeyValue>>::Extract(pMessage, &pData->m_FormFields);
        if (pMessage->m_IsError) {
            return;
        }
        _Type_qvector<_Type_byte>::Extract(pMessage, &pData->m_RootCaCert);
        if (pMessage->m_IsError) {
            return;
        }
        if (!isWithoutHeader) {
            DDLStructure::EndExtract(pMessage, start, size);
        }
    }

    u8 m_StructureVersion; // 0x04
    u32 m_Version; // 0x08
    String m_Url; // 0x0C
    qVector<DataStoreKeyValue> m_RequestHeaders; // 0x14
    qVector<DataStoreKeyValue> m_FormFields; // 0x20
    qVector<u8> m_RootCaCert; // 0x2C
};
ASSERT_OFFSET(_DDL_DataStoreReqUpdateInfo, m_RootCaCert, 0x2C);
ASSERT_SIZE(_DDL_DataStoreReqUpdateInfo, 0x38);
} // namespace nex
} // namespace nn
