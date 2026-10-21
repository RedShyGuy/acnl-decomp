#pragma once

#include "decomp.h"
#include "nn/nex/nex_AuthenticationInfo.h"
#include "nn/nex/nex_CustomDataHolder.h"
#include "nn/nex/nex_RendezVous.h"

namespace nn {
namespace nex {
class JobLogin;
class NgsBridgeInterface;
class ProtocolCallContext;

// The back end of a 3DS game: the login with the friends server (JobCTRLogin) and the bridge for
// pia. The names of the members and of the functions without a reference name are ours.
// RTTI N2nn3nex9NgsFacadeE @ 0x008CF768
// vtable 0x008FFD40 (vptr 0x008FFD48), offset_to_top 0, 5 entries
class NgsFacade : public ::nn::nex::RendezVous
{
public:
    // the error code bases of GetLastLoginErrorCode and the game server ids of the sample server
    struct LoginErrorCodes
    {
        u32 m_ResponseResultBase;   // 0x0 (the HTTP status was 200)
        u32 m_HttpStatusCodeBase;   // 0x4
        u32 m_SampleServerIdFirst;  // 0x8
        u32 m_SampleServerIdLast;   // 0xC
    };
    static LoginErrorCodes s_LoginErrorCodes;

    NgsFacade(); // 0x003D725C | fefates:bytes-fuzzy [tier B]
    virtual ~NgsFacade(); // 0x003D73D0 slot 0x00 | fefates:bytes
    // 0x003D73C0 (deleting dtor)
    // the pre-terminate callback (the context is the facade)
    static void OnPreTerminate(nn::nex::CallContext* pContext, const nn::nex::UserContext* pUserContext); // 0x003D6B28
    // the terminate job of the facade is queued (it waits for the login job)
    void StartTerminate(nn::nex::CallContext* pContext); // 0x003D6B38
    nn::nex::qResult LoginAndRequestAuthenticationTokenImpl(nn::nex::ProtocolCallContext* pContext, unsigned int gameServerId, const wchar_t* pAccessKey, const char* pKeyHash,
                                                            char* pServiceToken, const void* pUnsupported, int timeout, unsigned int principalId); // 0x003D6CC4 | fefates:bytes [tier B]
    nn::nex::qResult Login(nn::nex::ProtocolCallContext* pContext, unsigned int gameServerId, const wchar_t* pAccessKey, int timeout, unsigned int principalId); // 0x003D7164 | fefates:bytes [tier B]
    nn::nex::qResult Logout(nn::nex::ProtocolCallContext* pContext); // 0x003D719C | fefates:bytes [tier B]
    u32 GetLastLoginErrorCode() const; // 0x0072EA08 | fefates:bytes [tier B]

    u32 m_Unknown0x118;                     // 0x118
    JobLogin* m_pLoginJob;                  // 0x11C
    Job* m_pTerminateJob;                   // 0x120
    NgsBridgeInterface* m_pNgsBridge;       // 0x124
    s32 m_LastResponseResult;               // 0x128 (of the friends server; JobCTRLogin)
    s32 m_LastHttpStatusCode;               // 0x12C
    qResult m_LoginResult;                  // 0x130 (JobLogin::Complete)
    u32 m_Unknown0x13C;                     // 0x13C (the error code of the login, if not 0)
    bool m_Unknown0x140;                    // 0x140 (no server version in the authentication info)
    bool m_Unknown0x141;                    // 0x141 (no check of the friends login)
    bool m_Unknown0x142;                    // 0x142
    CustomDataHolder<AuthenticationInfo> m_AuthenticationInfo; // 0x144
};
ASSERT_OFFSET(NgsFacade, m_pNgsBridge, 0x124);
ASSERT_OFFSET(NgsFacade, m_LoginResult, 0x130);
ASSERT_OFFSET(NgsFacade, m_AuthenticationInfo, 0x144);
ASSERT_SIZE(NgsFacade, 0x150);
} // namespace nex
} // namespace nn
