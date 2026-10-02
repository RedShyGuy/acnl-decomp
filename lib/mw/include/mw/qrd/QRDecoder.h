#pragma once

#include "decomp.h"

namespace mw {
namespace qrd {
// RTTI N2mw3qrd9QRDecoderE @ 0x008CDBB0
// vtable 0x008FBC04 (vptr 0x008FBC0C), offset_to_top 0, 2 entries
class QRDecoder
{
public:
    QRDecoder(); // ctor candidate(s) 0x00345360 (unverified)
    virtual void vf_0x00(); // 0x00345374 slot 0x00 | virtual slot, introduced by mw::qrd::QRDecoder
    virtual void vf_0x04(); // 0x00345370 slot 0x04 | virtual slot, introduced by mw::qrd::QRDecoder
    void Finalize(); // 0x0012FD74 | fefates:bytes [tier B]
    void Initialize(void*, unsigned int); // 0x00344CB0 | fefates:bytes [tier B]
};
} // namespace qrd
} // namespace mw
