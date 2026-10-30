#include <cstring>

#include "mw/qrd/qrd_Internal.h"

namespace mw {
namespace qrd {
namespace detail {

// 0x00976CB0
Segment* s_pSegments;
// 0x00AE6960
BitSource s_BitSource;

namespace {

// the characters of the alphanumeric mode (the numeric mode takes the first ten)
// 0x008AFB28
const s32 ALPHANUMERIC_CHARS[45] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', ' ', '$', '%', '*', '+', '-', '.', '/', ':',
};

// bits of the character count for versions 1-9, 10-26 and 27-40
// 0x008AFBDC
const s32 COUNT_BITS_NONE[4] = {0, 0, 0, -1};
// 0x008AFBEC
const s32 COUNT_BITS_NUMERIC[4] = {10, 12, 14, -1};
// 0x008AFBFC
const s32 COUNT_BITS_ALPHANUMERIC[4] = {9, 11, 13, -1};
// 0x008AFC0C
const s32 COUNT_BITS_8BIT[4] = {8, 16, 16, -1};
// 0x008AFC1C
const s32 COUNT_BITS_KANJI[4] = {8, 10, 12, -1};

// the segment list grows at the end of the work memory
inline Segment* GrowSegments(s32 segmentNum)
{
    if (segmentNum == 0)
    {
        return static_cast<Segment*>(AllocateTop(sizeof(Segment)));
    }
    return static_cast<Segment*>(AllocateTop((segmentNum + 1) * sizeof(Segment)));
}

} // namespace

// 0x0016D220
s32 ParseSegments(u8* data, s32 dataSize, s32* version, u8* out)
{
    s32 segmentNum = 0;
    u8* buffer = NULL;
    s32 mode;

    Yield();
    s_BitSource.byteOffset = 0;
    s_BitSource.bytes = data;
    s_BitSource.bitOffset = 0;
    s_BitSource.size = dataSize;

    do
    {
        switch (ReadBits(4))
        {
        case MODE_NUMERIC:
            mode = MODE_NUMERIC;
            break;
        case MODE_ALPHANUMERIC:
            mode = MODE_ALPHANUMERIC;
            break;
        case MODE_STRUCTURED_APPEND:
            mode = MODE_STRUCTURED_APPEND;
            break;
        case MODE_8BIT:
            mode = MODE_8BIT;
            break;
        case MODE_KANJI:
            mode = MODE_KANJI;
            break;
        default:
            mode = 0;
            break;
        }
        if (mode == 0)
        {
            continue;
        }

        if (mode == MODE_STRUCTURED_APPEND)
        {
            // symbol position, symbol number and parity
            u8* header = static_cast<u8*>(Allocate(3));
            if (header != NULL)
            {
                header[0] = 0;
                header[1] = 0;
                header[2] = 0;
                header[0] = ReadBits(4);
                header[1] = ReadBits(4);
                buffer = header;
                header[2] = ReadBits(8);
            }
            if (buffer == NULL)
            {
                goto failure;
            }
            Segment* segments = GrowSegments(segmentNum);
            if (segments == NULL)
            {
                goto failure;
            }
            s_pSegments = segments;
            segments[segmentNum].mode = MODE_STRUCTURED_APPEND;
            segments[segmentNum].length = 16;
            segments[segmentNum].data = buffer;
            segmentNum++;
            continue;
        }

        {
            s32 v = *version;
            s32 sizeIndex = v <= 9 ? 0 : (v <= 26 ? 1 : 2);
            const s32* countBits;
            if (mode == MODE_NUMERIC)
            {
                countBits = COUNT_BITS_NUMERIC;
            }
            else if (mode == MODE_ALPHANUMERIC)
            {
                countBits = COUNT_BITS_ALPHANUMERIC;
            }
            else if (mode == MODE_8BIT)
            {
                countBits = COUNT_BITS_8BIT;
            }
            else if (mode == MODE_KANJI)
            {
                countBits = COUNT_BITS_KANJI;
            }
            else
            {
                countBits = COUNT_BITS_NONE;
            }
            s32 count = ReadBits(countBits[sizeIndex]);
            s32 length = count;

            switch (mode)
            {
            case MODE_NUMERIC:
            {
                s32 index = 0;
                u8* text = static_cast<u8*>(Allocate(count));
                Yield();
                if (text != NULL)
                {
                    std::memset(text, 0, count);
                    // three digits in 10 bits, the rest in 7 or 4 bits
                    while (count >= 3)
                    {
                        s32 value = ReadBits(10);
                        if (value >= 1000)
                        {
                            Free(text);
                            goto numericDone;
                        }
                        text[index++] = ALPHANUMERIC_CHARS[value / 100];
                        text[index++] = ALPHANUMERIC_CHARS[(value / 10) % 10];
                        text[index++] = ALPHANUMERIC_CHARS[value % 10];
                        count -= 3;
                    }
                    if (count == 2)
                    {
                        s32 value = ReadBits(7);
                        if (value >= 100)
                        {
                            Free(text);
                            goto numericDone;
                        }
                        text[index++] = ALPHANUMERIC_CHARS[value / 10];
                        text[index] = ALPHANUMERIC_CHARS[value % 10];
                    }
                    else if (count == 1)
                    {
                        s32 value = ReadBits(4);
                        if (value >= 10)
                        {
                            Free(text);
                            goto numericDone;
                        }
                        text[index] = ALPHANUMERIC_CHARS[value];
                    }
                    buffer = text;
                }
            numericDone:
                break;
            }
            case MODE_ALPHANUMERIC:
            {
                s32 index = 0;
                u8* text = static_cast<u8*>(Allocate(count));
                if (text != NULL)
                {
                    std::memset(text, 0, count);
                    // two characters in 11 bits, the rest in 6 bits
                    while (count > 1)
                    {
                        s32 value = ReadBits(11);
                        text[index++] = ALPHANUMERIC_CHARS[value / 45];
                        text[index++] = ALPHANUMERIC_CHARS[value % 45];
                        count -= 2;
                    }
                    if (count == 1)
                    {
                        text[index] = ALPHANUMERIC_CHARS[ReadBits(6)];
                    }
                    buffer = text;
                }
                break;
            }
            case MODE_8BIT:
            {
                u8* text = static_cast<u8*>(Allocate(count));
                if (text != NULL)
                {
                    std::memset(text, 0, count);
                    s32 available = (s_BitSource.size - s_BitSource.byteOffset) * 8 - s_BitSource.bitOffset;
                    if (available < count * 8)
                    {
                        Free(text);
                    }
                    else
                    {
                        for (s32 i = 0; i < count; i++)
                        {
                            text[i] = ReadBits(8);
                        }
                        buffer = text;
                    }
                }
                break;
            }
            case MODE_KANJI:
            {
                s32 index = 0;
                length = count * 2;
                u8* text = static_cast<u8*>(Allocate(length));
                Yield();
                if (text != NULL)
                {
                    std::memset(text, 0, length);
                    // 13 bits per Shift-JIS character
                    for (; count > 0; count--)
                    {
                        s32 value = ReadBits(13);
                        s32 assembled = ((value / 0xC0) << 8) | (value % 0xC0);
                        if (assembled < 0x1F00)
                        {
                            assembled += 0x8140;
                        }
                        else
                        {
                            assembled += 0xC140;
                        }
                        text[index] = assembled >> 8;
                        text[index + 1] = assembled;
                        index += 2;
                    }
                    buffer = text;
                }
                break;
            }
            default:
                goto failure;
            }

            if (buffer == NULL)
            {
                goto failure;
            }
            Segment* segments = GrowSegments(segmentNum);
            if (segments == NULL)
            {
                goto failure;
            }
            s_pSegments = segments;
            segments[segmentNum].mode = mode;
            segments[segmentNum].length = length;
            segments[segmentNum].data = buffer;
            segmentNum++;
            buffer = NULL;
        }
    } while (s_BitSource.byteOffset < dataSize && mode != 0);
    return segmentNum;

failure:
    Segment* segments = s_pSegments;
    for (s32 i = 0; i < segmentNum; i++)
    {
        if (segments[i].data != NULL)
        {
            Free(segments[i].data);
        }
    }
    FreeTop(segments);
    return 0;
}

// 0x00189950
s32 ReadBits(s32 bitNum)
{
    s32 result = 0;
    if (static_cast<u32>(bitNum - 1) >= 32)
    {
        return -1;
    }

    // the rest of the current byte
    if (s_BitSource.bitOffset > 0)
    {
        s32 bitsLeft = 8 - s_BitSource.bitOffset;
        s32 toRead = bitNum < bitsLeft ? bitNum : bitsLeft;
        s32 bitsToNotRead = bitsLeft - toRead;
        s32 mask = (0xFF >> (8 - toRead)) << bitsToNotRead;
        result = (s_BitSource.bytes[s_BitSource.byteOffset] & mask) >> bitsToNotRead;
        bitNum -= toRead;
        s_BitSource.bitOffset += toRead;
        if (s_BitSource.bitOffset == 8)
        {
            s_BitSource.bitOffset = 0;
            s_BitSource.byteOffset++;
        }
    }

    if (bitNum > 0)
    {
        // whole bytes
        s32 byteNum = bitNum / 8;
        for (s32 i = 0; i < byteNum; i++)
        {
            result = (result << 8) | s_BitSource.bytes[s_BitSource.byteOffset];
            s_BitSource.byteOffset++;
            bitNum -= 8;
        }
        // the start of the last byte
        if (bitNum > 0)
        {
            s32 bitsToNotRead = 8 - bitNum;
            s32 mask = (0xFF >> bitsToNotRead) << bitsToNotRead;
            result = (result << bitNum) | ((s_BitSource.bytes[s_BitSource.byteOffset] & mask) >> bitsToNotRead);
            s_BitSource.bitOffset += bitNum;
        }
    }
    return result;
}

} // namespace detail
} // namespace qrd
} // namespace mw
