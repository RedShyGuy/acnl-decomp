#include "g3d/modelEffect/dModelEffectHandle.h"
#include "Bs/dBsModelEffectMgr.h"
#include "Other/dHeapInit.h"
#include "Other/dWorldCurve.h"
#include "g3d/dResourceLoader.h"
#include "g3d/modelEffect/dModelEffectContainer.h"
#include "math/dAngle16.h"
#include "sead/seadExpHeap.h"
#include "sead/seadMathCalcCommon.h"
#include "sead/seadSafeStringBase.h"
#include "ssys/ma/dHeapAllocator.h"
#include "ssys/ma/ma_Math.h"
#include "stage/stage_Api.h"

namespace g3d {
namespace modelEffect {
// the heap of the model effect files
// 0x00953AA8
ssys::ma::HeapAllocator s_HeapAllocator;
// the model effect files
// 0x00953AB0
const char* s_ResourcePaths[RESOURCE_COUNT] = {
    "ModelEffect/cg_grd_soil.bcres",       "ModelEffect/cg_mp_a.bcres",            "ModelEffect/cg_mp_ha_l.bcres",
    "ModelEffect/cg_mp_ha_r.bcres",        "ModelEffect/cg_mp_hate.bcres",         "ModelEffect/cg_mp_love.bcres",
    "ModelEffect/cg_mp_nolove.bcres",      "ModelEffect/cg_mp_pikon.bcres",        "ModelEffect/cg_ply_diveshadow.bcres",
    "ModelEffect/cg_ply_divewater.bcres",  "ModelEffect/cg_ply_divewater_s.bcres", "ModelEffect/cg_sdw_sea.bcres",
    "ModelEffect/mydesign_fireworks.bcres",
};
// 0x00953AE4
s32 ModelEffectContainer::s_Count = 0;
// 0x009CA198
ResourceLoader s_ResourceLoaders[RESOURCE_COUNT];

// constructs s_HeapAllocator and s_ResourceLoaders
// 0x00799E08 (static initializer)

// 0x004EB8DC (name is ours)
ResourceLoader* GetResourceLoader(s32 resourceIdx)
{
    return &s_ResourceLoaders[resourceIdx];
}

// 0x004EB8F0 (name is ours)
void LoadResources()
{
    s_HeapAllocator.CreateExpHeap(HeapInit::s_pModelEffectResHeap->getMaxAllocatableSize(4), HeapInit::s_pModelEffectResHeap,
                                  sead::SafeStringBase<char>("BsModelEffectMgr"), sead::Heap::HEAP_DIRECTION_FORWARD, false);
    u32 maxSize = 0;
    sead::Heap* heap = s_HeapAllocator.m_pHeap;
    for (s32 i = 0; i < RESOURCE_COUNT; i++) {
        const u32 freeSize = s_HeapAllocator.m_pHeap->getMaxAllocatableSize(4);
        ResourceLoader* loader = &s_ResourceLoaders[i];
        loader->Load(sead::SafeStringBase<char>(s_ResourcePaths[i]), heap, ResourceLoader::CGFX_ALIGNMENT);
        loader->InitializeCgfx(&s_HeapAllocator, NULL, true);
        // (the largest file; not used)
        const u32 size = freeSize - s_HeapAllocator.m_pHeap->getMaxAllocatableSize(4);
        if (size > maxSize) {
            maxSize = size;
        }
    }
    s_HeapAllocator.Adjust();
}

// 0x004EBA1C (name is ours)
ssys::ma::HeapAllocator* GetHeapAllocator()
{
    return &s_HeapAllocator;
}

// 0x004EBCC8 (name is ours)
bool Create(ModelEffectHandle* handle, u8 resourceIdx, const nn::math::MTX34& matrix, const char* modelName, const char* animName)
{
    BsModelEffectMgr* mgr = BsModelEffectMgr::GetInstance();
    if (mgr == NULL) {
        return false;
    }
    if (!mgr->CreateEffect(handle, resourceIdx, modelName, animName)) {
        return false;
    }
    if (handle != NULL) {
        ModelEffectContainer* container = handle->GetContainer();
        // (a copy that is not used: so in the original)
        nn::math::MTX34 copy(matrix);
        container->SetModelMatrix(matrix);
    }
    return true;
}

// 0x004EBD4C (name is ours)
bool Create(ModelEffectHandle* handle, u8 resourceIdx, const nn::math::VEC3& position, const math::Angle16* rotation,
            const nn::math::VEC3& scale, bool fromCamera, const char* modelName, const char* animName)
{
    nn::math::VEC3 curvedPosition = position;
    math::Angle16 angles[3];
    angles[0] = rotation[0];
    angles[1] = rotation[1];
    angles[2] = rotation[2];
    math::Angle16 curveAngle;
    curveAngle.value = 0;
    if (stage::HasWorldCurve(stage::GetCurrent())) {
        curveAngle = fromCamera ? GetWorldCurvePositionFromCamera(&curvedPosition, position) : GetWorldCurvePosition(&curvedPosition, position);
    }
    nn::math::MTX34 matrix;
    matrix.m[0][0] = 1.0f;
    matrix.m[0][1] = 0.0f;
    matrix.m[0][2] = 0.0f;
    matrix.m[1][0] = 0.0f;
    matrix.m[1][1] = 1.0f;
    matrix.m[1][2] = 0.0f;
    matrix.m[2][0] = 0.0f;
    matrix.m[2][1] = 0.0f;
    matrix.m[2][2] = 1.0f;
    matrix.m[0][3] = curvedPosition.x;
    matrix.m[1][3] = curvedPosition.y;
    matrix.m[2][3] = curvedPosition.z;
    if (static_cast<s16>(curveAngle.value) != 0) {
        ssys::ma::RotateX(&matrix, curveAngle.value);
    }
    ssys::ma::RotateYXZ(&matrix, angles[0].value, angles[1].value, angles[2].value);
    sead::Matrix34CalcCtr<f32>::scaleBases(matrix, scale.x, scale.y, scale.z);
    return Create(handle, resourceIdx, matrix, modelName, animName);
}

} // namespace modelEffect
} // namespace g3d
