#pragma once

#include "decomp.h"

namespace sead {
namespace ptcl {
class PtclProjectEmitterSet
{
public:
    void bindTarget(int, sead::ptcl::PtclResource*, int); // 0x0055C1C8 | mk7dlp:bytes [tier B]
    void unbindResource(sead::ptcl::PtclResource*); // 0x0055C244 | mk7dlp:bytes-fuzzy [tier B]
    void setOrder(int); // 0x0055C30C | mk7dlp:bytes [tier B]
};
} // namespace ptcl
} // namespace sead
