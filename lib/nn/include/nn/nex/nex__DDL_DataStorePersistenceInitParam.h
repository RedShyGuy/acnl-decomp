#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// The DDL part of DataStorePersistenceInitParam (DataStore). The member names are from the DDL declarations in the binary.
// RTTI N2nn3nex34_DDL_DataStorePersistenceInitParamE @ 0x008CF364
// vtable 0x008FF704 (vptr 0x008FF70C), offset_to_top 0, 2 entries
class _DDL_DataStorePersistenceInitParam : public ::nn::nex::RootObject
{
public:
    // (inline)
    _DDL_DataStorePersistenceInitParam() : m_StructureVersion(0) {}
    virtual ~_DDL_DataStorePersistenceInitParam(); // 0x003CB928 slot 0x00
    // 0x003CB924 slot 0x04 (deleting dtor)
    _DDL_DataStorePersistenceInitParam& operator=(const _DDL_DataStorePersistenceInitParam& other); // 0x003CB92C

    // (inline)
    static DECOMP_ALWAYS_INLINE void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_DataStorePersistenceInitParam& data)
    {
        bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
        u32 start = 0;
        if (!isWithoutHeader) {
            start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion);
        }
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_PersistenceSlotId), sizeof(u16));
        *pMessage << data.m_DeleteLastObject;
        if (!isWithoutHeader) {
            DDLStructure::EndAdd(pMessage, start);
        }
    }
    // (inline)
    static DECOMP_ALWAYS_INLINE void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_DataStorePersistenceInitParam* pData)
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
        pMessage->ExtractPrimitive(&pData->m_PersistenceSlotId);
        if (pMessage->m_IsError) {
            return;
        }
        pMessage->ExtractPrimitive(reinterpret_cast<u8*>(&pData->m_DeleteLastObject));
        if (pMessage->m_IsError) {
            return;
        }
        if (!isWithoutHeader) {
            DDLStructure::EndExtract(pMessage, start, size);
        }
    }

    u8 m_StructureVersion; // 0x04
    u16 m_PersistenceSlotId; // 0x06
    bool m_DeleteLastObject; // 0x08
};
ASSERT_OFFSET(_DDL_DataStorePersistenceInitParam, m_DeleteLastObject, 0x8);
ASSERT_SIZE(_DDL_DataStorePersistenceInitParam, 0xC);
} // namespace nex
} // namespace nn
