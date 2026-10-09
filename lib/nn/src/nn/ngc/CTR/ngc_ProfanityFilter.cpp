#include "nn/ngc/CTR/ngc_ProfanityFilter.h"
#include "nn/fs/fs_Api.h"
#include "nn/fs/fs_FileInputStream.h"
#include "nn/ngc/ngc_RegexDfaConverter.h"
#include "nn/ngc/ngc_RegexFastMatch.h"
#include "nn/ngc/ngc_RegexMatch.h"
#include "nn/ngc/ngc_RegexNfaParser.h"
#include "nn/ngc/ngc_RegexScanner.h"
#include <new>
#include <wchar.h>

namespace nn {
namespace ngc {
namespace CTR {
namespace {
const char MOUNT_NAME[] = "ngword:";
// the shared contents of the word lists
const u64 NG_WORD_PROGRAM_ID = 0x000400DB00010302ULL;
const size_t PATH_LENGTH_MAX = 64;
// converted text, regular expression and text map in m_ConvertMemory
const int TEXT_LENGTH_MAX = 512;
const u32 REGULAR_EXPRESSION_OFFSET = 1024;
const u32 TEXT_MAP_OFFSET = 1536;
// GetPatternListsFromRegion gives at most two
const int PATTERN_LIST_COUNT_MAX = 2;

// (in the code of the original)
const wchar_t EMPTY_TEXT[] = L"";

// mail addresses
// 0x008A34D0
const wchar_t s_MailAddressPattern[] = L"[a-zA-Z0-9][a-zA-Z0-9_\\.\\-]+@[a-zA-Z0-9][a-zA-Z0-9_\\.\\-]+\\.[a-zA-Z0-9]+";

template <typename T>
inline T* NewInPool(ProfanityFilterTemporaryPool& pool)
{
    void* buffer = pool.Allocate((sizeof(T) + ProfanityFilterTemporaryPool::UNIT_SIZE - 1) / ProfanityFilterTemporaryPool::UNIT_SIZE);
    return new (buffer) T;
}

// the arguments of the masking functions (name is ours)
inline nn::Result CheckArgumentsText(const ProfanityFilter* filter, const wchar_t* text)
{
    if (filter->m_WorkMemory == 0) {
        return nn::Result(detail::RESULT_NOT_INITIALIZED);
    }
    if (text == NULL) {
        return nn::Result(detail::RESULT_INVALID_POINTER);
    }
    return nn::Result();
}
} // namespace

// 0x0072EC20 | fefates:bytes [tier B]
nn::Result nn::ngc::CTR::ProfanityFilter::CheckArguments_Word(const unsigned int* results, const wchar_t** words, unsigned int count) const
{
    if (m_WorkMemory == 0) {
        return nn::Result(detail::RESULT_NOT_INITIALIZED);
    }
    if (results == NULL || words == NULL) {
        return nn::Result(detail::RESULT_INVALID_POINTER);
    }
    if (count > WORD_COUNT_MAX) {
        return nn::Result(detail::RESULT_INVALID_SIZE);
    }
    return nn::Result();
}

// 0x003DFFFC | fefates:bytes [tier B]
void nn::ngc::CTR::ProfanityFilter::CheckWords(unsigned int* results, const wchar_t* pattern, unsigned int length, const wchar_t** words,
                                               unsigned int count)
{
    ProfanityFilterTemporaryPool pool;
    pool.Initialize(m_PoolMemory);
    RegexScanner* scanner = NewInPool<RegexScanner>(pool);
    RegexFastMatch* fastMatch = NewInPool<RegexFastMatch>(pool);
    RegexNfaParser* parser = NewInPool<RegexNfaParser>(pool);
    RegexDfaConverter* converter = NewInPool<RegexDfaConverter>(pool);
    RegexMatch* match = NewInPool<RegexMatch>(pool);
    if (scanner->LexicalAnalysis(&pool, pattern, length) != RegexScanner::RESULT_SUCCESS) {
        return;
    }
    // the simple patterns go without automaton
    if (fastMatch->MatchFast(&scanner->m_Tokens, EMPTY_TEXT) != RegexFastMatch::RESULT_NOT_SIMPLE) {
        for (u32 i = 0; i < count; i++) {
            if (results[i] & (1 << m_PatternList)) {
                continue;
            }
            if (fastMatch->MatchFast(&scanner->m_Tokens, words[i]) == RegexFastMatch::RESULT_MATCH) {
                results[i] |= 1 << m_PatternList;
            }
        }
        return;
    }
    if (parser->Parse(&pool, &scanner->m_Tokens) != RegexNfaParser::RESULT_SUCCESS) {
        return;
    }
    if (!converter->Convert(&pool, &parser->m_States, parser->m_End)) {
        return;
    }
    for (u32 i = 0; i < count; i++) {
        if (results[i] & (1 << m_PatternList)) {
            continue;
        }
        if (match->IsMatch(&converter->m_DfaStates, words[i])) {
            results[i] |= 1 << m_PatternList;
        }
    }
}

// 0x003E01E4 | fefates:bytes [tier B]
nn::Result nn::ngc::CTR::ProfanityFilter::Initialize(uptr workMemory)
{
    if (workMemory == 0) {
        return nn::Result(detail::RESULT_INVALID_POINTER);
    }
    if (m_IsMemoryBlockUsed) {
        return nn::Result(detail::RESULT_ALREADY_DONE);
    }
    m_WorkMemory = workMemory;
    m_PoolMemory = workMemory;
    m_FileBuffer = reinterpret_cast<wchar_t*>(workMemory + POOL_MEMORY_SIZE);
    m_MountMemory = reinterpret_cast<void*>(workMemory + POOL_MEMORY_SIZE + FILE_BUFFER_SIZE);
    m_ConvertMemory = reinterpret_cast<wchar_t*>(workMemory + POOL_MEMORY_SIZE + FILE_BUFFER_SIZE + MOUNT_MEMORY_SIZE);
    nn::Result result = MountSharedContents();
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result();
}

// 0x003E0240 slot 0x08
u32 nn::ngc::CTR::ProfanityFilter::GetContentVersion()
{
    nn::fs::FileInputStream stream;
    if (stream.TryInitialize(L"ngword:/version.dat").IsFailure()) {
        return 0;
    }
    if (stream.GetSize() != sizeof(u32)) {
        return 0;
    }
    u32 version = 0;
    if (stream.Read(&version, sizeof(version)) != sizeof(u32)) {
        return 0;
    }
    return version;
}

// 0x003E0360 slot 0x10
nn::Result nn::ngc::CTR::ProfanityFilter::CheckProfanityWords(unsigned int* results, nn::ngc::CTR::ProfanityFilterPatternList list,
                                                              const wchar_t** words, unsigned int count)
{
    nn::Result result = CheckArguments_Word(results, words, count);
    if (result.IsFailure()) {
        return result;
    }
    for (u32 i = 0; i < count; i++) {
        results[i] = 0;
    }
    wchar_t path[PATH_LENGTH_MAX];
    m_PatternList = list;
    swprintf(path, PATH_LENGTH_MAX, L"ngword:/%d.txt", list);
    nn::fs::FileInputStream stream;
    result = stream.TryInitialize(path);
    if (result.IsSuccess()) {
        result = CheckProfanityWords_Impl(results, &stream, words, count);
    }
    return result;
}

// 0x003E048C slot 0x0C
nn::Result nn::ngc::CTR::ProfanityFilter::CheckProfanityWords(unsigned int* results, const wchar_t** words, unsigned int count)
{
    nn::Result result = CheckArguments_Word(results, words, count);
    if (result.IsFailure()) {
        return result;
    }
    for (u32 i = 0; i < count; i++) {
        results[i] = 0;
    }
    // every word list there is
    for (int list = 0;; list++) {
        wchar_t path[PATH_LENGTH_MAX];
        swprintf(path, PATH_LENGTH_MAX, L"ngword:/%d.txt", list);
        nn::fs::FileInputStream stream;
        if (stream.TryInitialize(path).IsFailure()) {
            return nn::Result();
        }
        m_PatternList = list;
        result = CheckProfanityWords_Impl(results, &stream, words, count);
        if (result.IsFailure()) {
            return result;
        }
    }
}

// 0x003E0640 slot 0x14 | fefates:bytes
nn::Result nn::ngc::CTR::ProfanityFilter::CheckProfanityWords(unsigned int* results, bool flag, const wchar_t** words, unsigned int count)
{
    nn::Result result = CheckArguments_Word(results, words, count);
    if (result.IsFailure()) {
        return result;
    }
    for (u32 i = 0; i < count; i++) {
        results[i] = 0;
    }
    ProfanityFilterPatternList lists[PATTERN_LIST_COUNT_MAX];
    int listCount;
    GetPatternListsFromRegion(lists, &listCount, flag);
    for (int i = 0; i < listCount; i++) {
        wchar_t path[PATH_LENGTH_MAX];
        swprintf(path, PATH_LENGTH_MAX, L"ngword:/%d.txt", lists[i]);
        nn::fs::FileInputStream stream;
        result = stream.TryInitialize(path);
        if (result.IsFailure()) {
            return result;
        }
        m_PatternList = lists[i];
        result = CheckProfanityWords_Impl(results, &stream, words, count);
        if (result.IsFailure()) {
            return result;
        }
    }
    return nn::Result();
}

// 0x003E0834 | fefates:bytes [tier B]
nn::Result nn::ngc::CTR::ProfanityFilter::MountSharedContents()
{
    if (nn::fs::GetContentRequiredMemorySize(nn::fs::MEDIA_TYPE_NAND, NG_WORD_PROGRAM_ID, 0, 1, 1) > MOUNT_MEMORY_SIZE) {
        return nn::Result(detail::RESULT_WRONG_SIZE);
    }
    nn::Result result = nn::fs::MountContent(MOUNT_NAME, nn::fs::MEDIA_TYPE_NAND, NG_WORD_PROGRAM_ID, 0, 1, 1, m_MountMemory, MOUNT_MEMORY_SIZE, false);
    if (result.IsSuccess()) {
        m_IsMounted = true;
        return nn::Result();
    }
    return result;
}

// 0x003E08D0 | fefates:bytes [tier B]
nn::Result nn::ngc::CTR::ProfanityFilter::CheckProfanityWords_Impl(unsigned int* results, nn::fs::FileInputStream* stream, const wchar_t** words,
                                                                   unsigned int count)
{
    wchar_t* converted[WORD_COUNT_MAX];
    for (s32 i = 0; i < static_cast<s32>(WORD_COUNT_MAX); i++) {
        converted[i] = m_ConvertMemory + i * WORD_LENGTH_MAX;
    }
    for (u32 i = 0; i < count; i++) {
        if (words[i] == NULL) {
            return nn::Result(detail::RESULT_INVALID_POINTER);
        }
        ConvertUserInputForWord(converted[i], WORD_LENGTH_MAX, words[i]);
    }
    if (!m_IsAtSignCheckSkipped) {
        // mail addresses are not allowed
        for (u32 i = 0; i < count; i++) {
            if (IsIncludesAtSign(converted[i], WORD_LENGTH_MAX)) {
                results[i] |= 1 << m_PatternList;
            }
        }
    }

    // the word list: UTF-16 with BOM, a regular expression per line
    s32 size = stream->GetSize();
    if (size < sizeof(wchar_t) || size > static_cast<s32>(FILE_BUFFER_SIZE)) {
        return nn::Result(detail::RESULT_WRONG_SIZE);
    }
    if (stream->Read(m_FileBuffer, size) != size) {
        return nn::Result(detail::RESULT_WRONG_SIZE);
    }
    const wchar_t* list = m_FileBuffer;
    if (list[0] != 0xFEFF) {
        return nn::Result(detail::RESULT_BROKEN_DATA);
    }
    u32 start = 1;
    u32 i;
    for (i = 1; i * sizeof(wchar_t) < static_cast<u32>(size); i++) {
        if (list[i] == L'\n') {
            i++;
            CheckWords(results, &list[start], i - start - 1, const_cast<const wchar_t**>(converted), count);
            start = i;
        } else if (list[i] == 0) {
            return nn::Result(detail::RESULT_BROKEN_DATA);
        }
    }
    if (i - 1 != start) {
        CheckWords(results, &list[start], i - start, const_cast<const wchar_t**>(converted), count);
    }
    return nn::Result();
}

// 0x003E0AC8 slot 0x1C
nn::Result nn::ngc::CTR::ProfanityFilter::MaskProfanityWordsInText(int* count, nn::ngc::CTR::ProfanityFilterPatternList list, wchar_t* text)
{
    nn::Result result = CheckArgumentsText(this, text);
    if (result.IsFailure()) {
        return result;
    }
    if (count != NULL) {
        *count = 0;
    }
    wchar_t* converted = m_ConvertMemory;
    u8* textMap = reinterpret_cast<u8*>(converted) + TEXT_MAP_OFFSET;
    ConvertUserInputForText(converted, textMap, TEXT_LENGTH_MAX, text);
    wchar_t path[PATH_LENGTH_MAX];
    m_PatternList = list;
    swprintf(path, PATH_LENGTH_MAX, L"ngword:/%d.txt", list);
    nn::fs::FileInputStream stream;
    result = stream.TryInitialize(path);
    if (result.IsSuccess()) {
        result = MaskProfanityWordsInText_Impl(count, text, converted, textMap, &stream);
    }
    return result;
}

// 0x003E0C08 slot 0x18
nn::Result nn::ngc::CTR::ProfanityFilter::MaskProfanityWordsInText(int* count, wchar_t* text)
{
    nn::Result result = CheckArgumentsText(this, text);
    if (result.IsFailure()) {
        return result;
    }
    if (count != NULL) {
        *count = 0;
    }
    wchar_t* converted = m_ConvertMemory;
    u8* textMap = reinterpret_cast<u8*>(converted) + TEXT_MAP_OFFSET;
    ConvertUserInputForText(converted, textMap, TEXT_LENGTH_MAX, text);
    for (int list = 0;; list++) {
        wchar_t path[PATH_LENGTH_MAX];
        swprintf(path, PATH_LENGTH_MAX, L"ngword:/%d.txt", list);
        nn::fs::FileInputStream stream;
        if (stream.TryInitialize(path).IsFailure()) {
            return nn::Result();
        }
        m_PatternList = list;
        result = MaskProfanityWordsInText_Impl(count, text, converted, textMap, &stream);
        if (result.IsFailure()) {
            return result;
        }
    }
}

// 0x003E0DD4 slot 0x20
nn::Result nn::ngc::CTR::ProfanityFilter::MaskProfanityWordsInText(int* count, bool flag, wchar_t* text)
{
    nn::Result result = CheckArgumentsText(this, text);
    if (result.IsFailure()) {
        return result;
    }
    if (count != NULL) {
        *count = 0;
    }
    wchar_t* converted = m_ConvertMemory;
    u8* textMap = reinterpret_cast<u8*>(converted) + TEXT_MAP_OFFSET;
    ConvertUserInputForText(converted, textMap, TEXT_LENGTH_MAX, text);
    ProfanityFilterPatternList lists[PATTERN_LIST_COUNT_MAX];
    int listCount;
    GetPatternListsFromRegion(lists, &listCount, flag);
    for (int i = 0; i < listCount; i++) {
        wchar_t path[PATH_LENGTH_MAX];
        swprintf(path, PATH_LENGTH_MAX, L"ngword:/%d.txt", lists[i]);
        nn::fs::FileInputStream stream;
        result = stream.TryInitialize(path);
        if (result.IsFailure()) {
            return result;
        }
        m_PatternList = lists[i];
        result = MaskProfanityWordsInText_Impl(count, text, converted, textMap, &stream);
        if (result.IsFailure()) {
            return result;
        }
    }
    return nn::Result();
}

// 0x003E0FDC (name is ours)
nn::Result nn::ngc::CTR::ProfanityFilter::MaskProfanityWordsInText_Impl(int* count, wchar_t* text, wchar_t* converted, u8* textMap,
                                                                        nn::fs::FileInputStream* stream)
{
    wchar_t* regularExpression = reinterpret_cast<wchar_t*>(reinterpret_cast<u8*>(m_ConvertMemory) + REGULAR_EXPRESSION_OFFSET);
    if (!m_IsAtSignCheckSkipped) {
        MaskWord(count, text, converted, textMap, s_MailAddressPattern, false);
    }
    s32 size = stream->GetSize();
    if (size < sizeof(wchar_t) || size > static_cast<s32>(FILE_BUFFER_SIZE)) {
        return nn::Result(detail::RESULT_WRONG_SIZE);
    }
    if (stream->Read(m_FileBuffer, size) != size) {
        return nn::Result(detail::RESULT_WRONG_SIZE);
    }
    const wchar_t* list = m_FileBuffer;
    if (list[0] != 0xFEFF) {
        return nn::Result(detail::RESULT_BROKEN_DATA);
    }
    u32 start = 1;
    u32 i;
    for (i = 1; i * sizeof(wchar_t) < static_cast<u32>(size); i++) {
        if (list[i] == L'\n') {
            i++;
            if (ConvertPatternToRegularExpression(regularExpression, &list[start], i - start - 1)) {
                MaskWord(count, text, converted, textMap, regularExpression, true);
            }
            start = i;
        } else if (list[i] == 0) {
            return nn::Result(detail::RESULT_BROKEN_DATA);
        }
    }
    if (i - 1 != start) {
        if (ConvertPatternToRegularExpression(regularExpression, &list[start], i - start)) {
            MaskWord(count, text, converted, textMap, regularExpression, true);
        }
    }
    return nn::Result();
}

// 0x003E1178 (name is ours)
void nn::ngc::CTR::ProfanityFilter::MaskWord(int* count, wchar_t* text, wchar_t* converted, u8* textMap, const wchar_t* pattern, bool isSkipSpace)
{
    ProfanityFilterTemporaryPool pool;
    pool.Initialize(m_PoolMemory);
    RegexScanner* scanner = NewInPool<RegexScanner>(pool);
    RegexNfaParser* parser = NewInPool<RegexNfaParser>(pool);
    RegexDfaConverter* converter = NewInPool<RegexDfaConverter>(pool);
    RegexMatch* match = NewInPool<RegexMatch>(pool);
    if (scanner->LexicalAnalysis(&pool, pattern) != RegexScanner::RESULT_SUCCESS) {
        return;
    }
    if (parser->Parse(&pool, &scanner->m_Tokens) != RegexNfaParser::RESULT_SUCCESS) {
        return;
    }
    if (!converter->Convert(&pool, &parser->m_States, parser->m_End)) {
        return;
    }
    int position = 0;
    int matchStart;
    int matchLength;
    while (match->Search(&converter->m_DfaStates, converted, position, isSkipSpace, &matchStart, &matchLength)) {
        if (matchLength == 0) {
            position++;
            continue;
        }
        // the match in text (a character of converted can stand for two of text)
        int textStart = 0;
        for (int i = 0; i < matchStart; i++) {
            textStart += textMap[i];
        }
        int textLength = 0;
        for (int i = matchStart; i < matchStart + matchLength; i++) {
            textLength += textMap[i];
        }
        if (m_IsMaskEveryCharacter) {
            for (int i = textStart; i < textStart + textLength; i++) {
                text[i] = L'*';
            }
            for (int i = matchStart; i < matchStart + matchLength; i++) {
                converted[i] = L'*';
            }
            position = matchStart + matchLength;
        } else {
            // one * for the whole word
            text[textStart] = L'*';
            for (int i = textStart + 1;; i++) {
                text[i] = text[i + textLength - 1];
                if (text[i + textLength - 1] == 0) {
                    break;
                }
            }
            converted[matchStart] = L'*';
            textMap[matchStart] = 1;
            for (int i = matchStart + 1;; i++) {
                converted[i] = converted[i + matchLength - 1];
                textMap[i] = textMap[i + matchLength - 1];
                if (converted[i + matchLength - 1] == 0) {
                    break;
                }
            }
            position = matchStart + 1;
        }
        if (count != NULL) {
            (*count)++;
        }
    }
}

// 0x003E1504 | fefates:bytes [tier B]
nn::ngc::CTR::ProfanityFilter::ProfanityFilter()
    : m_IsMemoryBlockUsed(false), m_IsMounted(false), m_IsAtSignCheckSkipped(false), m_WorkMemory(0), m_PoolMemory(0), m_FileBuffer(NULL),
      m_MountMemory(NULL), m_ConvertMemory(NULL), m_IsMaskEveryCharacter(true)
{
}

// 0x003E15CC slot 0x00
// 0x003E1554 (deleting dtor)
nn::ngc::CTR::ProfanityFilter::~ProfanityFilter()
{
    if (m_IsMounted) {
        nn::fs::Unmount(MOUNT_NAME);
    }
    if (m_IsMemoryBlockUsed) {
        m_MemoryBlock.Finalize();
    }
    m_IsMemoryBlockUsed = false;
    m_IsMounted = false;
    m_IsAtSignCheckSkipped = false;
    m_WorkMemory = 0;
    m_PoolMemory = 0;
    m_FileBuffer = NULL;
    m_MountMemory = NULL;
    m_ConvertMemory = NULL;
    m_IsMaskEveryCharacter = true;
}

} // namespace CTR
} // namespace ngc
} // namespace nn
