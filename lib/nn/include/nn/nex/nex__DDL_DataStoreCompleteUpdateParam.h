#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// The DDL part of DataStoreCompleteUpdateParam (DataStore). The member names are from the DDL declarations in the binary.
// RTTI N2nn3nex33_DDL_DataStoreCompleteUpdateParamE @ 0x008CF328
class _DDL_DataStoreCompleteUpdateParam : public ::nn::nex::RootObject
{
public:
    // (inline)
    _DDL_DataStoreCompleteUpdateParam() : m_StructureVersion(0) {}
    // (inline: the original has no vtable of its own)
    virtual ~_DDL_DataStoreCompleteUpdateParam() {}

    // (inline)
    static DECOMP_ALWAYS_INLINE void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_DataStoreCompleteUpdateParam& data)
    {
        bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
        u32 start = 0;
        if (!isWithoutHeader) {
            start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion);
        }
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_DataId), sizeof(u64));
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_Version), sizeof(u32));
        *pMessage << data.m_IsSuccess;
        if (!isWithoutHeader) {
            DDLStructure::EndAdd(pMessage, start);
        }
    }
    // (inline)
    static DECOMP_ALWAYS_INLINE void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_DataStoreCompleteUpdateParam* pData)
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
        pMessage->ExtractPrimitive(&pData->m_Version);
        if (pMessage->m_IsError) {
            return;
        }
        pMessage->ExtractPrimitive(reinterpret_cast<u8*>(&pData->m_IsSuccess));
        if (pMessage->m_IsError) {
            return;
        }
        if (!isWithoutHeader) {
            DDLStructure::EndExtract(pMessage, start, size);
        }
    }

    u8 m_StructureVersion; // 0x04
    u64 m_DataId; // 0x08
    u32 m_Version; // 0x10
    bool m_IsSuccess; // 0x14
};
ASSERT_OFFSET(_DDL_DataStoreCompleteUpdateParam, m_IsSuccess, 0x14);
ASSERT_SIZE(_DDL_DataStoreCompleteUpdateParam, 0x18);
} // namespace nex
} // namespace nn
