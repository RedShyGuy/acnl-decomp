#pragma once

#include "decomp.h"

namespace nw {
namespace gfx {
class SceneContext;
class SceneEnvironment;
} // namespace gfx
} // namespace nw
#include "Other/dBase.h"

// RTTI 10BsLightMgr @ 0x008CAF78
// vtable 0x008EBD74 (vptr 0x008EBD7C), offset_to_top 0, 16 entries
class BsLightMgr : public ::Base
{
public:
    class LightMgrHioNode;
    BsLightMgr(); // ctor candidate(s) 0x00190F20 (unverified)
    virtual ~BsLightMgr(); // 0x00190FBC slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x00190F98 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual oml::framework::Result Initialize(); // 0x00190DB8 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual oml::framework::Result Finalize(); // 0x00190EFC slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual oml::framework::Result Calc(); // 0x00190EF4 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual oml::framework::Result Draw(); // 0x00190D58 slot 0x30 | slot vf_0x30 of oml::framework::Process

    // (names are ours) the instance (NULL: none)
    static BsLightMgr* GetInstance(); // 0x0019023C
    // the light sets of the game for the scene instead of the ones of g3d
    void SetupLightSets(nw::gfx::SceneContext* sceneContext, nw::gfx::SceneEnvironment* environment); // 0x00190360
};
