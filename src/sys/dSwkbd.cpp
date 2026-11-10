#include "sys/dSwkbd.h"

#include <cstdio>
#include <cstring>

#include "Other/dRegion.h"
#include "sead/seadHeap.h"
#include "sead/seadNew.h"
#include "sead/seadSafeStringBase.h"
#include "ssys/ma/dAllocator.h"
#include "ssys/ma/dLoadSplit.h"
#include "ssys/ma/lyt/lyt_Api.h"
#include "sys/dLineManager.h"
#include "sys/dTextManager.h"

namespace {
// the rows of KeyboardParam::rows that each value of KeyboardParam::_unknown0x000 uses (0: none)
// 0x0089847A
const u8 s_RowsUsed[6][3] = {
    {0, 0, 1}, {1, 0, 1}, {1, 1, 1}, {0, 0, 0}, {0, 0, 2}, {0, 0, 0},
};
// private-use characters of the keyboard that the control characters of the initial text become
const u16 CHAR_TAB = 0xE06C;
const u16 CHAR_CARRIAGE_RETURN = 0xE06D;
const u16 CHAR_FORM_FEED = 0xE06E;
} // namespace

namespace sys {
// 0x0095F0E8
bool g_Unknown95F0E8 = false;
// 0x0095F0E9
bool g_IsFinished = false;
// 0x0095F0EA
bool g_IsBufferOwned = true;
// 0x0095F0EB
u8 g_Unknown95F0EB = 19;
// 0x0095F100
LMS_MessageBinary* g_pMessage = NULL;
// 0x0095F104
const MessageStyleBlock* g_pMessageStyles = NULL;
// 0x0095F108
wchar_t* g_pInitialText = NULL;
// 0x0095F10C
s32 g_BufferSize = 0;
// 0x0095F110
wchar_t* g_pTextBuffer = NULL;
// 0x0095F114
s32 g_Unknown95F114 = 0;
// 0x0095F118
s32 g_ForbiddenCharNum = 0;
// 0x0095F11C
TextManager* g_pTextManager = NULL;
// 0x0095F120
s32 g_LineNum = 1;
// 0x0095F1D8
f32 g_Unknown95F1D8[2] = {0.0f, 0.0f};
// 0x00AD7140
KeyboardParam g_KeyboardParam;
// 0x00AD75C0
u16 g_ForbiddenChars[32];
// 0x00AD7540
KeyboardSetting g_KeyboardSetting;
// 0x00AD7580
KeyboardSetting g_SavedKeyboardSetting;
// 0x00AD6F68
ssys::ma::LoadSplit g_MessageFile;
// 0x00AD7054
ssys::ma::LoadSplit g_StyleFile;

// 0x00523F60
bool IsDigitChar(u16 c)
{
    if ((g_KeyboardParam.flags & KeyboardParam::FLAG_DIGITS) == 0 || g_KeyboardParam.maxDigits != 0) {
        return false;
    }
    if ((c >= '0' && c <= '9') || (c >= 0xFF10 && c <= 0xFF19) || (c >= 0x2070 && c <= 0x2079) || (c >= 0x2080 && c <= 0x2089) ||
        (c >= 0x2160 && c <= 0x216B) || (c >= 0x2170 && c <= 0x217B) || (c >= 0x2460 && c <= 0x2473) || (c >= 0x2474 && c <= 0x2487) ||
        (c >= 0x2488 && c <= 0x249B) || (c >= 0x2776 && c <= 0x277F) || (c >= 0x2780 && c <= 0x2789) || (c >= 0x278A && c <= 0x2793)) {
        return true;
    }
    return false;
}

// 0x0052402C
void InitSetting()
{
    KeyboardSetting& setting = g_KeyboardSetting;
    ResetSetting(&setting);
    setting._unknown0x00 = -1;

    s32 mode = setting.mode;
    if (GetRegion() != 0) {
        if (mode != 0 && mode != 2 && mode != 3 && mode != 4) {
            mode = 0;
        }
    } else {
        if (mode != 0 && mode != 1 && mode != 2 && mode != 4) {
            mode = 1;
        }
    }
    setting.mode = mode;

    s32 kanaMode = setting.kanaMode;
    if (GetRegion() != 0 || static_cast<u32>(kanaMode) >= 3) {
        kanaMode = 0;
    }
    setting.kanaMode = kanaMode;

    s32 kanaKeySet = setting.kanaKeySet;
    if (GetRegion() != 0 || static_cast<u32>(kanaKeySet) >= 2) {
        kanaKeySet = 0;
    }
    setting.kanaKeySet = kanaKeySet;

    s32 cellPhoneKeySet = setting.cellPhoneKeySet;
    if (GetRegion() != 0) {
        if (cellPhoneKeySet != 2 && cellPhoneKeySet != 3 && cellPhoneKeySet != 6) {
            cellPhoneKeySet = 2;
        }
    } else {
        switch (cellPhoneKeySet) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 6:
            break;
        default:
            cellPhoneKeySet = 0;
            break;
        }
    }
    setting.cellPhoneKeySet = cellPhoneKeySet;
    if (setting._unknown0x14 <= 0) {
        setting._unknown0x14 = 0;
    }
    if (setting._unknown0x18 <= 0) {
        setting._unknown0x18 = 0;
    }
    g_SavedKeyboardSetting = setting;
}

// 0x005241D0
void ResetSetting(KeyboardSetting* setting)
{
    setting->kanaMode = 0;
    setting->kanaKeySet = 0;
    setting->_unknown0x14 = 0;
    setting->_unknown0x18 = 0;
    if (GetRegion() != 0) {
        setting->cellPhoneKeySet = 2;
        setting->mode = 0;
        setting->defaultMode = 0;
    } else {
        setting->mode = 1;
        setting->cellPhoneKeySet = 0;
        setting->defaultMode = 1;
    }
}

// 0x00524218
void RestoreSetting()
{
    KeyboardSetting& setting = g_KeyboardSetting;
    const KeyboardSetting& saved = g_SavedKeyboardSetting;
    setting.defaultMode = saved.defaultMode;
    setting.cellPhoneKeySet = saved.cellPhoneKeySet;
    setting._unknown0x00 = saved._unknown0x00;
    setting.mode = saved.mode;
    setting.kanaMode = saved.kanaMode;
    setting._unknown0x20 = saved._unknown0x20;
    setting.kanaKeySet = 0;
    setting._unknown0x14 = 0;
    setting._unknown0x18 = 0;
    setting.cellPhoneKeySet = GetRegion() != 0 ? 2 : 0;
    if (setting.kanaMode == 2) {
        setting.kanaMode = 1;
    }
    if (setting.mode == 2) {
        setting.mode = setting.defaultMode;
    }
}

// (inline) the text has a character that is no space
static inline bool HasNonSpace(const wchar_t* text, s32 length)
{
    for (s32 i = 0; i < length; i++) {
        if (text[i] != ' ' && text[i] != 0x3000) {
            return true;
        }
    }
    return false;
}

// 0x005242A4
bool IsTextAcceptable()
{
    const wchar_t* text = g_pTextBuffer;
    const TextManager* textManager = g_pTextManager;
    switch (g_KeyboardParam.acceptMode) {
    case 1:
        // not empty
        return textManager->m_Length > 0;
    case 2:
        // not empty and not only spaces
        if (textManager->m_Length <= 0) {
            return false;
        }
        return HasNonSpace(text, textManager->m_Length);
    case 3:
        // empty or not only spaces
        if (textManager->m_Length == 0) {
            return true;
        }
        return HasNonSpace(text, textManager->m_Length);
    case 4:
        // full
        return textManager->m_Length >= g_KeyboardParam.maxLength;
    default:
        return true;
    }
}

// 0x0052440C
void SetFinished(bool isFinished, s32 result)
{
    g_IsFinished = isFinished;
    if (isFinished && result != -1) {
        g_KeyboardParam.result = result;
    }
}

// 0x00524434
bool Create()
{
    KeyboardParam& param = g_KeyboardParam;
    param._unknown0x000 = 0;
    param.result = -1;
    param.flags |= KeyboardParam::FLAG_AT_SIGN;
    param._unknown0x144 = -1;
    param._unknown0x148 = 0;
    param.isMasked = 0;
    param._unknown0x010 = 0;
    param._unknown0x014 = 0;
    param._unknown0x01C = 0;
    param._unknown0x022 = 0;
    param.maxDigits = 0;
    std::memset(param.rows, 0, sizeof(param.rows));
    param._unknown0x08C = 0;
    std::memset(param._unknown0x090, 0, sizeof(param._unknown0x090));
    param._unknown0x112 = 0;
    param._unknown0x115[0] = 0;
    param._unknown0x115[1] = 0;
    param._unknown0x115[2] = 0;
    param._unknown0x115[3] = 0;
    param._unknown0x115[4] = 0;
    param._unknown0x124 = -1;
    param._unknown0x128 = -1;
    param._unknown0x130 = 0;
    param._unknown0x12C = -1;
    param._unknown0x134 = 0;
    param._unknown0x13C = -1;
    param._unknown0x14C = 0;
    param._unknown0x140 = -1;
    std::memset(param._unknown0x150, 0, sizeof(param._unknown0x150));

    g_BufferSize = 0;
    g_Unknown95F114 = 0;
    g_ForbiddenCharNum = 0;
    g_IsFinished = false;
    g_Unknown95F1D8[0] = 0.0f;
    g_Unknown95F1D8[1] = 0.0f;
    sead::Heap* heap = ssys::ma::lyt::GetAllocator()->m_pHeap;
    std::memset(g_ForbiddenChars, 0, sizeof(g_ForbiddenChars));
    wchar_t* initialText = g_pInitialText;
    param.initialTextOffset = initialText == NULL ? -1 : 0;
    g_Unknown95F0E8 = false;

    if (param.isFullWidthMask) {
        param.isNewlineAllowed = false;
        if (param.maxLength > 32) {
            param.maxLength = 32;
        }
    }
    if (param.flags != 0) {
        if ((param.flags & KeyboardParam::FLAG_SHORT) != 0) {
            param.isNewlineAllowed = false;
            if (param.maxLength > 64) {
                param.maxLength = 64;
            }
        }
        if (param.maxLength > 2000) {
            param.maxLength = 2000;
        }
    }

    if (static_cast<u32>(param._unknown0x000) >= 6 || static_cast<u32>(param._unknown0x004) >= 3 || static_cast<u32>(param.acceptMode) >= 5 ||
        static_cast<u32>(param.isMasked) >= 3) {
        param.result = -2;
        g_IsFinished = true;
        return false;
    }
    // the rows that are used must end within 16 characters
    for (u32 i = 0; i < 3; i++) {
        if (s_RowsUsed[param._unknown0x000][i] == 0) {
            continue;
        }
        for (s32 j = 0; j < 17; j++) {
            if (param.rows[i][j] == 0) {
                break;
            }
            if (j == 16) {
                param.result = -2;
                g_IsFinished = true;
                return false;
            }
        }
    }

    if ((param.flags & KeyboardParam::FLAG_AT_SIGN) != 0) {
        g_ForbiddenChars[g_ForbiddenCharNum++] = '@';
        g_ForbiddenChars[g_ForbiddenCharNum++] = 0xFF20;
    }
    if ((param.flags & KeyboardParam::FLAG_PERCENT) != 0) {
        g_ForbiddenChars[g_ForbiddenCharNum++] = '%';
    }
    if ((param.flags & KeyboardParam::FLAG_BACKSLASH) != 0) {
        g_ForbiddenChars[g_ForbiddenCharNum++] = '\\';
    }
    if (g_Unknown95F0EB == 17 || g_Unknown95F0EB == 18) {
        g_ForbiddenChars[g_ForbiddenCharNum++] = '-';
        g_ForbiddenChars[g_ForbiddenCharNum++] = ':';
        g_ForbiddenChars[g_ForbiddenCharNum++] = '\'';
        g_ForbiddenChars[g_ForbiddenCharNum++] = '`';
        g_ForbiddenChars[g_ForbiddenCharNum++] = ',';
        g_ForbiddenChars[g_ForbiddenCharNum++] = '.';
        g_ForbiddenChars[g_ForbiddenCharNum++] = '\\';
        g_ForbiddenChars[g_ForbiddenCharNum++] = '/';
        g_ForbiddenChars[g_ForbiddenCharNum++] = '=';
        g_ForbiddenChars[g_ForbiddenCharNum++] = '[';
        g_ForbiddenChars[g_ForbiddenCharNum++] = ']';
        g_ForbiddenChars[g_ForbiddenCharNum++] = '?';
        g_ForbiddenChars[g_ForbiddenCharNum++] = '!';
    }

    if (param.initialTextOffset != -1) {
        // the control characters of the initial text become characters of the keyboard
        wchar_t* text = reinterpret_cast<wchar_t*>(reinterpret_cast<uptr>(initialText) + param.initialTextOffset);
        for (s32 i = 0; i < param.maxLength; i++) {
            u16 c = text[i];
            if (c == 0) {
                break;
            }
            if (c == '\t') {
                c = CHAR_TAB;
            } else if (c == '\f') {
                c = CHAR_FORM_FEED;
            } else if (c == '\r') {
                c = CHAR_CARRIAGE_RETURN;
            }
            text[i] = c;
        }
    }

    g_BufferSize = param.maxLength + 1;
    TextManager* textManager = new (heap, 4) TextManager(param.maxLength, g_pTextBuffer);
    g_pTextManager = textManager;
    LineManager* lineManager = new (heap, 4) LineManager(g_LineNum, textManager);
    textManager->m_pLineManager = lineManager;
    lineManager->m_LineStarts = new (heap, 4) u16[6]();
    return true;
}

// 0x00524964
void ClearLastNewline()
{
    if (g_pTextManager != NULL) {
        g_pTextManager->m_IsLastNewline = false;
    }
}

// 0x00524980
void Destroy()
{
    if (g_pTextManager != NULL) {
        delete g_pTextManager;
        g_pTextManager = NULL;
    }
    if (g_IsBufferOwned && g_pTextBuffer != NULL) {
        delete[] g_pTextBuffer;
        g_pTextBuffer = NULL;
    }
}

// 0x005249CC
bool LoadMessage()
{
    // the folders of the regions (event::Region; Korea uses the Japanese one); copied from the image 0x898240
    const char* const folders[13] = {
        "JP_Japanese", "US_English", "US_Spanish", "US_French", "EU_English", "EU_Spanish", "EU_French",
        "EU_Italian",  "EU_German",  "JP_Japanese", NULL, NULL, NULL,
    };
    const s32 region = GetRegion();
    g_pMessageStyles = NULL;
    char messagePath[64];
    std::sprintf(messagePath, "Layout/Swkbd/message/%s/swkbd.msbt", folders[region]);
    char stylePath[64];
    std::sprintf(stylePath, "Layout/Swkbd/message/%s/RI.mstl", folders[region]);
    sead::Heap* heap = ssys::ma::lyt::GetAllocator()->m_pHeap;
    if (!g_MessageFile.Read(sead::SafeStringBase<char>(messagePath), heap, 128)) {
        return false;
    }
    if (!g_StyleFile.Read(sead::SafeStringBase<char>(stylePath), heap, 128)) {
        return false;
    }
    const u8* message = g_MessageFile.m_pData;
    g_pMessageStyles = reinterpret_cast<const MessageStyleBlock*>(g_StyleFile.m_pData);
    if (message != NULL && g_pMessage == NULL) {
        g_pMessage = LMS_InitMessage(message);
    }
    return true;
}

// 0x00524B24
void UnloadMessage()
{
    if (g_pMessage != NULL) {
        LMS_CloseMessage(g_pMessage);
        g_pMessage = NULL;
    }
    g_MessageFile.Reset();
    g_StyleFile.Reset();
    g_pMessageStyles = NULL;
}

// 0x00524B6C
const wchar_t* GetMessageText(const char* label)
{
    return static_cast<const wchar_t*>(LMS_GetTextByLabel(g_pMessage, label));
}

} // namespace sys
