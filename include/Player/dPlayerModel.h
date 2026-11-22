#pragma once

#include "decomp.h"
#include "Bank/dBank.h"
#include "Bank/dBankTableBase.h"
#include "Exchange/dExchangeBankKeyword.h"
#include "Human/dHumanModel.h"
#include "Sv/dSvPlayer.h"
#include "nn/math/math_MTX34.h"

class HumanFaceAnimControl;
class HumanResTextureAnimBank;
class PlayerAcceBank;
class PlayerBottomsBank;
class PlayerCapBank;
class PlayerFaceBank;
class PlayerHeadBank;
class PlayerLegsBank;
class PlayerShoesBank;
class PlayerTopsBank;
class ToolBank;
struct SvPlayer;
namespace nw {
namespace gfx {
class CalculatedTransform;
namespace res {
struct ResModelData;
} // namespace res
} // namespace gfx
} // namespace nw

// The model of a player (class name from the RTTI; the layout from the constructor, all other names are ours): the body
// model of a shared file with the parts of the appearance and the outfit from the bank tables of BsPlayerMgr (face
// textures, head with the Mii, tops, bottoms, legs, shoes, accessory, cap and the tool in the right hand, eye and mouth
// texture animations). The parts are exchanged when the outfit changes (ExchangeParts); the tool has its own animation in
// slot 2 and follows speed, frame and play mode of slot 0. On the stage of the night disco of Club LOL the head nods with
// a rhythm animation in slot 3 to the beat of the DJ.
// RTTI 11PlayerModel @ 0x008CB31C
// vtable 0x008ECC90 (vptr 0x008ECC98), offset_to_top 0, 11 entries
class PlayerModel : public ::HumanModel
{
public:
    // m_Flags
    enum Flag
    {
        FLAG_IS_MALE = 1 << 0,       // (the value of LoadAppearance)
        FLAG_SETUP = 1 << 1,         // the banks are set up (Setup), the parts load
        FLAG_LOADED = 1 << 2,        // the model is built and the parts are there
        FLAG_ANIM_UPPER_BODY = 1 << 3, // the body animation plays in slot 1 (only the upper body, PlayBodyAnim)
    };
    // the animation slots of the tool and of the rhythm animation of the head
    static const u32 ANIM_SLOT_TOOL = 2;
    static const u32 ANIM_SLOT_HEAD = 3;

    PlayerModel(); // 0x001D38F8
    // PlayAnimWithTool with duration also for the tool (changed is not set)
    bool PlayAnim(u16 id, u32 playMode, u32 flags, f32 frame, f32 speed, f32 duration, bool* changed,
                  u8 morphType) override; // 0x001D3428 slot 0x00
    // the frames of the animations of the model, the head bank and the tool
    void vf_0x08() override; // 0x001D2C8C slot 0x08
    // Update(true, true)
    void vf_0x0C() override; // 0x001AC87C slot 0x0C
    // the model is updated, then the parts follow their bones
    void vf_0x10() override; // 0x001D34A0 slot 0x10
    // Draw(true, false)
    void vf_0x14() override; // 0x001D2D70 slot 0x14
    HumanFaceAnimControl* GetFaceCtrl() override; // 0x0027231C slot 0x18
    virtual ~PlayerModel(); // 0x001D3A24 slot 0x1C
    // 0x001D3A14 slot 0x20 (deleting dtor)
    // the world matrix of the model and of the clothes (on the curve of the world for the animations of Func6E5880)
    void SetMtx(const nn::math::MTX34* mtx) override; // 0x001CECB4 slot 0x24
    bool vf_0x28() override; // 0x0070F178 slot 0x28

    // the animations of the model are evaluated (isEvaluate), the tool turns, the face ends its fallback animation (force)
    // and the head turns
    void Update(bool isEvaluate, bool force); // 0x001CE97C
    // the scale goes a frames-th of the way to target (0 frames: at once)
    void ChangeScale(f32 target, s32 frames); // 0x001CEA8C
    void SetScale(f32 scale); // 0x001D3454
    // the body animation (PlayBodyAnim; changed is local), then the animation of the tool of the held item over
    // toolDuration (not for SIGNBOARD_WAIT_1 with a tool of item::Kind 20) and, when the body animation changed, the
    // animation of the tool bank with toolMorphFrames
    bool PlayAnimWithTool(u16 id, u32 playMode, u8 flags, f32 frame, f32 speed, f32 duration, f32 toolDuration,
                          f32 toolMorphFrames, u8 morphType); // 0x001CEB04
    // CHAIR_SITDOWN_WAIT_1 with PlayAnimWithTool
    bool PlayChairSitAnim(f32 duration, f32 toolDuration, f32 toolMorphFrames); // 0x001D2C50
    // the body animation: in slot 1 when the animation and the current one allow it (flags of the animation table),
    // otherwise in slot 0; the Mii head takes the expression of the animation
    bool PlayBodyAnim(u16 id, u32 playMode, u8 flags, f32 frame, f32 speed, f32 duration, bool* changed,
                      u8 morphType); // 0x001CFC90
    // the animation of the tool of item in slot 2 (armsOnly: all arms of the animation; force: also when the tool is
    // hidden; restart: also when it plays already; clearRightArm: the right arm is not counted as taken; stopArmAnim: the
    // arm animation ends first); false: no animation
    bool PlayToolAnim(const SvFgName* item, bool armsOnly, bool force, f32 duration, bool restart, bool clearRightArm,
                      bool stopArmAnim); // 0x001CFF80
    void StopToolAnim(f32 duration); // 0x001D0AD8
    // an animation of the tool bank itself (see ToolBank::PlayAnim)
    bool PlayToolBankAnim(u16 id, s32 playMode, f32 frame, f32 speed, f32 morphFrames); // 0x001CF478
    // the speed / the frame / the play mode of slot idx (slot 0 also for the tool when its kind entry has the flag)
    void SetAnimSpeed(u32 idx, f32 speed); // 0x001CF3C0
    void SetAnimFrame(u32 idx, f32 frame); // 0x001CFBCC
    void SetAnimPlayMode(s32 playMode, u32 idx); // 0x001D090C
    // the scale of the tool (m_ToolScale * m_ToolScaleFactor)
    void SetToolScale(f32 scale); // 0x001CFEF4
    void SetToolScaleFactor(f32 factor); // 0x001D0B04
    // the position of the bone "eff" of the tool in the curved world
    bool GetToolEffectPosition(nn::math::VEC3* out); // 0x001D0438
    // position in the space of the tool model in the curved world
    bool TransformByTool(nn::math::VEC3* out, const nn::math::VEC3* position); // 0x001D04B8

    // the parts of the player load: Setup once, then the face textures and ExchangeParts; true when all are there (the
    // model is built and slot 0x10 called once)
    bool Load(const SvPlayer* player, bool isToolShown, bool isFaceB, bool isBothFaces, u32 playerNo); // 0x001CF134
    bool LoadAppearance(const SvPlayerAppearance* appearance, bool isMale, bool isToolShown, bool isFaceB,
                        bool isBothFaces, u32 playerNo); // 0x001CF17C
    // the banks are handed out and set up for appearance, the model is built (always true)
    bool Setup(const SvPlayerAppearance* appearance, bool isMale, bool isFaceB, bool isBothFaces,
               u32 playerNo); // 0x001CF518
    // the body model of the shared file with its animations
    bool Build(); // 0x001D0298
    // the parts are exchanged for the outfit: cap, head and accessory together (loaded first, then shown one per frame),
    // then tool, bottoms, tops, legs and shoes; true: all are there
    bool ExchangeParts(); // 0x001D0B90
    // the colors of skin (the tan or the Mii with the Mii Mask), hair and eyes on the parts
    void SetSkinColors(); // 0x001D0654
    // the parts are drawn this frame (the bottoms only under tops of Func76A68C or without tops, cap and accessory not with
    // the Mii Mask); isSkipExchange: no ExchangeParts (false then)
    bool Draw(bool isDrawTool, bool isSkipExchange); // 0x001D2D7C
    // the banks are given back (true: all)
    bool Unload(); // 0x001D3174
    // the brightness of all parts / a frames-th of the way to target (the result: the value of the tool)
    void SetBrightness(f32 brightness); // 0x001D228C
    f32 FadeBrightness(f32 target, s32 frames); // 0x001D264C
    // the outfit / the hair of the head keyword (from the game object of the player)
    void SetOutfit(const SvPlayerOutfit* outfit); // 0x001D3884
    void SetHair(u8 style, s32 color); // 0x001D3448
    // the base bone of the animation is turned by the pitch of the model (m_Rotation.x); m_BaseMtx keeps it unturned
    void ApplyBodyPitch(); // 0x001D2B74
    // the rhythm animation of the head starts or ends with animation id (only on the stage CLUB444_DJKK) / its frame
    // follows the beat of the DJ
    void UpdateRhythmHead(u16 id); // 0x001D053C
    void UpdateRhythmHeadFrame(); // 0x001D09B0

    // the colors of the materials "m", "h" (skin, hair), "e" (skin, eyes), "cloth" and "legs" of model by flags (bits in
    // this order)
    static void SetModelColors(const nw::gfx::res::ResModelData* model, s32 hairColor, s32 eyeColor,
                               const ExchangeBankKeywordUnknown3* skinColor, u32 flags); // 0x001CEEF0

    // the tool below this scale is hidden (no tool animation; .data)
    static f32 s_ToolScaleMin;
    // the number of the rhythm animations of the head (a constant the original reads from memory)
    static const u32 s_RhythmHeadAnimNum;

private:
    // (inline) the banks of the handles
    Bank<11059u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >* GetBodyBank();
    PlayerFaceBank* GetFaceBank(BankHandle* handle);
    PlayerHeadBank* GetHeadBank();
    PlayerTopsBank* GetTopsBank();
    PlayerBottomsBank* GetBottomsBank();
    PlayerLegsBank* GetLegsBank();
    PlayerShoesBank* GetShoesBank();
    PlayerAcceBank* GetAcceBank();
    PlayerCapBank* GetCapBank();
    ToolBank* GetToolBank();
    HumanResTextureAnimBank* GetEyeBank(BankHandle* handle);
    HumanResTextureAnimBank* GetMouthBank(BankHandle* handle);

public:
    BankHandle m_BodyHandle;                       // 0x144
    BankHandle m_FaceHandleA;                      // 0x14C
    BankHandle m_FaceHandleB;                      // 0x154
    BankHandle m_HeadHandle;                       // 0x15C
    BankHandle m_TopsHandle;                       // 0x164
    BankHandle m_BottomsHandle;                    // 0x16C
    BankHandle m_LegsHandle;                       // 0x174
    BankHandle m_ShoesHandle;                      // 0x17C
    BankHandle m_AcceHandle;                       // 0x184
    BankHandle m_CapHandle;                        // 0x18C
    BankHandle m_ToolHandle;                       // 0x194
    BankHandle m_EyeHandleA;                       // 0x19C
    BankHandle m_EyeHandleB;                       // 0x1A4
    BankHandle m_MouthHandleA;                     // 0x1AC
    BankHandle m_MouthHandleB;                     // 0x1B4
    u8 m_ToolArmFlags;                             // 0x1BC HumanModel::ArmFlag of the tool animation
    bool m_IsRhythmHeadPlaying;                    // 0x1BD
    u8 m_RhythmHeadIndex;                          // 0x1BE (0..s_RhythmHeadAnimNum-1)
    s32 m_Tan;                                     // 0x1C0 (Func713798 of the appearance)
    const SvPlayerAppearance* m_pAppearance;       // 0x1C4
    u8 m_Flags;                                    // 0x1C8 Flag
    nw::gfx::CalculatedTransform* m_pBaseTransform; // 0x1CC the local transform of the bone base
    nn::math::MTX34 m_BaseMtx;                     // 0x1D0 its matrix without the pitch (ApplyBodyPitch)
    SvPlayerOutfit m_Outfit;                       // 0x200
    PlayerHeadBankKeyword m_HeadKeyword;           // 0x224 (item and skin color: ExchangeParts)
    f32 m_Scale;                                   // 0x238
    f32 m_ToolScale;                               // 0x23C (LoadAppearance: 1 with isToolShown, else 0)
    f32 m_ToolScaleFactor;                         // 0x240
    RootCtrl m_RootCtrl;                           // 0x244
};
ASSERT_OFFSET(PlayerModel, m_BodyHandle, 0x144);
ASSERT_OFFSET(PlayerModel, m_ToolArmFlags, 0x1BC);
ASSERT_OFFSET(PlayerModel, m_BaseMtx, 0x1D0);
ASSERT_OFFSET(PlayerModel, m_Outfit, 0x200);
ASSERT_OFFSET(PlayerModel, m_HeadKeyword, 0x224);
ASSERT_OFFSET(PlayerModel, m_RootCtrl, 0x244);
ASSERT_SIZE(PlayerModel, 0x270);

// (game code that falls into the functions of PlayerModel: the PlayerModel at +0x1B4 of a game object of the player)
bool Func1CF470(void* owner, u16 id, s32 playMode, f32 frame, f32 speed, f32 morphFrames); // 0x001CF470
// the brightness of the player model at +0x1B4 by Func1AC174 of the position at +0x14 of the game object
void Func1D2270(void* owner); // 0x001D2270
