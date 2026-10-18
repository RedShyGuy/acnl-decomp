#pragma once

#include "decomp.h"
#include "nn/nex/nex_JobLogin.h"

namespace nn {
namespace nex {
// A login job with the data of a Nintendo Network ID (a token, a password and the game server).
// armlink kept only its steps (no constructor, no vtable, no RTTI); the class name is from the
// fefates symbols, the names of the members are ours.
class JobNNIDLogin : public ::nn::nex::JobLogin
{
public:
    // the login data (the layout is from StepGameLogin; the names are ours)
    struct LoginData
    {
        char m_Token[0x204];        // 0x000
        char m_Password[0x44];      // 0x204
        char m_ServerAddress[0x10]; // 0x248
        u16 m_ServerPort;           // 0x258
    };

    void StepGameLogin(); // 0x0035D194 | fefates:bytes [tier B]
    void StepWaitingForGameLogin(); // 0x0035D3AC | fefates:bytes [tier B]

    LoginData* m_pLoginData; // 0x118
    u32 m_PrincipalId;       // 0x11C
};
ASSERT_OFFSET(JobNNIDLogin, m_pLoginData, 0x118);
} // namespace nex
} // namespace nn
