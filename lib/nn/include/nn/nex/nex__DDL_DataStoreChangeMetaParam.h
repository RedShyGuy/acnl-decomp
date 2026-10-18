#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_DataStoreChangeMetaCompareParam.h"
#include "nn/nex/nex_DataStorePermission.h"
#include "nn/nex/nex_DataStorePersistenceTarget.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex_qBuffer.h"
#include "nn/nex/_Type_string/_Type_string_Api.h"
#include "nn/nex/nex_DataStoreConstants.h"
#include "nn/nex/_Type_qBuffer/_Type_qBuffer_Api.h"

namespace nn {
namespace nex {
// The DDL part of DataStoreChangeMetaParam (DataStore). The member names are from the DDL declarations in the binary.
// RTTI N2nn3nex29_DDL_DataStoreChangeMetaParamE @ 0x008CF154
// vtable 0x008FF334 (vptr 0x008FF33C), offset_to_top 0, 2 entries
class _DDL_DataStoreChangeMetaParam : public ::nn::nex::RootObject
{
public:
    // (inline)
    _DDL_DataStoreChangeMetaParam() : m_StructureVersion(1), m_Permission(DataStoreConstants::PERMISSION_DEFAULT, qVector<u32>()), m_DelPermission(DataStoreConstants::PERMISSION_DEFAULT, qVector<u32>()), m_MetaBinary(0, 0), m_Tags(0) {}
    virtual ~_DDL_DataStoreChangeMetaParam(); // 0x003C1530 slot 0x00
    // 0x003C1520 slot 0x04 (deleting dtor)
    nn::nex::_DDL_DataStoreChangeMetaParam& operator=(const nn::nex::_DDL_DataStoreChangeMetaParam& other); // 0x003C15BC

    // (inline)
    static DECOMP_ALWAYS_INLINE void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_DataStoreChangeMetaParam& data)
    {
        bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
        u32 start = 0;
        if (!isWithoutHeader) {
            start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion, 1);
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
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_ReferredCnt), sizeof(u32));
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_DataType), sizeof(u16));
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_Status), sizeof(u8));
        DDLClassMetaType<DataStoreChangeMetaCompareParam>::Add(pMessage, data.m_CompareParam);
        if (!isWithoutHeader) {
            if (data.m_StructureVersion >= 1) {
                DDLClassMetaType<DataStorePersistenceTarget>::Add(pMessage, data.m_PersistenceTarget);
            }
            DDLStructure::EndAdd(pMessage, start);
        }
    }
    // (inline)
    static DECOMP_ALWAYS_INLINE void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_DataStoreChangeMetaParam* pData)
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
        pMessage->ExtractPrimitive(&pData->m_ReferredCnt);
        if (pMessage->m_IsError) {
            return;
        }
        pMessage->ExtractPrimitive(&pData->m_DataType);
        if (pMessage->m_IsError) {
            return;
        }
        pMessage->ExtractPrimitive(&pData->m_Status);
        if (pMessage->m_IsError) {
            return;
        }
        DDLClassMetaType<DataStoreChangeMetaCompareParam>::Extract(pMessage, &pData->m_CompareParam);
        if (pMessage->m_IsError) {
            return;
        }
        if (pData->m_StructureVersion >= 1) {
            DDLClassMetaType<DataStorePersistenceTarget>::Extract(pMessage, &pData->m_PersistenceTarget);
            if (pMessage->m_IsError) {
                return;
            }
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
    u32 m_ReferredCnt; // 0x80
    u16 m_DataType; // 0x84
    u8 m_Status; // 0x86
    DataStoreChangeMetaCompareParam m_CompareParam; // 0x88
    DataStorePersistenceTarget m_PersistenceTarget; // 0xFC
};
ASSERT_OFFSET(_DDL_DataStoreChangeMetaParam, m_PersistenceTarget, 0xFC);
ASSERT_SIZE(_DDL_DataStoreChangeMetaParam, 0x110);
} // namespace nex
} // namespace nn
