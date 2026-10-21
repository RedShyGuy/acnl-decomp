#pragma once

// The texture manager of the GL functions (__tx_initializeTexManager): texture objects in a hash table
// of 512 lists sorted by name. An object is a 2D texture, a cube map, a texture collection (the
// bindings of all units, glBindTexture 0x6600) or a lookup table (glBindTexture 0x6610-0x662F). All
// names are ours.

#include "decomp.h"
#include "nn/gx/gx_Types.h"
#include "nn/gx/detail/gx_Surface.h"

namespace nn {
namespace gx {
namespace detail {
const u32 TEXTURE_BUCKET_COUNT = 512;
const u32 TEXTURE_UNIT_COUNT = 3;
const u32 LUT_COUNT = 32;
const u32 CUBE_FACE_COUNT = 6;

// the parameters of a texture (glTexParameter*; 0x34 bytes)
struct TextureParameters
{
    GLenum m_MagFilter;      // 0x00 (0x2600 at the start)
    GLenum m_MinFilter;      // 0x04 (0x2600)
    GLenum m_WrapS;          // 0x08 (0x2901)
    GLenum m_WrapT;          // 0x0C (0x2901)
    u32 m_Unknown0x10;       // 0x10
    GLint m_MinLod;          // 0x14 (-1000)
    f32 m_BorderColor[4];    // 0x18
    f32 m_LodBias;           // 0x28 (parameter 0x8558; the bias of the LOD register of the unit)
    u32 m_Location;          // 0x2C (LOCATION_*: where the image is)
    bool m_IsGenerateMipmap; // 0x30
    u8 _unknown0x31[3];
};
ASSERT_SIZE(TextureParameters, 0x34);

struct Texture2D
{
    TextureParameters m_Parameters; // 0x00
    Surface m_Surface;              // 0x34
};
ASSERT_SIZE(Texture2D, 0x78);

struct TextureCube
{
    TextureParameters m_Parameters;            // 0x000
    Surface m_Faces[CUBE_FACE_COUNT];          // 0x034
};
ASSERT_SIZE(TextureCube, 0x1CC);

// a lookup table (0x820 bytes)
struct TextureLut
{
    GLuint m_Name;           // 0x000
    u8 m_Data[0x800];        // 0x004
    void* m_pBuffers[6];     // 0x804
    s32 m_Unknown0x81C;      // 0x81C (-1 at the start)
};
ASSERT_SIZE(TextureLut, 0x820);

// the bindings of a texture collection (0x9C bytes)
struct TextureCollection
{
    GLuint m_Name;                         // 0x00
    GLuint m_2dNames[TEXTURE_UNIT_COUNT];  // 0x04
    GLuint m_CubeNames[TEXTURE_UNIT_COUNT];// 0x10
    GLuint m_LutNames[LUT_COUNT];          // 0x1C
};
ASSERT_SIZE(TextureCollection, 0x9C);

// a texture object
struct TextureObject
{
    void* m_pData;           // 0x0 (Texture2D / TextureCube / TextureCollection / TextureLut)
    u32 m_Kind;              // 0x4 (0 2D, 1 cube map, 2 collection, 3 lookup table)
    GLuint m_Name;           // 0x8
    TextureObject* m_pNext;  // 0xC
};
ASSERT_SIZE(TextureObject, 0x10);

// the tables of the manager (in the memory of the GL state)
struct TextureTables
{
    Texture2D* m_pDefault2d;                              // 0x000
    TextureCube* m_pDefaultCube;                          // 0x004
    TextureCollection* m_pDefaultCollection;              // 0x008
    GLuint m_PendingDeleteName;                           // 0x00C (the bound collection deleted)
    TextureObject* m_pTextures[TEXTURE_BUCKET_COUNT];     // 0x010
    TextureObject* m_pBound2d[TEXTURE_UNIT_COUNT];        // 0x810
    TextureObject* m_pBoundCube[TEXTURE_UNIT_COUNT];      // 0x81C
    TextureObject* m_pBoundLuts[LUT_COUNT];               // 0x828
    TextureObject* m_pBoundCollection;                    // 0x8A8
};
ASSERT_SIZE(TextureTables, 0x8AC);

struct TextureManager
{
    TextureTables* m_pTables;  // 0x0
    GLuint m_NextName;         // 0x4
};

extern TextureManager s_TextureManager;

// the data of a new 2D texture / cube map with the defaults
Texture2D* CreateTexture2D(GLuint name); // 0x0012EB18 (name is ours)
TextureCube* CreateTextureCube(GLuint name); // 0x0012EA50 (name is ours)
// a new object in its list (sorted by name)
void InsertTexture(TextureObject* pObject, GLuint name); // 0x007C4194 (name is ours)
} // namespace detail
} // namespace gx
} // namespace nn

extern "C" {
nn::gx::detail::TextureLut* __tx_getBoundTextureLut(u32 index);
}
