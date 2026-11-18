#include "Bs/dBsPlayerMgr.h"

#include <cstring>
#include "Bs/dBsPlayerMgrExtern.h"
#include "Bs/dBsSvMgr.h"
#include "Other/dHeapInit.h"
#include "Other/dTourData.h"
#include "Player/dPlayerApi.h"
#include "Player/dPlayerState.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvPlayer.h"
#include "field/dFdInfo.h"
#include "field/dFieldTypes.h"
#include "field/field_Extern.h"
#include "g3d/dSkeletalModel.h"
#include "netgame/dGameFunctions.h"
#include "netgame/dGameGlobals.h"
#include "netgame/dNetGameMgr.h"
#include "netgame/netgame_Api.h"
#include "script/dScriptExtern.h"
#include "sead/seadExpHeap.h"
#include "sead/seadSafeStringBase.h"
#include "stage/stage_Api.h"
#include "stage/stage_Extern.h"


// 0x00951378 (name is ours)
BsPlayerMgr* BsPlayerMgr::s_pInstance;

namespace {
// the heap of one bank of each table in "PlayerBankHeap" (the slot heaps with their headers and the VRAM heap objects)
const u32 BODY_BANK_HEAP_SIZE = 11136;
const u32 FACE_BANK_HEAP_SIZE = 40608;
const u32 HEAD_BANK_HEAP_SIZE = 55648;
const u32 BOTTOMS_BANK_HEAP_SIZE = 40096;
const u32 TOPS_BANK_HEAP_SIZE = 70208;
const u32 LEGS_BANK_HEAP_SIZE = 40192;
const u32 SHOES_BANK_HEAP_SIZE = 39840;
const u32 ACCE_BANK_HEAP_SIZE = 31200;
const u32 CAP_BANK_HEAP_SIZE = 67360;
const u32 TOOL_BANK_HEAP_SIZE = 53504;
const u32 TEXTURE_ANIM_BANK_HEAP_SIZE = 3136;

// the players on a stage that need banks: 4, one on the model house and mini game stages and on the move to Welcome
// amiibo, 7 on the exhibition stages
const u32 BANK_NUM_DEFAULT = 4;
const u32 BANK_NUM_EXHIBITION = 7;
const u32 BANK_NUM_SINGLE = 1;
} // namespace

// 0x001C4530 (name is ours)
bool BsPlayerMgr::CreateBankHeap()
{
    const stage::Name name = stage::GetCurrent();
    if (name == stage::NAME_TITLE || m_pHeap != NULL) {
        return false;
    }
    u32 num = BANK_NUM_DEFAULT;
    if (stage::IsKind(stage::KIND_MODEL_HOUSE, stage::GetCurrent())) {
        num = BANK_NUM_SINGLE;
    } else if (stage::IsKind(stage::KIND_EXHIBITION, stage::GetCurrent())) {
        num = BANK_NUM_EXHIBITION;
    } else if (stage::IsKind(stage::KIND_MINI_GAME, stage::GetCurrent()) || name == stage::NAME_GARDEN_MANUAL_IMPORT) {
        num = BANK_NUM_SINGLE;
    }
    const u32 num2 = num * 2;
    u32 size = BankTableBase::Min(num, 7) * BODY_BANK_HEAP_SIZE;
    size += BankTableBase::Min(num2, 14) * FACE_BANK_HEAP_SIZE;
    size += BankTableBase::Min(num, 7) * HEAD_BANK_HEAP_SIZE;
    size += BankTableBase::Min(num, 7) * BOTTOMS_BANK_HEAP_SIZE;
    size += BankTableBase::Min(num, 7) * TOPS_BANK_HEAP_SIZE;
    size += BankTableBase::Min(num, 7) * LEGS_BANK_HEAP_SIZE;
    size += BankTableBase::Min(num, 7) * SHOES_BANK_HEAP_SIZE;
    size += BankTableBase::Min(num, 7) * ACCE_BANK_HEAP_SIZE;
    size += BankTableBase::Min(num, 7) * CAP_BANK_HEAP_SIZE;
    size += BankTableBase::Min(num, 7) * TOOL_BANK_HEAP_SIZE;
    size += BankTableBase::Min(num2, 14) * TEXTURE_ANIM_BANK_HEAP_SIZE;
    size += BankTableBase::Min(num2, 14) * TEXTURE_ANIM_BANK_HEAP_SIZE;
    m_pHeap = sead::ExpHeap::create(size, sead::SafeStringBase<char>("PlayerBankHeap"), HeapInit::s_pStageHeap,
                                    sead::Heap::HEAP_DIRECTION_FORWARD, false);
    return m_BankTables.Initialize(m_pHeap, num);
}

// 0x001C472C slot 0x48
bool BsPlayerMgr::EndInitialize()
{
    s_pInstance = this;
    return true;
}

// 0x001C4740 slot 0x44
oml::framework::Result BsPlayerMgr::UpdateInitialize()
{
    return oml::framework::RESULT_SUCCESS;
}

// 0x001C4748 slot 0x40
bool BsPlayerMgr::StartInitialize()
{
    if (stage::IsKind(stage::KIND_KOTOBUKI, stage::GetCurrent())) {
        std::memset(Func5C1A20(), 0xFF, 4);
    }
    if (stage::GetPrevious() == stage::NAME_TOUR_DESK || stage::GetPrevious() == stage::NAME_PREPARE_TOUR) {
        if (stage::IsKind(stage::KIND_TOUR, stage::GetCurrent())) {
            GiveTourItems();
        }
    }
    Func5C72A8();
    CreateBankHeap();
    return true;
}

// 0x001C47D0 slot 0x54
bool BsPlayerMgr::EndFinalize()
{
    if (m_pHeap != NULL) {
        m_pHeap->destroy();
        m_pHeap = NULL;
    }
    return true;
}

// 0x001C4800 slot 0x50
oml::framework::Result BsPlayerMgr::UpdateFinalize()
{
    return m_BankTables.Finalize() ? oml::framework::RESULT_SUCCESS : oml::framework::RESULT_RETRY;
}

// 0x001C481C slot 0x4C
bool BsPlayerMgr::StartFinalize()
{
    s_pInstance = NULL;
    return true;
}

// 0x001C4834 (name is ours)
void BsPlayerMgr::PlaceOtherPlayers()
{
    if (!netgame::IsMultiplePlayer()) {
        return;
    }
    for (u32 i = 0; i < 4; i++) {
        const netgame::PlayerNo player = static_cast<netgame::PlayerNo>(i & 0xFF);
        if (netgame::GetMyPlayerNo() == player || !netgame::ExistsPlayer(player)) {
            continue;
        }
        const PlayerState* state = PlayerGetState(player);
        nn::math::VEC3 position(static_cast<f32>(state->positionX) * 0.25f, 0.0f,
                                static_cast<f32>(state->positionZ) * 0.25f);
        position.y = FdInfo::GetHeight(position, FIELD_NAME_CURRENT);
        math::Angle16Vec3 rotation;
        rotation.x.value = 0;
        rotation.y.value = state->rotation;
        rotation.z.value = 0;
        PlayerSetStart(player, &position, &rotation, PLAYER_ACTION_HIDE, 1);
    }
}

// 0x001C4940 (name is ours)
BsPlayerMgr::BankTables* BsPlayerMgr::GetBankTables()
{
    return &s_pInstance->m_BankTables;
}

// 0x001C4954 (name is ours)
void BsPlayerMgr::GiveTourItems()
{
    TourData* tour = TourData::s_pInstance;
    if (tour != NULL) {
        const bool isShovel = GetTourType(tour->m_Tour) == 3 && Func76FB7C(&tour->m_Seed) == 8;
        SvFgName item(*tour->Func76F580());
        for (u32 i = 0; i < 4; i++) {
            SvPlayer* player = SvGetNetPlayer(static_cast<netgame::PlayerNo>(i & 0xFF));
            if (player == NULL) {
                continue;
            }
            SvPlayerOutfit* outfit = &player->playerAppearance.playerOutfit;
            if (Func769644(Func71B3B8(outfit))) {
                Func27B1EC(outfit);
            }
            if (Func769644(Func71B394(outfit))) {
                outfit->heldItem = SvFgName::s_Empty;
            }
            const u32 kind = Func76BB44(&item);
            if (kind == 3) {
                SvFgName worn(*Func71B3B8(outfit));
                if (worn.GetKind() == 8) {
                    if (item.GetKind() != 8) {
                        Func27B1EC(outfit);
                        Func2BFDE0(&player->inventory, &worn, 0, 1);
                        Func27B1F4(outfit, &item);
                    }
                } else {
                    Func27B1F4(outfit, &item);
                }
            } else if (kind == 7) {
                outfit->heldItem = item;
            } else {
                outfit->heldItem = SvFgName::s_Empty;
            }
            if (isShovel) {
                SvFgName shovel(static_cast<SvFgName::ID>(SvFgName::NAME_RENTAL_SHOVEL));
                Func2BFDE0(&player->inventory, &shovel, 0, 1);
            }
        }
    }
    Func583528();
}

// 0x001C4B40 slot 0x30
oml::framework::Result BsPlayerMgr::Draw()
{
    if (s_pInstance != NULL) {
        s_pInstance->m_ModelLists[MODEL_LIST_ACCE].AddToScenes(1);
        s_pInstance->m_ModelLists[MODEL_LIST_7].AddToScenes(1);
        s_pInstance->m_ModelLists[MODEL_LIST_CAP].AddToScenes(1);
        s_pInstance->m_ModelLists[MODEL_LIST_HEAD].AddToScenes(1);
        s_pInstance->m_ModelLists[MODEL_LIST_LEGS].AddToScenes(1);
        s_pInstance->m_ModelLists[MODEL_LIST_TOPS].AddToScenes(1);
        s_pInstance->m_ModelLists[MODEL_LIST_BOTTOMS].AddToScenes(1);
        s_pInstance->m_ModelLists[MODEL_LIST_SHOES].AddToScenes(1);
    }
    return oml::framework::RESULT_SUCCESS;
}

// 0x001C4BF8 slot 0x24
oml::framework::Result BsPlayerMgr::Calc()
{
    if ((netgame::g_FrameCounter & 0x1FF) == 0) {
        Func5C78E8();
    }
    Func5C99E4(g_UnknownAAFD58);
    return oml::framework::RESULT_SUCCESS;
}

// 0x001C4C24 (name is ours)
BsPlayerMgr::BsPlayerMgr() : m_pHeap(NULL)
{
}

// 0x001C50E0 slot 0x00
// 0x001C5038 (deleting dtor)
BsPlayerMgr::~BsPlayerMgr()
{
    // (only the members and the bases are destroyed)
}

// 0x0029EFAC (name is ours)
bool BsPlayerMgr::BankTables::Initialize(sead::Heap* heap, u32 num)
{
    const u32 num2 = num * 2;
    bool isAll = true;
    isAll &= m_Body.Initialize(heap, num, "PlayerBodyBank");
    isAll &= m_Face.Initialize(heap, num2, "PlayerFaceBank");
    isAll &= m_Head.Initialize(heap, num, "PlayerHeadBank");
    isAll &= m_Bottoms.Initialize(heap, num, "PlayerBottomsBank");
    isAll &= m_Tops.Initialize(heap, num, "PlayerTopsBank");
    isAll &= m_Legs.Initialize(heap, num, "PlayerLegsBank");
    isAll &= m_Shoes.Initialize(heap, num, "PlayerShoesBank");
    isAll &= m_Acce.Initialize(heap, num, "PlayerAcceBank");
    isAll &= m_Cap.Initialize(heap, num, "PlayerCapBank");
    isAll &= m_Tool.Initialize(heap, num, "PlayerToolBank");
    isAll &= m_EyeTextureAnim.Initialize(heap, num2, "PlayerEyeResTextureAnimBank");
    isAll &= m_MouthTextureAnim.Initialize(heap, num2, "PlayerMouthResTextureAnimBank");
    return isAll;
}

// 0x002A0F78 (name is ours)
bool BsPlayerMgr::BankTables::Finalize()
{
    return m_MouthTextureAnim.FinalizeTable() && m_EyeTextureAnim.FinalizeTable() && m_Tool.FinalizeTable() &&
           m_Cap.FinalizeTable() && m_Acce.FinalizeTable() && m_Shoes.FinalizeTable() && m_Legs.FinalizeTable() &&
           m_Tops.FinalizeTable() && m_Bottoms.FinalizeTable() && m_Head.FinalizeTable() && m_Face.FinalizeTable() &&
           m_Body.FinalizeTable();
}

// 0x007C88B4 (name is ours)
void BsPlayerMgr::ModelList::AddToScenes(u32 sceneMask)
{
    for (u32 scene = 0; scene < 3; scene++) {
        if (((sceneMask >> scene) & 1) == 0) {
            continue;
        }
        for (u32 i = 0; i < m_Count; i++) {
            if (m_pModels[i] != NULL) {
                m_pModels[i]->AddToScene(scene);
            }
        }
    }
    for (u32 i = 0; i < 7; i++) {
        m_pModels[i] = NULL;
    }
    m_Count = 0;
}
