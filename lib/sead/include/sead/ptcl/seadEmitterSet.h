#pragma once

#include "decomp.h"

namespace sead {
namespace ptcl {
class EmitterSet
{
public:
    void emitParticle(const sead::Vector3<float>&); // 0x0054E44C | mk7dlp:bytes [tier B]
    void kill(); // 0x0054E4FC | mk7dlp:bytes [tier B]
    void setMtx(const sead::Matrix34<float>&); // 0x0054E544 | mk7dlp:bytes [tier B]
    EmitterSet(); // 0x0054E734 | mk7dlp:bytes [tier B]
};
} // namespace ptcl
} // namespace sead
