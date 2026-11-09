#include "script/dNumberFormat.h"
#include "Other/dRegion.h"
#include "sead/seadWFixedSafeString.h"
#include "script/dWordCPtr.h"
#include "script/dWordFix.h"
#include "script/dWordRes.h"
#include <cmath>

namespace script {
// 0x00AD69AC
char16 NumberFormat::s_Buffer[NumberFormat::BUFFER_SIZE];

namespace {
// the thousands separators
// 0x0089171E
extern const char16 s_Separators[3] = {',', '.', ' '};
} // namespace

// 0x005F7614
void NumberFormat::Format(IWord* pDst, s32 value, s32 width)
{
    Format(pDst, value, width, PAD_BY_REGION, GetRegion() == 8 ? SEPARATOR_PERIOD : SEPARATOR_BY_REGION);
}

// 0x005F7664
void NumberFormat::FormatDecimal(IWord* pDst, f32 value, s32 decimals)
{
    f32 integer;
    f32 fraction = std::modf(value, &integer);
    s32 integerValue = integer;
    for (s32 i = 0; i < decimals; i++) {
        fraction *= 10.0f;
    }
    f32 fractionInteger;
    std::modf(fraction, &fractionInteger);
    fractionInteger = std::fabs(fractionInteger);
    s32 fractionValue = fractionInteger;
    Format(pDst, integerValue, 10, PAD_NONE, SEPARATOR_BY_REGION);

    const char16* point;
    s32 region = GetRegion();
    if (region == 2 || region == 5 || region == 3 || region == 6 || region == 7 || region == 8) {
        point = u",";
    } else {
        point = u".";
    }
    WordRes pointWord(point);
    pDst->Append(pointWord);
    WordFix<4> fractionWord;
    Format(&fractionWord, fractionValue, decimals, PAD_ZERO_RIGHT, SEPARATOR_BY_REGION);
    pDst->Append(fractionWord);
}

// 0x005F77EC
void NumberFormat::FormatHex(IWord* pDst, u64 value)
{
    sead::WFixedSafeString<32> string;
    string.format(L"%04X-%04X-%04X", static_cast<u16>(value >> 32), static_cast<u16>(value >> 16), static_cast<u16>(value));
    WordCPtr word(reinterpret_cast<const char16*>(string.getCString()), 32);
    pDst->Copy(word);
}

inline s32 NumberFormat::GetLength()
{
    s32 length = BUFFER_SIZE - 1;
    for (s32 i = 0; i < static_cast<s32>(BUFFER_SIZE); i++) {
        if (s_Buffer[i] == WORD_NULL_CHAR) {
            length = i;
            break;
        }
    }
    return length;
}

// 0x005F789C
void NumberFormat::PadLeft(s32 width, char16 c)
{
    s32 count = width - GetLength();
    if (count <= 0) {
        return;
    }
    for (s32 i = BUFFER_SIZE - 1 - count; i >= 0; i--) {
        s_Buffer[i + count] = s_Buffer[i];
    }
    for (s32 i = count - 1; i >= 0; i--) {
        s_Buffer[i] = c;
    }
}

// 0x005F7980
void NumberFormat::Format(IWord* pDst, s32 value, s32 width, u8 padMode, u8 separator)
{
    m_Word.Clear();
    if (static_cast<u32>(width - 1) <= 9) {
        if (separator == SEPARATOR_BY_REGION) {
            s32 region = GetRegion();
            if (region == 2 || region == 5 || region == 3 || region == 6 || region == 8) {
                separator = SEPARATOR_SPACE;
            } else if (region == 7) {
                separator = SEPARATOR_PERIOD;
            } else {
                separator = SEPARATOR_COMMA;
            }
        }
        if (padMode == PAD_BY_REGION) {
            u32 region = GetRegionIndex();
            padMode = region == 1 || region == 2 ? PAD_NONE_SEPARATED : PAD_NONE;
        }
        if (width < 4 || static_cast<u32>(value + 9999) < 19999) {
            // numbers of four digits get no separator in Spanish and Italian
            s32 region = GetRegion();
            if (region == 2 || region == 5 || region == 7) {
                if (padMode == PAD_NONE_SEPARATED) {
                    padMode = PAD_NONE;
                } else if (padMode == PAD_SPACE_RIGHT_SEPARATED) {
                    padMode = PAD_SPACE_RIGHT;
                } else if (padMode == PAD_SPACE_LEFT_SEPARATED) {
                    padMode = PAD_SPACE_LEFT;
                } else if (padMode == PAD_ZERO_LEFT_SEPARATED) {
                    padMode = PAD_ZERO_LEFT;
                }
            }
        }
        m_String.format(L"%d", value);

        if (padMode == PAD_SPACE_RIGHT || padMode == PAD_SPACE_RIGHT_SEPARATED) {
            s32 length = GetLength();
            for (s32 i = 0; i < width - length; i++) {
                s_Buffer[length + i] = ' ';
            }
        } else if (padMode == PAD_SPACE_LEFT || padMode == PAD_SPACE_LEFT_SEPARATED) {
            PadLeft(width, ' ');
        } else if (padMode == PAD_ZERO_LEFT || padMode == PAD_ZERO_LEFT_SEPARATED) {
            PadLeft(width, '0');
        } else if (padMode == PAD_ZERO_RIGHT) {
            s32 length = GetLength();
            for (s32 i = 0; i < width - length; i++) {
                s_Buffer[length + i] = '0';
            }
        }

        if (padMode == PAD_NONE_SEPARATED || padMode == PAD_SPACE_RIGHT_SEPARATED || padMode == PAD_SPACE_LEFT_SEPARATED ||
            padMode == PAD_ZERO_LEFT_SEPARATED) {
            char16 c = s_Separators[separator];
            s32 digits = 0;
            for (s32 i = GetLength() - 1; i >= 0; i--) {
                if (static_cast<u32>(s_Buffer[i] - '0') > 9) {
                    continue;
                }
                digits++;
                if (digits <= 3) {
                    continue;
                }
                // a separator behind the digit
                s32 pos = i + 1;
                digits = 1;
                if (pos >= 0) {
                    for (s32 j = BUFFER_SIZE - 2; j >= pos; j--) {
                        s_Buffer[j + 1] = s_Buffer[j];
                    }
                }
                s_Buffer[pos] = c;
            }
        }
    }
    pDst->Copy(m_Word);
}

// 0x005F7D0C
NumberFormat::NumberFormat() : m_String(reinterpret_cast<wchar_t*>(s_Buffer), BUFFER_SIZE), m_Word(s_Buffer, BUFFER_SIZE)
{
}
} // namespace script
