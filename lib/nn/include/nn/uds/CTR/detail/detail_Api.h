#pragma once

// The state of nn::uds and its core functions. In the binary all of nn::uds::CTR (this file,
// CTR_Api.cpp, NetworkDescription and the readers) is one translation unit, uds_Api.cpp
// (static initializer __sti___11_uds_Api_cpp); the shared variables are defined in
// detail_Api.cpp. Variable and member names are ours.

#include "decomp.h"
#include "nn/Handle.h"
#include "nn/Result.h"
#include "nn/cfg/CTR/cfg_Types.h"
#include "nn/err/CTR/CTR_Api.h"
#include "nn/os/os_CriticalSection.h"
#include "nn/os/os_Event.h"
#include "nn/os/os_IpcSession.h"
#include "nn/os/os_TransferMemoryBlock.h"
#include "nn/uds/CTR/uds_NetworkDescription.h"
#include "nn/uds/CTR/uds_Result.h"
#include "nn/uds/CTR/uds_Types.h"
#include "nn/uds/CTR/detail/uds_Uds.h"

namespace nn {
namespace uds {
namespace CTR {
namespace detail {

// an endpoint of CreateEndpoint; Attach binds it and gets the event of received data
struct EndpointEntry
{
    EndpointEntry(); // 0x00469EA0
    ~EndpointEntry(); // 0x00469EAC

    nn::Handle event;   // 0x0
    u32 id;             // 0x4 EndpointDescriptor::id, 0 = free
    bool isAttached;    // 0x8
};
ASSERT_SIZE(EndpointEntry, 0xC);

const s32 ENDPOINT_MAX = 16;

// the version InitializeWithVersion reports to the service
const u16 UDS_VERSION = 0x400;

extern u8 s_ChannelForCreateNetwork;            // 0 = the caller's channel
extern u8 s_ChannelForStartScan;                // 0 = the caller's channel
extern bool s_IsInitialized;
extern u32 s_LastEndpointId;
extern nn::os::ipc::Session s_ReceiveSession;   // for PullPacket
extern nn::os::ipc::Session s_Session;          // all other commands
extern nn::os::Event* s_pStatusEvent;           // the caller's event of Initialize
extern NetworkDescription s_NetworkDescription; // of CreateNetwork
extern nn::os::CriticalSection s_CriticalSection;
extern EndpointEntry s_Endpoints[ENDPOINT_MAX];
extern nn::os::TransferMemoryBlock s_TransferMemory;
// work memory of NetworkDescriptionReader::GetNodeInformationList
extern NodeInformationList s_NodeInformationList;
extern NetworkDescription s_ReaderNetworkDescription;
extern NodeInformationElement s_ReaderNodeInformationElements[2];

inline Uds GetUds()
{
    return Uds(s_Session.GetHandle());
}

// a session closed by the service is fatal
inline void ThrowIfSessionClosed(nn::Result result)
{
    if (result == nn::Result(RESULT_SESSION_CLOSED)) {
        nn::err::CTR::ThrowFatalErrAllIfFailure(result);
    }
}

nn::Result GetMacAddress(u8* mac); // 0x004693A8 | fefates:bytes [tier B]
// buffer / bufferSize: memory for the service (transfer memory)
nn::Result InitializeCore(nn::os::Event* statusEvent, void* buffer, size_t bufferSize,
                          const nn::cfg::CTR::UserName* userName); // 0x004693F0 | mk7dlp:callseq [tier A]
void FinalizeCore(); // 0x004692A8 | tier C
nn::Result DestroyEndpoint(EndpointDescriptor* endpoint, ReceiveReport* report); // 0x0046962C | tier C
void ScrambleLocalFriendCode(ScrambledLocalFriendCode* scrambled, u64 localFriendCodeSeed,
                             u16 nodeId); // 0x00469764 | fefates:bytes [tier B]

} // namespace detail
} // namespace CTR
} // namespace uds
} // namespace nn
