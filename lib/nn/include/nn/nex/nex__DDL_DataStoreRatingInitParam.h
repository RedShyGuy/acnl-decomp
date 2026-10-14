#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// The DDL part of DataStoreRatingInitParam (DataStore). The member names are from the DDL declarations in the binary.
// RTTI N2nn3nex29_DDL_DataStoreRatingInitParamE @ 0x008CF184
// vtable 0x008FF374 (vptr 0x008FF37C), offset_to_top 0, 2 entries
class _DDL_DataStoreRatingInitParam : public ::nn::nex::RootObject
{
public:
    // (inline)
    _DDL_DataStoreRatingInitParam() : m_StructureVersion(0) {}
    virtual ~_DDL_DataStoreRatingInitParam(); // 0x003C1CB0 slot 0x00
    // 0x003C1CAC slot 0x04 (deleting dtor)
    _DDL_DataStoreRatingInitParam& operator=(const _DDL_DataStoreRatingInitParam& other); // 0x003C1CB4

    static DECOMP_NOINLINE void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_DataStoreRatingInitParam& data); // 0x003C1B74
    // (inline)
    static DECOMP_ALWAYS_INLINE void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_DataStoreRatingInitParam* pData)
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
        pMessage->ExtractPrimitive(&pData->m_Flag);
        if (pMessage->m_IsError) {
            return;
        }
        pMessage->ExtractPrimitive(&pData->m_InternalFlag);
        if (pMessage->m_IsError) {
            return;
        }
        pMessage->ExtractPrimitive(&pData->m_LockType);
        if (pMessage->m_IsError) {
            return;
        }
        pMessage->ExtractPrimitive(reinterpret_cast<u64*>(&pData->m_InitialValue));
        if (pMessage->m_IsError) {
            return;
        }
        pMessage->ExtractPrimitive(reinterpret_cast<u32*>(&pData->m_RangeMin));
        if (pMessage->m_IsError) {
            return;
        }
        pMessage->ExtractPrimitive(reinterpret_cast<u32*>(&pData->m_RangeMax));
        if (pMessage->m_IsError) {
            return;
        }
        pMessage->ExtractPrimitive(reinterpret_cast<u8*>(&pData->m_PeriodHour));
        if (pMessage->m_IsError) {
            return;
        }
        pMessage->ExtractPrimitive(reinterpret_cast<u16*>(&pData->m_PeriodDuration));
        if (pMessage->m_IsError) {
            return;
        }
        if (!isWithoutHeader) {
            DDLStructure::EndExtract(pMessage, start, size);
        }
    }

    u8 m_StructureVersion; // 0x04
    u8 m_Flag; // 0x05
    u8 m_InternalFlag; // 0x06
    u8 m_LockType; // 0x07
    s64 m_InitialValue; // 0x08
    s32 m_RangeMin; // 0x10
    s32 m_RangeMax; // 0x14
    s8 m_PeriodHour; // 0x18
    s16 m_PeriodDuration; // 0x1A
};
ASSERT_OFFSET(_DDL_DataStoreRatingInitParam, m_PeriodDuration, 0x1A);
ASSERT_SIZE(_DDL_DataStoreRatingInitParam, 0x20);
} // namespace nex
} // namespace nn
