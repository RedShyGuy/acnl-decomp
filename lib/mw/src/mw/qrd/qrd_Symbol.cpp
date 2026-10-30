#include <cstring>

#include "mw/qrd/qrd_Internal.h"

namespace mw {
namespace qrd {
namespace detail {

namespace {

// integral image of the adaptive threshold, kept between the attempts of one read
// 0x0097F8CC
s32* s_pIntegralImage;

} // namespace

// 0x0015C62C
s32 CorrectBlocks(Codewords* codewords)
{
    s32 result = 1;
    s32 offset = 0;
    s32 dataNum = 0;

    Yield();
    s_ZeroPolynomial.coefficients = NULL;
    s_OnePolynomial.coefficients = NULL;
    InitializePolynomial(&s_ZeroPolynomial, 0);
    InitializePolynomial(&s_OnePolynomial, 1);

    for (s32 i = 0; i < codewords->blockNum; i++)
    {
        dataNum += codewords->blocks[i].dataNum;
    }
    u8* data = static_cast<u8*>(Allocate(dataNum));
    if (data == NULL)
    {
        return 0;
    }
    *reinterpret_cast<s32*>(data) = 0;

    for (s32 i = 0; i < codewords->blockNum; i++)
    {
        s32 totalNum = codewords->blocks[i].totalNum;
        u8* blockData = codewords->blocks[i].data;
        s32 blockDataNum = codewords->blocks[i].dataNum;
        result = 1;
        s32* work = static_cast<s32*>(Allocate(totalNum * 4));
        if (work != NULL)
        {
            std::memset(work, 0, totalNum * 4);
            for (s32 j = 0; j < totalNum; j++)
            {
                work[j] = blockData[j];
            }
            result = DecodeReedSolomon(work, totalNum, totalNum - blockDataNum);
            if (result == 1)
            {
                for (s32 j = 0; j < blockDataNum; j++)
                {
                    blockData[j] = work[j];
                }
                Free(work);
            }
            else
            {
                Free(work);
                if (result == 0)
                {
                    break;
                }
            }
        }

        for (s32 j = 0; j < codewords->blocks[i].dataNum; j++)
        {
            data[offset + j] = codewords->blocks[i].data[j];
        }
        offset += codewords->blocks[i].dataNum;
    }

    Free(codewords->data);
    codewords->data = data;
    codewords->size = dataNum;
    FinalizePolynomial(&s_OnePolynomial);
    FinalizePolynomial(&s_ZeroPolynomial);
    return result;
}

// 0x0015C908
s32 LocateFinderPatterns(const BitImage* bitImage, FinderPattern** finders, s32* count, SymbolPosition* position)
{
    Yield();
    *finders = s_FinderPatterns;
    s32 result = SearchFinderPatterns(bitImage, finders, count);
    if (result != 0)
    {
        position->points[0].x = s_FinderPatterns[0].x;
        position->points[0].y = s_FinderPatterns[0].y;
        position->points[0].moduleSize = s_FinderPatterns[0].moduleSize;
        position->points[1].x = s_FinderPatterns[1].x;
        position->points[1].y = s_FinderPatterns[1].y;
        position->points[1].moduleSize = s_FinderPatterns[1].moduleSize;
        position->points[2].x = s_FinderPatterns[2].x;
        position->points[2].y = s_FinderPatterns[2].y;
        position->points[2].moduleSize = s_FinderPatterns[2].moduleSize;
        position->isFound = 1;
    }
    return result;
}

// 0x0015C9FC
s32 ExtractData(u8* data, s32 dataSize, s32* version, u8* out, s32* appendIndex, s32* appendCount,
                s32* appendParity)
{
    s32 length = 0;
    s32 segmentNum = ParseSegments(data, dataSize, version, out);
    Yield();
    *appendIndex = 0;
    *appendCount = 0;
    *appendParity = 0;

    for (s32 i = 0; i < segmentNum; i++)
    {
        Segment* segment = &s_pSegments[i];
        switch (segment->mode)
        {
        case MODE_NUMERIC:
        case MODE_ALPHANUMERIC:
        case MODE_8BIT:
        case MODE_KANJI:
            std::memcpy(out + length, segment->data, segment->length);
            length += s_pSegments[i].length;
            break;
        case MODE_STRUCTURED_APPEND:
        {
            u8 header[32] = {};
            std::memcpy(header, segment->data, segment->length);
            *appendIndex = header[0];
            // (the original stores only the low bytes of these two)
            *reinterpret_cast<u8*>(appendCount) = header[1];
            *reinterpret_cast<u8*>(appendParity) = header[2];
            break;
        }
        }
    }

    Segment* segments = s_pSegments;
    for (s32 i = 0; i < segmentNum; i++)
    {
        if (segments[i].data != NULL)
        {
            Free(segments[i].data);
        }
    }
    FreeTop(segments);
    return length;
}

// 0x0015CB88
bool ExtractCodewords(const ModuleMatrix* matrix, const s32* version, const FormatInformation* format,
                      Codewords* codewords)
{
    s32 isUpward = 1;
    s32 count = 0;
    s32 value = 0;
    s32 bitNum = 0;
    s32 size = matrix->size;

    Yield();
    // the modules of the function patterns
    ModuleMatrix reserved;
    reserved.moduleNum = 0;
    reserved.size = 0;
    reserved.modules = NULL;
    reserved.modules = static_cast<u8*>(Allocate(matrix->moduleNum));
    if (reserved.modules == NULL)
    {
        return false;
    }
    std::memset(reserved.modules, 0, matrix->moduleNum);
    reserved.size = matrix->size;
    reserved.moduleNum = matrix->moduleNum;

    ModuleMatrix* mask = &reserved;
    s32 n = reserved.size;
    Yield();
    FillRect(mask, 0, 0, 9, 9);
    FillRect(mask, 0, n - 8, 9, 8);
    FillRect(mask, n - 8, 0, 8, 9);
    s32 alignmentNum = GetAlignmentNum(*version);
    for (s32 i = 0; i < alignmentNum; i++)
    {
        s32 row = GetAlignmentPosition(*version, i) - 2;
        for (s32 j = 0; j < alignmentNum; j++)
        {
            // not where the finder patterns are
            if (i == 0 ? j == 0 || j == alignmentNum - 1 : i == alignmentNum - 1 && j == 0)
            {
                continue;
            }
            FillRect(mask, row, GetAlignmentPosition(*version, j) - 2, 5, 5);
        }
    }
    FillRect(mask, 9, 6, n - 17, 1);
    FillRect(mask, 6, 9, 1, n - 17);
    if (*version > 6)
    {
        FillRect(mask, 0, n - 11, 6, 3);
        FillRect(mask, n - 11, 0, 3, 6);
    }

    s32 total = 0;
    s32 v = *version;
    s32 ecNum = GetErrorCorrectionCodewordNum(v, format->errorCorrectionLevel);
    Yield();
    s32 groupNum = GetBlockGroupNum(v, format->errorCorrectionLevel);
    for (s32 i = 0; i < groupNum; i++)
    {
        const ErrorCorrectionInfo& info = s_ErrorCorrectionInfos[v - 1][GetLevelIndex(format->errorCorrectionLevel)];
        total += info.groups[i].blockNum * (GetDataCodewordNum(v, format->errorCorrectionLevel, i) + ecNum);
    }

    u8* data = static_cast<u8*>(Allocate(total));
    if (data == NULL)
    {
        return false;
    }
    std::memset(data, 0, total);

    // two columns at a time from the right, up and down in turns, the timing column is skipped
    for (s32 column = size - 1; column > 0; column -= 2)
    {
        if (column == 6)
        {
            column = 5;
        }
        for (s32 i = 0; i < size; i++)
        {
            s32 row = isUpward ? size - i - 1 : i;
            for (s32 k = 0; k < 2; k++)
            {
                s32 index = GetModuleIndex(&reserved, row, column - k);
                if (reserved.modules[index] == 0)
                {
                    bitNum++;
                    value <<= 1;
                    if (matrix->modules[index] != 0)
                    {
                        value |= 1;
                    }
                    if (bitNum == 8)
                    {
                        data[count++] = value;
                        value = 0;
                        bitNum = 0;
                    }
                }
            }
        }
        isUpward = isUpward == 0;
    }

    Free(reserved.modules);
    if (count != total)
    {
        return false;
    }
    codewords->data = data;
    codewords->size = count;
    return true;
}

// 0x0015D2A4
bool ReadFormat(const ModuleMatrix* matrix, FormatInformation* format)
{
    s32 bits = 0;
    s32 n = matrix->size;

    Yield();
    // the copy around the top left finder pattern
    for (s32 i = 0; i < 6; i++)
    {
        bits <<= 1;
        if (matrix->modules[8 * matrix->size + i] != 0)
        {
            bits |= 1;
        }
    }
    bits <<= 1;
    if (matrix->modules[8 * matrix->size + 7] != 0)
    {
        bits |= 1;
    }
    bits <<= 1;
    if (matrix->modules[8 * matrix->size + 8] != 0)
    {
        bits |= 1;
    }
    bits <<= 1;
    if (matrix->modules[7 * matrix->size + 8] != 0)
    {
        bits |= 1;
    }
    for (s32 i = 5; i >= 0; i--)
    {
        bits <<= 1;
        if (matrix->modules[i * matrix->size + 8] != 0)
        {
            bits |= 1;
        }
    }
    if (DecodeFormat(bits, &format->maskPattern, &format->errorCorrectionLevel) != 0)
    {
        return true;
    }
    if (DecodeFormat(bits ^ 0x5412, &format->maskPattern, &format->errorCorrectionLevel) != 0)
    {
        return true;
    }

    // the copy at the other two finder patterns
    bits = 0;
    for (s32 i = n - 1; i >= n - 8; i--)
    {
        bits <<= 1;
        if (matrix->modules[i * matrix->size + 8] != 0)
        {
            bits |= 1;
        }
    }
    for (s32 i = n - 7; i < n; i++)
    {
        bits <<= 1;
        if (matrix->modules[8 * matrix->size + i] != 0)
        {
            bits |= 1;
        }
    }
    if (DecodeFormat(bits, &format->maskPattern, &format->errorCorrectionLevel) != 0)
    {
        return true;
    }
    if (DecodeFormat(bits ^ 0x5412, &format->maskPattern, &format->errorCorrectionLevel) != 0)
    {
        return true;
    }
    return false;
}

// 0x0015D488
bool ExtractLuminance(const SourceImage* src, GrayImage* dst, Params* params, const Rect* rect)
{
    bool result = true;

    Yield();
    dst->pixels = s_WorkState.grayPixels;
    if (dst->pixels == NULL)
    {
        return false;
    }
    std::memset(dst->pixels, 0, src->width * src->height);

    u8 darkest = 255;
    u8 lightest = 0;
    u8* out = dst->pixels;
    const u8* in = src->pixels;
    Yield();

    // YUYV: every second byte is a luminance value
    s32 size = src->width * src->height * 2;
    if (rect->top != 0 || rect->left != 0 || rect->right != 0 || rect->bottom != 0)
    {
        for (s32 y = 0; y < rect->bottom - rect->top; y++)
        {
            const u8* row = in + y * src->width * 2 + rect->top * src->width * 2 + rect->left * 2;
            for (s32 x = 0; x < (rect->right - rect->left) * 2; x += 4)
            {
                u8 value = row[x];
                if (value < darkest)
                {
                    darkest = value;
                }
                if (value > lightest)
                {
                    lightest = value;
                }
                *out++ = value;

                value = row[x + 2];
                if (value < darkest)
                {
                    darkest = value;
                }
                if (value > lightest)
                {
                    lightest = value;
                }
                *out++ = value;
            }
        }
    }
    else
    {
        for (s32 i = 0; i < size; i += 4)
        {
            u8 value = in[i];
            if (value < darkest)
            {
                darkest = value;
            }
            if (value > lightest)
            {
                lightest = value;
            }
            *out++ = value;

            value = in[i + 2];
            if (value < darkest)
            {
                darkest = value;
            }
            if (value > lightest)
            {
                lightest = value;
            }
            *out++ = value;
        }
    }
    params->threshold = (darkest + lightest) / 2;

    if (rect->top != 0 || rect->left != 0 || rect->right != 0 || rect->bottom != 0)
    {
        dst->height = rect->bottom - rect->top;
        dst->width = rect->right - rect->left;
    }
    else
    {
        dst->height = src->height;
        dst->width = src->width;
    }
    return result;
}

// 0x0015D738
bool SplitBlocks(const s32* version, const FormatInformation* format, Codewords* codewords)
{
    s32 blockNum = 0;
    s32 index = 0;

    Yield();
    codewords->blockNum = 0;
    codewords->blocks = NULL;
    s32 groupNum = 0;
    BlockGroup groups[2];
    groups[0].blockNum = 0;
    groups[0].dataCodewordNum = 0;
    groups[1].blockNum = 0;
    groups[1].dataCodewordNum = 0;
    s32 level = GetLevelIndex(format->errorCorrectionLevel);
    if (*version <= 0 || *version > 40)
    {
        groupNum = 0;
    }
    else
    {
        const ErrorCorrectionInfo* info = &s_ErrorCorrectionInfos[*version - 1][level];
        if (info->groups[0].blockNum > -1)
        {
            groupNum = 1;
        }
        if (info->groups[1].blockNum > -1)
        {
            groupNum++;
        }
        std::memcpy(groups, info->groups, groupNum * sizeof(BlockGroup));
    }

    for (s32 i = 0; i < groupNum; i++)
    {
        blockNum += groups[i].blockNum;
    }
    Block* blocks = static_cast<Block*>(Allocate(blockNum * sizeof(Block)));
    if (blocks == NULL)
    {
        return false;
    }
    std::memset(blocks, 0, blockNum * sizeof(Block));

    for (s32 i = 0; i < groupNum; i++)
    {
        for (s32 j = 0; j < groups[i].blockNum; j++)
        {
            s32 dataNum = GetDataCodewordNum(*version, format->errorCorrectionLevel, i);
            s32 totalNum = GetErrorCorrectionCodewordNum(*version, format->errorCorrectionLevel) + dataNum;
            blocks[index].dataNum = dataNum;
            blocks[index].totalNum = totalNum;
            blocks[index].data = static_cast<u8*>(Allocate(totalNum));
            if (blocks[index].data == NULL)
            {
                goto failure;
            }
            std::memset(blocks[index].data, 0, totalNum);
            index++;
        }
    }

    {
        // the blocks of the second group are one codeword longer
        s32 last = blockNum - 1;
        for (s32 i = 1; i < blockNum + 1; i++, last--)
        {
            if (blocks[last].totalNum == blocks[0].totalNum)
            {
                break;
            }
            if (blocks[last].totalNum != blocks[0].totalNum + 1)
            {
                goto failure;
            }
        }
        s32 longStart = last + 1;

        // the codewords are interleaved: first all data codewords, then all error correction codewords
        s32 shortDataNum = blocks[0].totalNum - GetErrorCorrectionCodewordNum(*version, format->errorCorrectionLevel);
        s32 pos = 0;
        for (s32 i = 0; i < shortDataNum; i++)
        {
            for (s32 j = 0; j < index; j++)
            {
                blocks[j].data[i] = codewords->data[pos++];
            }
        }
        for (s32 j = longStart; j < index; j++)
        {
            blocks[j].data[shortDataNum] = codewords->data[pos++];
        }
        for (s32 i = shortDataNum; i < blocks[0].totalNum; i++)
        {
            for (s32 j = 0; j < index; j++)
            {
                blocks[j].data[j < longStart ? i : i + 1] = codewords->data[pos++];
            }
        }

        codewords->blockNum = index;
        codewords->blocks = blocks;
        return codewords->size == pos;
    }

failure:
    // (the original stores the number of groups)
    codewords->blockNum = groupNum;
    codewords->blocks = blocks;
    return false;
}

// 0x0015DC68
bool ReadVersion(const ModuleMatrix* matrix, s32* version)
{
    s32 bits = 0;
    s32 estimated = (matrix->size - 17) >> 2;

    Yield();
    if (matrix->size <= 0)
    {
        return false;
    }
    *version = estimated;
    // versions 1 to 6 have no version information
    if (estimated >= 1 && estimated <= 6)
    {
        return true;
    }

    // the copy at the top right finder pattern
    for (s32 row = 5; row >= 0; row--)
    {
        for (s32 column = matrix->size - 9; column >= matrix->size - 11; column--)
        {
            bits <<= 1;
            if (matrix->modules[row * matrix->size + column] != 0)
            {
                bits |= 1;
            }
        }
    }
    if (DecodeVersion(bits) > 0)
    {
        *version = estimated;
        return true;
    }

    // the copy at the bottom left finder pattern
    bits = 0;
    for (s32 column = 5; column >= 0; column--)
    {
        for (s32 row = matrix->size - 9; row >= matrix->size - 11; row--)
        {
            bits <<= 1;
            if (matrix->modules[row * matrix->size + column] != 0)
            {
                bits |= 1;
            }
        }
    }
    if (DecodeVersion(bits) > 0)
    {
        *version = estimated;
        return true;
    }
    return false;
}

// 0x0015DD98
bool Binarize(Params* params, const GrayImage* gray, BitImage* out, SymbolPosition* position)
{
    s32 columnSums[640];

    s32 width = gray->width;
    s32 height = gray->height;
    u32* bits = s_WorkState.bitImage;
    u32 size = static_cast<u32>(width * height + 7) >> 3;
    Yield();
    if (bits == NULL)
    {
        return false;
    }
    std::memset(bits, 0, size);

    if (params->isAdaptive != 0)
    {
        // a pixel is light when it is lighter than the mean of the window around it
        s32 rowNum = height;
        f32 factor = 1.0f - params->thresholdBias;
        params->threshold = 128;
        f32 windowSize = params->windowScale;
        s32 startX = 0;
        if (windowSize == 0.0f && width < 640)
        {
            windowSize = width * (20.0f / 109.0f);
        }
        s32 half = (windowSize != 0.0f ? static_cast<s32>(windowSize) : width / 50) / 2;

        if (s_pIntegralImage == NULL)
        {
            s32* integral = s_WorkState.integralImage;
            s_pIntegralImage = integral;
            Yield();
            std::memset(columnSums, 0, sizeof(columnSums));
            for (s32 i = 0; i < gray->height; i++)
            {
                s32 index = (gray->height - i - 1) * gray->width;
                columnSums[0] += gray->pixels[index];
                integral[index] = columnSums[0];
                for (s32 x = 1; x < gray->width; x++)
                {
                    columnSums[x] += gray->pixels[index + x];
                    integral[index + x] = integral[index + x - 1] + columnSums[x];
                }
            }
        }

        for (s32 y = 0; y < rowNum; y++)
        {
            s32 y0 = y - half;
            if (y0 < 0)
            {
                y0 = 0;
            }
            s32 y1 = y + half;
            if (y1 >= height)
            {
                y1 = height - 1;
            }
            s32 h = y1 - y0;
            s32 index = y * width + startX;
            for (s32 x = startX; x < width; x++, index++)
            {
                s32 x0 = x - half;
                if (x0 < 0)
                {
                    x0 = 0;
                }
                s32 x1 = x + half;
                if (x1 >= width)
                {
                    x1 = width - 1;
                }
                s32 sum = s_pIntegralImage[y0 * width + x1] - s_pIntegralImage[y1 * width + x1] -
                          s_pIntegralImage[y0 * width + x0] + s_pIntegralImage[y1 * width + x0];
                if (gray->pixels[index] * ((x1 - x0) * h) >= static_cast<s32>(sum * factor))
                {
                    bits[index >> 5] |= 1 << (index & 31);
                }
            }
        }
    }
    else
    {
        s32 pixelNum = width * height;
        for (s32 i = 0; i < pixelNum; i++)
        {
            if (gray->pixels[i] > params->threshold)
            {
                bits[i >> 5] |= 1 << (i & 31);
            }
        }
        s_pIntegralImage = NULL;
    }

    out->width = width;
    out->height = height;
    out->bits = bits;
    return true;
}

// 0x0015E1A0
bool Unmask(ModuleMatrix* matrix, const FormatInformation* format)
{
    s32 mask = format->maskPattern;
    Yield();

    switch (mask)
    {
    case 0:
        // (the size is odd, so the parity of the index is the parity of row + column)
        for (s32 i = 0; i < matrix->moduleNum; i++)
        {
            if (i % 2 == 0)
            {
                matrix->modules[i] = ~matrix->modules[i];
            }
        }
        break;
    case 1:
    {
        s32 n = matrix->size;
        Yield();
        for (s32 y = 0; y < n; y++)
        {
            for (s32 x = 0; x < n; x++)
            {
                if (y % 2 == 0)
                {
                    matrix->modules[y * n + x] = ~matrix->modules[y * n + x];
                }
            }
        }
        break;
    }
    case 2:
    {
        s32 n = matrix->size;
        Yield();
        for (s32 x = 0; x < n; x++)
        {
            if (x % 3 == 0)
            {
                for (s32 y = 0; y < n; y++)
                {
                    matrix->modules[y * n + x] = ~matrix->modules[y * n + x];
                }
            }
        }
        break;
    }
    case 3:
    {
        s32 n = matrix->size;
        Yield();
        for (s32 y = 0; y < n; y++)
        {
            for (s32 x = 0; x < n; x++)
            {
                if ((y + x) % 3 == 0)
                {
                    matrix->modules[y * n + x] = ~matrix->modules[y * n + x];
                }
            }
        }
        break;
    }
    case 4:
    {
        s32 n = matrix->size;
        Yield();
        for (s32 x = 0; x < matrix->size; x++)
        {
            for (s32 y = 0; y < matrix->size; y++)
            {
                if ((y / 2 + x / 3) % 2 == 0)
                {
                    matrix->modules[y * n + x] = ~matrix->modules[y * n + x];
                }
            }
        }
        break;
    }
    case 5:
    {
        s32 n = matrix->size;
        Yield();
        for (s32 y = 0; y < matrix->size; y++)
        {
            for (s32 x = 0; x < matrix->size; x++)
            {
                if ((y * x) % 2 + (y * x) % 3 == 0)
                {
                    matrix->modules[y * n + x] = ~matrix->modules[y * n + x];
                }
            }
        }
        break;
    }
    case 6:
    {
        s32 n = matrix->size;
        Yield();
        for (s32 y = 0; y < n; y++)
        {
            for (s32 x = 0; x < n; x++)
            {
                if (((y * x) % 2 + (y * x) % 3) % 2 == 0)
                {
                    matrix->modules[y * n + x] = ~matrix->modules[y * n + x];
                }
            }
        }
        break;
    }
    case 7:
    {
        s32 n = matrix->size;
        for (s32 y = 0; y < n; y++)
        {
            for (s32 x = 0; x < matrix->size; x++)
            {
                if (((y + x) % 2 + (y * x) % 3) % 2 == 0)
                {
                    matrix->modules[n * y + x] = ~matrix->modules[n * y + x];
                }
            }
        }
        break;
    }
    default:
        return false;
    }
    return true;
}

// 0x0015E58C
bool ResetContext(Context* context)
{
    Yield();
    if (context->bitImage.bits != NULL)
    {
        context->bitImage.bits = NULL;
    }
    if (context->finders != NULL)
    {
        context->finders = NULL;
    }
    if (context->codewords.data != NULL)
    {
        Free(context->codewords.data);
        context->codewords.data = NULL;
    }
    if (context->_4C != NULL)
    {
        Free(context->_4C);
        context->_4C = NULL;
    }
    if (context->matrix.modules != NULL)
    {
        Free(context->matrix.modules);
        context->matrix.modules = NULL;
    }
    if (context->codewords.blocks != NULL)
    {
        for (s32 i = 0; i < context->codewords.blockNum; i++)
        {
            if (context->codewords.blocks[i].data != NULL)
            {
                Free(context->codewords.blocks[i].data);
                context->codewords.blocks[i].data = NULL;
            }
        }
        Free(context->codewords.blocks);
        context->codewords.blocks = NULL;
    }
    return true;
}

} // namespace detail
} // namespace qrd
} // namespace mw
