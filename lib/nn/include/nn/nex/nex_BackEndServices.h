#pragma once

#include "decomp.h"
#include "nn/nex/nex_CallContext.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_StationURL.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex_UserContext.h"
#include "nn/nex/nex_qResult.h"

namespace nn {
namespace nex {
class AuthenticationClient;
class Credentials;
class Data;
class Job;
class NintendoNotificationEventManager;
class NotificationEventManager;
class RendezVousLoginOperation;
class RendezVousLogoutOperation;
class SecureConnectionClient;
class StreamManager;
template <typename T0, typename T1>
class AnyObjectHolder;

// The login to the back end of a game (the authentication server, then the secure server) and the
// logout and termination. The names of the members and of the functions without a reference name
// are ours.
// RTTI N2nn3nex15BackEndServicesE @ 0x008CE46C
// vtable 0x008FCE44 (vptr 0x008FCE4C), offset_to_top 0, 5 entries
class BackEndServices : public ::nn::nex::RootObject
{
public:
    // the function that runs before the termination (it completes the call context)
    typedef void (*PreTerminateFunction)(nn::nex::CallContext* pContext, const nn::nex::UserContext* pUserContext);
    // the function that creates the stream manager of the login (null: a ClientStreamManager)
    typedef nn::nex::StreamManager* (*StreamManagerFactory)();

    // (inline) the pre-terminate callback; the destructor clears the flag
    struct PreTerminateCallback
    {
        PreTerminateCallback() : m_IsSet(false) { m_Context.m_Value = 0; }
        ~PreTerminateCallback() { m_IsSet = false; }

        PreTerminateFunction m_pFunction; // 0x0 (not set by the constructor)
        UserContext m_Context;            // 0x4
        bool m_IsSet;                     // 0x8
    };

    BackEndServices(); // 0x00377100
    virtual ~BackEndServices(); // 0x00377288 slot 0x00 | fefates:callseq
    // 0x00377274 slot 0x04 (deleting dtor)
    virtual void PostLogoutCleanup(); // 0x003765D4 slot 0x08 | fefates:bytes
    virtual void RegisterServerProtocols(); // 0x00376964 slot 0x0C | fefates:bytes
    virtual void UnregisterServerProtocols(); // 0x003769C0 slot 0x10 | fefates:bytes
    // false if a logout is in progress or the call does not start
    bool LogoutImpl(nn::nex::CallContext* pContext, nn::nex::Credentials* pCredentials); // 0x00376098 | fefates:bytes [tier B]
    // the login operation completes with the call (the operations are only created)
    void OnLoginStart(nn::nex::CallContext& context); // 0x0037626C | fefates:callgraph [tier C]
    void OnLogoutStart(nn::nex::CallContext& context); // 0x00376350 | fefates:callgraph [tier C]
    // the termination job is queued (false if one is in progress)
    bool TerminateImpl(); // 0x0037643C | fefates:callgraph [tier C]
    void RegisterPreTerminateCallback(void (*pFunction)(nn::nex::CallContext*, const nn::nex::UserContext*), const nn::nex::UserContext& context); // 0x00376C00 | mk7dlp:bytes [tier A]
    // false if a job is in progress
    bool Logout_(nn::nex::CallContext* pContext, nn::nex::Credentials* pCredentials); // 0x00376C78 | fefates:callgraph [tier C]
    bool LoginImpl(nn::nex::CallContext* pContext, nn::nex::qResult* pResult, const wchar_t* pName, const char* pPassword, const wchar_t* pAddress, unsigned short port,
                   nn::nex::Credentials** ppCredentials, nn::nex::String* pServerName, nn::nex::AnyObjectHolder<nn::nex::Data, nn::nex::String>* pData); // 0x00376DA8 | fefates:callgraph [tier C]
    bool Terminate(nn::nex::CallContext* pContext); // 0x0037708C | fefates:bytes [tier B]
    // whether the login completed and the secure connection is up
    nn::nex::qResult IsConnected() const; // 0x0072B468
    bool LoginJobIsInProgress() const; // 0x0072B5F8 | fefates:bytes [tier B]
    nn::nex::AuthenticationClient* GetAuthenticationClient() const; // 0x0072B614 | fefates:bytes [tier B]
    bool TerminateJobIsInProgress() const; // 0x0072B660 | fefates:bytes [tier B]
    // the handler of the notification events (false without a manager)
    bool RegisterNotificationEventHandler(nn::nex::NotificationEventHandler* pHandler); // 0x00376C20 (name is ours)
    bool UnregisterNotificationEventHandler(nn::nex::NotificationEventHandler* pHandler); // 0x00376C3C (name is ours)
    nn::nex::SecureConnectionClient* GetSecureConnectionClient() const; // 0x0072B67C | fefates:bytes [tier B]
    // the text of a result: "name - (Facility:..., ErrorCode:...) = 0x..." (used by the game)
    static nn::nex::String GetResultString(const nn::nex::String& name, nn::nex::qResult result); // 0x00376634
    // the key of the streams (StreamManager); armlink placed it among the functions of StreamManager
    static void SetSandboxAccessKey(const nn::nex::String& key); // 0x0036E674

    static wchar_t s_SandboxAccessKey[128];

    PreTerminateCallback m_PreTerminateCallback;     // 0x004
    CallContext m_PreTerminateCallContext;           // 0x010
    CallContext* m_pTerminateContext;                // 0x070 (of the caller of Terminate)
    s32 m_LoginTimeout;                              // 0x074 (milliseconds left for the login; -1: none)
    // the credentials of the game (GetSecureConnectionClient; pia::inet::NexFacade::Bind)
    Credentials* m_pCredentials;                     // 0x078
    u16 m_Port;                                      // 0x07C (of the stream manager)
    u8 m_StreamPort;                                 // 0x07E
    u32 m_StreamFlags;                               // 0x080 (StreamManager::Initialize)
    NotificationEventManager* m_pNotificationEventManager;                 // 0x084
    NintendoNotificationEventManager* m_pNintendoNotificationEventManager; // 0x088
    u8 m_Unknown0x8C;                                // 0x08C
    StreamManagerFactory m_pCreateStreamManager;     // 0x090
    Job* m_pLoginJob;                                // 0x094
    Job* m_pLogoutJob;                               // 0x098
    Job* m_pTerminateJob;                            // 0x09C
    s32 m_LoginJobTimeout;                           // 0x0A0 (0: m_LoginTimeout)
    RendezVousLoginOperation* m_pLoginOperation;     // 0x0A4
    RendezVousLogoutOperation* m_pLogoutOperation;   // 0x0A8
    bool m_UpdatesStationURLs;                       // 0x0AC (the login updates the URLs of the Network)
    StationURL m_URL;                                // 0x0B0 (the public URL of the registration)
};
ASSERT_OFFSET(BackEndServices, m_pTerminateContext, 0x70);
ASSERT_OFFSET(BackEndServices, m_pCreateStreamManager, 0x90);
ASSERT_OFFSET(BackEndServices, m_URL, 0xB0);
ASSERT_SIZE(BackEndServices, 0x118);
} // namespace nex
} // namespace nn
