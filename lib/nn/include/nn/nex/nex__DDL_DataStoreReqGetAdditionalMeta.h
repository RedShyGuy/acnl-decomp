#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_qBuffer.h"
#include "nn/nex/_Type_qBuffer/_Type_qBuffer_Api.h"

namespace nn {
namespace nex {
// The DDL part of DataStoreReqGetAdditionalMeta (DataStore). The member names are from the DDL declarations in the binary.
// RTTI N2nn3nex34_DDL_DataStoreReqGetAdditionalMetaE @ 0x008CF370
// vtable 0x008FF714 (vptr 0x008FF71C), offset_to_top 0, 2 entries
class _DDL_DataStoreReqGetAdditionalMeta : public ::nn::nex::RootObject
{
public:
    // (inline)
    _DDL_DataStoreReqGetAdditionalMeta() : m_StructureVersion(0), m_MetaBinary(0, 0) {}
    virtual ~_DDL_DataStoreReqGetAdditionalMeta(); // 0x003CB968 slot 0x00
    // 0x003CB948 slot 0x04 (deleting dtor)

    // (inline)
    static DECOMP_ALWAYS_INLINE void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_DataStoreReqGetAdditionalMeta& data)
    {
        bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
        u32 start = 0;
        if (!isWithoutHeader) {
            start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion);
        }
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_OwnerId), sizeof(u32));
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_DataType), sizeof(u16));
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_Version), sizeof(u16));
        _Type_qBuffer::Add(pMessage, data.m_MetaBinary);
        if (!isWithoutHeader) {
            DDLStructure::EndAdd(pMessage, start);
        }
    }
    // (inline)
    static DECOMP_ALWAYS_INLINE void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_DataStoreReqGetAdditionalMeta* pData)
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
        pMessage->ExtractPrimitive(&pData->m_OwnerId);
        if (pMessage->m_IsError) {
            return;
        }
        pMessage->ExtractPrimitive(&pData->m_DataType);
        if (pMessage->m_IsError) {
            return;
        }
        pMessage->ExtractPrimitive(&pData->m_Version);
        if (pMessage->m_IsError) {
            return;
        }
        _Type_qBuffer::Extract(pMessage, &pData->m_MetaBinary);
        if (pMessage->m_IsError) {
            return;
        }
        if (!isWithoutHeader) {
            DDLStructure::EndExtract(pMessage, start, size);
        }
    }

    u8 m_StructureVersion; // 0x04
    u32 m_OwnerId; // 0x08
    u16 m_DataType; // 0x0C
    u16 m_Version; // 0x0E
    qBuffer m_MetaBinary; // 0x10
};
ASSERT_OFFSET(_DDL_DataStoreReqGetAdditionalMeta, m_MetaBinary, 0x10);
ASSERT_SIZE(_DDL_DataStoreReqGetAdditionalMeta, 0x30);
} // namespace nex
} // namespace nn
