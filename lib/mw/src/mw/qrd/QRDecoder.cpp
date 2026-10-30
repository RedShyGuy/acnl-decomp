#include "mw/qrd/QRDecoder.h"

#include <cstring>

#include "mw/qrd/qrd_Internal.h"

namespace mw {
namespace qrd {

namespace detail {

// numbers of numeric characters per version (rows) and error correction level (L, M, Q, H) after the
// QR code standard
// 0x0095F344
s32 s_NumericCapacity[40][4] = {
    {41, 34, 27, 17},         {77, 63, 48, 34},         {127, 101, 77, 58},       {187, 149, 111, 82},
    {255, 202, 144, 106},     {322, 255, 178, 139},     {370, 293, 207, 154},     {461, 365, 259, 202},
    {552, 432, 312, 235},     {652, 513, 364, 288},     {772, 604, 427, 331},     {883, 691, 489, 374},
    {1022, 796, 580, 427},    {1101, 871, 621, 468},    {1250, 991, 703, 530},    {1408, 1082, 775, 602},
    {1548, 1212, 876, 674},   {1725, 1346, 948, 746},   {1903, 1500, 1063, 813},  {2061, 1600, 1159, 919},
    {2232, 1708, 1224, 969},  {2409, 1872, 1358, 1056}, {2620, 2059, 1468, 1108}, {2812, 2188, 1588, 1228},
    {3057, 2395, 1718, 1286}, {3283, 2544, 1804, 1425}, {3517, 2701, 1933, 1501}, {3669, 2857, 2085, 1581},
    {3909, 3035, 2181, 1677}, {4158, 3289, 2358, 1782}, {4417, 3486, 2473, 1897}, {4686, 3693, 2670, 2022},
    {4965, 3909, 2805, 2157}, {5253, 4134, 2949, 2301}, {5529, 4343, 3081, 2361}, {5836, 4588, 3244, 2524},
    {6153, 4775, 3417, 2625}, {6479, 5039, 3599, 2735}, {6743, 5313, 3791, 2927}, {7089, 5596, 3993, 3057},
};

// 0x00AD7660
Params s_Params;

} // namespace detail

namespace {

inline u32 AlignUp32(s32 size)
{
    return (size + 31) & ~31;
}

} // namespace

// 0x00345360
mw::qrd::QRDecoder::QRDecoder()
{
    // empty (in the original too; only the vptr is set)
}

// 0x00345374
// 0x00345370 (deleting dtor)
mw::qrd::QRDecoder::~QRDecoder()
{
    // empty (in the original too)
}

bool mw::qrd::QRDecoder::Initialize(void* work, u32 workSize)
{
    using namespace detail;

    if (work == NULL || workSize == 0)
    {
        return false;
    }

    s_WorkState.workUsed = 0;
    s_WorkState.blockNum = 0;
    s_WorkState.work = static_cast<u8*>(work);
    s_WorkState.workSize = workSize;
    std::memset(s_WorkBlocks, 0, sizeof(s_WorkBlocks));
    return true;
}

void mw::qrd::QRDecoder::Finalize()
{
    using namespace detail;

    s_WorkState.work = NULL;
    s_WorkState.blockNum = 0;
    s_WorkState.workUsed = 0;
    s_WorkState.workSize = 0;
    std::memset(s_WorkBlocks, 0, sizeof(s_WorkBlocks));
}

s32 mw::qrd::QRDecoder::Decode(DecodeResult* result, const ImageInfo* image)
{
    using namespace detail;

    // column of s_NumericCapacity for each error correction level (as in the format information: M, L, H, Q)
    const s32 CAPACITY_INDEX[4] = {1, 0, 3, 2}; // 0x0089868C

    s32 dataSize = 0;
    s32 version = 0;

    Yield();
    if (s_WorkState.work == NULL)
    {
        return RESULT_NOT_INITIALIZED;
    }
    if (result == NULL || image == NULL || image->rect.top < 0 || image->rect.left < 0 || image->rect.right < 0 ||
        image->rect.bottom < 0)
    {
        s_WorkState.status = 1;
        return RESULT_INVALID_ARGUMENT;
    }
    if (GetWorkSize(image->width, image->height) - 8 > s_WorkState.workSize)
    {
        return RESULT_OUT_OF_MEMORY;
    }

    // the images first, the blocks of the reader behind them
    s_WorkState.blockNum = 0;
    std::memset(s_WorkBlocks, 0, sizeof(s_WorkBlocks));
    u8* work = s_WorkState.work;
    s_WorkState.grayPixels = work;
    u32 integralOffset = AlignUp32(image->width * image->height);
    s_WorkState.integralImage = reinterpret_cast<s32*>(work + integralOffset);
    u32 bitOffset = integralOffset + AlignUp32(image->width * image->height * 4);
    s_WorkState.workUsed = bitOffset + AlignUp32(image->width * image->height / 8);
    s_WorkState.bitImage = reinterpret_cast<u32*>(work + bitOffset);

    ImageInfo img;
    std::memset(&img, 0, sizeof(img));
    std::memset(&s_Params, 0, sizeof(s_Params));
    SymbolPosition position;
    std::memset(&position, 0, sizeof(position));
    s_Params.windowScale = 0.0f;
    s_Params.thresholdBias = 0.025f;
    img.width = image->width;
    img.height = image->height;
    img.pixels = image->pixels;
    img.rect.top = image->rect.top;
    img.rect.left = image->rect.left;
    img.rect.right = image->rect.right;
    img.rect.bottom = image->rect.bottom;
    Begin();

    // when nothing is found, try again with other binarization parameters
    for (s32 attempt = 0; attempt < 6; attempt++)
    {
        version = 0;
        dataSize = 0;
        s32 count = 1;
        s_WorkState.status = DetectSymbol(&s_Params, &img, &dataSize, &version, &count, &position);
        f32 moduleSize = s_SampleState.moduleSize;
        if (s_WorkState.status == 0)
        {
            u8* data = static_cast<u8*>(Allocate(dataSize));
            if (data == NULL)
            {
                s_WorkState.status = 2;
                return RESULT_OUT_OF_MEMORY;
            }

            s_WorkState.status = DecodeSymbol(data, dataSize, &version);
            if (s_WorkState.status == 0)
            {
                s32 appendCount = 0;
                s32 appendIndex = 0;
                s32 appendParity = 0;
                switch (data[0] >> 4)
                {
                case 1:
                    s_WorkState.dataMode = DATA_MODE_NUMERIC;
                    break;
                case 2:
                    s_WorkState.dataMode = DATA_MODE_ALPHANUMERIC;
                    break;
                case 4:
                    s_WorkState.dataMode = DATA_MODE_8BIT;
                    break;
                case 8:
                    s_WorkState.dataMode = DATA_MODE_KANJI;
                    break;
                default:
                    s_WorkState.dataMode = DATA_MODE_8BIT;
                    break;
                }

                s32 symbolVersion = version;
                s32 length = ExtractData(data, dataSize, &symbolVersion, result->data, &appendIndex, &appendCount,
                                         &appendParity);
                if (length <= 0)
                {
                    s_WorkState.status = 2;
                }
                // (the original looks at the row of the next version)
                else if (length > s_NumericCapacity[version][CAPACITY_INDEX[s_Context.format.errorCorrectionLevel]])
                {
                    s_WorkState.status = 1;
                }
                else
                {
                    result->dataSize = length;
                    result->appendCount = appendCount;
                    result->appendIndex = appendIndex;
                    result->appendParity = appendParity;
                    std::memset(result->data + length, 0, sizeof(result->data) - length);
                }
            }
            Free(data);
        }

        if (s_WorkState.status == 0)
        {
            break;
        }

        switch (attempt)
        {
        case 0:
            if (moduleSize != 0.0f)
            {
                s_Params.windowScale = moduleSize * 10.0f;
            }
            s_Params.isAdaptive = 1;
            break;
        case 1:
            if (moduleSize != 0.0f)
            {
                s_Params.windowScale = moduleSize * 10.0f;
            }
            s_Params.thresholdBias = 0.0f;
            break;
        case 2:
            s_Params.windowScale *= 0.75f;
            s_Params.thresholdBias = 0.15f;
            break;
        case 3:
            s_Params.windowScale = moduleSize * 3.5f;
            s_Params.thresholdBias = 0.025f;
            break;
        case 4:
            s_Params.windowScale *= 6.17309952f;
            break;
        }
    }

    End();
    switch (s_WorkState.status)
    {
    case 0:
        result->version = version;
        result->dataMode = s_WorkState.dataMode;
        result->top = position.top + image->rect.top;
        result->left = position.left + image->rect.left;
        result->right = position.right + image->rect.left;
        result->bottom = position.bottom + image->rect.top;
        return RESULT_SUCCESS;
    case 2:
        return RESULT_OUT_OF_MEMORY;
    case 6:
    case 8:
        if (version > 40)
        {
            return RESULT_VERSION_TOO_HIGH;
        }
        return RESULT_NOT_FOUND;
    default:
        return RESULT_NOT_FOUND;
    }
}

u32 mw::qrd::QRDecoder::GetWorkSize(u32 width, u32 height)
{
    if (width > 640 || height > 480)
    {
        return 0;
    }
    // luminance image, integral image, bit image and the blocks of the reader
    s32 pixelNum = width * height;
    return (pixelNum * 5 + pixelNum / 8 + 0x40000 + 31) & ~31;
}

} // namespace qrd
} // namespace mw
