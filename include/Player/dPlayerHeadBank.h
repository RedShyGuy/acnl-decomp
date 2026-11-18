#pragma once

#include "decomp.h"
#include "Exchange/dExchangeModelBank.h"
#include "Human/dHumanFaceAnimControl.h"
#include "cfl/MiiModelSimple.h"
#include "g3d/dResourceLoader.h"

class HumanResTextureAnimBank;
class PlayerFaceBank;
struct SvPlayerAppearance;

namespace sead {
template <typename T>
class BufferedSafeStringBase;
} // namespace sead

// The bank of the heads of the player (class name from the RTTI, layout size after libgarden; the members and the
// non-virtual functions are ours): per slot the head model of the hair style ("Player/head_mdl/<boy|girl><style>a/b",
// a bald model for some headwear) with the face textures of a PlayerFaceBank and the material animations of the face,
// or, for the item 0x2000, the Mii of the player (CFL).
// RTTI 14PlayerHeadBank @ 0x008CBD58
// vtable 0x008F0630 (vptr 0x008F0638), offset_to_top 0, 11 entries
// vtable 0x008F0664 (vptr 0x008F066C), offset_to_top -636, 3 entries
class PlayerHeadBank : public ::ExchangeModelBank<25891u, BankVramConfig<1800u, 4u, 6600u, 4u, false>, PlayerHeadBankKeyword,
                                                  g3d::ResourceLoader, 1u, false>
{
public:
    typedef PlayerHeadBankKeyword Keyword;

    // the Mii of a slot
    struct HeadMii
    {
        // (copies for the arrays of the bank)
        DECOMP_EMIT_COPY HeadMii() {} // 0x002723C4
        DECOMP_EMIT_COPY ~HeadMii() {} // 0x002723D8

        bool m_IsUsed;                // 0x0 the slot shows the Mii
        cfl::MiiModelSimple m_Model;  // 0x4
    };
    ASSERT_SIZE(HeadMii, 0x94);

    // the expression of the Mii from an animation of the face: the digit at index 2 of the name of the face texture
    // of the frame selects it (the material animation anim1 of the animation table)
    struct MiiExpressionAnim
    {
        // (the frame goes on; at the end frame it stays there)
        void Update(); // 0x00271708

        f32 m_Frame;      // 0x0
        s32 m_Expression; // 0x4 (CFL expression)
        f32 m_EndFrame;   // 0x8 (0: no end)
        u16 m_AnimId;     // 0xC HumanAnimName
    };
    ASSERT_SIZE(MiiExpressionAnim, 0x10);

    PlayerHeadBank(); // 0x00272404
    virtual ~PlayerHeadBank(); // 0x00272798 slot 0x00

    virtual bool LoadResource(const Keyword& keyword, u32 slot) override; // 0x002718F8 slot 0x10
    virtual s32 LoadResourceSplit(const Keyword& keyword, u32 slot) override; // 0x00271D84 slot 0x14
    virtual bool Initialize(const Keyword& keyword, u32 slot, bool isReady) override; // 0x002719F4 slot 0x18
    virtual bool Finalize(const Keyword& keyword, u32 slot) override; // 0x00271D04 slot 0x1C

    // the shown slot is drawn (the Mii; the model goes into the model list of the object 0x00951378)
    void RequestDraw(); // 0x00271584
    // the expression animation of the Mii starts with the animation id
    void StartMiiExpressionAnim(u16 id, f32 frame); // 0x00271608
    // the face and the expression of the Mii in the frame
    void Update(); // 0x00271680
    // the colors of the shown model: the hair and eye color of its keyword and colors (false: no player or no model)
    bool SetFaceColors(const ExchangeBankKeywordUnknown3* colors); // 0x0027184C
    // the path of the head model of the hair style for the headwear item (false: no head model)
    bool GetHeadModelPath(sead::BufferedSafeStringBase<char>* path, u8 hairStyle, SvFgName item); // 0x00271EC8
    // the shown model (the Mii turned) at the bone of parent
    void SetBoneMatrix(g3d::SkeletalModel* parent, u32 bone); // 0x00272234
    // the brightness of the shown model (index: the model resource) or of the Mii
    bool SetBrightness(u32 index, f32 brightness); // 0x002722C8
    // the face control of the shown slot (NULL without a model)
    HumanFaceAnimControl* GetFaceAnimControl(); // 0x00272388
    void SetBanks(PlayerFaceBank* faceBank0, PlayerFaceBank* faceBank1, const SvPlayerAppearance* player,
                  HumanResTextureAnimBank* textureAnimBankA0, HumanResTextureAnimBank* textureAnimBankA1,
                  HumanResTextureAnimBank* textureAnimBankB0, HumanResTextureAnimBank* textureAnimBankB1); // 0x002723EC
    // (inline, copy emitted with the face control) the fallback animation of the face of the shown slot ends
    DECOMP_EMIT_COPY void EndFaceFallback(bool force) // 0x0031DEC0
    {
        m_FaceAnimControls[GetCurrentSlot()].EndFallbackB(true, force);
    }

    // (inline) no banks, no player
    void ClearBanks()
    {
        m_pPlayer = NULL;
        m_pFaceBanks[0] = NULL;
        m_pFaceBanks[1] = NULL;
        m_pTextureAnimBanks[0][0] = NULL;
        m_pTextureAnimBanks[1][0] = NULL;
        m_Miis[0].m_IsUsed = false;
        m_pTextureAnimBanks[0][1] = NULL;
        m_pTextureAnimBanks[1][1] = NULL;
        m_Miis[1].m_IsUsed = false;
    }

    PlayerFaceBank* m_pFaceBanks[2];                       // 0x29C (keyword.m_Unknown10: the second one first)
    const SvPlayerAppearance* m_pPlayer;                    // 0x2A4 the player data after the checksum (its Mii)
    HeadMii m_Miis[2];                                     // 0x2A8
    HumanResTextureAnimBank* m_pTextureAnimBanks[2][2];    // 0x3D0 [face slot][bank slot]
    HumanFaceAnimControl m_FaceAnimControls[2];            // 0x3E0
    MiiExpressionAnim m_MiiExpressionAnim;                 // 0x508
};
ASSERT_SIZE(PlayerHeadBank, 0x518);
