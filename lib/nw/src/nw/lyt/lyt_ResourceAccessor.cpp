#include "nw/lyt/lyt_ResourceAccessor.h"

#include "nw/font/font_ResFont.h"
#include "nw/lyt/lyt_Api.h"
#include "nw/lyt/lyt_Layout.h"

namespace nw {
namespace lyt {
// 0x001322B8
nw::lyt::ResourceAccessor::ResourceAccessor()
{
    // empty (in the original too: only the vtable)
}

// 0x004B5E84 slot 0x00 (name is ours)
// 0x004B5E80 (deleting dtor)
nw::lyt::ResourceAccessor::~ResourceAccessor()
{
    // empty (in the original too)
}

// 0x004B5D38 slot 0x14 (name is ours)
TextureInfo nw::lyt::ResourceAccessor::LoadTexture(const char* name)
{
    u32 texSize = 0;
    void* pTexRes = GetResource(RESOURCE_TYPE_TEXTURE, name, &texSize);
    if (pTexRes == nullptr || texSize == 0) {
        return TextureInfo();
    }
    const TextureInfo texInfo = lyt::LoadTexture(pTexRes, texSize, 0);
    if (texInfo.physicalAddress == 0) {
        return TextureInfo();
    }
    return texInfo;
}

// 0x004B5DD4 slot 0x18 (name is ours)
font::Font* nw::lyt::ResourceAccessor::LoadFont(const char* name)
{
    u32 fontSize = 0;
    void* pFontRes = GetResource(RESOURCE_TYPE_FONT, name, &fontSize);
    if (pFontRes == nullptr || fontSize == 0) {
        return nullptr;
    }

    font::ResFont* pResFont = Layout::NewObj<font::ResFont>();
    if (pResFont == nullptr) {
        return nullptr;
    }
    if (!pResFont->SetResource(pFontRes)) {
        Layout::DeleteObj(pResFont);
        return nullptr;
    }
    pResFont->SetDrawBuffer(Layout::AllocMemory(font::ResFont::GetDrawBufferSize(pFontRes)));
    return pResFont;
}

} // namespace lyt
} // namespace nw
