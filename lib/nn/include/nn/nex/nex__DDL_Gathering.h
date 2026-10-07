#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_String.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex14_DDL_GatheringE @ 0x008CE424
// vtable 0x008FCDEC (vptr 0x008FCDF4), offset_to_top 0, 8 entries
//
// The members are from pia::inet::NexMatchmakeSession (the names are ours; Clone creates 48
// bytes).
class _DDL_Gathering : public ::nn::nex::RootObject
{
public:
    _DDL_Gathering(); // ctor address unknown
    virtual ~_DDL_Gathering(); // 0x00375F64 slot 0x00 | slot vf_0x00 of nn::nex::_DDL_Gathering
    // 0x00375F44 slot 0x04 | slot vf_0x04 of nn::nex::_DDL_Gathering (deleting dtor)
    virtual void Clone() const; // 0x0072B42C slot 0x08 | nintendogs:bytes
    virtual void GetGatheringType() const; // 0x0072B3EC slot 0x0C | mk7dlp:bytes
    // whether the type name is the own one / the own one or the one of a base (names are ours)
    virtual bool IsA(const String& typeName) const; // 0x0072B408 slot 0x10
    virtual bool IsAKindOf(const String& typeName) const; // 0x0072B44C slot 0x14
    virtual void StreamIn(nn::nex::Message*) const; // 0x00375BAC slot 0x18 | slot vf_0x18 of nn::nex::_DDL_Gathering
    virtual void StreamOut(nn::nex::Message*); // 0x00375D10 slot 0x1C | slot vf_0x1C of nn::nex::_DDL_Gathering
    void Add(nn::nex::Message*, const nn::nex::_DDL_Gathering&); // 0x00375BBC | fefates:bytes [tier B]
    void Extract(nn::nex::Message*, nn::nex::_DDL_Gathering*); // 0x00375D20 | fefates:bytes [tier B]

    u8 m_Unknown0x4;              // 0x04
    u32 m_Id;                     // 0x08
    u32 m_OwnerPrincipalId;       // 0x0C
    u32 m_HostPrincipalId;        // 0x10
    u16 m_MinParticipants;        // 0x14
    u16 m_MaxParticipants;        // 0x16
    u32 m_ParticipationPolicy;    // 0x18
    u32 m_PolicyArgument;         // 0x1C
    u32 m_Flags;                  // 0x20
    u32 m_State;                  // 0x24
    String m_Description;         // 0x28
};
ASSERT_OFFSET(_DDL_Gathering, m_Id, 0x8);
ASSERT_OFFSET(_DDL_Gathering, m_Flags, 0x20);
ASSERT_OFFSET(_DDL_Gathering, m_Description, 0x28);
ASSERT_SIZE(_DDL_Gathering, 0x30);
} // namespace nex
} // namespace nn
