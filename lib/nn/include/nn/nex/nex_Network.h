#pragma once

#include "decomp.h"
#include "nn/nex/nex_RefCountedObject.h"
#include "nn/nex/nex_StationURL.h"
#include "nn/nex/nex_qList.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex7NetworkE @ 0x008CF680
// vtable 0x008FFB68 (vptr 0x008FFB70), offset_to_top 0, 2 entries
class Network : public ::nn::nex::RefCountedObject
{
public:
    Network(); // ctor address unknown
    virtual ~Network(); // 0x003D3434 slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
    // 0x003D3404 slot 0x04 | slot vf_0x04 of nn::nex::RefCountedObject (deleting dtor)
    void ReleaseInstance(); // 0x003D2980 | fefates:bytes [tier B]
    static Network* GetInstance(); // 0x003D2828 | fefates:callgraph [tier C]

    // (only the members pia uses; the names are ours)
    u8 m_Unknown0x9[0x6F];             // 0x09
    // the station URLs of the console
    qList<StationURL> m_StationUrls;   // 0x78
    RootTransport* m_pRootTransport;   // 0x90
};
ASSERT_OFFSET(Network, m_pRootTransport, 0x90);
} // namespace nex
} // namespace nn
