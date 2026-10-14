#include "nn/nex/nex_UserMessage.h"

namespace nn {
namespace nex {
// 0x0035C418 slot 0x00
// 0x0035C3F0 (deleting dtor)
nn::nex::UserMessage::~UserMessage()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x0035C3C0 slot 0x20
void nn::nex::UserMessage::Trace() const
{
    StringStream stream;
    stream << *this;
    // (the trace of the text is removed)
}

// 0x003D9DEC
nn::nex::StringStream& operator<<(nn::nex::StringStream& stream, const nn::nex::UserMessage& message)
{
    stream << L" UserMessage (" << L" ID=" << message.m_uiID << L" ParentID=" << message.m_uiParentID
           << L" RecipientID=" << message.m_idRecipient << L" RecipientType=" << message.m_uiRecipientType
           << L" SenderID=" << message.m_pidSender << L" SenderName=" << message.m_strSender << L" RxTime="
           << message.m_receptiontime << L" LifeTime=" << message.m_uiLifeTime << L" Flags=" << message.m_uiFlags
           << L" Subject=" << message.m_strSubject << L") ";
    return stream;
}

} // namespace nex
} // namespace nn
