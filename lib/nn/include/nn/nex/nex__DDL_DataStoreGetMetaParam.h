#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_DataStorePersistenceTarget.h"

namespace nn {
namespace nex {
// The DDL part of DataStoreGetMetaParam (DataStore). The member names are from the DDL declarations in the binary.
// RTTI N2nn3nex26_DDL_DataStoreGetMetaParamE @ 0x008CEFA0
// vtable 0x008FEE60 (vptr 0x008FEE68), offset_to_top 0, 2 entries
class _DDL_DataStoreGetMetaParam : public ::nn::nex::RootObject
{
public:
    // (inline)
    _DDL_DataStoreGetMetaParam() : m_StructureVersion(0) {}
    virtual ~_DDL_DataStoreGetMetaParam(); // 0x003BBBE8 slot 0x00
    // 0x003BBBD8 slot 0x04 (deleting dtor)

    // (inline)
    static DECOMP_ALWAYS_INLINE void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_DataStoreGetMetaParam& data)
    {
        bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
        u32 start = 0;
        if (!isWithoutHeader) {
            start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion);
        }
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_DataId), sizeof(u64));
        DDLClassMetaType<DataStorePersistenceTarget>::Add(pMessage, data.m_PersistenceTarget);
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_ResultOption), sizeof(u8));
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_AccessPassword), sizeof(u64));
        if (!isWithoutHeader) {
            DDLStructure::EndAdd(pMessage, start);
        }
    }
    // (inline)
    static DECOMP_ALWAYS_INLINE void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_DataStoreGetMetaParam* pData)
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
        DDLClassMetaType<DataStorePersistenceTarget>::Extract(pMessage, &pData->m_PersistenceTarget);
        if (pMessage->m_IsError) {
            return;
        }
        pMessage->ExtractPrimitive(&pData->m_ResultOption);
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
    u64 m_DataId; // 0x08
    DataStorePersistenceTarget m_PersistenceTarget; // 0x10
    u8 m_ResultOption; // 0x20
    u64 m_AccessPassword; // 0x28
};
ASSERT_OFFSET(_DDL_DataStoreGetMetaParam, m_AccessPassword, 0x28);
ASSERT_SIZE(_DDL_DataStoreGetMetaParam, 0x30);
} // namespace nex
} // namespace nn
