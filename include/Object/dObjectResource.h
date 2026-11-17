#pragma once

#include "decomp.h"
#include "ssys/ma/dHeapAllocator.h"

namespace g3d {
class Model;
class ResourceLoader;
} // namespace g3d
namespace nn {
namespace math {
struct MTX34;
} // namespace math
} // namespace nn

// The model resource of a game object: a virtual base of the actors (AcObjectBase) whose model comes from a graphics
// file (class name after the RTTI, the rest is ours). The Resource* classes implement it: they build a model (and
// its animations) from the first entries of the file; the file is either set by its owner (m_pResourceLoader) or
// loaded by LoadFile.
// RTTI 14ObjectResource @ 0x008CBD38
// vtable 0x008F05A0 (vptr 0x008F05A8), offset_to_top 0, 11 entries
class ObjectResource
{
public:
    ObjectResource(); // 0x00270C48
    virtual ~ObjectResource(); // 0x00270CA8 slot 0x00
    // 0x00270C70 (deleting dtor)

    // the model (and its animations) from the file of path (false: not yet; called again in the next frame)
    virtual bool Create(const char* path) = 0; // slot 0x08
    virtual void Destroy() = 0; // slot 0x0C
    // the local matrix of the model, then its animations and its world matrix
    virtual void Update(const nn::math::MTX34& matrix) = 0; // slot 0x10
    // the next frames of the animations
    virtual void UpdateFrames() = 0; // slot 0x14
    // the model into the main scene of g3d
    virtual void AddToScene() = 0; // slot 0x18
    // the file of path into the loader (false: not yet)
    virtual bool LoadFile(const char* path) = 0; // slot 0x1C
    virtual g3d::Model* GetModel(); // 0x00270C40 slot 0x20
    // the heap of the resource: all of the current heap of sead (nothing if there is one)
    virtual void CreateHeap(); // 0x00270BB8 slot 0x24
    virtual void DestroyHeap(); // 0x00270C28 slot 0x28

    ssys::ma::HeapAllocator m_Allocator;       // 0x04
    g3d::ResourceLoader* m_pResourceLoader;    // 0x0C the graphics file
    u32 m_BufferOption;                        // 0x10 of the model (g3d::Model::Create)
};
ASSERT_SIZE(ObjectResource, 0x14);
