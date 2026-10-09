#include "nn/gr/CTR/gr_Combiner_Stage.h"
#include "nn/gr/CTR/detail/gr_Command.h"

namespace nn {
namespace gr {
namespace CTR {
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

// 0x0034B4E4 | fefates:bytes [tier B]
nn::gr::CTR::Combiner::Stage::Stage()
{
}

} // namespace CTR
} // namespace gr
} // namespace nn
