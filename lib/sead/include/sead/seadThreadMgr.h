#pragma once

#include "decomp.h"
#include "sead/hostio/seadNode.h"

namespace sead {
// RTTI N4sead9ThreadMgrE @ 0x008D23E4
// vtable 0x00907140 (vptr 0x00907148), offset_to_top 0, 3 entries
class ThreadMgr : public ::sead::hostio::Node
{
public:
    class SingletonDisposer_;
    virtual void vf_0x00(); // 0x0074EE78 slot 0x00 | virtual slot, introduced by (anonymous namespace)::EditHostIO
    virtual ~ThreadMgr(); // 0x00562B14 slot 0x04 | slot vf_0x04 of sead::ThreadMgr
    // 0x00562B04 slot 0x08 | slot vf_0x08 of sead::ThreadMgr (deleting dtor)
    ThreadMgr(); // 0x0053DE74 | nintendogs:bytes [tier A]
    static ThreadMgr* s_pInstance; // 0x009763F0
};
} // namespace sead
