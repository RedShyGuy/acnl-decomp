#include "nw/snd/internal/snd_CurveLfo.h"

#include <cstring>

#include "nn/math/math_Api.h"
#include "nw/snd/internal/Util/Util_Api.h"

namespace nw {
namespace snd {
namespace internal {
// 0x00AF0788
CurveLfo::CurveFunc CurveLfo::s_CurveFuncTable[CURVE_TYPE_COUNT];

namespace {
// 0x004C9B44 | fefates:bytes [tier B]
f32 CurveSine(f32 t)
{
    return nn::math::SinIdx(static_cast<u32>(t * 65536.0f));
}

// 0x004C9AE0 | fefates:bytes [tier B]
f32 CurveTriangle(f32 t)
{
    // per quarter of the cycle: value = slope * t + offset
    // 0x008B618C
    const f32 slopes[5] = {4.0f, -4.0f, -4.0f, 4.0f, 4.0f};
    const f32 offsets[5] = {0.0f, 2.0f, 2.0f, -4.0f, 0.0f};
    const u32 quarter = static_cast<u32>(t * 4.0f);
    return offsets[quarter] + slopes[quarter] * t;
}

// 0x004C9B40 (name is ours)
f32 CurveSaw(f32 t)
{
    return t;
}

// 0x004C9AC4 | fefates:bytes [tier B]
f32 CurveSquare(f32 t)
{
    return t < 0.5f ? 0.0f : 1.0f;
}

// 0x004C9AA4 | fefates:bytes [tier B]
f32 CurveRandom(f32 /*t*/)
{
    return Util::CalcRandom() * (1.0f / 0xFFFF);
}
} // namespace

// 0x004D48F8 | fefates:bytes [tier B]
void nw::snd::internal::CurveLfo::InitializeCurveTable()
{
    std::memset(s_CurveFuncTable, 0, sizeof(s_CurveFuncTable));
    s_CurveFuncTable[CURVE_SINE] = CurveSine;
    s_CurveFuncTable[CURVE_TRIANGLE] = CurveTriangle;
    s_CurveFuncTable[CURVE_SAW] = CurveSaw;
    s_CurveFuncTable[CURVE_SQUARE] = CurveSquare;
    s_CurveFuncTable[CURVE_RANDOM] = CurveRandom;
}

// 0x004D4950 | fefates:bytes [tier B]
void nw::snd::internal::CurveLfo::Reset()
{
    m_DelayCounter = 0;
    m_Counter = 0.0f;
    m_RandomValue = 1.0f;
    m_IsStarted = false;
    m_IsNextCycle = false;
}

// 0x004D497C | nintendogs:callseq-callee [tier A]
void nw::snd::internal::CurveLfo::Update(int msec)
{
    if (m_DelayCounter < m_Param.delay) {
        if (m_DelayCounter + msec <= m_Param.delay) {
            m_DelayCounter += msec;
            return;
        }
        msec -= m_Param.delay - m_DelayCounter;
        m_DelayCounter = m_Param.delay;
    }
    if (m_Param.speed > 0.0f) {
        if (!m_IsStarted) {
            m_Counter = m_Param.phase * (1.0f / 127);
            m_IsStarted = true;
        }
        m_Counter += m_Param.speed * (msec * 0.001f);
        m_IsNextCycle = m_Counter >= 1.0f;
        m_Counter -= static_cast<int>(m_Counter);
    }
}

// 0x004D4A38 | fefates:bytes [tier B]
nw::snd::internal::CurveLfo::CurveLfo()
{
    Reset();
}

// 0x00743000 | nintendogs:callseq-callee [tier A]
f32 nw::snd::internal::CurveLfo::GetValue() const
{
    if (m_Param.depth == 0.0f) {
        return 0.0f;
    }
    if (m_DelayCounter < m_Param.delay) {
        return 0.0f;
    }
    f32 value;
    const CurveFunc func = s_CurveFuncTable[m_Param.curve];
    if (func == nullptr) {
        value = 1.0f;
    } else if (m_Param.curve == CURVE_RANDOM) {
        // a new random value per cycle
        if (m_IsNextCycle) {
            value = func(m_Counter);
            m_RandomValue = value;
        } else {
            value = m_RandomValue;
        }
    } else {
        value = func(m_Counter);
    }
    return m_Param.depth * value * m_Param.range;
}

} // namespace internal
} // namespace snd
} // namespace nw
