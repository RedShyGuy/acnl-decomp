#include "fgobj/dObjectDeco.h"
#include "Sc/dScStage.h"
#include "fgobj/dProc.h"
#include "fgobj/fgobj_Globals.h"
#include "nw/gfx/gfx_Api.h"

namespace fgobj {
namespace {
// the model and the texture animation in FgObjAnim.bcres
const char* const DECO_MODEL_NAME = "fg_treeB_d";
const char* const DECO_MODEL_NAME_TEXTURED = "fg_treeB_d_t";
const char* const DECO_ANIM_NAME = "fg_treeB_d";
// the buffer option of the model (nw::gfx::Model::Description::bufferOption)
const u32 DECO_BUFFER_OPTION = 16;
} // namespace

// 0x00592074 slot 0x00
// 0x00592030 (deleting dtor)
fgobj::ObjectDeco::~ObjectDeco()
{
    // (only the members are destroyed)
}

// 0x00591FFC slot 0x10
bool fgobj::ObjectDeco::vf_0x10()
{
    return true;
}

// 0x00592004 slot 0x20 (name is ours)
s32 fgobj::ObjectDeco::Execute()
{
    m_Model.UpdateAnims();
    UpdateMatrix(true, 0.0f);
    return EXECUTE_CONTINUE;
}

// 0x00591E18 (name is ours)
void fgobj::ObjectDeco::Setup(const SvFgName& item, const s32* x, const s32* z, bool isTextured)
{
    const char* name = isTextured ? DECO_MODEL_NAME_TEXTURED : DECO_MODEL_NAME;
    const void* model;
    if (s_IsOutdoor && s_pProc != NULL) {
        model = nw::gfx::GetModel(reinterpret_cast<const void* const*>(&s_pProc->m_AnimLoader.m_GraphicsFile), name);
    } else {
        model = NULL;
    }
    m_Item = item;
    Initialize(model, *x, *z, false, DECO_BUFFER_OPTION, true);
    UpdateMatrix(true, 0.0f);
    const void* anim;
    if (s_IsOutdoor && s_pProc != NULL) {
        anim = s_pProc->m_AnimLoader.m_GraphicsFile.GetTextureAnims(DECO_ANIM_NAME);
    } else {
        anim = NULL;
    }
    m_MaterialAnim.Build(&m_Model, nw::anim::res::ResAnim(anim), &m_Allocator, false);
    m_Model.SetMaterialAnim(&m_MaterialAnim, 0);
    u32 start = *z * 13 + *x * 7 + g_StageFrameCounter;
    m_MaterialAnim.SetFrame(static_cast<f32>(start % static_cast<u32>(m_MaterialAnim.GetEndFrame())));
    m_Model.EvaluateAnims();
    SetSortKey(*z, 0, -1, false, true, false, false);
}

} // namespace fgobj
