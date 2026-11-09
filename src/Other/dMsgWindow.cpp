#include "Other/dMsgWindow.h"

// 0x006E751C
MsgWindow::MsgWindow()
    : m_pTextBox(NULL), m_pTextBox2(NULL), m_pRubyTextBox(NULL), m_pRubyTextBox2(NULL), m_pNameTextBox(NULL),
      m_pSpecialTextBox(NULL), m_pShakePane(NULL), m_IsNameShown(1), m_Unknown0xAED(0), m_Unknown0xAEE(0),
      m_Unknown0xAEF(0), m_Unknown0xAF0(7), m_Unknown0xAF1(2)
{
    for (u32 i = 0; i < 5; i++) {
        m_Unknown0xAD8[i] = 0;
    }
    for (u32 i = 0; i < EXTRA_TEXT_BOX_NUM; i++) {
        m_pSpecialTextBoxes[i] = NULL;
    }
}

// 0x006E7754 slot 0x00
// 0x006E76A8 (deleting dtor)
MsgWindow::~MsgWindow()
{
    // (only the members are destroyed)
}

// 0x0082D7B8 slot 0x08
const char* MsgWindow::GetName(Func func) const
{
    return FindName(func);
}

// 0x006E6F84 (name is ours)
bool MsgWindow::Setup()
{
}

// 0x006E7014 (name is ours)
void MsgWindow::Release()
{
}

// 0x006E6AA8 (name is ours)
void MsgWindow::SetWaitIconVisible(bool)
{
}

// 0x00770A18 (name is ours)
bool MsgWindow::Func770A18() const
{
}
