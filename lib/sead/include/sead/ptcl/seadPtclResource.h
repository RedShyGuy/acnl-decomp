#pragma once

#include "decomp.h"
#include "sead/hostio/seadNode.h"

namespace sead {
namespace ptcl {
// RTTI N4sead4ptcl12PtclResourceE @ 0x008D2060
// vtable 0x00906B0C (vptr 0x00906B14), offset_to_top 0, 3 entries
class PtclResource : public ::sead::hostio::Node
{
public:
    PtclResource(); // ctor address unknown
    virtual void vf_0x00(); // 0x0074EE78 slot 0x00 | virtual slot, introduced by (anonymous namespace)::EditHostIO
    virtual void vf_0x04(); // 0x00557D7C slot 0x04 | mk7dlp:callseq
    virtual ~PtclResource(); // 0x00557D6C slot 0x08 | slot vf_0x08 of sead::ptcl::PtclResource
    void bindTarget(int, sead::ptcl::ResourceBind*, sead::ptcl::EmitterTblData*, int, const sead::SafeStringBase<char>&, unsigned); // 0x00557578 | mk7dlp:bytes [tier B]
    void unbind(sead::ptcl::ResourceBind*, bool, bool); // 0x00557988 | mk7dlp:bytes [tier B]
};
} // namespace ptcl
} // namespace sead
