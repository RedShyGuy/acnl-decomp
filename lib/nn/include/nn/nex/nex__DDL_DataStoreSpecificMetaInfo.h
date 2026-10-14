#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// The DDL part of DataStoreSpecificMetaInfo (DataStore). The member names are from the DDL declarations in the binary.
// RTTI N2nn3nex30_DDL_DataStoreSpecificMetaInfoE @ 0x008CF200
// vtable 0x008FF4D8 (vptr 0x008FF4E0), offset_to_top 0, 2 entries
class _DDL_DataStoreSpecificMetaInfo : public ::nn::nex::RootObject
{
public:
    // (inline)
    _DDL_DataStoreSpecificMetaInfo() : m_StructureVersion(0) {}
    virtual ~_DDL_DataStoreSpecificMetaInfo(); // 0x003C48B4 slot 0x00
    // 0x003C48B0 slot 0x04 (deleting dtor)

    // (inline)
    static DECOMP_ALWAYS_INLINE void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_DataStoreSpecificMetaInfo& data)
    {
        bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
        u32 start = 0;
        if (!isWithoutHeader) {
            start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion);
        }
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_DataId), sizeof(u64));
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_OwnerId), sizeof(u32));
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_Size), sizeof(u32));
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_DataType), sizeof(u16));
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_Version), sizeof(u32));
        if (!isWithoutHeader) {
            DDLStructure::EndAdd(pMessage, start);
        }
    }
    // (inline)
    static DECOMP_ALWAYS_INLINE void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_DataStoreSpecificMetaInfo* pData)
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
        pMessage->ExtractPrimitive(&pData->m_OwnerId);
        if (pMessage->m_IsError) {
            return;
        }
        pMessage->ExtractPrimitive(&pData->m_Size);
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
        if (!isWithoutHeader) {
            DDLStructure::EndExtract(pMessage, start, size);
        }
    }

    u8 m_StructureVersion; // 0x04
    u64 m_DataId; // 0x08
    u32 m_OwnerId; // 0x10
    u32 m_Size; // 0x14
    u16 m_DataType; // 0x18
    u32 m_Version; // 0x1C
};
ASSERT_OFFSET(_DDL_DataStoreSpecificMetaInfo, m_Version, 0x1C);
ASSERT_SIZE(_DDL_DataStoreSpecificMetaInfo, 0x20);
} // namespace nex
} // namespace nn
