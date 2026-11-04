#pragma once

#include "decomp.h"
#include "nn/os/os_CriticalSection.h"
#include "nn/os/os_Event.h"

namespace net {
// The connection to the access point (nn::ac), shared by all users with a reference count. The
// disconnect event tells whether the connection was lost (all names are ours).
class AcConnection
{
public:
    // the error code that the game shows when the connection failed with RESULT_AC_...F84
    static const u32 ERROR_CODE_AC_FAILED = 30399;

    static void Create(); // 0x0050D3EC
    DECOMP_EMIT_COPY static void Destroy() // 0x0012C0A4
    {
        if (s_pInstance != NULL) {
            delete s_pInstance;
            s_pInstance = NULL;
        }
    }
    // connects or takes a reference to the connection; errorCode is the error code of a failure
    static bool Connect(u32* errorCode); // 0x0050D320
    // releases a reference; the last one closes the connection
    static void Disconnect(); // 0x0050D3A4
    // whether the connection is still there (its disconnect event was not signaled)
    static bool IsConnected(); // 0x0050D46C

private:
    static bool ConnectAc(u32* errorCode, nn::os::Event* disconnectEvent); // 0x0050D4C8

    nn::os::CriticalSection m_CriticalSection; // 0x00
    nn::os::Event m_DisconnectEvent;           // 0x0C
    u8 m_RefCount;                             // 0x10
    bool m_IsConnected;                        // 0x11

    static AcConnection* s_pInstance;
};
ASSERT_SIZE(AcConnection, 0x14);
} // namespace net
