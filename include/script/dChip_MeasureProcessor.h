#pragma once

#include "decomp.h"
#include "nn/math/math_Vector2.h"
#include "nw/font/font_TextWriterBase.h"
#include "script/dChip.h"

// The tag processor for measuring: only the tag of the font scale changes the writer. Member names are ours.
// RTTI N6script4Chip16MeasureProcessorE
// vtable 0x0090A56C (vptr 0x0090A574)
class script::Chip::MeasureProcessor : public ::nw::font::TagProcessorBase<wchar_t>
{
public:
    // (inline)
    explicit MeasureProcessor(const nn::math::VEC2& scale) : m_Scale(scale) {}
    virtual ~MeasureProcessor();
    virtual Operation CalcRect(nw::ut::Rect* pRect, u16 code, nw::font::PrintContext<wchar_t>* ctx); // 0x005EA9F0 slot 0x0C

    nn::math::VEC2 m_Scale; // 0x4 the scale of the writer for the scale 100 %
};
ASSERT_SIZE(script::Chip::MeasureProcessor, 0xC);
