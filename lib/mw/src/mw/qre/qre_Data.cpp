#include <cstdlib>
#include <cstring>

#include "mw/qre/qre_Internal.h"

namespace mw {
namespace qre {
namespace detail {

namespace {

// a Shift-JIS character of the kanji mode (0x8140-0x9FFC, 0xE040-0xEBBF)
inline bool IsKanji(u32 code)
{
    u32 low = code & 0xFF;
    if (code - 0x8140 > 0x1EBC && code - 0xE040 >= 0xB80)
    {
        return false;
    }
    if (low == 0x7F || low == 0xFD || low == 0xFE || low == 0xFF || low <= 0x5F)
    {
        return false;
    }
    return true;
}

inline void SetBit(u8* stream, s32 bit)
{
    stream[bit / 8] |= 1 << (7 - bit % 8);
}

} // namespace

// the mode that fits all of the data
// 0x00152A58
s32 DetectMode(const u8* data, u32 size)
{
    s32 mode;
    u32 i;

    if (size >= 2 && IsKanji(data[0] << 8 | data[1]))
    {
        mode = MODE_KANJI;
        i = 2;
    }
    else
    {
        u8 c = data[0];
        if (c - '0' <= 9u && c - '0' >= 0)
        {
            mode = MODE_NUMERIC;
        }
        else if (GetAlphanumericIndex(c) >= 0)
        {
            mode = MODE_ALPHANUMERIC;
        }
        else
        {
            return MODE_8BIT;
        }
        i = 1;
    }

    for (; i < size; i++)
    {
        switch (mode)
        {
        case MODE_KANJI:
            if (i >= size - 1 || !IsKanji(data[i] << 8 | data[i + 1]))
            {
                return MODE_8BIT;
            }
            i++;
            break;
        case MODE_NUMERIC:
            if (data[i] - '0' <= 9u && data[i] - '0' >= 0)
            {
                break;
            }
            if (GetAlphanumericIndex(data[i]) < 0)
            {
                return MODE_8BIT;
            }
            mode = MODE_ALPHANUMERIC;
            break;
        case MODE_ALPHANUMERIC:
            if (GetAlphanumericIndex(data[i]) < 0)
            {
                return MODE_8BIT;
            }
            break;
        }
    }
    return mode;
}

// 0x00157AE0
DECOMP_NOINLINE s32 GetCharCountBits(s32 version, s32 mode)
{
    switch (mode)
    {
    case MODE_NUMERIC:
        if (version <= 9)
        {
            return 10;
        }
        if (version <= 26)
        {
            return 12;
        }
        if (version <= 40)
        {
            return 14;
        }
        break;
    case MODE_ALPHANUMERIC:
        if (version <= 9)
        {
            return 9;
        }
        if (version <= 26)
        {
            return 11;
        }
        if (version <= 40)
        {
            return 13;
        }
        break;
    case MODE_8BIT:
        if (version <= 9)
        {
            return 8;
        }
        if (version <= 26 || version <= 40)
        {
            return 16;
        }
        break;
    case MODE_KANJI:
        if (version <= 9)
        {
            return 8;
        }
        if (version <= 26)
        {
            return 10;
        }
        if (version <= 40)
        {
            return 12;
        }
        break;
    }
    return -1;
}

// bits of a segment with its mode indicator and character count
// 0x00157B98
s32 GetBitLength(s32 version, s32 mode, s32 length)
{
    switch (mode)
    {
    case MODE_NUMERIC:
    {
        s32 bits = GetCharCountBits(version, mode) + 4 + length / 3 * 10;
        switch (length % 3)
        {
        case 1:
            return bits + 4;
        case 2:
            return bits + 7;
        }
        return bits;
    }
    case MODE_ALPHANUMERIC:
        return GetCharCountBits(version, mode) + 4 + length / 2 * 11 + length % 2 * 6;
    case MODE_8BIT:
        return GetCharCountBits(version, mode) + 4 + length * 8;
    case MODE_KANJI:
        return GetCharCountBits(version, mode) + 4 + length * 13;
    }
    return -1;
}

// 0x0015EBBC
bool ComputeParity(const DataSpec* data, u8* parity)
{
    SetError(ERROR_NONE, true);
    *parity = 0;
    for (s32 i = 0; i < data->size; i++)
    {
        *parity ^= data->data[i];
    }
    return true;
}

// the smallest version that holds the data (or checks the requested one)
// 0x0015EC38
bool ChooseVersion(s32* version, u8* level, u8* mode, DataSpec* data, Part* part)
{
    bool result = false;
    s32 codewordNum;
    u8* stream = NULL;

    SetError(ERROR_NONE, true);
    if (*version != 0)
    {
        result = BuildDataStream(version, level, mode, data, &codewordNum, &stream, part);
        Free(stream);
        stream = NULL;
    }
    else
    {
        s32 maxVersion = 40;
        for (s32 i = 1; i <= maxVersion; i++)
        {
            s32 tryVersion = i;
            stream = NULL;
            result = BuildDataStream(&tryVersion, level, mode, data, &codewordNum, &stream, part);
            Free(stream);
            stream = NULL;
            if (result)
            {
                *version = tryVersion;
                break;
            }
            if (GetError() != ERROR_DATA_TOO_LARGE)
            {
                break;
            }
        }
    }
    return result;
}

// the data codewords: segments with mode indicator and character count, terminator and pad bytes
// 0x0015ED40
bool BuildDataStream(s32* version, u8* level, u8* mode, DataSpec* data, s32* codewordNum, u8** stream, Part* part)
{
    Segment* segments = NULL;
    u16 appendHeader;
    s32 segmentNum;

    SetError(ERROR_NONE, true);
    if (part != NULL)
    {
        s32 index = part->index;
        s32 count = part->count;
        if (static_cast<u32>(count - 2) >= 15 || index < 1 || count < index)
        {
            SetError(ERROR_INVALID_MODE, true);
            return false;
        }
        appendHeader = (index - 1) << 12 | (count - 1) << 8 | part->parity;
    }

    switch (*mode)
    {
    case MODE_NUMERIC:
    case MODE_ALPHANUMERIC:
    case MODE_8BIT:
        segmentNum = 1;
        segments = static_cast<Segment*>(Allocate(sizeof(Segment)));
        segments->mode = *mode;
        segments->start = 0;
        segments->end = data->size - 1;
        segments->count = data->size;
        break;
    case MODE_KANJI:
        if (data->size & 1)
        {
            SetError(ERROR_INVALID_CHARACTER, true);
            return false;
        }
        segmentNum = 1;
        segments = static_cast<Segment*>(Allocate(sizeof(Segment)));
        segments->mode = *mode;
        segments->start = 0;
        segments->end = data->size - 1;
        segments->count = data->size / 2;
        break;
    case MODE_AUTO:
        segmentNum = SplitSegments(*version, data, &segments);
        break;
    default:
        SetError(ERROR_INVALID_MODE, true);
        return false;
    }

    ErrorCorrectionInfo info;
    s32 totalNum;
    if (!GetErrorCorrectionInfo(*version, *level, &info) || !GetTotalCodewordNum(*version, &totalNum))
    {
        SetError(ERROR_INVALID_VERSION, true);
        Free(segments);
        segments = NULL;
        return false;
    }
    s32 dataNum = totalNum - info.ecCodewordNum;
    s_StreamInfo.dataBitNum = dataNum * 8;
    *codewordNum = totalNum;
    *stream = static_cast<u8*>(Allocate(totalNum));
    std::memset(*stream, 0, *codewordNum);

    s32 bit = 0;
    bool isOverflow = false;
    if (part != NULL)
    {
        s32 capacity = s_StreamInfo.dataBitNum;
        if (capacity < 4)
        {
            isOverflow = true;
            goto finish;
        }
        for (s32 i = 0; i < 4; i++, bit++)
        {
            if (s_StreamInfo.appendMode & (1 << (3 - i)))
            {
                SetBit(*stream, bit);
            }
        }
        if (part != NULL)
        {
            if (bit + 16 > capacity)
            {
                isOverflow = true;
                goto finish;
            }
            for (s32 i = 0; i < 16; i++, bit++)
            {
                if (appendHeader & (1 << (15 - i)))
                {
                    SetBit(*stream, bit);
                }
            }
        }
    }

    for (s32 s = 0; s < segmentNum; s++)
    {
        s32 capacity = s_StreamInfo.dataBitNum;
        if (bit + 4 > capacity)
        {
            isOverflow = true;
            break;
        }
        for (s32 i = 0; i < 4; i++, bit++)
        {
            if (segments[s].mode & (1 << (3 - i)))
            {
                SetBit(*stream, bit);
            }
        }
        s32 countBits = GetCharCountBits(*version, segments[s].mode);
        if (bit + countBits > capacity)
        {
            isOverflow = true;
            break;
        }
        for (s32 i = 0; i < countBits; i++, bit++)
        {
            if (segments[s].count & (1 << (countBits - i - 1)))
            {
                SetBit(*stream, bit);
            }
        }
        if (GetBitLength(*version, segments[s].mode, segments[s].count) + bit - countBits - 4 > capacity)
        {
            isOverflow = true;
            break;
        }

        switch (segments[s].mode)
        {
        case MODE_8BIT:
        {
            const u8* src = data->data + segments[s].start;
            for (s32 i = 0; i < segments[s].count * 8; i++, bit++)
            {
                if (src[i / 8] & (1 << (7 - i % 8)))
                {
                    SetBit(*stream, bit);
                }
            }
            break;
        }
        case MODE_NUMERIC:
        {
            // three digits in 10 bits, the rest in 7 or 4 bits
            const u8* src = data->data + segments[s].start;
            s32 rest = segments[s].count % 3;
            s32 groupNum = segments[s].count / 3 + (rest != 0 ? 1 : 0);
            for (s32 g = 0; g < groupNum; g++)
            {
                char digits[4] = {};
                s32 num = g >= groupNum - 1 && rest != 0 ? (rest == 2 ? 2 : 1) : 3;
                std::strncpy(digits, reinterpret_cast<const char*>(src + g * 3), num);
                for (s32 k = 0; k < 3; k++)
                {
                    if (digits[k] == 0)
                    {
                        break;
                    }
                    if (static_cast<u32>(digits[k] - '0') > 9)
                    {
                        bit = -1;
                        goto checkSegment;
                    }
                }
                s32 value = std::atoi(digits);
                s32 bitNum = g >= groupNum - 1 && rest != 0 ? (rest == 2 ? 6 : 3) : 9;
                for (s32 k = bitNum; k >= 0; k--, bit++)
                {
                    if (value & (1 << k))
                    {
                        SetBit(*stream, bit);
                    }
                }
            }
            break;
        }
        case MODE_ALPHANUMERIC:
        {
            // two characters in 11 bits, the rest in 6 bits
            const u8* src = data->data + segments[s].start;
            s32 count = segments[s].count;
            s32 pairNum = count / 2 + (count & 1);
            for (s32 g = 0; g < pairNum; g++)
            {
                s32 value;
                s32 bitNum;
                if (g >= pairNum - 1 && (count & 1))
                {
                    value = GetAlphanumericIndex(src[g * 2]);
                    bitNum = 6;
                }
                else
                {
                    s32 first = GetAlphanumericIndex(src[g * 2]);
                    if (first < 0)
                    {
                        goto invalid;
                    }
                    s32 second = GetAlphanumericIndex(src[g * 2 + 1]);
                    if (second < 0)
                    {
                        goto invalid;
                    }
                    value = first * 45 + second;
                    bitNum = 11;
                }
                if (value < 0)
                {
                    goto invalid;
                }
                for (s32 k = bitNum - 1; k >= 0; k--, bit++)
                {
                    if (value & (1 << k))
                    {
                        SetBit(*stream, bit);
                    }
                }
            }
            break;
        }
        case MODE_KANJI:
        {
            // 13 bits per Shift-JIS character
            const u8* src = data->data + segments[s].start;
            s32 num = segments[s].count * 2 / 2;
            for (s32 g = 0; g < num; g++)
            {
                u32 code = src[g * 2] << 8 | src[g * 2 + 1];
                u32 value;
                if (!IsKanji(code))
                {
                    goto invalid;
                }
                if (code - 0x8140 <= 0x1EBC)
                {
                    value = code - 0x8140;
                }
                else
                {
                    value = code - 0xC140;
                }
                value = (value >> 8) * 0xC0 + (value & 0xFF);
                for (s32 k = 12; k >= 0; k--, bit++)
                {
                    if (value & (1 << k))
                    {
                        SetBit(*stream, bit);
                    }
                }
            }
            break;
        }
        default:
            Free(*stream);
            *stream = NULL;
            Free(segments);
            segments = NULL;
            SetError(ERROR_INVALID_MODE, true);
            return false;
        }

    checkSegment:
        if (bit < 0)
        {
            goto invalid;
        }
    }

    {
        // terminator, byte alignment and pad bytes
        s32 end = bit + 4;
        if (s_StreamInfo.dataBitNum < end)
        {
            end = s_StreamInfo.dataBitNum;
        }
        while (end & 7)
        {
            end++;
        }
        u16 padBytes = s_StreamInfo.padBytes;
        s32 k = 15;
        while (end / 8 + 1 <= dataNum)
        {
            if (padBytes & (1 << k))
            {
                SetBit(*stream, end);
            }
            end++;
            k = k == 0 ? 15 : k - 1;
        }
    }

finish:
    Free(segments);
    segments = NULL;
    if (!isOverflow)
    {
        return true;
    }
    Free(*stream);
    *stream = NULL;
    SetError(ERROR_DATA_TOO_LARGE, true);
    return false;

invalid:
    Free(*stream);
    *stream = NULL;
    Free(segments);
    segments = NULL;
    SetError(ERROR_INVALID_CHARACTER, true);
    return false;
}

// splits the data into runs of one mode each and merges runs where that needs fewer bits
// 0x00176A7C
s32 SplitSegments(s32 version, const DataSpec* data, Segment** out)
{
    s32 num = 0;
    Segment* segments = NULL;
    s32 mode = MODE_8BIT;

    for (s32 i = 0; i < data->size; i++)
    {
        s8 c = data->data[i];
        if (i < data->size - 1 && IsKanji(data->data[i] << 8 | data->data[i + 1]))
        {
            if (num != 0 && mode == MODE_KANJI)
            {
                segments[num - 1].count++;
                segments[num - 1].end += 2;
            }
            else
            {
                num++;
                segments = static_cast<Segment*>(Reallocate(segments, num * sizeof(Segment)));
                segments[num - 1].mode = MODE_KANJI;
                segments[num - 1].start = i;
                segments[num - 1].end = i + 1;
                segments[num - 1].count = 1;
            }
            mode = MODE_KANJI;
            i++;
        }
        else if (c - '0' <= 9u && c - '0' >= 0)
        {
            if (num != 0 && mode == MODE_NUMERIC)
            {
                segments[num - 1].count++;
                segments[num - 1].end++;
            }
            else
            {
                num++;
                segments = static_cast<Segment*>(Reallocate(segments, num * sizeof(Segment)));
                segments[num - 1].start = i;
                segments[num - 1].mode = MODE_NUMERIC;
                segments[num - 1].end = i;
                segments[num - 1].count = 1;
            }
            mode = MODE_NUMERIC;
        }
        else if (GetAlphanumericIndex(c) >= 0)
        {
            if (num != 0 && mode == MODE_ALPHANUMERIC)
            {
                segments[num - 1].count++;
                segments[num - 1].end++;
            }
            else
            {
                num++;
                segments = static_cast<Segment*>(Reallocate(segments, num * sizeof(Segment)));
                segments[num - 1].mode = MODE_ALPHANUMERIC;
                segments[num - 1].start = i;
                segments[num - 1].end = i;
                segments[num - 1].count = 1;
            }
            mode = MODE_ALPHANUMERIC;
        }
        else
        {
            if (num != 0 && mode == MODE_8BIT)
            {
                segments[num - 1].count++;
                segments[num - 1].end++;
            }
            else
            {
                num++;
                segments = static_cast<Segment*>(Reallocate(segments, num * sizeof(Segment)));
                segments[num - 1].mode = MODE_8BIT;
                segments[num - 1].start = i;
                segments[num - 1].end = i;
                segments[num - 1].count = 1;
            }
            mode = MODE_8BIT;
        }
    }

    for (s32 k = 0; k < num - 1; k++)
    {
        Segment* a = &segments[k];
        Segment* b = &segments[k + 1];
        if (a->mode == b->mode)
        {
            a->end = b->end;
            a->count += b->count;
            goto remove;
        }
        {
            s32 bitsA = GetBitLength(version, a->mode, a->count);
            s32 bitsB = GetBitLength(version, b->mode, b->count);
            s32 merged;
            u8 mergedMode = MODE_8BIT;
            if (a->mode == MODE_8BIT && b->mode == MODE_KANJI &&
                bitsA + bitsB > GetBitLength(version, MODE_8BIT, merged = a->count + b->count * 2))
            {
                goto merge;
            }
            if (a->mode == MODE_KANJI && b->mode == MODE_8BIT &&
                bitsA + bitsB > GetBitLength(version, MODE_8BIT, merged = a->count * 2 + b->count))
            {
                goto merge;
            }
            if (a->mode == MODE_ALPHANUMERIC && b->mode == MODE_NUMERIC &&
                bitsA + bitsB > GetBitLength(version, MODE_ALPHANUMERIC, merged = a->count + b->count))
            {
                mergedMode = MODE_ALPHANUMERIC;
                goto merge;
            }
            if (a->mode == MODE_NUMERIC && b->mode == MODE_ALPHANUMERIC &&
                bitsA + bitsB > GetBitLength(version, MODE_ALPHANUMERIC, merged = a->count + b->count))
            {
                mergedMode = MODE_ALPHANUMERIC;
                goto merge;
            }
            if (a->mode == MODE_8BIT && b->mode == MODE_ALPHANUMERIC &&
                bitsA + bitsB > GetBitLength(version, MODE_8BIT, merged = a->count + b->count))
            {
                goto merge;
            }
            if (a->mode == MODE_ALPHANUMERIC && b->mode == MODE_8BIT &&
                bitsA + bitsB > GetBitLength(version, MODE_8BIT, merged = a->count + b->count))
            {
                goto merge;
            }
            if (a->mode == MODE_KANJI && b->mode == MODE_NUMERIC &&
                bitsA + bitsB > GetBitLength(version, MODE_8BIT, merged = a->count * 2 + b->count))
            {
                goto merge;
            }
            if (a->mode == MODE_NUMERIC && b->mode == MODE_KANJI &&
                bitsA + bitsB > GetBitLength(version, MODE_8BIT, merged = a->count + b->count * 2))
            {
                goto merge;
            }
            if (a->mode == MODE_KANJI && b->mode == MODE_ALPHANUMERIC &&
                bitsA + bitsB > GetBitLength(version, MODE_8BIT, merged = a->count * 2 + b->count))
            {
                goto merge;
            }
            if (a->mode == MODE_ALPHANUMERIC && b->mode == MODE_KANJI &&
                bitsA + bitsB > GetBitLength(version, MODE_8BIT, merged = a->count + b->count * 2))
            {
                goto merge;
            }
            if (a->mode == MODE_8BIT && b->mode == MODE_NUMERIC &&
                bitsA + bitsB > GetBitLength(version, MODE_8BIT, merged = a->count + b->count))
            {
                goto merge;
            }
            if (a->mode == MODE_NUMERIC && b->mode == MODE_8BIT &&
                bitsA + bitsB > GetBitLength(version, MODE_8BIT, merged = a->count + b->count))
            {
                goto merge;
            }
            continue;

        merge:
            a->count = merged;
            a->mode = mergedMode;
            a->end = b->end;
        }

    remove:
        for (s32 j = k + 1; j < num - 1; j++)
        {
            segments[j] = segments[j + 1];
        }
        num--;
        k--;
    }
    *out = segments;
    return num;
}

// 0x0016A0EC
DECOMP_NOINLINE s32 GetAlphanumericIndex(s32 c)
{
    for (s32 i = 0; i < 45; i++)
    {
        if (s_AlphanumericChars[i] == c)
        {
            return i;
        }
    }
    return -1;
}

} // namespace detail
} // namespace qre
} // namespace mw
