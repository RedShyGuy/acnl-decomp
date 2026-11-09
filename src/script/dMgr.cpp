#include "script/dMgr.h"
#include "script/dChoiceStandardWin.h"
#include "script/dFlowSystem.h"
#include "script/dITalkRecept.h"
#include "script/dMailMgr.h"
#include "script/dMailMgr_SingletonDisposer_.h"
#include "script/dMgr_SingletonDisposer_.h"
#include "script/dProject.h"
#include "script/dProject_SingletonDisposer_.h"
#include "script/dStrMgr.h"
#include "script/dUiMgr.h"
#include "script/dUiMgr_SingletonDisposer_.h"
#include "script/dWordRes.h"
#include "script/dRenderBase.h"
#include "script/dRenderMain.h"
#include "script/dRenderRuby.h"
#include "script/dTalkSystem.h"
#include "script/dTalkWin.h"
#include "sead/seadNew.h"
#include "time/time_Api.h"

#include <new>

namespace script {
// 0x0095EECC
Mgr* Mgr::s_pInstance = NULL;
// 0x0095EED0
Mgr::SingletonDisposer_* Mgr::s_pSingletonDisposer = NULL;
// 0x0095EEF4 (name is ours)
sead::Heap* Mgr::s_pMessageHeap;
// 0x0095EEF8 (name is ours)
sead::Heap* Mgr::s_pDataHeap;
// 0x0095EEFC (name is ours)
sead::Heap* Mgr::s_pFileHeap;
// 0x0095EF00 (name is ours)
sead::Heap* Mgr::s_pMailHeap;
// 0x0095EF04 (name is ours)
sead::Heap* Mgr::s_pFlowHeap;

// 0x005EA318 (name after sead)
Mgr* Mgr::createInstance(sead::Heap* heap)
{
    if (s_pInstance == NULL) {
        u8* buffer = new (heap, 4) u8[sizeof(Mgr)];
        Mgr* instance = NULL;
        if (buffer == NULL) {
            s_pSingletonDisposer = NULL;
        } else {
            s_pSingletonDisposer = new (buffer) SingletonDisposer_(heap);
            instance = new (buffer) Mgr();
        }
        s_pInstance = instance;
    }
    return s_pInstance;
}

// 0x005EA38C (name after sead)
void Mgr::deleteInstance()
{
    SingletonDisposer_* disposer = s_pSingletonDisposer;
    if (disposer == NULL) {
        return;
    }
    s_pSingletonDisposer = NULL;
    disposer->~SingletonDisposer_();
    if (s_pInstance != NULL) {
        delete s_pInstance;
    }
    s_pInstance = NULL;
}

// 0x005EA444 (name is ours)
void Mgr::Draw()
{
    TalkSystem* talk = m_pTalk;
    if (talk->m_pRecept == NULL) {
        return;
    }
    talk->m_pWin->m_Window.Draw();
    talk->m_pChoice->m_Window.Draw();
}

// 0x005EA478 (name is ours)
bool Mgr::Initialize()
{
    if (m_pDate == NULL) {
        m_pDate = new (std::nothrow) nn::fnd::DateTimeParameters(time::GetFirstDate());
    }
    if (m_pTalk == NULL) {
        m_pTalk = new (std::nothrow) TalkSystem(m_pDate);
    }
    if (m_pFlow == NULL) {
        m_pFlow = new (std::nothrow) FlowSystem(m_pDate);
    }
    for (u32 i = 0; i < FlowSystem::SEQ_NUM; i++) {
        // (empty in the original too)
    }
    TalkSystem* talk = m_pTalk;
    bool isReady = talk->m_pWin->Setup();
    return talk->m_pChoice->SetupTextBoxes(0) & isReady;
}

// 0x005EA5A8 (name is ours)
void Mgr::Finalize()
{
    if (m_pFlow != NULL) {
        delete m_pFlow;
        m_pFlow = NULL;
    }
    if (m_pTalk != NULL) {
        m_pTalk->Release();
        delete m_pTalk;
        m_pTalk = NULL;
    }
    if (m_pDate != NULL) {
        delete m_pDate;
        m_pDate = NULL;
    }
}

// 0x005EA6E0 (name is ours)
void Mgr::Calc()
{
    m_pFlow->Update();
    TalkSystem* talk = m_pTalk;
    talk->m_pSong->Update();
    talk->m_pMain->ShiftPositions();
    talk->m_pRuby->ShiftPositions();
    talk->m_pMain->ClearBuf2();
    talk->m_pRuby->ClearBuf2();
    talk->Update();
    if (talk->m_State != TalkSystem::STATE_IDLE) {
        talk->m_pWin->Update();
        talk->m_pChoice->Run();
    }
    if (talk->m_pRecept != NULL) {
        talk->m_pRecept->Slot94();
    }
}

namespace {
// the memory functions of libms
// 0x005253E4 (name is ours)
void* AllocMessageMemory(u32 size)
{
    return Mgr::s_pMessageHeap->tryAlloc(size, 4);
}

// 0x005253C0 (name is ours)
void FreeMessageMemory(void* ptr)
{
    if (ptr != NULL) {
        Mgr::s_pMessageHeap->free(ptr);
    }
}
} // namespace

// 0x005E9DD0 (name is ours)
bool Setup()
{
    libms::Initialize(AllocMessageMemory, FreeMessageMemory);

    // the message project
    Project* project = Project::s_pInstance;
    if (project == NULL) {
        sead::Heap* heap = NULL;
        u8* buffer = new (heap, 4) u8[sizeof(Project)];
        if (buffer == NULL) {
            Project::s_pSingletonDisposer = NULL;
        } else {
            Project::s_pSingletonDisposer = new (buffer) Project::SingletonDisposer_(heap);
            project = new (buffer) Project();
        }
        Project::s_pInstance = project;
    }
    bool isReady = true;
    if (!project->m_Project.IsInitialized()) {
        if (!project->m_Loader.Load(sead::SafeStringBase<char>("Script/CTR_GardenPlus.msbp"), Mgr::s_pDataHeap, 0)) {
            isReady = false;
        } else {
            project->m_Project.Initialize(project->m_Loader.GetData());
        }
    }

    // the string lists
    StrMgr* strMgr = StrMgr::s_pInstance;
    if (strMgr == NULL) {
        strMgr = StrMgr::createInstance(NULL);
    }
    isReady = strMgr->Load() & isReady;

    // the letters with the first characters of the keyword lists
    MailMgr* mailMgr = MailMgr::s_pInstance;
    if (mailMgr == NULL) {
        sead::Heap* heap = NULL;
        u8* buffer = new (heap, 4) u8[sizeof(MailMgr)];
        if (buffer == NULL) {
            MailMgr::s_pSingletonDisposer = NULL;
        } else {
            MailMgr::s_pSingletonDisposer = new (buffer) MailMgr::SingletonDisposer_(heap);
            mailMgr = new (buffer) MailMgr();
        }
        MailMgr::s_pInstance = mailMgr;
    }
    if (!mailMgr->m_IsLoaded) {
        MailChecker::Matcher* matcher = &mailMgr->m_pChecker->m_Matcher;
        bool isLoaded;
        if (matcher->m_FirstCharNum != 0) {
            isLoaded = true;
        } else {
            matcher->m_pStrMgr = StrMgr::s_pInstance;
            if (!StrMgr::s_pInstance->m_IsLoaded) {
                isLoaded = false;
            } else {
                s32 count = StrMgr::s_pInstance->GetCount("STR_Mailchk");
                matcher->m_FirstCharNum = count > static_cast<s32>(MailChecker::FIRST_CHAR_NUM)
                                              ? static_cast<s32>(MailChecker::FIRST_CHAR_NUM)
                                              : count;
                WordRes word;
                for (s32 i = 0; i < matcher->m_FirstCharNum; i++) {
                    matcher->m_pStrMgr->Find(&word, "STR_Mailchk", i);
                    matcher->m_FirstChars[i] = *static_cast<const IWord&>(word).GetWord();
                }
                isLoaded = matcher->m_FirstCharNum != 0;
            }
        }
        if (isLoaded) {
            mailMgr->m_IsLoaded = 1;
        }
    }
    isReady = mailMgr->m_IsLoaded & isReady;

    // the texts of the user interface
    if (UiMgr::s_pInstance == NULL) {
        sead::Heap* heap = NULL;
        u8* buffer = new (heap, 4) u8[sizeof(UiMgr)];
        UiMgr* uiMgr = NULL;
        if (buffer == NULL) {
            UiMgr::s_pSingletonDisposer = NULL;
        } else {
            UiMgr::s_pSingletonDisposer = new (buffer) UiMgr::SingletonDisposer_(heap);
            uiMgr = new (buffer) UiMgr();
        }
        UiMgr::s_pInstance = uiMgr;
    }
    return isReady;
}
} // namespace script
