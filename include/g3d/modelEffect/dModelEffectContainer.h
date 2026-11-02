#pragma once

#include "decomp.h"
#include "nn/math/math_MTX34.h"
#include "nn/math/math_Vector3.h"
#include "ssys/ma/dHeapAllocator.h"

namespace sead {
class Heap;
} // namespace sead

namespace g3d {
class MaterialAnim;
class ResourceLoader;
class SkeletalAnim;
class SkeletalModel;
class VisibilityAnim;

namespace modelEffect {
// A model effect: a skeletal model of one of the model effect files with its animations, in a heap of its own; it
// plays once (or loops) and then fades out (names are ours).
// RTTI N3g3d11modelEffect20ModelEffectContainerE @ 0x008D0C8C
// vtable 0x00903964 (vptr 0x0090396C), offset_to_top 0, 2 entries
class ModelEffectContainer
{
public:
    // m_State
    enum State : u8
    {
        STATE_PLAYING = 0,
        STATE_FADING = 1,
        STATE_FADED = 2,
        STATE_FINISHED = 3,
    };
    // the frames of the fade at the end
    static const s32 FADE_FRAMES = 10;
    // the material animations
    static const s32 MATERIAL_ANIM_COUNT = 4;

    explicit ModelEffectContainer(sead::Heap* heap); // 0x004EC9FC
    // finalizes the animations and the model and destroys the heap
    virtual ~ModelEffectContainer(); // 0x004ECB34 slot 0x00
    // 0x004ECA68 (deleting dtor)

    // the effect of model effect file resourceIdx (see Build)
    bool Build(u8 resourceIdx, const char* modelName, const char* animName); // 0x004EC084
    // the model of that name (NULL: the first) of the file with its skeletal, material and visibility animations of
    // that name (NULL: the first ones, up to four material animations) in a heap of all that m_pHeap can give
    bool Build(ResourceLoader* loader, u32 unused, const char* modelName, const char* animName); // 0x004EC274
    // the animations and the model of the frame; a finished effect that does not loop starts to fade, a fading one
    // loses the alpha of constant color 0 of its materials (not for file 0)
    void Update(); // 0x004EC77C
    // the effect fades out over the frames (false if it is not playing)
    bool StartFade(s32 frames); // 0x004EC05C
    // a finished effect (all animations at their end) plays again from frame 0 (false if not finished)
    bool Restart(); // 0x004EC0E0
    // the model is put into a scene (only while it plays or fades)
    void AddToScene(s32 sceneIdx); // 0x004EC0BC
    // the matrix of the model split into rotation and scale
    void SetMatrix(const nn::math::MTX34& matrix); // 0x004EBEE0
    // the local matrix / scale of the model
    void SetModelMatrix(const nn::math::MTX34& matrix); // 0x004EEB48
    void SetModelScale(const nn::math::VEC3& scale); // 0x004EEA68

    // (inline; a copy for the game) whether an animation loops
    bool IsLooping() const;

    // the containers that exist
    static s32 s_Count;

    sead::Heap* m_pHeap;                                      // 0x04 the parent of the heap
    SkeletalModel* m_pModel;                                  // 0x08
    SkeletalAnim* m_pSkeletalAnim;                            // 0x0C
    MaterialAnim* m_pMaterialAnims[MATERIAL_ANIM_COUNT];      // 0x10
    VisibilityAnim* m_pVisibilityAnim;                        // 0x20
    ssys::ma::HeapAllocator m_HeapAllocator;                  // 0x24
    u8 m_State;                                               // 0x2C State
    u8 m_ResourceIdx;                                         // 0x2D the model effect file (RESOURCE_COUNT: none)
    s32 m_FadeFrames;                                         // 0x30
    s32 m_FadeCounter;                                        // 0x34 the frames of the fade that are left
};
ASSERT_SIZE(ModelEffectContainer, 0x38);
} // namespace modelEffect
} // namespace g3d
