#pragma once

#include "decomp.h"

namespace sead {
class Geometry
{
public:
    void calcSquaredDistancePointToSegment(const sead::Vector3<float>&, const sead::Segment<sead::Vector3<float>>&, float*); // 0x00560F34 | nintendogs:bytes [tier A]
};
} // namespace sead
