#pragma once

#include "decomp.h"
#include "Bank/dBank.h"
#include "Bank/dBankTable.h"
#include "Human/dHumanResTextureAnimBank.h"
#include "Other/dBase.h"
#include "Other/dToolBank.h"
#include "Player/dPlayerAcceBank.h"
#include "Player/dPlayerBottomsBank.h"
#include "Player/dPlayerCapBank.h"
#include "Player/dPlayerFaceBank.h"
#include "Player/dPlayerHeadBank.h"
#include "Player/dPlayerLegsBank.h"
#include "Player/dPlayerShoesBank.h"
#include "Player/dPlayerTopsBank.h"
#include "Utl/dUtlBase.h"

namespace g3d {
class SkeletalModel;
} // namespace g3d

namespace sead {
class ExpHeap;
} // namespace sead

// The manager of the banks of the player parts (class name from the RTTI, the bank tables after libgarden; the other
// members and the functions are ours): the bank tables live in a heap "PlayerBankHeap" of the stage heap, made in
// StartInitialize with as many banks per table as the stage needs, and the banks of the shown parts add their models to
// the model lists, which Draw adds to the scenes.
// RTTI 11BsPlayerMgr @ 0x008CB284
// vtable 0x008EC94C (vptr 0x008EC954), offset_to_top 0, 22 entries
class BsPlayerMgr : public ::UtlBase<Base>
{
public:
    // the tables of the banks of the player parts (layout and names after libgarden)
    class BankTables
    {
    public:
        // (inline: the constructor of BsPlayerMgr builds the tables)
        DECOMP_ALWAYS_INLINE BankTables() {}

        // num banks per table (14 for the tables with two banks per player) with their heaps in heap (false: a table got
        // fewer than it needs or a heap is missing)
        DECOMP_NOINLINE bool Initialize(sead::Heap* heap, u32 num); // 0x0029EFAC
        // the heaps of all banks are destroyed, the tables from the last to the first (false: a bank is handed out still,
        // the tables before it are left)
        DECOMP_NOINLINE bool Finalize(); // 0x002A0F78

        BankTable<Bank<11059u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >, 7u> m_Body; // 0x00000
        BankTable<PlayerFaceBank, 14u> m_Face;                                                // 0x0009C
        BankTable<PlayerHeadBank, 7u> m_Head;                                                 // 0x01034
        BankTable<PlayerBottomsBank, 7u> m_Bottoms;                                           // 0x033EC
        BankTable<PlayerTopsBank, 7u> m_Tops;                                                 // 0x054B0
        BankTable<PlayerLegsBank, 7u> m_Legs;                                                 // 0x0768C
        BankTable<PlayerShoesBank, 7u> m_Shoes;                                               // 0x09868
        BankTable<PlayerAcceBank, 7u> m_Acce;                                                 // 0x0B910
        BankTable<PlayerCapBank, 7u> m_Cap;                                                   // 0x0CAA0
        BankTable<ToolBank, 7u> m_Tool;                                                       // 0x0ECD0
        BankTable<HumanResTextureAnimBank, 14u> m_EyeTextureAnim;                             // 0x12978
        BankTable<HumanResTextureAnimBank, 14u> m_MouthTextureAnim;                           // 0x12AA0
    };
    ASSERT_SIZE(BankTables, 0x12BC8);

    // the models of the shown parts of one kind for this frame (at most 7)
    struct ModelList
    {
        ModelList()
        {
            m_Count = 0;
            for (u32 i = 0; i < 7; i++) {
                m_pModels[i] = NULL;
            }
        }

        // (inline) model is drawn this frame
        void Add(g3d::SkeletalModel* model)
        {
            if (m_Count < 7) {
                m_pModels[m_Count] = model;
                m_Count++;
            }
        }
        // the models to the scenes of sceneMask (bit per scene 0..2); then the list is empty
        void AddToScenes(u32 sceneMask); // 0x007C88B4

        g3d::SkeletalModel* m_pModels[7]; // 0x00
        u32 m_Count;                      // 0x1C
    };
    ASSERT_SIZE(ModelList, 0x20);

    // the model lists (the banks of the parts add to them)
    enum ModelListId
    {
        MODEL_LIST_HEAD = 0,
        MODEL_LIST_CAP = 1,
        MODEL_LIST_ACCE = 2,
        MODEL_LIST_TOPS = 3,
        MODEL_LIST_BOTTOMS = 4,
        MODEL_LIST_SHOES = 5,
        MODEL_LIST_LEGS = 6,
        MODEL_LIST_7 = 7, // (no bank adds to it in the known code)
        MODEL_LIST_NUM = 8,
    };

    BsPlayerMgr(); // 0x001C4C24
    virtual ~BsPlayerMgr(); // 0x001C50E0 slot 0x00
    // 0x001C5038 slot 0x04 (deleting dtor)
    virtual oml::framework::Result Calc() override; // 0x001C4BF8 slot 0x24
    virtual oml::framework::Result Draw() override; // 0x001C4B40 slot 0x30
    bool StartInitialize() override; // 0x001C4748 slot 0x40
    oml::framework::Result UpdateInitialize() override; // 0x001C4740 slot 0x44
    bool EndInitialize() override; // 0x001C472C slot 0x48
    bool StartFinalize() override; // 0x001C481C slot 0x4C
    oml::framework::Result UpdateFinalize() override; // 0x001C4800 slot 0x50
    bool EndFinalize() override; // 0x001C47D0 slot 0x54

    // the heap "PlayerBankHeap" with the banks (false: not on the title screen, or there is one already)
    DECOMP_NOINLINE bool CreateBankHeap(); // 0x001C4530
    // places the other players of a multiplayer game
    void PlaceOtherPlayers(); // 0x001C4834
    // the bank tables of the instance
    static BankTables* GetBankTables(); // 0x001C4940
    // (coming from the tour desk to a tour) the players get the item of the tour in their hands or on their head
    // (for the insect tour kind 8 also a rental shovel into the pockets)
    DECOMP_NOINLINE void GiveTourItems(); // 0x001C4954

    // (inline) model is drawn this frame in list
    static void AddModel(u32 list, g3d::SkeletalModel* model)
    {
        if (s_pInstance != NULL) {
            s_pInstance->m_ModelLists[list].Add(model);
        }
    }

    // the instance between EndInitialize and StartFinalize
    static BsPlayerMgr* s_pInstance;

    sead::ExpHeap* m_pHeap;                    // 0x14 "PlayerBankHeap"
    BankTables m_BankTables;                   // 0x18
    ModelList m_ModelLists[MODEL_LIST_NUM];    // 0x12BE0
};
ASSERT_SIZE(BsPlayerMgr, 0x12CE0);
ASSERT_OFFSET(BsPlayerMgr, m_ModelLists, 0x12BE0);
