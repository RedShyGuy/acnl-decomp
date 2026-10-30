#include <cstring>

#include "mw/qrd/qrd_Internal.h"

namespace mw {
namespace qrd {
namespace detail {

// 0x00AE68C8
SourceImage s_SourceImage;
// 0x00AE68D4
GrayImage s_GrayImage;
// 0x00AE68E0
Context s_Context;
// 0x00976CA4
SampleState s_SampleState;

namespace {

// the steps of DetectSymbol after the sampling, for the second sampling (1 = fine)
inline s32 ReadSampledSymbol()
{
    s32 version;
    s32 dataSize;

    if (!ReadSymbolInformation(&version))
    {
        return 8;
    }
    if (!UnmaskSymbol())
    {
        return 9;
    }
    if (!ReadCodewords())
    {
        return 10;
    }
    if (!ReadBlocks(&dataSize))
    {
        return 11;
    }
    return dataSize == 0 ? 3 : 1;
}

} // namespace

// 0x00158544
void End()
{
    Yield();
    Release();
}

// 0x00158590
void Release()
{
    Yield();
    if (s_GrayImage.pixels != NULL)
    {
        s_GrayImage.width = 0;
        s_GrayImage.height = 0;
        s_GrayImage.pixels = NULL;
    }
    if (s_SampleState.modules != NULL)
    {
        Free(s_SampleState.modules);
        s_SampleState.modules = NULL;
    }
}

// 0x001585E4
bool ReadData(u8* data, s32 dataSize, s32* version)
{
    s32 offset = 0;

    Yield();
    if (!CorrectBlocks(&s_Context.codewords))
    {
        // try again with the modules of the second sampling
        if (s_SampleState.modules == NULL)
        {
            return false;
        }

        Yield();
        Cleanup();
        s_Context.matrix.modules = s_SampleState.modules;
        s_Context.matrix.size = s_SampleState.symbolSize;
        s_Context.matrix.moduleNum = s_SampleState.symbolSize * s_SampleState.symbolSize;
        s32 result = ReadSampledSymbol();
        ReleaseModules();
        s_Context.matrix.modules = s_SampleState.modules;
        if (result != 1)
        {
            return false;
        }
        if (!CorrectBlocks(&s_Context.codewords))
        {
            return false;
        }
    }

    std::memset(data, 0, dataSize);
    for (s32 i = 0; i < s_Context.codewords.blockNum; i++)
    {
        std::memcpy(data + offset, s_Context.codewords.blocks[i].data, s_Context.codewords.blocks[i].dataNum);
        offset += s_Context.codewords.blocks[i].dataNum;
    }
    *version = s_Context.version;
    return true;
}

// 0x00158D44
bool FindFinderPatterns(s32* count, SymbolPosition* position)
{
    Yield();
    bool result = LocateFinderPatterns(&s_Context.bitImage, &s_Context.finders, count, position);
    if (result)
    {
        FinderPattern* finders = s_Context.finders;
        f64 size = finders[1].moduleSize > finders[2].moduleSize ? finders[1].moduleSize : finders[2].moduleSize;
        size = finders[0].moduleSize > size ? finders[0].moduleSize : size;
        s_SampleState.moduleSize = size;
    }
    return result;
}

// 0x00158DB8
bool ReadCodewords()
{
    Yield();
    if (ExtractCodewords(&s_Context.matrix, &s_Context.version, &s_Context.format, &s_Context.codewords))
    {
        return true;
    }
    ReleaseModules();
    return false;
}

// 0x00158ED8
bool ReadBlocks(s32* dataSize)
{
    Yield();
    if (SplitBlocks(&s_Context.version, &s_Context.format, &s_Context.codewords))
    {
        if (dataSize != NULL)
        {
            *dataSize = s_Context.codewords.size;
        }
        return true;
    }
    ReleaseModules();
    return false;
}

// 0x00158F48
bool SampleModules(const PatternPoint* alignment)
{
    Yield();
    FinderPattern* finders = s_Context.finders;
    u8* modules = SampleGrid(&finders[0], &finders[1], &finders[2], alignment, s_SampleState.symbolSize);
    s_Context.matrix.modules = modules;
    s_Context.matrix.size = s_SampleState.symbolSize;
    s_Context.matrix.moduleNum = s_SampleState.symbolSize * s_SampleState.symbolSize;
    if (modules != NULL)
    {
        return true;
    }
    // the grid failed, take the second sampling
    if (s_SampleState.modules != NULL)
    {
        s_Context.matrix.modules = s_SampleState.modules;
        s_SampleState.modules = NULL;
        return true;
    }
    return false;
}

// 0x00158FC0
bool EstimateVersion(s32* version, PatternPoint* alignment, SymbolPosition* position)
{
    Yield();
    *version = 0;
    FinderPattern* finders = s_Context.finders;
    SetFinderCentres(s_Context.bitImage.width, s_Context.bitImage.height, &finders[0], &finders[1], &finders[2]);
    s_SampleState.modules = SampleByPerspective();
    f32 moduleSize = EstimateModuleSize(&s_Context.finders[0], &s_Context.finders[1], &s_Context.finders[2]);

    // the version of the second sampling, if it found one
    s32 symbolSize = s_SampleInfo.version;
    if (symbolSize != -1)
    {
        symbolSize = symbolSize * 4 + 17;
    }
    s_SampleState.symbolSize = symbolSize;
    if (symbolSize == -1)
    {
        symbolSize = EstimateSymbolSize(&s_Context.finders[0], &s_Context.finders[1], &s_Context.finders[2], moduleSize);
        s_SampleState.symbolSize = symbolSize;
    }
    if (symbolSize <= 17 || symbolSize % 4 != 1)
    {
        ReleaseModules();
        return false;
    }

    s32 estimated = (symbolSize - 17) >> 2;
    if (estimated > 40)
    {
        *version = estimated;
        s_Context.version = estimated;
        ReleaseModules();
        return false;
    }

    // where the fourth corner would be
    finders = s_Context.finders;
    f32 x0 = finders[0].x;
    f32 y0 = finders[0].y;
    f32 cornerX = static_cast<f32>(finders[1].x) - x0 + static_cast<f32>(finders[2].x);
    f32 cornerY = static_cast<f32>(finders[1].y) - y0 + static_cast<f32>(finders[2].y);
    if (!(cornerX >= 0.0f && cornerY >= 0.0f))
    {
        ReleaseModules();
        return false;
    }

    if (estimated == 1)
    {
        // version 1 has no alignment pattern
        position->points[3].x = position->points[1].x + position->points[2].x - position->points[0].x;
        position->points[3].y = position->points[1].y + position->points[2].y - position->points[0].y;
        position->points[3].moduleSize =
            position->points[1].moduleSize + position->points[2].moduleSize - position->points[0].moduleSize;
        ComputeBoundingBox(position, s_Context.bitImage.width, s_Context.bitImage.height);
        return true;
    }

    // the alignment pattern lies 3 modules inside the fourth corner
    f32 t = 1.0f - 3.0f / (static_cast<f32>(symbolSize) - 7.0f);
    s32 x = finders[0].x + t * (cornerX - finders[0].x);
    s32 y = finders[0].y + t * (cornerY - finders[0].y);
    s32 found;
    s32 radius = 4;
    do
    {
        found = FindAlignmentPattern(x, y, &s_Context.finders[0], &s_Context.finders[1], &s_Context.finders[2],
                                     alignment, moduleSize, radius);
        if (found == 1)
        {
            break;
        }
        radius *= 2;
    } while (radius <= 16);
    if (found == 0)
    {
        ReleaseModules();
        return false;
    }
    if (alignment->moduleSize == -1.0f)
    {
        ReleaseModules();
        return false;
    }

    position->points[3] = *alignment;
    position->isFound = 1;
    ComputeBoundingBox(position, s_Context.bitImage.width, s_Context.bitImage.height);
    return true;
}

// 0x001592F0
bool ReadSymbolInformation(s32* version)
{
    Yield();
    *version = 0;
    if (ReadVersion(&s_Context.matrix, &s_Context.version))
    {
        if (s_Context.version > 40)
        {
            ReleaseModules();
            *version = s_Context.version;
            return false;
        }
        if (ReadFormat(&s_Context.matrix, &s_Context.format))
        {
            return true;
        }
    }
    ReleaseModules();
    return false;
}

// 0x00159720
void Begin()
{
    Yield();
    Reset();
}

// 0x0015976C
void Reset()
{
    Yield();
    s_GrayImage.width = 0;
    s_GrayImage.height = 0;
    s_GrayImage.pixels = NULL;
    std::memset(&s_Context, 0, sizeof(s_Context));
}

} // namespace detail
} // namespace qrd
} // namespace mw
