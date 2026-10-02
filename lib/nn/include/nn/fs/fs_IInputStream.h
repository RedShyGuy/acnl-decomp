#pragma once

#include "decomp.h"
#include "nn/fs/fs_IPositionable.h"

namespace nn {
namespace fs {
// RTTI N2nn2fs12IInputStreamE @ 0x008CDBE8
// vtable 0x0085FD74 (vptr 0x0085FD7C), offset_to_top 0, 13 entries
// vtable 0x008A35FC (vptr 0x008A3604), offset_to_top 0, 12 entries
class IInputStream : public virtual ::nn::fs::IPositionable
{
public:
    IInputStream(); // ctor address unknown
    virtual void vf_0x00(); // 0x00346018 slot 0x00 | virtual slot, introduced by nn::fs::IInputStream
    virtual void vf_0x04(); // 0x00346014 slot 0x04 | virtual slot, introduced by nn::fs::IInputStream
    virtual void vf_0x08(); // 0x0011C12F slot 0x08 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x0C(); // 0x0011C12F slot 0x0C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x10(); // 0x0011C12F slot 0x10 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x14(); // 0x0011C12F slot 0x14 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x18(); // 0x0011C12F slot 0x18 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x1C(); // 0x0011C12F slot 0x1C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x20(); // 0x0011C12F slot 0x20 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x24(); // 0x0011C12F slot 0x24 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x28(); // 0x0011C12F slot 0x28 | slot vf_0x00 of ChangeRentalBase
    virtual void Read(void*, unsigned int); // 0x0011C12F slot 0x2C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x30(); // 0x0052A4A8 slot 0x30 | virtual slot, introduced by nn::fs::IInputStream
};
} // namespace fs
} // namespace nn
