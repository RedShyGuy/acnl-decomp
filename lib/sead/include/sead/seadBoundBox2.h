#pragma once

#include "decomp.h"
#include "sead/seadVector2.h"

#include <cfloat>

namespace sead {
// Instantiations found in the binary:
//   sead::BoundBox2<float>  typeinfo 0x008D2270
//
// An axis-aligned box (empty after construction: min at the largest value, max at the smallest;
// names are ours).
template <typename T>
class BoundBox2
{
public:
    BoundBox2() : mMin(FLT_MAX, FLT_MAX), mMax(-FLT_MAX, -FLT_MAX) {}

    // the box between the two points (in any order)
    void set(T x0, T y0, T x1, T y1)
    {
        if (x0 < x1) {
            mMin.x = x0;
            mMax.x = x1;
        } else {
            mMin.x = x1;
            mMax.x = x0;
        }
        if (y0 < y1) {
            mMin.y = y0;
            mMax.y = y1;
        } else {
            mMin.y = y1;
            mMax.y = y0;
        }
    }

    Vector2<T> mMin; // 0x0
    Vector2<T> mMax; // 0x8
};
} // namespace sead
