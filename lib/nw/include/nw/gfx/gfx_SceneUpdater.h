#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_ISceneUpdater.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx12SceneUpdaterE @ 0x008D05B0
// vtable 0x0090242C (vptr 0x00902434), offset_to_top 0, 9 entries
class SceneUpdater : public ::nw::gfx::ISceneUpdater
{
public:
    class Builder;
    SceneUpdater(); // ctor candidate(s) 0x00491864 (unverified)
    virtual ~SceneUpdater(); // 0x00491DBC slot 0x00 | nintendogs:bytes
    virtual void vf_0x04(); // 0x00491CF4 slot 0x04 | virtual slot, introduced by nw::gfx::SceneUpdater
    virtual void vf_0x08(); // 0x007386CC slot 0x08 | virtual slot, introduced by nw::gfx::SceneUpdater
    virtual void vf_0x0C(); // 0x007386C4 slot 0x0C | virtual slot, introduced by nw::gfx::SceneUpdater
    virtual void SetDepthSortMode(nw::gfx::ISceneUpdater::DepthSortMode); // 0x0049185C slot 0x10 | slot vf_0x10 of nw::gfx::SceneUpdater
    virtual void vf_0x14(); // 0x00491B38 slot 0x14 | virtual slot, introduced by nw::gfx::SceneUpdater
    virtual void SubmitView(nw::gfx::BasicRenderQueue<nw::gfx::BasicRenderElement<unsigned long long>, nw::ut::MoveArray<nw::gfx::BasicRenderElement<unsigned long long>>, nw::gfx::BasicRenderKeyFactory<unsigned long long>>*, nw::gfx::SceneContext*, const nw::gfx::Camera&, unsigned char, nw::gfx::ISceneUpdater::RenderSortMode); // 0x00491314 slot 0x18 | nintendogs:bytes
    virtual void vf_0x1C(); // 0x00491368 slot 0x1C | virtual slot, introduced by nw::gfx::SceneUpdater
    virtual void vf_0x20(); // 0x004913BC slot 0x20 | virtual slot, introduced by nw::gfx::SceneUpdater
};
} // namespace gfx
} // namespace nw
