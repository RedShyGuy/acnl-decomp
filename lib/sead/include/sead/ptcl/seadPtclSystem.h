#pragma once

#include "decomp.h"
#include "sead/hostio/seadNode.h"

namespace sead {
namespace ptcl {
// RTTI N4sead4ptcl10PtclSystemE @ 0x008D2024
// vtable 0x00906AAC (vptr 0x00906AB4), offset_to_top 0, 1 entries
class PtclSystem : public ::sead::hostio::Node
{
public:
    PtclSystem(); // ctor address unknown
    virtual void vf_0x00(); // 0x0074EE78 slot 0x00 | virtual slot, introduced by (anonymous namespace)::EditHostIO
    void beginRender(unsigned*, const sead::Matrix44<float>&, const sead::Matrix34<float>&, const sead::Matrix34<float>&); // 0x0054F690 | mk7dlp:bytes-fuzzy [tier B]
    void killEmitter(sead::ptcl::EmitterInstance*); // 0x0054F6EC | mk7dlp:bytes-fuzzy [tier B]
    void removeStripe(sead::ptcl::PtclStripe*); // 0x0054F8A8 | mk7dlp:bytes-fuzzy [tier B]
    void initIBO(void*, int); // 0x00550EAC | mk7dlp:bytes [tier B]
    void calcEditor(); // 0x006D272C | mk7dlp:bytes [tier B]
};
} // namespace ptcl
} // namespace sead
