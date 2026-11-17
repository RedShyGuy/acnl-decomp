#include "hobj/dMyHouse.h"

#include "Other/dHeapInit.h"
#include "Sound/sound_Extern.h"
#include "g3d/dResourceLoader.h"
#include "g3d/g3d_System.h"
#include "hobj/hobj_Extern.h"
#include "nw/gfx/gfx_Model.h"
#include "sead/seadExpHeap.h"
#include "sead/seadFixedSafeString.h"
#include "sead/seadStringFormatter.h"

namespace hobj {
// 0x00953CC8 (name is ours)
const char* g_DoorShapeSuffixes[2] = {"a", "b"};

inline void MyHouse::ApplyWallColor()
{
    const f32 brightness = Func1E7A5C();
    g3d::SetConstantColor(m_pWallMaterial, 2, static_cast<s32>(m_Color[3] * brightness),
                          static_cast<s32>(m_Color[2] * brightness), static_cast<s32>(m_Color[1] * brightness), -1);
}

// 0x00533CA8 slot 0x14 (name is ours)
void MyHouse::Update(const nn::math::VEC3* world, bool isPaused)
{
    m_Door.Update(world, isPaused);
}

// 0x00533CB4 slot 0x0C (name is ours)
void MyHouse::Build(const void* houseData)
{
    g3d::ResourceLoader* shared = Func6DB618(g_Unknown9516C0.p4);
    shared->InitializeCgfx(&m_Allocator, NULL, true);
    g3d::ResourceLoader* model = m_ResourceMgr.Get(m_ModelKey);
    model->Read(0x10000, 0x20000, 0x30000);
    model->InitializeCgfx(&m_Allocator, NULL, true);
    model->InitializeCgfx(&m_Allocator, shared, true);
    m_Model.Create(nw::gfx::res::ResSceneObject(model->GetGraphicsFile().GetModels(0)), &m_Allocator, &m_Allocator, 0,
                   true, 1);
    g3d::ResourceLoader* roof = m_ResourceMgr.Get(m_RoofKey);
    g3d::ReplaceTexture(&m_Model, roof, &m_Allocator, "m_roof", "roof_dummy");
    g3d::ResourceLoader* wall = m_ResourceMgr.Get(m_WallKey);
    g3d::ReplaceTexture(&m_Model, wall, &m_Allocator, "m_wall", "wall_dummy");
    g3d::ResourceLoader* wallMask = m_ResourceMgr.Get(m_WallMaskKey);
    g3d::ReplaceTexture(&m_Model, wallMask, &m_Allocator, "m_wall", "wall_mask_dummy");
    m_pWallMaterial = detail::FindMaterialData(m_Model, "m_wall");
    if (detail::GetMaterialIndex(m_Model, "m_lamp") >= 0) {
        m_pLampMaterial = detail::FindMaterialData(m_Model, "m_lamp");
        g3d::ReplaceTexture(&m_Model, wall, &m_Allocator, "m_lamp", "wall_dummy");
        g3d::ReplaceTexture(&m_Model, wallMask, &m_Allocator, "m_lamp", "wall_mask_dummy");
    }
    m_Door.Build();
    u8 color[4];
    Func720F9C(color, houseData);
    m_Color[3] = color[3];
    m_Color[2] = color[2];
    m_Color[1] = color[1];
    m_Color[0] = color[0];
    ApplyWallColor();
}

// 0x00534210 slot 0x08 (name is ours)
bool MyHouse::Load()
{
    sead::FixedSafeString<128> path;
    {
        const s32 size = m_Exterior.houseSize;
        const u8 doorShape = m_Exterior.houseDoorShape;
        sead::StringFormatter formatter(&path);
        formatter.append("Strc/hobj/my_house/hobj_myhouse_%02d/hobj_myhouse_%02d%s.bcres");
        formatter.appendValue(size);
        formatter.appendValue(size);
        formatter.append(g_DoorShapeSuffixes[doorShape]);
        formatter.flush();
    }
    m_ModelKey = m_ResourceMgr.Load(path.getCString());
    {
        const s32 brick = m_Exterior.houseBrick;
        sead::StringFormatter formatter(&path);
        formatter.append("Strc/hobj/my_house/Textures/season00/wall_%02d.bcres");
        formatter.appendValue(brick);
        formatter.flush();
    }
    m_WallKey = m_ResourceMgr.Load(path.getCString());
    {
        const s32 roof = m_Exterior.houseRoof;
        sead::StringFormatter formatter(&path);
        formatter.append("Strc/hobj/my_house/Textures/%s/roof_%02d.bcres");
        formatter.append(g_SeasonTextureFolders[Func630618(g_Unknown9516C0.p4)]);
        formatter.appendValue(roof);
        formatter.flush();
    }
    m_RoofKey = m_ResourceMgr.Load(path.getCString());
    sead::FixedSafeString<80> maskPath;
    {
        sead::StringFormatter formatter(&maskPath);
        formatter.append("Strc/hobj/my_house/Textures/season00/wall_mask_%02d.bcres");
        formatter.appendValue(m_Exterior.houseBrick);
        formatter.flush();
    }
    m_WallMaskKey = m_ResourceMgr.Load(maskPath.getCString());
    const bool isLoaded = m_ModelKey.IsValid() & m_WallKey.IsValid() & m_RoofKey.IsValid() & m_WallMaskKey.IsValid();
    return m_Door.Load() & isLoaded;
}

// 0x005346E4 (name is ours)
void MyHouse::Initialize(const SvHouseExterior* exterior, const nn::math::VEC3* world)
{
    sead::Heap* parent = HeapInit::s_pStrcHeap;
    m_Allocator.CreateExpHeap(0x1600, parent, "hobj::MyHouse::Allocator", sead::Heap::HEAP_DIRECTION_FORWARD, false);
    m_ResourceMgr.Initialize(parent, 0x1E400);
    m_Exterior = *exterior;
    m_Door.Initialize(exterior->houseDoorShape, exterior->houseDoor, world);
}

// 0x00534888 slot 0x20 (name is ours)
void MyHouse::OpenDoor(bool isSecondAnim, netgame::PlayerNo player)
{
    m_Door.Open(isSecondAnim, player);
}

// 0x00534894 slot 0x28 (name is ours)
bool MyHouse::HoldDoorOpen(bool isSecondAnim)
{
    return m_Door.HoldOpen(isSecondAnim);
}

// 0x005348A0 slot 0x2C (name is ours)
void MyHouse::CloseDoor()
{
    m_Door.Close();
}

// 0x005348AC slot 0x34 (name is ours)
void MyHouse::vf_0x34()
{
    // empty (in the original too)
}

// 0x005348B0 slot 0x3C (name is ours)
void MyHouse::vf_0x3C()
{
    // empty (in the original too)
}

// 0x005348B4 slot 0x38 (name is ours)
void MyHouse::vf_0x38()
{
    // empty (in the original too)
}

// 0x005348B8 slot 0x30 (name is ours)
void MyHouse::UpdateColor(const void* houseData)
{
    u8 color[4];
    Func720F9C(color, houseData);
    if (color[3] == m_Color[3] && color[2] == m_Color[2] && color[1] == m_Color[1] && !Func1E8844()) {
        return;
    }
    m_Color[3] = color[3];
    m_Color[2] = color[2];
    m_Color[1] = color[1];
    m_Color[0] = color[0];
    ApplyWallColor();
}

// 0x00534990 slot 0x24 (name is ours)
void MyHouse::OpenDoorWithChime(bool isSecondAnim, netgame::PlayerNo player, const u8* packedMelody)
{
    m_Door.OpenWithChime(isSecondAnim, packedMelody, player);
}

// 0x005349A8 slot 0x1C (name is ours)
void MyHouse::AddToScene()
{
    m_Door.AddToScene();
    m_Model.AddToScene(0);
}

// 0x005349D0 slot 0x18 (name is ours)
void MyHouse::UpdateMatrix(const nn::math::MTX34& matrix)
{
    const u8* object = static_cast<const u8*>(g_Unknown9516C0.p4);
    if (m_pLampMaterial != NULL) {
        // (the alpha of the lamp is a value of the object at +0x6AC8)
        g3d::SetConstantColor(m_pLampMaterial, 0, -1, -1, -1, *reinterpret_cast<const s32*>(object + 0x6AC8));
    }
    m_Door.UpdateMatrix(matrix);
    m_Model.SetMatrix(matrix);
    m_Model.g3d::SkeletalModel::Update();
}

// 0x00534A50 slot 0x10 (name is ours)
void MyHouse::Finalize()
{
    m_Door.Finalize();
    if (m_Model.GetNode() != NULL) {
        m_Model.g3d::Model::Finalize();
    }
    m_ResourceMgr.Finalize();
    m_Allocator.DestroyHeap();
}

// 0x00534B38 (name is ours)
MyHouse::MyHouse() : m_pWallMaterial(NULL), m_pLampMaterial(NULL)
{
    m_Color[0] = 255;
    m_Color[1] = 0;
    m_Color[2] = 0;
    m_Color[3] = 0;
    m_Exterior.SetDefault();
}

// 0x00534C70 slot 0x00
// 0x00534BF4 (deleting dtor)
MyHouse::~MyHouse()
{
    // (only the members are destroyed)
}
} // namespace hobj
