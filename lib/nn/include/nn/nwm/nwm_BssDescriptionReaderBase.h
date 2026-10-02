#pragma once

#include "decomp.h"
#include "nn/nwm/nwm_Types.h"

namespace nn {
namespace nwm {

// reads a BssDescription of the wireless driver (member names are ours)
class BssDescriptionReaderBase
{
public:
    BssDescriptionReaderBase(); // ctor address unknown
    explicit BssDescriptionReaderBase(const nn::nwm::BssDescription* bss) : m_pBss(bss) {}
    virtual ~BssDescriptionReaderBase() {}

    nn::nwm::Mac GetBssid() const; // 0x0072EC8C | fefates:bytes [tier B]
    u8 GetChannel() const; // 0x0072EC6C | tier C
    // the s16 at +6 of the description, 0 without one (name is ours)
    s16 GetSignalStrength() const; // 0x0072EC7C

protected:
    const nn::nwm::BssDescription* m_pBss;  // 0x4
};

} // namespace nwm
} // namespace nn
