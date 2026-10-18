#pragma once

#include "decomp.h"
#include "nn/friends/CTR/friends_Types.h"
#include "nn/nex/nex_JobLogin.h"
#include "nn/os/os_Event.h"

namespace nn {
namespace nex {
// The login of a 3DS: the game server address and token from the friends server (game
// authentication), then the login to the game server, and optionally a service token (service
// locator). The names of the members and of the steps without a reference name are ours.
// RTTI N2nn3nex11JobCTRLoginE @ 0x008CE024
// vtable 0x008FC2BC (vptr 0x008FC2C4), offset_to_top 0, 14 entries
class JobCTRLogin : public ::nn::nex::JobLogin
{
public:
    // the result of the game server that asks for a new game authentication
    static const int RESULT_AUTHENTICATION_EXPIRED = static_cast<int>(0x80680006);

    // (inline)
    explicit JobCTRLogin(const DebugString& debugString) : JobLogin(debugString), m_IsEventInitialized(false), m_ServiceLocatorData() {}
    virtual ~JobCTRLogin(); // 0x003586D4 slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
    // 0x00358668 slot 0x04 | fefates:bytes (deleting dtor)
    virtual void StepFirst(); // 0x0035857C slot 0x34 | fefates:bytes
    void InitializeEvent(); // 0x00357C40 | fefates:bytes [tier B]
    void StepLogin(); // 0x00357920 (name is ours)
    void StepWaitingForGameLogin(); // 0x00357CDC | fefates:bytes [tier B]
    void StepGetGameAuthentication(); // 0x00357DBC | fefates:bytes [tier B]
    void StepRequestAuthenticationToken(); // 0x00357F2C | fefates:bytes [tier B]
    void StepWaitingForGameAuthentication(); // 0x003580B8 (name is ours)
    void StepWaitingForRequestAuthenticationToken(); // 0x00358324 | fefates:bytes [tier B]

    nn::os::Event m_Event;                              // 0x118 (of the friends requests)
    bool m_IsEventInitialized;                          // 0x11C
    nnfriesndsServiceLocatorData m_ServiceLocatorData;  // 0x120
};
ASSERT_OFFSET(JobCTRLogin, m_ServiceLocatorData, 0x120);
ASSERT_SIZE(JobCTRLogin, 0x2B8);
} // namespace nex
} // namespace nn
