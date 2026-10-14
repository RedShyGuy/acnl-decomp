#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_DataStoreRatingInitParam.h"

namespace nn {
namespace nex {
// The DDL part of DataStoreRatingInitParamWithSlot (DataStore). The member names are from the DDL declarations in the binary.
// RTTI N2nn3nex37_DDL_DataStoreRatingInitParamWithSlotE @ 0x008CF40C
// vtable 0x008FF800 (vptr 0x008FF808), offset_to_top 0, 2 entries
class _DDL_DataStoreRatingInitParamWithSlot : public ::nn::nex::RootObject
{
public:
    // (inline)
    _DDL_DataStoreRatingInitParamWithSlot() : m_StructureVersion(0) {}
    virtual ~_DDL_DataStoreRatingInitParamWithSlot(); // 0x003CC3E8 slot 0x00
    // 0x003CC3D8 slot 0x04 (deleting dtor)
    _DDL_DataStoreRatingInitParamWithSlot& operator=(const _DDL_DataStoreRatingInitParamWithSlot& other); // 0x003CC3F8

    // (inline)
    static DECOMP_ALWAYS_INLINE void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_DataStoreRatingInitParamWithSlot& data)
    {
        bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
        u32 start = 0;
        if (!isWithoutHeader) {
            start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion);
        }
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_Slot), sizeof(s8));
        DDLClassMetaType<DataStoreRatingInitParam>::Add(pMessage, data.m_Param);
        if (!isWithoutHeader) {
            DDLStructure::EndAdd(pMessage, start);
        }
    }
    // (inline)
    static DECOMP_ALWAYS_INLINE void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_DataStoreRatingInitParamWithSlot* pData)
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
        DDLClassMetaType<DataStoreRatingInitParam>::Extract(pMessage, &pData->m_Param);
        if (pMessage->m_IsError) {
            return;
        }
        if (!isWithoutHeader) {
            DDLStructure::EndExtract(pMessage, start, size);
        }
    }

    u8 m_StructureVersion; // 0x04
    s8 m_Slot; // 0x05
    DataStoreRatingInitParam m_Param; // 0x08
};
ASSERT_OFFSET(_DDL_DataStoreRatingInitParamWithSlot, m_Param, 0x8);
ASSERT_SIZE(_DDL_DataStoreRatingInitParamWithSlot, 0x28);
} // namespace nex
} // namespace nn
