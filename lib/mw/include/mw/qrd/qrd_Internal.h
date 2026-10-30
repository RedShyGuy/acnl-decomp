#pragma once

// The QR code reader behind mw::qrd::QRDecoder. The binary has no names for it: all names in this
// header and in qrd_*.cpp are ours.

#include "decomp.h"
#include "mw/qrd/QRDecoder.h"

namespace mw {
namespace qrd {
namespace detail {

// --------------------------------------------------------------- work memory

// the work memory given to QRDecoder::Initialize and its use
struct WorkState
{
    u8 dataMode;             // +0x00 QRDecoder::DataMode of the last decoded data
    u8 status;               // +0x01 status of the last step (0 = fine)
    u8 isYieldRequested;     // +0x02 Yield sleeps once when set (nothing sets it in ACNL)
    u8 isYieldDisabled;      // +0x03
    u8* work;                // +0x04
    u32 workSize;            // +0x08
    u32 workUsed;            // +0x0C
    u32 blockNum;            // +0x10 used entries of s_WorkBlocks
    u8* grayPixels;          // +0x14 luminance image (width * height)
    s32* integralImage;      // +0x18 sums for the adaptive threshold (width * height words)
    u32* bitImage;           // +0x1C binarized image (one bit per pixel)
};
ASSERT_SIZE(WorkState, 0x20);

// a block of the work memory; block 0 is taken from the end of the work memory, the others from its start
struct WorkBlock
{
    u32 size;   // +0x00
    void* ptr;  // +0x04
    u8 isUsed;  // +0x08
};
ASSERT_SIZE(WorkBlock, 0xC);

const s32 WORK_BLOCK_NUM = 1024;

extern WorkState s_WorkState;
extern WorkBlock s_WorkBlocks[WORK_BLOCK_NUM];

// sleeps once when someone asked for it (called everywhere in the reader)
void Yield();
void* AllocateTop(u32 size);
void FreeTop(void* ptr);
void* Allocate(u32 size);
void Free(void* ptr);

// --------------------------------------------------------------- QR code tables

// the blocks of one error correction level of a version
struct BlockGroup
{
    s32 blockNum;         // +0x00 -1 = the group is unused
    s32 dataCodewordNum;  // +0x04 per block
};
ASSERT_SIZE(BlockGroup, 0x8);

struct ErrorCorrectionInfo
{
    s32 ecCodewordNum;      // +0x00 per block
    BlockGroup groups[2];   // +0x04
};
ASSERT_SIZE(ErrorCorrectionInfo, 0x14);

struct FormatCode
{
    u32 code;   // +0x00
    s32 info;   // +0x04 level << 3 | mask pattern
};
ASSERT_SIZE(FormatCode, 0x8);

// the capacity of each version (1..40) and error correction level (L, M, Q, H) in numeric characters
extern s32 s_NumericCapacity[40][4];
extern const s32 s_AlignmentPositions[40][8];
extern const ErrorCorrectionInfo s_ErrorCorrectionInfos[40][4];
extern const FormatCode s_FormatCodes[32];
extern const s32 s_NibbleBitCounts[16];

// the error correction level as in the format information (M = 0, L = 1, H = 2, Q = 3) to the table index
// (L, M, Q, H)
inline s32 GetLevelIndex(u8 level)
{
    switch (level)
    {
    case 1:
        return 0;
    case 0:
        return 1;
    case 3:
        return 2;
    case 2:
        return 3;
    }
    return 0;
}

inline s32 GetErrorCorrectionCodewordNum(s32 version, u8 level)
{
    if (version <= 0 || version > 40)
    {
        return 0;
    }
    return s_ErrorCorrectionInfos[version - 1][GetLevelIndex(level)].ecCodewordNum;
}

inline s32 GetBlockGroupNum(s32 version, u8 level)
{
    s32 num = 0;
    if (version <= 0 || version > 40)
    {
        return 0;
    }
    const ErrorCorrectionInfo* info = &s_ErrorCorrectionInfos[version - 1][GetLevelIndex(level)];
    if (info->groups[0].blockNum > -1)
    {
        num = 1;
    }
    if (info->groups[1].blockNum > -1)
    {
        num++;
    }
    return num;
}

inline s32 GetDataCodewordNum(s32 version, u8 level, s32 group)
{
    s32 num = 0;
    if (version <= 0 || version > 40)
    {
        return 0;
    }
    s32 value = s_ErrorCorrectionInfos[version - 1][GetLevelIndex(level)].groups[group].dataCodewordNum;
    if (value > -1)
    {
        num = value;
    }
    return num;
}

inline s32 GetAlignmentPosition(s32 version, s32 index)
{
    if (version <= 0 || version > 40)
    {
        return 0;
    }
    return s_AlignmentPositions[version - 1][index];
}

inline s32 GetAlignmentNum(s32 version)
{
    s32 num = 0;
    if (version <= 0 || version > 40)
    {
        return 0;
    }
    for (s32 i = 0; i < 32; i++)
    {
        if (s_AlignmentPositions[version - 1][i] <= -1)
        {
            break;
        }
        num++;
    }
    return num;
}

// --------------------------------------------------------------- images

// parameters of the binarization (QRDecoder::Decode changes them between its attempts)
struct Params
{
    f32 windowScale;    // +0x00 size of the adaptive threshold window (0 = from the image width)
    f32 thresholdBias;  // +0x04 a pixel is light when it is above (1 - bias) * mean of its window
    s32 isAdaptive;     // +0x08 0 = one threshold for the whole image
    u8 threshold;       // +0x0C the global threshold (middle between darkest and lightest pixel)
};
ASSERT_SIZE(Params, 0x10);

// the camera image as the reader sees it
struct SourceImage
{
    s32 width;          // +0x00
    s32 height;         // +0x04
    const u8* pixels;   // +0x08 YUYV
};
ASSERT_SIZE(SourceImage, 0xC);

struct GrayImage
{
    s32 width;          // +0x00
    s32 height;         // +0x04
    u8* pixels;         // +0x08
};
ASSERT_SIZE(GrayImage, 0xC);

struct BitImage
{
    s32 width;          // +0x00
    s32 height;         // +0x04
    u32* bits;          // +0x08 bit (y * width + x) set = light pixel
};
ASSERT_SIZE(BitImage, 0xC);

// a finder pattern (one of the three squares in the corners)
struct FinderPattern
{
    f64 x;              // +0x00 centre
    f64 y;              // +0x08
    f64 _10;            // +0x10
    f64 moduleSize;     // +0x18
};
ASSERT_SIZE(FinderPattern, 0x20);

// a pattern centre in the image
struct PatternPoint
{
    f32 x;              // +0x00
    f32 y;              // +0x04
    f32 moduleSize;     // +0x08 -1 = not found (alignment pattern)
    s32 count;          // +0x0C how often the pattern was found
};
ASSERT_SIZE(PatternPoint, 0x10);

// a growing list of pattern candidates
struct PatternList
{
    s32 capacity;         // +0x00
    s32 count;            // +0x04
    PatternPoint* items;  // +0x08
};
ASSERT_SIZE(PatternList, 0xC);

// where the symbol is in the image
struct SymbolPosition
{
    s32 _00;                  // +0x00
    s32 isFound;              // +0x04
    s32 left;                 // +0x08 bounding box
    s32 right;                // +0x0C
    s32 top;                  // +0x10
    s32 bottom;               // +0x14
    PatternPoint points[4];   // +0x18 the three finder patterns and the alignment pattern (or the fourth corner)
};
ASSERT_SIZE(SymbolPosition, 0x58);

// --------------------------------------------------------------- the symbol

// the sampled modules of the symbol (0 = light)
struct ModuleMatrix
{
    s32 moduleNum;      // +0x00 size * size
    s32 size;           // +0x04 modules per side (version * 4 + 17)
    u8* modules;        // +0x08
};
ASSERT_SIZE(ModuleMatrix, 0xC);

// the module index; 0 for positions outside the matrix (and size itself counts as inside)
inline s32 GetModuleIndex(const ModuleMatrix* matrix, s32 row, s32 column)
{
    if (matrix == NULL || column < 0 || column > matrix->size || row < 0 || row > matrix->size)
    {
        return 0;
    }
    return row * matrix->size + column;
}

struct FormatInformation
{
    u8 errorCorrectionLevel;  // +0x00 M = 0, L = 1, H = 2, Q = 3
    s8 maskPattern;           // +0x01
    u8 _02[2];                // +0x02
};
ASSERT_SIZE(FormatInformation, 0x4);

// a Reed-Solomon block
struct Block
{
    s32 dataNum;    // +0x00
    s32 totalNum;   // +0x04 data and error correction codewords
    u8* data;       // +0x08
};
ASSERT_SIZE(Block, 0xC);

struct Codewords
{
    s32 size;       // +0x00
    s32 blockNum;   // +0x04
    u8* data;       // +0x08
    Block* blocks;  // +0x0C
};
ASSERT_SIZE(Codewords, 0x10);

// state of one read
struct Context
{
    u8 _00[0x44];                // +0x00
    s32 symbolSize;              // +0x44
    FinderPattern* finders;      // +0x48 the three finder patterns (top left, top right, bottom left)
    void* _4C;                   // +0x4C
    s32 version;                 // +0x50
    BitImage bitImage;           // +0x54
    ModuleMatrix matrix;         // +0x60
    Codewords codewords;         // +0x6C
    FormatInformation format;    // +0x7C
};
ASSERT_SIZE(Context, 0x80);

// result of the sampling
struct SampleState
{
    s32 symbolSize;     // +0x00
    u8* modules;        // +0x04 second sampling (by perspective), freed with ReleaseModules
    f32 moduleSize;     // +0x08 module size of the largest finder pattern
};
ASSERT_SIZE(SampleState, 0xC);

// mode indicators of the segments
enum Mode
{
    MODE_NUMERIC = 1,
    MODE_ALPHANUMERIC = 2,
    MODE_STRUCTURED_APPEND = 3,
    MODE_8BIT = 4,
    MODE_KANJI = 8,
};

// a decoded data segment
struct Segment
{
    u8 _00[0x10];   // +0x00
    s32 length;     // +0x10
    u8* data;       // +0x14
    u8 mode;        // +0x18 Mode
};
ASSERT_SIZE(Segment, 0x1C);

// reads the data codewords bit by bit
struct BitSource
{
    s32 size;         // +0x00
    s32 byteOffset;   // +0x04
    s32 bitOffset;    // +0x08
    const u8* bytes;  // +0x0C
};
ASSERT_SIZE(BitSource, 0x10);

// a polynomial over GF(256) for the Reed-Solomon decoder
struct Polynomial
{
    s32* coefficients;  // +0x00
    s32 length;         // +0x04
    s32 capacity;       // +0x08
};
ASSERT_SIZE(Polynomial, 0xC);

// GF(256) of the Reed-Solomon codes
struct GaloisField
{
    s32 primitive;  // +0x000
    s32 exp[256];   // +0x004
    s32 log[256];   // +0x404
};
ASSERT_SIZE(GaloisField, 0x804);

extern GaloisField s_GaloisField;

DECOMP_EMIT_COPY inline s32 GfMultiply(s32 a, s32 b) // 0x0016A07C
{
    if (a == 0 || b == 0)
    {
        return 0;
    }
    if (a == 1)
    {
        return b;
    }
    if (b == 1)
    {
        return a;
    }
    return s_GaloisField.exp[(s_GaloisField.log[a] + s_GaloisField.log[b]) % 255];
}

inline s32 GfInverse(s32 a)
{
    if (a == 0)
    {
        return -1;
    }
    return s_GaloisField.exp[255 - s_GaloisField.log[a]];
}

inline s32 GfLog(s32 a)
{
    if (a == 0)
    {
        return -1;
    }
    return s_GaloisField.log[a];
}

inline void FinalizePolynomial(Polynomial* polynomial)
{
    polynomial->capacity = 0;
    polynomial->length = 0;
    if (polynomial->coefficients != NULL)
    {
        Free(polynomial->coefficients);
        polynomial->coefficients = NULL;
    }
}

extern Params s_Params;
extern SourceImage s_SourceImage;
extern GrayImage s_GrayImage;
extern Context s_Context;
extern SampleState s_SampleState;
extern Segment* s_pSegments;
extern BitSource s_BitSource;
extern FinderPattern s_FinderPatterns[3];
extern Polynomial s_ZeroPolynomial;
extern Polynomial s_OnePolynomial;
extern Polynomial s_SigmaOmega[2];

// frees the modules of the second sampling
inline void ReleaseModules()
{
    Yield();
    if (s_SampleState.modules != NULL)
    {
        Free(s_SampleState.modules);
        s_SampleState.modules = NULL;
    }
}

// --------------------------------------------------------------- second sampling

struct Point2i
{
    s32 x;  // +0x00
    s32 y;  // +0x04
};
ASSERT_SIZE(Point2i, 0x8);

struct Point2f
{
    f32 x;  // +0x00
    f32 y;  // +0x04
};
ASSERT_SIZE(Point2f, 0x8);

struct Point2d
{
    f64 x;  // +0x00
    f64 y;  // +0x08
};
ASSERT_SIZE(Point2d, 0x10);

// the outline of a finder pattern
struct FinderArea
{
    Point2f corners[4];     // +0x00 outer corners (top left, top right, bottom left, bottom right)
    Point2f edges[4];       // +0x20 edge points along both symbol axes
    Point2i edgePixels[4];  // +0x40
    f32 widths[2];          // +0x60 along both axes
    Point2f centre;         // +0x68
};
ASSERT_SIZE(FinderArea, 0x70);

struct SampleInfo
{
    Point2d* rowLines;      // +0x00 [9][size] points of the module rows through the finder and alignment patterns
    Point2d* columnLines;   // +0x04 [9][size] the same for the columns
    s32 version;            // +0x08 -1 = unknown
    s32 width;              // +0x0C of the bit image
    s32 height;             // +0x10
    s32 versionH;           // +0x14 from the distance top left to top right
    s32 versionV;           // +0x18 from the distance top left to bottom left
};
ASSERT_SIZE(SampleInfo, 0x1C);

// module steps of the grid (and how they change per module)
struct SampleSteps
{
    Point2d deltaX;  // +0x00
    Point2d deltaY;  // +0x10
    Point2d stepX;   // +0x20
    Point2d stepY;   // +0x30
};
ASSERT_SIZE(SampleSteps, 0x40);

extern SampleInfo s_SampleInfo;
extern FinderArea s_FinderAreas[3];
extern SampleSteps s_SampleSteps;
// the alignment pattern positions other than 6 per version: number and positions
extern const u8 s_AlignmentCentres[40][7];
// the search spiral of RefineAlignment
extern const s8 s_SpiralOffsets[121][2];

bool FindEdge(Point2i* out, Point2f centre, f32 angle);
bool IntersectLines(Point2f* out, Point2f a1, Point2f a2, Point2f b1, Point2f b2);
bool LocateCorners();
void TraceCornerA(Point2i start, Point2i* out, Point2f centre);
void TraceCornerB(Point2i start, Point2i* out, Point2f centre);
bool BuildSampleLines();
void InterpolateRowLine(s32 from, s32 to, s32 line);
void InterpolateColumnLine(s32 from, s32 to, s32 line);
void ExtrapolateRowLine(s32 line, s32 from, s32 to, s32 target);
void ExtrapolateColumnLine(s32 line, s32 from, s32 to, s32 target);
void RefineAlignment(Point2d* point);
bool CorrectSampleLinesLarge();
bool CorrectSampleLinesSmall();
f32 MeasureRun(s32 fromX, s32 fromY, s32 toX, s32 toY);
f32 MeasureRunBothWays(s32 fromX, s32 fromY, s32 toX, s32 toY);
f32 MeasureModuleSize(const FinderPattern* pattern, const FinderPattern* other);

// --------------------------------------------------------------- alignment patterns

// the search area of SearchAlignment
struct AlignmentFinder
{
    s32 startX;               // +0x00
    s32 startY;               // +0x04
    s32 width;                // +0x08
    s32 height;               // +0x0C
    f32 moduleSize;           // +0x10
    PatternList* candidates;  // +0x14
};
ASSERT_SIZE(AlignmentFinder, 0x18);

extern AlignmentFinder s_AlignmentFinder;
extern PatternPoint s_FinderCentres[3];

s32 SearchAlignment(const FinderPattern* finder0, const FinderPattern* finder1, const FinderPattern* finder2,
                    PatternPoint* out);
bool FoundAlignmentCross(const s32* counts);
s32 HandleAlignmentCenter(const s32* counts, s32 i, s32 j, PatternPoint* point);
void InitializeAlignmentFinder(s32 startX, s32 startY, s32 width, s32 height, f32 moduleSize);
f32 PointDistance(Point2f a, Point2f b);
int CompareAlignmentCandidates(const void* a, const void* b);

// --------------------------------------------------------------- grid sampling

// a perspective transform (x, y, 1) * a
struct Transform
{
    f32 a11;  // +0x00
    f32 a12;  // +0x04
    f32 a13;  // +0x08
    f32 a21;  // +0x0C
    f32 a22;  // +0x10
    f32 a23;  // +0x14
    f32 a31;  // +0x18
    f32 a32;  // +0x1C
    f32 a33;  // +0x20
};
ASSERT_SIZE(Transform, 0x24);

void SquareToQuadrilateral(Transform* out, f32 x0, f32 y0, f32 x1, f32 y1, f32 x2, f32 y2, f32 x3, f32 y3);

// --------------------------------------------------------------- the reader

s32 DetectSymbol(Params* params, const ImageInfo* image, s32* dataSize, s32* version, s32* count,
                 SymbolPosition* position);
s32 DecodeSymbol(u8* data, s32 dataSize, s32* version);
s32 ExtractData(u8* data, s32 dataSize, s32* version, u8* out, s32* appendIndex, s32* appendCount,
                s32* appendParity);
void Begin();
void Reset();
void End();
void Release();
bool Cleanup();

// the steps
bool PrepareImage(Params* params, const ImageInfo* image, SymbolPosition* position);
bool FindFinderPatterns(s32* count, SymbolPosition* position);
bool EstimateVersion(s32* version, PatternPoint* alignment, SymbolPosition* position);
bool SampleModules(const PatternPoint* alignment);
bool ReadSymbolInformation(s32* version);
bool UnmaskSymbol();
bool ReadCodewords();
bool ReadBlocks(s32* dataSize);
bool ReadData(u8* data, s32 dataSize, s32* version);

// images
bool ExtractLuminance(const SourceImage* src, GrayImage* dst, Params* params, const Rect* rect);
bool Binarize(Params* params, const GrayImage* gray, BitImage* out, SymbolPosition* position);

// patterns
s32 LocateFinderPatterns(const BitImage* bitImage, FinderPattern** finders, s32* count, SymbolPosition* position);
s32 SearchFinderPatterns(const BitImage* bitImage, FinderPattern** finders, s32* count);
bool IsLight(s32 x, s32 y);
bool IsDark(s32 x, s32 y);
PatternList* CreatePatternList(s32 capacity);
bool SetPatternListItem(PatternList* list, const PatternPoint* point, s32 index);
int ComparePatternCount(const void* a, const void* b);
bool CheckRatios(const s32* counts);
void SelectTriangle(PatternList* list, s32 num, s32 width, s32 height);
bool SelectBestPatterns(PatternList* candidates, PatternList* out, s32 width, s32 height);
bool HandlePossibleCenter(const s32* counts, s32 y, s32 end, const BitImage* image, PatternList* list);
void SetFinderCentres(s32 width, s32 height, const FinderPattern* finder0, const FinderPattern* finder1,
                      const FinderPattern* finder2);
u8* SampleByPerspective();
f32 EstimateModuleSize(const FinderPattern* finder0, const FinderPattern* finder1, const FinderPattern* finder2);
s32 EstimateSymbolSize(const FinderPattern* finder0, const FinderPattern* finder1, const FinderPattern* finder2,
                       f32 moduleSize);
s32 FindAlignmentPattern(s32 x, s32 y, const FinderPattern* finder0, const FinderPattern* finder1,
                         const FinderPattern* finder2, PatternPoint* alignment, f32 moduleSize, f32 radius);
void ComputeBoundingBox(SymbolPosition* position, s32 width, s32 height);
u8* SampleGrid(const FinderPattern* finder0, const FinderPattern* finder1, const FinderPattern* finder2,
               const PatternPoint* alignment, s32 symbolSize);

// symbol
void FillRect(ModuleMatrix* matrix, s32 row, s32 column, s32 rowNum, s32 columnNum);
bool ReadVersion(const ModuleMatrix* matrix, s32* version);
s32 DecodeVersion(s32 bits);
bool ReadFormat(const ModuleMatrix* matrix, FormatInformation* format);
s32 DecodeFormat(s32 bits, s8* maskPattern, u8* errorCorrectionLevel);
bool Unmask(ModuleMatrix* matrix, const FormatInformation* format);
bool ExtractCodewords(const ModuleMatrix* matrix, const s32* version, const FormatInformation* format,
                      Codewords* codewords);
bool SplitBlocks(const s32* version, const FormatInformation* format, Codewords* codewords);
bool ResetContext(Context* context);

// error correction
s32 CorrectBlocks(Codewords* codewords);
s32 DecodeReedSolomon(s32* data, s32 totalNum, s32 ecNum);
bool InitializePolynomial(Polynomial* polynomial, s32 value);
bool CopyPolynomial(Polynomial* dst, const Polynomial* src);
bool AddPolynomials(Polynomial* dst, const Polynomial* a, const Polynomial* b);
bool MultiplyPolynomials(Polynomial* dst, const Polynomial* a, const Polynomial* b);
bool EuclideanStep(Polynomial* r, Polynomial* rLast, Polynomial* s, Polynomial* sLast, Polynomial* t,
                   Polynomial* tLast);
Polynomial* RunEuclideanAlgorithm(Polynomial* a, Polynomial* b, s32 r);

// data
s32 ParseSegments(u8* data, s32 dataSize, s32* version, u8* out);
s32 ReadBits(s32 bitNum);

} // namespace detail
} // namespace qrd
} // namespace mw
