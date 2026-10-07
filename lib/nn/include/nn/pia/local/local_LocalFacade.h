#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/local/local_LocalNetworkTypes.h"

namespace nn {
namespace pia {
namespace transport {
class StationConnectionInfo;
} // namespace transport
namespace local {
// RTTI N2nn3pia5local11LocalFacadeE @ 0x008CFABC
// vtable 0x0090084C (vptr 0x00900854), offset_to_top 0, 5 entries
//
// The connection of the session layer to the local network (a singleton, like inet::NexFacade):
// Startup gives transport::StationConnectionInfoTable the infos of the local station and of the
// host (the transport id is the extension id of the address) and passes the update events of
// the network to the mesh. The member names are ours.
class LocalFacade : public ::nn::pia::common::RootObject
{
public:
    static LocalFacade* s_pInstance; // 0x00975A70

    static nn::Result CreateInstance(); // 0x00414678 | fefates:bytes [tier B]
    static void DestroyInstance(); // 0x00414710 | fefates:bytes [tier B]

    LocalFacade() : m_IsStarted(false) {}
    virtual nn::Result Startup(); // 0x004147F8 slot 0x00 | fefates:bytes-fuzzy
    virtual void Cleanup(); // 0x004147C8 slot 0x04 | fefates:bytes
    virtual ~LocalFacade(); // 0x0041493C slot 0x08
    // 0x00414938 slot 0x0C (deleting dtor)
    virtual void vf_0x10(); // 0x0072FBAC slot 0x10

    static void LocalFacadeUpdateEventCallback(nn::pia::local::LocalUpdateEvent event, u8 nodeId, void* pArg); // 0x00414740 | fefates:bytes [tier B]
    nn::Result GetHostStationConnectionInfo(nn::pia::transport::StationConnectionInfo* pInfo) const; // 0x0072FAD4 | fefates:bytes [tier B]
    nn::Result ConvertTransportIdToStationConnectionInfo(u8 transportId, nn::pia::transport::StationConnectionInfo* pInfo) const; // 0x0072FB04 | fefates:bytes [tier B]

    bool m_IsStarted; // 0x4
};
ASSERT_SIZE(LocalFacade, 0x8);
} // namespace local
} // namespace pia
} // namespace nn
