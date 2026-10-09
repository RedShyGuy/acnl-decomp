#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/boss/detail/boss_Privileged.h"
#include "nn/boss/detail/boss_User.h"
#include "nn/os/os_IpcSession.h"

namespace nn {
namespace boss {
namespace detail {
// RTTI N2nn4boss6detail10IpcManagerE @ 0x008D039C
// The sessions to the service (one object, s_IpcManager; the constructor is inline in the static
// initializer of boss_IpcManager.cpp, 0x007998DC; the member names are ours).
class IpcManager
{
public:
    IpcManager() : m_IsUserInitialized(false), m_IsPrivilegedInitialized(false), m_Unknown06(false), m_Unknown28(0), m_ProgramId(0) {}
    virtual ~IpcManager();

    nn::Result FinalizeUserIpc(); // 0x0046CF24 | nintendogs:bytes [tier A]
    nn::Result InitializeUserIpc(); // 0x0046CF6C | nintendogs:bytes [tier A]

    bool m_IsUserInitialized;                    // 0x04
    bool m_IsPrivilegedInitialized;              // 0x05
    bool m_Unknown06;                            // 0x06
    nn::os::ipc::Session m_UserSession;          // 0x08, boss:U
    nn::os::ipc::Session m_PrivilegedSession;    // 0x0C, boss:P (never opened)
    nn::os::ipc::Session m_Session10;            // 0x10
    nn::boss::detail::User m_User;               // 0x14
    nn::boss::detail::Privileged m_Privileged;   // 0x18
    u32 m_Unknown28;                             // 0x1C
    u64 m_ProgramId;                             // 0x20, 0: this program
};
ASSERT_SIZE(IpcManager, 0x28);
} // namespace detail
} // namespace boss
} // namespace nn
