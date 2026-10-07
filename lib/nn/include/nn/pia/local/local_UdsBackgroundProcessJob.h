#pragma once

#include "decomp.h"
#include "nn/pia/local/local_LocalBackgroundProcessJob.h"
#include "nn/pia/local/local_LocalNetworkTypes.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local23UdsBackgroundProcessJobE @ 0x008CFC70
// vtable 0x0090101C (vptr 0x00901024), offset_to_top 0, 11 entries
//
// The background job of the network with uds: it keeps a copy of the arguments of the request
// and calls uds in its steps. The layout is from the constructor; the member names are ours.
class UdsBackgroundProcessJob : public ::nn::pia::local::LocalBackgroundProcessJob
{
public:
    // the wait for the event of the creation / connection (in steps of WAIT_INTERVAL_MSEC)
    static const u16 WAIT_INTERVAL_MSEC = 15;
    static const u32 CREATE_NETWORK_WAIT_COUNT_MAX = 200;
    static const u32 CONNECT_NETWORK_WAIT_COUNT_MAX = 300;

    UdsBackgroundProcessJob(); // 0x0041F670 | fefates:bytes [tier B]
    // (the complete destructor is a nop that falls through into the base destructor)
    virtual ~UdsBackgroundProcessJob(); // 0x00420268 slot 0x00
    // 0x0041F6FC slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x00731688 slot 0x14
    virtual nn::Result StartupCreateNetwork(nn::pia::common::CallContext* pCallContext, const nn::pia::local::LocalCreateNetworkSetting* pSetting); // 0x0041F16C slot 0x18
    virtual nn::Result StartupDestroyNetwork(nn::pia::common::CallContext* pCallContext); // 0x0041F41C slot 0x1C | fefates:bytes
    virtual nn::Result StartupScanNetwork(nn::pia::common::CallContext* pCallContext, const nn::pia::local::LocalScanNetworkSetting* pSetting); // 0x0041F0E8 slot 0x20
    virtual nn::Result StartupConnectNetwork(nn::pia::common::CallContext* pCallContext, const nn::pia::local::LocalConnectNetworkSetting* pSetting); // 0x0041F298 slot 0x24
    virtual nn::Result StartupDisconnectNetwork(nn::pia::common::CallContext* pCallContext); // 0x0041F600 slot 0x28 | fefates:bytes

    common::ExecuteResult ScanNetwork(); // 0x0041EA0C
    common::ExecuteResult CreateNetwork(); // 0x0041EBF8 | fefates:bytes [tier B]
    common::ExecuteResult ConnectNetwork(); // 0x0041ED64 | fefates:bytes [tier B]
    common::ExecuteResult DestroyNetwork(); // 0x0041EF58 | fefates:bytes [tier B]
    common::ExecuteResult DisconnectNetwork(); // 0x0041F000 | fefates:bytes [tier B]
    common::ExecuteResult WaitCreateNetworkEvent(); // 0x0041F488
    common::ExecuteResult WaitConnectNetworkEvent(); // 0x0041F544

    nn::pia::local::LocalCreateNetworkSetting m_CreateSetting;   // 0x048
    nn::pia::local::LocalScanNetworkSetting m_ScanSetting;       // 0x224
    nn::pia::local::LocalConnectNetworkSetting m_ConnectSetting; // 0x23C
    u32 m_WaitCount;                                             // 0x340
};
ASSERT_OFFSET(UdsBackgroundProcessJob, m_ScanSetting, 0x224);
ASSERT_OFFSET(UdsBackgroundProcessJob, m_ConnectSetting, 0x23C);
ASSERT_SIZE(UdsBackgroundProcessJob, 0x348);
} // namespace local
} // namespace pia
} // namespace nn
