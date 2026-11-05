#include "font/dMgr.h"
#include "font/dMgr_SingletonDisposer_.h"
#include "Other/dHeapInit.h"
#include "nn/os/os_Thread.h"
#include "photo/dMgr.h"
#include "sead/seadExpHeap.h"
#include "sead/seadHeap.h"
#include "sead/seadNew.h"
#include "sead/seadStringFormatter.h"

#include <new>

namespace font {
// 0x009539CC
Mgr* Mgr::s_pInstance = NULL;
// 0x009539D0
Mgr::SingletonDisposer_* Mgr::s_pSingletonDisposer = NULL;

// (inline, name after sead)
inline font::Mgr* font::Mgr::createInstance(sead::Heap* heap)
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

// 0x0052EAE4 (name is ours)
bool font::Mgr::Initialize()
{
    Mgr* mgr = createInstance(NULL);
    if (mgr->m_MsgFont.m_State == FontData::STATE_NONE) {
        mgr->m_MsgFont.Setup("Garden_msg_size16", true, NULL);
    }
    if (mgr->m_RubyFont.m_State == FontData::STATE_NONE) {
        mgr->m_RubyFont.Setup("Garden_ruby", true, NULL);
    }
    if (mgr->m_NoFont.m_State == FontData::STATE_NONE) {
        mgr->m_NoFont.Setup("Garden_no", true, NULL);
    }
    if (mgr->m_NoF16Font.m_State == FontData::STATE_NONE) {
        mgr->m_NoF16Font.Setup("Garden_no_f16", true, NULL);
    }
    bool isReady = mgr->m_MsgFont.Load();
    isReady &= mgr->m_RubyFont.Load();
    isReady &= mgr->m_NoFont.Load();
    isReady &= mgr->m_NoF16Font.Load();
    return isReady;
}

// 0x0012C3C4 (name is ours)
void font::Mgr::Destroy()
{
    Mgr* mgr = s_pInstance;
    if (mgr == NULL) {
        return;
    }
    mgr->m_NoF16Font.Unload();
    mgr->m_NoFont.Unload();
    mgr->m_RubyFont.Unload();
    mgr->m_MsgFont.Unload();
    // deleteInstance (name after sead)
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

// 0x00132BD8 (name is ours)
font::Mgr::~Mgr()
{
    // (only the members are destroyed)
}

bool font::Mgr::IsReady() const
{
    return true;
}

void font::Mgr::Draw()
{
}

void font::Mgr::Finalize()
{
}

// 0x0052EC58 (name is ours)
void font::Mgr::Calc()
{
    m_JisLevel1Font.Calc();
}

// 0x0052ED44 (name is ours)
void font::Mgr::RequestLoadJisLevel1Font()
{
    if (m_JisLevel1Font.m_LoadState == ReloadableFontData::LOAD_STATE_NONE) {
        m_JisLevel1Font.m_LoadState = ReloadableFontData::LOAD_STATE_FREE_PHOTO;
    }
}

// 0x0052ED5C (name is ours)
void font::Mgr::RequestUnloadJisLevel1Font()
{
    if (m_JisLevel1Font.m_LoadState == ReloadableFontData::LOAD_STATE_LOADED) {
        m_JisLevel1Font.m_LoadState = ReloadableFontData::LOAD_STATE_UNLOADING;
    }
}

// 0x0052ED74 (name after libgarden)
const nw::font::Font* font::Mgr::GetFont(FontID font) const
{
    if (font == GARDEN_MSG_16) {
        return m_MsgFont.GetFont();
    }
    if (font == GARDEN_RUBY) {
        return m_RubyFont.GetFont();
    }
    if (font == GARDEN_NO) {
        return m_NoFont.GetFont();
    }
    if (font == GARDEN_PLUS_JIS_LEVEL1) {
        return m_JisLevel1Font.GetFont();
    }
    return m_NoF16Font.GetFont();
}

// 0x00748D88 (name after libgarden)
const sead::FixedSafeString<32>* font::Mgr::GetFontName(FontID font) const
{
    if (font == GARDEN_MSG_16) {
        return &m_MsgFont.m_FileName;
    }
    if (font == GARDEN_RUBY) {
        return &m_RubyFont.m_FileName;
    }
    if (font == GARDEN_NO) {
        return &m_NoFont.m_FileName;
    }
    if (font == GARDEN_PLUS_JIS_LEVEL1) {
        return &m_JisLevel1Font.m_FileName;
    }
    return &m_NoF16Font.m_FileName;
}

// 0x0052EFB8 (name is ours)
font::Mgr::FontData::FontData(bool useLoadSplit)
    : m_UseLoadSplit(useLoadSplit), m_IsFileLoaded(false), m_File(), m_LoadSplit(), m_Flag(false),
      m_State(STATE_NONE), m_pHeap(NULL)
{
}

// 0x0052EF04 (name is ours)
void font::Mgr::FontData::Setup(const char* name, bool flag, sead::Heap* heap)
{
    {
        sead::StringFormatter formatter(&m_FileName);
        formatter.append("%s.bcfnt").append(name).flush();
    }
    {
        sead::StringFormatter formatter(&m_Path);
        formatter.append("Font/%s").append(m_FileName.getCString()).flush();
    }
    m_Flag = flag;
    if (heap != NULL) {
        m_pHeap = heap;
    } else {
        m_pHeap = HeapInit::s_pFontHeap;
    }
}

// 0x0052EE18 (name is ours)
bool font::Mgr::FontData::Load()
{
    if (m_State == STATE_NONE) {
        m_State = STATE_REQUESTED;
    }
    if (m_State == STATE_REQUESTED) {
        if (m_UseLoadSplit) {
            if (m_LoadSplit.Read(m_Path, m_pHeap, 128)) {
                m_IsFileLoaded = m_LoadSplit.IsSucceeded();
                m_State = STATE_FILE_LOADED;
            }
        } else {
            m_IsFileLoaded = m_File.Load(m_Path, m_pHeap, 128);
            m_State = STATE_FILE_LOADED;
        }
    }
    if (m_State == STATE_FILE_LOADED) {
        void* data = m_UseLoadSplit ? m_LoadSplit.m_pData : m_File.m_pData;
        if (m_Font.SetResource(data)) {
            void* buffer = m_pHeap->tryAlloc(nw::font::ResFont::GetDrawBufferSize(data), 4);
            if (buffer != NULL) {
                m_Font.SetDrawBuffer(buffer);
            }
        }
        m_State = STATE_READY;
        return true;
    }
    return m_State == STATE_READY;
}

// 0x0052F14C (name is ours)
void font::Mgr::ReloadableFontData::StateLoading()
{
    if (m_State == STATE_NONE) {
        Setup("GardenPlus_JisLevel1", true, HeapInit::s_pPhotoHeap);
    }
    if (Load()) {
        m_LoadState = LOAD_STATE_LOADED;
    }
}

// 0x0052F1AC (name is ours)
void font::Mgr::ReloadableFontData::StateUnloading()
{
    if (m_State != STATE_NONE) {
        Unload();
    }
    m_LoadState = LOAD_STATE_RESTORE_PHOTO;
}

// 0x0052F1D0 (name is ours)
void font::Mgr::ReloadableFontData::StateFreePhoto()
{
    photo::Mgr::s_pInstance->FreeBuffers();
    m_LoadState = LOAD_STATE_LOADING;
}

// 0x0052F1F4 (name is ours)
void font::Mgr::ReloadableFontData::StateRestorePhoto()
{
    photo::Mgr::s_pInstance->Func5B321C();
    m_LoadState = LOAD_STATE_NONE;
}

// 0x0052F218 (name is ours)
void font::Mgr::ReloadableFontData::Sync()
{
    if (m_LoadState == LOAD_STATE_FREE_PHOTO || m_LoadState == LOAD_STATE_LOADING) {
        while (m_LoadState != LOAD_STATE_LOADED) {
            nn::os::Thread::SleepImpl(nn::fnd::TimeSpan::FromMilliSeconds(1));
            Calc();
        }
    } else if (m_LoadState == LOAD_STATE_LOADED) {
        m_LoadState = LOAD_STATE_UNLOADING;
    }
}

// 0x0052F2AC (name is ours)
void font::Mgr::SyncJisLevel1Font()
{
    if (s_pInstance != NULL) {
        s_pInstance->m_JisLevel1Font.Sync();
    }
}
} // namespace font
