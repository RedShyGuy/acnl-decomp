#include "nn/gxlow/CTR/CTR_Api.h"
#include "nw/font/internal/font_TextureObject.h"

namespace nw {
namespace font {
namespace internal {
// 0x00141B5C | nintendogs:bytes [tier A]
void nw::font::internal::TextureObject::Set(u32 name, const nw::font::Font* pFont, const void* pImage, u16 format, u16 width, u16 height)
{
    m_Name = name;
    m_pFont = pFont;
    m_ImageAddress = pImage != nullptr ? nngxGetPhysicalAddr(reinterpret_cast<uptr>(pImage)) >> 3 : 0;
    m_Format = static_cast<u8>(format);
    m_Width = width;
    m_Height = height;
}

} // namespace internal
} // namespace font
} // namespace nw
