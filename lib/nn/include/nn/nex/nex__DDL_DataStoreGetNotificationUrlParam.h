#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/_Type_string/_Type_string_Api.h"

namespace nn {
namespace nex {
// The DDL part of DataStoreGetNotificationUrlParam (DataStore). The member names are from the DDL declarations in the binary.
// RTTI N2nn3nex37_DDL_DataStoreGetNotificationUrlParamE @ 0x008CF400
// vtable 0x008FF7F0 (vptr 0x008FF7F8), offset_to_top 0, 2 entries
class _DDL_DataStoreGetNotificationUrlParam : public ::nn::nex::RootObject
{
public:
    // (inline)
    _DDL_DataStoreGetNotificationUrlParam() : m_StructureVersion(0) {}
    virtual ~_DDL_DataStoreGetNotificationUrlParam(); // 0x003CC3BC slot 0x00
    // 0x003CC39C slot 0x04 (deleting dtor)

    // (inline)
    static DECOMP_ALWAYS_INLINE void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_DataStoreGetNotificationUrlParam& data)
    {
        bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
        u32 start = 0;
        if (!isWithoutHeader) {
            start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion);
        }
        _Type_string::Add(pMessage, data.m_PreviousUrl);
        if (!isWithoutHeader) {
            DDLStructure::EndAdd(pMessage, start);
        }
    }
    // (inline)
    static DECOMP_ALWAYS_INLINE void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_DataStoreGetNotificationUrlParam* pData)
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
        _Type_string::Extract(pMessage, &pData->m_PreviousUrl);
        if (pMessage->m_IsError) {
            return;
        }
        if (!isWithoutHeader) {
            DDLStructure::EndExtract(pMessage, start, size);
        }
    }

    u8 m_StructureVersion; // 0x04
    String m_PreviousUrl; // 0x08
};
ASSERT_OFFSET(_DDL_DataStoreGetNotificationUrlParam, m_PreviousUrl, 0x8);
ASSERT_SIZE(_DDL_DataStoreGetNotificationUrlParam, 0x10);
} // namespace nex
} // namespace nn
