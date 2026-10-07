#pragma once

#include "decomp.h"
#include "nn/pia/local/local_LocalMatchmakeSession.h"
#include "nn/pia/local/local_LocalNetworkTypes.h"
#include "nn/pia/local/local_UdsNetworkDescription.h"

namespace nn {
namespace pia {
namespace session {
template <typename T> class SessionInfoList;
} // namespace session
namespace local {
class UdsSessionInfo;

// RTTI N2nn3pia5local19UdsMatchmakeSessionE @ 0x008CFBC8
// vtable 0x00900D1C (vptr 0x00900D24), offset_to_top 0, 45 entries
//
// The matchmake session of the UDS network: it owns the network settings and fills the session
// info list of Session with the found networks that match the search criteria. The layout is
// from the constructor; the member names are ours.
class UdsMatchmakeSession : public ::nn::pia::local::LocalMatchmakeSession
{
public:
    static const u32 SCAN_DESCRIPTION_NUM = 16;

    UdsMatchmakeSession(); // 0x0041AB10
    virtual ~UdsMatchmakeSession(); // 0x0041AC70 slot 0x00
    // 0x0041AC14 slot 0x04 (deleting dtor)
    virtual bool IsBrowseCompleted(); // 0x0041A79C slot 0x18
    virtual nn::pia::session::ISessionInfoList* GetSessionInfoList(); // 0x0041AA20 slot 0x1C
    virtual bool IsCreateCompleted(u32* pSessionId); // 0x0041A98C slot 0x24
    virtual void SetCreateSessionSetting(const nn::pia::local::LocalCreateSessionSetting* pSetting); // 0x0041AA28 slot 0xA8
    virtual nn::Result SetJoinSetting(const nn::pia::local::LocalSessionInfo* pInfo, const void* pPassphrase, u32 passphraseSize); // 0x0041AAAC slot 0xAC
    virtual nn::pia::local::LocalConnectNetworkSetting* GetConnectNetworkSetting(); // 0x007311D4 slot 0xB0

    nn::pia::local::LocalConnectNetworkSetting m_ConnectNetworkSetting;        // 0x074, of the join
    nn::pia::local::UdsNetworkDescription m_NetworkDescription;                // 0x178, of the join
    nn::pia::session::SessionInfoList<nn::pia::local::UdsSessionInfo>* m_pSessionInfoList; // 0x284, Session's
    nn::pia::local::UdsNetworkDescription m_ScanDescriptions[SCAN_DESCRIPTION_NUM]; // 0x288, of the browse
};
ASSERT_OFFSET(UdsMatchmakeSession, m_NetworkDescription, 0x178);
ASSERT_OFFSET(UdsMatchmakeSession, m_ScanDescriptions, 0x288);
ASSERT_SIZE(UdsMatchmakeSession, 0x1348);
} // namespace local
} // namespace pia
} // namespace nn
