#pragma once

#include "decomp.h"
#include "Bank/dBankTableBase.h"
#include "Human/dHumanFaceAnimControl.h"
#include "Human/dHumanModel.h"
#include "Sv/dSvFgName.h"
#include "g3d/dResourceLoader.h"

class HumanResTextureAnimBank;

namespace sead {
template <typename T>
class IDelegate1;
template <typename T>
class BufferedSafeStringBase;
} // namespace sead

namespace stage {
struct NpcInfo;
} // namespace stage

namespace nw {
namespace gfx {
class CalculatedTransform;
namespace res {
struct ResModelData;
} // namespace res
} // namespace gfx
namespace ut {
struct FloatColor;
} // namespace ut
} // namespace nw

// The model of a villager or a special character (class name from the RTTI, the layout from the constructor
// 0x0020674C; all other names are ours): the model of "Npc/Normal/Model/<villager>.bcres" or
// "Npc/Special/Model/<model>.bcres" in its own ResourceLoader (Load) or the models of BsNpcMgr (the guests, "mka",
// "pkn"), the face in a HumanFaceAnimControl with the banks of the derived classes, the root translations and an
// offset of the height (sitting on a chair, the base bone).
// RTTI 12NpcModelBase @ 0x008CB7A8
// vtable 0x008EE6D4 (vptr 0x008EE6DC), offset_to_top 0, 20 entries
class NpcModelBase : public ::HumanModel
{
public:
    // m_LoadState of Load
    enum LoadState
    {
        LOAD_STATE_BANKS = 0,  // the banks are acquired
        LOAD_STATE_FILE = 1,   // the file is requested
        LOAD_STATE_SETUP = 2,  // the graphics file is set up
        LOAD_STATE_MODEL = 3,  // the model is built
        LOAD_STATE_DONE = 4,
    };
    // m_Flags
    enum Flag
    {
        FLAG_LOADED = 1 << 1,       // Load built the model
        FLAG_UPPER_BODY = 1 << 2,   // PlayAnim plays in slot 1 from the bone chest
        FLAG_PKN_MODEL = 1 << 5,    // the model of BsNpcMgr "pkn"
        FLAG_MKA_MODEL = 1 << 6,    // the model of BsNpcMgr "mka"
        FLAG_GUEST_MODEL = 1 << 7,  // the guest model of BsNpcMgr
    };
    // the variants of a special model by the stage (ApplyModelVariant; meaning unknown)
    enum ModelVariant
    {
        MODEL_VARIANT_MINI_GAME_1 = 1,
        MODEL_VARIANT_MINI_GAME_0 = 2,
        MODEL_VARIANT_AMIIBO_CAMERA = 3,
        MODEL_VARIANT_NONE = 4,
    };
    // a material name with its render priority (the table g_NpcMeshPriorities)
    struct MeshPriority
    {
        const char* name; // 0x0
        u32 length;       // 0x4 the characters compared
        u8 priority;      // 0x8 (2: the face takes the talk animations from the face control)
    };
    struct MeshPriorityTable
    {
        MeshPriority* entries; // 0x0
        u32 num;               // 0x4
        u8 defaultPriority;    // 0x8
    };

    NpcModelBase(); // 0x0020674C
    // PlayAnim of HumanModel (with FLAG_UPPER_BODY in slot 1 from the bone chest), then the root and the callback
    bool PlayAnim(u16 id, u32 playMode, u32 flags, f32 frame, f32 speed, f32 duration, bool* changed, u8 morphType) override; // 0x0020639C slot 0x00
    // the frames of the animations and the face
    void vf_0x08() override; // 0x004F14A0 slot 0x08
    // the animations are evaluated and the height offset follows the animation
    void vf_0x0C() override; // 0x00206534 slot 0x0C
    // the world matrices with the head turned
    void vf_0x10() override; // 0x001AC808 slot 0x10
    // the model goes into the first sort group (after slot 0x4C)
    void vf_0x14() override; // 0x004EE434 slot 0x14
    HumanFaceAnimControl* GetFaceCtrl() override; // 0x0020630C slot 0x18
    virtual ~NpcModelBase(); // 0x00206954 slot 0x1C
    // 0x002068B8 slot 0x20 (deleting dtor)
    // the world matrix (scaled by m_Scale in y and z), turned with the curve of the world at the root
    void SetMtx(const nn::math::MTX34* mtx) override; // 0x00204228 slot 0x24
    // (Unload)
    virtual bool vf_0x2C(); // 0x00304260 slot 0x2C
    // Load with info (the model index and the flag of the derived class)
    virtual bool StartLoad(const stage::NpcInfo* info, u16 modelIndex); // 0x002058A8 slot 0x30
    // a running load ends (Load with skipCommon), then the model is finalized, the file unloaded and the banks given
    // back (false: the load goes on)
    virtual bool Unload(); // 0x00206668 slot 0x34
    // the banks of the model are acquired / the allocator of the model and the face banks of the handles / the banks
    // are given back
    virtual bool AcquireBanks(BankHandle* model, BankHandle* faceA, BankHandle* faceB) = 0; // slot 0x38
    virtual ssys::ma::Allocator* GetModelAllocator(BankHandle* handle) = 0; // slot 0x3C
    virtual HumanResTextureAnimBank* GetFaceBankA(BankHandle* handle) = 0; // slot 0x40
    virtual HumanResTextureAnimBank* GetFaceBankB(BankHandle* handle) = 0; // slot 0x44
    virtual void ReleaseBanks(BankHandle* model, BankHandle* faceA, BankHandle* faceB) = 0; // slot 0x48
    // (vf_0x14 calls it before the model goes into the sort group; NpcModelBase: 0; meaning unknown)
    virtual u32 vf_0x4C(); // 0x00205E9C slot 0x4C

    // the model in its own ResourceLoader (the steps of LoadState; skipCommon: done without the common resources of
    // BsNpcMgr); true while it goes on or when it is done
    bool Load(const stage::NpcInfo* info, u16 modelIndex, bool skipCommon); // 0x002058B4
    // the model of its own ResourceLoader again (m_ModelIndex 0)
    bool SetupModel(); // 0x002042C0
    // the same without the flags of the models of BsNpcMgr
    void SetupModelNoGuest(); // 0x00204B24
    void SetupModelNoMka(); // 0x00204B40
    void SetupModelNoPkn(); // 0x00204B5C
    // the models of BsNpcMgr (false: not loaded there)
    bool SetupGuestModel(); // 0x00204B78
    bool SetupMkaModel(); // 0x00204F28
    bool SetupPknModel(u32 unused, u8 bodyTexture); // 0x002052D8
    // the texture "b.<variant>" of the file on the material "b" of model (variant 0..4)
    bool SetBodyTexture(const nw::gfx::res::ResModelData* model, u8 variant); // 0x002056CC
    // the texture of the file on a material of the model
    bool SetTexture(const char* textureName, const char* materialName, const char* slotName); // 0x00204638
    // the path of the model file of info
    void BuildModelPath(sead::BufferedSafeStringBase<char>* path, const stage::NpcInfo* info); // 0x00204758
    // the meshes of a material are visible or not
    void SetMeshVisible(const char* materialName, bool visible); // 0x00205EA4
    // the scale of the model
    void SetScale(f32 scale); // 0x002066F8
    // the colors of the model of the file
    void SetEmission(const nw::ut::FloatColor& color); // 0x00206314
    void SetAmbientAndDiffuse(f32 value); // 0x0020633C
    void SetEmission(f32 value); // 0x0020636C
    void SetItem37C(SvFgName item); // 0x0020650C

    // the variant of a special model (name: the model; out: the item of "upb"; variant: ModelVariant); false: no change
    static bool ApplyModelVariant(sead::BufferedSafeStringBase<char>* name, SvFgName* out, s32 variant); // 0x0020601C

    // the default chair offset (none)
    static const f32 s_DefaultChairOffset[2];

    BankHandle m_ModelHandle;                     // 0x144
    BankHandle m_FaceHandleA;                     // 0x14C
    BankHandle m_FaceHandleB;                     // 0x154
    g3d::ResourceLoader m_ResourceLoader;         // 0x15C
    BankHandle m_TopsHandle;                      // 0x264 (the banks of NpcModel)
    BankHandle m_ToolHandle;                      // 0x26C
    BankHandle m_AccessoryHandle;                 // 0x274
    u16 m_LoadState;                              // 0x27C LoadState
    u16 m_ModelIndex;                             // 0x27E
    u8 m_Flags;                                   // 0x280 Flag
    u8 m_ItemFlags;                               // 0x281 (NpcModel: 1 tops, 2 accessory, 4 tool)
    RootCtrl m_RootCtrl;                          // 0x284
    HumanFaceAnimControl m_FaceCtrl;              // 0x2B0
    f32 m_Scale;                                  // 0x344
    f32 m_HeightOffset;                           // 0x348 (blends over m_BlendFrames)
    f32 m_Unknown34C;                             // 0x34C
    f32 m_FootOffsetX;                            // 0x350 (the bone Lfoot1 in the resource)
    f32 m_BlendFrames;                            // 0x354
    nw::gfx::CalculatedTransform* m_pBaseTransform;  // 0x358
    nw::gfx::CalculatedTransform* m_pChestTransform; // 0x35C
    const stage::NpcInfo* m_pInfo;                // 0x360
    const f32* m_pChairOffset;                    // 0x364 (the user data "chair" of the model)
    u32 m_Unknown368;                             // 0x368 (the user data "move" of the model, else 1)
    u8 m_BodyTexture;                             // 0x36C (255: none)
    bool m_Unknown36D;                            // 0x36D
    const char* m_pModelName;                     // 0x370 (a name of the derived classes in place of the one of the info)
    sead::IDelegate1<HumanModel*>* m_pUpdateCallback; // 0x374 (vf_0x08)
    sead::IDelegate1<HumanModel*>* m_pAnimCallback;   // 0x378 (PlayAnim)
    SvFgName m_Item37C;                           // 0x37C
    SvFgName m_Item380;                           // 0x380

private:
    // the common part of the setup functions (the old model is finalized, the new one built from loader)
    DECOMP_ALWAYS_INLINE bool SetupModelImpl(g3d::ResourceLoader* loader, u16 modelIndex, u32 bufferOption,
                                             ssys::ma::Allocator* allocator);
};
ASSERT_OFFSET(NpcModelBase, m_ResourceLoader, 0x15C);
ASSERT_OFFSET(NpcModelBase, m_LoadState, 0x27C);
ASSERT_OFFSET(NpcModelBase, m_RootCtrl, 0x284);
ASSERT_OFFSET(NpcModelBase, m_FaceCtrl, 0x2B0);
ASSERT_OFFSET(NpcModelBase, m_Scale, 0x344);
ASSERT_OFFSET(NpcModelBase, m_pAnimCallback, 0x378);
ASSERT_SIZE(NpcModelBase, 0x384);

// the render priorities of the materials of the villagers (game data; name is ours)
extern NpcModelBase::MeshPriorityTable g_NpcMeshPriorities;
// the model name of a villager id (the five characters of its entry in g_UnknownA84AEC; name is ours)
const char* GetVillagerModelName(const u16* villagerId); // 0x0071C3DC
// the animation sits on a chair (CHAIR_SITDOWN_WAIT_1, CHAIR_SLEEP_WAIT_1; name is ours)
bool IsChairWaitAnim(u32 id); // 0x006E58FC
