#pragma once

#include "decomp.h"
#include "sead/hostio/seadNode.h"

namespace sead {
// RTTI N4sead8AudioMgrE @ 0x008D217C
// vtable 0x00906CB0 (vptr 0x00906CB8), offset_to_top 0, 3 entries
class AudioMgr : public ::sead::hostio::Node
{
public:
    class SingletonDisposer_;
    AudioMgr(); // ctor address unknown
    virtual void vf_0x00(); // 0x0074EE78 slot 0x00 | virtual slot, introduced by (anonymous namespace)::EditHostIO
    virtual void vf_0x04(); // 0x00560C0C slot 0x04 | virtual slot, introduced by sead::AudioMgr
    virtual void vf_0x08(); // 0x00560B78 slot 0x08 | virtual slot, introduced by sead::AudioMgr
    static AudioMgr* s_pInstance; // 0x009762D8
};
} // namespace sead
