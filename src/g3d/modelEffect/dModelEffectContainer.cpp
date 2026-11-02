#include "g3d/modelEffect/dModelEffectContainer.h"
#include <cmath>
#include "g3d/dMaterialAnim.h"
#include "g3d/dResourceLoader.h"
#include "g3d/dSkeletalAnim.h"
#include "g3d/dSkeletalModel.h"
#include "g3d/dVisibilityAnim.h"
#include "g3d/g3d_System.h"
#include "g3d/modelEffect/dModelEffectHandle.h"
#include "nw/gfx/gfx_Material.h"
#include "nw/gfx/gfx_Model.h"
#include "nw/gfx/res/gfx_ResGraphicsFile.h"
#include "nw/gfx/res/gfx_ResMaterial.h"
#include "nw/gfx/res/gfx_ResModel.h"
#include "nw/ut/ut_FloatColor.h"
#include "sead/seadHeap.h"
#include "sead/seadNew.h"
#include "sead/seadSafeStringBase.h"

namespace g3d {
namespace modelEffect {
namespace {
// the buffer option of the models
const u32 MODEL_BUFFER_OPTION = 0x834;

// (inline) the resource of a material that its animations change: its buffer if it has one, else the original
// (name is ours)
inline nw::gfx::res::ResMaterialData* GetMaterialData(nw::gfx::Material* material)
{
    nw::gfx::res::ResMaterialData* data = (material->m_BufferCount > 0) ? material->m_Buffers[0] : NULL;
    if (data == NULL) {
        data = static_cast<nw::gfx::res::ResMaterialData*>(const_cast<void*>(static_cast<const void*>(material->GetResSceneObject().ptr())));
    }
    return data;
}
} // namespace

// 0x004EBEE0 (name is ours)
void g3d::modelEffect::ModelEffectContainer::SetMatrix(const nn::math::MTX34& matrix)
{
    nn::math::VEC3 scale;
    nn::math::MTX34 rotation(matrix);
    scale.x = std::sqrt(matrix.m[0][0] * matrix.m[0][0] + matrix.m[1][0] * matrix.m[1][0] + matrix.m[2][0] * matrix.m[2][0]);
    scale.y = std::sqrt(matrix.m[0][1] * matrix.m[0][1] + matrix.m[1][1] * matrix.m[1][1] + matrix.m[2][1] * matrix.m[2][1]);
    scale.z = std::sqrt(matrix.m[0][2] * matrix.m[0][2] + matrix.m[1][2] * matrix.m[1][2] + matrix.m[2][2] * matrix.m[2][2]);
    if (scale.x > 0.0f) {
        const f32 inverse = 1.0f / scale.x;
        rotation.m[0][0] = matrix.m[0][0] * inverse;
        rotation.m[1][0] = matrix.m[1][0] * inverse;
        rotation.m[2][0] = matrix.m[2][0] * inverse;
    } else {
        rotation.m[2][0] = 0.0f;
        rotation.m[1][0] = 0.0f;
        rotation.m[0][0] = 0.0f;
    }
    if (scale.y > 0.0f) {
        const f32 inverse = 1.0f / scale.y;
        rotation.m[0][1] = matrix.m[0][1] * inverse;
        rotation.m[1][1] = matrix.m[1][1] * inverse;
        rotation.m[2][1] = matrix.m[2][1] * inverse;
    } else {
        rotation.m[2][1] = 0.0f;
        rotation.m[1][1] = 0.0f;
        rotation.m[0][1] = 0.0f;
    }
    if (scale.z > 0.0f) {
        const f32 inverse = 1.0f / scale.z;
        rotation.m[0][2] = matrix.m[0][2] * inverse;
        rotation.m[1][2] = matrix.m[1][2] * inverse;
        rotation.m[2][2] = matrix.m[2][2] * inverse;
    } else {
        rotation.m[2][2] = 0.0f;
        rotation.m[1][2] = 0.0f;
        rotation.m[0][2] = 0.0f;
    }
    m_pModel->SetMatrix(rotation);
    m_pModel->SetScale(scale);
}

// 0x004EC05C (name is ours)
bool g3d::modelEffect::ModelEffectContainer::StartFade(s32 frames)
{
    if (m_State != STATE_PLAYING) {
        return false;
    }
    m_FadeCounter = frames;
    m_State = STATE_FADING;
    m_FadeFrames = frames;
    return true;
}

// 0x004EC084 (name is ours)
bool g3d::modelEffect::ModelEffectContainer::Build(u8 resourceIdx, const char* modelName, const char* animName)
{
    m_ResourceIdx = resourceIdx;
    return Build(GetResourceLoader(resourceIdx), 0, modelName, animName);
}

// 0x004EC0BC (name is ours)
void g3d::modelEffect::ModelEffectContainer::AddToScene(s32 sceneIdx)
{
    if ((m_State == STATE_PLAYING || m_State == STATE_FADING) && m_pModel != NULL) {
        m_pModel->AddToScene(sceneIdx);
    }
}

// 0x004EC0E0 (name is ours)
bool g3d::modelEffect::ModelEffectContainer::Restart()
{
    if (m_State != STATE_FINISHED) {
        if (m_pSkeletalAnim != NULL && !m_pSkeletalAnim->IsFinished()) {
            return false;
        }
        for (u32 i = 0; i < MATERIAL_ANIM_COUNT; i++) {
            if (m_pMaterialAnims[i] != NULL && !m_pMaterialAnims[i]->IsFinished()) {
                return false;
            }
        }
        if (m_pVisibilityAnim != NULL && !m_pVisibilityAnim->IsFinished()) {
            return false;
        }
    }
    if (m_pSkeletalAnim != NULL) {
        m_pSkeletalAnim->SetFrame(0.0f);
    }
    for (u32 i = 0; i < MATERIAL_ANIM_COUNT; i++) {
        if (m_pMaterialAnims[i] != NULL) {
            m_pMaterialAnims[i]->SetFrame(0.0f);
        }
    }
    if (m_pVisibilityAnim != NULL) {
        m_pVisibilityAnim->SetFrame(0.0f);
    }
    if (m_pModel != NULL && m_ResourceIdx != 0) {
        const s32 count = m_pModel->GetModel()->GetResModel().ptr()->materials.count;
        for (s32 i = 0; i < count; i++) {
            nw::gfx::res::ResMaterialData* data = GetMaterialData(m_pModel->GetModel()->m_Materials[i]);
            nw::ut::FloatColor color;
            color.r = 1.0f;
            color.g = 1.0f;
            color.b = 1.0f;
            color.a = 1.0f;
            SetConstantColor(data, color, 0);
        }
    }
    m_State = STATE_PLAYING;
    return true;
}

// 0x004EC274 (name is ours)
bool g3d::modelEffect::ModelEffectContainer::Build(ResourceLoader* loader, u32 /*unused*/, const char* modelName, const char* animName)
{
    if (!m_HeapAllocator.CreateExpHeap(m_pHeap->getMaxAllocatableSize(4), m_pHeap,
                                       sead::SafeStringBase<char>("ModelEffectContainer"), sead::Heap::HEAP_DIRECTION_FORWARD, false)) {
        return false;
    }
    m_pModel = new (m_HeapAllocator.m_pHeap, 4) SkeletalModel;
    if (m_pModel == NULL) {
        return false;
    }
    const nw::gfx::res::ResGraphicsFileData* file = loader->GetGraphicsFile().ptr();
    void* resModel = (modelName != NULL) ? file->models.GetData(modelName) : file->models.GetData(0);
    if (resModel == NULL) {
        return false;
    }
    const s32 animCount = file->skeletalAnims.count + file->textureAnims.count + file->visibilityAnims.count;
    if (!m_pModel->Create(nw::gfx::res::ResSceneObject(resModel), &m_HeapAllocator, &m_HeapAllocator, MODEL_BUFFER_OPTION,
                          animCount != 0, animCount)) {
        return false;
    }
    if (file->skeletalAnims.count != 0) {
        m_pSkeletalAnim = new (m_HeapAllocator.m_pHeap, 4) SkeletalAnim;
        if (m_pSkeletalAnim == NULL) {
            return false;
        }
        void* anim = (animName != NULL) ? file->skeletalAnims.GetData(animName) : file->skeletalAnims.GetData(0);
        m_pSkeletalAnim->Build(m_pModel, nw::anim::res::ResAnim(anim), &m_HeapAllocator, 0);
        m_pModel->SetSkeletalAnim(m_pSkeletalAnim);
    }
    if (animName != NULL) {
        if (file->textureAnims.count != 0) {
            void* anim = file->textureAnims.GetData(animName);
            if (anim != NULL) {
                m_pMaterialAnims[0] = new (m_HeapAllocator.m_pHeap, 4) MaterialAnim;
                if (m_pMaterialAnims[0] == NULL) {
                    return false;
                }
                m_pMaterialAnims[0]->Build(m_pModel, nw::anim::res::ResAnim(anim), &m_HeapAllocator, false);
                m_pModel->SetMaterialAnim(m_pMaterialAnims[0], 0);
            }
        }
    } else {
        for (s32 i = 0; static_cast<u32>(i) < static_cast<u32>(file->textureAnims.count) && i < MATERIAL_ANIM_COUNT; i++) {
            m_pMaterialAnims[i] = new (m_HeapAllocator.m_pHeap, 4) MaterialAnim;
            if (m_pMaterialAnims[i] == NULL) {
                return false;
            }
            m_pMaterialAnims[i]->Build(m_pModel, nw::anim::res::ResAnim(file->textureAnims.GetData(i)), &m_HeapAllocator, false);
            m_pModel->SetMaterialAnim(m_pMaterialAnims[i], i);
        }
    }
    if (file->visibilityAnims.count != 0) {
        void* anim = (animName != NULL) ? file->visibilityAnims.GetData(animName) : file->visibilityAnims.GetData(0);
        if (anim != NULL) {
            m_pVisibilityAnim = new (m_HeapAllocator.m_pHeap, 4) VisibilityAnim;
            if (m_pVisibilityAnim == NULL) {
                return false;
            }
            m_pVisibilityAnim->Build(m_pModel, nw::anim::res::ResAnim(anim), &m_HeapAllocator);
            m_pModel->SetVisibilityAnim(m_pVisibilityAnim);
        }
    }
    m_HeapAllocator.Adjust();
    return true;
}

// 0x004EC77C (name is ours)
void g3d::modelEffect::ModelEffectContainer::Update()
{
    if (m_State == STATE_FINISHED) {
        return;
    }
    if (m_State == STATE_FADED) {
        m_State = STATE_FINISHED;
        return;
    }
    if ((m_State == STATE_PLAYING || m_State == STATE_FADING) && m_pModel != NULL) {
        m_pModel->UpdateAnims();
        m_pModel->Update();
    }
    if (m_State == STATE_PLAYING) {
        if (IsLooping()) {
            return;
        }
        if (m_pSkeletalAnim != NULL && !m_pSkeletalAnim->IsFinished()) {
            goto fade;
        }
        for (u32 i = 0; i < MATERIAL_ANIM_COUNT; i++) {
            if (m_pMaterialAnims[i] != NULL && !m_pMaterialAnims[i]->IsFinished()) {
                goto fade;
            }
        }
        if (m_pVisibilityAnim != NULL && !m_pVisibilityAnim->IsFinished()) {
            goto fade;
        }
        StartFade(FADE_FRAMES);
    }
fade:
    if (m_State != STATE_FADING) {
        return;
    }
    if (--m_FadeCounter <= 0) {
        m_State = STATE_FADED;
    }
    if (m_ResourceIdx == 0 || m_pModel == NULL) {
        return;
    }
    const s32 count = m_pModel->GetModel()->GetResModel().ptr()->materials.count;
    for (s32 i = 0; i < count; i++) {
        nw::gfx::res::ResMaterialData* data = GetMaterialData(m_pModel->GetModel()->m_Materials[i]);
        nw::ut::FloatColor color;
        GetConstantColor(&color, data, 0);
        color.a -= color.a / m_FadeCounter;
        SetConstantColor(data, color, 0);
    }
}

// 0x004EC9FC (name is ours)
g3d::modelEffect::ModelEffectContainer::ModelEffectContainer(sead::Heap* heap)
    : m_pHeap(heap), m_pModel(NULL), m_pSkeletalAnim(NULL), m_pVisibilityAnim(NULL), m_State(STATE_PLAYING),
      m_ResourceIdx(RESOURCE_COUNT), m_FadeFrames(0), m_FadeCounter(0)
{
    for (s32 i = 0; i < MATERIAL_ANIM_COUNT; i++) {
        m_pMaterialAnims[i] = NULL;
    }
    s_Count++;
}

// 0x004ECB34 slot 0x00
// 0x004ECA68 (deleting dtor)
g3d::modelEffect::ModelEffectContainer::~ModelEffectContainer()
{
    if (m_pVisibilityAnim != NULL) {
        m_pVisibilityAnim->Finalize();
        m_pVisibilityAnim = NULL;
    }
    for (u32 i = 0; i < MATERIAL_ANIM_COUNT; i++) {
        if (m_pMaterialAnims[i] != NULL) {
            m_pMaterialAnims[i]->Finalize();
            m_pMaterialAnims[i] = NULL;
        }
    }
    if (m_pSkeletalAnim != NULL) {
        m_pSkeletalAnim->Finalize();
        m_pSkeletalAnim = NULL;
    }
    if (m_pModel != NULL) {
        m_pModel->Finalize();
        m_pModel = NULL;
    }
    m_HeapAllocator.DestroyHeap();
    s_Count--;
}

// 0x004EEA68 (name is ours)
void g3d::modelEffect::ModelEffectContainer::SetModelScale(const nn::math::VEC3& scale)
{
    m_pModel->SetScale(scale);
}

// 0x004EEB48 (name is ours)
void g3d::modelEffect::ModelEffectContainer::SetModelMatrix(const nn::math::MTX34& matrix)
{
    m_pModel->SetMatrix(matrix);
}

// 0x00746654 (name is ours; a copy for the game, inline in the original)
bool g3d::modelEffect::ModelEffectContainer::IsLooping() const
{
    if (m_pSkeletalAnim != NULL && m_pSkeletalAnim->m_PlayMode == FrameController::PLAY_MODE_LOOP) {
        return true;
    }
    for (s32 i = 0; i < MATERIAL_ANIM_COUNT; i++) {
        if (m_pMaterialAnims[i] != NULL && m_pMaterialAnims[i]->m_PlayMode == FrameController::PLAY_MODE_LOOP) {
            return true;
        }
    }
    if (m_pVisibilityAnim != NULL && m_pVisibilityAnim->m_PlayMode == FrameController::PLAY_MODE_LOOP) {
        return true;
    }
    return false;
}

} // namespace modelEffect
} // namespace g3d
