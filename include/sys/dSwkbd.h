#pragma once

#include "decomp.h"
#include "sys/dTextManager.h"
#include "libms/lms_Api.h"

namespace ssys {
namespace ma {
class LoadSplit;
}
} // namespace ssys

// The software keyboard of the game ("Swkbd" after its files Layout/Swkbd/...): its parameters and settings, the text
// manager of the text being typed, and its message file. All names except the file names are ours.
namespace sys {
class LineManager;
class TextManager;

// the parameters of the software keyboard (set by the menus that open it)
struct KeyboardParam
{
    s32 _unknown0x000;            // 0x000 (reset by Create)
    s32 _unknown0x004;            // 0x004 (< 3)
    s32 acceptMode;               // 0x008 (< 5) when the text is accepted, see IsTextAcceptable
    s32 isMasked;                 // 0x00C (< 3) the characters are shown as '*' (a password; Create resets it to 0
                                  // before it checks it, like _unknown0x000)
    s32 _unknown0x010;            // 0x010
    s32 _unknown0x014;            // 0x014
    u32 flags;                    // 0x018 FLAG_*
    s32 _unknown0x01C;            // 0x01C
    u16 maxLength;                // 0x020
    u16 _unknown0x022;            // 0x022
    u16 digitOnlyException;       // 0x024 with FLAG_DIGITS_ONLY: any character is allowed while it is set
    u16 rows[3][17];              // 0x026 characters of three rows (each ends with 0 within 16 characters)
    s32 _unknown0x08C;            // 0x08C
    u16 _unknown0x090[65];        // 0x090
    u8 _unknown0x112;             // 0x112
    bool isNewlineAllowed;        // 0x113 a line feed may be typed while nothing is being composed
    bool isFullWidthMask;         // 0x114 masked characters are shown as the full-width '＊' (and at most 32)
    u8 _unknown0x115[11];         // 0x115
    s32 initialTextOffset;        // 0x120 -1: no initial text (else an offset into g_pInitialText)
    s32 _unknown0x124;            // 0x124
    s32 _unknown0x128;            // 0x128
    s32 _unknown0x12C;            // 0x12C
    s32 _unknown0x130;            // 0x130
    u16 _unknown0x134;            // 0x134
    s32 result;                   // 0x138 (-2: the parameters were invalid)
    s32 _unknown0x13C;            // 0x13C
    s32 _unknown0x140;            // 0x140
    s32 _unknown0x144;            // 0x144
    u16 _unknown0x148;            // 0x148
    s32 _unknown0x14C;            // 0x14C
    u16 _unknown0x150[257];       // 0x150

    // the bits of flags
    static const u32 FLAG_DIGITS_ONLY = 1 << 0;
    static const u32 FLAG_AT_SIGN = 1 << 1;      // '@' and '＠' are allowed (Create always sets it)
    static const u32 FLAG_PERCENT = 1 << 2;      // '%' is allowed
    static const u32 FLAG_BACKSLASH = 1 << 3;    // '\' is allowed
    static const u32 FLAG_SHORT = 1 << 4;        // at most 64 characters, no line feed
};
ASSERT_OFFSET(KeyboardParam, rows, 0x26);
ASSERT_OFFSET(KeyboardParam, isNewlineAllowed, 0x113);
ASSERT_OFFSET(KeyboardParam, result, 0x138);
ASSERT_OFFSET(KeyboardParam, _unknown0x150, 0x150);

// the settings of the keyboard
struct KeyboardSetting
{
    struct Words
    {
        u32 words[8];
    };

    s32 _unknown0x00;   // 0x00 (-1)
    s32 mode;           // 0x04 (2 and 3: the typed characters speak as 158)
    s32 kanaMode;       // 0x08 2: katakana ('n' becomes ン instead of ん)
    s32 _unknown0x0C;   // 0x0C
    s32 _unknown0x10;   // 0x10
    s32 _unknown0x14;   // 0x14
    s32 _unknown0x18;   // 0x18
    s32 defaultMode;    // 0x1C mode goes back to it when it is 2
    Words _unknown0x20; // 0x20
};
ASSERT_SIZE(KeyboardSetting, 0x40);

// a style of the message file RI.mstl (only the fields that are read)
struct MessageStyle
{
    u8 _unknown0x00[0x20];
    f32 lineSpace;  // 0x20
    f32 charSpace;  // 0x24
    u8 _unknown0x28[0x4];
};
ASSERT_SIZE(MessageStyle, 0x2C);

// RI.mstl: the number of styles, then the styles
struct MessageStyleBlock
{
    u32 num;
    MessageStyle styles[1];
};

extern bool g_Unknown95F0E8;                      // (read and passed through, purpose unknown)
extern bool g_IsFinished;                         // the keyboard has ended (or could not be made)
extern bool g_IsBufferOwned;                      // g_pTextBuffer is deleted by Destroy
extern u8 g_Unknown95F0EB;                        // 17 and 18: more characters are allowed (purpose unknown)
extern LMS_MessageBinary* g_pMessage;             // swkbd.msbt
extern const MessageStyleBlock* g_pMessageStyles; // RI.mstl
extern wchar_t* g_pInitialText;
extern s32 g_BufferSize;                          // maxLength + 1
extern wchar_t* g_pTextBuffer;
extern s32 g_Unknown95F114;
extern s32 g_AllowedCharNum;
extern TextManager* g_pTextManager;
extern s32 g_LineNum;
extern f32 g_Unknown95F1D8[2];
extern KeyboardParam g_KeyboardParam;
extern u16 g_AllowedChars[32];                    // characters allowed besides the usual ones
extern KeyboardSetting g_KeyboardSetting;
extern KeyboardSetting g_SavedKeyboardSetting;
extern ssys::ma::LoadSplit g_MessageFile;
extern ssys::ma::LoadSplit g_StyleFile;

// the lines of the text manager (NULL: none; inline, the copy is called by the keyboard code)
DECOMP_EMIT_COPY inline LineManager* GetLineManager() // 0x00523F48
{
    return g_pTextManager != NULL ? g_pTextManager->m_pLineManager : NULL;
}
// with FLAG_DIGITS_ONLY (and no exception): the character is a digit (also full-width, circled, roman and the like)
bool IsDigitChar(u16 c); // 0x00523F60
// the settings for the region; saved
void InitSetting(); // 0x0052402C
void ResetSetting(KeyboardSetting* setting); // 0x005241D0
// the saved settings come back (partly)
void RestoreSetting(); // 0x00524218
// the text can be accepted (after KeyboardParam::acceptMode)
bool IsTextAcceptable(); // 0x005242A4
void SetFinished(bool isFinished, s32 result); // 0x0052440C
// the parameters are checked and completed, the text manager is made; false: they are invalid
bool Create(); // 0x00524434
void ClearLastNewline(); // 0x00524964
void Destroy(); // 0x00524980
// the message file and the styles of the region
bool LoadMessage(); // 0x005249CC
void UnloadMessage(); // 0x00524B24
const wchar_t* GetMessageText(const char* label); // 0x00524B6C
} // namespace sys
