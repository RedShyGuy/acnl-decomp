#pragma once

#include "decomp.h"
#include "nn/math/math_Vector2.h"
#include "nn/math/math_Vector3.h"
#include "Other/dMsgWindow.h"
#include "nw/lyt/lyt_Pane.h"
#include "nw/lyt/lyt_TextBox.h"

namespace nw {
namespace lyt {
class TextBox;
} // namespace lyt
} // namespace nw

namespace script {
class RenderBase;

// The window of the conversations: the message window of the game with the text boxes of the main and ruby renderers
// (as their tag processors) and the shaking of the window. Member names are ours.
// RTTI N6script7TalkWinE @ 0x008D3B10
// vtable 0x0090A99C (vptr 0x0090A9A4), offset_to_top 0, 2 entries
class TalkWin
{
public:
    // The shaking of a pane: a damped spring from an impulse (names are ours).
    struct Shake
    {
        // the strengths of the shakes (TalkSeq starts them)
        struct Param
        {
            nn::math::VEC2 impulse;  // 0x00
            nn::math::VEC2 spring;   // 0x08
            nn::math::VEC2 damping;  // 0x10
        };
        static const u8 STATE_NONE = 3;

        // the spring moves the pane until it rests
        void Update(); // 0x005FCA8C
        // (inline) the pane rests at once
        void Stop()
        {
            state = STATE_NONE;
            offset.x = 0.0f;
            offset.y = 0.0f;
            velocity.x = 0.0f;
            velocity.y = 0.0f;
            if (pPane != NULL) {
                pPane->m_Translate.x = base.x;
                pPane->m_Translate.y = base.y;
                pPane->m_Translate.z = base.z;
                pPane->m_Flag &= ~nw::lyt::Pane::FLAG_MTX_CONDITION_MASK;
            }
        }

        nw::lyt::Pane* pPane;     // 0x00
        nn::math::VEC3 base;      // 0x04 the translation of the pane at rest
        u8 state;                 // 0x10 the parameter set (STATE_NONE: resting)
        nn::math::VEC2 offset;    // 0x14
        nn::math::VEC2 velocity;  // 0x1C
    };

    TalkWin(RenderBase* pMain, RenderBase* pRuby); // 0x005FD22C
    virtual ~TalkWin();

    // the window is prepared and the text boxes get the renderers; true when ready
    bool Setup(); // 0x005FCCBC
    // the texts of the renderers into the text boxes, the shaking and the window
    void Update(); // 0x005FD028
    // (inline) the text boxes and the window are released
    void Release()
    {
        nw::lyt::TextBox* textBox = m_Window.m_pTextBox;
        nw::lyt::TextBox* textBox2 = m_Window.m_pTextBox2;
        nw::lyt::TextBox* rubyTextBox = m_Window.m_pRubyTextBox;
        nw::lyt::TextBox* rubyTextBox2 = m_Window.m_pRubyTextBox2;
        m_Shake.pPane = NULL;
        if (m_IsSpecialSetup) {
            if (m_Window.m_pSpecialTextBox != NULL) {
                m_Window.m_pSpecialTextBox->FreeStringBuffer();
            }
            for (s32 i = 0; m_Window.m_pSpecialTextBoxes[i] != NULL; i++) {
                m_Window.m_pSpecialTextBoxes[i]->FreeStringBuffer();
            }
            m_IsSpecialSetup = 0;
        }
        if (textBox != NULL) {
            textBox->FreeStringBuffer();
        }
        if (textBox2 != NULL) {
            textBox2->FreeStringBuffer();
        }
        if (rubyTextBox != NULL) {
            rubyTextBox->FreeStringBuffer();
        }
        if (rubyTextBox2 != NULL) {
            rubyTextBox2->FreeStringBuffer();
        }
        m_Window.Release();
        m_State = 0;
        m_IsReady = 0;
    }
    // (inline) the text box of the main text (the extra one of Stage 101/102 when in use)
    nw::lyt::TextBox* GetTextBox() const { return m_IsSpecial ? m_Window.m_pSpecialTextBox : m_Window.m_pTextBox; }

private:
    // the text of the main renderer into a text box (if a text was written)
    DECOMP_NOINLINE void CommitText(nw::lyt::TextBox* pTextBox); // 0x005FCBE0

public:
    RenderBase* m_pMain;   // 0x004
    RenderBase* m_pRuby;   // 0x008
    u8 m_IsSpecialSetup;   // 0x00C the extra text boxes of Stage 101/102 are set up
    u8 m_IsSpecial;        // 0x00D the text goes into the extra text boxes
    u8 m_OpenSound;        // 0x00E the sound of the opening (< 5; else by m_WindowKind)
    u8 m_CloseSound;       // 0x00F the sound of the closing (< 3; else the default)
    u8 m_State;            // 0x010 1 → 2 and 3 → 0 when the window leaves its state 0x0087BA9C
    u8 m_IsReady;          // 0x011
    MsgWindow m_Window;    // 0x014
    Shake m_Shake;         // 0xB08
    u8 m_WindowKind;       // 0xB2C the window of the speaker (TalkSeq::s_SpeakerWindows; 11 by default)

    static const Shake::Param s_ShakeParams[3];
};
ASSERT_SIZE(TalkWin, 0xB30);
} // namespace script
