#include "mw/qrd/qrd_Internal.h"

namespace mw {
namespace qrd {
namespace detail {

// 0x007A74BC
s32 DecodeSymbol(u8* data, s32 dataSize, s32* version)
{
    Yield();
    if (data == NULL || version == NULL)
    {
        Cleanup();
        return 1;
    }
    if (!ReadData(data, dataSize, version))
    {
        Cleanup();
        return 12;
    }
    Cleanup();
    return 0;
}

// 0x007A7560
s32 DetectSymbol(Params* params, const ImageInfo* image, s32* dataSize, s32* version, s32* count,
                 SymbolPosition* position)
{
    Yield();
    if (params == NULL || image == NULL || dataSize == NULL)
    {
        return 1;
    }

    PatternPoint alignment;
    alignment.x = 0.0f;
    alignment.y = 0.0f;
    alignment.moduleSize = 0.0f;
    alignment.count = 0;
    if (!PrepareImage(params, image, position))
    {
        Cleanup();
        return 4;
    }
    if (!FindFinderPatterns(count, position))
    {
        Cleanup();
        return 5;
    }
    if (!EstimateVersion(version, &alignment, position))
    {
        Cleanup();
        return 6;
    }
    if (!SampleModules(&alignment))
    {
        Cleanup();
        return 7;
    }
    if (!ReadSymbolInformation(version))
    {
        Cleanup();
        return 8;
    }
    if (!UnmaskSymbol())
    {
        Cleanup();
        return 9;
    }
    if (!ReadCodewords())
    {
        Cleanup();
        return 10;
    }
    if (!ReadBlocks(dataSize))
    {
        Cleanup();
        return 11;
    }
    return *dataSize == 0 ? 3 : 0;
}

} // namespace detail
} // namespace qrd
} // namespace mw
