#pragma once

#include "decomp.h"
#include "Player/dPlayerModel.h"

// RTTI 19PlayerBoardingModel @ 0x008CCA2C
// vtable 0x008F50DC (vptr 0x008F50E4), offset_to_top 0, 11 entries
class PlayerBoardingModel : public ::PlayerModel
{
public:
    PlayerBoardingModel(); // ctor address unknown
    virtual ~PlayerBoardingModel(); // 0x002FABB4 slot 0x1C | slot vf_0x1C of HumanModel
    // 0x002FABA4 slot 0x20 | slot vf_0x20 of HumanModel (deleting dtor)
    virtual void vf_0x24(); // 0x002FAA24 slot 0x24 | virtual slot, introduced by HumanModel
};
