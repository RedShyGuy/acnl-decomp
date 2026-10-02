#pragma once

#include "decomp.h"
#include "sead/hostio/seadNode.h"

namespace sead {
namespace ptcl {
// RTTI N4sead4ptcl11PtclPreviewE @ 0x008D2038
// vtable 0x00906AC4 (vptr 0x00906ACC), offset_to_top 0, 3 entries
class PtclPreview : public ::sead::hostio::Node
{
public:
    PtclPreview(); // ctor address unknown
    virtual void vf_0x00(); // 0x0074EE78 slot 0x00 | virtual slot, introduced by (anonymous namespace)::EditHostIO
    virtual void vf_0x04(); // 0x005556EC slot 0x04 | virtual slot, introduced by sead::ptcl::PtclPreview
    virtual void vf_0x08(); // 0x005556CC slot 0x08 | virtual slot, introduced by sead::ptcl::PtclPreview
    void resetAutoMove_(bool); // 0x005552E0 | mk7dlp:bytes [tier B]
    void applyToEmitterSet(sead::ptcl::PtclProjectEmitterSet*, sead::ptcl::Handle*); // 0x00555390 | mk7dlp:bytes-fuzzy [tier B]
    void start(); // 0x00555518 | mk7dlp:bytes [tier B]
};
} // namespace ptcl
} // namespace sead
