#include "asr/asr_Language.h"
#include <cstdio>
#include <cstring>

// Japanese (katakana -> romaji -> phones) and Chinese (pinyin of the characters -> sound rules -> phones).
namespace asr {
extern const char* const s_KanaOne[];
extern const char* const s_KanaTwo[];
extern const char* const s_RomajiPhones[];
extern const char* const s_PinyinPhones[];

namespace {
const s32 TEXT_SIZE = 240;
const s32 ROMAJI_SIZE = 320;

// 0x00AF8208
struct KanaTables
{
    s32 oneNum;              // 0x0
    s32 twoNum;              // 0x4
    const char* const* one;  // 0x8
    const char* const* two;  // 0xC
} s_KanaTables;
// 0x00982EE8
s32 s_RomajiPhoneNum;

DECOMP_ALWAYS_INLINE s32 CountStrings(const char* const* table)
{
    s32 num = 0;
    while (table[num][0] != '\0') {
        num++;
    }
    return num;
}

// from Shift-JIS to EUC-JP
// 0x00163DF8
DECOMP_NOINLINE void ShiftJisToEuc(const char* text, char* out)
{
    s32 length = strlen(text);
    for (s32 i = 0; i < length; i++) {
        u32 c = static_cast<u8>(text[i]);
        u32 d = static_cast<u8>(text[i + 1]);
        if (c < 129 || d < 64) {
            *out++ = c;
            continue;
        }
        if (d - 64 <= 94) {
            if (c - 129 <= 30) {
                out[0] = c * 2 - 97;
            } else if (c - 224 <= 15) {
                out[0] = c * 2 - 225;
            }
            out[1] = d <= 127 ? d + 97 : d + 96;
        } else if (d - 159 <= 93) {
            if (c - 129 <= 30) {
                out[0] = c * 2 - 96;
            } else if (c - 224 <= 15) {
                out[0] = c * 2 - 224;
            }
            out[1] = d + 2;
        }
        i++;
        out += 2;
    }
    *out = '\0';
}

// 0x007A2F38
DECOMP_NOINLINE s32 KanaToRomaji(const char* text, char* out)
{
    s32 used = 0;
    *out = '\0';
    s32 length = strlen(text);
    for (s32 i = 0; i < length; i += 2) {
        const KanaTables& tables = s_KanaTables;
        s32 index;
        for (index = 0; index < tables.twoNum; index++) {
            if (strncmp(text + i, tables.two[index], 4) == 0) {
                break;
            }
        }
        if (index < tables.twoNum) {
            const char* romaji = tables.two[index] + 5;
            s32 size = strlen(romaji);
            used += size;
            if (used + 2 >= ROMAJI_SIZE) {
                return 0;
            }
            memcpy(out, romaji, size);
            out[size] = ' ';
            out += size + 1;
            used++;
            *out = '\0';
            i += 2;
            continue;
        }
        for (index = 0; index < tables.oneNum; index++) {
            if (strncmp(text + i, tables.one[index], 2) == 0) {
                break;
            }
        }
        if (index >= tables.oneNum) {
            return 0;
        }
        const char* romaji = tables.one[index] + 3;
        s32 size = strlen(romaji);
        used += size;
        if (used + 2 >= ROMAJI_SIZE) {
            return 0;
        }
        memcpy(out, romaji, size);
        out[size] = ' ';
        out += size + 1;
        *out = '\0';
        used++;
        if (index > 72 && i == 0) {
            return 0;
        }
    }
    return 1;
}

// 0x007C3130
DECOMP_NOINLINE s32 RomajiToPhones(const char* text, u16* out, s32* size)
{
    s32 num = 0;
    s32 position = 0;
    s32 isGood = 1;
    if (strlen(text) != 0) {
        do {
            s32 index;
            for (index = s_RomajiPhoneNum - 1; index >= 0; index--) {
                const char* phone = s_RomajiPhones[index];
                if (strncmp(text + position, phone, strlen(phone)) == 0) {
                    break;
                }
            }
            if (index < 0) {
                isGood = 0;
                break;
            }
            position += strlen(s_RomajiPhones[index]) + 1;
            out[num++] = index;
            if (*size <= num) {
                isGood = 0;
                break;
            }
        } while (strlen(text) > static_cast<u32>(position));
    }
    out[num] = 0;
    out[num + 1] = 0;
    *size = num + 2;
    return isGood;
}

// voiceless vowels: the words are repeated with "i" and "u" between voiceless consonants replaced
// 0x0015ADA8
DECOMP_NOINLINE s32 AddDevoicedVariant(u8* flags, const char* text, u16* out, s32* num)
{
    s32 savedNum = *num;
    s32 end = strlen(text) - 1;
    for (s32 i = 2; i < end; i++) {
        u8 flag = 0;
        if (text[i] == 'i' || text[i] == 'u') {
            s8 left = text[i - 2];
            bool isLeft = left == 'p' || left == 'f' || left == 'h' || left == 'k' || left == 's' || left == 't';
            bool isRight = true;
            if (i != end - 1) {
                s8 right = text[i + 2];
                isRight = right == 'p' || right == 'c' || right == 'f' || right == 'h' || right == 'k' || right == 'q' ||
                          right == 's' || right == 't';
            }
            if (isLeft && isRight) {
                flag = 3;
            }
        }
        flags[i] = flag;
    }
    s32 num2 = end - 2;
    s32 count;
    while (true) {
        count = 0;
        for (s32 i = 0; i < num2; i++) {
            if (flags[i + 2] != 0) {
                count++;
            }
        }
        if (count <= 1) {
            break;
        }
        for (s32 kind = 1; kind < 4 && count > 1; kind++) {
            for (s32 i = 2; i < end && count > 1; i++) {
                if (flags[i] == kind) {
                    count--;
                    flags[i] = 0;
                }
            }
        }
    }
    if (count <= 0) {
        return 1;
    }
    char* variant = const_cast<char*>(text) + ROMAJI_SIZE;
    strcpy(variant, text);
    for (s32 i = 2; i < end; i++) {
        switch (flags[i]) {
        case 1:
            variant[i] = 'o';
            break;
        case 2:
            variant[i] = 'e';
            break;
        case 3:
            variant[i] = ' ';
            break;
        }
    }
    if (count != 1) {
        return 1;
    }
    RomajiToPhones(text, out, num);
    s32 length = strlen(variant);
    for (s32 i = 2; i < length; i++) {
        if (strncmp(variant + i, "  ", 2) == 0) {
            for (s32 repeat = 0; repeat < 2; repeat++) {
                s32 j = i;
                if (variant[j] != '\0') {
                    while (variant[j + 1] != '\0') {
                        variant[j] = variant[j + 1];
                        j++;
                    }
                    variant[j] = '\0';
                }
            }
            length -= 2;
        }
    }
    u16* second = reinterpret_cast<u16*>(flags + 1600);
    RomajiToPhones(variant, second, &savedNum);
    s32 used = *num - 1;
    for (s32 i = 0; i < savedNum; i++) {
        out[used + i] = second[i];
    }
    if (savedNum > 0) {
        used += savedNum;
    }
    *num = used;
    return 2;
}

// 0x007B2D0C
DECOMP_NOINLINE s32 PinyinToPhones(const char* text, u16* out, s32* size)
{
    if (text == NULL) {
        return -2;
    }
    s32 num = 0;
    while (*text != '\0') {
        char phone[8];
        phone[0] = '\0';
        s8 c = *text;
        switch (c) {
        case 'a':
        case 'e':
        case 'h':
        case 'i':
        case 'l':
        case 'm':
        case 'n':
        case 'o':
        case 'u':
            phone[0] = c;
            phone[1] = '\0';
            text++;
            break;
        case 'c':
        case 'k':
        case 'p':
        case 's':
        case 't': {
            phone[0] = c;
            s8 next = text[1];
            text++;
            s32 length = 1;
            if (next == c && text[1] != c) {
                length = 2;
                text++;
                phone[1] = next;
            }
            phone[length] = '\0';
            break;
        }
        case 'C':
            strcpy(phone, "ch");
            text++;
            break;
        case 'G':
            strcpy(phone, "ng");
            text++;
            break;
        case 'K':
            strcpy(phone, "kh");
            text++;
            break;
        case 'P':
            strcpy(phone, "ph");
            text++;
            break;
        case 'T':
            strcpy(phone, "th");
            text++;
            break;
        case 'A':
            strcpy(phone, "ay");
            text++;
            break;
        case 'E':
            strcpy(phone, "ey");
            text++;
            break;
        case 'O':
            strcpy(phone, "oy");
            text++;
            break;
        case 'U':
            strcpy(phone, "uy");
            text++;
            break;
        case 'w':
        case 'y': {
            phone[0] = c;
            s8 next = *++text;
            if (next == '\0') {
                return -2;
            }
            if (next == 'a' || next == 'e' || next == 'o' || next == 'u' || next == 'i') {
                phone[1] = next;
                phone[2] = '\0';
            } else if (next == 'A') {
                phone[1] = '\0';
                strcat(phone, "ay");
            } else if (next == 'E') {
                phone[1] = '\0';
                strcat(phone, "ey");
            } else {
                return -2;
            }
            text++;
            break;
        }
        default:
            return -2;
        }
        s32 index;
        for (index = 0; index < 53; index++) {
            if (strcmp(s_PinyinPhones[index], phone) == 0) {
                break;
            }
        }
        if (index >= 53) {
            return -2;
        }
        if (num + 2 > *size / 2) {
            return -2;
        }
        out[num++] = index;
    }
    out[num] = 0;
    out[num + 1] = 0;
    *size = num * 2 + 4;
    return 0;
}
} // namespace

// 0x007C32CC
s32 ReplaceText(char* text, const char* find, const char* left, const char* right, const char* target, s32 mode)
{
    char leftText[8];
    char rightText[8];
    char pattern[TEXT_SIZE];
    char replacement[TEXT_SIZE];
    char rest[TEXT_SIZE];
    if (text == NULL || find == NULL || left == NULL || right == NULL || target == NULL) {
        printf("input pointer is NULL!\n");
        return -2;
    }
    leftText[0] = '\0';
    rightText[0] = '\0';
    if (strstr(text, find) == NULL) {
        return -1;
    }
    if (strcmp(left, "ANY") == 0 || strcmp(left, "^") == 0) {
        leftText[0] = '\0';
    } else if (strchr(left, '|') != NULL) {
        while (*left != '\0') {
            s32 i = 0;
            while (*left != '\0' && *left != '|') {
                rest[i++] = *left++;
            }
            rest[i] = '\0';
            if (*left == '|') {
                left++;
            }
            if (ReplaceText(text, find, rest, right, target, mode) == -2) {
                return -2;
            }
        }
        return 0;
    } else {
        strcpy(leftText, left);
    }
    if (strcmp(right, "ANY") == 0 || strcmp(right, "$") == 0) {
        rightText[0] = '\0';
    } else if (strchr(right, '|') != NULL) {
        while (*right != '\0') {
            s32 i = 0;
            while (*right != '\0' && *right != '|') {
                rest[i++] = *right++;
            }
            rest[i] = '\0';
            if (*right == '|') {
                right++;
            }
            if (ReplaceText(text, find, left, rest, target, mode) == -2) {
                return -2;
            }
        }
        return 0;
    } else {
        strcpy(rightText, right);
    }
    strcpy(pattern, leftText);
    if (strlen(pattern) + strlen(find) < TEXT_SIZE) {
        strcat(pattern, find);
    }
    if (strlen(pattern) + strlen(rightText) < TEXT_SIZE) {
        strcat(pattern, rightText);
    }
    strcpy(replacement, leftText);
    if (strlen(replacement) + strlen(target) < TEXT_SIZE) {
        strcat(replacement, target);
    }
    if (strlen(replacement) + strlen(rightText) < TEXT_SIZE) {
        strcat(replacement, rightText);
    }
    char* position = strstr(text, pattern);
    if (position == NULL) {
        return -1;
    }
    bool isEdge = false;
    if (strcmp(left, "^") == 0 && position == text) {
        isEdge = true;
    }
    if (strcmp(right, "$") == 0 && position[strlen(replacement)] == '\0') {
        isEdge = true;
    }
    if ((strcmp(left, "^") != 0 && strcmp(right, "$") != 0) || isEdge) {
        strcpy(rest, position + strlen(pattern));
        *position = '\0';
        if (strlen(text) + strlen(replacement) >= TEXT_SIZE) {
            printf("romanization is too long!\n");
            return -2;
        }
        strcat(text, replacement);
        if (strlen(text) + strlen(rest) >= TEXT_SIZE) {
            printf("romanization is too long!\n");
            return -2;
        }
        strcat(text, rest);
    }
    if (mode == REPLACE_ALL) {
        return ReplaceText(position + strlen(target), find, left, right, target, mode);
    }
    return 0;
}

// 0x001588B0
bool LanguageJapaneseInitialize(void*)
{
    s_KanaTables.one = s_KanaOne;
    s_KanaTables.oneNum = CountStrings(s_KanaOne);
    s_KanaTables.two = s_KanaTwo;
    s_KanaTables.twoNum = CountStrings(s_KanaTwo);
    s_RomajiPhoneNum = CountStrings(s_RomajiPhones);
    return true;
}

// 0x0015AC30
s8 LanguageJapaneseConvert(void* tables, const char* text, s32, u16* out, s32* num, s32* size)
{
    JapaneseTablesData* data = static_cast<JapaneseTablesData*>(tables);
    ShiftJisToEuc(text, data->euc);
    s32 result = KanaToRomaji(data->euc, data->romaji);
    if (result != 0) {
        // the long vowel mark repeats the vowel before
        char* p = data->romaji;
        s32 length = strlen(p);
        for (s32 i = 0; i < length; i++) {
            if (p[i] == '-' && i > 1) {
                p[i] = p[i - 2];
            }
        }
        result = RomajiToPhones(data->romaji, out, size);
    }
    *num = 1;
    data->check[0] = '\0';
    for (s32 i = 0; out[i] != 0; i++) {
        if (out[i] >= s_RomajiPhoneNum) {
            out[i] = 0;
            out[i + 1] = 0;
            *size = i + 2;
            break;
        }
        strcat(data->check, s_RomajiPhones[out[i]]);
        strcat(data->check, " ");
    }
    if (strcmp(data->romaji, data->check) != 0) {
        result = 0;
    }
    *num = AddDevoicedVariant(data->flags, data->romaji, out, size);
    *size = U16ListSize(out);
    return result;
}

// 0x001597C8
s8 LanguageChineseConvert(void* tables, const char* text, s32 variants, u16* out, s32* num, s32* size)
{
    const ChineseTables* chinese = static_cast<const ChineseTables*>(tables);
    u16 wide[20];
    if (text == NULL) {
        return 0;
    }
    s32 length = 0;
    const u8* p = reinterpret_cast<const u8*>(text);
    if (*p != 0) {
        do {
            length++;
            p += *p >= 129 ? 1 : 0;
        } while (*++p != 0);
    }
    p = reinterpret_cast<const u8*>(text);
    s32 count = 0;
    if (*p != 0) {
        do {
            if (count >= length) {
                wide[length] = 0;
                return 0;
            }
            if (static_cast<u32>(*p - 129) > 71 || p[1] < 65 || p[1] > 254) {
                return 0;
            }
            wide[count++] = p[1] + (*p << 8);
            p++;
        } while (*++p != 0);
    }
    wide[count] = 0;
    if (U16Length(wide) > 16 || static_cast<u32>(variants) > 4) {
        return 0;
    }
    char pinyin[TEXT_SIZE + 4];
    pinyin[0] = '\0';
    for (const u16* c = wide; *c != 0; c++) {
        u32 low = *c & 0xFF;
        u32 high = *c >> 8;
        u32 first = chinese->ranges[high - 129];
        u32 last = (chinese->ranges[high - 128] - 1) & 0xFFFF;
        if (first > last) {
            return 0;
        }
        s32 entry = -1;
        s32 span = last - first + 1;
        while (true) {
            s32 half = span / 2;
            if (half == 0) {
                if (span == 0 || chinese->entries[first * 4] != low) {
                    return 0;
                }
                entry = first;
                break;
            }
            u32 middle = (first + ((span & 1) ? half : half - 1)) & 0xFFFF;
            s32 order = static_cast<s32>(low) - chinese->entries[middle * 4];
            if (order == 0) {
                entry = middle;
                break;
            }
            if (order < 0) {
                last = (middle - 1) & 0xFFFF;
                span = (span & 1) ? half : half - 1;
            } else {
                first = (middle + 1) & 0xFFFF;
                span = half;
            }
            if (first > last) {
                return 0;
            }
        }
        for (s32 i = 1; i < 4; i++) {
            u8 phone = chinese->entries[entry * 4 + i];
            if (phone == 0) {
                break;
            }
            strcat(pinyin, s_PinyinPhones[phone]);
        }
        strcat(pinyin, ".");
    }
    if (ApplySoundRules(pinyin) != 0) {
        return 0;
    }
    if (size == NULL || out == NULL) {
        return 0;
    }
    if (PinyinToPhones(pinyin, out, size) != 0) {
        return 0;
    }
    if (num != NULL) {
        *num = 1;
    }
    *size = U16ListSize(out);
    return 1;
}
} // namespace asr
