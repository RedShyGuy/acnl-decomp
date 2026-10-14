#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_DateTime.h"
#include "nn/nex/nex_ResultRange.h"
#include "nn/nex/_Type_string/_Type_string_Api.h"

namespace nn {
namespace nex {
// The DDL part of DataStoreSearchParam (DataStore). The member names are from the DDL declarations in the binary.
// RTTI N2nn3nex25_DDL_DataStoreSearchParamE @ 0x008CEF0C
// vtable 0x008FEB7C (vptr 0x008FEB84), offset_to_top 0, 2 entries
class _DDL_DataStoreSearchParam : public ::nn::nex::RootObject
{
public:
    // (inline)
    _DDL_DataStoreSearchParam() : m_StructureVersion(3), m_OwnerIds(0), m_DestinationIds(0), m_Tags(0), m_DataTypes(0) {}
    virtual ~_DDL_DataStoreSearchParam(); // 0x003B8AB0 slot 0x00
    // 0x003B8AA0 slot 0x04 (deleting dtor)

    // (inline)
    static DECOMP_ALWAYS_INLINE void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_DataStoreSearchParam& data)
    {
        bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
        u32 start = 0;
        if (!isWithoutHeader) {
            start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion, 3);
        }
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_SearchTarget), sizeof(u8));
        _Type_qvector<_Type_uint32>::Add(pMessage, data.m_OwnerIds);
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_OwnerType), sizeof(u8));
        _Type_qvector<_Type_uint32>::Add(pMessage, data.m_DestinationIds);
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_DataType), sizeof(u16));
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_CreatedAfter), sizeof(u64));
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_CreatedBefore), sizeof(u64));
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_UpdatedAfter), sizeof(u64));
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_UpdatedBefore), sizeof(u64));
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_ReferDataId), sizeof(u32));
        _Type_qvector<_Type_string>::Add(pMessage, data.m_Tags);
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_ResultOrderColumn), sizeof(u8));
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_ResultOrder), sizeof(u8));
        DDLClassMetaType<ResultRange>::Add(pMessage, data.m_ResultRange);
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_ResultOption), sizeof(u8));
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_MinimalRatingFrequency), sizeof(u32));
        if (!isWithoutHeader) {
            if (data.m_StructureVersion >= 1) {
                *pMessage << data.m_UseCache;
                if (data.m_StructureVersion >= 2) {
                    *pMessage << data.m_TotalCountEnabled;
                    if (data.m_StructureVersion >= 3) {
                        _Type_qvector<_Type_uint16>::Add(pMessage, data.m_DataTypes);
                    }
                }
            }
            DDLStructure::EndAdd(pMessage, start);
        }
    }
    // (inline)
    static DECOMP_ALWAYS_INLINE void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_DataStoreSearchParam* pData)
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
        pMessage->ExtractPrimitive(&pData->m_SearchTarget);
        if (pMessage->m_IsError) {
            return;
        }
        _Type_qvector<_Type_uint32>::Extract(pMessage, &pData->m_OwnerIds);
        if (pMessage->m_IsError) {
            return;
        }
        pMessage->ExtractPrimitive(&pData->m_OwnerType);
        if (pMessage->m_IsError) {
            return;
        }
        _Type_qvector<_Type_uint32>::Extract(pMessage, &pData->m_DestinationIds);
        if (pMessage->m_IsError) {
            return;
        }
        pMessage->ExtractPrimitive(&pData->m_DataType);
        if (pMessage->m_IsError) {
            return;
        }
        {
            u64 value;
            pMessage->ExtractPrimitive(&value);
            pData->m_CreatedAfter = DateTime(value);
        }
        if (pMessage->m_IsError) {
            return;
        }
        {
            u64 value;
            pMessage->ExtractPrimitive(&value);
            pData->m_CreatedBefore = DateTime(value);
        }
        if (pMessage->m_IsError) {
            return;
        }
        {
            u64 value;
            pMessage->ExtractPrimitive(&value);
            pData->m_UpdatedAfter = DateTime(value);
        }
        if (pMessage->m_IsError) {
            return;
        }
        {
            u64 value;
            pMessage->ExtractPrimitive(&value);
            pData->m_UpdatedBefore = DateTime(value);
        }
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
        pMessage->ExtractPrimitive(&pData->m_ResultOrderColumn);
        if (pMessage->m_IsError) {
            return;
        }
        pMessage->ExtractPrimitive(&pData->m_ResultOrder);
        if (pMessage->m_IsError) {
            return;
        }
        DDLClassMetaType<ResultRange>::Extract(pMessage, &pData->m_ResultRange);
        if (pMessage->m_IsError) {
            return;
        }
        pMessage->ExtractPrimitive(&pData->m_ResultOption);
        if (pMessage->m_IsError) {
            return;
        }
        pMessage->ExtractPrimitive(&pData->m_MinimalRatingFrequency);
        if (pMessage->m_IsError) {
            return;
        }
        if (pData->m_StructureVersion >= 1) {
            pMessage->ExtractPrimitive(reinterpret_cast<u8*>(&pData->m_UseCache));
            if (pMessage->m_IsError) {
                return;
            }
        }
        if (pData->m_StructureVersion >= 2) {
            pMessage->ExtractPrimitive(reinterpret_cast<u8*>(&pData->m_TotalCountEnabled));
            if (pMessage->m_IsError) {
                return;
            }
        }
        if (pData->m_StructureVersion >= 3) {
            _Type_qvector<_Type_uint16>::Extract(pMessage, &pData->m_DataTypes);
            if (pMessage->m_IsError) {
                return;
            }
        }
        if (!isWithoutHeader) {
            DDLStructure::EndExtract(pMessage, start, size);
        }
    }

    u8 m_StructureVersion; // 0x04
    u8 m_SearchTarget; // 0x05
    qVector<u32> m_OwnerIds; // 0x08
    u8 m_OwnerType; // 0x14
    qVector<u32> m_DestinationIds; // 0x18
    u16 m_DataType; // 0x24
    DateTime m_CreatedAfter; // 0x28
    DateTime m_CreatedBefore; // 0x30
    DateTime m_UpdatedAfter; // 0x38
    DateTime m_UpdatedBefore; // 0x40
    u32 m_ReferDataId; // 0x48
    qVector<String> m_Tags; // 0x4C
    u8 m_ResultOrderColumn; // 0x58
    u8 m_ResultOrder; // 0x59
    ResultRange m_ResultRange; // 0x5C
    u8 m_ResultOption; // 0x6C
    u32 m_MinimalRatingFrequency; // 0x70
    bool m_UseCache; // 0x74
    bool m_TotalCountEnabled; // 0x75
    qVector<u16> m_DataTypes; // 0x78
};
ASSERT_OFFSET(_DDL_DataStoreSearchParam, m_DataTypes, 0x78);
ASSERT_SIZE(_DDL_DataStoreSearchParam, 0x88);
} // namespace nex
} // namespace nn
