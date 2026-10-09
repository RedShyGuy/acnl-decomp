#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/ngc/CTR/ngc_ProfanityFilterBase.h"
#include "nn/os/os_MemoryBlock.h"

namespace nn {
namespace ngc {
namespace CTR {
// The profanity filter: checks words against the word lists of the shared contents
// (ngword:/<n>.txt, a regular expression per line) and masks the words of the lists in a text.
// The results of the checks have a bit per word list. Initialize takes a work buffer of
// WORK_MEMORY_SIZE. The names of the virtual functions other than CheckProfanityWords(..., bool,
// ...) and of the members are ours.
// RTTI N2nn3ngc3CTR15ProfanityFilterE @ 0x008CF7B0
// vtable 0x008FFDB8 (vptr 0x008FFDC0), offset_to_top 0, 9 entries
class ProfanityFilter : public ::nn::ngc::CTR::ProfanityFilterBase
{
public:
    static const u32 WORD_COUNT_MAX = 16;
    static const u32 POOL_MEMORY_SIZE = 0xB000;
    static const u32 FILE_BUFFER_SIZE = 0x4000;
    static const u32 MOUNT_MEMORY_SIZE = 0x800;
    static const u32 CONVERT_MEMORY_SIZE = 0x800;
    static const u32 WORK_MEMORY_SIZE = POOL_MEMORY_SIZE + FILE_BUFFER_SIZE + MOUNT_MEMORY_SIZE + CONVERT_MEMORY_SIZE;

    ProfanityFilter(); // 0x003E1504 | fefates:bytes [tier B]
    virtual ~ProfanityFilter();
    // the version of the word lists (ngword:/version.dat), 0 if it cannot be read
    virtual u32 GetContentVersion(); // 0x003E0240 slot 0x08
    // checks the words against all word lists
    virtual nn::Result CheckProfanityWords(unsigned int* results, const wchar_t** words, unsigned int count); // 0x003E048C slot 0x0C
    // checks the words against one word list
    virtual nn::Result CheckProfanityWords(unsigned int* results, nn::ngc::CTR::ProfanityFilterPatternList list, const wchar_t** words,
                                           unsigned int count); // 0x003E0360 slot 0x10
    // checks the words against the word lists of the region of the system
    virtual nn::Result CheckProfanityWords(unsigned int* results, bool flag, const wchar_t** words, unsigned int count); // 0x003E0640 slot 0x14 | fefates:bytes
    // masks the words of all word lists in text; *count gets the number of masked words
    virtual nn::Result MaskProfanityWordsInText(int* count, wchar_t* text); // 0x003E0C08 slot 0x18
    virtual nn::Result MaskProfanityWordsInText(int* count, nn::ngc::CTR::ProfanityFilterPatternList list, wchar_t* text); // 0x003E0AC8 slot 0x1C
    virtual nn::Result MaskProfanityWordsInText(int* count, bool flag, wchar_t* text); // 0x003E0DD4 slot 0x20

    nn::Result Initialize(uptr workMemory); // 0x003E01E4 | fefates:bytes [tier B]
    // sets the bit of the word list m_PatternList in results for the words that the regular
    // expression pattern matches
    void CheckWords(unsigned int* results, const wchar_t* pattern, unsigned int length, const wchar_t** words, unsigned int count); // 0x003DFFFC | fefates:bytes [tier B]
    nn::Result MountSharedContents(); // 0x003E0834 | fefates:bytes [tier B]
    nn::Result CheckProfanityWords_Impl(unsigned int* results, nn::fs::FileInputStream* stream, const wchar_t** words, unsigned int count); // 0x003E08D0 | fefates:bytes [tier B]
    nn::Result MaskProfanityWordsInText_Impl(int* count, wchar_t* text, wchar_t* converted, u8* textMap, nn::fs::FileInputStream* stream); // 0x003E0FDC (name is ours)
    // masks the matches of the regular expression pattern in text and converted (name is ours)
    void MaskWord(int* count, wchar_t* text, wchar_t* converted, u8* textMap, const wchar_t* pattern, bool isSkipSpace); // 0x003E1178 (name is ours)

    // (inline in the original, with a copy out of line)
    nn::Result CheckArguments_Word(const unsigned int* results, const wchar_t** words, unsigned int count) const; // 0x0072EC20 | fefates:bytes [tier B]

    nn::os::MemoryBlock m_MemoryBlock;  // 0x04
    bool m_IsMemoryBlockUsed;           // 0x18, the work memory is m_MemoryBlock
    bool m_IsMounted;                   // 0x19
    bool m_IsAtSignCheckSkipped;        // 0x1A, words with @ (mail addresses) are not found
    u8 m_PatternList;                   // 0x1B, ProfanityFilterPatternList, the one being checked
    uptr m_WorkMemory;                  // 0x1C
    uptr m_PoolMemory;                  // 0x20, ProfanityFilterTemporaryPool
    wchar_t* m_FileBuffer;              // 0x24, a word list
    void* m_MountMemory;                // 0x28
    wchar_t* m_ConvertMemory;           // 0x2C, the converted words (WORD_LENGTH_MAX each) / text
    bool m_IsMaskEveryCharacter;        // 0x30, a * per character (else one * per word)
};
ASSERT_SIZE(ProfanityFilter, 0x34);

namespace detail {
// level, summary, module 58 and description after 3dbrew
// (permanent, nothing happened, not initialized)
const bit32 RESULT_NOT_INITIALIZED = 0xD820EBF8;
// (usage, invalid argument, invalid pointer)
const bit32 RESULT_INVALID_POINTER = 0xE0E0EBF6;
// (usage, invalid argument, invalid size): more than WORD_COUNT_MAX words
const bit32 RESULT_INVALID_SIZE = 0xE0E0EBEC;
// (usage, invalid argument, already done): initialized already
const bit32 RESULT_ALREADY_DONE = 0xE0E0EBEB;
// (permanent, nothing happened, invalid size): a word list or the mount memory has the wrong size
const bit32 RESULT_WRONG_SIZE = 0xD820EBEC;
// (fatal, nothing happened, busy): a word list is broken (no BOM, a 0 inside)
const bit32 RESULT_BROKEN_DATA = 0xF820EBF0;
} // namespace detail


// the digits in text (also the full width, enclosed, Roman, super- and subscript ones); -1 for null
int CountNumbers(const wchar_t* text); // 0x003DFEF8 | fefates:bytes [tier B]
} // namespace CTR
} // namespace ngc
} // namespace nn
