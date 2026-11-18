#pragma once

#include "decomp.h"

namespace g3d {
class SkeletalModel;
} // namespace g3d

// The interface of the model banks: the model of the shown slot (class, slot and member names after libgarden; no
// destructor, ExchangeModelBank implements it).
// RTTI 21ExchangeModelBankBase @ 0x008CCCE4
class ExchangeModelBankBase
{
public:
    ExchangeModelBankBase() : unknown(1.0f) {}

    // the model of the shown slot (NULL: none)
    virtual g3d::SkeletalModel* GetCurrentModel() = 0; // slot 0x00
    virtual const g3d::SkeletalModel* GetCurrentModel() const = 0; // slot 0x04
    // the resource of model index of the shown slot (nw::gfx::res::ResModelData; NULL: none)
    virtual const void* GetCurrentResource(u32 index) const = 0; // slot 0x08

    f32 unknown; // 0x4 (1.0f)
};
