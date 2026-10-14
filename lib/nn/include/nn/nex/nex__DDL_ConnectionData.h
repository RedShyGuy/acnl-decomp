#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_StationURL.h"
#include "nn/nex/_Type_stationurl/_Type_stationurl_Api.h"

namespace nn {
namespace nex {
// The DDL part of ConnectionData (SecureConnectionService). The member names are from the DDL declarations in the binary.
// RTTI N2nn3nex19_DDL_ConnectionDataE @ 0x008CE814
// vtable 0x008FD720 (vptr 0x008FD728), offset_to_top 0, 2 entries
class _DDL_ConnectionData : public ::nn::nex::RootObject
{
public:
    // (inline)
    _DDL_ConnectionData() : m_StructureVersion(0) {}
    virtual ~_DDL_ConnectionData(); // 0x00394CFC slot 0x00
    // 0x00394CDC slot 0x04 (deleting dtor)

    // (inline)
    static DECOMP_ALWAYS_INLINE void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_ConnectionData& data)
    {
        bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
        u32 start = 0;
        if (!isWithoutHeader) {
            start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion);
        }
        _Type_stationurl::Add(pMessage, data.m_StationUrl);
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_ConnectionID), sizeof(u32));
        if (!isWithoutHeader) {
            DDLStructure::EndAdd(pMessage, start);
        }
    }
    // (inline)
    static DECOMP_ALWAYS_INLINE void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_ConnectionData* pData)
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
        _Type_stationurl::Extract(pMessage, &pData->m_StationUrl);
        if (pMessage->m_IsError) {
            return;
        }
        pMessage->ExtractPrimitive(&pData->m_ConnectionID);
        if (pMessage->m_IsError) {
            return;
        }
        if (!isWithoutHeader) {
            DDLStructure::EndExtract(pMessage, start, size);
        }
    }

    u8 m_StructureVersion; // 0x04
    StationURL m_StationUrl; // 0x08
    u32 m_ConnectionID; // 0x70
};
ASSERT_OFFSET(_DDL_ConnectionData, m_ConnectionID, 0x70);
ASSERT_SIZE(_DDL_ConnectionData, 0x74);
} // namespace nex
} // namespace nn
