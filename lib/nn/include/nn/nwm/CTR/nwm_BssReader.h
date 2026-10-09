#pragma once

#include "decomp.h"
#include "nn/nwm/nwm_Types.h"

namespace nn {
namespace nwm {
namespace CTR {
// RTTI N2nn3nwm3CTR9BssReaderE @ 0x008CF7EC
// Walks the information elements (tag, length, data) of a beacon. The names of the virtual
// functions and of the functions marked "(name is ours)" are ours.
class BssReader
{
public:
    virtual ~BssReader() {}
    // the first element
    virtual const u8* GetIePointer() const = 0;
    // the size of the elements
    virtual u32 GetIeSize() const = 0;

    // the number of elements
    u8 GetIeNum() const; // 0x0072ECF4 (name is ours)
    // the SSID element (empty without one)
    Ssid GetSsid() const; // 0x0072EEB8 (name is ours)
    // the last vendor specific element (tag 0xDD) with this OUI; points to its tag
    const u8* GetVendorIe(const u8* oui) const; // 0x0072ED54 (name after the overload)
    // the same with this OUI type
    const u8* GetVendorIe(const u8* oui, u8 type) const; // 0x0072EE00 | fefates:bytes [tier B]
};
} // namespace CTR
} // namespace nwm
} // namespace nn
