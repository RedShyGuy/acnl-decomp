#pragma once

#include "decomp.h"
#include "script/dMsgEngine.h"

namespace nw {
namespace lyt {
class TextBox;
}
} // namespace nw

namespace script {
class RenderBase;

// Writes a message into the text buffers of the renderers: the text and the tags of the main text into the main
// renderer, line feeds, colors and the rubies into the ruby renderer (if any). For a ruby, both texts get a tag with
// the measured width. Member names are ours.
// RTTI N6script11RenderTraceE @ 0x008D3808
// vtable 0x0090A050 (vptr 0x0090A058), offset_to_top 0, 8 entries
class RenderTrace : public ::script::MsgEngine
{
public:
    // (inline)
    RenderTrace(RenderBase* pMain, RenderBase* pSub)
        : m_pMain(pMain), m_pSub(pSub), m_pTextBox(NULL), m_pTextBoxSub(NULL), m_LineNum(0)
    {
    }
    virtual ~RenderTrace();
    virtual void Begin(); // 0x005DFE20 slot 0x08
    virtual void End(); // 0x005DFD58 slot 0x0C
    virtual bool IsContinue(); // 0x005DFE48 slot 0x10
    virtual void OnChar(const CharInfo* info); // 0x005DFB20 slot 0x14
    virtual void OnTag(const Tag* tag); // 0x005DFD5C slot 0x18

    DECOMP_NOINLINE void OnRuby(const Tag* tag); // 0x005DFB90

    RenderBase* m_pMain;               // 0x60
    RenderBase* m_pSub;                // 0x64 the ruby text (NULL: none)
    nw::lyt::TextBox* m_pTextBox;      // 0x68 for measuring the ruby bases (NULL: no measuring)
    nw::lyt::TextBox* m_pTextBoxSub;   // 0x6C for measuring the rubies
    s32 m_LineNum;                     // 0x70
};
ASSERT_SIZE(RenderTrace, 0x74);
} // namespace script
