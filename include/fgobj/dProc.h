#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "g3d/dResourceLoader.h"
#include "fgobj/dDrawList.h"
#include "fgobj/dFieldBit.h"
#include "fgobj/dObjectList.h"
#include "fgobj/fgobj_Globals.h"
#include "g3d/dFunc.h"
#include "g3d/dModel.h"
#include "Other/ddLoadSplit.h"
#include "Other/dSeadParticleHandle.h"
#include "ssys/st/dListPriority.h"
#include "ssys/ma/dHeapAllocator.h"

namespace nw {
namespace gfx {
class Model;
} // namespace gfx
} // namespace nw

namespace fgobj {
class ObjectBase;
// an entry of the model table of the field objects in the Proc (by ParamFgEntry::fgModel; layout from ObjectBase, names are
// ours)
struct FgModel
{
    s8 shadowSize;      // 0x00 the size of the shadow (0: none)
    s8 shadowKind;      // 0x01 0..3 (ObjectBase::m_ShadowKind 0, 2, 3, 4)
    u8 unk_0x2[2];      // 0x02
    s8 material;        // 0x04 the material byte of the sort key (0: the model index)
    char name[16];      // 0x05 the name of the model in the graphics file of the field objects
    u8 nameEnd;         // 0x15 the terminator of the name (set to 0 before the name is used)
};
ASSERT_SIZE(FgModel, 0x16);

// the glitter of the watered flowers (ObjectBase::FLAG_UNK_10): now and then one of them gets the effect 250, every second
// time one next to the player when there is one (names are ours)
struct FlowerGlitter
{
    static const u32 TIMER_NUM = 8;
    // the frames between two glitters (each timer is set when it starts one; all count down, a free one starts the next)
    static const u8 TIMER_FRAMES = 90;
    // the frames after a glitter until the next can start / until the glitter of a flower ends
    static const u8 DELAY_FRAMES = 15;
    static const u8 FLOWER_GLITTER_FRAMES = 45;

    // the flowers are counted and their sway, delayed effects and glitter are updated (the scales of the sway are
    // s_SwayScales); when all have glittered they may glitter again
    void Update(ObjectList* objects); // 0x005A1610
    // a random flower (nearPlayer: of the 3 x 3 units around the player) starts to glitter (timer: the timer that started it)
    void Start(ObjectList* objects, s32 timer, bool nearPlayer); // 0x005A1824

    SeadParticleHandle m_Handle;    // 0x00
    s32 m_NearNum;                  // 0x18 the flowers next to the player that can glitter
    s32 m_Num;                      // 0x1C all flowers that can glitter
    u8 m_Timers[TIMER_NUM];         // 0x20 (start with TIMER_FRAMES)
    u8 m_Delay;                     // 0x28
};
ASSERT_SIZE(FlowerGlitter, 0x2C);
// the scale of a swaying field object for each frame of the sway ({x, y}, z is 1)
extern const f32 s_SwayScales[10][2];

// RTTI N5fgobj4ProcE @ 0x008D2A24
// vtable 0x00908DE4 (vptr 0x00908DEC), offset_to_top 0, 16 entries
class Proc : public ::Base
{
public:
    Proc(); // 0x0059F2F8 (name is ours)
    virtual ~Proc(); // 0x0059F64C slot 0x00 | slot vf_0x00 of oml::framework::Process
    virtual oml::framework::Result Initialize(); // 0x0059E03C slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual oml::framework::Result Finalize(); // 0x0059EF40 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual oml::framework::Result Calc(); // 0x0059EAA4 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual oml::framework::Result Draw(); // 0x0059DFC4 slot 0x30 | slot vf_0x30 of oml::framework::Process

    // m_FieldObjects: which objects RemoveObjectsAt removes
    enum RemoveMode
    {
        REMOVE_MODE_ALL = 0,
        REMOVE_MODE_FLAG_4 = 1,     // only objects with ObjectBase::FLAG_UNK_4
        REMOVE_MODE_NO_FLAG_4 = 2,  // only objects without it
    };

    // whether the Proc exists (s_pProc)
    static bool Exists(); // 0x0059E588

    // (names are ours) the states of LoadSeasonResources
    enum LoadState
    {
        LOAD_STATE_NONE,
        LOAD_STATE_LOAD,  // FgShape.bin and the graphics file of the season are loaded
        LOAD_STATE_SETUP, // one frame
        LOAD_STATE_FINISH,
        LOAD_STATE_DONE,
        // LoadItemResources
        LOAD_STATE_ITEM_SETUP,
        LOAD_STATE_ITEM_FINISH,
        LOAD_STATE_ITEM_DONE,
        // Initialize: Fg/Bcres/FgObjAnim.bcres
        LOAD_STATE_ANIM_SETUP,
        LOAD_STATE_ANIM_FINISH,
        LOAD_STATE_ANIM_DONE,
    };
    static const u32 QUEUED_UNIT_NUM = 10;

    // the outdoor resources (the model table Fg/Param/FgShape.bin, the graphics file Fg/Bcres/Season%02d.bcres of the
    // season part of the date and the addresses of the tree textures); true when done (names are ours)
    bool LoadSeasonResources(); // 0x0059AB58
    // the field objects of the queued units are created (indoors with a second try and then Func150218, outdoors also
    // the missing units of m_Areas[1] until the counter reaches 10)
    void CreateQueuedFieldObjects(); // 0x0059AF08
    // the field object of the item at the unit x, z (isUpper: of layer 1 indoors) in a small pool heap; outdoors only in the
    // areas (the units of m_FieldBit2 in m_Areas[0]), with the fruits of a ripe fruit tree and the decoration of a cedar;
    // indoors registered with ModuleFtr; false when there is none (then the unit is marked as done, name is ours)
    bool CreateFieldObject(s32 x, s32 z, bool isUpper); // 0x0059E5A0
    // the item models: Fg/Bcres/FgItemAutoCamp.bcres (stage::KIND_AUTO_CAMP, with fog), FgItemOutside or FgItemInside, and
    // the "sold out" signs of the region; true when done (states LOAD_STATE_DONE..; name is ours)
    bool LoadItemResources(); // 0x0059B12C
    // the texture textureName (names[0]) of the material of the item model modelName is replaced by names[region of the town]
    void ReplaceRegionTexture(const char* modelName, const char* textureName, const char* const* names); // 0x0059D5D0
    // the decoration of a cedar (NAME_CEDAR_XMAS) at the unit x, z: an ObjectDeco in a large pool heap among the field
    // objects
    void PlaceDeco(const SvFgName& item, s32 x, s32 z); // 0x0059BDAC
    // the visible objects of the list from first go to the draw lists (FLAG_UNK_20000: the translucent one) and their
    // shadows are set (indoors; outdoors AddToDrawListsOutdoor)
    void AddToDrawLists(ObjectBase* first); // 0x0059BEB8
    void AddToDrawListsOutdoor(ObjectBase* first); // 0x0059DE40
    // the objects of list are drawn with the view of the top screen (s_DrawMethods) and the list is emptied; the
    // functions of m_Func for the opaque and the translucent pass
    void DrawObjects(nw::gfx::MeshRenderer* renderer, DrawList* list); // 0x0059BFE0
    static void DrawOpaque(nw::gfx::MeshRenderer* renderer, void* proc); // 0x0059BFC8
    static void DrawTranslucent(nw::gfx::MeshRenderer* renderer, void* proc); // 0x0059D6B4
    // the areas of units around the point of the camera (indoors the whole field): m_Areas[0] where the field objects are
    // shown, [1] where they are created, [2] = [0] one unit smaller; by the stage (Downtown, the exhibition, the town with
    // the ground kinds 11..13/16/17 of the block of the camera: larger); false without the camera (name is ours)
    DECOMP_NOINLINE bool UpdateAreas(); // 0x0059C730
    // the heap "BsFgObject" in m_Allocator3 with the pool heaps of the objects: small FrameHeaps (s_SmallHeaps) and large
    // ExpHeaps (s_LargeHeaps); their number and size by the stage (auto camp, outdoors, indoors); once (name is ours)
    DECOMP_NOINLINE void CreateHeaps(); // 0x0059CC8C
    // the heaps of the Proc on the stage heap: "BsFgProc::Setup" (m_Allocator), "BsFgProc::Resource" (m_Allocator2) and
    // outdoors "BsFgProc::MyDsnMgr" for the design textures; then LoadSeasonResources starts (name is ours)
    void CreateResourceHeaps(); // 0x0059D2C8
    // the field object at the unit x, z (NULL: none; name is ours)
    ObjectBase* FindFieldObject(s32 x, s32 z) const; // 0x0059D3E0
    // the fruits of the fruit tree tree at the unit x, z: three ObjectFruit of fruit (a perfect one where the tree has its
    // perfect fruit; on the island a little further in front) / two of a palm (the offsets and angles of a banana palm
    // differ) among the field objects (names are ours)
    void PlaceFruits(const SvFgName* tree, const SvFgName* fruit, s32 x, s32 z); // 0x0059D4AC
    void PlacePalmFruits(const SvFgName* tree, const SvFgName* fruit, s32 x, s32 z); // 0x0059DA74
    // the physical address of the texture name of the graphics file of the season goes to s_TreeTextureAddresses[index]
    void SetTreeTextureAddress(u32 index, const char* name); // 0x0059D99C

    // the object leaves the field bit, its list (isFieldObject: m_FieldObjects, else m_ExeObjects; only with
    // removeFromList) and the draw lists, then it is finalized
    void RemoveObject(ObjectBase* object, bool isFieldObject, bool removeFromList); // 0x0059F240
    // the field objects at the unit x, z (by mode) are removed and the field bit of the unit is reset
    DECOMP_NOINLINE void RemoveObjectsAt(s32 x, s32 z, u32 mode); // 0x005A3158
    // the table of the models (the buffer of m_FgModelLoad, FG_MODEL_NUM entries; NULL before it is loaded)
    FgModel* GetFgModels() const
    {
        return reinterpret_cast<FgModel*>(m_FgModelLoad.m_pData);
    }

    // a trapezoid of units: the rows of z from minZ narrow by (z - minZ) * scale units on both sides (names are ours)
    struct UnitArea
    {
        // whether the unit x, z is inside
        bool Contains(s32 x, s32 z) const; // 0x0059573C
        // the area around center: halfWidth to both sides in x, back in z and front in z (world units); the rows narrow by
        // narrowing units over the depth
        void Set(const nn::math::VEC3* center, f32 halfWidth, f32 back, f32 front, f32 narrowing); // 0x005957BC

        s32 minX;   // 0x00
        s32 minZ;   // 0x04
        s32 maxX;   // 0x08
        s32 maxZ;   // 0x0C
        f32 scale;  // 0x10
    };

    // the size of the model table
    static const u32 FG_MODEL_NUM = 135;

    // (layout from the constructor; names are ours)
    ssys::ma::HeapAllocator m_Allocator;    // 0x0014 the memory of the models and the FuncNode
    ssys::ma::HeapAllocator m_Allocator2;   // 0x001C (its heap loads FgObjAnim.bcres)
    ssys::ma::HeapAllocator m_Allocator3;   // 0x0024
    g3d::ResourceLoader m_FgLoader;         // 0x002C the models of the field objects (FgModel::name, cracks, holes)
    g3d::ResourceLoader m_ItemLoader;       // 0x0134 the models of the items (s_ModelInfos)
    g3d::ResourceLoader m_AnimLoader;       // 0x023C Fg/Bcres/FgObjAnim.bcres
    dLoadSplit m_FgModelLoad;               // 0x0344 the table of the models (GetFgModels)
    FlowerGlitter m_Glitter;                // 0x0430 the glitter of the watered flowers
    MyDesignTextures m_MyDesignTextures;    // 0x045C (s_pMyDesignTextures)
    SeadParticleHandle m_ParticleHandles[PARTICLE_HANDLE_NUM]; // 0x3024 (s_pParticleHandles)
    // the units whose field objects are created next (CreateQueuedFieldObjects takes them from the end)
    u8 m_QueuedUnitX[QUEUED_UNIT_NUM];      // 0x3204
    u8 m_QueuedUnitZ[QUEUED_UNIT_NUM];      // 0x320E
    u8 m_QueuedUnitNum;                     // 0x3218
    u32 m_Unknown0x321C[100];               // 0x321C
    u32 m_Unknown0x33AC[100];               // 0x33AC
    // the units of the current field (0, 0 to the blocks * 16, set by Initialize)
    s32 m_UnitMinX;                         // 0x353C
    s32 m_UnitMinZ;                         // 0x3540
    s32 m_UnitMaxX;                         // 0x3544
    s32 m_UnitMaxZ;                         // 0x3548
    UnitArea m_Areas[3];                    // 0x354C
    s32 m_CameraUnitX;                      // 0x3588 the unit of the point of the camera (UpdateAreas)
    s32 m_CameraUnitZ;                      // 0x358C
    s16 m_FieldObjectCounter;               // 0x3590 starts at -1000, one more per created field object
    s16 m_RemovedObjectCounter;             // 0x3592 the field objects removed in this frame (Calc removes at most 20)
    ObjectList m_ExeObjects;                // 0x3594 the objects that run (ObjectExe::Execute), by sort key
    ObjectList m_FieldObjects;              // 0x35A0 the objects that stand on the field (with the decorations)
    DrawList m_DrawLists[2];                // 0x35AC (of ObjectBase::m_DrawNode)
    FieldBit m_FieldBit;                    // 0x35C4 the units whose objects changed
    FieldBit m_FieldBit3;                   // 0x3B08 (tested before a field object is created; meaning unknown)
    FieldBit m_FieldBit2;                   // 0x404C units whose bit RefreshUnit moves to m_FieldBit instead of removing the objects
    g3d::Model m_SettingModelObjects[2];    // 0x4590 the objects of m_pSettingModels
    nw::gfx::Model* m_pSettingModels[2];    // 0x45A8 the models "setting_t_outside" and "setting_t_outside_2"
    g3d::Func m_Func;                       // 0x45B0 draws the field objects (Initialize)
    u8 m_LoadState;                         // 0x45B8 LoadState (LoadSeasonResources)
};
ASSERT_OFFSET(Proc, m_FgLoader, 0x2C);
ASSERT_OFFSET(Proc, m_ItemLoader, 0x134);
ASSERT_OFFSET(Proc, m_FgModelLoad, 0x344);
ASSERT_OFFSET(Proc, m_Glitter, 0x430);
ASSERT_OFFSET(Proc, m_MyDesignTextures, 0x45C);
ASSERT_OFFSET(Proc, m_ParticleHandles, 0x3024);
ASSERT_OFFSET(Proc, m_UnitMinX, 0x353C);
ASSERT_OFFSET(Proc, m_ExeObjects, 0x3594);
ASSERT_OFFSET(Proc, m_FieldBit, 0x35C4);
ASSERT_OFFSET(Proc, m_FieldBit2, 0x404C);
ASSERT_OFFSET(Proc, m_pSettingModels, 0x45A8);
ASSERT_OFFSET(Proc, m_LoadState, 0x45B8);
ASSERT_SIZE(Proc, 0x45BC);

// the objects of the unit x, z of the current Proc end (the field objects and the running ones) and the unit is marked
// as changed (name is ours)
void ClearUnit(s32 x, s32 z); // 0x0059E4F0
} // namespace fgobj
