#pragma once

#include "decomp.h"

namespace nn {
namespace http {
namespace detail {
// RTTI N2nn4http6detail10LibManagerE @ 0x008D03BC
// vtable 0x009021FC (vptr 0x00902204), offset_to_top 0, 2 entries
class LibManager
{
public:
    LibManager(); // ctor candidate(s) 0x0079A294 (unverified)
    virtual ~LibManager(); // 0x0046FE70 slot 0x00 | fefates:bytes
    virtual void vf_0x04(); // 0x0046FE14 slot 0x04 | virtual slot, introduced by nn::http::detail::LibManager
    void Initialize(unsigned int, unsigned int); // 0x0046FD10 | fefates:bytes [tier B]
};
} // namespace detail
} // namespace http
} // namespace nn
