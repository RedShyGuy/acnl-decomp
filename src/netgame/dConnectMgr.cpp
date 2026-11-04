#include "netgame/dConnectMgr.h"

#include "net/dNet.h"
#include "netgame/dNetGameMgr.h"

namespace netgame {
namespace {
const s32 CMD_QUEUE_SIZE = 8;
const u8 CMD_CONNECTION_MASK = 0x3F;
const u8 CMD_DISCONNECT = 0x80;
// the connections over the internet
const u8 INTERNET_MASK = 0x3E;

const u8 BIT_LOCAL = 1 << ConnectMgr::CONNECTION_LOCAL;
const u8 BIT_FRIEND_MATCH = 1 << ConnectMgr::CONNECTION_FRIEND_MATCH;
const u8 BIT_RANDOM_MATCH = 1 << ConnectMgr::CONNECTION_RANDOM_MATCH;
const u8 BIT_BEST_FRIEND = 1 << ConnectMgr::CONNECTION_BEST_FRIEND;
const u8 BIT_DATASTORE = 1 << ConnectMgr::CONNECTION_DATASTORE;
const u8 BIT_INDEPENDENT = 1 << ConnectMgr::CONNECTION_INDEPENDENT;

// the steps for the connection of best friends
enum BestFriendStep
{
    STEP_INITIALIZE,
    STEP_LOGIN,
    STEP_ABORT_INITIALIZE,
    STEP_ABORT_LOGIN,
};

DECOMP_ALWAYS_INLINE ConnectMgr::Connection GetConnection(u8 cmd)
{
    for (s32 i = 0; i < ConnectMgr::CONNECTION_NONE; i++) {
        if ((cmd & (1 << i)) != 0) {
            return static_cast<ConnectMgr::Connection>(i);
        }
    }
    return ConnectMgr::CONNECTION_NONE;
}

DECOMP_ALWAYS_INLINE bool IsReady(ConnectMgr::Connection connection)
{
    if (connection == ConnectMgr::CONNECTION_BEST_FRIEND) {
        return true;
    }
    net::Net& net = net::Net::s_Instance;
    if (connection == ConnectMgr::CONNECTION_INDEPENDENT) {
        return net.IsInitialized(true);
    }
    return net.IsInitialized(false);
}
} // namespace

DECOMP_ALWAYS_INLINE void netgame::ConnectMgr::EraseCmd(s32 index)
{
    if (index < CMD_QUEUE_SIZE) {
        for (s32 i = index; i < CMD_QUEUE_SIZE - 1; i++) {
            m_CmdQueue[i] = m_CmdQueue[i + 1];
        }
        m_CmdQueue[CMD_QUEUE_SIZE - 1] = 0;
    }
}

DECOMP_ALWAYS_INLINE void netgame::ConnectMgr::EraseBestFriendDisconnect()
{
    for (s32 i = 0; i < CMD_QUEUE_SIZE; i++) {
        if (m_CmdQueue[i] == (BIT_BEST_FRIEND | CMD_DISCONNECT)) {
            EraseCmd(i);
            return;
        }
    }
}

DECOMP_ALWAYS_INLINE void netgame::ConnectMgr::FinishCmd()
{
    m_CurrentCmd = 0;
    m_Step = 0;
}

// 0x006297BC (name is ours)
void netgame::ConnectMgr::Clear()
{
    for (s32 i = 0; i < CMD_QUEUE_SIZE; i++) {
        m_CmdQueue[i] = 0;
    }
    m_CurrentCmd = 0;
    m_ConnectedFlags = 0;
    m_Step = 0;
    m_HasNetRef = false;
}

// 0x006297E0 (name is ours)
void netgame::ConnectMgr::Process()
{
    net::Net& net = net::Net::s_Instance;
    if (m_CurrentCmd == 0) {
        u8 cmd = m_CmdQueue[0];
        if (cmd == 0) {
            return;
        }
        m_CurrentCmd = cmd;
        m_Step = 0;
        EraseCmd(0);
        if ((m_CurrentCmd & CMD_DISCONNECT) == 0 && m_ConnectedFlags == 0 && !net.IsLoggedIn(true)) {
            net.ClearError();
        }
        if (m_CurrentCmd == 0) {
            return;
        }
    }
    NetGameMgr* mgr = NetGameMgr::s_pInstance;
    bool isDisconnect = (m_CurrentCmd & CMD_DISCONNECT) != 0;
    switch (GetConnection(m_CurrentCmd)) {
    case CONNECTION_LOCAL:
        if (isDisconnect) {
            if (net.Disconnect()) {
                FinishCmd();
                m_ConnectedFlags &= ~BIT_LOCAL;
            }
        } else if (net.Connect(true)) {
            FinishCmd();
            m_ConnectedFlags |= BIT_LOCAL;
        } else if (net.IsError()) {
            FinishCmd();
        }
        break;
    case CONNECTION_FRIEND_MATCH:
        if (isDisconnect) {
            if ((m_ConnectedFlags & (BIT_RANDOM_MATCH | BIT_BEST_FRIEND)) != 0 || net.Disconnect()) {
                FinishCmd();
                m_ConnectedFlags &= ~BIT_FRIEND_MATCH;
            }
        } else if (net.Connect(false)) {
            FinishCmd();
            m_ConnectedFlags |= BIT_FRIEND_MATCH;
        } else if (net.IsError()) {
            FinishCmd();
        }
        break;
    case CONNECTION_RANDOM_MATCH:
        if (isDisconnect) {
            if ((m_ConnectedFlags & (BIT_FRIEND_MATCH | BIT_BEST_FRIEND)) != 0 || net.Disconnect()) {
                FinishCmd();
                m_ConnectedFlags &= ~BIT_RANDOM_MATCH;
            }
        } else if (net.Connect(false)) {
            FinishCmd();
            m_ConnectedFlags |= BIT_RANDOM_MATCH;
        } else if (net.IsError()) {
            FinishCmd();
        }
        break;
    case CONNECTION_BEST_FRIEND:
        if (isDisconnect) {
            if (m_Step == 0) {
                if (!net.IsInitialized(false)) {
                    m_HasNetRef = false;
                } else {
                    if ((m_ConnectedFlags & (BIT_FRIEND_MATCH | BIT_RANDOM_MATCH)) == 0 && !net.Disconnect()) {
                        return;
                    }
                    if (m_HasNetRef) {
                        mgr->FiniNet();
                        m_HasNetRef = false;
                    }
                }
                if ((m_ConnectedFlags & (BIT_FRIEND_MATCH | BIT_RANDOM_MATCH)) == 0) {
                    m_ConnectedFlags &= ~BIT_BEST_FRIEND;
                    FinishCmd();
                    return;
                }
                if (!net.IsError()) {
                    m_Step++;
                    return;
                }
            } else if (m_Step == 1) {
                if (!mgr->m_FriendListMgr.SendStatus(false, false) && !net.IsError()) {
                    return;
                }
            } else {
                return;
            }
            FinishCmd();
            m_ConnectedFlags &= ~BIT_BEST_FRIEND;
            return;
        }
        switch (m_Step) {
        case STEP_INITIALIZE:
            if (!m_HasNetRef) {
                m_HasNetRef = mgr->InitNetForConnect();
                mgr->OnConnectBestFriend();
            }
            if (net.IsInitialized(false) && net.Connect(false)) {
                m_Step = STEP_LOGIN;
                return;
            }
            if (net.IsError()) {
                m_Step = STEP_ABORT_INITIALIZE;
            }
            break;
        case STEP_LOGIN:
            if (mgr->m_FriendListMgr.SendStatus(true, true)) {
                FinishCmd();
                m_ConnectedFlags |= BIT_BEST_FRIEND;
            } else if (net.IsError()) {
                m_Step = STEP_ABORT_LOGIN;
            }
            break;
        case STEP_ABORT_INITIALIZE:
            if (m_HasNetRef) {
                mgr->FiniNet();
                m_HasNetRef = false;
            }
            EraseBestFriendDisconnect();
            FinishCmd();
            break;
        case STEP_ABORT_LOGIN:
            if (!net.IsInitialized(false)) {
                m_HasNetRef = false;
            } else {
                if ((m_ConnectedFlags & (BIT_FRIEND_MATCH | BIT_RANDOM_MATCH)) == 0 && !net.Disconnect()) {
                    return;
                }
                if (m_HasNetRef) {
                    mgr->FiniNet();
                    m_HasNetRef = false;
                }
            }
            EraseBestFriendDisconnect();
            FinishCmd();
            m_ConnectedFlags &= ~BIT_BEST_FRIEND;
            break;
        }
        break;
    case CONNECTION_DATASTORE:
        if (isDisconnect) {
            if (net.DisconnectDataStore()) {
                FinishCmd();
                m_ConnectedFlags &= ~BIT_DATASTORE;
            }
        } else if (net.ConnectDataStore(0)) {
            FinishCmd();
            m_ConnectedFlags |= BIT_DATASTORE;
        } else if (net.IsError()) {
            FinishCmd();
        }
        break;
    case CONNECTION_INDEPENDENT:
        if (isDisconnect) {
            if (net.DisconnectIndependent()) {
                FinishCmd();
                m_ConnectedFlags &= ~BIT_INDEPENDENT;
            }
        } else if (net.ConnectIndependent(1)) {
            FinishCmd();
            m_ConnectedFlags |= BIT_INDEPENDENT;
        } else if (net.IsError()) {
            FinishCmd();
        }
        break;
    default:
        break;
    }
}

// 0x00629F10 (name after libgarden)
bool netgame::ConnectMgr::Connect(Connection connection)
{
    if (!IsReady(connection)) {
        return false;
    }
    u8 mask = 1 << connection;
    if ((mask & CMD_CONNECTION_MASK) == 0) {
        return false;
    }
    if (IsConnectedOrConnecting(connection)) {
        return true;
    }
    bool isInternet = (mask & INTERNET_MASK) != 0;
    if (m_ConnectedFlags != 0) {
        bool connectedInternet = (m_ConnectedFlags & INTERNET_MASK) != 0;
        if (connectedInternet != isInternet) {
            // the other kind has to go first
            if (isInternet) {
                Disconnect(CONNECTION_LOCAL);
            } else {
                Disconnect(CONNECTION_FRIEND_MATCH);
                Disconnect(CONNECTION_RANDOM_MATCH);
                Disconnect(CONNECTION_BEST_FRIEND);
                Disconnect(CONNECTION_DATASTORE);
                Disconnect(CONNECTION_INDEPENDENT);
                NetGameMgr::s_pInstance->m_BestFriendMgr.Stop();
            }
            return false;
        }
    } else if (m_CurrentCmd != 0) {
        bool currentInternet = (m_CurrentCmd & INTERNET_MASK) != 0;
        if (currentInternet != isInternet) {
            if ((m_CurrentCmd & CMD_DISCONNECT) == 0) {
                return false;
            }
            Enqueue(mask);
            return true;
        }
    }
    if (isInternet) {
        Enqueue(mask);
        return true;
    }
    if (m_CurrentCmd != 0) {
        return m_CurrentCmd == mask;
    }
    m_CurrentCmd = mask;
    m_Step = 0;
    return true;
}

// 0x0062A0B0 (name after libgarden)
bool netgame::ConnectMgr::Disconnect(Connection connection)
{
    if (!IsReady(connection)) {
        return false;
    }
    if (IsConnectedOrConnecting(connection)) {
        u8 mask = 1 << connection;
        u8 cmd = mask | CMD_DISCONNECT;
        if ((mask & INTERNET_MASK) != 0) {
            Enqueue(cmd);
        } else {
            if (m_CurrentCmd != 0) {
                return m_CurrentCmd == cmd;
            }
            m_CurrentCmd = cmd;
            m_Step = 0;
        }
    }
    return true;
}

// 0x0062A168 (name is ours)
bool netgame::ConnectMgr::Enqueue(u8 cmd)
{
    if ((cmd & CMD_CONNECTION_MASK) == 0) {
        return false;
    }
    for (s32 i = CMD_QUEUE_SIZE - 1; i >= 0; i--) {
        if ((m_CmdQueue[i] & 0x7F) == (cmd & 0x7F)) {
            // a waiting command for the opposite cancels out
            if ((m_CmdQueue[i] & CMD_DISCONNECT) != (cmd & CMD_DISCONNECT)) {
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

// 0x0062A280 (name is ours)
void netgame::ConnectMgr::SetPlayerName()
{
    Principal principal;
    principal.SetFromMyself();
    char16 text[20];
    EncodePrincipal(text, &principal);
    net::Net::s_Instance.SetPlayerName(text, 16);
}

// 0x0062A2C8 (name is ours)
netgame::ConnectMgr::ConnectMgr()
{
    m_CurrentCmd = 0;
    m_ConnectedFlags = 0;
    m_Step = 0;
    m_HasNetRef = false;
}

// 0x0076003C (name after libgarden)
bool netgame::ConnectMgr::IsConnectingOrDisconnecting(Connection connection) const
{
    return IsConnectedOrConnecting(connection) || IsCurrentCmd(connection, true) || IsCurrentCmd(connection, false);
}

// 0x007600CC (name is ours)
bool netgame::ConnectMgr::IsConnectedOrConnecting(Connection connection) const
{
    if (connection >= CONNECTION_NONE) {
        return false;
    }
    u8 mask = 1 << connection;
    if ((mask & CMD_CONNECTION_MASK) == 0) {
        return false;
    }
    for (s32 i = 0; i < CMD_QUEUE_SIZE; i++) {
        if ((m_CmdQueue[i] & 0x7F) == mask) {
            return (m_CmdQueue[i] & CMD_DISCONNECT) == 0;
        }
    }
    if (m_CurrentCmd != 0 && (m_CurrentCmd & 0x7F) == mask) {
        return (m_CurrentCmd & CMD_DISCONNECT) == 0;
    }
    return IsConnected(connection);
}

// 0x00760198 (name is ours)
bool netgame::ConnectMgr::IsCurrentCmd(Connection connection, bool isDisconnect) const
{
    if (m_CurrentCmd == 0) {
        return false;
    }
    if (connection >= CONNECTION_NONE) {
        return false;
    }
    u8 mask = 1 << connection;
    if ((mask & CMD_CONNECTION_MASK) == 0 || (mask & m_CurrentCmd) == 0) {
        return false;
    }
    return ((m_CurrentCmd & CMD_DISCONNECT) != 0) == isDisconnect;
}

// 0x007601E0 (name after libgarden)
bool netgame::ConnectMgr::IsConnected(Connection connection) const
{
    if (connection >= CONNECTION_NONE) {
        return false;
    }
    u8 mask = 1 << connection;
    if ((mask & CMD_CONNECTION_MASK) == 0) {
        return false;
    }
    return (m_ConnectedFlags & mask) != 0;
}
} // namespace netgame
