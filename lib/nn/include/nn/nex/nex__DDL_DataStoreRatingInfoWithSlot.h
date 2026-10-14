#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_DataStoreRatingInfo.h"

namespace nn {
namespace nex {
// The DDL part of DataStoreRatingInfoWithSlot (DataStore). The member names are from the DDL declarations in the binary.
// RTTI N2nn3nex32_DDL_DataStoreRatingInfoWithSlotE @ 0x008CF2D4
// vtable 0x008FF678 (vptr 0x008FF680), offset_to_top 0, 2 entries
class _DDL_DataStoreRatingInfoWithSlot : public ::nn::nex::RootObject
{
public:
    // (inline)
    _DDL_DataStoreRatingInfoWithSlot() : m_StructureVersion(0) {}
    virtual ~_DDL_DataStoreRatingInfoWithSlot(); // 0x003CAE14 slot 0x00
    // 0x003CAE04 slot 0x04 (deleting dtor)

    // (inline)
    static DECOMP_ALWAYS_INLINE void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_DataStoreRatingInfoWithSlot& data)
    {
        bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
        u32 start = 0;
        if (!isWithoutHeader) {
            start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion);
        }
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_Slot), sizeof(s8));
        DDLClassMetaType<DataStoreRatingInfo>::Add(pMessage, data.m_Rating);
        if (!isWithoutHeader) {
            DDLStructure::EndAdd(pMessage, start);
        }
    }
    // (inline)
    static DECOMP_ALWAYS_INLINE void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_DataStoreRatingInfoWithSlot* pData)
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
        pMessage->ExtractPrimitive(reinterpret_cast<u8*>(&pData->m_Slot));
        if (pMessage->m_IsError) {
            return;
        }
        DDLClassMetaType<DataStoreRatingInfo>::Extract(pMessage, &pData->m_Rating);
        if (pMessage->m_IsError) {
            return;
        }
        if (!isWithoutHeader) {
            DDLStructure::EndExtract(pMessage, start, size);
        }
    }

    u8 m_StructureVersion; // 0x04
    s8 m_Slot; // 0x05
    DataStoreRatingInfo m_Rating; // 0x08
};
ASSERT_OFFSET(_DDL_DataStoreRatingInfoWithSlot, m_Rating, 0x8);
ASSERT_SIZE(_DDL_DataStoreRatingInfoWithSlot, 0x28);
} // namespace nex
} // namespace nn
