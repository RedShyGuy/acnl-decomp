#include "nn/gr/CTR/gr_Combiner_Stage.h"
#include "nn/gr/CTR/detail/gr_Command.h"

namespace nn {
namespace gr {
namespace CTR {
namespace {
// the PICA fields of a stage (3dbrew "GPU/Internal Registers"; names are ours)
const u8 COMBINE_REPLACE = 0;
const u8 COMBINE_MODULATE = 1;
const u8 SOURCE_PRIMARY_COLOR = 0;
const u8 SOURCE_TEXTURE0 = 3;
const u8 SOURCE_PREVIOUS = 15;
} // namespace

// 0x0034B398 | fefates:bytes-fuzzy [tier B]
nn::gr::CTR::Combiner::Stage::Stage(int index)
    : m_Rgb(static_cast<u8>(index)), m_Alpha(static_cast<u8>(index)), m_ColorR(0), m_ColorG(0), m_ColorB(0), m_ColorA(0)
{
    // the first register of the stage
    switch (index) {
    case 0:
        m_Register = 0xC0;
        break;
    case 1:
        m_Register = 0xC8;
        break;
    case 2:
        m_Register = 0xD0;
        break;
    case 3:
        m_Register = 0xD8;
        break;
    case 4:
        m_Register = 0xF0;
        break;
    case 5:
        m_Register = 0xF8;
        break;
    }
}

// 0x0034B2CC (name is ours)
void nn::gr::CTR::Combiner::Stage::SetReplaceTexture0()
{
    m_Rgb.m_Combine = COMBINE_REPLACE;
    m_Rgb.m_Operand[0] = 0;
    m_Rgb.m_Operand[1] = 0;
    m_Rgb.m_Operand[2] = 0;
    m_Rgb.m_Source[0] = SOURCE_TEXTURE0;
    m_Rgb.m_Source[1] = SOURCE_TEXTURE0;
    m_Rgb.m_Source[2] = SOURCE_TEXTURE0;
    m_Rgb.m_Scale = 0;
    m_Rgb.m_BufferInput = 0;
    m_Alpha.m_Combine = COMBINE_REPLACE;
    m_Alpha.m_Operand[0] = 0;
    m_Alpha.m_Operand[1] = 0;
    m_Alpha.m_Operand[2] = 0;
    m_Alpha.m_Source[0] = SOURCE_TEXTURE0;
    m_Alpha.m_Source[1] = SOURCE_TEXTURE0;
    m_Alpha.m_Source[2] = SOURCE_TEXTURE0;
    m_Alpha.m_Scale = 0;
    m_Alpha.m_BufferInput = 0;
}

// 0x0034B328 (name is ours)
void nn::gr::CTR::Combiner::Stage::SetModulatePrimaryColorTexture0()
{
    m_Rgb.m_Combine = COMBINE_MODULATE;
    m_Rgb.m_Operand[0] = 0;
    m_Rgb.m_Operand[1] = 0;
    m_Rgb.m_Operand[2] = 0;
    m_Rgb.m_Source[0] = SOURCE_PRIMARY_COLOR;
    m_Rgb.m_Source[1] = SOURCE_TEXTURE0;
    m_Rgb.m_Source[2] = SOURCE_PREVIOUS;
    m_Rgb.m_Scale = 0;
    m_Rgb.m_BufferInput = 0;
    m_Alpha.m_Combine = COMBINE_MODULATE;
    m_Alpha.m_Operand[0] = 0;
    m_Alpha.m_Operand[1] = 0;
    m_Alpha.m_Operand[2] = 0;
    m_Alpha.m_Source[0] = SOURCE_PRIMARY_COLOR;
    m_Alpha.m_Source[1] = SOURCE_TEXTURE0;
    m_Alpha.m_Source[2] = SOURCE_PREVIOUS;
    m_Alpha.m_Scale = 0;
    m_Alpha.m_BufferInput = 0;
}

// 0x0034B4E4 | fefates:bytes [tier B]
nn::gr::CTR::Combiner::Stage::Stage()
{
    // nothing to do: the members and bases are constructed by the compiler
}

} // namespace CTR
} // namespace gr
} // namespace nn
