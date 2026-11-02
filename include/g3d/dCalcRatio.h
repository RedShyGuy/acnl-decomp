#pragma once

#include "decomp.h"

namespace g3d {
// the weight of the previous pose while a skeletal animation morphs into the next one: it falls from 1 to 0 over the
// morph (names are ours)
// RTTI N3g3d9CalcRatioE @ 0x008D0DB0
// vtable 0x00903C60 (vptr 0x00903C68), offset_to_top 0, 2 entries
class CalcRatio
{
public:
    // m_Type: how the weight falls
    enum Type
    {
        TYPE_SMOOTH = 0,   // over a duration in frames with an eased weight
        TYPE_LINEAR = 1,   // frame by frame: t / (t + 1)
        TYPE_COUNT = 2,
    };
    // the start of a morph of the type (the duration in frames) / one frame of it
    typedef void (CalcRatio::*StartFunc)(f32 duration);
    typedef void (CalcRatio::*UpdateFunc)();

    // (inline)
    CalcRatio() : m_Weight(0.0f), m_Time(1.0f), m_Step(0.0f), m_Ratio(0.0f), m_Type(TYPE_SMOOTH) {}
    virtual ~CalcRatio(); // 0x004F36EC slot 0x00
    // 0x004F36E8 (deleting dtor)

    void UpdateLinear(); // 0x004F360C
    void StartLinear(f32 duration); // 0x004F3658
    void UpdateSmooth(); // 0x004F3660
    void StartSmooth(f32 duration); // 0x004F36C8

    // (inline) the morph of the type over duration frames (0 or less: none)
    void Start(u8 type, f32 duration)
    {
        if (duration > 0.0f) {
            m_Weight = 1.0f;
            m_Ratio = 1.0f;
            m_Type = type;
            (this->*s_StartFuncs[type])(duration);
        } else {
            m_Weight = 0.0f;
            m_Time = 1.0f;
            m_Ratio = 0.0f;
        }
    }
    // (inline) one frame of the morph
    void Update() { (this->*s_UpdateFuncs[m_Type])(); }
    bool IsMorphing() const { return m_Weight > 0.0f; }

    static const StartFunc s_StartFuncs[TYPE_COUNT];
    static const UpdateFunc s_UpdateFuncs[TYPE_COUNT];

    f32 m_Weight;  // 0x04 (0: no morph)
    f32 m_Time;    // 0x08
    f32 m_Step;    // 0x0C
    f32 m_Ratio;   // 0x10 the weight the evaluator blends with
    u8 m_Type;     // 0x14 Type
};
ASSERT_SIZE(CalcRatio, 0x18);
} // namespace g3d
