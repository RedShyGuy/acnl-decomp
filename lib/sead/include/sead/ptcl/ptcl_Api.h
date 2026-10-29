#pragma once

#include "decomp.h"

namespace sead {
namespace ptcl {
class PtclInstance;
struct SimpleEmitterData;

// the texture pattern of the particle in this frame
void _calcTexPatAnim(const SimpleEmitterData* res, PtclInstance* ptcl); // 0x00557F54 | mk7dlp:bytes [tier B]
// the color of the particle in this frame: one of the three by random or between them over its life
void _interpolate3Colors(const SimpleEmitterData* res, PtclInstance* ptcl); // 0x0055B308 | mk7dlp:bytes [tier B]
} // namespace ptcl
} // namespace sead
