#pragma once

#include "decomp.h"

// The pixels of an image texture of a graphics file (no RTTI; the classes and all names are ours): the data of the first
// image with its width and height; the pixels lie in 8x8 tiles in Morton order (the 3DS texture layout).
struct TextureImage
{
    // the image of resTexture (an image texture, else no data)
    TextureImage(const void* resTexture); // 0x005BFFE8
    // (inline) the pixels data of width x height
    TextureImage(u16* data, u32 width, u32 height) : m_pData(data), m_Width(width), m_Height(height) {}

    // (copies for the game) the index of the pixel at x, y
    u32 GetIndex(u32 x, u32 y) const; // 0x00758ABC
    DECOMP_EMIT_COPY u32 GetWidth() const { return m_Width; } // 0x00758B40
    DECOMP_EMIT_COPY u32 GetHeight() const { return m_Height; } // 0x00758B48

    u16* m_pData; // 0x0
    u32 m_Width;  // 0x4
    u32 m_Height; // 0x8

    // the index of a pixel in its tile by y and x within the tile
    static const u32 s_TileIndices[8][8];
};
ASSERT_SIZE(TextureImage, 0xC);

// an image of 16 bits per pixel (RGBA4 for the designs)
struct TextureImage16 : public TextureImage
{
    TextureImage16(const void* resTexture); // 0x001C51D0
    // (inline)
    TextureImage16(u16* data, u32 width, u32 height) : TextureImage(data, width, height) {}

    // (copy for the game) the pixel at x, y
    DECOMP_EMIT_COPY u16 GetPixel(u32 x, u32 y) const // 0x0070E454
    {
        return m_pData[GetIndex(x, y)];
    }
    // the color of the pixel at x, y (the low four bits, the alpha, are kept)
    void SetColor(u32 x, u32 y, u16 color); // 0x001C5184
    void SetPixel(u32 x, u32 y, u16 value); // 0x001C51B0
};
ASSERT_SIZE(TextureImage16, 0xC);
