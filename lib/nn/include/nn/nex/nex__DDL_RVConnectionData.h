#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_DateTime.h"
#include "nn/nex/nex_StationURL.h"
#include "nn/nex/_Type_stationurl/_Type_stationurl_Api.h"

namespace nn {
namespace nex {
// The DDL part of RVConnectionData (AuthenticationFoundation). The member names are from the DDL declarations in the binary.
// RTTI N2nn3nex21_DDL_RVConnectionDataE @ 0x008CEB98
// vtable 0x008FDFF4 (vptr 0x008FDFFC), offset_to_top 0, 2 entries
class _DDL_RVConnectionData : public ::nn::nex::RootObject
{
public:
    // (inline)
    _DDL_RVConnectionData() : m_StructureVersion(1) {}
    virtual ~_DDL_RVConnectionData(); // 0x0039BFF4 slot 0x00
    // 0x0039BFE4 slot 0x04 (deleting dtor)

    // (inline)
    static DECOMP_ALWAYS_INLINE void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_RVConnectionData& data)
    {
        bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
        u32 start = 0;
        if (!isWithoutHeader) {
            start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion, 1);
        }
        _Type_stationurl::Add(pMessage, data.m_urlRegularProtocols);
        _Type_std_list<_Type_byte>::Add(pMessage, data.m_lstSpecialProtocols);
        _Type_stationurl::Add(pMessage, data.m_urlSpecialProtocols);
        if (!isWithoutHeader) {
            if (data.m_StructureVersion >= 1) {
                u64 currentUTCTime = data.m_currentUTCTime;
                pMessage->AppendRaw(reinterpret_cast<const u8*>(&currentUTCTime), sizeof(u64));
            }
            DDLStructure::EndAdd(pMessage, start);
        }
    }
    static DECOMP_NOINLINE void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_RVConnectionData* pData); // 0x0039BD24

    u8 m_StructureVersion; // 0x04
    StationURL m_urlRegularProtocols; // 0x08
    qList<u8> m_lstSpecialProtocols; // 0x70
    StationURL m_urlSpecialProtocols; // 0x88
    DateTime m_currentUTCTime; // 0xF0
};
ASSERT_OFFSET(_DDL_RVConnectionData, m_currentUTCTime, 0xF0);
ASSERT_SIZE(_DDL_RVConnectionData, 0xF8);
} // namespace nex
} // namespace nn
