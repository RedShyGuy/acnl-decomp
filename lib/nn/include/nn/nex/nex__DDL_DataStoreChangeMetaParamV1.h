#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_DataStorePermission.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex_qBuffer.h"
#include "nn/nex/_Type_string/_Type_string_Api.h"
#include "nn/nex/nex_DataStoreConstants.h"
#include "nn/nex/_Type_qBuffer/_Type_qBuffer_Api.h"

namespace nn {
namespace nex {
// The DDL part of DataStoreChangeMetaParamV1 (DataStore). The member names are from the DDL declarations in the binary.
// RTTI N2nn3nex31_DDL_DataStoreChangeMetaParamV1E @ 0x008CF248
// vtable 0x008FF53C (vptr 0x008FF544), offset_to_top 0, 2 entries
class _DDL_DataStoreChangeMetaParamV1 : public ::nn::nex::RootObject
{
public:
    // (inline)
    _DDL_DataStoreChangeMetaParamV1() : m_StructureVersion(0), m_Permission(DataStoreConstants::PERMISSION_DEFAULT, qVector<u32>()), m_DelPermission(DataStoreConstants::PERMISSION_DEFAULT, qVector<u32>()), m_MetaBinary(0, 0), m_Tags(0) {}
    virtual ~_DDL_DataStoreChangeMetaParamV1(); // 0x003C6CC0 slot 0x00
    // 0x003C6CB0 slot 0x04 (deleting dtor)

    // (inline)
    static DECOMP_ALWAYS_INLINE void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_DataStoreChangeMetaParamV1& data)
    {
        bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
        u32 start = 0;
        if (!isWithoutHeader) {
            start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion);
        }
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_DataId), sizeof(u64));
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_ModifiesFlag), sizeof(u32));
        _Type_string::Add(pMessage, data.m_Name);
        DDLClassMetaType<DataStorePermission>::Add(pMessage, data.m_Permission);
        DDLClassMetaType<DataStorePermission>::Add(pMessage, data.m_DelPermission);
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_Period), sizeof(u16));
        _Type_qBuffer::Add(pMessage, data.m_MetaBinary);
        _Type_qvector<_Type_string>::Add(pMessage, data.m_Tags);
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_UpdatePassword), sizeof(u64));
        if (!isWithoutHeader) {
            DDLStructure::EndAdd(pMessage, start);
        }
    }
    // (inline)
    static DECOMP_ALWAYS_INLINE void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_DataStoreChangeMetaParamV1* pData)
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
        pMessage->ExtractPrimitive(&pData->m_ModifiesFlag);
        if (pMessage->m_IsError) {
            return;
        }
        _Type_string::Extract(pMessage, &pData->m_Name);
        if (pMessage->m_IsError) {
            return;
        }
        DDLClassMetaType<DataStorePermission>::Extract(pMessage, &pData->m_Permission);
        if (pMessage->m_IsError) {
            return;
        }
        DDLClassMetaType<DataStorePermission>::Extract(pMessage, &pData->m_DelPermission);
        if (pMessage->m_IsError) {
            return;
        }
        pMessage->ExtractPrimitive(&pData->m_Period);
        if (pMessage->m_IsError) {
            return;
        }
        _Type_qBuffer::Extract(pMessage, &pData->m_MetaBinary);
        if (pMessage->m_IsError) {
            return;
        }
        _Type_qvector<_Type_string>::Extract(pMessage, &pData->m_Tags);
        if (pMessage->m_IsError) {
            return;
        }
        pMessage->ExtractPrimitive(&pData->m_UpdatePassword);
        if (pMessage->m_IsError) {
            return;
        }
        if (!isWithoutHeader) {
            DDLStructure::EndExtract(pMessage, start, size);
        }
    }

    u8 m_StructureVersion; // 0x04
    u64 m_DataId; // 0x08
    u32 m_ModifiesFlag; // 0x10
    String m_Name; // 0x14
    DataStorePermission m_Permission; // 0x1C
    DataStorePermission m_DelPermission; // 0x30
    u16 m_Period; // 0x44
    qBuffer m_MetaBinary; // 0x48
    qVector<String> m_Tags; // 0x68
    u64 m_UpdatePassword; // 0x78
};
ASSERT_OFFSET(_DDL_DataStoreChangeMetaParamV1, m_UpdatePassword, 0x78);
ASSERT_SIZE(_DDL_DataStoreChangeMetaParamV1, 0x80);
} // namespace nex
} // namespace nn
