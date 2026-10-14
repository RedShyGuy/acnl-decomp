#pragma once

#include "decomp.h"
#include "nn/nex/nex_Data.h"
#include "nn/nex/nex_DateTime.h"
#include "nn/nex/nex_String.h"

namespace nn {
namespace nex {
// The DDL part of UserMessage (ProtocolFoundation), a polymorphic Data. The member names are from the
// DDL declarations in the binary; the version of this part follows the one of Data.
// RTTI N2nn3nex16_DDL_UserMessageE @ 0x008CE65C
// vtable 0x008FD304 (vptr 0x008FD30C), offset_to_top 0, 8 entries
class _DDL_UserMessage : public ::nn::nex::Data
{
public:
    // (inline)
    _DDL_UserMessage() : m_StructureVersion(0) {}
    virtual ~_DDL_UserMessage(); // 0x0038462C slot 0x00
    // 0x00384604 slot 0x04 (deleting dtor)
    virtual nn::nex::Data* Clone() const; // 0x0072BBA8 slot 0x08
    virtual nn::nex::String GetDataType() const; // 0x0072BB60 slot 0x0C
    virtual bool IsA(const nn::nex::String& name) const; // 0x0072BB80 slot 0x10
    virtual bool IsAKindOf(const nn::nex::String& name) const; // 0x0072BCB0 slot 0x14
    virtual void StreamIn(nn::nex::Message* pMessage) const; // 0x003842C4 slot 0x18
    virtual void StreamOut(nn::nex::Message* pMessage); // 0x0038443C slot 0x1C

    // the members of this part (not the version of Data)
    nn::nex::_DDL_UserMessage& operator=(const nn::nex::_DDL_UserMessage& other); // 0x00384650 (name is ours)

    // the part of Data first
    static void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_UserMessage& data); // 0x003842D4 (name is ours)
    static void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_UserMessage* pData); // 0x0038444C (name is ours)

    u8 m_StructureVersion; // 0x05
    u32 m_uiID; // 0x08
    u32 m_idRecipient; // 0x0C
    u32 m_uiRecipientType; // 0x10
    u32 m_uiParentID; // 0x14
    u32 m_pidSender; // 0x18
    DateTime m_receptiontime; // 0x20
    u32 m_uiLifeTime; // 0x28
    u32 m_uiFlags; // 0x2C
    String m_strSubject; // 0x30
    String m_strSender; // 0x38
};
ASSERT_OFFSET(_DDL_UserMessage, m_StructureVersion, 0x05);
ASSERT_OFFSET(_DDL_UserMessage, m_receptiontime, 0x20);
ASSERT_OFFSET(_DDL_UserMessage, m_strSender, 0x38);
ASSERT_SIZE(_DDL_UserMessage, 0x40);
} // namespace nex
} // namespace nn
