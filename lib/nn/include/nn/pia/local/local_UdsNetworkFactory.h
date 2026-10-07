#pragma once

#include "decomp.h"
#include "nn/pia/local/local_LocalNetworkFactory.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local17UdsNetworkFactoryE @ 0x008CFB5C
// vtable 0x009009F0 (vptr 0x009009F8), offset_to_top 0, 44 entries
//
// The factory of the UDS network: its session infos and its matchmake session.
class UdsNetworkFactory : public ::nn::pia::local::LocalNetworkFactory
{
public:
    UdsNetworkFactory(); // 0x00416DA0
    // (a nop that falls into the destructor of LocalNetworkFactory)
    virtual ~UdsNetworkFactory(); // 0x004183AC slot 0x00
    // 0x00416DB8 slot 0x04 (deleting dtor)
    virtual nn::pia::session::ISessionInfoList* CreateSessionInfoList(u32 capacity); // 0x00416CCC slot 0x90
    virtual nn::pia::session::CommonMatchmakeSession* CreateMatchmakeSession(); // 0x00416D7C slot 0x94
    virtual u32 GetSessionInfoNumMax(); // 0x007302AC slot 0xA8
    // (empty)
    virtual void vf_0xAC(); // 0x007302B4 slot 0xAC
};
} // namespace local
} // namespace pia
} // namespace nn
