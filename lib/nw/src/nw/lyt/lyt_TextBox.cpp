#include "nw/lyt/lyt_TextBox.h"

#include <cmath>
#include <cstring>
#include <cwchar>

#include "nn/gx/gx_Api.h"
#include "nn/math/math_ARMv6.h"
#include "nw/font/font_CharWriter.h"
#include "nw/font/font_DispStringBuffer.h"
#include "nw/font/font_TextWriterBase.h"
#include "nw/lyt/lyt_DrawInfo.h"
#include "nw/lyt/lyt_Drawer.h"
#include "nw/lyt/lyt_GraphicsResource.h"
#include "nw/lyt/lyt_Layout.h"
#include "nw/lyt/lyt_Material.h"
#include "nw/lyt/lyt_ResourceAccessor.h"
#include "nw/lyt/lyt_ResourceFormat.h"

namespace nw {
namespace lyt {
// 0x0097616C (name is ours)
const PaneTypeInfo nw::lyt::TextBox::s_TypeInfo(&Pane::s_TypeInfo);

// sets s_TypeInfo
// 0x00793028 (static initializer)

// 0x004BC5C4 | nintendogs:callseq-callee [tier A]
nw::lyt::TextBox::TextBox(const res::TextBox* pBlock, const ResBlockSet& resBlockSet, u32 allocFlags) : Pane(pBlock)
{
    u16 allocStrLen = pBlock->textBufBytes / sizeof(wchar_t);
    if (allocStrLen > 0) {
        allocStrLen -= 1;
    }
    Init(allocStrLen, allocFlags);

    for (int i = 0; i < 2; ++i) {
        m_TextColors[i] = pBlock->textCols[i];
    }
    m_FontSize = pBlock->fontSize;
    m_TextPosition = pBlock->textPosition;
    m_TextAlignment = pBlock->textAlignment;
    m_CharSpace = pBlock->charSpace;
    m_LineSpace = pBlock->lineSpace;

    const res::FontList* pFontList = resBlockSet.pFontList;
    const char* fontName = reinterpret_cast<const char*>(reinterpret_cast<uptr>(pFontList->nameOffsets) + pFontList->nameOffsets[pBlock->fontIdx]);
    m_pFont = resBlockSet.pResAccessor->GetFont(fontName);

    if (pBlock->textStrBytes >= sizeof(wchar_t) && m_TextBuf != nullptr) {
        const wchar_t* pBlockText = reinterpret_cast<const wchar_t*>(reinterpret_cast<uptr>(pBlock) + pBlock->textStrOffset);
        const u16 resStrLen = pBlock->textStrBytes / sizeof(wchar_t) - 1;
        SetStringImpl(pBlockText, 0, resStrLen);
    }

    const res::MaterialList* pMaterialList = resBlockSet.pMaterialList;
    const res::Material* pResMaterial =
        reinterpret_cast<const res::Material*>(reinterpret_cast<uptr>(pMaterialList) + pMaterialList->materialOffsets[pBlock->materialIdx]);
    m_pMaterial = Layout::NewObj<Material>(pResMaterial, resBlockSet);
}

// 0x004BC3A4 (name is ours)
void nw::lyt::TextBox::Init(u16 allocStrLen, u32 allocFlags)
{
    m_TextBuf = nullptr;
    m_TextBufBytes = 0;
    m_TextLen = 0;
    m_pFont = nullptr;
    m_FontSize = Size(0.0f, 0.0f);
    m_TextPosition = (m_TextPosition / 3) * 3 + POSITION_CENTER;
    m_TextPosition = m_TextPosition % 3 + POSITION_CENTER * 3;
    m_LineSpace = 0.0f;
    m_CharSpace = 0.0f;
    m_pTagProcessor = nullptr;
    m_pDispStringBuf = nullptr;
    m_TextAlignment = 0;
    m_IsTextChanged = false;

    AllocStringBuffer(allocStrLen, allocFlags);
}

// 0x004BC824 slot 0x00 | libgarden
// 0x004BC774 (deleting dtor)
nw::lyt::TextBox::~TextBox()
{
    if (m_pMaterial != nullptr && !m_pMaterial->IsUserAllocated()) {
        Layout::DeleteObj(m_pMaterial);
        m_pMaterial = nullptr;
    }
    FreeStringBuffer();
}

// 0x0073E784 slot 0x08 (name is ours)
const PaneTypeInfo* nw::lyt::TextBox::GetRuntimeTypeInfo() const
{
    return &s_TypeInfo;
}

// 0x0073E5E0 slot 0x0C | nintendogs:bytes
ut::Color8 nw::lyt::TextBox::GetVtxColor(u32 idx) const
{
    return m_TextColors[idx / 2];
}

// 0x004BBF98 slot 0x10
void nw::lyt::TextBox::SetVtxColor(u32 idx, ut::Color8 value)
{
    m_TextColors[idx / 2] = value;
}

// 0x0073E790 slot 0x1C | mk7dlp:bytes
u8 nw::lyt::TextBox::GetVtxColorElement(u32 idx) const
{
    return reinterpret_cast<const u8*>(&m_TextColors[idx / 8])[idx % 4];
}

// 0x004BC38C slot 0x20 | nintendogs:bytes
void nw::lyt::TextBox::SetVtxColorElement(u32 idx, u8 value)
{
    reinterpret_cast<u8*>(&m_TextColors[idx / 8])[idx % 4] = value;
}

// 0x0073E6B4 slot 0x24
u8 nw::lyt::TextBox::GetMaterialNum() const
{
    return m_pMaterial != nullptr ? 1 : 0;
}

// 0x0073E5D0 slot 0x28
Material* nw::lyt::TextBox::GetMaterial(u32 idx)
{
    return idx == 0 ? m_pMaterial : nullptr;
}

// 0x004BC508 slot 0x64 (name is ours)
void nw::lyt::TextBox::DrawSelf(DrawInfo& drawInfo)
{
    if (m_TextLen == 0 || m_pFont == nullptr || m_pMaterial == nullptr) {
        return;
    }

    GraphicsResource::FinalizeGraphics();
    GraphicsResource& graphicsResource = *drawInfo.m_pGraphicsResource;
    graphicsResource.ResetGlState();

    font::TextWriterBase<wchar_t> writer;
    writer.m_pTextWriterResource = &graphicsResource.m_TextWriterResource;
    SetupTextWriter(&writer);
    glUseProgram(graphicsResource.m_TextWriterResource.m_ProgramId);
    LoadMtx(drawInfo);
    writer.m_ColorMappingMin = m_pMaterial->m_Colors[Material::MATCOLOR_BLACK];
    writer.m_ColorMappingMax = m_pMaterial->m_Colors[Material::MATCOLOR_WHITE];
    writer.m_Alpha = m_GlbAlpha;
    writer.SetupGX();
    writer.Print(m_TextBuf, m_TextLen);
    font::TextWriterResource::DisableVertexAttribArrays();
}

// 0x0073E7A8 slot 0x68 | libgarden
u32* nw::lyt::TextBox::MakeUniformDataSelf(u32* pCommand, DrawInfo& drawInfo, Drawer& drawer)
{
    if (m_TextLen == 0 || m_pFont == nullptr || m_pMaterial == nullptr) {
        return pCommand;
    }

    if (drawer.m_VertexIndexCount != 0) {
        pCommand = drawer.UniformAndDraw(pCommand);
    }
    if (!(m_pDispStringBuf->m_Flags & 1)) {
        if (m_IsTextChanged || (!m_pDispStringBuf->m_IsCommandGenerated && &drawer != nullptr)) {
            SetupDrawCharData(&drawer);
        }
    }
    return drawer.SetUpTextBox(pCommand, this, m_pMaterial, drawInfo);
}

// 0x004BC430 slot 0x6C (name is ours)
void nw::lyt::TextBox::LoadMtx(DrawInfo& drawInfo)
{
    nn::math::MTX34 mtx;
    GetTextGlobalMtx(&mtx);
    drawInfo.m_pGraphicsResource->m_TextWriterResource.SetViewMtx(mtx);
}

// 0x0013B9BC slot 0x70 | libgarden
void nw::lyt::TextBox::AllocStringBuffer(u16 minLen, u32 flags)
{
    if (m_pDispStringBuf != nullptr && m_pDispStringBuf->m_Flags != flags) {
        FreeStringBuffer();
    }
    if (minLen == 0) {
        return;
    }

    const u32 allocBytes = (minLen + 1) * sizeof(wchar_t);
    if (allocBytes <= m_TextBufBytes) {
        return;
    }
    FreeStringBuffer();

    u32 dispBufSize = font::CharWriter::GetDispStringBufferSize(minLen);
    u32 cmdBufSize = 0;
    if (flags & ALLOC_FLAG_COMMAND_BUFFER) {
        dispBufSize = (dispBufSize + 15) & ~15;
        cmdBufSize = font::DispStringBuffer::CalcCommandBufferSize(minLen) * sizeof(u32);
    }

    wchar_t* textBuf = Layout::NewArray<wchar_t>(minLen + 1);
    void* pDispStringBuf;
    if (flags & ALLOC_FLAG_COMMAND_BUFFER) {
        pDispStringBuf = Layout::AllocDeviceMemory(dispBufSize + cmdBufSize, 16);
    } else {
        pDispStringBuf = Layout::AllocMemory(dispBufSize);
    }

    if (textBuf == nullptr) {
        if (pDispStringBuf != nullptr) {
            if (flags & ALLOC_FLAG_COMMAND_BUFFER) {
                Layout::FreeDeviceMemory(pDispStringBuf);
            } else {
                Layout::FreeMemory(pDispStringBuf);
            }
        }
        return;
    }
    if (pDispStringBuf == nullptr) {
        Layout::FreeMemory(textBuf);
        return;
    }

    m_TextBuf = textBuf;
    m_TextBufBytes = allocBytes;
    m_pDispStringBuf = font::CharWriter::InitDispStringBuffer(pDispStringBuf, minLen);
    if (flags & ALLOC_FLAG_COMMAND_BUFFER) {
        m_pDispStringBuf->m_pCommandBufferAlt = reinterpret_cast<u32*>(reinterpret_cast<uptr>(pDispStringBuf) + dispBufSize);
    }
}

// 0x004BC210 slot 0x74 (name is ours)
void nw::lyt::TextBox::FreeStringBuffer()
{
    if (m_TextBuf != nullptr) {
        Layout::FreeMemory(m_TextBuf);
        m_TextBuf = nullptr;
        m_TextBufBytes = 0;
        m_TextLen = 0;
    }
    if (m_pDispStringBuf != nullptr) {
        if (m_pDispStringBuf->m_Flags & ALLOC_FLAG_COMMAND_BUFFER) {
            Layout::FreeDeviceMemory(m_pDispStringBuf);
        } else {
            Layout::FreeMemory(m_pDispStringBuf);
        }
        m_pDispStringBuf = nullptr;
    }
}

// 0x004BBFAC slot 0x78 (name is ours)
u16 nw::lyt::TextBox::SetString(const wchar_t* str, u16 dstIdx)
{
    return SetStringImpl(str, dstIdx, std::wcslen(str));
}

// 0x004BC5C0 slot 0x7C | libgarden
u16 nw::lyt::TextBox::SetString(const wchar_t* str, u16 dstIdx, u16 strLen)
{
    return SetStringImpl(str, dstIdx, strLen);
}

// 0x004BBFDC (name is ours)
u16 nw::lyt::TextBox::SetStringImpl(const wchar_t* str, u16 dstIdx, u16 strLen)
{
    if (m_pFont == nullptr || m_TextBuf == nullptr) {
        return 0;
    }

    const u16 bufLen = (m_TextBufBytes != 0) ? m_TextBufBytes / sizeof(wchar_t) - 1 : 0;
    if (dstIdx >= bufLen || m_TextLen < dstIdx) {
        return 0;
    }
    if (strLen != 0 && (str == nullptr || str[0] == L'\0')) {
        return 0;
    }

    u16 cpLen = bufLen - dstIdx;
    if (strLen <= cpLen) {
        cpLen = strLen;
    }
    std::memcpy(m_TextBuf + dstIdx, str, cpLen * sizeof(wchar_t));
    m_TextLen = dstIdx + cpLen;
    m_TextBuf[m_TextLen] = L'\0';
    m_IsTextChanged = true;
    return cpLen;
}

// 0x004BC098 | nintendogs:callseq [tier A]
void nw::lyt::TextBox::SetupTextWriter(font::TextWriterBase<wchar_t>* pWriter)
{
    pWriter->m_pFont = m_pFont;
    if (m_pFont != nullptr) {
        pWriter->SetFontSize(m_FontSize.width, m_FontSize.height);
        pWriter->m_LineSpace = m_LineSpace;
        pWriter->m_CharSpace = m_CharSpace;
        pWriter->m_WidthLimit = m_Size.width;
    }
    if (m_pTagProcessor != nullptr) {
        pWriter->m_pTagProcessor = m_pTagProcessor;
    }

    u32 drawFlag = 0;
    switch (m_TextAlignment) {
    case 1:
        break;
    case 2:
        drawFlag = font::TextWriterBase<wchar_t>::HORIZONTAL_ALIGN_CENTER;
        break;
    case 3:
        drawFlag = font::TextWriterBase<wchar_t>::HORIZONTAL_ALIGN_RIGHT;
        break;
    default:
        switch (m_TextPosition % 3) {
        case 1:
            drawFlag = font::TextWriterBase<wchar_t>::HORIZONTAL_ALIGN_CENTER;
            break;
        case 2:
            drawFlag = font::TextWriterBase<wchar_t>::HORIZONTAL_ALIGN_RIGHT;
            break;
        }
        break;
    }
    switch (m_TextPosition % 3) {
    case 1:
        drawFlag |= font::TextWriterBase<wchar_t>::HORIZONTAL_ORIGIN_CENTER;
        break;
    case 2:
        drawFlag |= font::TextWriterBase<wchar_t>::HORIZONTAL_ORIGIN_RIGHT;
        break;
    }
    switch (m_TextPosition / 3) {
    case 1:
        drawFlag |= font::TextWriterBase<wchar_t>::VERTICAL_ORIGIN_MIDDLE;
        break;
    case 2:
        drawFlag |= font::TextWriterBase<wchar_t>::VERTICAL_ORIGIN_BOTTOM;
        break;
    }
    pWriter->m_DrawFlag = drawFlag;

    const ut::Color8 topColor = m_TextColors[0];
    const ut::Color8 bottomColor = m_TextColors[1];
    pWriter->m_GradationMode = (topColor.ToU32() != bottomColor.ToU32()) ? font::CharWriter::GRADATION_MODE_V : font::CharWriter::GRADATION_MODE_NONE;
    pWriter->UpdateVertexColors();
    pWriter->m_TextColors[0] = topColor;
    pWriter->m_TextColors[1] = bottomColor;
    pWriter->UpdateVertexColors();
}

// 0x004BC2D0 | nintendogs:callseq [tier A]
void nw::lyt::TextBox::SetupDrawCharData(Drawer* pDrawer)
{
    font::TextWriterBase<wchar_t> writer;
    writer.m_pDispStringBuffer = m_pDispStringBuf;
    SetupTextWriter(&writer);
    if (m_IsTextChanged) {
        writer.StartPrint();
        writer.Print(m_TextBuf, m_TextLen);
        m_IsTextChanged = false;
    }

    font::DispStringBuffer* pDispStringBuf = m_pDispStringBuf;
    if (!pDispStringBuf->m_IsCommandGenerated && pDrawer != nullptr) {
        if ((pDispStringBuf->m_Flags & ALLOC_FLAG_COMMAND_BUFFER) && pDispStringBuf->m_pCommandBufferAlt != nullptr) {
            // the two command buffers take turns
            if (pDispStringBuf->m_pCommandBuffer == pDispStringBuf->m_pCommandBufferBase) {
                pDispStringBuf->m_pCommandBuffer = pDispStringBuf->m_pCommandBufferAlt;
            } else {
                pDispStringBuf->m_pCommandBuffer = pDispStringBuf->m_pCommandBufferBase;
            }
        }
        pDrawer->BuildTextCommand(&writer);
    }
}

// 0x0073E5F4 | nintendogs:bytes [tier A]
nn::math::VEC2 nw::lyt::TextBox::AdjustTextPos(const Size& size, bool isCeil) const
{
    nn::math::VEC2 pos;

    switch (m_TextPosition % 3) {
    case 1:
        pos.x = size.width * 0.5f;
        if (isCeil) {
            pos.x = std::ceil(pos.x);
        }
        break;
    case 2:
        pos.x = size.width;
        break;
    default:
        pos.x = 0.0f;
        break;
    }

    switch (m_TextPosition / 3) {
    case 1:
        pos.y = size.height * 0.5f;
        if (isCeil) {
            pos.y = std::ceil(pos.y);
        }
        break;
    case 2:
        pos.y = size.height;
        break;
    default:
        pos.y = 0.0f;
        break;
    }
    return pos;
}

// 0x0073E6C4 | nintendogs:bytes [tier A]
void nw::lyt::TextBox::GetTextGlobalMtx(nn::math::MTX34* pMtx) const
{
    nn::math::ARMv6::MTX34CopyAsm(pMtx, &m_GlbMtx);

    const nn::math::VEC2 vtxPos = GetVtxPos();
    const nn::math::VEC2 txtPos = AdjustTextPos(m_Size, false);
    const f32 x = vtxPos.x + txtPos.x;
    const f32 y = vtxPos.y - txtPos.y;
    for (int i = 0; i < 3; ++i) {
        pMtx->m[i][3] = pMtx->m[i][0] * x + pMtx->m[i][1] * y + pMtx->m[i][3];
    }
    // the text goes downwards
    for (int i = 0; i < 3; ++i) {
        pMtx->m[i][1] = -pMtx->m[i][1];
    }
}

// 0x004BC270 (name is ours)
void nw::lyt::TextBox::SetTextPositionH(u8 pos)
{
    const bool isChanged = (m_TextPosition % 3) != pos;
    m_IsTextChanged |= isChanged;
    if (isChanged) {
        m_TextPosition = (m_TextPosition / 3) * 3 + pos;
    }
}

} // namespace lyt
} // namespace nw
