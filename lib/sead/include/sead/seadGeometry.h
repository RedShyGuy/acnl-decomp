#pragma once

#include "decomp.h"
#include "sead/seadVector3.h"

namespace sead {
// a line segment from mPos0 to mPos1 (member names are ours)
template <typename T>
class Segment
{
public:
    T mPos0;
    T mPos1;
};

class Geometry
{
public:
    // the squared distance of point to the nearest point of segment; its position on the segment
    // (0 at mPos0, 1 at mPos1) goes to pT if it is not NULL
    static f32 calcSquaredDistancePointToSegment(const Vector3<f32>& point, const Segment<Vector3<f32>>& segment, f32* pT); // 0x00560F34 | nintendogs:bytes [tier A]
};
} // namespace sead
