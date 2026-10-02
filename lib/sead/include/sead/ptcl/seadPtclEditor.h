#pragma once

#include "decomp.h"
#include "sead/hostio/seadNode.h"
#include "sead/hostio/seadPaletteEventListener.h"

namespace sead {
namespace ptcl {
// RTTI N4sead4ptcl10PtclEditorE @ 0x008D2004
// vtable 0x00906A94 (vptr 0x00906A9C), offset_to_top 0, 4 entries
class PtclEditor : public ::sead::hostio::Node, public ::sead::hostio::PaletteEventListener
{
public:
    PtclEditor(); // ctor address unknown
    virtual void vf_0x00(); // 0x0074EE78 slot 0x00 | virtual slot, introduced by (anonymous namespace)::EditHostIO
    virtual ~PtclEditor(); // 0x0054EF04 slot 0x04 | slot vf_0x04 of sead::ptcl::PtclEditor
    // 0x0054EEF4 slot 0x08 | slot vf_0x08 of sead::ptcl::PtclEditor (deleting dtor)
    virtual void vf_0x0C(); // 0x0054EB44 slot 0x0C | virtual slot, introduced by sead::ptcl::PtclEditor
    void stopPreview(bool); // 0x0054E778 | mk7dlp:bytes [tier B]
    void clearResource(sead::ptcl::PtclResource*); // 0x0054E80C | mk7dlp:bytes [tier B]
};
} // namespace ptcl
} // namespace sead
