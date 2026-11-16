#include "fgobj/dObjectPlant.h"
#include "Sound/dSoundLabel.h"
#include "Sound/dSoundMgr.h"
#include "fgobj/fgobj_Globals.h"
#include "nw/gfx/gfx_CalculatedTransform.h"
#include "nw/gfx/gfx_Model.h"

namespace fgobj {
namespace {
const u8 PLANT_FRAME_NUM = 15;
// the scales of the frames in x and z / in y
// 0x008476F4
const f32 s_PlantScalesXZ[PLANT_FRAME_NUM] = {0.69f, 0.77f, 0.96f, 1.16f, 1.27f, 1.29f, 1.28f, 1.2f,
                                              1.1f,  1.0f,  0.96f, 0.97f, 0.98f, 0.99f, 1.0f};
// 0x00847730
const f32 s_PlantScalesY[PLANT_FRAME_NUM] = {1.2f, 0.95f, 0.69f, 0.67f, 0.67f, 0.69f, 0.75f, 0.86f,
                                             0.99f, 1.1f, 1.14f, 1.12f, 1.07f, 1.02f, 1.0f};
} // namespace

// 0x00592CC8 (name is ours)
fgobj::ObjectPlant* fgobj::ObjectPlant::Create(const SvFgName& item, s32 x, s32 z, bool playSound)
{
    ObjectPlant* plant = NewObject<ObjectPlant>(s_SmallHeaps, SMALL_HEAP_NUM);
    if (plant == NULL) {
        return NULL;
    }
    plant->Setup(item, x, z, false);
    plant->m_Frame = 0;
    if (playSound) {
        SoundMgr::SPlaySharedSound(SE_OBJ_PLANT_CMN_GROW, &plant->m_Position, plant, true);
    }
    plant->InitializeShadow();
    plant->m_Shadow.m_Opacity = 0.0f;
    return plant;
}

// 0x00592DB8 slot 0x20 (name is ours)
s32 fgobj::ObjectPlant::Execute()
{
    if (m_Frame >= PLANT_FRAME_NUM) {
        return EXECUTE_FINISHED;
    }
    nn::math::VEC3 scale(s_PlantScalesXZ[m_Frame], s_PlantScalesY[m_Frame], s_PlantScalesXZ[m_Frame]);
    m_Model.SetScale(scale);
    m_Model.Update();
    m_Model.GetModel()->m_WorldTransform.m_Flags |= nw::gfx::CalculatedTransform::FLAG_IS_SCALE_ONE;
    m_Frame++;
    f32 opacity = m_Frame * (1.0f / PLANT_FRAME_NUM);
    if (opacity < 0.0f) {
        opacity = 0.0f;
    } else if (opacity > 1.0f) {
        opacity = 1.0f;
    }
    m_Shadow.m_Opacity = opacity;
    return EXECUTE_CONTINUE;
}

// 0x00592EB8 slot 0x00
// 0x00592E88 (deleting dtor)
fgobj::ObjectPlant::~ObjectPlant()
{
    // (only the base is destroyed)
}

} // namespace fgobj
