#pragma once

#include "decomp.h"
#include "lyt/dTextAreaOneLine.h"
#include "nw/lyt/lyt_Types.h"
#include "script/dRenderGarden.h"
#include "script/dRenderRubySet.h"

namespace lyt {
// The one-line text area of the keyboards with a title window (cmn_title_win): the title (SYS_2D_UI) and a second
// line depend on the kind of keyboard (sys::g_Unknown95F0EB), the text box and its decoration differ per kind and
// region, and the address kinds (17, 18) show the text in three boxes of four characters (only the class name is from
// the RTTI; all other names are ours).
// RTTI N3lyt23TextAreaOneLineComTitleE @ 0x008D0F7C
// vtable 0x00904504 (vptr 0x0090450C), offset_to_top 0, 14 entries
class TextAreaOneLineComTitle : public ::lyt::TextAreaOneLine
{
public:
    // (inline)
    TextAreaOneLineComTitle(u32 unknown, s32 priority, sead::Heap* heap)
        : TextAreaOneLine(unknown, priority, heap), m_Title(48, 6), m_Subtitle(11), m_AddressFontSize(0.0f, 0.0f)
    {
        for (s32 i = 0; i < 3; i++) {
            m_pAddressParts[i] = NULL;
        }
    }
    virtual ~TextAreaOneLineComTitle(); // 0x005095C4 slot 0x00
    // 0x00509530 slot 0x04 (deleting dtor)
    void Setup(ssys::ma::lyt::ArcResAccReader* arcReader, ssys::ma::lyt::ArcResAccReader* decorArcReader) override; // 0x005086F0 slot 0x10
    // the text into the three boxes of four characters (their font narrowed to fit)
    void vf_0x18() override; // 0x00507E90 slot 0x18
    const char* GetLayoutName() override { return "cmn_title_win"; } // 0x00747950 slot 0x1C

    // the title and the second line of the kind of keyboard
    void SetTitle(); // 0x005084E4

    script::RenderRubySet m_Title;                 // 0x6F8
    script::RenderGarden m_Subtitle;               // 0x7F8
    script::RenderGarden* m_pAddressParts[3];      // 0x8BC T_address_NN
    nw::lyt::Size m_AddressFontSize;               // 0x8C8
};
ASSERT_OFFSET(TextAreaOneLineComTitle, m_Title, 0x6F8);
ASSERT_OFFSET(TextAreaOneLineComTitle, m_pAddressParts, 0x8BC);
ASSERT_SIZE(TextAreaOneLineComTitle, 0x8D0);
} // namespace lyt
