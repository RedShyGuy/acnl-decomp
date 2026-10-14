#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// The DDL part of DataStoreRatingTarget (DataStore). The member names are from the DDL declarations in the binary.
// RTTI N2nn3nex26_DDL_DataStoreRatingTargetE @ 0x008CEFC4
class _DDL_DataStoreRatingTarget : public ::nn::nex::RootObject
{
public:
    // (inline)
    _DDL_DataStoreRatingTarget() : m_StructureVersion(0) {}
    // (inline: the original has no vtable of its own)
    virtual ~_DDL_DataStoreRatingTarget() {}

    static DECOMP_NOINLINE void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_DataStoreRatingTarget& data); // 0x003BBCEC
    // (inline)
    static DECOMP_ALWAYS_INLINE void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_DataStoreRatingTarget* pData)
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
        pMessage->ExtractPrimitive(reinterpret_cast<u8*>(&pData->m_Slot));
        if (pMessage->m_IsError) {
            return;
        }
        if (!isWithoutHeader) {
            DDLStructure::EndExtract(pMessage, start, size);
        }
    }

    u8 m_StructureVersion; // 0x04
    u64 m_DataId; // 0x08
    s8 m_Slot; // 0x10
};
ASSERT_OFFSET(_DDL_DataStoreRatingTarget, m_Slot, 0x10);
ASSERT_SIZE(_DDL_DataStoreRatingTarget, 0x18);
} // namespace nex
} // namespace nn
