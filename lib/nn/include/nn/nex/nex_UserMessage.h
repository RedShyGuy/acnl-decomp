#pragma once

#include "decomp.h"
#include "nn/nex/nex__DDL_UserMessage.h"
#include "nn/nex/nex_StringStream.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex11UserMessageE @ 0x008CE0A4
// vtable 0x008FC490 (vptr 0x008FC498), offset_to_top 0, 9 entries
class UserMessage : public ::nn::nex::_DDL_UserMessage
{
public:
    // (inline)
    UserMessage()
    {
        m_uiID = 0;
        m_uiParentID = 0;
        m_pidSender = 0;
        m_idRecipient = 0;
        m_uiRecipientType = 0;
        m_receptiontime = *DateTime::s_pNever;
        m_uiLifeTime = 0;
        m_uiFlags = 0;
        m_strSubject = L"";
    }
    virtual ~UserMessage(); // 0x0035C418 slot 0x00
    // 0x0035C3F0 slot 0x04 (deleting dtor)
    // writes the message into a text (the trace of it is not in this build; the name is ours)
    virtual void Trace() const; // 0x0035C3C0 slot 0x20
};
ASSERT_SIZE(UserMessage, 0x40);

// the fields of the message as text (the name is ours)
nn::nex::StringStream& operator<<(nn::nex::StringStream& stream, const nn::nex::UserMessage& message); // 0x003D9DEC
} // namespace nex
} // namespace nn
