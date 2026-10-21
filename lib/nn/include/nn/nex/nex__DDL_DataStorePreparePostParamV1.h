#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_DataStorePermission.h"
#include "nn/nex/nex_DataStoreRatingInitParamWithSlot.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex_qBuffer.h"
#include "nn/nex/_Type_string/_Type_string_Api.h"
#include "nn/nex/nex_DataStoreConstants.h"
#include "nn/nex/_Type_qBuffer/_Type_qBuffer_Api.h"

namespace nn {
namespace nex {
// The DDL part of DataStorePreparePostParamV1 (DataStore). The member names are from the DDL declarations in the binary.
// RTTI N2nn3nex32_DDL_DataStorePreparePostParamV1E @ 0x008CF2BC
// vtable 0x008FF658 (vptr 0x008FF660), offset_to_top 0, 2 entries
class _DDL_DataStorePreparePostParamV1 : public ::nn::nex::RootObject
{
public:
    // (inline)
    _DDL_DataStorePreparePostParamV1() : m_StructureVersion(0), m_MetaBinary(0, 0), m_Permission(DataStoreConstants::PERMISSION_DEFAULT, qVector<u32>()), m_DelPermission(DataStoreConstants::PERMISSION_DEFAULT, qVector<u32>()), m_Tags(0), m_RatingInitParams(0) {}
    virtual ~_DDL_DataStorePreparePostParamV1(); // 0x003CAB80 slot 0x00
    // 0x003CAB70 slot 0x04 (deleting dtor)

    static DECOMP_NOINLINE void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_DataStorePreparePostParamV1& data); // 0x003CA8DC
    // (inline)
    static DECOMP_ALWAYS_INLINE void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_DataStorePreparePostParamV1* pData)
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
        pMessage->ExtractPrimitive(&pData->m_Size);
        if (pMessage->m_IsError) {
            return;
        }
        _Type_string::Extract(pMessage, &pData->m_Name);
        if (pMessage->m_IsError) {
            return;
        }
        pMessage->ExtractPrimitive(&pData->m_DataType);
        if (pMessage->m_IsError) {
            return;
        }
        _Type_qBuffer::Extract(pMessage, &pData->m_MetaBinary);
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
        pMessage->ExtractPrimitive(&pData->m_Flag);
        if (pMessage->m_IsError) {
            return;
        }
        pMessage->ExtractPrimitive(&pData->m_Period);
        if (pMessage->m_IsError) {
            return;
        }
        pMessage->ExtractPrimitive(&pData->m_ReferDataId);
        if (pMessage->m_IsError) {
            return;
        }
        _Type_qvector<_Type_string>::Extract(pMessage, &pData->m_Tags);
        if (pMessage->m_IsError) {
            return;
        }
        _Type_qvector<DDLClassMetaType<DataStoreRatingInitParamWithSlot>>::Extract(pMessage, &pData->m_RatingInitParams);
        if (pMessage->m_IsError) {
            return;
        }
        if (!isWithoutHeader) {
            DDLStructure::EndExtract(pMessage, start, size);
        }
    }

    u8 m_StructureVersion; // 0x04
    u32 m_Size; // 0x08
    String m_Name; // 0x0C
    u16 m_DataType; // 0x14
    qBuffer m_MetaBinary; // 0x18
    DataStorePermission m_Permission; // 0x38
    DataStorePermission m_DelPermission; // 0x4C
    u32 m_Flag; // 0x60
    u16 m_Period; // 0x64
    u32 m_ReferDataId; // 0x68
    qVector<String> m_Tags; // 0x6C
    qVector<DataStoreRatingInitParamWithSlot> m_RatingInitParams; // 0x78
};
ASSERT_OFFSET(_DDL_DataStorePreparePostParamV1, m_RatingInitParams, 0x78);
ASSERT_SIZE(_DDL_DataStorePreparePostParamV1, 0x84);
} // namespace nex
} // namespace nn
