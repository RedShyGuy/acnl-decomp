#include "hobj/dNpcHouse.h"

#include "Sound/sound_Extern.h"
#include "g3d/dResourceLoader.h"
#include "g3d/g3d_System.h"
#include "hobj/dHouseBase.h"
#include "hobj/hobj_Extern.h"
#include "nw/gfx/res/gfx_ResSkeleton.h"
#include "sead/seadFixedSafeString.h"
#include "sead/seadStringFormatter.h"

namespace hobj {
namespace {
// the first texture of a loaded file
inline nw::gfx::res::ResTexture GetFirstTexture(g3d::ResourceLoader* file)
{
    return nw::gfx::res::ResTexture(file->GetGraphicsFile().ptr()->textures.GetData(0));
}
} // namespace

// 0x00530708 (name is ours)
void NpcHouse::FormatRoofPath(sead::BufferedSafeStringBase<char>* path, s32 kind, s32 roof)
{
    sead::StringFormatter formatter(path);
    formatter.append("Strc/hobj/npc_house/hobj_npchouse_%02d/Textures/%s/roof_%d_%02d.bcres");
    formatter.appendValue(kind);
    formatter.append(g_SeasonTextureFolders[Func630618(g_Unknown9516C0.p4)]);
    formatter.appendValue(kind);
    formatter.appendValue(roof);
    formatter.flush();
}

// 0x00534CE8 (name is ours)
void NpcHouse::Build(const nn::math::VEC3* world)
{
    g3d::ResourceLoader* shared = Func6DB618(g_Unknown9516C0.p4);
    shared->InitializeCgfx(&m_Allocator, NULL, true);
    g3d::ResourceLoader* model = m_ResourceMgr.Get(m_ModelKey);
    model->Read(0x10000, 0x20000, 0x30000);
    model->InitializeCgfx(&m_Allocator, NULL, true);
    model->InitializeCgfx(&m_Allocator, shared, true);
    const nw::gfx::res::ResSkeletalModelData* resModel =
        static_cast<const nw::gfx::res::ResSkeletalModelData*>(model->GetGraphicsFile().GetModels(0));
    m_Model.Create(nw::gfx::res::ResSceneObject(resModel), &m_Allocator, &m_Allocator, 0, true, 1);
    g3d::ResourceLoader* roof = m_ResourceMgr.Get(m_RoofKey);
    roof->InitializeCgfx(&m_Allocator, NULL, true);
    g3d::SetTexture(m_Model.GetModel()->GetResModel().FindMaterial("m_roof"), "roof_dummy", GetFirstTexture(roof));
    g3d::ResourceLoader* wall = m_ResourceMgr.Get(m_WallKey);
    wall->InitializeCgfx(&m_Allocator, NULL, true);
    g3d::SetTexture(m_Model.GetModel()->GetResModel().FindMaterial("m_wall"), "wall_dummy", GetFirstTexture(wall));
    g3d::ResourceLoader* wallMask = m_ResourceMgr.Get(m_WallMaskKey);
    wallMask->InitializeCgfx(&m_Allocator, NULL, true);
    g3d::SetTexture(m_Model.GetModel()->GetResModel().FindMaterial("m_wall"), "wall_mask_dummy",
                    GetFirstTexture(wallMask));
    m_pWallMaterial = detail::FindMaterialData(m_Model, "m_wall");

    const nw::gfx::res::ResSkeletonData* skeleton =
        static_cast<const nw::gfx::res::ResSkeletonData*>(resModel->skeleton.ToPtr());
    nw::ut::ResDicPatricia bones = {skeleton->boneDic.To<nw::ut::ResDicPatricia::Data>()};
    const nw::gfx::res::ResBoneData* smoke = static_cast<const nw::gfx::res::ResBoneData*>(bones.GetData("smoke"));
    m_SmokePosition.x = smoke->transform.translate.x;
    m_SmokePosition.y = smoke->transform.translate.y;
    m_SmokePosition.z = smoke->transform.translate.z;
    m_SmokeMatrix.m[0][0] = 1.0f;
    m_SmokeMatrix.m[0][1] = 0.0f;
    m_SmokeMatrix.m[0][2] = 0.0f;
    m_SmokeMatrix.m[1][0] = 0.0f;
    m_SmokeMatrix.m[1][1] = 1.0f;
    m_SmokeMatrix.m[1][2] = 0.0f;
    m_SmokeMatrix.m[2][0] = 0.0f;
    m_SmokeMatrix.m[2][1] = 0.0f;
    m_SmokeMatrix.m[2][2] = 1.0f;
    m_SmokeMatrix.m[0][3] = m_SmokePosition.x;
    m_SmokeMatrix.m[1][3] = m_SmokePosition.y;
    m_SmokeMatrix.m[2][3] = m_SmokePosition.z;
    m_SmokePosition.x += world->x;
    m_SmokePosition.y += world->y;
    m_SmokePosition.z += world->z;

    u8 color[4];
    Func1E63B0(color, 20);
    m_Color[3] = color[3];
    m_Color[2] = color[2];
    m_Color[1] = color[1];
    m_Color[0] = color[0];
    g3d::SetConstantColor(m_pWallMaterial, 2, m_Color[3], m_Color[2], m_Color[1], -1);
}

// 0x005353B4 (name is ours)
bool NpcHouse::Load()
{
    sead::FixedSafeString<128> path;
    {
        const s32 kind = m_Kind;
        sead::StringFormatter formatter(&path);
        formatter.append("Strc/hobj/npc_house/hobj_npchouse_%02d/hobj_npchouse_%02d.bcres");
        formatter.appendValue(kind);
        formatter.appendValue(kind);
        formatter.flush();
    }
    m_ModelKey = m_ResourceMgr.Load(path.getCString());
    {
        const s32 wallVariant = m_Wall;
        const s32 kind = m_Kind;
        sead::StringFormatter formatter(&path);
        formatter.append("Strc/hobj/npc_house/hobj_npchouse_%02d/Textures/season00/wall_%d_%02d.bcres");
        formatter.appendValue(kind);
        formatter.appendValue(kind);
        formatter.appendValue(wallVariant);
        formatter.flush();
    }
    m_WallKey = m_ResourceMgr.Load(path.getCString());
    sead::FixedSafeString<80> maskPath;
    {
        sead::StringFormatter formatter(&maskPath);
        formatter.append("Strc/hobj/npc_house/hobj_npchouse_%02d/Textures/season00/wall_mask_%d_%02d.bcres");
        formatter.appendValue(static_cast<s32>(m_Kind));
        formatter.appendValue(static_cast<s32>(m_Kind));
        formatter.appendValue(static_cast<s32>(m_Wall));
        formatter.flush();
    }
    m_WallMaskKey = m_ResourceMgr.Load(maskPath.getCString());
    FormatRoofPath(&path, m_Kind, m_Roof);
    m_RoofKey = m_ResourceMgr.Load(path.getCString());
    return m_ModelKey.IsValid() & m_WallKey.IsValid() & m_WallMaskKey.IsValid() & m_RoofKey.IsValid();
}
} // namespace hobj
