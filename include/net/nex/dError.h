#pragma once

#include "decomp.h"
#include "net/nex/dCommand.h"

namespace net {
namespace nex {
// The worst error of the network layer: its level (0 none, 1 server, 2 connection, 3 login, ... 4 and
// higher stay), an error code to show, the command and the type (names after libgarden: Error, Set;
// the others are ours)
class Error
{
public:
    Error(); // 0x0051B790
    DECOMP_EMIT_COPY DECOMP_NOINLINE void Clear() // 0x00138874
    {
        m_Level = 0;
        m_ErrorCode = 0;
        m_Command = Command::NONE;
        m_Type = 6;
    }
    // the error of a command (level 1 if there was none)
    void Set(Command::ID command, u8 type = 6); // 0x0051B740 | libgarden
    // a higher level replaces the error; the same level fills a missing error code
    void Raise(u8 level, u32 errorCode); // 0x0051B764
    // clears the error unless its level is 4 or higher
    DECOMP_EMIT_COPY DECOMP_NOINLINE void ClearRecoverable() // 0x0013884C
    {
        if (m_Level < 4) {
            Clear();
        }
    }
    // takes the error if it is worse
    DECOMP_EMIT_COPY DECOMP_NOINLINE void Merge(const Error& other) // 0x00132B88
    {
        if (other.m_Level > m_Level) {
            m_Level = other.m_Level;
            m_ErrorCode = other.m_ErrorCode;
            m_Command = other.m_Command;
            m_Type = other.m_Type;
        }
    }

    u8 m_Level;           // 0x0
    u32 m_ErrorCode;      // 0x4
    Command::ID m_Command; // 0x8
    u8 m_Type;            // 0x9
};
ASSERT_SIZE(Error, 0xC);
} // namespace nex
} // namespace net
