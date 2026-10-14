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
// The DDL part of DataStoreChangeMetaCompareParam (DataStore). The member names are from the DDL declarations in the binary.
// RTTI N2nn3nex36_DDL_DataStoreChangeMetaCompareParamE @ 0x008CF3D0
// vtable 0x008FF7BC (vptr 0x008FF7C4), offset_to_top 0, 2 entries
class _DDL_DataStoreChangeMetaCompareParam : public ::nn::nex::RootObject
{
public:
    // (inline)
    _DDL_DataStoreChangeMetaCompareParam() : m_StructureVersion(0), m_Permission(DataStoreConstants::PERMISSION_DEFAULT, qVector<u32>()), m_DelPermission(DataStoreConstants::PERMISSION_DEFAULT, qVector<u32>()), m_MetaBinary(0, 0), m_Tags(0) {}
    virtual ~_DDL_DataStoreChangeMetaCompareParam(); // 0x003CC0D4 slot 0x00
    // 0x003CC048 slot 0x04 (deleting dtor)
    _DDL_DataStoreChangeMetaCompareParam& operator=(const _DDL_DataStoreChangeMetaCompareParam& other); // 0x003CC15C

    static DECOMP_NOINLINE void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_DataStoreChangeMetaCompareParam& data); // 0x003CBEC4
    // (inline)
    static DECOMP_ALWAYS_INLINE void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_DataStoreChangeMetaCompareParam* pData)
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
        pMessage->ExtractPrimitive(&pData->m_ComparisonFlag);
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
        if (!isWithoutHeader) {
            DDLStructure::EndExtract(pMessage, start, size);
        }
    }

    u8 m_StructureVersion; // 0x04
    u32 m_ComparisonFlag; // 0x08
    String m_Name; // 0x0C
    DataStorePermission m_Permission; // 0x14
    DataStorePermission m_DelPermission; // 0x28
    u16 m_Period; // 0x3C
    qBuffer m_MetaBinary; // 0x40
    qVector<String> m_Tags; // 0x60
    u32 m_ReferredCnt; // 0x6C
    u16 m_DataType; // 0x70
    u8 m_Status; // 0x72
};
ASSERT_OFFSET(_DDL_DataStoreChangeMetaCompareParam, m_Status, 0x72);
ASSERT_SIZE(_DDL_DataStoreChangeMetaCompareParam, 0x74);
} // namespace nex
} // namespace nn
