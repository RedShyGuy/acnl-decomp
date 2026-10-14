#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_DataStorePermission.h"
#include "nn/nex/nex_DataStorePersistenceInitParam.h"
#include "nn/nex/nex_DataStoreRatingInitParamWithSlot.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex_qBuffer.h"
#include "nn/nex/_Type_string/_Type_string_Api.h"
#include "nn/nex/nex_DataStoreConstants.h"
#include "nn/nex/_Type_qBuffer/_Type_qBuffer_Api.h"

namespace nn {
namespace nex {
// The DDL part of DataStorePreparePostParam (DataStore). The member names are from the DDL declarations in the binary.
// RTTI N2nn3nex30_DDL_DataStorePreparePostParamE @ 0x008CF1F4
// vtable 0x008FF4C8 (vptr 0x008FF4D0), offset_to_top 0, 2 entries
class _DDL_DataStorePreparePostParam : public ::nn::nex::RootObject
{
public:
    // (inline)
    _DDL_DataStorePreparePostParam() : m_StructureVersion(0), m_MetaBinary(0, 0), m_Permission(DataStoreConstants::PERMISSION_DEFAULT, qVector<u32>()), m_DelPermission(DataStoreConstants::PERMISSION_DEFAULT, qVector<u32>()), m_Tags(0), m_RatingInitParams(0), m_ExtraData(0) {}
    virtual ~_DDL_DataStorePreparePostParam(); // 0x003C46F0 slot 0x00
    // 0x003C46E0 slot 0x04 (deleting dtor)
    _DDL_DataStorePreparePostParam& operator=(const _DDL_DataStorePreparePostParam& other); // 0x003C47E0

    static DECOMP_NOINLINE void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_DataStorePreparePostParam& data); // 0x003C433C
    // (inline)
    static DECOMP_ALWAYS_INLINE void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_DataStorePreparePostParam* pData)
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
        DDLClassMetaType<DataStorePersistenceInitParam>::Extract(pMessage, &pData->m_PersistenceInitParam);
        if (pMessage->m_IsError) {
            return;
        }
        _Type_qvector<_Type_string>::Extract(pMessage, &pData->m_ExtraData);
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
    DataStorePersistenceInitParam m_PersistenceInitParam; // 0x84
    qVector<String> m_ExtraData; // 0x90
};
ASSERT_OFFSET(_DDL_DataStorePreparePostParam, m_ExtraData, 0x90);
ASSERT_SIZE(_DDL_DataStorePreparePostParam, 0x9C);
} // namespace nex
} // namespace nn
