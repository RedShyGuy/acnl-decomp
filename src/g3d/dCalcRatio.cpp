#include "g3d/dCalcRatio.h"

namespace g3d {
// the morph functions per type
// 0x00953A88
const CalcRatio::StartFunc CalcRatio::s_StartFuncs[CalcRatio::TYPE_COUNT] = {
    &CalcRatio::StartSmooth,
    &CalcRatio::StartLinear,
};
// 0x00953A98
const CalcRatio::UpdateFunc CalcRatio::s_UpdateFuncs[CalcRatio::TYPE_COUNT] = {
    &CalcRatio::UpdateSmooth,
    &CalcRatio::UpdateLinear,
};

// 0x004F360C (name is ours)
void g3d::CalcRatio::UpdateLinear()
{
    f32 time = m_Time - 1.0f;
    m_Time = time;
    if (time < 1.0f) {
        m_Weight = 0.0f;
        m_Time = 1.0f;
        m_Ratio = 0.0f;
        return;
    }
    f32 ratio = time / (time + 1.0f);
    m_Weight = ratio;
    m_Ratio = ratio;
}

// 0x004F3658 (name is ours)
void g3d::CalcRatio::StartLinear(f32 duration)
{
    m_Time = duration;
}

// 0x004F3660 (name is ours)
void g3d::CalcRatio::UpdateSmooth()
{
    f32 time = m_Time + m_Step;
    m_Time = time;
    f32 ratio;
    if (time >= 1.0f) {
        m_Time = 1.0f;
        ratio = 0.0f;
        m_Weight = 0.0f;
    } else {
        f32 weight = m_Weight;
        f32 newWeight = weight - weight * time * time;
        m_Weight = newWeight;
        f32 relative = newWeight / weight;
        ratio = relative * (1.0f / (relative + (1.0f - newWeight)));
    }
    m_Ratio = ratio;
}

// 0x004F36C8 (name is ours)
void g3d::CalcRatio::StartSmooth(f32 duration)
{
    m_Time = 0.0f;
    m_Step = 1.0f / duration;
}

// 0x004F36EC slot 0x00
// 0x004F36E8 (deleting dtor)
g3d::CalcRatio::~CalcRatio()
{
    // empty (in the original too)
}

} // namespace g3d
