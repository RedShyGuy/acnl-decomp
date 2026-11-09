#include "script/dMsgEngine.h"
#include "script/dIWord.h"
#include <cstring>

namespace script {
// 0x0095EF34 (name is ours)
const char16* MsgEngine::s_pNoStop;

// 0x005ED460 (name is ours)
CharInfo::CharInfo(const char16* text) : pText(text), units(0), code(0), size(0)
{
    u32 c = text[0];
    u32 charSize = sizeof(char16);
    u32 charUnits = c;
    u32 charCode = c;
    if (c >= 0xD800 && c - 0xE000 >= 0x2000) {
        // a surrogate pair
        charSize = 2 * sizeof(char16);
        charUnits = (c << 16) | text[1];
        charCode = (charUnits & 0x3FF) | (((charUnits >> 6) + 0x10000) & 0xF0000) | ((charUnits >> 6) & 0xFC00);
    }
    units = charUnits;
    code = charCode;
    size = charSize;
}

// 0x005D855C (name after libgarden)
Tag Tag::GetTag(const char16* const* text)
{
    Tag tag;
    tag.data = NULL;
    tag.group = 0x7FFFFFFF;
    tag.index = -1;
    tag.paramSize = 0;
    tag.args = NULL;
    tag.length = 0;
    const char16* data = *text;
    if (data != NULL) {
        tag.data = data;
        tag.group = data[1];
        tag.index = data[2];
        tag.paramSize = data[3];
        tag.args = *text + 4;
        tag.length = tag.paramSize + 8;
    }
    return tag;
}

// 0x0060230C (name is ours)
MsgEngine::Range::Range() : pCur(NULL), pEnd(NULL)
{
}

// 0x006024F0 (name is ours)
MsgEngine::MsgEngine() : m_pCur(NULL), m_pEnd(NULL), m_StackNum(0)
{
    for (u32 i = 0; i < 10; i++) {
        m_Stack[i].pCur = NULL;
        m_Stack[i].pEnd = NULL;
    }
}

// 0x00602558 slot 0x00
// 0x00602548 (deleting dtor)
MsgEngine::~MsgEngine()
{
    // empty (in the original too)
}

// 0x006024E4 slot 0x08
void MsgEngine::Begin()
{
    // empty (in the original too)
}

// 0x00602478 slot 0x0C
void MsgEngine::End()
{
    // empty (in the original too)
}

// 0x006024E8 slot 0x10
bool MsgEngine::IsContinue()
{
    return true;
}

// 0x00602304 slot 0x14
void MsgEngine::OnChar(const CharInfo*)
{
    // empty (in the original too)
}

// 0x0060247C slot 0x18
void MsgEngine::OnTag(const Tag*)
{
    // empty (in the original too)
}

// 0x00602308 slot 0x1C
void MsgEngine::OnTagEnd()
{
    // empty (in the original too)
}

// 0x00602480 | libgarden [tier A]
void MsgEngine::SetText(char16_t const* const* begin, char16_t const* const* end)
{
    ResetText();
    m_pCur = *begin;
    m_pEnd = *end;
    Begin();
}

// 0x0060231C | libgarden [tier A]
void MsgEngine::SetText2(char16_t const* const* stop)
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
        if (info.units == 0 || m_pCur == m_pEnd) {
            // the end of this part: continue the interrupted text
            if (m_StackNum == 0) {
                End();
                return;
            }
            m_pCur = m_Stack[m_StackNum - 1].pCur;
            m_pEnd = m_Stack[m_StackNum - 1].pEnd;
            m_StackNum--;
            m_Stack[m_StackNum].pCur = NULL;
            m_Stack[m_StackNum].pEnd = NULL;
        } else if (info.units == Tag::TAG_BEGIN) {
            Tag tag = Tag::GetTag(&m_pCur);
            m_pCur += tag.length / sizeof(char16);
            OnTag(&tag);
        } else if (info.units == Tag::TAG_END) {
            m_pCur += 3;
            OnTagEnd();
        } else {
            m_pCur += info.size / sizeof(char16);
            OnChar(&info);
        }
    }
}

// 0x0060224C (name is ours)
void MsgEngine::PushText(char16_t const* const* begin, char16_t const* const* end)
{
    if (*begin == NULL || m_StackNum >= 10) {
        return;
    }
    Range* range = &m_Stack[m_StackNum++];
    range->pCur = m_pCur;
    range->pEnd = m_pEnd;
    m_pCur = *begin;
    m_pEnd = *end;
}

// 0x006022A0 (name is ours)
void MsgEngine::PushWord(const IWord* word)
{
    const char16* text = word->GetWord();
    if (text == NULL || m_StackNum >= 10) {
        return;
    }
    Range* range = &m_Stack[m_StackNum++];
    range->pCur = m_pCur;
    range->pEnd = m_pEnd;
    m_pCur = text;
    m_pEnd = s_pNoStop;
}

// 0x005EACEC (name is ours)
bool PutChar(char16** dst, char16* const* last, const CharInfo* info)
{
    u32 units = info->units;
    switch (info->size / sizeof(char16)) {
    case 1:
        if (*dst + 1 > *last) {
            return false;
        }
        **dst = units;
        *dst += 1;
        return true;
    case 2:
        if (*dst + 2 > *last) {
            return false;
        }
        std::memcpy(*dst, &units, sizeof(units));
        *dst += 2;
        return true;
    }
    return false;
}

// 0x005EACA0 (name is ours)
bool PutTag(char16** dst, char16* const* last, const Tag* tag)
{
    u32 num = tag->length / sizeof(char16);
    char16* end = *dst + num;
    if (end > *last) {
        return false;
    }
    std::memcpy(*dst, tag->data, num * sizeof(char16));
    *dst = end;
    return true;
}
} // namespace script
