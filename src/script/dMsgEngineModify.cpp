#include "script/dMsgEngineModify.h"

namespace script {
// 0x0095EF30 (name is ours)
char16* MsgEngineModify::s_pNoStop;

// 0x005E701C (name is ours)
void MsgEngineModify::Run(char16* const* stop)
{
    if (m_pCur == NULL) {
        return;
    }
    for (;;) {
        if (m_pCur == *stop) {
            End();
            return;
        }
        if (!IsContinue()) {
            return;
        }
        CharInfo info(m_pCur);
        if (info.units == 0) {
            End();
            return;
        }
        if (info.units == Tag::TAG_BEGIN) {
            const char16* text = m_pCur;
            Tag tag = Tag::GetTag(&text);
            m_pChar = m_pCur;
            m_pCur += tag.length / sizeof(char16);
            OnTag(&tag);
        } else if (info.units == Tag::TAG_END) {
            // (the original does not advance here)
            continue;
        } else {
            m_pChar = m_pCur;
            m_pCur += info.size / sizeof(char16);
            OnChar(&info);
        }
    }
}

// 0x005E7124 slot 0x0C
void MsgEngineModify::End()
{
    // empty (in the original too)
}

// 0x005E7128 slot 0x18
void MsgEngineModify::OnTag(const Tag*)
{
    // empty (in the original too)
}

// 0x005E712C slot 0x08
void MsgEngineModify::Begin()
{
    // empty (in the original too)
}

// 0x005E7130 slot 0x10
bool MsgEngineModify::IsContinue()
{
    return true;
}
} // namespace script
