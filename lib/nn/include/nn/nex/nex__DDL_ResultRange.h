#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// The DDL part of ResultRange (ProtocolFoundation). The member names are from the DDL declarations in the binary.
// RTTI N2nn3nex16_DDL_ResultRangeE @ 0x008CE644
// vtable 0x008FD2C8 (vptr 0x008FD2D0), offset_to_top 0, 2 entries
class _DDL_ResultRange : public ::nn::nex::RootObject
{
public:
    // (inline)
    _DDL_ResultRange() : m_StructureVersion(0) {}
    virtual ~_DDL_ResultRange(); // 0x00384100 slot 0x00
    // 0x003840FC slot 0x04 (deleting dtor)
    _DDL_ResultRange& operator=(const _DDL_ResultRange& other); // 0x00384104

    static DECOMP_NOINLINE void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_ResultRange& data); // 0x00384024
    // (inline)
    static void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_ResultRange* pData)
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
        pMessage->ExtractPrimitive(&pData->m_uiOffset);
        if (pMessage->m_IsError) {
            return;
        }
        pMessage->ExtractPrimitive(&pData->m_uiSize);
        if (pMessage->m_IsError) {
            return;
        }
        if (!isWithoutHeader) {
            DDLStructure::EndExtract(pMessage, start, size);
        }
    }

    u8 m_StructureVersion; // 0x04
    u32 m_uiOffset; // 0x08
    u32 m_uiSize; // 0x0C
};
ASSERT_OFFSET(_DDL_ResultRange, m_uiSize, 0xC);
ASSERT_SIZE(_DDL_ResultRange, 0x10);
} // namespace nex
} // namespace nn
