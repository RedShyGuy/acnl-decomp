#pragma once

#include "decomp.h"
#include "Other/dBase.h"

namespace g3d {
namespace modelEffect {
class ModelEffectContainer;
class ModelEffectHandle;
} // namespace modelEffect
} // namespace g3d

// RTTI 16BsModelEffectMgr @ 0x008CC2B4
// vtable 0x008F289C (vptr 0x008F28A4), offset_to_top 0, 16 entries
class BsModelEffectMgr : public ::Base
{
public:
    BsModelEffectMgr(); // ctor candidate(s) 0x002B6A8C (unverified)
    virtual ~BsModelEffectMgr(); // 0x002B6AC0 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x002B6AB0 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual oml::framework::Result Initialize(); // 0x002B6958 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual oml::framework::Result Finalize(); // 0x002B69F0 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual oml::framework::Result Calc(); // 0x002B69AC slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual oml::framework::Result Draw(); // 0x002B6914 slot 0x30 | slot vf_0x30 of oml::framework::Process

    // (names are ours; only the parts g3d uses) the instance (NULL: none)
    static BsModelEffectMgr* GetInstance(); // 0x002B6590
    // a new model effect for the handle (see g3d::modelEffect::ModelEffectContainer::Build); false if there is no
    // free slot or it could not be made
    bool CreateEffect(g3d::modelEffect::ModelEffectHandle* handle, u8 resourceIdx, const char* modelName, const char* animName); // 0x002B65A0
    // the container of the slot if it has that serial and plays (NULL: else)
    g3d::modelEffect::ModelEffectContainer* FindEffect(s32 index, s32 serial); // 0x0071D904
};
