#pragma once

#include "decomp.h"
#include "sead/hostio/seadNode.h"

namespace sead {
namespace ptcl {
// RTTI N4sead4ptcl12PtclModelMgrE @ 0x008D2054
// vtable 0x00906AF8 (vptr 0x00906B00), offset_to_top 0, 3 entries
class PtclModelMgr : public ::sead::hostio::Node
{
public:
    PtclModelMgr(); // ctor address unknown
    virtual void vf_0x00(); // 0x0074EE78 slot 0x00 | virtual slot, introduced by (anonymous namespace)::EditHostIO
    virtual ~PtclModelMgr(); // 0x005561BC slot 0x04 | slot vf_0x04 of sead::ptcl::PtclModelMgr
    // 0x005561AC slot 0x08 | slot vf_0x08 of sead::ptcl::PtclModelMgr (deleting dtor)
};
} // namespace ptcl
} // namespace sead
