#pragma once

#include "decomp.h"

namespace g3d {
class Model;
class ResourceLoader;
} // namespace g3d
namespace nw {
namespace os {
class IAllocator;
} // namespace os
} // namespace nw

// Game code that the house objects (hobj) call and that is not decompiled yet (names after the addresses).

// (the object that the season test is called on and the files of the door animations come from: the pointer of
// g_Unknown9516C0 in Sound/sound_Extern.h; its byte +0x6ACE keeps a closing door of the long animation open)
// the texture folders of the seasons: "season00" and the snow season "season09" (index: Func630618)
extern const char* g_SeasonTextureFolders[2];

// whether the stage shows the snow textures (a nop entry into Func63061C; the object is not used)
bool Func630618(void* object); // 0x00630618
// the graphics file of a global resource manager (0x009990CC) under the key 0x00951824 (NULL: none)
g3d::ResourceLoader* Func2B6AC4(); // 0x002B6AC4
// the shared graphics file of the object (set up before the door model)
g3d::ResourceLoader* Func6DB618(void* object); // 0x006DB618
// the files of the two door animations of the object (the first animation is also the one of the closed door)
g3d::ResourceLoader* Func6DF40C(void* object); // 0x006DF40C
g3d::ResourceLoader* Func6DF244(void* object); // 0x006DF244
// the model of a file: the file is read into VRAM, set up against the shared file, its textures (optional) and the
// file of shared, then the first model of the file is made
void Func4DBE3C(g3d::Model* model, g3d::ResourceLoader* modelFile, g3d::ResourceLoader* textureFile,
                g3d::ResourceLoader* shared, nw::os::IAllocator* allocator); // 0x004DBE3C

// the color of the house in the house data (its bytes +26..+29 in reverse order: out[1..3] = blue, green, red)
void Func720F9C(u8* out, const void* houseData); // 0x00720F9C
// the brightness of the light for the house colors
f32 Func1E7A5C(); // 0x001E7A5C
// whether the light has changed (the house colors are set again)
bool Func1E8844(); // 0x001E8844
// a color of the environment (index 20: the color of the walls of the villager houses; same byte order as Func720F9C)
void Func1E63B0(u8* out, s32 index); // 0x001E63B0
