#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/_Type_string/_Type_string_Api.h"

namespace nn {
namespace nex {
// The DDL part of DataStoreKeyValue (DataStore). The member names are from the DDL declarations in the binary.
// RTTI N2nn3nex22_DDL_DataStoreKeyValueE @ 0x008CEC8C
// vtable 0x008FE1FC (vptr 0x008FE204), offset_to_top 0, 2 entries
class _DDL_DataStoreKeyValue : public ::nn::nex::RootObject
{
public:
    // (inline)
    _DDL_DataStoreKeyValue() : m_StructureVersion(0) {}
    virtual ~_DDL_DataStoreKeyValue(); // 0x0039ED80 slot 0x00
    // 0x0039ED58 slot 0x04 (deleting dtor)

    // (inline)
    static DECOMP_ALWAYS_INLINE void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_DataStoreKeyValue& data)
    {
        bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
        u32 start = 0;
        if (!isWithoutHeader) {
            start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion);
        }
        _Type_string::Add(pMessage, data.m_Key);
        _Type_string::Add(pMessage, data.m_Value);
        if (!isWithoutHeader) {
            DDLStructure::EndAdd(pMessage, start);
        }
    }
    // (inline)
    static DECOMP_ALWAYS_INLINE void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_DataStoreKeyValue* pData)
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
        _Type_string::Extract(pMessage, &pData->m_Key);
        if (pMessage->m_IsError) {
            return;
        }
        _Type_string::Extract(pMessage, &pData->m_Value);
        if (pMessage->m_IsError) {
            return;
        }
        if (!isWithoutHeader) {
            DDLStructure::EndExtract(pMessage, start, size);
        }
    }

    u8 m_StructureVersion; // 0x04
    String m_Key; // 0x08
    String m_Value; // 0x10
};
ASSERT_OFFSET(_DDL_DataStoreKeyValue, m_Value, 0x10);
ASSERT_SIZE(_DDL_DataStoreKeyValue, 0x18);
} // namespace nex
} // namespace nn
