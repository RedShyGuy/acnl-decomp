#pragma once

#include "decomp.h"
#include "Resource/dResourceGetSkeletal.h"
#include "g3d/dResourceLoader.h"

// ResourceGetSkeletal with a loader of its own: LoadFile loads the file into the heap of the resource at once (names
// are ours).
// RTTI 20ResourceLoadSkeletal @ 0x008CCC00
// vtable 0x00882684 (vptr 0x0088268C), offset_to_top 0, 11 entries
// vtable 0x008F5A8C (vptr 0x008F5A94), offset_to_top 0, 11 entries
// vtable 0x0088276C (vptr 0x00882774), offset_to_top -312, 11 entries
// vtable 0x008F5AE8 (vptr 0x008F5AF0), offset_to_top -312, 11 entries
class ResourceLoadSkeletal : public ::ResourceGetSkeletal
{
public:
    ResourceLoadSkeletal(); // 0x00321E5C
    virtual ~ResourceLoadSkeletal(); // 0x00321F20 slot 0x00
    // 0x00321F0C (deleting dtor)
    // (the file is unloaded as well)
    void Destroy() override; // 0x00321E2C slot 0x0C
    // (OnUpdate after the animations)
    void Update(const nn::math::MTX34& matrix) override; // 0x00321D8C slot 0x10
    bool LoadFile(const char* path) override; // 0x00321DC0 slot 0x24
    // empty
    virtual void OnUpdate(); // 0x00321D88 slot 0x28

    g3d::ResourceLoader m_Loader; // 0x30
};
