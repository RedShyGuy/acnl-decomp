#pragma once

#include "decomp.h"
#include "Human/dHumanModel.h"

// RTTI 11PlayerModel @ 0x008CB31C
// vtable 0x008ECC90 (vptr 0x008ECC98), offset_to_top 0, 11 entries
class PlayerModel : public ::HumanModel
{
public:
    PlayerModel(); // ctor candidate(s) 0x001D38F8 (unverified)
    virtual void vf_0x00(); // 0x001D3428 slot 0x00 | virtual slot, introduced by HumanModel
    virtual void vf_0x08(); // 0x001D2C8C slot 0x08 | virtual slot, introduced by HumanModel
    virtual void vf_0x0C(); // 0x001AC87C slot 0x0C | virtual slot, introduced by HumanModel
    virtual void vf_0x10(); // 0x001D34A0 slot 0x10 | virtual slot, introduced by HumanModel
    virtual void vf_0x14(); // 0x001D2D70 slot 0x14 | virtual slot, introduced by HumanModel
    virtual void vf_0x18(); // 0x0027231C slot 0x18 | virtual slot, introduced by HumanModel
    virtual ~PlayerModel(); // 0x001D3A24 slot 0x1C | slot vf_0x1C of HumanModel
    // 0x001D3A14 slot 0x20 | slot vf_0x20 of HumanModel (deleting dtor)
    virtual void vf_0x24(); // 0x001CECB4 slot 0x24 | virtual slot, introduced by HumanModel
    virtual void vf_0x28(); // 0x0070F178 slot 0x28 | virtual slot, introduced by HumanModel
};
