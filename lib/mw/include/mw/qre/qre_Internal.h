#pragma once

// The QR code writer behind mw::qre::QREncoder. The binary has no names for it: all names in this header
// and in qre_*.cpp are ours.

#include "decomp.h"

namespace mw {
namespace qre {
namespace detail {

// --------------------------------------------------------------- work memory

// the work memory given to QREncoder::Initialize
struct WorkState
{
    u8* work;         // +0x00
    u32 workSize;     // +0x04
    u32 workUsed;     // +0x08
    u32 blockNum;     // +0x0C
};
ASSERT_SIZE(WorkState, 0x10);

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

void ResetMemory();
void* Allocate(u32 size);
void Free(void* ptr);
void* Reallocate(void* ptr, u32 size);

// --------------------------------------------------------------- errors

// error codes (also the messages of the encoder class)
enum Error
{
    ERROR_NONE = 0,
    ERROR_INVALID_MODE = 1,
    ERROR_DATA_TOO_LARGE = 2,
    ERROR_INVALID_VERSION = 7,
    ERROR_INVALID_CHARACTER = 8,
};

void ResetError();
u8 GetError();
void SetError(u8 error, bool force);

// --------------------------------------------------------------- tables

// mode indicators
enum Mode
{
    MODE_NUMERIC = 1,
    MODE_ALPHANUMERIC = 2,
    MODE_STRUCTURED_APPEND = 3,
    MODE_8BIT = 4,
    MODE_KANJI = 8,
    MODE_AUTO = 0xFF,
};

struct BlockGroup
{
    u8 totalNum;        // +0x00 codewords per block
    u8 dataNum;         // +0x01
    u8 correctableNum;  // +0x02
};
ASSERT_SIZE(BlockGroup, 0x3);

// the error correction of one level of a version
struct ErrorCorrectionInfo
{
    s16 ecCodewordNum;        // +0x00
    s8 blockNum;              // +0x02
    s8 groupBlockNum[2];      // +0x03
    BlockGroup groups[2];     // +0x05
    u8 _0B;                   // +0x0B
};
ASSERT_SIZE(ErrorCorrectionInfo, 0xC);

struct VersionInfo
{
    s16 totalCodewordNum;                // +0x00
    u8 size;                             // +0x02 modules per side
    s8 alignmentNum;                     // +0x03
    ErrorCorrectionInfo levels[4];       // +0x04 by the level bits of the format information (M, L, H, Q)
    u8 alignmentX[46];                   // +0x34
    u8 alignmentY[46];                   // +0x62
};
ASSERT_SIZE(VersionInfo, 0x90);

// constants of the data stream
struct StreamInfo
{
    u8 appendMode;    // +0x00 the mode indicator of structured append
    u8 _01;           // +0x01
    u16 padBytes;     // +0x02 0xEC, 0x11
    s32 dataBitNum;   // +0x04 capacity of the current version and level
};
ASSERT_SIZE(StreamInfo, 0x8);

extern const VersionInfo s_VersionInfos[40];
extern StreamInfo s_StreamInfo;
extern s32 s_AlphanumericChars[45];

bool GetTotalCodewordNum(s32 version, s32* num);
bool GetAlignmentPositions(s32 version, s32* num, s32* x, s32* y);
bool GetErrorCorrectionInfo(s32 version, u8 level, ErrorCorrectionInfo* info);
bool GetSymbolSize(s32 version, s32* size);

// --------------------------------------------------------------- data

// the input data
struct DataSpec
{
    s32 size;        // +0x00
    const u8* data;  // +0x04
};
ASSERT_SIZE(DataSpec, 0x8);

// a run of data in one mode
struct Segment
{
    s32 start;   // +0x00
    s32 end;     // +0x04
    s32 count;   // +0x08 characters
    u8 mode;     // +0x0C Mode
};
ASSERT_SIZE(Segment, 0x10);

// mask pattern and error correction level of a symbol
struct Format
{
    u8 maskPattern;  // +0x00 0xFF = the best one
    u8 level;        // +0x01 error correction level as in the format information (M = 0, L = 1, H = 2, Q = 3)
};
ASSERT_SIZE(Format, 0x2);

// one symbol of the (structured append) series
struct Part
{
    s32 index;            // +0x00 1-based
    s32 count;            // +0x04
    u8 parity;            // +0x08
    s32 version;          // +0x0C requested version (0 = smallest fitting)
    s32 resultVersion;    // +0x10
    DataSpec data;        // +0x14
    s32 cellSize;         // +0x1C pixels per module
    s32 quietZone;        // +0x20 modules
    Format format;        // +0x24 requested
    Format usedFormat;    // +0x26
    u8 _28[3];            // +0x28
    u8 mode;              // +0x2B Mode
    u8 parityCopy;        // +0x2C
};
ASSERT_SIZE(Part, 0x30);

s32 DetectMode(const u8* data, u32 size);
s32 GetCharCountBits(s32 version, s32 mode);
s32 GetBitLength(s32 version, s32 mode, s32 length);
s32 GetAlphanumericIndex(s32 c);
bool ComputeParity(const DataSpec* data, u8* parity);
bool ChooseVersion(s32* version, u8* level, u8* mode, DataSpec* data, Part* part);
bool BuildDataStream(s32* version, u8* level, u8* mode, DataSpec* data, s32* codewordNum, u8** stream, Part* part);
s32 SplitSegments(s32 version, const DataSpec* data, Segment** segments);

// --------------------------------------------------------------- error correction

struct RsCode
{
    s32 rootNum;    // +0x00 error correction codewords
    s32 firstRoot;  // +0x04
};
ASSERT_SIZE(RsCode, 0x8);

// a Reed-Solomon block of the symbol
struct RsBlock
{
    s32 totalNum;     // +0x00
    s32 dataNum;      // +0x04
    s32 blockNum;     // +0x08 of the symbol
    s32 index;        // +0x0C
    u8* codewords;    // +0x10
};
ASSERT_SIZE(RsBlock, 0x14);

extern RsCode s_RsCode;
extern s32 s_AlphaTo[256];
extern s32 s_IndexOf[256];
extern s32 s_Generator[256];

void EncodeBch(s32 codeLength, s32 dataLength, const s8* data, s8* out);
void EncodeRs(const u8* data, u8* parity);
void InitializeRs(s32 rootNum, s32 firstRoot);
bool AddErrorCorrection(s32 version, u8 level, u8* stream);

// --------------------------------------------------------------- the symbol

// value of a module that a function pattern takes
const u8 MODULE_RESERVED = 0x80;

bool PlaceCodewords(s32 version, s32 codewordNum, const u8* stream, u8*** out);
bool WriteFormatInformation(s32 version, Format format, u8** matrix);
bool SelectMask(s32 version, Format* format, u8 level, u8** matrix);
bool DrawFinderPattern(u8** matrix, s32 column, s32 row);
bool ComputeRuns(s32** runs, s32 size, u8** matrix, bool isVertical);
bool DrawFunctionPatterns(s32 version, u8** matrix);

} // namespace detail
} // namespace qre
} // namespace mw
