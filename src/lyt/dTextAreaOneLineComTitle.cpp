#include "lyt/dTextAreaOneLineComTitle.h"

#include <cmath>
#include <cstdio>
#include <new>

#include "Other/dFormatBuffer.h"
#include "Other/dRegion.h"
#include "nn/math/math_ARMv6.h"
#include "nn/math/math_Api.h"
#include "nw/lyt/lyt_TextBox.h"
#include "script/dUiMgr.h"
#include "script/dWordCPtr.h"
#include "script/dWordFix.h"
#include "sead/seadWFixedSafeString.h"
#include "sys/dSwkbd.h"

namespace {
// the kinds of keyboards with a title window (sys::g_Unknown95F0EB)
const s32 KIND_ADDRESS = 17;
const s32 KIND_ADDRESS_2 = 18;
// the address is shown in three parts of four characters
const s32 ADDRESS_PART_NUM = 3;
const s32 ADDRESS_PART_LENGTH = 4;

// (inline) the visible flag is set without comparing
inline void SetFlagVisible(nw::lyt::Pane* pane, bool visible)
{
    pane->m_Flag = (pane->m_Flag & ~nw::lyt::Pane::FLAG_VISIBLE) | visible;
}
} // namespace

// 0x00507E90 slot 0x18
void lyt::TextAreaOneLineComTitle::vf_0x18()
{
    script::WordCPtr words[ADDRESS_PART_NUM];
    sead::WFixedSafeString<ADDRESS_PART_LENGTH * ADDRESS_PART_NUM + 1> text;
    sead::WFixedSafeString<ADDRESS_PART_LENGTH + 1> parts[ADDRESS_PART_NUM];
    for (s32 i = 0; i < ADDRESS_PART_NUM; i++) {
        parts[i].clear();
    }
    parts[1].overwriteAt(0, parts[0].getPart(0), ADDRESS_PART_LENGTH);
    parts[2].overwriteAt(0, parts[0].getPart(0), ADDRESS_PART_LENGTH);
    parts[0].overwriteAt(0, parts[1].getPart(0), ADDRESS_PART_LENGTH);

    // the typed text in parts of four characters
    text.copy(sead::SafeStringBase<wchar_t>(sys::g_pTextBuffer));
    s32 length = text.getLength();
    if (length < 0) {
        length = 0;
    } else if (length > ADDRESS_PART_LENGTH * ADDRESS_PART_NUM) {
        length = ADDRESS_PART_LENGTH * ADDRESS_PART_NUM;
    }
    const s32 fullParts = length / ADDRESS_PART_LENGTH;
    const s32 rest = length % ADDRESS_PART_LENGTH;
    s32 index = 0;
    for (s32 i = 0; i < fullParts; i++) {
        for (s32 j = 0; j < ADDRESS_PART_LENGTH; j++) {
            parts[i].overwriteAt(j, text.getPart(index), 1);
            index++;
        }
    }
    for (s32 j = 0; j < rest; j++) {
        parts[fullParts].overwriteAt(j, text.getPart(index), 1);
        index++;
    }

    // each part into its box, the font narrowed until it fits
    for (s32 i = 0; i < ADDRESS_PART_NUM; i++) {
        words[i].Set(reinterpret_cast<const char16*>(parts[i].getCString()), ADDRESS_PART_LENGTH + 1);
        m_pAddressParts[i]->SetText(&words[i]);
        m_pAddressParts[i]->CommitText();
        g_FormatBuffer.format("%s_%02d", "T_address", i);
        nw::lyt::TextBox* textBox = m_Layout.FindTextBox(g_FormatBuffer.getCString());
        nw::lyt::Size fontSize = m_AddressFontSize;
        const f32 width = textBox->m_Size.width;
        textBox->SetFontSize(fontSize);
        while (m_pAddressParts[i]->MeasureWidth() > width) {
            fontSize.width *= width / m_pAddressParts[i]->MeasureWidth();
            textBox->SetFontSize(fontSize);
        }
    }
}

// 0x005084E4
void lyt::TextAreaOneLineComTitle::SetTitle()
{
    // the messages of SYS_2D_UI
    s32 title = -1;
    s32 subtitle = -1;
    switch (sys::g_Unknown95F0EB) {
    case 4:
        title = 165;
        break;
    case 5:
        title = 164;
        subtitle = 108;
        break;
    case 6:
        title = 166;
        break;
    case 7:
        title = 167;
        break;
    case 8:
        title = 168;
        break;
    case 9:
        title = 169;
        break;
    case 10:
        title = 171;
        break;
    case 11:
        title = 173;
        break;
    case 12:
        title = 172;
        subtitle = 108;
        break;
    case 13:
        title = 170;
        break;
    case 14:
        title = 184;
        break;
    case KIND_ADDRESS:
    case KIND_ADDRESS_2:
        title = 159;
        break;
    default:
        break;
    }
    script::WordFix<64> word;
    script::UiMgr::s_pInstance->Begin(NULL);
    script::UiMgr::s_pInstance->ReadWord(&word, "SYS_2D_UI", title, 0);
    script::UiMgr::s_pInstance->End();
    m_Title.SetText(&word);
    m_Title.CommitText();
    if (subtitle >= 0) {
        script::UiMgr::s_pInstance->Begin(NULL);
        script::UiMgr::s_pInstance->ReadWord(&word, "SYS_2D_UI", subtitle, 0);
        script::UiMgr::s_pInstance->End();
        m_Subtitle.SetText(&word);
        m_Subtitle.CommitText();
    }
}

// 0x005086F0 slot 0x10
void lyt::TextAreaOneLineComTitle::Setup(ssys::ma::lyt::ArcResAccReader* arcReader, ssys::ma::lyt::ArcResAccReader* decorArcReader)
{
    char name[64];
    std::sprintf(name, "%s.bclyt", GetLayoutName());
    Build(arcReader, name, 0x5000);

    // the title: one line in the western regions, with rubies in Japan
    if (GetRegionIndex() != 0) {
        nw::lyt::TextBox* text = m_Layout.FindTextBox("T_msg_00_W");
        SetFlagVisible(text, true);
        nw::lyt::TextBox* shadow = m_Layout.FindTextBox("S_msg_00_W");
        SetFlagVisible(shadow, true);
        m_Title.SetTextBoxes(text, NULL, shadow, NULL);
        SetFlagVisible(m_Layout.FindTextBox("T_msg_00"), false);
        SetFlagVisible(m_Layout.FindTextBox("RT_msg_00"), false);
        SetFlagVisible(m_Layout.FindTextBox("S_msg_00"), false);
        SetFlagVisible(m_Layout.FindTextBox("RS_msg_00"), false);
    } else {
        nw::lyt::TextBox* text = m_Layout.FindTextBox("T_msg_00");
        SetFlagVisible(text, true);
        nw::lyt::TextBox* textRuby = m_Layout.FindTextBox("RT_msg_00");
        SetFlagVisible(textRuby, true);
        nw::lyt::TextBox* shadow = m_Layout.FindTextBox("S_msg_00");
        SetFlagVisible(shadow, true);
        nw::lyt::TextBox* shadowRuby = m_Layout.FindTextBox("RS_msg_00");
        SetFlagVisible(shadowRuby, true);
        m_Title.SetTextBoxes(text, textRuby, shadow, shadowRuby);
        SetFlagVisible(m_Layout.FindTextBox("T_msg_00_W"), false);
        SetFlagVisible(m_Layout.FindTextBox("S_msg_00_W"), false);
    }
    m_Subtitle.SetTextBoxes(m_Layout.FindTextBox("T_mura_00"), m_Layout.FindTextBox("S_mura_00"));
    SetTitle();
    SetFlagVisible(FindPane("N_pin4"), false);
    SetFlagVisible(FindPane("N_address"), false);
    SetFlagVisible(FindPane("N_kabu"), false);
    SetFlagVisible(FindPane("N_bel"), false);
    SetFlagVisible(FindPane("N_text13"), false);
    SetFlagVisible(FindPane("N_text10"), false);
    SetFlagVisible(FindPane("N_text4"), false);
    SetFlagVisible(FindPane("N_text6"), false);
    SetFlagVisible(FindPane("N_text12"), false);

    // the text box and its decoration of the kind
    nw::lyt::Pane* textPane = NULL;
    nw::lyt::Pane* decorPane = NULL;
    switch (sys::g_Unknown95F0EB) {
    case 4:
    case 5:
    case 6:
    case 11:
    case 12:
        SetFlagVisible(FindPane("N_text6"), true);
        textPane = FindPane("T_text6");
        decorPane = FindPane("N_decor_01");
        m_pTextAreaGroup = m_Layout.GetLayout().GetGroupContainer()->FindGroupByName("G_textArea_6");
        break;
    case 7:
        if (GetRegion() != 0 && GetRegion() != 9) {
            // the longer box of the western regions
            SetFlagVisible(FindPane("N_text10"), true);
            textPane = FindPane("T_text10");
            const f32 height = textPane->m_Size.height;
            textPane->m_Size.width = 118.0f;
            FindPane("B_tex10")->m_Size = nw::lyt::Size(118.0f, height);
            nw::lyt::Pane* center = FindPane("P_bgC_02");
            center->m_Size = nw::lyt::Size(110.0f, center->m_Size.height);
            nw::lyt::Pane* right = FindPane("P_bgR_02");
            right->SetTranslate(nn::math::VEC3(110.0f, right->m_Translate.y, right->m_Translate.z));
            FindPane("P_line_02")->m_Size.width = 124.0f;
            decorPane = FindPane("N_decor_02");
            m_pTextAreaGroup = m_Layout.GetLayout().GetGroupContainer()->FindGroupByName("G_textArea_10");
        } else {
            SetFlagVisible(FindPane("N_text4"), true);
            textPane = FindPane("T_text4");
            decorPane = FindPane("N_decor_00");
            m_pTextAreaGroup = m_Layout.GetLayout().GetGroupContainer()->FindGroupByName("G_textArea_4");
        }
        break;
    case 8:
    case 9:
    case 10:
    case 13:
    case 14:
        if (GetRegion() != 0 && GetRegion() != 9) {
            SetFlagVisible(FindPane("N_text13"), true);
            textPane = FindPane("T_text13");
            decorPane = FindPane("N_decor_04");
            m_pTextAreaGroup = m_Layout.GetLayout().GetGroupContainer()->FindGroupByName("G_textArea_13");
        } else {
            SetFlagVisible(FindPane("N_text12"), true);
            textPane = FindPane("T_text12");
            decorPane = FindPane("N_decor_03");
            m_pTextAreaGroup = m_Layout.GetLayout().GetGroupContainer()->FindGroupByName("G_textArea_12");
        }
        break;
    case KIND_ADDRESS:
    case KIND_ADDRESS_2: {
        // the address in three boxes of four characters
        SetFlagVisible(FindPane("N_address"), true);
        textPane = FindPane("T_text12");
        decorPane = FindPane("N_decor_03");
        m_pTextAreaGroup = m_Layout.GetLayout().GetGroupContainer()->FindGroupByName("G_textArea_12");
        for (s32 i = 0; i < ADDRESS_PART_NUM; i++) {
            m_pAddressParts[i] = new (std::nothrow) script::RenderGarden(ADDRESS_PART_LENGTH + 1);
            g_FormatBuffer.format("%s_%02d", "T_address", i);
            m_pAddressParts[i]->SetTextBoxes(m_Layout.FindTextBox(g_FormatBuffer.getCString()), NULL);
        }
        nw::lyt::TextBox* first = nw::lyt::DynamicCast<nw::lyt::TextBox>(FindPane("T_address_00"));
        m_AddressFontSize = first->m_FontSize;
        break;
    }
    default:
        break;
    }
    if (decorPane != NULL) {
        decorPane->SetName("N_decor");
    }
    if (decorArcReader != NULL) {
        CreateDecor(decorArcReader, g_DecorPaneName);
    } else {
        CreateDecor(arcReader, g_DecorPaneName);
    }

    // one line of at most the maximum length, as TextAreaOneLine
    m_pTextBox = nw::lyt::DynamicCast<nw::lyt::TextBox>(textPane);
    const nw::lyt::Size size = m_pTextBox->m_Size;
    m_pTextBox->m_Size = nw::lyt::Size(size.width + 16.0f, size.height);
    sys::g_LineNum = 1;
    sys::GetLineManager()->m_LineNum = 1;
    sys::GetLineManager()->Setup(m_pTextBox, sys::g_KeyboardParam.maxLength, false, m_pTextBox->m_Size.width - 16.0f);
    m_Writer.m_pFont = m_pTextBox->m_pFont;
    m_Writer.SetFontSize(m_pTextBox->m_FontSize.width, m_pTextBox->m_FontSize.height);
    m_Writer.m_LineSpace = m_pTextBox->m_LineSpace;
    m_Writer.m_CharSpace = m_pTextBox->m_CharSpace;
    m_pTextBox->m_Size.width += 3.0f;
    m_pTextBox->SetTagProcessor(&m_TagProcessor);
    m_Layout.Animate();

    // from the lower screen into the text box
    nn::math::MTX34 inverse;
    if (nn::math::ARMv6::MTX34InverseAsm(&inverse, &m_pTextBox->m_GlbMtx)) {
        nn::math::MTX44 mtx;
        nn::math::ARMv6::MTX34CopyAsm(reinterpret_cast<nn::math::MTX34*>(&mtx), &inverse);
        mtx.m[3][2] = 0.0f;
        mtx.m[3][1] = 0.0f;
        mtx.m[3][0] = 0.0f;
        mtx.m[3][3] = 1.0f;
        m_InvMtx = mtx;
    } else {
        nn::math::MTX44SetIdentity(&m_InvMtx);
    }

    // the string buffer of the box: the characters that fit (estimated with narrow ones)
    s32 length = static_cast<s32>(std::ceil(sys::GetLineManager()->m_Width / (GetCharWidth('i') + m_Writer.m_CharSpace)));
    length *= GetLineNum();
    if (length > sys::g_BufferSize - 1) {
        length = sys::g_BufferSize - 1;
    }
    m_pTextBox->AllocStringBuffer(static_cast<u16>(length), 0);
    m_TextColor = m_pTextBox->m_TextColors[0];
    LoadInitialText();
    if (m_pGuideTextBox != NULL) {
        SetText(reinterpret_cast<const wchar_t*>(sys::g_KeyboardParam._unknown0x090), m_pGuideTextBox, false, 0, 0);
    }
    sys::g_pTextManager->SetCursor(sys::g_pTextManager->m_Length, true, true);
}

// 0x005095C4 slot 0x00
// 0x00509530 (deleting dtor)
lyt::TextAreaOneLineComTitle::~TextAreaOneLineComTitle()
{
    for (s32 i = 0; i < ADDRESS_PART_NUM; i++) {
        if (m_pAddressParts[i] != NULL) {
            m_pAddressParts[i]->Clear();
            delete m_pAddressParts[i];
            m_pAddressParts[i] = NULL;
        }
    }
}
