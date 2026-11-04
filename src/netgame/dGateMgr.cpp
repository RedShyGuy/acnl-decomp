#include "netgame/dGateMgr.h"

#include "net/dNet.h"

namespace netgame {
// 0x0095461C
u16 s_GateParam;

namespace {
const s32 CMD_QUEUE_SIZE = 5;
const u8 CMD_FLAG_MASK = 0x1F;
const u8 CMD_OPEN = 0x80;
} // namespace

DECOMP_ALWAYS_INLINE void netgame::GateMgr::EraseCmd(s32 index)
{
    if (index < CMD_QUEUE_SIZE) {
        for (s32 i = index; i < CMD_QUEUE_SIZE - 1; i++) {
            m_CmdQueue[i] = m_CmdQueue[i + 1];
        }
        m_CmdQueue[CMD_QUEUE_SIZE - 1] = 0;
    }
}

// 0x00623948 (name is ours)
void netgame::GateMgr::Process()
{
    if (m_CurrentCmd == 0) {
        u8 cmd = m_CmdQueue[0];
        if (cmd == 0) {
            return;
        }
        m_CurrentCmd = cmd;
        EraseCmd(0);
        if (m_CurrentCmd == 0) {
            return;
        }
    }
    net::Net& net = net::Net::s_Instance;
    if (net.IsError()) {
        m_CurrentCmd = 0;
        return;
    }
    u8 flags = m_CurrentCmd & 0x7F;
    if ((m_CurrentCmd & CMD_OPEN) != 0) {
        u8 remaining = m_CloseFlags & ~flags;
        if (remaining != 0) {
            m_CloseFlags = remaining;
            m_CurrentCmd = 0;
            return;
        }
        // the last reason is gone
        bool setsParam = false;
        u16 param = 0;
        if ((flags & 1) != 0) {
            setsParam = true;
            param = s_GateParam;
        }
        if (net.OpenSession(setsParam, param)) {
            m_CurrentCmd = 0;
            m_CloseFlags &= ~flags;
        }
    } else {
        if (m_CloseFlags == 0 && !net.CloseSession()) {
            return;
        }
        m_CurrentCmd = 0;
        m_CloseFlags |= flags;
    }
}

// 0x00623A78 (name after libgarden)
bool netgame::GateMgr::Open(Flag flag)
{
    if (!net::Net::s_Instance.IsInitialized(false)) {
        return false;
    }
    Enqueue((1 << flag) | CMD_OPEN);
    return true;
}

// 0x00623ABC (name after libgarden)
bool netgame::GateMgr::Close(Flag flag)
{
    if (!net::Net::s_Instance.IsInitialized(false)) {
        return false;
    }
    u8 mask = 1 << flag;
    if ((mask & CMD_FLAG_MASK) == 0) {
        return false;
    }
    Enqueue(mask);
    return true;
}

// 0x00623B04 (name is ours)
bool netgame::GateMgr::Enqueue(u8 cmd)
{
    if ((cmd & CMD_FLAG_MASK) == 0) {
        return false;
    }
    for (s32 i = CMD_QUEUE_SIZE - 1; i >= 0; i--) {
        if ((m_CmdQueue[i] & 0x7F) == (cmd & 0x7F)) {
            // a waiting command for the opposite cancels out
            if ((m_CmdQueue[i] & CMD_OPEN) != (cmd & CMD_OPEN)) {
                EraseCmd(i);
            }
            return true;
        }
    }
    if (m_CurrentCmd == cmd) {
        return true;
    }
    for (s32 i = 0; i < CMD_QUEUE_SIZE; i++) {
        if (m_CmdQueue[i] == 0) {
            m_CmdQueue[i] = cmd;
            return true;
        }
    }
    return false;
}

// 0x00623C40 (name is ours)
void netgame::GateMgr::Reset()
{
    for (s32 i = 0; i < CMD_QUEUE_SIZE; i++) {
        m_CmdQueue[i] = 0;
    }
    m_CurrentCmd = 0;
    m_CloseFlags = 0;
}

// 0x00623C5C (name is ours)
netgame::GateMgr::GateMgr()
{
    m_CurrentCmd = 0;
    m_CloseFlags = 0;
}

// 0x0075F774 (name after libgarden)
bool netgame::GateMgr::IsClosed(Flag flag) const
{
    if (flag > FLAG_UNKNOWN) {
        return false;
    }
    u8 mask = 1 << flag;
    if ((mask & CMD_FLAG_MASK) == 0) {
        return false;
    }
    return (m_CloseFlags & mask) != 0;
}
} // namespace netgame
