#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// The DDL part of DataStorePermission (DataStore). The member names are from the DDL declarations in the binary.
// RTTI N2nn3nex24_DDL_DataStorePermissionE @ 0x008CEE38
// vtable 0x008FE958 (vptr 0x008FE960), offset_to_top 0, 2 entries
class _DDL_DataStorePermission : public ::nn::nex::RootObject
{
public:
    // (inline)
    _DDL_DataStorePermission() : m_StructureVersion(0), m_RecipientIds(0) {}
    virtual ~_DDL_DataStorePermission(); // 0x003B57A8 slot 0x00
    // 0x003B5780 slot 0x04 (deleting dtor)
    _DDL_DataStorePermission& operator=(const _DDL_DataStorePermission& other); // 0x003B57CC

    // (inline)
    static DECOMP_ALWAYS_INLINE void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_DataStorePermission& data)
    {
        bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
        u32 start = 0;
        if (!isWithoutHeader) {
            start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion);
        }
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_Permission), sizeof(u8));
        _Type_qvector<_Type_uint32>::Add(pMessage, data.m_RecipientIds);
        if (!isWithoutHeader) {
            DDLStructure::EndAdd(pMessage, start);
        }
    }
    // (inline)
    static DECOMP_ALWAYS_INLINE void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_DataStorePermission* pData)
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
        pMessage->ExtractPrimitive(&pData->m_Permission);
        if (pMessage->m_IsError) {
            return;
        }
        _Type_qvector<_Type_uint32>::Extract(pMessage, &pData->m_RecipientIds);
        if (pMessage->m_IsError) {
            return;
        }
        if (!isWithoutHeader) {
            DDLStructure::EndExtract(pMessage, start, size);
        }
    }

    u8 m_StructureVersion; // 0x04
    u8 m_Permission; // 0x05
    qVector<u32> m_RecipientIds; // 0x08
};
ASSERT_OFFSET(_DDL_DataStorePermission, m_RecipientIds, 0x8);
ASSERT_SIZE(_DDL_DataStorePermission, 0x14);
} // namespace nex
} // namespace nn
