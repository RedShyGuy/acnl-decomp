#include "net/nex/dError.h"

namespace net {
namespace nex {
// 0x0051B790 (name is ours)
net::nex::Error::Error()
{
    Clear();
}

// 0x0051B740 | libgarden
void net::nex::Error::Set(Command::ID command, u8 type)
{
    m_Command = command;
    m_Type = type;
    if (m_Level < 1) {
        m_Level = 1;
        m_ErrorCode = 0;
    }
}

// 0x0051B764 (name is ours)
void net::nex::Error::Raise(u8 level, u32 errorCode)
{
    if (m_Level < level) {
        m_Level = level;
        m_ErrorCode = errorCode;
    } else if (m_Level == level && m_ErrorCode == 0 && errorCode != 0) {
        m_ErrorCode = errorCode;
    }
}
} // namespace nex
} // namespace net
