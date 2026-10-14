#pragma once

#include "decomp.h"
#include "nn/nex/_Type_string/_Type_string_Api.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_DateTime.h"
#include "nn/nex/nex_Gathering.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex_qVector.h"

namespace nn {
namespace nex {
// The DDL part of PersistentGathering (MatchmakeExtension). The binary has no DDL declaration of
// it; the member names are ours (the game uses none of them).
// RTTI N2nn3nex24_DDL_PersistentGatheringE @ 0x008CEE5C
// vtable 0x008FE988 (vptr 0x008FE990), offset_to_top 0, 9 entries
class _DDL_PersistentGathering : public ::nn::nex::Gathering
{
public:
    // (inline)
    _DDL_PersistentGathering() : m_StructureVersion(0), m_Unknown0x40(0), m_Unknown0x4C(0) {}
    virtual ~_DDL_PersistentGathering(); // 0x003B5F70 slot 0x00 | fefates:bytes
    // 0x003B5F60 slot 0x04 (deleting dtor)
    virtual nn::nex::Gathering* Clone() const; // 0x0072D608 slot 0x08 | fefates:callseq
    virtual nn::nex::String GetGatheringType() const; // 0x0072D5A0 slot 0x0C | fefates:bytes
    virtual bool IsA(const nn::nex::String& typeName) const; // 0x0072D5D0 slot 0x10
    virtual bool IsAKindOf(const nn::nex::String& typeName) const; // 0x0072D628 slot 0x14
    virtual void StreamIn(nn::nex::Message* pMessage) const; // 0x003B5BC4 slot 0x18
    virtual void StreamOut(nn::nex::Message* pMessage); // 0x003B5DAC slot 0x1C

    static void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_PersistentGathering& data); // 0x003B5BD4 (name is ours)
    static void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_PersistentGathering* pData); // 0x003B5DBC | fefates:bytes [tier B]

    u8 m_StructureVersion; // 0x30
    u32 m_Unknown0x34; // 0x34
    String m_Unknown0x38; // 0x38
    qVector<u32> m_Unknown0x40; // 0x40
    qVector<u8> m_Unknown0x4C; // 0x4C
    DateTime m_Unknown0x58; // 0x58
    DateTime m_Unknown0x60; // 0x60
    u32 m_Unknown0x68; // 0x68
    u32 m_Unknown0x6C; // 0x6C
};
ASSERT_OFFSET(_DDL_PersistentGathering, m_Unknown0x40, 0x40);
ASSERT_OFFSET(_DDL_PersistentGathering, m_Unknown0x58, 0x58);
ASSERT_SIZE(_DDL_PersistentGathering, 0x70);
} // namespace nex
} // namespace nn
