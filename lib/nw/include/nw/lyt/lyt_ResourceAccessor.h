#pragma once

#include "decomp.h"
#include "nw/lyt/lyt_Types.h"

namespace nw {
namespace font {
class Font;
} // namespace font

namespace lyt {
// the resource types (the directories of an archive; names are ours)
const u32 RESOURCE_TYPE_TEXTURE = 0x74696D67; // 'timg'
const u32 RESOURCE_TYPE_FONT = 0x666F6E74;    // 'font'

// RTTI N2nw3lyt16ResourceAccessorE @ 0x008D0848
// vtable 0x00902B28 (vptr 0x00902B30), offset_to_top 0, 7 entries
//
// Finds the files a layout needs (textures, fonts) and loads them. The class name and
// GetTexture are from the reference symbols; the other names are ours.
class ResourceAccessor
{
public:
    ResourceAccessor(); // 0x001322B8
    virtual ~ResourceAccessor(); // 0x004B5E84 slot 0x00 (name is ours)
    // 0x004B5E80 (deleting dtor)
    // the data of a file of the type (0: any) and its size; NULL if there is none
    virtual void* GetResource(u32 resType, const char* name, u32* pSize) = 0; // slot 0x08
    // the font of the name (loaded and kept)
    virtual font::Font* GetFont(const char* name) = 0; // slot 0x0C
    // the texture of the name (loaded and kept)
    virtual TextureInfo GetTexture(const char* name) = 0; // slot 0x10
    // the texture / font is loaded from the file of the name
    virtual TextureInfo LoadTexture(const char* name); // 0x004B5D38 slot 0x14 (name is ours)
    virtual font::Font* LoadFont(const char* name); // 0x004B5DD4 slot 0x18 (name is ours)
};
} // namespace lyt
} // namespace nw
