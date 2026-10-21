#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// The DDL part of DataStorePrepareGetParamV1 (DataStore). The member names are from the DDL declarations in the binary.
// RTTI N2nn3nex31_DDL_DataStorePrepareGetParamV1E @ 0x008CF26C
class _DDL_DataStorePrepareGetParamV1 : public ::nn::nex::RootObject
{
public:
    // (inline)
    _DDL_DataStorePrepareGetParamV1() : m_StructureVersion(0) {}
    // (inline: the original has no vtable of its own)
    virtual ~_DDL_DataStorePrepareGetParamV1() {}

    // (inline)
    static DECOMP_ALWAYS_INLINE void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_DataStorePrepareGetParamV1& data)
    {
        bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
        u32 start = 0;
        if (!isWithoutHeader) {
            start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion);
        }
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_DataId), sizeof(u32));
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_LockId), sizeof(u32));
        if (!isWithoutHeader) {
            DDLStructure::EndAdd(pMessage, start);
        }
    }
    // (inline)
    static DECOMP_ALWAYS_INLINE void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_DataStorePrepareGetParamV1* pData)
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
        pMessage->ExtractPrimitive(&pData->m_LockId);
        if (pMessage->m_IsError) {
            return;
        }
        if (!isWithoutHeader) {
            DDLStructure::EndExtract(pMessage, start, size);
        }
    }

    u8 m_StructureVersion; // 0x04
    u32 m_DataId; // 0x08
    u32 m_LockId; // 0x0C
};
ASSERT_OFFSET(_DDL_DataStorePrepareGetParamV1, m_LockId, 0xC);
ASSERT_SIZE(_DDL_DataStorePrepareGetParamV1, 0x10);
} // namespace nex
} // namespace nn
