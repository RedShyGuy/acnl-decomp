#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_DataStorePersistenceTarget.h"
#include "nn/nex/_Type_string/_Type_string_Api.h"

namespace nn {
namespace nex {
// The DDL part of DataStorePrepareGetParam (DataStore). The member names are from the DDL declarations in the binary.
// RTTI N2nn3nex29_DDL_DataStorePrepareGetParamE @ 0x008CF16C
// vtable 0x008FF354 (vptr 0x008FF35C), offset_to_top 0, 2 entries
class _DDL_DataStorePrepareGetParam : public ::nn::nex::RootObject
{
public:
    // (inline)
    _DDL_DataStorePrepareGetParam() : m_StructureVersion(0), m_ExtraData(0) {}
    virtual ~_DDL_DataStorePrepareGetParam(); // 0x003C1A48 slot 0x00
    // 0x003C19F8 slot 0x04 (deleting dtor)

    static DECOMP_NOINLINE void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_DataStorePrepareGetParam& data); // 0x003C180C
    // (inline)
    static DECOMP_ALWAYS_INLINE void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_DataStorePrepareGetParam* pData)
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
        pMessage->ExtractPrimitive(&pData->m_LockId);
        if (pMessage->m_IsError) {
            return;
        }
        DDLClassMetaType<DataStorePersistenceTarget>::Extract(pMessage, &pData->m_PersistenceTarget);
        if (pMessage->m_IsError) {
            return;
        }
        pMessage->ExtractPrimitive(&pData->m_AccessPassword);
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
    u64 m_DataId; // 0x08
    u32 m_LockId; // 0x10
    DataStorePersistenceTarget m_PersistenceTarget; // 0x14
    u64 m_AccessPassword; // 0x28
    qVector<String> m_ExtraData; // 0x30
};
ASSERT_OFFSET(_DDL_DataStorePrepareGetParam, m_ExtraData, 0x30);
ASSERT_SIZE(_DDL_DataStorePrepareGetParam, 0x40);
} // namespace nex
} // namespace nn
