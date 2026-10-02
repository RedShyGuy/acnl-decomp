#pragma once

#include "decomp.h"
#include "g3d/dFrameController.h"

namespace g3d {
// RTTI N3g3d8BaseAnimE @ 0x008D0D8C
// vtable 0x00903C04 (vptr 0x00903C0C), offset_to_top 0, 8 entries
class BaseAnim : public ::g3d::FrameController
{
public:
    BaseAnim(); // ctor address unknown
    virtual void vf_0x00(); // 0x004F2DCC slot 0x00 | virtual slot, introduced by g3d::BaseAnim
    virtual void vf_0x04(); // 0x004F2DAC slot 0x04 | virtual slot, introduced by g3d::BaseAnim
    virtual void vf_0x08(); // 0x004F2CB8 slot 0x08 | virtual slot, introduced by g3d::BaseAnim
    virtual void vf_0x0C(); // 0x007474F8 slot 0x0C | virtual slot, introduced by g3d::BaseAnim
    virtual void vf_0x10(); // 0x004F2D2C slot 0x10 | virtual slot, introduced by g3d::BaseAnim
    virtual void vf_0x14(); // 0x004F2CE0 slot 0x14 | virtual slot, introduced by g3d::BaseAnim
    virtual void vf_0x18(); // 0x0011C12F slot 0x18 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x1C(); // 0x0011C12F slot 0x1C | slot vf_0x00 of ChangeRentalBase
};
} // namespace g3d
