#pragma once

#include "decomp.h"
#include "nn/nex/_Type_string/_Type_string_Api.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_String.h"

namespace nn {
namespace nex {
class Gathering;

// The DDL part of Gathering (MatchMakingService), the polymorphic base of the gatherings.
// The member names are ours (from pia::inet::NexMatchmakeSession; the DDL declarations in the binary
// call them m_idMyself, m_pidOwner, m_pidHost, m_uiMinParticipants, m_uiMaxParticipants,
// m_uiParticipationPolicy, m_uiPolicyArgument, m_uiFlags, m_uiState and m_strDescription).
// RTTI N2nn3nex14_DDL_GatheringE @ 0x008CE424
// vtable 0x008FCDEC (vptr 0x008FCDF4), offset_to_top 0, 8 entries
class _DDL_Gathering : public ::nn::nex::RootObject
{
public:
    // (inline)
    _DDL_Gathering() : m_StructureVersion(0) {}
    virtual ~_DDL_Gathering(); // 0x00375F64 slot 0x00
    // 0x00375F44 slot 0x04 (deleting dtor)
    virtual nn::nex::Gathering* Clone() const; // 0x0072B42C slot 0x08 | fefates:bytes
    virtual nn::nex::String GetGatheringType() const; // 0x0072B3EC slot 0x0C | mk7dlp:bytes
    // whether the type name is the own one / the own one or the one of a base (names are ours)
    virtual bool IsA(const nn::nex::String& typeName) const; // 0x0072B408 slot 0x10
    virtual bool IsAKindOf(const nn::nex::String& typeName) const; // 0x0072B44C slot 0x14
    virtual void StreamIn(nn::nex::Message* pMessage) const; // 0x00375BAC slot 0x18
    virtual void StreamOut(nn::nex::Message* pMessage); // 0x00375D10 slot 0x1C

    nn::nex::_DDL_Gathering& operator=(const nn::nex::_DDL_Gathering& other); // 0x00375F80 (name is ours)

    static void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_Gathering& data); // 0x00375BBC | fefates:bytes [tier B]
    static void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_Gathering* pData); // 0x00375D20 | fefates:bytes [tier B]

    u8 m_StructureVersion;        // 0x04
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
