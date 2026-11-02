#include "g3d/dAnimPack.h"
#include <new>
#include "g3d/dModel.h"
#include "nw/gfx/gfx_AnimOverrider.h"

namespace g3d {
// 0x004EF2DC (name is ours)
bool g3d::AnimPack::Build(Model* model, s32 maxAnimObjects, nw::os::IAllocator* allocator)
{
    Create(maxAnimObjects, allocator);
    m_pAnimObject->TryBind(model->GetMaterialAnimGroup());
    return true;
}

// 0x004F2AD8 (name is ours)
bool g3d::AnimPack::Create(s32 maxAnimObjects, nw::os::IAllocator* allocator)
{
    void* memory = allocator->Alloc(sizeof(nw::gfx::AnimOverrider), 4);
    nw::gfx::AnimOverrider* overrider = NULL;
    if (memory != NULL) {
        overrider = new (memory) nw::gfx::AnimOverrider(allocator);
        u32 result = nw::gfx::RESULT_SUCCESS;
        void* buffer = overrider->GetAllocator()->Alloc(maxAnimObjects * sizeof(nw::gfx::AnimObject*), 4);
        if (buffer == NULL) {
            result |= nw::gfx::RESULT_FAILURE_MASK;
        }
        if (!(result & nw::gfx::RESULT_FAILURE_MASK)) {
            nw::ut::MoveArray<nw::gfx::AnimObject*> animObjects(buffer, maxAnimObjects, overrider->GetAllocator());
            overrider->m_AnimObjects = animObjects;
        }
    }
    m_pAnimObject = overrider;
    return m_pAnimObject != NULL;
}

// 0x004F2C30 (name is ours)
void g3d::AnimPack::Finalize()
{
    nw::gfx::SafeDestroy(m_pAnimObject);
}

// 0x004F2C74 (name is ours)
g3d::AnimPack::AnimPack() : m_pAnimObject(NULL)
{
}

// 0x004F2C98 slot 0x00
// 0x004F2C88 (deleting dtor)
g3d::AnimPack::~AnimPack()
{
    // empty (in the original too)
}

} // namespace g3d
