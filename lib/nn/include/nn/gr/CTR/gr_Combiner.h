#pragma once

#include "decomp.h"

namespace nn {
namespace gr {
namespace CTR {
// The six texture combiner stages and the combiner buffer (member names are ours; values are the
// PICA register fields on 3dbrew, source 14 = constant color, 15 = previous stage).
class Combiner
{
public:
    // the RGB or alpha half of a stage (name is ours)
    struct Combine
    {
        explicit Combine(u8 stageIndex)
            : m_Combine(0), m_Scale(0), m_BufferInput(0)
        {
            m_Operand[0] = 0;
            m_Operand[1] = 0;
            m_Operand[2] = 0;
            // the first stage starts from the constant color, the others from the previous stage
            m_Source[0] = (stageIndex == 0) ? 14 : 15;
            m_Source[1] = (stageIndex == 0) ? 14 : 15;
            m_Source[2] = (stageIndex == 0) ? 14 : 15;
        }

        Combine() {}

        u8 m_Combine;       // 0x0
        u8 m_Operand[3];    // 0x1
        u8 m_Source[3];     // 0x4
        u8 m_Scale;         // 0x7
        u8 m_BufferInput;   // 0x8, the previous buffer instead of the previous stage
    };

    class Stage
    {
    public:
        Stage(); // 0x0034B4E4 | fefates:bytes [tier B]
        explicit Stage(int index); // 0x0034B398 | fefates:bytes-fuzzy [tier B]

        u32* MakeCommand(u32* command) const; // 0x00728A80 | fefates:bytes [tier B]

        Combine m_Rgb;      // 0x00
        Combine m_Alpha;    // 0x09
        u8 m_ColorR;        // 0x12, constant color
        u8 m_ColorG;        // 0x13
        u8 m_ColorB;        // 0x14
        u8 m_ColorA;        // 0x15
        u16 m_Register;     // 0x16, first register of the stage
    };

    static const s32 STAGE_COUNT = 6;

    Combiner(); // 0x0034B548 | fefates:bytes-fuzzy [tier B]

    u32* MakeCommand(u32* command) const; // 0x0072893C | fefates:bytes [tier B]

    Stage m_Stages[STAGE_COUNT];    // 0x00
    u8 m_BufferColorR;              // 0x90
    u8 m_BufferColorG;              // 0x91
    u8 m_BufferColorB;              // 0x92
    u8 m_BufferColorA;              // 0x93
};
ASSERT_SIZE(Combiner::Combine, 0x9);
ASSERT_SIZE(Combiner::Stage, 0x18);
ASSERT_SIZE(Combiner, 0x94);
} // namespace CTR
} // namespace gr
} // namespace nn
