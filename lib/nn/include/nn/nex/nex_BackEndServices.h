#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex15BackEndServicesE @ 0x008CE46C
// vtable 0x008FCE44 (vptr 0x008FCE4C), offset_to_top 0, 5 entries
class BackEndServices : public ::nn::nex::RootObject
{
public:
    BackEndServices(); // ctor candidate(s) 0x00377100 (unverified)
    virtual ~BackEndServices(); // 0x00377288 slot 0x00 | fefates:callseq
    // 0x00377274 slot 0x04 | slot vf_0x04 of nn::nex::BackEndServices (deleting dtor)
    virtual void PostLogoutCleanup(); // 0x003765D4 slot 0x08 | fefates:bytes
    virtual void RegisterServerProtocols(); // 0x00376964 slot 0x0C | fefates:bytes
    virtual void UnregisterServerProtocols(); // 0x003769C0 slot 0x10 | fefates:bytes
    void LogoutImpl(nn::nex::CallContext*, nn::nex::Credentials*); // 0x00376098 | fefates:bytes [tier B]
    void RegisterPreTerminateCallback(void(*)(nn::nex::CallContext*, const nn::nex::UserContext*), const nn::nex::UserContext&); // 0x00376C00 | mk7dlp:bytes [tier A]
    void Terminate(nn::nex::CallContext*); // 0x0037708C | fefates:bytes [tier B]
    void LoginJobIsInProgress() const; // 0x0072B5F8 | fefates:bytes [tier B]
    void GetAuthenticationClient() const; // 0x0072B614 | fefates:bytes [tier B]
    void TerminateJobIsInProgress() const; // 0x0072B660 | fefates:bytes [tier B]
    void GetSecureConnectionClient() const; // 0x0072B67C | fefates:bytes [tier B]

    // (only the members pia uses; the names are ours)
    u8 m_Unknown0x4[0x74];                 // 0x004
    // the credentials of the game (GetSecureConnectionClient; pia::inet::NexFacade::Bind)
    Credentials* m_pCredentials;           // 0x078
    u8 m_Unknown0x7C[0xA8];                // 0x07C
    NgsBridgeInterface* m_pNgsBridge;      // 0x124
};
ASSERT_OFFSET(BackEndServices, m_pNgsBridge, 0x124);
} // namespace nex
} // namespace nn
