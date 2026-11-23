#include "Other/dTextureImage.h"

#include "nw/gfx/res/gfx_ResTexture.h"

// 0x008584D0
const u32 TextureImage::s_TileIndices[8][8] = {
    {0, 1, 4, 5, 16, 17, 20, 21},     {2, 3, 6, 7, 18, 19, 22, 23},     {8, 9, 12, 13, 24, 25, 28, 29},
    {10, 11, 14, 15, 26, 27, 30, 31}, {32, 33, 36, 37, 48, 49, 52, 53}, {34, 35, 38, 39, 50, 51, 54, 55},
    {40, 41, 44, 45, 56, 57, 60, 61}, {42, 43, 46, 47, 58, 59, 62, 63},
};

// 0x005BFFE8
TextureImage::TextureImage(const void* resTexture) : m_pData(NULL), m_Width(0), m_Height(0)
{
    const nw::gfx::res::ResImageTextureData* texture = nw::gfx::res::DynamicCastData<nw::gfx::res::ResImageTextureData>(
        resTexture, nw::gfx::res::ResTexture::TYPE_IMAGE);
    const nw::gfx::res::ResImageData* image = texture->image.To<nw::gfx::res::ResImageData>();
    m_pData = const_cast<u16*>(static_cast<const u16*>(image->data.ToPtr()));
    m_Width = texture->width;
    m_Height = texture->height;
}

// 0x00758ABC
u32 TextureImage::GetIndex(u32 x, u32 y) const
{
    const u32 tilesX = m_Width >> 3;
    const u32 tileX = m_Width >= 8 ? (x >> 3) % tilesX : 0;
    const u32 tileY = m_Height >= 8 ? (y >> 3) % (m_Height >> 3) : 0;
    return s_TileIndices[(y - tileY * 8) & 7][(x - tileX * 8) & 7] + (tileY * tilesX + tileX) * 64;
}

// 0x001C51D0
TextureImage16::TextureImage16(const void* resTexture) : TextureImage(resTexture)
{
}

// 0x001C5184
void TextureImage16::SetColor(u32 x, u32 y, u16 color)
{
    u16* pixel = &m_pData[GetIndex(x, y)];
    *pixel = (*pixel & 15) | color;
}

// 0x001C51B0
void TextureImage16::SetPixel(u32 x, u32 y, u16 value)
{
    m_pData[GetIndex(x, y)] = value;
}
