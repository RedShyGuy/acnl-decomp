#pragma once

#include "decomp.h"
#include "nn/nex/nex_AnyObjectHolder.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_Gathering.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_String.h"

namespace nn {
namespace nex {
class MatchmakeSession;

// The DDL part of PlayingSession (MatchmakeExtension): a session a principal plays (the list of
// pia::inet::NexMatchmakeSession: 20 bytes per entry). The member names are ours.
// RTTI N2nn3nex19_DDL_PlayingSessionE @ 0x008CE82C
// vtable 0x008FD740 (vptr 0x008FD748), offset_to_top 0, 2 entries
class _DDL_PlayingSession : public ::nn::nex::RootObject
{
public:
    // (inline)
    _DDL_PlayingSession() : m_StructureVersion(0) {}
    virtual ~_DDL_PlayingSession(); // 0x0039535C slot 0x00
    // 0x00395324 slot 0x04 (deleting dtor)
    // the gathering if it is a matchmake session, else null (inline; armlink placed it at the
    // end of the code; name is ours)
    MatchmakeSession* GetMatchmakeSession() const; // 0x0072B208

    // (inline)
    static DECOMP_ALWAYS_INLINE void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_PlayingSession& data)
    {
        bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
        u32 start = 0;
        if (!isWithoutHeader) {
            start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion);
        }
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_PrincipalId), sizeof(u32));
        data.m_Gathering.StreamIn(pMessage);
        if (!isWithoutHeader) {
            DDLStructure::EndAdd(pMessage, start);
        }
    }
    // (inline)
    static DECOMP_ALWAYS_INLINE void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_PlayingSession* pData)
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
        pMessage->ExtractPrimitive(&pData->m_PrincipalId);
        if (pMessage->m_IsError) {
            return;
        }
        pData->m_Gathering.StreamOut(pMessage);
        if (pMessage->m_IsError) {
            return;
        }
        if (!isWithoutHeader) {
            DDLStructure::EndExtract(pMessage, start, size);
        }
    }

    u8 m_StructureVersion;                            // 0x04
    u32 m_PrincipalId;                                // 0x08
    AnyObjectHolder<Gathering, String> m_Gathering;   // 0x0C
};
ASSERT_OFFSET(_DDL_PlayingSession, m_Gathering, 0xC);
ASSERT_SIZE(_DDL_PlayingSession, 0x14);
} // namespace nex
} // namespace nn
