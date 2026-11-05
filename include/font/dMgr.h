#pragma once

#include "decomp.h"
#include "Other/ddLoadSplit.h"
#include "nw/font/font_ResFont.h"
#include "sead/seadFixedSafeString.h"
#include "ssys/ma/dFileData.h"

namespace sead {
class Heap;
} // namespace sead

namespace font {
// the fonts of the game (names after libgarden; GARDEN_PLUS_JIS_LEVEL1 after its file name, libgarden: UNK)
enum FontID
{
    GARDEN_MSG_16 = 0,
    GARDEN_RUBY = 1,
    GARDEN_NO = 2,
    GARDEN_NO_F16 = 3,
    GARDEN_PLUS_JIS_LEVEL1 = 4,
};

// The fonts of the game, "Font/<name>.bcfnt" from the ROM: four that stay loaded and one (GardenPlus_JisLevel1) that is
// loaded into the heap of the photos on request (the photos free their buffers for it). A singleton whose
// SingletonDisposer_ lies in its first 16 bytes. (class and GetFont/GetFontName after libgarden, all other names ours)
class Mgr
{
public:
    class SingletonDisposer_;

    // a font file and the font made from it
    class FontData
    {
    public:
        enum State : u8
        {
            STATE_NONE = 0,
            STATE_REQUESTED = 1,  // Load starts reading the file
            STATE_FILE_LOADED = 2,
            STATE_READY = 3,
        };

        // useLoadSplit: the file is read in pieces (async) instead of at once
        DECOMP_NOINLINE explicit FontData(bool useLoadSplit); // 0x0052EFB8

        // the name without extension; heap NULL: the font heap
        void Setup(const char* name, bool flag, sead::Heap* heap); // 0x0052EF04
        // goes on loading; true when the font is ready
        bool Load(); // 0x0052EE18
        // frees the font and the file and forgets the name
        DECOMP_EMIT_COPY DECOMP_NOINLINE void Unload() // 0x00132C60
        {
            m_Font.ReleaseResource();
            if (m_Font.m_pTextureObjects != NULL) {
                m_pHeap->free(m_Font.m_pTextureObjects);
            }
            if (m_UseLoadSplit) {
                m_LoadSplit.Reset();
            } else {
                m_File.Unload();
            }
            m_IsFileLoaded = false;
            m_Path.clear();
            m_FileName.clear();
            m_Flag = false;
            m_State = STATE_NONE;
            m_pHeap = NULL;
        }
        bool IsReady() const { return m_State == STATE_READY; }
        const nw::font::Font* GetFont() const { return IsReady() ? &m_Font : NULL; }

        bool m_UseLoadSplit;                   // 0x000
        bool m_IsFileLoaded;                   // 0x001
        ssys::ma::FileData m_File;             // 0x004
        dLoadSplit m_LoadSplit;                // 0x014
        sead::FixedSafeString<64> m_Path;      // 0x100 "Font/<file name>"
        sead::FixedSafeString<32> m_FileName;  // 0x14C "<name>.bcfnt"
        nw::font::ResFont m_Font;              // 0x178
        bool m_Flag;                           // 0x190 (set by Setup, nothing reads it)
        u8 m_State;                            // 0x191 State
        sead::Heap* m_pHeap;                   // 0x194
    };

    // a font that is loaded and unloaded on request; Calc runs one step a frame
    class ReloadableFontData : public FontData
    {
    public:
        enum LoadState : u8
        {
            LOAD_STATE_NONE = 0,
            LOAD_STATE_FREE_PHOTO = 1,     // the photos free their buffers
            LOAD_STATE_LOADING = 2,
            LOAD_STATE_LOADED = 3,
            LOAD_STATE_UNLOADING = 4,
            LOAD_STATE_RESTORE_PHOTO = 5,  // the photos get their buffers back
            LOAD_STATE_NUM = 6,
        };
        typedef void (ReloadableFontData::*StateFunction)();

        ReloadableFontData() : FontData(true), m_LoadState(LOAD_STATE_NONE) {}

        void Calc()
        {
            StateFunction function = s_StateFunctions[m_LoadState];
            if (function != NULL) {
                (this->*function)();
            }
        }
        // a load finishes now, a loaded font starts unloading
        void Sync(); // 0x0052F218

        void StateLoading(); // 0x0052F14C
        void StateUnloading(); // 0x0052F1AC
        void StateFreePhoto(); // 0x0052F1D0
        void StateRestorePhoto(); // 0x0052F1F4

        static const StateFunction s_StateFunctions[LOAD_STATE_NUM];

        u8 m_LoadState; // 0x198 LoadState
    };

    // creates the manager and loads the four fonts that stay; true when they are ready
    static bool Initialize(); // 0x0052EAE4
    // unloads the four fonts and destroys the manager
    static void Destroy(); // 0x0012C3C4
    static void SyncJisLevel1Font(); // 0x0052F2AC

    ~Mgr(); // 0x00132BD8

    void Calc(); // 0x0052EC58
    // (the original calls of these three are replaced by their only instruction, so their copies are unknown)
    DECOMP_NOIPA bool IsReady() const;
    DECOMP_NOIPA void Draw();
    DECOMP_NOIPA void Finalize();

    void RequestLoadJisLevel1Font(); // 0x0052ED44
    void RequestUnloadJisLevel1Font(); // 0x0052ED5C
    DECOMP_EMIT_COPY bool IsJisLevel1FontLoaded() const // 0x00748DC8
    {
        return m_JisLevel1Font.m_LoadState == ReloadableFontData::LOAD_STATE_LOADED;
    }
    DECOMP_EMIT_COPY bool IsJisLevel1FontUnloaded() const // 0x00748DDC
    {
        return m_JisLevel1Font.m_LoadState == ReloadableFontData::LOAD_STATE_NONE;
    }

    // NULL if the font is not ready
    const nw::font::Font* GetFont(FontID font) const; // 0x0052ED74 | libgarden
    const sead::FixedSafeString<32>* GetFontName(FontID font) const; // 0x00748D88 | libgarden

    static Mgr* s_pInstance;
    static SingletonDisposer_* s_pSingletonDisposer;

private:
    Mgr() : m_MsgFont(true), m_RubyFont(false), m_NoFont(false), m_NoF16Font(false) {}
    static Mgr* createInstance(sead::Heap* heap);

public:
    u32 m_SingletonDisposerBuffer[4];      // 0x000, the SingletonDisposer_
    FontData m_MsgFont;                    // 0x010 "Garden_msg_size16"
    FontData m_RubyFont;                   // 0x1A8 "Garden_ruby"
    FontData m_NoFont;                     // 0x340 "Garden_no"
    FontData m_NoF16Font;                  // 0x4D8 "Garden_no_f16"
    ReloadableFontData m_JisLevel1Font;    // 0x670 "GardenPlus_JisLevel1"
};
ASSERT_SIZE(Mgr::FontData, 0x198);
ASSERT_SIZE(Mgr, 0x80C);
} // namespace font
