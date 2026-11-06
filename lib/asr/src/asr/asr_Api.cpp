#include "asr/asr_Api.h"
#include "asr/asr_Frontend.h"
#include "asr/asr_Network.h"
#include "asr/asr_Search.h"
#include <cstdlib>
#include <cstring>

// The interface for the application.
namespace asr {
extern const u8 s_VersionMasks[16][12];
extern const char s_Version[14];
extern const u8 s_VersionKey[24];

namespace {
// 0x00976CA0
Work* s_pWork;

const s32 FILE_NUM = 6;
const s32 FILE_HEADER_SIZE = 4;

// the first word of a file is the XOR of the rest (in big endian at the end)
DECOMP_ALWAYS_INLINE u32 CalcChecksum(const u8* data, s32 size)
{
    u32 checksum = 0;
    s32 words = size >> 2;
    for (s32 i = 0; i < words; i++) {
        checksum ^= reinterpret_cast<const u32*>(data)[i];
    }
    data += words * 4;
    s32 rest = size & 3;
    if (rest != 0) {
        u32 last = 0;
        s32 shift = 24;
        for (s32 i = 0; i < rest; i++) {
            last += data[i] << shift;
            shift -= 8;
        }
        checksum ^= last;
    }
    return checksum;
}

// the words of the list: "id:word", the kind of characters depends on the language
// 0x001527A4
DECOMP_NOINLINE s32 CheckWordList(const char* data, s16* num)
{
    if (num != NULL) {
        *num = 0;
    }
    if (data == NULL) {
        return -1;
    }
    s32 count = 0;
    while (true) {
        if (data[0] == 'E' && data[1] == 'N' && data[2] == 'D' && data[3] == '\0') {
            break;
        }
        if (count == MAX_WORDS) {
            return -3;
        }
        s32 length = 0;
        while (true) {
            if (*data == '\0') {
                return -5;
            }
            if (*data == ':') {
                break;
            }
            length++;
            data++;
            if (length > 134) {
                return -5;
            }
        }
        data++;
        const char* word = data;
        s32 error = -5;
        u8 c = *data;
        while (c == ' ') {
            data++;
            c = *data;
        }
        s32 wordLength = 0;
        bool isOnlySpace = true;
        if (c == '#') {
            size_t size = strlen(data + 1);
            if (size == 0) {
                return static_cast<s16>(error);
            }
            if (size > 128) {
                error = -4;
                return static_cast<s16>(error);
            }
        } else if (c == '\0') {
            return static_cast<s16>(error);
        } else {
            u8 language = s_pWork->language;
            do {
                c = *data;
                bool isValid = false;
                switch (language) {
                case 1:
                    if (c & 0x80) {
                        wordLength += 2;
                        isOnlySpace = false;
                        data += 2;
                        isValid = true;
                    }
                    break;
                case 9:
                    if (c < 0xA1) {
                        break;
                    }
                    if (c == 0xA3) {
                        u8 d = data[1];
                        if ((d >= 0xC1 && d <= 0xDA) || (d >= 0xE1 && d <= 0xFA)) {
                            wordLength += 2;
                            isOnlySpace = false;
                            data += 2;
                            isValid = true;
                            break;
                        }
                    }
                    if (static_cast<u32>(c - 0xB0) > 71 || data[1] < 0xA1 || data[1] > 0xFE) {
                        return static_cast<s16>(error);
                    }
                    wordLength += 2;
                    isOnlySpace = false;
                    data += 2;
                    isValid = true;
                    break;
                case 10:
                    if (static_cast<u32>(c - 0x81) <= 71 && data[1] >= 0x41 && data[1] <= 0xFE) {
                        isOnlySpace = false;
                        data++;
                        wordLength++;
                    } else if (c != ' ') {
                        return static_cast<s16>(error);
                    }
                    wordLength++;
                    data++;
                    isValid = true;
                    break;
                }
                if (!isValid) {
                    if (static_cast<u32>(c - 'a') <= 25 || static_cast<u32>(c - 'A') <= 25 || c >= 0x80) {
                        isOnlySpace = false;
                    } else if (c != '-' && c != ' ' && c != '\'' && c != '_') {
                        return static_cast<s16>(error);
                    }
                    wordLength++;
                    data++;
                }
                if (wordLength > 128) {
                    error = -4;
                    return static_cast<s16>(error);
                }
            } while (*data != '\0');
            if (isOnlySpace) {
                return static_cast<s16>(error);
            }
        }
        while (*word != '\0') {
            word++;
        }
        count++;
        if (count > MAX_WORDS) {
            break;
        }
        data = word + 1;
    }
    if (num != NULL) {
        *num = count;
    }
    return 0;
}

// the results at the end of an utterance
// 0x00144C94
DECOMP_NOINLINE void FinishUtterance()
{
    Work* work = s_pWork;
    GetResults(&work->rec, work->resultIds, work->scores, &work->resultNum, work->outScores);
    work = s_pWork;
    for (s32 i = 0; i < work->resultNum; i++) {
        s32 start = work->rec.startScore;
        s32 divisor = start != 0 ? (start >= 0 ? start : -start) : 1;
        work->scores[i] = (work->scores[i] - start) * 1000 / divisor + 600;
    }
    if (work->resultNum > work->maxResults) {
        work->resultNum = work->maxResults;
    }
    ResetRecognition(&work->rec);
    work = s_pWork;
    work->frameFill = 0;
    work->frameLeft = FRAME_SAMPLES;
    work->isStarted = 0;
    work->silenceFrames = 0;
    work->speechFrames = 0;
}

// 0x001523FC
DECOMP_NOINLINE void StartUtterance(Work* work)
{
    work->frameFill = 0;
    work->isFirstFrame = 1;
    work->frameLeft = FIRST_FRAME_SAMPLES;
    work->isStarted = 0;
    work->silenceFrames = 0;
    work->speechFrames = 0;
    work->resultNum = 0;
}
} // namespace

// 0x00151F00
u32 GetWorkSize()
{
    return WORK_SIZE;
}

// 0x00151E0C
s16 GetResult(s16 index)
{
    Work* work = s_pWork;
    if (work == NULL || index < 1 || work->resultNum < index) {
        return -2;
    }
    return work->resultIds[static_cast<s16>(index - 1)];
}

// 0x00151E64
s32 GetConfig(Config* config)
{
    Work* work = s_pWork;
    if (work == NULL) {
        return -2;
    }
    if (config == NULL) {
        return -1;
    }
    config->maxResults = work->maxResults;
    config->silenceLimit = work->silenceLimit;
    config->speechLimit = work->speechLimit;
    config->endSilence = work->endSilence;
    config->maxLevel = work->rec.params->maxLevel;
    config->isAgc = work->rec.isAgc;
    return 0;
}

// 0x00152608
s32 SetConfig(Config* config)
{
    Work* work = s_pWork;
    if (work == NULL) {
        return -2;
    }
    if (config == NULL) {
        return -1;
    }
    Config saved = *config;
    s32 value = config->maxResults;
    if (static_cast<u32>(value - 1) < 10) {
        work->params.searchResults = value > 1 ? value : 2;
        work->maxResults = value;
        value = config->silenceLimit;
        if (static_cast<u32>(value) < 10001) {
            work->silenceLimit = value;
            work->silenceLimitFrames = value * 11025 / 12800;
            value = config->speechLimit;
            if (static_cast<u32>(value) < 10001) {
                work->speechLimit = value;
                work->speechLimitFrames = value * 11025 / 12800;
                work->_2F750 = value * 11025 / 254600;
                value = config->endSilence;
                if (static_cast<u32>(value - 100) <= 900) {
                    work->endSilence = value;
                    work->endSilenceFrames = value * 1000 / 11025;
                    value = config->maxLevel;
                    if (static_cast<u32>(value - 60) < 69) {
                        work->rec.params->maxLevel = value;
                        value = config->isAgc;
                        if (value == 1 || value == 0) {
                            work->rec.isAgc = value;
                            return 0;
                        }
                    }
                }
            }
        }
    }
    *config = saved;
    return -1;
}

// 0x00151F10
s32 Initialize(u8 language, File* files, s32 fileNum, void* workArea)
{
    // the library only starts with its own version
    u8 version[24] = {0};
    s32 length = strlen(s_Version);
    if (length <= 0) {
        return -2;
    }
    memcpy(version, s_Version, length);
    u32 sum = 0;
    for (s32 i = 0; i < length; i++) {
        sum += version[i];
    }
    memset(version, 0, sizeof(version));
    const u8* mask = s_VersionMasks[sum & 15];
    const u8* key = s_VersionKey;
    for (s32 i = 0; i < 12; i++) {
        version[i] = key[i * 2] ^ mask[i];
        version[i + 12] = mask[i] ^ key[i * 2 + 1];
    }
    if (memcmp(version, s_Version, strlen(s_Version)) != 0) {
        return -2;
    }

    GlobalInitialize();
    if (workArea == NULL || files == NULL || fileNum != FILE_NUM) {
        return -1;
    }
    Work* work = static_cast<Work*>(workArea);
    s_pWork = work;
    for (s32 i = FILE_NUM - 1; i >= 0; i--) {
        const File& file = files[i];
        if (file.data == NULL) {
            if (file.size != 0) {
                return -1;
            }
        }
        if (file.size != 0) {
            const u8* data = file.data;
            u32 checksum = CalcChecksum(data + FILE_HEADER_SIZE, file.size - FILE_HEADER_SIZE);
            if (*reinterpret_cast<const u32*>(data) != checksum) {
                Finalize(NULL);
                return -2;
            }
        }
    }
    memset(&work->params, 0, sizeof(work->params));
    work->params.sampleRate = 11025;
    RecognizerInitialize(&work->rec, &work->params, 0, 0, 0);
    for (s32 i = 0; i < FILE_NUM; i++) {
        File& file = files[i];
        if (file.size != 0) {
            file.data += FILE_HEADER_SIZE;
            file.size -= FILE_HEADER_SIZE;
        }
    }
    work = s_pWork;
    work->language = language;
    s32 result = RecognizerLoadFiles(&work->rec, language, files, fileNum);
    for (s32 i = 0; i < FILE_NUM; i++) {
        File& file = files[i];
        if (file.size != 0) {
            file.data -= FILE_HEADER_SIZE;
            file.size += FILE_HEADER_SIZE;
        }
    }
    if (result != 0) {
        Finalize(NULL);
        return -2;
    }
    work = s_pWork;
    work->frameLeft = 0;
    work->resultNum = 0;
    work->wordNum = 0;
    work->silenceLimit = 1000;
    work->silenceLimitFrames = 861;
    work->speechLimit = 1000;
    work->speechLimitFrames = 861;
    work->_2F750 = 43;
    work->params.searchResults = 10;
    work->maxResults = 10;
    work->endSilenceFrames = 30;
    work->endSilence = 350;
    return 0;
}

// (a nop in the original that falls into Finalize)
// 0x00142BC0
s32 FinalizeEntry(void* out)
{
    return Finalize(out);
}

// 0x00142BC4
s32 Finalize(void* out)
{
    Work* work = s_pWork;
    if (work == NULL) {
        return -2;
    }
    FinishRecognition(&work->rec);
    work = s_pWork;
    work->wordNum = 0;
    work->frameLeft = 0;
    work->resultNum = 0;
    s32 size = 0;
    if (out != NULL) {
        s32 written = static_cast<s16>(SaveChannelStats(&work->rec, static_cast<u8*>(out) + 4));
        size = written;
        if (written != 0) {
            *static_cast<u32*>(out) = CalcChecksum(static_cast<u8*>(out) + 4, written);
        }
        size = static_cast<s16>(written + 4);
    }
    s_pWork = NULL;
    return size;
}

// 0x001523C4
s32 Start()
{
    Work* work = s_pWork;
    if (work == NULL) {
        return -2;
    }
    work->_2F738 = 0;
    if (!work->rec.isReady) {
        return -2;
    }
    StartUtterance(work);
    return 0;
}

// 0x00152474
s32 SetWords(const char* data, s16 size)
{
    Work* work = s_pWork;
    if (work == NULL) {
        return -2;
    }
    if (data == NULL) {
        return work->wordNum != 0 ? 0 : -1;
    }
    if (size <= 0) {
        return -1;
    }
    work->wordNum = 0;
    NetworkReset(&work->rec);
    s_pWork->frameLeft = 0;
    s16 num = 0;
    s32 result = CheckWordList(data, &num);
    if (result != 0) {
        return result;
    }
    if (num != size) {
        return -1;
    }
    if (size > MAX_WORDS) {
        return -3;
    }
    result = -5;
    num = 0;
    work = s_pWork;
    do {
        char id[12];
        char* out = id;
        while (*data != ':') {
            if (*data == '\0') {
                s_pWork->wordNum = 0;
                return result;
            }
            *out++ = *data++;
        }
        *out = '\0';
        s16 value = atoi(id);
        work = s_pWork;
        work->ids[num] = value;
        if (value == 0 || value > 9999) {
            s_pWork->wordNum = 0;
            return result;
        }
        data++;
        work->words[num] = data;
        while (*data != '\0') {
            data++;
        }
        data++;
        num++;
    } while (size > num);
    s_pWork->wordNum = size;
    if (NetworkBuild(&work->rec, 0, work->words, work->ids, size) == 0) {
        return 0;
    }
    s_pWork->wordNum = 0;
    return -6;
}

// 0x00142CFC
s32 Process(const s16* samples, u32 count)
{
    Work* work = s_pWork;
    if (work == NULL || samples == NULL) {
        return -2;
    }
    if (count == 0) {
        FinishUtterance();
        s32 num = s_pWork->resultNum;
        return static_cast<s16>(num == 0 ? -7 : num);
    }
    if (work->frameLeft == 0) {
        return -2;
    }
    u32 left = count;
    while (left != 0) {
        work = s_pWork;
        s32 num = work->frameLeft;
        if (num > static_cast<s32>(left)) {
            num = left;
        }
        memcpy(&work->rec.samples[work->frameFill], &samples[count - left], num * sizeof(s16));
        work = s_pWork;
        left -= num;
        work->frameLeft -= num;
        work->frameFill += num;
        if (work->frameLeft != 0) {
            continue;
        }
        if (work->isFirstFrame != 0) {
            if (ProcessFirstFrame(&work->rec) != 0) {
                return -2;
            }
            s_pWork->isFirstFrame = 0;
        } else {
            s32 result = ProcessFrame(&work->rec, &work->result);
            if (result != 0) {
                return result == -4 ? -9 : -2;
            }
            work = s_pWork;
            switch (work->result->state) {
            case VAD_SILENCE:
                work->silenceFrames++;
                if (work->silenceLimitFrames != 0 && work->silenceFrames > work->silenceLimitFrames) {
                    return -7;
                }
                if (work->isStarted != 0) {
                    work->speechFrames++;
                    if (work->endSilenceFrames <= work->silenceFrames) {
                        FinishUtterance();
                        return static_cast<s16>(s_pWork->resultNum);
                    }
                }
                break;
            case VAD_SPEECH:
                work->speechFrames++;
                if (work->speechLimitFrames != 0 && work->speechFrames > work->speechLimitFrames) {
                    return -8;
                }
                if (work->isStarted == 0) {
                    work->isStarted = 1;
                }
                work->silenceFrames = 0;
                break;
            }
        }
        work = s_pWork;
        work->frameFill = 0;
        work->frameLeft = FRAME_SAMPLES;
    }
    return s_pWork->isStarted != 0 ? FRAME_SAMPLES : 0;
}
} // namespace asr
