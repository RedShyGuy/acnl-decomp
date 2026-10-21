#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/_Type_string/_Type_string_Api.h"

namespace nn {
namespace nex {
// The DDL part of DataStorePrepareUpdateParam (DataStore). The member names are from the DDL declarations in the binary.
// RTTI N2nn3nex32_DDL_DataStorePrepareUpdateParamE @ 0x008CF2C8
// vtable 0x008FF668 (vptr 0x008FF670), offset_to_top 0, 2 entries
class _DDL_DataStorePrepareUpdateParam : public ::nn::nex::RootObject
{
public:
    // (inline)
    _DDL_DataStorePrepareUpdateParam() : m_StructureVersion(0), m_ExtraData(0) {}
    virtual ~_DDL_DataStorePrepareUpdateParam(); // 0x003CADB8 slot 0x00
    // 0x003CAD68 slot 0x04 (deleting dtor)

    static DECOMP_NOINLINE void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_DataStorePrepareUpdateParam& data); // 0x003CAC3C
    // (inline)
    static DECOMP_ALWAYS_INLINE void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_DataStorePrepareUpdateParam* pData)
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
        pMessage->ExtractPrimitive(&pData->m_DataId);
        if (pMessage->m_IsError) {
            return;
        }
        pMessage->ExtractPrimitive(&pData->m_Size);
        if (pMessage->m_IsError) {
            return;
        }
        pMessage->ExtractPrimitive(&pData->m_UpdatePassword);
        if (pMessage->m_IsError) {
            return;
        }
        _Type_qvector<_Type_string>::Extract(pMessage, &pData->m_ExtraData);
        if (pMessage->m_IsError) {
            return;
        }
        if (!isWithoutHeader) {
            DDLStructure::EndExtract(pMessage, start, size);
        }
    }

    u8 m_StructureVersion; // 0x04
    u64 m_DataId; // 0x08
    u32 m_Size; // 0x10
    u64 m_UpdatePassword; // 0x18
    qVector<String> m_ExtraData; // 0x20
};
ASSERT_OFFSET(_DDL_DataStorePrepareUpdateParam, m_ExtraData, 0x20);
ASSERT_SIZE(_DDL_DataStorePrepareUpdateParam, 0x30);
} // namespace nex
} // namespace nn
