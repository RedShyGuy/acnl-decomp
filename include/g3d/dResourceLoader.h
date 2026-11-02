#pragma once

#include "decomp.h"
#include "g3d/dResourceSet.h"
#include "ssys/ma/dLoadSplit.h"

namespace nw {
namespace os {
class IAllocator;
} // namespace os
} // namespace nw

namespace sead {
class Heap;
template <typename T>
class SafeStringBase;
} // namespace sead

namespace g3d {
// a graphics file loaded from the file system (names after libgarden except the functions of our own)
// RTTI N3g3d14ResourceLoaderE @ 0x008D0CD8
// vtable 0x00903A34 (vptr 0x00903A3C), offset_to_top 0, 2 entries
class ResourceLoader : public ::g3d::ResourceSet
{
public:
    // the alignment of a graphics file (libgarden: CGFX_ALIGNMENT)
    static const u32 CGFX_ALIGNMENT = 0x80;

    ResourceLoader(); // 0x004EEC1C | libgarden
    virtual ~ResourceLoader(); // 0x00318014 slot 0x00
    // 0x004EEC5C (deleting dtor)

    // the file into a buffer of heap at once / in pieces by the thread of the LoadSplitMgr; the buffer becomes the
    // graphics file once it is there
    bool Load(const sead::SafeStringBase<char>& path, sead::Heap* heap, u32 alignment); // 0x004EEBE4 | libgarden
    bool LoadAsync(const sead::SafeStringBase<char>& path, sead::Heap* heap, u32 alignment); // 0x00317C38 | libgarden
    // the buffers of the file in memory areas of nngx: the textures in textureArea, the vertex and index buffers of
    // the shapes in the other two (libgarden: Read)
    void Read(u32 textureArea, u32 vertexArea, u32 indexArea); // 0x004EB42C | libgarden
    // the setup of the file (against the file of additional, else itself): materials of "DefaultShader" get the
    // shader program of their texture coordinators; with setupSystemShaders the missing shaders come from the shader
    // file of the engine; then the lookup tables are resolved
    void InitializeCgfx(nw::os::IAllocator* allocator, ResourceLoader* additional, bool setupSystemShaders); // 0x00317C70 | libgarden
    // the file cleaned up and the loader reset (names are ours)
    void Unload(); // 0x002F7210
    // whether loading has finished without success
    bool IsLoadFailed(); // 0x004EEBBC
    // the lookup tables of the samplers of the materials resolved by name (name is ours)
    void ResolveLookupTables(bool resolveAll); // 0x004EB570

    ssys::ma::LoadSplit m_LoadSplit; // 0x1C
};
ASSERT_SIZE(ResourceLoader, 0x108);
} // namespace g3d
