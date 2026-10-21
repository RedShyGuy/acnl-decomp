#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/_Type_string/_Type_string_Api.h"

namespace nn {
namespace nex {
// The DDL part of DataStoreReqGetNotificationUrlInfo (DataStore). The member names are from the DDL declarations in the binary.
// RTTI N2nn3nex39_DDL_DataStoreReqGetNotificationUrlInfoE @ 0x008CF44C
// vtable 0x008FF84C (vptr 0x008FF854), offset_to_top 0, 2 entries
class _DDL_DataStoreReqGetNotificationUrlInfo : public ::nn::nex::RootObject
{
public:
    // (inline)
    _DDL_DataStoreReqGetNotificationUrlInfo() : m_StructureVersion(0), m_RootCaCert(0) {}
    virtual ~_DDL_DataStoreReqGetNotificationUrlInfo(); // 0x003CC53C slot 0x00
    // 0x003CC4FC slot 0x04 (deleting dtor)

    // (inline)
    static DECOMP_ALWAYS_INLINE void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_DataStoreReqGetNotificationUrlInfo& data)
    {
        bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
        u32 start = 0;
        if (!isWithoutHeader) {
            start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion);
        }
        _Type_string::Add(pMessage, data.m_Url);
        _Type_string::Add(pMessage, data.m_Key);
        _Type_string::Add(pMessage, data.m_Query);
        _Type_qvector<_Type_byte>::Add(pMessage, data.m_RootCaCert);
        if (!isWithoutHeader) {
            DDLStructure::EndAdd(pMessage, start);
        }
    }
    // (inline)
    static DECOMP_ALWAYS_INLINE void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_DataStoreReqGetNotificationUrlInfo* pData)
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
        _Type_string::Extract(pMessage, &pData->m_Url);
        if (pMessage->m_IsError) {
            return;
        }
        _Type_string::Extract(pMessage, &pData->m_Key);
        if (pMessage->m_IsError) {
            return;
        }
        _Type_string::Extract(pMessage, &pData->m_Query);
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
    String m_Url; // 0x08
    String m_Key; // 0x10
    String m_Query; // 0x18
    qVector<u8> m_RootCaCert; // 0x20
};
ASSERT_OFFSET(_DDL_DataStoreReqGetNotificationUrlInfo, m_RootCaCert, 0x20);
ASSERT_SIZE(_DDL_DataStoreReqGetNotificationUrlInfo, 0x2C);
} // namespace nex
} // namespace nn
