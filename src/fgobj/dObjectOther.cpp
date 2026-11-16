#include "fgobj/dObjectOther.h"
#include "Sv/dSvFgName.h"
#include "fgobj/fgobj_Globals.h"

namespace fgobj {
// 0x00592A70 slot 0x24 (name is ours)
bool fgobj::ObjectOther::Refresh(const SvFgName* item, const nn::math::VEC3* position, const nn::math::VEC3* scale, s32 id)
{
    if (!Func2FE704(&m_Item, item)) {
        return false;
    }
    if (m_Id != id) {
        return false;
    }
    m_Refresh = 1;
    m_Position = *position;
    m_Model.SetScale(*scale);
    return true;
}

// 0x00592AD8 (name is ours)
fgobj::ObjectOther* fgobj::ObjectOther::Create(const SvFgName& item, const nn::math::VEC3* position, const nn::math::VEC3* scale, s32 id,
                                               f32 shadowSize, bool faceCameraIndoors, bool hasShadow, s16 rotateY, bool withFlag40000)
{
    ObjectOther* other = NewObject<ObjectOther>(s_SmallHeaps, SMALL_HEAP_NUM);
    if (other == NULL) {
        return NULL;
    }
    if (withFlag40000) {
        other->m_Flags |= FLAG_UNK_40000;
    }
    other->Setup(item, 0, 0, false);
    other->m_Position = *position;
    other->m_Model.SetScale(*scale);
    other->m_Z = -1;
    other->m_X = -1;
    other->m_Refresh = 1;
    other->m_Id = id;
    if (s_IsIndoor & faceCameraIndoors) {
        other->m_Flags |= FLAG_FACE_CAMERA;
    } else {
        other->m_RotateY = rotateY;
    }
    if (hasShadow) {
        other->InitializeShadow();
        other->m_ShadowSize = shadowSize;
    }
    return other;
}

// 0x00592C38 slot 0x20 (name is ours)
s32 fgobj::ObjectOther::Execute()
{
    if (m_Refresh <= 0) {
        return EXECUTE_FINISHED_KEEP_REQUEST;
    }
    m_Refresh = 0;
    UpdateMatrix(true, 0.0f);
    return EXECUTE_CONTINUE;
}

// 0x00592C9C slot 0x00
// 0x00592C6C (deleting dtor)
fgobj::ObjectOther::~ObjectOther()
{
    // (only the base is destroyed)
}

} // namespace fgobj
