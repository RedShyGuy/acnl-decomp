#include <cstring>

#include "mw/qre/qre_Internal.h"

// A Reed-Solomon encoder over GF(256) in the classic style with alpha_to/index_of tables, and the BCH codes
// of the format and version information.

namespace mw {
namespace qre {
namespace detail {

// 0x00982EA8
RsCode s_RsCode;
// 0x00AF7560
s32 s_AlphaTo[256];
// 0x00AF7960
s32 s_IndexOf[256];
// 0x00AF7D60
s32 s_Generator[256];

namespace {

// x^8 + x^4 + x^3 + x^2 + 1, lowest power first
// 0x008C3340
const s32 PRIMITIVE_POLYNOMIAL[9] = {1, 0, 1, 1, 1, 0, 0, 0, 1};

// x^10 + x^8 + x^5 + x^4 + x^2 + x + 1
// 0x008C3364
const s8 FORMAT_GENERATOR[11] = {1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 1};
// x^12 + x^11 + x^10 + x^9 + x^8 + x^5 + x^2 + 1
// 0x008C336F
const s8 VERSION_GENERATOR[13] = {1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1};

const s32 A0 = 255;

inline s32 Modnn(s32 x)
{
    while (x >= 255)
    {
        x -= 255;
        x = (x >> 8) + (x & 255);
    }
    return x;
}

} // namespace

// splits the data codewords into the blocks of the level, adds their error correction codewords and
// interleaves everything back into stream
// 0x0015E690
bool AddErrorCorrection(s32 version, u8 level, u8* stream)
{
    ErrorCorrectionInfo info;
    RsBlock* blocks;
    u8 buffer[256];

    SetError(ERROR_NONE, true);
    if (!GetErrorCorrectionInfo(version, level, &info))
    {
        // (the original frees the blocks before allocating them)
        for (s32 i = 0; i < info.groupBlockNum[0] + info.groupBlockNum[1]; i++)
        {
            Free(blocks[i].codewords);
            blocks[i].codewords = NULL;
        }
        Free(blocks);
        SetError(ERROR_INVALID_VERSION, true);
        return false;
    }

    s32 blockNum = info.blockNum;
    blocks = static_cast<RsBlock*>(Allocate(blockNum * sizeof(RsBlock)));
    s32 k = 0;
    for (s32 i = 0; i < info.groupBlockNum[0]; i++, k++)
    {
        s32 totalNum = info.groups[0].totalNum;
        s32 dataNum = info.groups[0].dataNum;
        blocks[k].codewords = static_cast<u8*>(Allocate(totalNum));
        std::memset(blocks[k].codewords, 0, totalNum);
        blocks[k].index = k;
        blocks[k].blockNum = blockNum;
        blocks[k].totalNum = totalNum;
        blocks[k].dataNum = dataNum;
    }
    for (s32 i = 0; i < info.groupBlockNum[1]; i++, k++)
    {
        s32 totalNum = info.groups[1].totalNum;
        s32 dataNum = info.groups[1].dataNum;
        blocks[k].codewords = static_cast<u8*>(Allocate(totalNum));
        std::memset(blocks[k].codewords, 0, totalNum);
        blocks[k].index = k;
        blocks[k].blockNum = blockNum;
        blocks[k].totalNum = totalNum;
        blocks[k].dataNum = dataNum;
    }

    s32 position = 0;
    k = 0;
    for (s32 i = 0; i < info.groupBlockNum[0]; i++, k++)
    {
        for (s32 j = 0; j < blocks[k].dataNum; j++)
        {
            blocks[k].codewords[j] = stream[position + j];
        }
        position += blocks[k].dataNum;
    }
    for (s32 i = 0; i < info.groupBlockNum[1]; i++, k++)
    {
        for (s32 j = 0; j < blocks[k].dataNum; j++)
        {
            blocks[k].codewords[j] = stream[position + j];
        }
        position += blocks[k].dataNum;
    }

    InitializeRs(info.ecCodewordNum / info.blockNum, 0);
    for (s32 b = 0; b < blockNum; b++)
    {
        RsBlock* block = &blocks[b];
        s32 dataNum = block->dataNum;
        s32 totalNum = block->totalNum;
        s32 ecNum = totalNum - dataNum;
        if (ecNum != s_RsCode.rootNum)
        {
            return false;
        }

        // the encoder takes the codewords reversed
        for (s32 j = 0; j < dataNum; j++)
        {
            buffer[j] = block->codewords[dataNum - 1 - j];
        }
        s32 parityStart = 255 - ecNum;
        for (s32 j = dataNum; j < parityStart; j++)
        {
            buffer[j] = 0;
        }
        for (s32 j = 0; j < ecNum; j++)
        {
            buffer[parityStart + j] = block->codewords[totalNum - 1 - j];
        }
        EncodeRs(buffer, buffer + 255 - ecNum);
        for (s32 j = 0; j < dataNum; j++)
        {
            block->codewords[dataNum - 1 - j] = buffer[j];
        }
        for (s32 j = 0; j < ecNum; j++)
        {
            block->codewords[totalNum - 1 - j] = buffer[parityStart + j];
        }
    }

    s32 dataTotal = info.groupBlockNum[0] * info.groups[0].dataNum + info.groupBlockNum[1] * info.groups[1].dataNum;
    s32 total = info.groupBlockNum[0] * info.groups[0].totalNum + info.groupBlockNum[1] * info.groups[1].totalNum;
    s32 i;
    for (i = 0; i < dataTotal; i++)
    {
        s32 index = i / info.blockNum;
        s32 b = i % info.blockNum;
        if (info.blockNum * info.groups[0].dataNum <= i)
        {
            b += info.groupBlockNum[0];
        }
        stream[i] = blocks[b].codewords[index];
    }
    for (s32 n = 0; i < total; i++, n++)
    {
        s32 index = n / info.blockNum;
        s32 b = n % info.blockNum;
        s32 offset = info.groupBlockNum[0] > b ? info.groups[0].dataNum : info.groups[1].dataNum;
        stream[i] = blocks[b].codewords[index + offset];
    }

    for (s32 n = 0; n < info.groupBlockNum[0] + info.groupBlockNum[1]; n++)
    {
        Free(blocks[n].codewords);
        blocks[n].codewords = NULL;
    }
    Free(blocks);
    return true;
}

// the BCH remainder of the format (15, 5) or version (18, 6) information
// 0x007A685C
void EncodeBch(s32 codeLength, s32 dataLength, const s8* data, s8* out)
{
    s8 remainder[16];

    s32 ecLength = codeLength - dataLength;
    const s8* generator = codeLength == 15 ? FORMAT_GENERATOR : VERSION_GENERATOR;
    for (s32 i = 0; i < ecLength; i++)
    {
        remainder[i] = i < dataLength ? data[i] : 0;
    }
    if (ecLength >= 0)
    {
        remainder[ecLength] = ecLength < dataLength ? data[ecLength] : 0;
    }

    for (s32 j = 0; j < dataLength; j++)
    {
        if (remainder[0] != 0)
        {
            for (s32 i = 0; i < ecLength; i++)
            {
                remainder[i] = remainder[i + 1] != generator[i + 1];
            }
        }
        else
        {
            for (s32 i = 0; i < ecLength; i++)
            {
                remainder[i] = remainder[i + 1];
            }
            remainder[ecLength] = 0;
        }
    }

    for (s32 i = 0; i < ecLength; i++)
    {
        out[i] = remainder[i];
    }
}

// the parity bytes of data (255 - rootNum bytes, reversed) into parity
// 0x007A6A98
void EncodeRs(const u8* data, u8* parity)
{
    std::memset(parity, 0, s_RsCode.rootNum);
    for (s32 i = 254 - s_RsCode.rootNum; i >= 0; i--)
    {
        s32 rootNum = s_RsCode.rootNum;
        s32 feedback = s_IndexOf[data[i] ^ parity[rootNum - 1]];
        if (feedback != A0)
        {
            for (s32 j = rootNum - 1; j > 0; j--)
            {
                if (s_Generator[j] != A0)
                {
                    parity[j] = parity[j - 1] ^ s_AlphaTo[Modnn(s_Generator[j] + feedback)];
                }
                else
                {
                    parity[j] = parity[j - 1];
                }
            }
            parity[0] = s_AlphaTo[Modnn(s_Generator[0] + feedback)];
        }
        else
        {
            for (s32 j = rootNum - 1; j > 0; j--)
            {
                parity[j] = parity[j - 1];
            }
            parity[0] = 0;
        }
    }
}

// the GF(256) tables and the generator polynomial with rootNum roots from alpha^firstRoot
// 0x007AE81C
void InitializeRs(s32 rootNum, s32 firstRoot)
{
    s_RsCode.rootNum = rootNum;
    s_RsCode.firstRoot = firstRoot;

    s32 mask = 1;
    s_AlphaTo[8] = 0;
    for (s32 i = 0; i < 8; i++)
    {
        s_AlphaTo[i] = mask;
        s_IndexOf[s_AlphaTo[i]] = i;
        if (PRIMITIVE_POLYNOMIAL[i] != 0)
        {
            s_AlphaTo[8] ^= mask;
        }
        mask <<= 1;
    }
    s_IndexOf[s_AlphaTo[8]] = 8;
    mask >>= 1;
    for (s32 i = 9; i < 255; i++)
    {
        if (s_AlphaTo[i - 1] >= mask)
        {
            s_AlphaTo[i] = s_AlphaTo[8] ^ ((s_AlphaTo[i - 1] ^ mask) << 1);
        }
        else
        {
            s_AlphaTo[i] = s_AlphaTo[i - 1] << 1;
        }
        s_IndexOf[s_AlphaTo[i]] = i;
    }
    s_IndexOf[0] = A0;
    s_AlphaTo[255] = 0;

    s_Generator[0] = s_AlphaTo[firstRoot];
    s_Generator[1] = 1;
    for (s32 i = 2; i <= s_RsCode.rootNum; i++)
    {
        s_Generator[i] = 1;
        for (s32 j = i - 1; j > 0; j--)
        {
            if (s_Generator[j] != 0)
            {
                s_Generator[j] =
                    s_Generator[j - 1] ^ s_AlphaTo[Modnn(s_IndexOf[s_Generator[j]] + firstRoot + i - 1)];
            }
            else
            {
                s_Generator[j] = s_Generator[j - 1];
            }
        }
        s_Generator[0] = s_AlphaTo[Modnn(s_IndexOf[s_Generator[0]] + firstRoot + i - 1)];
    }
    for (s32 i = 0; i <= s_RsCode.rootNum; i++)
    {
        s_Generator[i] = s_IndexOf[s_Generator[i]];
    }
}

} // namespace detail
} // namespace qre
} // namespace mw
