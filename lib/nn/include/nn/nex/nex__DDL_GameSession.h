#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_Gathering.h"

namespace nn {
namespace nex {
// The DDL part of GameSession (MatchMakingService): a Gathering without own members (only the
// structure header with the size 0). The virtual functions are in the vtable of GameSession, this
// class has no own one in the binary.
// RTTI N2nn3nex16_DDL_GameSessionE @ 0x008CE638
class _DDL_GameSession : public ::nn::nex::Gathering
{
public:
    // (inline)
    _DDL_GameSession() : m_StructureVersion(0) {}
    virtual nn::nex::Gathering* Clone() const; // 0x0072B814 slot 0x08 | fefates:bytes
    virtual nn::nex::String GetGatheringType() const; // 0x0072B7CC slot 0x0C | mk7dlp:bytes
    virtual bool IsA(const nn::nex::String& typeName) const; // 0x0072B7EC slot 0x10
    virtual bool IsAKindOf(const nn::nex::String& typeName) const; // 0x0072B8B4 slot 0x14
    virtual void StreamIn(nn::nex::Message* pMessage) const; // 0x0072B844 slot 0x18 | fefates:bytes
    virtual void StreamOut(nn::nex::Message* pMessage); // 0x00383F88 slot 0x1C | fefates:bytes

    // (inline) the part of Gathering, then the header of the empty part
    static DECOMP_ALWAYS_INLINE void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_GameSession& data)
    {
        _DDL_Gathering::Add(pMessage, data);
        if (!pMessage->m_IsWithoutStructureHeader) {
            DDLStructure::BeginAdd(pMessage, data.m_StructureVersion);
        }
    }
    // (inline)
    static DECOMP_ALWAYS_INLINE void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_GameSession* pData)
    {
        _DDL_Gathering::Extract(pMessage, pData);
        if (pMessage->m_IsError) {
            return;
        }
        pData->m_StructureVersion = 0;
        u32 size = 0;
        if (!pMessage->m_IsWithoutStructureHeader) {
            DDLStructure::BeginExtractInline(pMessage, &pData->m_StructureVersion, &size);
        }
    }

    u8 m_StructureVersion; // 0x30
};
ASSERT_SIZE(_DDL_GameSession, 0x34);
} // namespace nex
} // namespace nn
