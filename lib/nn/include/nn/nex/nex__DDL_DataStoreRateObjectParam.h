#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// The DDL part of DataStoreRateObjectParam (DataStore). The member names are from the DDL declarations in the binary.
// RTTI N2nn3nex29_DDL_DataStoreRateObjectParamE @ 0x008CF178
// vtable 0x008FF364 (vptr 0x008FF36C), offset_to_top 0, 2 entries
class _DDL_DataStoreRateObjectParam : public ::nn::nex::RootObject
{
public:
    // (inline)
    _DDL_DataStoreRateObjectParam() : m_StructureVersion(0) {}
    virtual ~_DDL_DataStoreRateObjectParam(); // 0x003C1B70 slot 0x00
    // 0x003C1B6C slot 0x04 (deleting dtor)

    // (inline)
    static DECOMP_ALWAYS_INLINE void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_DataStoreRateObjectParam& data)
    {
        bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
        u32 start = 0;
        if (!isWithoutHeader) {
            start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion);
        }
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_RatingValue), sizeof(s32));
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_AccessPassword), sizeof(u64));
        if (!isWithoutHeader) {
            DDLStructure::EndAdd(pMessage, start);
        }
    }
    // (inline)
    static DECOMP_ALWAYS_INLINE void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_DataStoreRateObjectParam* pData)
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
        pMessage->ExtractPrimitive(reinterpret_cast<u32*>(&pData->m_RatingValue));
        if (pMessage->m_IsError) {
            return;
        }
        pMessage->ExtractPrimitive(&pData->m_AccessPassword);
        if (pMessage->m_IsError) {
            return;
        }
        if (!isWithoutHeader) {
            DDLStructure::EndExtract(pMessage, start, size);
        }
    }

    u8 m_StructureVersion; // 0x04
    s32 m_RatingValue; // 0x08
    u64 m_AccessPassword; // 0x10
};
ASSERT_OFFSET(_DDL_DataStoreRateObjectParam, m_AccessPassword, 0x10);
ASSERT_SIZE(_DDL_DataStoreRateObjectParam, 0x18);
} // namespace nex
} // namespace nn
