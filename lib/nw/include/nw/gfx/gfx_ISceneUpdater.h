#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_GfxObject.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx13ISceneUpdaterE @ 0x008D05E0
class ISceneUpdater : public ::nw::gfx::GfxObject
{
public:
    struct DepthSortMode { u32 _unknown; }; // TODO: real type unknown (placeholder)
    struct RenderSortMode { u32 _unknown; }; // TODO: real type unknown (placeholder)
    ISceneUpdater(); // ctor address unknown
};
} // namespace gfx
} // namespace nw
