#pragma once

#include "decomp.h"
#include "nn/math/math_MTX34.h"
#include "nn/math/math_Vector3.h"

namespace math {
class Angle16;
} // namespace math

namespace ssys {
namespace ma {
class HeapAllocator;
} // namespace ma
} // namespace ssys

namespace g3d {
class ResourceLoader;

namespace modelEffect {
class ModelEffectContainer;

// A handle of a model effect of the BsModelEffectMgr of the game: the slot of the container and the serial it had
// when it was made (names are ours).
// RTTI N3g3d11modelEffect17ModelEffectHandleE @ 0x008D0C84
// vtable 0x00903954 (vptr 0x0090395C), offset_to_top 0, 2 entries
class ModelEffectHandle
{
public:
    ModelEffectHandle(); // 0x004EBB00
    // the effect fades out unless it loops
    virtual ~ModelEffectHandle(); // 0x004EBC3C slot 0x00
    // 0x004EBB1C (deleting dtor)

    // the matrix of the effect (see ModelEffectContainer::SetMatrix)
    void SetMatrix(const nn::math::MTX34& matrix); // 0x004EBA28
    // the effect fades out and the handle is released (false if it is not playing)
    bool Fade(); // 0x004EBA68
    // the local matrix of the model of the effect
    void SetModelMatrix(const nn::math::MTX34& matrix); // 0x004EBAC0

    // (inline; a copy for the game) the container (NULL: no effect or no longer playing)
    ModelEffectContainer* GetContainer() const;
    // (inline) the handle no longer refers to an effect
    void Release()
    {
        m_Index = -1;
        m_Serial = -1;
    }

    s32 m_Index;   // 0x4 the slot in the manager
    s32 m_Serial;  // 0x8
};
ASSERT_SIZE(ModelEffectHandle, 0xC);

// the model effect files
const s32 RESOURCE_COUNT = 13;
// the loader of a model effect file
ResourceLoader* GetResourceLoader(s32 resourceIdx); // 0x004EB8DC
// the heap of the files
ssys::ma::HeapAllocator* GetHeapAllocator(); // 0x004EBA1C
// all model effect files into a heap "BsModelEffectMgr" of the ModelEffectResHeap, set up
void LoadResources(); // 0x004EB8F0
// a new effect of file resourceIdx with the model and animation of those names (see ModelEffectContainer::Build) at
// the matrix; false if the BsModelEffectMgr could not make it
bool Create(ModelEffectHandle* handle, u8 resourceIdx, const nn::math::MTX34& matrix, const char* modelName, const char* animName); // 0x004EBCC8
// the same at a position with a rotation (YXZ) and a scale; on a stage with a curved world the position follows the
// curve (seen from the camera if fromCamera) and the model tilts with it
bool Create(ModelEffectHandle* handle, u8 resourceIdx, const nn::math::VEC3& position, const math::Angle16* rotation,
            const nn::math::VEC3& scale, bool fromCamera, const char* modelName, const char* animName); // 0x004EBD4C
} // namespace modelEffect
} // namespace g3d
