#include "bg/dCharRoom.h"
#include "Other/dHeapInit.h"
#include "Sv/dSvHouse.h"
#include "g3d/g3d_System.h"
#include "nw/gfx/gfx_Api.h"
#include "nw/gfx/gfx_Model.h"
#include "nw/gfx/res/gfx_ResModel.h"
#include "stage/stage_Api.h"

namespace {
// the texture scale of a My Design flooring per room size (2 small, 3 medium, 4 large; the museum rooms like the large)
const f32 DESIGN_SCALE_DEFAULT = 4.0f;
const f32 DESIGN_SCALE_MEDIUM = 6.0f;
const f32 DESIGN_SCALE_LARGE = 8.0f;

// (inline) the first texture of a loaded file
inline nw::gfx::res::ResTexture GetFirstTexture(item::ResLoader* loader)
{
    return nw::gfx::res::ResTexture(loader->GetGraphicsFile().ptr()->textures.GetData(0));
}
} // namespace

// 0x00582DAC (name is ours)
CharRoomFloor::CharRoomFloor() : m_IsInitialized(false)
{
}

// 0x00582518 (name is ours)
bool CharRoomFloor::Request(const RoomCovering* covering)
{
    bool isChanged = false;
    const s32 result = m_Bank.Exchange(Bank::Keyword(covering->item));
    if (result == 2) {
        const nw::gfx::res::ResTexture texture =
            GetFirstTexture(m_Bank.GetLoader(m_Bank.GetCurrentSlot(), 0));
        if (texture.IsValid()) {
            nw::gfx::res::ResMaterial material = m_Model.GetModel()->GetResModel().GetMaterial("m_carpet");
            if (material.IsValid()) {
                g3d::SetTexture(material, "dummy_carpet", texture);
                nw::gfx::res::ResTextureCoordinatorData* coordinator = &material.ptr()->textureCoordinators[0];
                if (covering->item.GetKind() == item::KIND_MY_DESIGN_TEXTURE) {
                    // a design is repeated over the floor
                    f32 scale = DESIGN_SCALE_DEFAULT;
                    const SvRoom* room = Func6FA758(stage::GetCurrent());
                    if (room != NULL) {
                        if (room->flags.roomSize == 2) {
                        } else if (room->flags.roomSize == 3) {
                            scale = DESIGN_SCALE_MEDIUM;
                        } else if (room->flags.roomSize == 4) {
                            scale = DESIGN_SCALE_LARGE;
                        }
                    }
                    if (stage::IsKind(stage::KIND_MUSEUM_SHOWROOM, stage::GetCurrent())) {
                        scale = DESIGN_SCALE_LARGE;
                    }
                    if (coordinator->scale.x != scale || coordinator->scale.y != scale) {
                        coordinator->scale.x = scale;
                        coordinator->scale.y = scale;
                        coordinator->isDirty = true;
                    }
                } else if (coordinator->scale.x != 1.0f || coordinator->scale.y != 1.0f) {
                    coordinator->scale.x = 1.0f;
                    coordinator->scale.y = 1.0f;
                    coordinator->isDirty = true;
                }
                material.ptr()->textureCoordinatorsHash = 0;
            }
        }
        Func529DBC(&m_Model, covering->param);
        isChanged = true;
    } else if (result == 1) {
        Func529DBC(&m_Model, covering->param);
    }
    return isChanged;
}

// 0x00582A2C (name is ours)
void CharRoomFloor::Initialize(g3d::ResourceLoader* room, const RoomCovering* covering, f32 height)
{
    (void)covering;
    sead::Heap* heap = HeapInit::s_pStageHeap;
    m_Allocator.CreateExpHeap(51200, heap, sead::SafeStringBase<char>("絨毯"), sead::Heap::HEAP_DIRECTION_FORWARD, false);
    m_Bank.CreateHeaps(heap, "絨毯");
    m_Bank.m_Alignment = 2;
    const void* position = nw::gfx::GetModel(reinterpret_cast<const void* const*>(&room->m_GraphicsFile), "position");
    const void* carpet = nw::gfx::GetModel(reinterpret_cast<const void* const*>(&room->m_GraphicsFile), "carpet");
    m_Model.Create(nw::gfx::res::ResSceneObject(carpet), &m_Allocator, &m_Allocator, 0, true, 1);
    if (carpet != NULL) {
        const s32 bone = g3d::GetBoneIndex(static_cast<const nw::gfx::res::ResSkeletalModelData*>(position), "pos_carpet");
        if (bone >= 0) {
            g3d::GetBoneTranslate(&m_Position,
                                  static_cast<const nw::gfx::res::ResSkeletalModelData*>(position), bone);
            nn::math::MTX34 matrix;
            matrix.m[0][0] = 1.0f;
            matrix.m[0][1] = 0.0f;
            matrix.m[0][2] = 0.0f;
            matrix.m[0][3] = m_Position.x;
            matrix.m[1][0] = 0.0f;
            matrix.m[1][1] = 1.0f;
            matrix.m[1][2] = 0.0f;
            matrix.m[1][3] = m_Position.y + height;
            matrix.m[2][0] = 0.0f;
            matrix.m[2][1] = 0.0f;
            matrix.m[2][2] = 1.0f;
            matrix.m[2][3] = m_Position.z;
            m_Model.SetMatrix(matrix);
            m_Model.Update();
        }
    }
    g3d::SetConstantColor(nw::gfx::res::ResModel(carpet).GetMaterial("m_carpet").ptr(), 0, -1, -1, -1, 200);
    m_IsInitialized = true;
}
