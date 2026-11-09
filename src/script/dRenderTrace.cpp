#include "script/dRenderTrace.h"
#include "script/dChip.h"
#include "script/dRenderBase.h"
#include "script/dRenderTag.h"
#include <cstring>

namespace script {
// 0x005DFB20 slot 0x14
void RenderTrace::OnChar(const CharInfo* info)
{
    m_pMain->Put(info);
    if (info->units == '\n') {
        if (m_pSub != NULL) {
            m_pSub->Put(info);
        }
        m_LineNum++;
    }
}

// 0x005DFB90
void RenderTrace::OnRuby(const Tag* tag)
{
    if (m_pSub == NULL) {
        return;
    }
    // the ruby base follows the tag; the ruby is in the parameters (behind the sizes of both texts in bytes)
    const char16* base = NULL;
    const char16* baseEnd = NULL;
    const char16* ruby = NULL;
    const char16* rubyEnd = NULL;
    base = tag->data + tag->length / sizeof(char16);
    baseEnd = base + tag->args[0] / sizeof(char16);
    ruby = tag->args + 2;
    rubyEnd = ruby + tag->args[1] / sizeof(char16);

    f32 baseWidth = m_pTextBox != NULL ? Chip::MeasureWidth(m_pTextBox, &base, &baseEnd) : 0.0f;
    f32 rubyWidth = m_pTextBoxSub != NULL ? Chip::MeasureWidth(m_pTextBoxSub, &ruby, &rubyEnd) : 0.0f;

    Tag baseTag = GetStoreTag(baseWidth);
    m_pMain->Put(&baseTag);
    Tag rubyTag = GetStoreTag(rubyWidth);
    m_pSub->Put(&rubyTag);

    RenderBase* sub = m_pSub;
    s32 num = rubyEnd - ruby;
    char16* end = sub->m_pBuf->pText + num;
    if (end <= sub->m_pEnd && num > 0) {
        std::memcpy(sub->m_pBuf->pText, ruby, num * sizeof(char16));
        sub->m_pBuf->pText = end;
        *sub->m_pBuf2 = 1;
    }
}

// 0x005DFD58 slot 0x0C
void RenderTrace::End()
{
    // empty (in the original too)
}

// 0x005DFD5C slot 0x18
void RenderTrace::OnTag(const Tag* tag)
{
    if (tag->group == 0) {
        switch (tag->index) {
        case 0:
            OnRuby(tag);
            break;
        case 3:
            // the colors go into both texts
            m_pMain->Put(tag);
            if (m_pSub != NULL) {
                m_pSub->Put(tag);
            }
            break;
        case 2:
            m_pMain->Put(tag);
            break;
        }
    } else if (tag->group == 0xFF && tag->index == 6) {
        m_pMain->Put(tag);
    }
}

// 0x005DFE20 slot 0x08
void RenderTrace::Begin()
{
    m_pMain->Clear();
    if (m_pSub != NULL) {
        m_pSub->Clear();
    }
    m_LineNum = 0;
}

// 0x005DFE48 slot 0x10
bool RenderTrace::IsContinue()
{
    return true;
}

// 0x005DFE60 slot 0x00
// 0x005DFE50 (deleting dtor)
RenderTrace::~RenderTrace()
{
    // empty (in the original too)
}
} // namespace script
