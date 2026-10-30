#include <cstdio>
#include <cstring>

#include "mw/qre/qre_Encoder.h"

namespace mw {
namespace qre {
namespace detail {

// The messages are Shift-JIS, as in the original.
namespace {

// 指定されたバージョンではデータを格納できません。 (the data does not fit into the given version)
#define MESSAGE_DATA_TOO_LARGE                                                                                   \
    "\x8Ew\x92\xE8\x82\xB3\x82\xEA\x82\xBD\x83o\x81[\x83W\x83\x87\x83\x93\x82\xC5\x82\xCD\x83"                     \
    "f\x81[\x83^\x82\xF0\x8Ai\x94[\x82\xC5\x82\xAB\x82\xDC\x82\xB9\x82\xF1\x81"                                    \
    "B"

// the data split over the symbols of a structured append series by DivideInfo::method
enum DivideMethod
{
    DIVIDE_METHOD_NONE = 0,
    DIVIDE_METHOD_EVEN = 1,
    DIVIDE_METHOD_GIVEN = 2,
};

// a series has at most 16 symbols
const s32 PART_MAX = 16;

// the error correction level of EncodeParam (1 L, 2 H, 3 Q, else M) as in the format information
inline u8 ConvertLevel(u8 level)
{
    switch (level)
    {
    case 1:
        return 0;
    case 2:
        return 3;
    case 3:
        return 2;
    default:
        return 1;
    }
}

inline void FreeMatrix(u8** matrix, s32 size)
{
    if (matrix != NULL)
    {
        for (s32 i = 0; i < size; i++)
        {
            if (matrix[i] != NULL)
            {
                Free(matrix[i]);
            }
        }
        if (matrix != NULL)
        {
            Free(matrix);
        }
    }
}

} // namespace

// 0x00ADA670
Encoder s_Encoder;

// the static initializer constructs s_Encoder and registers its destructor
// 0x0079173C (static initializer)

// the number of symbols and the smallest version for the data (version 0 = choose one)
// 0x006E1C00
bool Encoder::CalcDivide(EncodeParam* param, s32* divideNum, s32* version, s32 mode)
{
    s32 totalNum = 0;
    ErrorCorrectionInfo info;
    u8 level = ConvertLevel(param->level);

    if (param->version != 0)
    {
        GetTotalCodewordNum(param->version, &totalNum);
        GetErrorCorrectionInfo(param->version, level, &info);
        s32 dataNum = totalNum - info.ecCodewordNum;
        if (dataNum < 1)
        {
            std::sprintf(m_Message, "%s", MESSAGE_DATA_TOO_LARGE);
            return false;
        }

        s32 bitNum = GetBitLength(param->version, mode, param->size);
        if (dataNum > (bitNum + 7) / 8)
        {
            *divideNum = 1;
            return true;
        }

        // each symbol has a structured append header (20 bits), a mode indicator and a character count
        s32 countBits = GetCharCountBits(param->version, mode);
        s32 partDataNum = dataNum - (countBits + 31) / 8;
        s32 num = (partDataNum + (bitNum - countBits + 3) / 8 - 1) / partDataNum;
        if (num > PART_MAX)
        {
            // コンテンツサイズが大きすぎます。分割数が16個を超えてしまいます。
            // (the content is too large: more than 16 symbols)
            std::sprintf(m_Message, "%s\n%s", MESSAGE_DATA_TOO_LARGE,
                         "\x83R\x83\x93\x83"
                         "e\x83\x93\x83"
                         "c\x83T\x83"
                         "C\x83Y\x82\xAA\x91\xE5\x82\xAB\x82\xB7\x82\xAC\x82\xDC\x82\xB7\x81"
                         "B\x95\xAA\x8A\x84\x90\x94\x82\xAA"
                         "16\x8C\xC2\x82\xF0\x92\xB4\x82\xA6\x82\xC4\x82\xB5\x82\xDC\x82\xA2\x82\xDC\x82\xB7\x81"
                         "B");
            return false;
        }
        *divideNum = num;
        return true;
    }

    GetTotalCodewordNum(40, &totalNum);
    GetErrorCorrectionInfo(40, level, &info);
    s32 dataNum = totalNum - info.ecCodewordNum;
    s32 bitNum = GetBitLength(40, mode, param->size);
    s32 num;
    s32 best = 40;
    if (dataNum > (bitNum + 7) / 8)
    {
        num = 1;
        for (s32 v = 39; v > 0; v--)
        {
            GetTotalCodewordNum(v, &totalNum);
            GetErrorCorrectionInfo(v, level, &info);
            if (bitNum > (totalNum - info.ecCodewordNum) * 8)
            {
                break;
            }
            best = v;
        }
    }
    else
    {
        // (the parts are measured in bits here and compared with bytes below)
        s32 dataBitNum = bitNum - GetCharCountBits(40, mode) - 4;
        s32 partDataNum = dataNum - (GetCharCountBits(40, mode) + 31) / 8;
        num = (partDataNum + (dataBitNum + 7) / 8 - 1) / partDataNum;
        if (mode == MODE_KANJI)
        {
            // in characters of 13 bits
            s32 partCharNum = partDataNum * 8 / 13;
            dataBitNum /= 13;
            num = (dataBitNum + partCharNum - 1) / partCharNum;
        }
        if (num > PART_MAX)
        {
            std::sprintf(m_Message, "%s", MESSAGE_DATA_TOO_LARGE);
            return false;
        }

        s32 partSize = (dataBitNum + num - 1) / num;
        for (s32 v = 39; v > 0; v--)
        {
            GetTotalCodewordNum(v, &totalNum);
            GetErrorCorrectionInfo(v, level, &info);
            if (totalNum - info.ecCodewordNum - (GetCharCountBits(v, mode) + 31) / 8 < partSize)
            {
                break;
            }
            best = v;
        }
    }
    *divideNum = num;
    *version = best;
    return true;
}

// draws the symbol into a new 24 bit image (dark modules black) with a white quiet zone
// 0x006E1F5C
bool Encoder::RenderImage(s32 size, s32 cellSize, s32 quietZone, u8** matrix, s32 index)
{
    s32 width = m_ImageWidth;
    s32 stride = width * 3;
    u8* pixels = static_cast<u8*>(Allocate(stride * width));
    if (pixels == NULL)
    {
        return false;
    }

    s32 margin = quietZone * cellSize;
    std::memset(pixels, 0xFF, margin * stride);
    u8* line = pixels + stride * margin;
    s32 row = 0;
    s32 y = margin;
    s32 end = (quietZone + size) * cellSize;
    for (s32 i = 0; y < end; i++, y++)
    {
        std::memset(line, 0xFF, margin * 3);
        s32 x = margin * 3;
        s32 column = 0;
        for (s32 k = 0; x < end * 3; k++, x += 3)
        {
            u8 value = ~matrix[row][column];
            line[x + 1] = value;
            line[x] = value;
            line[x + 2] = value;
            if (k % cellSize == cellSize - 1)
            {
                column++;
            }
        }
        std::memset(line + x, 0xFF, stride - x);
        line += stride;
        if (i % cellSize == cellSize - 1)
        {
            row++;
        }
    }
    std::memset(line, 0xFF, (width - y) * stride);

    ImageInfo* image = &m_Images[index];
    image->_10 = 0.0f;
    image->_0C = 0.0f;
    image->height = width;
    image->width = stride / 3;
    image->_14 = 0.0f;
    image->_08 = 0.0f;
    if (image->pixels != NULL)
    {
        Free(image->pixels);
    }
    image->pixels = pixels;
    return true;
}

// 0x006E2188
bool Encoder::EncodePart(Part* part, s32 index)
{
    u8* stream = NULL;
    u8** matrix = NULL;
    s32 version = part->version;
    s32 size;
    s32 codewordNum;

    if (!ChooseVersion(&version, &part->format.level, &part->mode, &part->data, part->count >= 2 ? part : NULL))
    {
        SetErrorMessage("");
        return false;
    }
    part->resultVersion = version;
    GetSymbolSize(version, &size);
    m_ImageWidth = (size + part->quietZone * 2) * part->cellSize;

    if (!BuildDataStream(&part->resultVersion, &part->format.level, &part->mode, &part->data, &codewordNum, &stream,
                         part->count >= 2 ? part : NULL))
    {
        SetErrorMessage("");
        return false;
    }
    if (!AddErrorCorrection(part->resultVersion, part->format.level, stream))
    {
        SetErrorMessage("");
        if (stream != NULL)
        {
            Free(stream);
        }
        return false;
    }

    bool isPlaced = PlaceCodewords(part->resultVersion, codewordNum, stream, &matrix);
    if (stream != NULL)
    {
        Free(stream);
    }
    if (!isPlaced)
    {
        SetErrorMessage("");
        return false;
    }

    part->usedFormat = part->format;
    if (!SelectMask(part->resultVersion, &part->usedFormat, part->format.level, matrix))
    {
        SetErrorMessage("");
        FreeMatrix(matrix, size);
        return false;
    }
    if (!WriteFormatInformation(part->resultVersion, part->usedFormat, matrix))
    {
        SetErrorMessage("");
        FreeMatrix(matrix, size);
        return false;
    }

    bool isRendered = RenderImage(size, part->cellSize, part->quietZone, matrix, index);
    FreeMatrix(matrix, size);
    if (isRendered)
    {
        return true;
    }
    // Rawdataを作成できません。 (cannot create the raw data)
    std::strcpy(m_Message, "Rawdata\x82\xF0\x8D\xEC\x90\xAC\x82\xC5\x82\xAB\x82\xDC\x82\xB9\x82\xF1\x81"
                           "B");
    return false;
}

// 0x006E2474
void Encoder::ClearImages()
{
    for (s32 i = 0; i < 16; i++)
    {
        ImageInfo* image = &m_Images[i];
        if (image->pixels != NULL)
        {
            Free(image->pixels);
            image->pixels = NULL;
        }
        image->width = 0;
        image->height = 0;
        image->_08 = 0.0f;
        image->_0C = 0.0f;
        image->_10 = 0.0f;
        image->_14 = 0.0f;
    }
}

// 0x006E24C4
bool Encoder::Encode(EncodeParam* param)
{
    if (param == NULL)
    {
        // 引数が不正です (invalid argument)
        std::strcpy(m_Message, "\x88\xF8\x90\x94\x82\xAA\x95s\x90\xB3\x82\xC5\x82\xB7");
        return false;
    }

    m_IsEncoded = false;
    ClearImages();
    ResetError();
    ResetMemory();

    DivideInfo info = {};
    bool result = true;
    s32 mode = DetectMode(param->data, param->size);
    m_DivideNum = param->divideNum;
    m_Version = param->version;
    if (param->divideNum == 0)
    {
        s32 divideNum = 0;
        s32 version = param->version;
        if (!CalcDivide(param, &divideNum, &version, mode))
        {
            return false;
        }
        param->divideNum = divideNum;
        param->version = version;
        m_DivideNum = divideNum;
    }

    if (!Divide(&info, param, mode))
    {
        return false;
    }

    // all symbols of an evenly split series get the largest version
    if (info.count > 1 && info.method == DIVIDE_METHOD_EVEN)
    {
        s32 maxVersion = 1;
        for (s32 i = 0; i < info.count; i++)
        {
            Part* part = &info.parts[i];
            s32 version = part->version;
            result = ChooseVersion(&version, &part->format.level, &part->mode, &part->data,
                                   part->count >= 2 ? part : NULL);
            if (!result)
            {
                SetErrorMessage("");
                if (info.parts != NULL)
                {
                    Free(info.parts);
                }
                if (info.data.data != NULL)
                {
                    Free(const_cast<u8*>(info.data.data));
                }
                return false;
            }
            if (version >= maxVersion)
            {
                maxVersion = version;
            }
        }
        for (s32 i = 0; i < info.count; i++)
        {
            info.parts[i].version = maxVersion;
        }
    }

    for (s32 i = 0; i < info.count; i++)
    {
        if (!EncodePart(&info.parts[i], i))
        {
            result = false;
            break;
        }
    }

    m_Version = info.parts->resultVersion;
    if (info.parts != NULL)
    {
        Free(info.parts);
    }
    if (info.data.data != NULL)
    {
        Free(const_cast<u8*>(info.data.data));
    }
    m_IsEncoded = true;
    return result;
}

// copies an encoded image into the caller's buffer
// 0x006E27B4
bool Encoder::GetImage(ImageInfo* image, s32 index)
{
    if (image == NULL || image->pixels == NULL || !m_IsEncoded)
    {
        return false;
    }

    const ImageInfo* source = &m_Images[index];
    image->width = source->width;
    image->height = source->height;
    image->_08 = source->_08;
    image->_14 = source->_14;
    image->_0C = source->_0C;
    image->_10 = source->_10;
    std::memcpy(image->pixels, source->pixels, source->width * source->height * 3);
    return true;
}

// the size of the raw RGB pixels or of a BMP file (rows padded to 4 bytes, 54 byte header)
// 0x006E2830
u32 Encoder::GetImageSize(s32 index, bool isBmp)
{
    if (!m_IsEncoded)
    {
        return 0;
    }

    const ImageInfo* image = &m_Images[index];
    if (!isBmp)
    {
        return image->width * image->height * 3;
    }
    return ((image->width * 3 + 3) & ~3) * image->height + 54;
}

// the message of the current error code followed by the detail
// 0x006E2878
void Encoder::SetErrorMessage(const char* detail)
{
    switch (GetError())
    {
    case ERROR_INVALID_MODE:
        // モード指示子が不正です。 (invalid mode indicator)
        std::sprintf(m_Message, "%s\n",
                     "\x83\x82\x81[\x83h\x8Ew\x8E\xA6\x8Eq\x82\xAA\x95s\x90\xB3\x82\xC5\x82\xB7\x81"
                     "B");
        break;
    case ERROR_INVALID_VERSION:
        // バージョンが不正です。 (invalid version)
        std::sprintf(m_Message, "%s\n",
                     "\x83o\x81[\x83W\x83\x87\x83\x93\x82\xAA\x95s\x90\xB3\x82\xC5\x82\xB7\x81"
                     "B");
        break;
    case ERROR_DATA_TOO_LARGE:
        std::sprintf(m_Message, "%s\n", MESSAGE_DATA_TOO_LARGE);
        break;
    case ERROR_INVALID_CHARACTER:
        // 指定されたモード指示子でエンコードできない文字が含まれています。
        // (the data has characters that the given mode cannot encode)
        std::sprintf(m_Message, "%s\n",
                     "\x8Ew\x92\xE8\x82\xB3\x82\xEA\x82\xBD\x83\x82\x81[\x83h\x8Ew\x8E\xA6\x8Eq\x82\xC5\x83G\x83\x93\x83R"
                     "\x81[\x83h\x82\xC5\x82\xAB\x82\xC8\x82\xA2\x95\xB6\x8E\x9A\x82\xAA\x8A\xDC\x82\xDC\x82\xEA\x82\xC4"
                     "\x82\xA2\x82\xDC\x82\xB7\x81"
                     "B");
        break;
    default:
        // システムエラー。 (system error)
        std::sprintf(m_Message, "%s\n",
                     "\x83V\x83X\x83"
                     "e\x83\x80\x83G\x83\x89\x81[\x81"
                     "B");
        break;
    }
    std::strcat(m_Message, detail);
}

// splits the data over the symbols
// 0x006E29BC
bool Encoder::Divide(DivideInfo* info, EncodeParam* param, s32 mode)
{
    bool isValid = true;
    info->count = 0;
    info->parts = NULL;
    info->data.size = 0;
    info->data.data = NULL;
    info->method = DIVIDE_METHOD_NONE;

    if (param->size == 0)
    {
        // データを入力してください。 (please give the data)
        std::strcpy(m_Message, "\x83"
                               "f\x81[\x83^\x82\xF0\x93\xFC\x97\xCD\x82\xB5\x82\xC4\x82\xAD\x82\xBE\x82\xB3\x82\xA2\x81"
                               "B");
        return false;
    }

    if (static_cast<u32>(param->divideNum) <= 1)
    {
        info->method = DIVIDE_METHOD_NONE;
        info->count = 1;
    }
    else
    {
        info->method = DIVIDE_METHOD_EVEN;
        info->count = param->divideNum;
    }

    info->data.size = param->size;
    u8* data = static_cast<u8*>(Allocate(param->size + 1));
    info->data.data = data;
    std::memset(data, 0, param->size + 1);
    std::memcpy(data, param->data, param->size);

    u8 parity;
    ComputeParity(&info->data, &parity);

    info->parts = static_cast<Part*>(Allocate(info->count * sizeof(Part)));
    std::memset(info->parts, 0, info->count * sizeof(Part));

    for (s32 i = 0; i < info->count; i++)
    {
        Part* part = &info->parts[i];
        part->index = i + 1;
        part->count = info->count;
        part->parityCopy = parity;
        part->parity = parity;
        part->version = param->version;
        part->resultVersion = 0;
        part->mode = mode;
        part->format.level = ConvertLevel(param->level);
        part->format.maskPattern = 0xFF;
        part->usedFormat = part->format;

        switch (info->method)
        {
        case DIVIDE_METHOD_NONE:
            info->parts->data = info->data;
            isValid = true;
            break;
        case DIVIDE_METHOD_EVEN:
        {
            f64 partSize = static_cast<f64>(info->data.size) / info->count;
            u8 partMode = info->parts[i].mode;
            s32 offset = 0;
            for (s32 j = 0; j < i; j++)
            {
                offset += info->parts[j].data.size;
            }
            s32 size;
            if (i == info->count - 1)
            {
                size = info->data.size - offset;
            }
            else
            {
                size = static_cast<s32>((i + 1) * partSize) - offset;
                // a kanji character takes two bytes
                if (partMode == MODE_KANJI && (size & 1))
                {
                    size--;
                }
            }
            info->parts[i].data.data = info->data.data + offset;
            info->parts[i].data.size = size;
            isValid = true;
            break;
        }
        case DIVIDE_METHOD_GIVEN:
            break;
        default:
            // 分割方法が不正です。 (invalid split method)
            std::strcpy(m_Message, "\x95\xAA\x8A\x84\x95\xFB\x96@\x82\xAA\x95s\x90\xB3\x82\xC5\x82\xB7\x81"
                                   "B");
            isValid = false;
            break;
        }
        if (!isValid)
        {
            return false;
        }

        part->quietZone = 4;
        part->cellSize = param->cellSize;
    }
    return true;
}

// 0x006E2CE8
Encoder::Encoder()
{
    m_Version = 0;
    m_DivideNum = 1;
    m_ImageWidth = 0;
    m_IsEncoded = false;
    for (s32 i = 0; i < 16; i++)
    {
        m_Images[i].width = 0;
        m_Images[i].height = 0;
        m_Images[i].pixels = NULL;
        m_Images[i]._08 = 0.0f;
        m_Images[i]._0C = 0.0f;
        m_Images[i]._10 = 0.0f;
        m_Images[i]._14 = 0.0f;
    }
    std::memset(m_Message, 0, sizeof(m_Message));
    ResetError();
}

// 0x006E2D58
Encoder::~Encoder()
{
    ClearImages();
}

} // namespace detail
} // namespace qre
} // namespace mw
