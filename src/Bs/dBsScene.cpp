#include "Bs/dBsScene.h"
#include "Audio/bgm/dBgmRequestMgr.h"
#include "Bs/dBsRoot.h"
#include "Bs/dBsSceneExtern.h"
#include "Bs/dBsSvMgr.h"
#include "Other/dBase.h"
#include "Other/dProcMgr.h"
#include "Other/dUlcdFramework.h"
#include "Sound/dSoundLabel.h"
#include "Sound/dSoundMgr.h"
#include "applet/dErrEulaMgr.h"
#include "netgame/dNetGameMgr.h"
#include "oml/framework/dProcessManager.h"
#include "stage/stage_Api.h"

// (no scene yet; the fades of the last change are kept)
// 0x009513CC
BsScene::ChangeRequest BsScene::s_ChangeRequest = {
    true,  false, false, FADE_TYPE_KEEP, FADE_TYPE_KEEP, FADE_TYPE_KEEP, FADE_TYPE_KEEP, false,
    NO_SCENE, NO_SCENE, NO_SCENE, 0, NULL, 0, 20, 20,
};
// 0x0094F484
u16 BsScene::s_LastRequested = NO_SCENE;

// 0x00607938 (name is ours)
BsScene::BsScene() : m_IsPaused(false), m_IsStarted(false)
{
    s_ChangeRequest.pScene = this;
    s_ChangeRequest.isFirstCalc = true;
    s_ChangeRequest.isShown = false;
}

// 0x00313AF4 slot 0x00
// 0x00607974 (deleting dtor)
BsScene::~BsScene()
{
    s_ChangeRequest.pScene = NULL;
}

// 0x006078B0 slot 0x08
bool BsScene::CanInitialize() const
{
    if (!Base::CanInitialize()) {
        return false;
    }
    oml::framework::ProcessManager::Get()->PauseChildren(m_Handle);
    oml::framework::ProcessManager::Get()->HideChildren(m_Handle);
    s_ChangeRequest.isShown = false;
    if (const_cast<BsScene*>(this)->vf_0x40() != oml::framework::RESULT_SUCCESS) {
        stage::ClearCurrent();
    }
    return true;
}

// 0x00607194 slot 0x10
void BsScene::HandleInitializationResult(oml::framework::Result result)
{
    Base::HandleInitializationResult(result);
}

// 0x00607924 slot 0x14
bool BsScene::CanFinalize() const
{
    return Base::CanFinalize();
}

// 0x00607198 slot 0x1C
void BsScene::HandleFinalizationResult(oml::framework::Result result)
{
    if (result == oml::framework::RESULT_SUCCESS) {
        s_ChangeRequest.isChanging = false;
        netgame::NetGameMgr::s_pInstance->ProcessEnd();
    }
    Base::HandleFinalizationResult(result);
}

// 0x006071DC slot 0x20
bool BsScene::CanCalc() const
{
    if (!Base::CanCalc()) {
        return false;
    }
    ChangeRequest& request = s_ChangeRequest;
    if (request.name != NO_SCENE) {
        // a change is requested: the fade out first, then the scene stops
        if (Func566AD0()) {
            SoundMgr::SSetFullStageChange();
            Func6A67E4(request.typeA, request.typeB, 0);
            Func6A66D8(request.fadeOutParam);
            request.isFading = true;
            return false;
        }
        if (!Func566B2C()) {
            return false;
        }
        request.isFading = true;
        netgame::NetGameMgr* net = netgame::NetGameMgr::s_pInstance;
        const u8 state = net->m_StageMgr.m_State;
        if (state != 0 && !applet::ErrEulaMgr::s_pInstance->m_FB0) {
            switch (state) {
            case 3:
                net->BuildPayloadD(net->GetMyPlayerNo());
                netgame::NetGameMgr::s_pInstance->m_StageMgr.m_State = 4;
                break;
            case 5:
                if (!net->m_StageMgr.unk_0x23) {
                    return false;
                }
                net->BuildPayloadE(net->GetMyPlayerNo());
                netgame::NetGameMgr::s_pInstance->m_StageMgr.m_State = 6;
                break;
            case 8:
                if (!net->m_StageMgr.AreAllPlayersConfirmed()) {
                    return false;
                }
                netgame::NetGameMgr::s_pInstance->BuildPayloadA();
                netgame::NetGameMgr::s_pInstance->m_StageMgr.m_State = 9;
                break;
            default:
                return false;
            }
        }
        if (stage::GetCurrent() == stage::NAME_DREAM &&
            audio::bgm::GetRequestMgr()->Has(audio::bgm::KIND_SHOP, STRM_BGM_IND_YUMEMI)) {
            return false;
        }
        const_cast<BsScene*>(this)->Stop();
        return false;
    }

    // the children of a new scene are started once the black wait is over
    const s32 handle = m_Handle;
    if (!m_IsStarted && oml::framework::ProcessManager::Get()->IsPausingChildren(handle)) {
        if (oml::framework::ProcessManager::Get()->HasUninitializedDescendant(this) == true) {
            return false;
        }
        if (m_IsPaused) {
            return false;
        }
        if (request.isShown) {
            if (!Func6CF2C0() && Func6CFA78()) {
                return false;
            }
            oml::framework::ProcessManager::Get()->ResumeChildren(handle);
            oml::framework::ProcessManager::Get()->ShowChildren(handle);
            const_cast<BsScene*>(this)->m_IsStarted = true;
            return false;
        }
        if (netgame::NetGameMgr::s_pInstance->m_StageMgr.m_State != 0) {
            return false;
        }
        if (!Func6CFA2C()) {
            return false;
        }
        request.isShown = true;
        switch (stage::GetCurrent()) {
        case stage::NAME_TITLE: {
            UlcdFramework* framework = UlcdFramework::GetInstance();
            if (framework != NULL) {
                framework->m_IsRendering3D = false;
            }
        }
            // fall through
        case stage::NAME_FORMAT:
            if (g_pUnknown94F4C8 != NULL) {
                g_pUnknown94F4C8[0xA10] = 0;
            }
            break;
        default:
            break;
        }
        return false;
    }

    // the fade in of the new scene
    if (request.isFading) {
        if (Func566B2C()) {
            Func6A67E4(request.faderA, request.faderB, 0);
            Func6A6660(request.fadeInParam);
            request.isFading = false;
        }
    } else if (Func566AD0()) {
        request.isFading = false;
    }
    request.isFirstCalc = false;
    return true;
}

// 0x006074F8 slot 0x28
void BsScene::HandleCalcResult(oml::framework::Result result)
{
    Base::HandleCalcResult(result);
}

// 0x00607898 slot 0x2C
bool BsScene::CanDraw() const
{
    return Base::CanDraw();
}

// 0x006078AC slot 0x34
void BsScene::ProcessDrawResult(oml::framework::Result result)
{
    oml::framework::Process::ProcessDrawResult(result);
}

// 0x0075E164 slot 0x3C (name is ours)
u32 BsScene::GetTypeMask()
{
    return TYPE_MASK_SCENE;
}

// 0x006074FC (name is ours)
void BsScene::RequestChange(u16 name, u8 typeA, u8 faderA, u8 typeB, u8 faderB, u32 arg)
{
    s_ChangeRequest.name = name;
    s_ChangeRequest.typeA = typeA;
    s_ChangeRequest.faderA = faderA;
    s_ChangeRequest.typeB = typeB;
    s_ChangeRequest.faderB = faderB;
    s_ChangeRequest.arg = arg;
    s_LastRequested = name;
}

// 0x0060753C (name is ours)
void BsScene::RequestBoot()
{
    const u16 name = PROC_SC_BOOT;
    s_ChangeRequest.name = name;
    s_ChangeRequest.typeA = 0;
    s_ChangeRequest.faderA = 0;
    s_ChangeRequest.typeB = 1;
    s_ChangeRequest.faderB = 1;
    s_ChangeRequest.arg = 0;
    s_LastRequested = name;
    s_ChangeRequest.isChanging = false;
}

// 0x0060758C (name is ours)
oml::framework::Process* BsScene::ProcessChange()
{
    ChangeRequest& request = s_ChangeRequest;
    if (request.isChanging || request.name == NO_SCENE || applet::ErrEulaMgr::s_pInstance->m_FB0) {
        return NULL;
    }
    const u8 state = netgame::NetGameMgr::s_pInstance->m_StageMgr.m_State;
    if (state != 0) {
        if (state == 14) {
            netgame::NetGameMgr::s_pInstance->m_StageMgr.OnArrive();
            netgame::NetGameMgr::s_pInstance->m_StageMgr.m_State = 15;
        } else if (state != 15) {
            return NULL;
        }
    }
    if (BsRoot::s_pInstance == NULL) {
        ProcMgr::Create(PROC_BS_ROOT, 0, PROC_TYPE_BASE);
        return NULL;
    }
    if (!CanChange(request.name)) {
        return NULL;
    }
    request.previous = request.current;
    request.current = request.name;
    oml::framework::Process* scene =
        ProcMgr::Create(static_cast<ProcName>(request.name), request.arg, PROC_TYPE_SCENE);
    if (scene == NULL) {
        return NULL;
    }
    if (state != 0) {
        netgame::NetGameMgr::s_pInstance->m_StageMgr.m_State = 16;
    }
    request.name = NO_SCENE;
    request.isChanging = true;
    return scene;
}

// 0x006076A8 (name is ours)
bool BsScene::CanChange(u16 name)
{
    if (s_ChangeRequest.pScene != NULL) {
        return true;
    }
    if (g_pUnknown950C4C != NULL) {
        return false;
    }
    Unknown95269C* object = g_pUnknown95269C;
    switch (object->state) {
    case 3:
        Func6A4D84(object);
        if (name == PROC_SC_BOOT) {
            Func6A5A70(g_pUnknown95269C);
        } else if (name == PROC_SC_STAGE || name == PROC_SC_STAGE_MINI_GAME_0 || name == PROC_SC_STAGE_MINI_GAME_1) {
            Func6A5008(g_pUnknown95269C);
        }
        if (!Func6A4CE0(object)) {
            return false;
        }
        return Func6A5BCC(object);
    case 0:
        if (name == PROC_SC_BOOT) {
            Func6A5A70(object);
        } else if (name == PROC_SC_STAGE || name == PROC_SC_STAGE_MINI_GAME_0 || name == PROC_SC_STAGE_MINI_GAME_1) {
            Func6A5008(object);
        }
        return Func6A5BCC(object);
    case 1:
        if (!Func6A4CE0(object)) {
            return false;
        }
        return Func6A5BCC(object);
    case 2:
        return Func6A5BCC(object);
    default:
        return false;
    }
}

// 0x006077F8 (name is ours)
void BsScene::Resume()
{
    m_IsPaused = false;
    oml::framework::ProcessManager* manager = oml::framework::ProcessManager::Get();
    if (manager->IsPausingChildren(m_Handle)) {
        manager->ResumeChildren(m_Handle);
    }
}

// 0x00607848 (name is ours)
void BsScene::Pause()
{
    m_IsPaused = true;
    oml::framework::ProcessManager* manager = oml::framework::ProcessManager::Get();
    if (!manager->IsPausingChildren(m_Handle)) {
        manager->PauseChildren(m_Handle);
    }
}
