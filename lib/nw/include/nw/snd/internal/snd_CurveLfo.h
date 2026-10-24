#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_CurveLfoParam.h"

namespace nw {
namespace snd {
namespace internal {
// An LFO: a curve over a cycle, scaled by depth and range, after a delay (the class and function
// names are from the reference symbols, the rest is ours).
class CurveLfo
{
public:
    // the curves of a cycle (by index in the table; the others are free for the game)
    enum CurveType : u8
    {
        CURVE_SINE = 0,
        CURVE_TRIANGLE = 1,
        CURVE_SAW = 2,
        CURVE_SQUARE = 3,
        CURVE_RANDOM = 4,
    };
    static const int CURVE_TYPE_COUNT = 128;
    // a curve: the value at a point 0 to 1 of the cycle
    typedef f32 (*CurveFunc)(f32 t);

    CurveLfo(); // 0x004D4A38 | fefates:bytes [tier B]

    // the built-in curves
    static void InitializeCurveTable(); // 0x004D48F8 | fefates:bytes [tier B]
    // the cycle and the delay started again
    void Reset(); // 0x004D4950 | fefates:bytes [tier B]
    // msec milliseconds on
    void Update(int msec); // 0x004D497C | nintendogs:callseq-callee [tier A]
    // the value (0 during the delay)
    f32 GetValue() const; // 0x00743000 | nintendogs:callseq-callee [tier A]

    static CurveFunc s_CurveFuncTable[CURVE_TYPE_COUNT];

    CurveLfoParam m_Param;    // 0x00
    u32 m_DelayCounter;       // 0x10
    f32 m_Counter;            // 0x14 the point in the cycle, 0 to 1
    mutable f32 m_RandomValue; // 0x18 the value of the random curve in this cycle
    bool m_IsStarted;         // 0x1C
    bool m_IsNextCycle;       // 0x1D a new cycle began in the last update
};
ASSERT_SIZE(CurveLfo, 0x20);
} // namespace internal
} // namespace snd
} // namespace nw
