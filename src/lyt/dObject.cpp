#include "lyt/dObject.h"

#include <cmath>
#include <cwchar>
#include <cstring>

#include "libms/lms_Api.h"
#include "lyt/dAnmObject.h"
#include "sys/dSwkbd.h"
#include "lyt/dTagProcessor.h"
#include "nw/font/font_TextWriterBase.h"
#include "nw/lyt/lyt_AnimResource.h"
#include "nw/lyt/lyt_Group.h"
#include "nw/lyt/lyt_GroupContainer.h"
#include "nw/lyt/lyt_Material.h"
#include "nw/lyt/lyt_TextBox.h"
#include "sead/seadHeap.h"
#include "sead/seadNew.h"
#include "ssys/ma/dAllocator.h"
#include "ssys/ma/lyt/dArcResAccReader.h"
#include "ssys/ma/lyt/dLayoutMgr.h"
#include "ssys/ma/lyt/lyt_Api.h"
#include "ut/dFrameController.h"

namespace lyt {
// the globals of the objects (a statically initialized block of several files builds the list, the iterator and the
// tag processor)
// 0x00AD7600
lyt::Object::List lyt::Object::s_List;
// 0x0095F19C
lyt::Object::List::iterator lyt::Object::s_DrawIterator(NULL);
// 0x0095F124
lyt::Object::DrawCallback lyt::Object::s_pDrawCallback = NULL;
// 0x0095F128
void* lyt::Object::s_DrawCallbackArg = NULL;
// 0x0095F0ED
bool lyt::Object::s_IsSetTagProcessor = false;
// 0x00AD760C
lyt::TagProcessor lyt::Object::s_TagProcessor;

// 0x00509650
void lyt::Object::SetGroupVisible(const char* groupName, bool visible)
{
    nw::lyt::Group* group = m_Layout.GetLayout().GetGroupContainer()->FindGroupByName(groupName);
    nw::lyt::Group::PaneLinkList& paneList = group->GetPaneList();
    for (nw::lyt::Group::PaneLinkList::iterator it = paneList.begin(); it != paneList.end(); ++it) {
        nw::lyt::Group::PaneLinkList::GetPointerFromNode(it.GetNode())->m_pTarget->SetVisible(visible);
    }
    m_State = STATE_CHANGED;
}

// 0x005096B8
void lyt::Object::TruncateText(wchar_t* dst, int dstLength, const wchar_t* src, nw::lyt::TextBox* textBox, int lineNum)
{
    if (lineNum <= 0) {
        std::swprintf(dst, dstLength, L"%ls", src);
        return;
    }
    nw::font::TextWriterBase<wchar_t> writer;
    writer.m_pFont = textBox->m_pFont;
    writer.SetFontSize(textBox->m_FontSize.width, textBox->m_FontSize.height);
    writer.m_LineSpace = textBox->m_LineSpace;
    writer.m_CharSpace = textBox->m_CharSpace;
    writer.m_WidthLimit = textBox->m_Size.width;
    if (textBox->m_pTagProcessor != NULL) {
        writer.m_pTagProcessor = textBox->m_pTagProcessor;
    }
    const f32 height = writer.CalcStringHeight(src, ut::GetTextLength(src));
    const f32 maxHeight = textBox->m_FontSize.height * lineNum + textBox->m_LineSpace * (lineNum - 1);
    if (height > maxHeight) {
        for (int i = 0; i < dstLength; i++) {
            dst[i] = src[i];
            if (writer.CalcStringHeight(dst, i) > maxHeight) {
                dst[i - 1] = L'\0';
                break;
            }
        }
    } else {
        std::swprintf(dst, dstLength, L"%ls", src);
    }
}

// 0x00509818
void lyt::Object::SetPaneVisible(bool visible, nw::lyt::Pane* pane)
{
    if (pane->IsVisible() != visible) {
        pane->m_Flag = (pane->m_Flag & ~nw::lyt::Pane::FLAG_VISIBLE) | visible;
        for (nw::lyt::Pane::PaneList::iterator it = pane->m_ChildList.begin(); it != pane->m_ChildList.end(); ++it) {
            SetPaneVisible(visible, nw::lyt::Pane::FromLink(it.GetNode()));
        }
        m_State = STATE_CHANGED;
    }
}

// 0x00509884
lyt::AnmObject* lyt::Object::CreateAnmObject(const char* animName, const char* groupName)
{
    AnmObject* anmObject = NULL;
    const void* res = m_pArcReader->GetResource(0, animName);
    if (res != NULL) {
        if (groupName == NULL) {
            nw::lyt::AnimResource animRes;
            animRes.Set(res);
            groupName = animRes.GetGroupArray()->name;
        }
        nw::lyt::Group* group = m_Layout.GetLayout().GetGroupContainer()->FindGroupByName(groupName);
        if (group != NULL) {
            nw::lyt::AnimTransform* animTransform = m_Layout.CreateAnimTransform(res, m_pArcReader->GetResourceAccessor());
            anmObject = new (m_pHeap, 4) AnmObject(animTransform, group, false);
            m_AnmObjectList.PushBack(anmObject);
        }
    }
    return anmObject;
}

// 0x0050994C
void lyt::Object::SetTagProcessor(nw::lyt::Pane* pane, nw::font::TagProcessorBase<wchar_t>* tagProcessor)
{
    for (nw::lyt::Pane::PaneList::iterator it = pane->m_ChildList.begin(); it != pane->m_ChildList.end(); ++it) {
        nw::lyt::Pane* child = nw::lyt::Pane::FromLink(it.GetNode());
        nw::lyt::TextBox* textBox = nw::lyt::DynamicCast<nw::lyt::TextBox>(child);
        if (textBox != NULL) {
            textBox->SetTagProcessor(tagProcessor);
        }
        SetTagProcessor(child, tagProcessor);
    }
}

// 0x005099F4
void lyt::Object::FitFontSize(nw::lyt::TextBox* textBox, const wchar_t* text, nw::lyt::Size* originalSize)
{
    bool isNew = true;
    for (FontSizeList::iterator it = m_FontSizeList.begin(); it != m_FontSizeList.end(); ++it) {
        FontSizeEntry* entry = FontSizeList::GetPointerFromNode(it.GetNode());
        if (entry->pTextBox == textBox) {
            textBox->SetFontSize(entry->size);
            isNew = false;
            break;
        }
    }
    *originalSize = textBox->m_FontSize;
    if (isNew) {
        FontSizeEntry* entry = new (m_pHeap, 4) FontSizeEntry;
        entry->pTextBox = textBox;
        entry->size = *originalSize;
        m_FontSizeList.PushBack(entry);
    }

    nw::font::TextWriterBase<wchar_t> writer;
    writer.m_pFont = textBox->m_pFont;
    writer.SetFontSize(textBox->m_FontSize.width, textBox->m_FontSize.height);
    writer.m_LineSpace = textBox->m_LineSpace;
    writer.m_CharSpace = textBox->m_CharSpace;
    if (textBox->m_pTagProcessor != NULL) {
        writer.m_pTagProcessor = textBox->m_pTagProcessor;
    }
    const f32 width = writer.CalcStringWidth(text, ut::GetTextLength(text));
    if (width > textBox->m_Size.width) {
        f32 percent = 0.0f;
        modff(textBox->m_Size.width / width * 100.0f, &percent);
        if (percent < 80.0f) {
            percent = 80.0f;
        }
        const f32 scale = percent * 0.01f;
        textBox->SetFontSize(nw::lyt::Size(originalSize->width * scale, originalSize->height));
    }
}

// 0x00509C38 slot 0x0C
void lyt::Object::Draw()
{
    if (m_pParent == NULL) {
        ssys::ma::lyt::LayoutMgr::Get()->Register(&m_Layout, true);
    }
}

// 0x00509C64
void lyt::Object::Build(ssys::ma::lyt::ArcResAccReader* arcReader, const char* name, u32 cmdCacheSize)
{
    m_pArcReader = arcReader;
    m_pChildren = new (m_pHeap, 4) ChildList;
    m_Layout.Build(name, arcReader, cmdCacheSize);

    List::iterator it = s_List.begin();
    for (; it != s_List.end(); ++it) {
        if (m_Priority > List::GetPointerFromNode(it.GetNode())->m_Priority) {
            break;
        }
    }
    s_List.insert(it, &m_Link);

    if (s_IsSetTagProcessor) {
        SetTagProcessor(GetRootPane(), &s_TagProcessor);
    }
    m_Layout.Animate();
}

// 0x00509D40 slot 0x08
void lyt::Object::Calc(bool isAnimate)
{
    if (m_State == STATE_FROZEN) {
        return;
    }
    for (AnmObjectList::iterator it = m_AnmObjectList.begin(); it != m_AnmObjectList.end(); ++it) {
        m_IsAnimated |= AnmObjectList::GetPointerFromNode(it.GetNode())->Calc();
    }
    if (isAnimate) {
        if (m_IsAnimated) {
            m_Layout.GetLayout().Animate(1);
        }
        if (m_State == STATE_CHANGED || m_IsAnimated == true) {
            m_Layout.Animate();
        }
    } else if (m_IsAnimated && m_pParent != NULL) {
        m_pParent->m_IsAnimated = true;
    }
    m_IsAnimated = false;
    m_State = STATE_NONE;
}

// 0x00509E14
void lyt::Object::SetMessage(const char* label, nw::lyt::TextBox* textBox, const wchar_t* text, bool isFit, int lineNum)
{
    const LMS_BinaryBlock* styleBlock = LMS_GetBlock(&sys::g_pMessage->common, "TSY1");
    const s32 index = LMS_GetTextIndexByLabel(sys::g_pMessage, label);
    const s32 styleIndex = reinterpret_cast<const s32*>(styleBlock->data)[index];
    const sys::MessageStyle* style = styleIndex == -1 ? NULL : &sys::g_pMessageStyles->styles[styleIndex];
    if (text == NULL) {
        text = static_cast<const wchar_t*>(LMS_GetTextByLabel(sys::g_pMessage, label));
    }
    if (style != NULL) {
        textBox->m_pFont->GetWidth();
        textBox->m_pFont->GetHeight();
        textBox->SetLineSpace(style->lineSpace);
        textBox->SetCharSpace(style->charSpace);
    }

    nw::lyt::Size originalSize;
    if (isFit) {
        FitFontSize(textBox, text, &originalSize);
    }
    if (lineNum > 0) {
        const u16 length = ut::GetTextLength(text);
        wchar_t* buffer = new (m_pHeap, 4) wchar_t[length + 1];
        if (buffer != NULL) {
            std::memcpy(buffer, text, (length + 1) * sizeof(wchar_t));
            TruncateText(buffer, length + 1, text, textBox, lineNum);
            const u16 truncatedLength = ut::GetTextLength(buffer);
            textBox->AllocStringBuffer(truncatedLength, 0);
            textBox->SetString(buffer, 0, truncatedLength);
            delete[] buffer;
        }
    } else {
        const u16 length = ut::GetTextLength(text);
        textBox->AllocStringBuffer(length, 0);
        textBox->SetString(text, 0, length);
    }
}

// 0x0050A044
void lyt::Object::SetMessage(const char* label, const char* paneName, const wchar_t* text, bool isFit, int lineNum)
{
    nw::lyt::TextBox* textBox = nw::lyt::DynamicCast<nw::lyt::TextBox>(GetRootPane()->FindPaneByName(paneName, true));
    if (textBox != NULL) {
        SetMessage(label, textBox, text, isFit, lineNum);
    }
}

// 0x0050A0E4
void lyt::Object::SetText(const wchar_t* text, const char* paneName, bool isFit, u16 length, int lineNum)
{
    nw::lyt::TextBox* textBox = nw::lyt::DynamicCast<nw::lyt::TextBox>(GetRootPane()->FindPaneByName(paneName, true));
    if (textBox != NULL) {
        SetText(text, textBox, isFit, length, lineNum);
    }
}

// 0x0050A184
void lyt::Object::SetText(const wchar_t* text, nw::lyt::TextBox* textBox, bool isFit, u16 length, int lineNum)
{
    nw::lyt::Size originalSize;
    if (textBox->m_UserData[0] == '!') {
        isFit = true;
    }
    if (isFit) {
        FitFontSize(textBox, text, &originalSize);
    }
    if (lineNum > 0) {
        if (length == 0) {
            length = ut::GetTextLength(text);
        }
        wchar_t* buffer = new (m_pHeap, 4) wchar_t[length + 1];
        if (buffer != NULL) {
            std::memcpy(buffer, text, (length + 1) * sizeof(wchar_t));
            TruncateText(buffer, length + 1, text, textBox, lineNum);
            length = ut::GetTextLength(buffer);
            textBox->AllocStringBuffer(length, 0);
            textBox->SetString(buffer, 0, length);
            delete[] buffer;
        }
    } else {
        if (length == 0) {
            length = ut::GetTextLength(text);
        }
        textBox->AllocStringBuffer(length, 0);
        textBox->SetString(text, 0, length);
    }
}

// 0x0050A2DC
lyt::Object::Object(u32 unknown, s32 priority, sead::Heap* heap)
    : m_pArcReader(NULL), m_pHeap(heap), m_pChildren(NULL), m_pParent(NULL), m_Unknown0x17C(unknown), m_Priority(priority), m_State(STATE_NONE),
      m_IsEnabled(true), m_IsAnimated(false), m_Unknown0x18A(0), m_Unknown0x19C(0), m_SoundPan(0.0f)
{
    if (m_pHeap == NULL) {
        m_pHeap = ssys::ma::lyt::GetAllocator()->m_pHeap;
    }
    std::memset(m_ParentPaneName, 0, sizeof(m_ParentPaneName));
}

// 0x0050A3B0 slot 0x00
// 0x0050A39C (deleting dtor)
lyt::Object::~Object()
{
    m_Layout.UnbindAllAnimation();
    for (AnmObjectList::iterator it = m_AnmObjectList.begin(); it != m_AnmObjectList.end();) {
        AnmObject* anmObject = AnmObjectList::GetPointerFromNode(it.GetNode());
        it = m_AnmObjectList.erase(it);
        anmObject->~AnmObject();
        m_pHeap->free(anmObject);
    }
    if (m_pChildren != NULL) {
        for (ChildList::iterator it = m_pChildren->begin(); it != m_pChildren->end();) {
            ChildList::GetPointerFromNode(it.GetNode())->ClearParent();
            it = m_pChildren->erase(it);
        }
    }
    if (m_pParent != NULL) {
        m_pParent->DetachChild(this);
    }
    if (m_pChildren != NULL) {
        m_pHeap->free(m_pChildren);
    }
    for (FontSizeList::iterator it = m_FontSizeList.begin(); it != m_FontSizeList.end();) {
        FontSizeEntry* entry = FontSizeList::GetPointerFromNode(it.GetNode());
        it = m_FontSizeList.erase(it);
        m_pHeap->free(entry);
    }
    s_List.Erase(this);
}

// 0x0050A57C
void lyt::Object::DrawAll(bool isRestart)
{
    if (s_pDrawCallback != NULL) {
        s_pDrawCallback(isRestart, s_DrawCallbackArg);
    }
    if (isRestart) {
        s_DrawIterator = s_List.begin();
    }
    while (s_DrawIterator != s_List.end()) {
        Object* object = List::GetPointerFromNode(s_DrawIterator.GetNode());
        if (object->m_IsEnabled) {
            object->Draw();
        }
        ++s_DrawIterator;
    }
}

// 0x0050A60C
void lyt::Object::DestroyAll()
{
    for (int i = 0; i < 2; i++) {
        for (List::iterator it = s_List.begin(); it != s_List.end();) {
            Object* object = List::GetPointerFromNode(it.GetNode());
            it = s_List.erase(it);
            delete object;
        }
    }
    s_pDrawCallback = NULL;
    s_DrawCallbackArg = NULL;
}

// 0x0050A68C
void lyt::Object::CalcAll()
{
    for (List::iterator it = s_List.begin(); it != s_List.end(); ++it) {
        Object* object = List::GetPointerFromNode(it.GetNode());
        if (object->m_IsEnabled) {
            object->Calc(object->m_Priority < PRIORITY_NO_ANIMATE);
        }
    }
}

// 0x0050A6EC
int lyt::Object::CreatePaneAnmObjects(nw::lyt::Pane* pane, const nw::lyt::AnimResource& animRes, AnmObject** anmObjects, int max)
{
    if (max == 0) {
        return 0;
    }
    int num = 0;
    nw::lyt::AnimationLink* link = pane->FindAnimationLinkSelf(animRes);
    if (link == NULL) {
        nw::lyt::Material* material = pane->GetMaterial();
        if (material != NULL) {
            link = material->FindAnimationLink(animRes);
        }
    }
    if (link != NULL) {
        AnmObject* anmObject = new (m_pHeap, 4) AnmObject(link->m_pAnimTransform, pane, NULL, true);
        *anmObjects = anmObject;
        m_AnmObjectList.PushBack(anmObject);
        num = 1;
    }
    for (nw::lyt::Pane::PaneList::iterator it = pane->m_ChildList.begin(); it != pane->m_ChildList.end(); ++it) {
        num += CreatePaneAnmObjects(nw::lyt::Pane::FromLink(it.GetNode()), animRes, anmObjects + num, max - num);
    }
    return num;
}

} // namespace lyt
