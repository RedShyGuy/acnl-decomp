#include <cstring>

#include "mw/qre/qre_Internal.h"

namespace mw {
namespace qre {
namespace detail {

namespace {

// places the next bit of stream at a module that is not reserved; the row counts without the timing row 6
inline void PlaceBit(u8** matrix, s32 row, s32 column, const u8* stream, s32* byteIndex, s16* bitIndex)
{
    u8* module = &matrix[row > 6 ? row : row - 1][column];
    if (*module != MODULE_RESERVED)
    {
        *module = (stream[*byteIndex] >> *bitIndex) & 1 ? 0xFF : 0;
        (*bitIndex)--;
    }
    if (*bitIndex == -1)
    {
        *bitIndex = 7;
        (*byteIndex)++;
    }
}

// inverts the modules of the mask pattern that are not reserved
inline void ApplyMask(u8** matrix, s32 size, s8 mask)
{
    for (s32 row = 0; row < size; row++)
    {
        for (s32 column = 0; column < size; column++)
        {
            s32 value;
            switch (mask)
            {
            case 0:
                value = (row + column) % 2;
                break;
            case 1:
                value = row % 2;
                break;
            case 2:
                value = column % 3;
                break;
            case 3:
                value = (row + column) % 3;
                break;
            case 4:
                value = (column / 3 + row / 2) % 2;
                break;
            case 5:
                value = (row * column) % 3 + (row * column) % 2;
                break;
            case 6:
                value = ((row * column) % 3 + (row * column) % 2) % 2;
                break;
            case 7:
                value = ((row * column) % 3 + (row + column) % 2) % 2;
                break;
            default:
                continue;
            }
            if (value == 0 && matrix[row][column] != MODULE_RESERVED)
            {
                matrix[row][column] ^= 0xFF;
            }
        }
    }
}

// penalty points for runs of five and more modules of one color
inline s32 PenaltyRuns(s32** runs, s32 size, s32 points)
{
    for (s32 i = 0; i < size; i++)
    {
        s32* line = runs[i];
        for (s32 k = 0; k < line[0]; k++)
        {
            s32 length = line[1 + k] < 0 ? -line[1 + k] : line[1 + k];
            if (length >= 5)
            {
                points += length - 2;
            }
        }
    }
    return points;
}

// penalty points for dark, light, 3 dark, light, dark runs
inline s32 PenaltyFinderLike(s32** runs, s32 size, s32 points)
{
    for (s32 i = 0; i < size; i++)
    {
        s32* line = runs[i];
        s32 count = line[0];
        for (s32 k = 1; k < count - 3; k++)
        {
            s32 run = line[k];
            if (run < 0 && run == line[k + 4] && run + line[k + 1] == 0 && run + line[k + 3] == 0 &&
                run * 3 == line[k + 2] && (k + 5 > count || line[k + 4] != line[k + 5]))
            {
                points += 40;
            }
        }
    }
    return points;
}

} // namespace

// the module matrix with the function patterns reserved and the codewords placed in the zigzag order
// 0x0015FC40
bool PlaceCodewords(s32 version, s32 codewordNum, const u8* stream, u8*** out)
{
    s32 byteIndex = 0;
    s16 bitIndex = 7;
    s32 size = 0;

    SetError(ERROR_NONE, true);
    if (!GetSymbolSize(version, &size))
    {
        return false;
    }
    *out = static_cast<u8**>(Allocate(size * sizeof(u8*)));
    for (s32 i = 0; i < size; i++)
    {
        (*out)[i] = static_cast<u8*>(Allocate(size));
        std::memset((*out)[i], 0, size);
    }

    s32 n;
    if (!GetSymbolSize(version, &n))
    {
        SetError(ERROR_INVALID_VERSION, true);
        return false;
    }

    // finder patterns and separators
    for (s32 i = 0; i < 8; i++)
    {
        std::memset((*out)[i], MODULE_RESERVED, 8);
    }
    for (s32 i = 0; i < 8; i++)
    {
        std::memset((*out)[i] + n - 8, MODULE_RESERVED, 8);
    }
    for (s32 i = 0; i < 8; i++)
    {
        std::memset((*out)[n - 1 - i], MODULE_RESERVED, 8);
    }

    {
        // alignment patterns
        s32 alignmentNum = 0;
        s32 x[46];
        s32 y[46];
        if (!GetAlignmentPositions(version, &alignmentNum, x, y))
        {
            SetError(ERROR_INVALID_VERSION, true);
        }
        else
        {
            for (s32 k = 0; k < alignmentNum; k++)
            {
                for (s32 row = y[k] - 2; row < y[k] + 3; row++)
                {
                    std::memset((*out)[row] + x[k] - 2, MODULE_RESERVED, 5);
                }
            }
        }
    }

    // version information
    if (version > 6)
    {
        for (s32 i = 0; i < 6; i++)
        {
            (*out)[i][n - 9] = MODULE_RESERVED;
            (*out)[i][n - 10] = MODULE_RESERVED;
            (*out)[i][n - 11] = MODULE_RESERVED;
        }
        std::memset((*out)[n - 9], MODULE_RESERVED, 6);
        std::memset((*out)[n - 10], MODULE_RESERVED, 6);
        std::memset((*out)[n - 11], MODULE_RESERVED, 6);
    }

    // format information and timing patterns
    for (s32 i = 0; i < 8; i++)
    {
        (*out)[i][8] = MODULE_RESERVED;
    }
    std::memset((*out)[8], MODULE_RESERVED, 9);
    for (s32 i = 0; i < 8; i++)
    {
        (*out)[n - 1 - i][8] = MODULE_RESERVED;
        (*out)[8][n - 1 - i] = MODULE_RESERVED;
    }
    for (s32 i = 0; i < n; i++)
    {
        (*out)[i][6] = MODULE_RESERVED;
        (*out)[6][i] = MODULE_RESERVED;
    }

    // two columns at a time from the right, up and down in turns
    // (the column pairs count without the timing column 6 too)
    bool isUpward = true;
    for (s32 column = size - 1; column >= 1; column -= 2)
    {
        s32 right = column > 6 ? column : column - 1;
        s32 left = column > 6 ? column - 1 : column - 2;
        if (isUpward)
        {
            for (s32 row = size - 1; row >= 1; row--)
            {
                if (byteIndex == codewordNum)
                {
                    break;
                }
                PlaceBit(*out, row, right, stream, &byteIndex, &bitIndex);
                if (byteIndex == codewordNum)
                {
                    break;
                }
                PlaceBit(*out, row, left, stream, &byteIndex, &bitIndex);
            }
        }
        else
        {
            for (s32 row = 1; row < size; row++)
            {
                if (byteIndex == codewordNum)
                {
                    break;
                }
                PlaceBit(*out, row, right, stream, &byteIndex, &bitIndex);
                if (byteIndex == codewordNum)
                {
                    break;
                }
                PlaceBit(*out, row, left, stream, &byteIndex, &bitIndex);
            }
        }
        isUpward = !isUpward;
    }
    return true;
}

// writes the format information and (from version 7) the version information
// 0x0015F81C
bool WriteFormatInformation(s32 version, Format format, u8** matrix)
{
    // 0x5412, lowest index = highest bit
    const s8 FORMAT_MASK[16] = {1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0}; // 0x0015FBD8
    s32 size = 0;
    s8 formatBits[16] = {};
    s8 versionBits[20] = {};

    SetError(ERROR_NONE, true);
    if (!GetSymbolSize(version, &size))
    {
        SetError(ERROR_INVALID_VERSION, true);
        return false;
    }

    switch (format.level)
    {
    case 0:
        formatBits[0] = 0;
        formatBits[1] = 0;
        break;
    case 1:
        formatBits[0] = 0;
        formatBits[1] = 1;
        break;
    case 2:
        formatBits[0] = 1;
        formatBits[1] = 0;
        break;
    case 3:
        formatBits[0] = 1;
        formatBits[1] = 1;
        break;
    default:
        SetError(ERROR_INVALID_MODE, true);
        return false;
    }
    switch (static_cast<s8>(format.maskPattern))
    {
    case 0:
        formatBits[2] = 0;
        formatBits[3] = 0;
        formatBits[4] = 0;
        break;
    case 1:
        formatBits[2] = 0;
        formatBits[3] = 0;
        formatBits[4] = 1;
        break;
    case 2:
        formatBits[2] = 0;
        formatBits[3] = 1;
        formatBits[4] = 0;
        break;
    case 3:
        formatBits[2] = 0;
        formatBits[3] = 1;
        formatBits[4] = 1;
        break;
    case 4:
        formatBits[2] = 1;
        formatBits[3] = 0;
        formatBits[4] = 0;
        break;
    case 5:
        formatBits[2] = 1;
        formatBits[3] = 0;
        formatBits[4] = 1;
        break;
    case 6:
        formatBits[2] = 1;
        formatBits[3] = 1;
        formatBits[4] = 0;
        break;
    case 7:
        formatBits[2] = 1;
        formatBits[3] = 1;
        formatBits[4] = 1;
        break;
    default:
        SetError(ERROR_INVALID_MODE, true);
        return false;
    }
    EncodeBch(15, 5, formatBits, formatBits + 5);
    for (s32 i = 0; i < 15; i++)
    {
        formatBits[i] = formatBits[i] != FORMAT_MASK[i] ? 0xFF : 0;
    }

    if (version >= 7)
    {
        for (s32 i = 0; i < 6; i++)
        {
            versionBits[i] = (version & (32 >> i)) ? 1 : 0;
        }
        EncodeBch(18, 6, versionBits, versionBits + 6);
        for (s32 i = 0; i < 18; i++)
        {
            versionBits[i] = versionBits[i] == 1 ? 0xFF : 0;
        }
    }

    // around the top left finder pattern
    s32 k = 0;
    for (s32 column = 0; column <= 8; column++)
    {
        if (column != 6)
        {
            matrix[8][column] = formatBits[k++];
        }
    }
    for (s32 i = 0; i < 8; i++)
    {
        if (i != 1)
        {
            matrix[7 - i][8] = formatBits[k++];
        }
    }
    // the second copy at the other two finder patterns and the dark module
    for (s32 i = 0; i < 7; i++)
    {
        matrix[size - 1 - i][8] = formatBits[i];
    }
    matrix[size - 8][8] = 0xFF;
    std::memcpy(matrix[8] + size - 8, formatBits + 7, 8);

    if (version >= 7)
    {
        for (s32 i = 0; i < 6; i++)
        {
            matrix[i][size - 11] = versionBits[17 - i * 3];
            matrix[i][size - 10] = versionBits[16 - i * 3];
            matrix[i][size - 9] = versionBits[15 - i * 3];
        }
        for (s32 i = 0; i < 6; i++)
        {
            matrix[size - 11][i] = versionBits[17 - i * 3];
            matrix[size - 10][i] = versionBits[16 - i * 3];
            matrix[size - 9][i] = versionBits[15 - i * 3];
        }
    }
    return true;
}

// tries all mask patterns (or applies the requested one) and keeps the one with the fewest penalty points
// 0x00160410
bool SelectMask(s32 version, Format* format, u8 level, u8** matrix)
{
    s32 size = 0;
    s32 penalties[8] = {};

    SetError(ERROR_NONE, true);
    if (!GetSymbolSize(version, &size))
    {
        SetError(ERROR_INVALID_VERSION, true);
        return false;
    }

    if (format->maskPattern == 0xFF)
    {
        s32** rowRuns = static_cast<s32**>(Allocate(size * sizeof(s32*)));
        s32** columnRuns = static_cast<s32**>(Allocate(size * sizeof(s32*)));
        u8** work = static_cast<u8**>(Allocate(size * sizeof(u8*)));
        for (s32 i = 0; i < size; i++)
        {
            work[i] = static_cast<u8*>(Allocate(size));
            std::memset(work[i], 0, size);
            rowRuns[i] = static_cast<s32*>(Allocate(size * sizeof(s32) + sizeof(s32)));
            columnRuns[i] = static_cast<s32*>(Allocate(size * sizeof(s32) + sizeof(s32)));
            std::memset(rowRuns[i], 0, size * sizeof(s32) + sizeof(s32));
            std::memset(columnRuns[i], 0, size * sizeof(s32) + sizeof(s32));
        }

        for (s32 mask = 0; mask < 8; mask++)
        {
            format->maskPattern = mask;
            for (s32 i = 0; i < size; i++)
            {
                for (s32 j = 0; j < size; j++)
                {
                    work[i][j] = matrix[i][j];
                }
            }
            ApplyMask(work, size, format->maskPattern);
            if (!DrawFunctionPatterns(version, work))
            {
                return false;
            }
            for (s32 i = 0; i < size; i++)
            {
                for (s32 j = 0; j < size; j++)
                {
                    if (work[i][j] == MODULE_RESERVED)
                    {
                        work[i][j] = 0;
                    }
                }
            }
            Format trial;
            trial.maskPattern = mask;
            trial.level = level;
            WriteFormatInformation(version, trial, work);

            for (s32 i = 0; i < size; i++)
            {
                std::memset(rowRuns[i], 0, size * sizeof(s32) + sizeof(s32));
                std::memset(columnRuns[i], 0, size * sizeof(s32) + sizeof(s32));
            }
            ComputeRuns(rowRuns, size, work, false);
            ComputeRuns(columnRuns, size, work, true);

            s32 runPoints = PenaltyRuns(columnRuns, size, PenaltyRuns(rowRuns, size, 0));

            s32 blockPoints = 0;
            for (s32 i = 0; i < size; i++)
            {
                for (s32 j = 0; j < size; j++)
                {
                    if (i <= size - 2 && j <= size - 2 && work[i][j] == work[i + 1][j] &&
                        work[i][j] == work[i][j + 1] && work[i][j] == work[i + 1][j + 1])
                    {
                        blockPoints += 3;
                    }
                }
            }

            s32 finderPoints = PenaltyFinderLike(columnRuns, size, PenaltyFinderLike(rowRuns, size, 0));

            // the proportion of dark modules in 5 % steps away from 50 %
            s32 darkNum = 0;
            for (s32 i = 0; i < size; i++)
            {
                for (s32 j = 0; j < size; j++)
                {
                    if (work[i][j] == 0xFF)
                    {
                        darkNum++;
                    }
                }
            }
            f64 ratio = static_cast<f64>(darkNum) / (size * size) * 100.0;
            s32 step;
            if (45.0 < ratio && ratio < 55.0)
            {
                step = 0;
            }
            else if ((ratio >= 55.0 && ratio < 60.0) || (ratio <= 45.0 && ratio > 40.0))
            {
                step = 1;
            }
            else if ((ratio >= 60.0 && ratio < 65.0) || (ratio <= 40.0 && ratio > 35.0))
            {
                step = 2;
            }
            else if ((ratio >= 65.0 && ratio < 70.0) || (ratio <= 35.0 && ratio > 30.0))
            {
                step = 3;
            }
            else if ((ratio >= 70.0 && ratio < 75.0) || (ratio <= 30.0 && ratio > 25.0))
            {
                step = 4;
            }
            else if ((ratio >= 75.0 && ratio < 80.0) || (ratio <= 25.0 && ratio > 20.0))
            {
                step = 5;
            }
            else if ((ratio >= 80.0 && ratio < 85.0) || (ratio <= 20.0 && ratio > 15.0))
            {
                step = 6;
            }
            else if ((ratio >= 85.0 && ratio < 90.0) || (ratio <= 15.0 && ratio > 10.0))
            {
                step = 7;
            }
            else if ((ratio >= 90.0 && ratio < 95.0) || (ratio <= 10.0 && ratio > 5.0))
            {
                step = 8;
            }
            else if ((ratio >= 95.0 && ratio < 100.0) || (ratio <= 5.0 && ratio > 0.0))
            {
                step = 9;
            }
            else
            {
                step = 10;
            }
            penalties[mask] = runPoints + blockPoints + finderPoints + step * 10;
        }

        s32 best = penalties[0];
        format->maskPattern = 0;
        for (s32 mask = 1; mask < 8; mask++)
        {
            if (best > penalties[mask])
            {
                best = penalties[mask];
                format->maskPattern = mask;
            }
        }

        for (s32 i = 0; i < size; i++)
        {
            Free(columnRuns[i]);
            Free(rowRuns[i]);
            Free(work[i]);
        }
        Free(columnRuns);
        Free(rowRuns);
        Free(work);
    }

    ApplyMask(matrix, size, format->maskPattern);
    return DrawFunctionPatterns(version, matrix) != 0;
}

// a finder pattern with its top left corner at (column, row)
// 0x00166A00
bool DrawFinderPattern(u8** matrix, s32 column, s32 row)
{
    for (s32 i = 0; i < 7; i++)
    {
        matrix[row][column + i] = 0xFF;
        matrix[row + 6][column + i] = 0xFF;
        matrix[row + i][column] = 0xFF;
        matrix[row + i][column + 6] = 0xFF;
    }
    for (s32 i = 0; i < 5; i++)
    {
        matrix[row + 1][column + 1 + i] = 0;
        matrix[row + 5][column + 1 + i] = 0;
        matrix[row + 1 + i][column + 1] = 0;
        matrix[row + 1 + i][column + 5] = 0;
    }
    for (s32 c = 2; c <= 4; c++)
    {
        for (s32 r = 2; r <= 4; r++)
        {
            matrix[row + r][column + c] = 0xFF;
        }
    }
    return true;
}

// the runs of one color along the rows (or columns): runs[line][0] = number, then the lengths (negative for
// dark runs)
// 0x0016DC58
bool ComputeRuns(s32** runs, s32 size, u8** matrix, bool isVertical)
{
    for (s32 line = 0; line < size; line++)
    {
        u8 previous = 0;
        s8 run = 0;
        for (s32 k = 0; k < size; k++)
        {
            s32 row = isVertical ? k : line;
            s32 column = isVertical ? line : k;
            u8 value = matrix[row][column];
            if (k == 0)
            {
                run = value == 0xFF ? run - 1 : run + 1;
                runs[line][0] = 1;
            }
            else if (k == size - 1)
            {
                if (value == previous)
                {
                    run = value == 0xFF ? run - 1 : run + 1;
                    runs[line][runs[line][0]] = run;
                }
                else
                {
                    runs[line][runs[line][0]] = run;
                    runs[line][0]++;
                    runs[line][runs[line][0]] = matrix[row][column] == 0xFF ? -1 : 1;
                }
            }
            else if (value == previous)
            {
                run = value == 0xFF ? run - 1 : run + 1;
            }
            else
            {
                runs[line][runs[line][0]] = run;
                runs[line][0]++;
                run = matrix[row][column] == 0xFF ? -1 : 1;
            }
            previous = matrix[row][column];
        }
    }
    return true;
}

// timing patterns, finder patterns with separators and alignment patterns
// 0x00171AF4
bool DrawFunctionPatterns(s32 version, u8** matrix)
{
    s32 size = 0;
    if (!GetSymbolSize(version, &size))
    {
        SetError(ERROR_INVALID_VERSION, true);
        return false;
    }

    for (s32 i = 0; i < size; i++)
    {
        matrix[i][6] = (i & 1) ? 0 : 0xFF;
    }
    for (s32 i = 0; i < size; i++)
    {
        matrix[6][i] = (i & 1) ? 0 : 0xFF;
    }
    DrawFinderPattern(matrix, 0, 0);
    DrawFinderPattern(matrix, size - 7, 0);
    DrawFinderPattern(matrix, 0, size - 7);
    for (s32 i = 0; i < 8; i++)
    {
        matrix[7][i] = 0;
        matrix[i][7] = 0;
        matrix[size - 8][i] = 0;
    }
    for (s32 i = 0; i < 8; i++)
    {
        matrix[size - 1 - i][7] = 0;
        matrix[7][size - 1 - i] = 0;
    }
    for (s32 i = 0; i < 8; i++)
    {
        matrix[i][size - 8] = 0;
    }

    s32 alignmentNum = 0;
    s32 x[46];
    s32 y[46];
    if (!GetAlignmentPositions(version, &alignmentNum, x, y))
    {
        SetError(ERROR_INVALID_VERSION, true);
        return false;
    }
    for (s32 k = 0; k < alignmentNum; k++)
    {
        s32 cx = x[k];
        s32 cy = y[k];
        for (s32 row = cy - 2; row < cy + 3; row++)
        {
            std::memset(matrix[row] + cx - 2, 0xFF, 5);
        }
        matrix[cy - 1][cx - 1] = 0;
        matrix[cy][cx - 1] = 0;
        matrix[cy + 1][cx - 1] = 0;
        matrix[cy - 1][cx] = 0;
        matrix[cy][cx] = 0;
        matrix[cy + 1][cx] = 0;
        matrix[cy - 1][cx + 1] = 0;
        matrix[cy][cx + 1] = 0;
        matrix[cy + 1][cx + 1] = 0;
        matrix[cy][cx] = 0xFF;
    }
    return true;
}

} // namespace detail
} // namespace qre
} // namespace mw
