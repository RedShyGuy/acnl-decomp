#pragma once

#include "decomp.h"
#include "nn/nex/_Type_buffertail/_Type_buffertail_Api.h"
#include "nn/nex/nex_Buffer.h"
#include "nn/nex/nex_Gathering.h"

namespace nn {
namespace nex {
// The DDL part of DynamicGathering (MatchMakingService): a Gathering followed by the rest of the
// message (buffertail, so the part has no structure header). The member name is from the DDL
// declarations in the binary.
// RTTI N2nn3nex21_DDL_DynamicGatheringE @ 0x008CEB74
// vtable 0x008FDF9C (vptr 0x008FDFA4), offset_to_top 0, 9 entries
class _DDL_DynamicGathering : public ::nn::nex::Gathering
{
public:
    // (inline)
    _DDL_DynamicGathering() : m_StructureVersion(0) {}
    virtual ~_DDL_DynamicGathering(); // 0x0039B6EC slot 0x00
    // 0x0039B6B8 slot 0x04 | fefates:bytes (deleting dtor)
    virtual nn::nex::Gathering* Clone() const; // 0x0072CECC slot 0x08 | fefates:bytes
    virtual nn::nex::String GetGatheringType() const; // 0x0072CE6C slot 0x0C | mk7dlp:bytes
    virtual bool IsA(const nn::nex::String& typeName) const; // 0x0072CE98 slot 0x10
    virtual bool IsAKindOf(const nn::nex::String& typeName) const; // 0x0072CF48 slot 0x14
    virtual void StreamIn(nn::nex::Message* pMessage) const; // 0x003846C8 slot 0x18
    virtual void StreamOut(nn::nex::Message* pMessage); // 0x0039B66C slot 0x1C | fefates:bytes

    // (inline) the part of Gathering first
    static DECOMP_ALWAYS_INLINE void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_DynamicGathering& data)
    {
        _DDL_Gathering::Add(pMessage, data);
        _Type_buffertail::Add(pMessage, data.m_bufTail);
    }
    // (inline)
    static DECOMP_ALWAYS_INLINE void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_DynamicGathering* pData)
    {
        _DDL_Gathering::Extract(pMessage, pData);
        if (pMessage->m_IsError) {
            return;
        }
        pData->m_StructureVersion = 0;
        if (pMessage->m_IsError) {
            return;
        }
        _Type_buffertail::Extract(pMessage, &pData->m_bufTail);
    }

    u8 m_StructureVersion; // 0x30
    Buffer m_bufTail; // 0x34
};
ASSERT_OFFSET(_DDL_DynamicGathering, m_bufTail, 0x34);
ASSERT_SIZE(_DDL_DynamicGathering, 0x54);
} // namespace nex
} // namespace nn
