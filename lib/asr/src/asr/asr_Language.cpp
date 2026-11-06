#include "asr/asr_Language.h"
#include <cstring>

// The lexicon and the letter to phone rules of the Western languages and Korean.
namespace asr {
namespace {
const s32 RULE_SCORE_BASE = 887;
const s32 LEXICON_MIN_SCORE = 592;

// 0x00159638
DECOMP_NOINLINE s32 BitWriterInitialize(const u8* table, u32 bits, s32 value, BitWriter* writer)
{
    writer->table = table;
    writer->tableSize = bits >> 3;
    memset(writer->bytes, 0, sizeof(writer->bytes));
    writer->bitNum = 0;
    writer->byteIndex = 0;
    writer->_90 = 0;
    writer->_94 = value;
    writer->mask = 1;
    return 0;
}

// 0x001594D8
DECOMP_NOINLINE void BitWriterClear(BitWriter* writer)
{
    memset(writer->bytes, 0, sizeof(writer->bytes));
    writer->bitNum = 0;
    writer->byteIndex = 0;
    writer->_90 = 0;
    writer->mask = 1;
}

// 0x00159508
DECOMP_NOINLINE s32 BitWriterAppend(BitWriter* writer, u32 character)
{
    s32 low = 0;
    s32 high = writer->tableSize - 1;
    const u8* table = writer->table;
    while (low <= high) {
        s32 middle = (low + high) / 2;
        const u8* entry = table + middle * 8;
        u32 code = *reinterpret_cast<const u16*>(entry);
        if (code > character) {
            high = middle - 1;
        } else if (code < character) {
            low = middle + 1;
        } else {
            s32 byte = 0;
            u32 bit = 1;
            for (s32 i = 0; i < entry[2]; i++) {
                u8 value = entry[3 + byte];
                u8* out = &writer->bytes[writer->byteIndex];
                if (value & bit) {
                    *out |= writer->mask;
                } else {
                    *out &= ~writer->mask;
                }
                writer->mask <<= 1;
                if (writer->mask == 0) {
                    writer->byteIndex++;
                    writer->mask = 1;
                }
                bit = (bit << 1) & 0xFF;
                if (bit == 0) {
                    bit = 1;
                    byte++;
                }
            }
            writer->bitNum += entry[2];
            return 0;
        }
    }
    return static_cast<s32>(0x80070057);
}

// 0x0015960C
DECOMP_NOINLINE DECOMP_NOIPA void BitWriterFinish(BitWriter* writer, const u8** bytes, s32* bitNum)
{
    *bytes = writer->bytes;
    *bitNum = writer->bitNum;
    writer->bitNum = 0;
    writer->byteIndex = 0;
    writer->_90 = 0;
    writer->mask = 1;
}

// 0x00159480
DECOMP_NOINLINE s32 BitReaderInitialize(const u16* tree, u32 treeBytes, const u8* bits, u32 bitNum, BitReader* reader)
{
    reader->mask = 1;
    reader->bitPos = 0;
    reader->tree = tree;
    reader->bytePos = 0;
    reader->treeSize = treeBytes >> 1;
    reader->bits = bits;
    reader->bitNum = bitNum;
    return 0;
}

// 0x001594B8
DECOMP_NOINLINE void BitReaderSeek(BitReader* reader, u32 bit)
{
    reader->bitPos = bit;
    reader->bytePos = bit >> 3;
    reader->mask = 1 << (bit & 7);
}

// reads one symbol; returns the number of bits it had
// 0x001593B4
DECOMP_NOINLINE s32 BitReaderRead(BitReader* reader, u16* out)
{
    s32 bits = 0;
    const u16* node = reinterpret_cast<const u16*>(reader->tree);
    while (static_cast<u32>(reader->bitPos) <= reader->bitNum) {
        u32 value = *node;
        if (value & 0x3F00) {
            if ((value & 0xFF00) == 0xFF00) {
                *out = (value ^ 0xFF00) ^ 0xFF;
            } else {
                *out = value;
            }
            return bits;
        }
        if (reader->bits[reader->bytePos] & reader->mask) {
            node += (value & 0xFF) + ((value >> 6) & 0x300);
        }
        reader->bitPos++;
        node++;
        bits++;
        reader->mask <<= 1;
        if (reader->mask == 0) {
            reader->mask = 1;
            reader->bytePos++;
        }
    }
    return 0;
}

} // namespace

// 0x00159678
DECOMP_NOINLINE s32 WesternInitialize(BitWriter* writer, const u8* table, u32 tableBits, BitReader* reader,
                                      const u16* tree, u32 treeBytes, s32 value, const u8* bits, u32 bytes)
{
    if (table != NULL) {
        if (BitWriterInitialize(table, tableBits, value, writer) < 0) {
            return 0;
        }
    }
    if (tree != NULL) {
        return 1 + (BitReaderInitialize(tree, treeBytes, bits, bytes << 3, reader) >> 31);
    }
    return 1;
}

namespace {
// 0x0015979C
DECOMP_NOINLINE s32 RuleSetHasLetter(const RuleSet* rules, u32 letter)
{
    if (letter > rules->letterNum) {
        return 0;
    }
    return rules->letterMap[letter] != -1;
}

} // namespace

// 0x0015A500
DECOMP_NOINLINE s32 KoreanLoad(u8, const File* file, KoreanTables* tables)
{
    if (file == NULL || tables == NULL) {
        return -1;
    }
    const u8* data = file->data;
    s32 size = file->size;
    if (size <= 0 || size < 28) {
        return -1;
    }
    const u32* header = reinterpret_cast<const u32*>(data);
    s32 keyNum = header[1];
    tables->keyNum = keyNum;
    s32 listNum = header[2];
    tables->listNum = listNum;
    s32 letterSize = header[3];
    tables->letterRangeNum = letterSize;
    tables->letterPhoneNum = header[4];
    s32 phoneListNum = header[5];
    tables->pairNum = phoneListNum;
    s32 phoneNum = header[6];
    data += 28;
    tables->phoneNum = phoneNum;
    if (phoneNum > 409) {
        return -1;
    }
    s32 used = 28 + keyNum * 4;
    if (static_cast<s32>(file->size) < used) {
        return -1;
    }
    tables->keys = data;
    data += keyNum * 4;
    used += listNum * 2;
    if (static_cast<s32>(file->size) < used) {
        return -1;
    }
    tables->lists = reinterpret_cast<const u16*>(data);
    data += listNum * 2;
    used += phoneListNum * 2;
    if (static_cast<s32>(file->size) < used) {
        return -1;
    }
    tables->phoneList = reinterpret_cast<const u16*>(data);
    data += phoneListNum * 2;
    used += tables->letterPhoneNum * 2;
    if (static_cast<s32>(file->size) < used) {
        return -1;
    }
    tables->letterPhones = reinterpret_cast<const u16*>(data);
    data += tables->letterPhoneNum * 2;
    used += phoneNum * 4;
    if (used > static_cast<s32>(file->size)) {
        return -1;
    }
    tables->pairs = reinterpret_cast<const u16*>(data);
    data += phoneNum * 4;
    for (s32 i = 0; i < tables->phoneNum; i++) {
        if (static_cast<s32>(file->size) < used) {
            return -1;
        }
        u32 length = *data++;
        if (length + 1 > 8) {
            return -1;
        }
        KoreanPhone& phone = tables->phones[i];
        used += 8;
        phone.length = length - 1;
        if (static_cast<s32>(file->size) < used) {
            return -1;
        }
        phone.name = reinterpret_cast<const char*>(data);
        data += 7;
        used += 2;
        phone.id = *reinterpret_cast<const u16*>(data);
        data += 2;
    }
    if (static_cast<s32>(file->size) < tables->letterRangeNum + used) {
        return -1;
    }
    tables->letterRanges = data;
    return 0;
}

// 0x0015A678
DECOMP_NOINLINE s32 ChineseLoad(u8, const File* file, ChineseTables* tables)
{
    if (file == NULL || tables == NULL) {
        return -1;
    }
    const u8* data = file->data;
    s32 size = file->size;
    if (size <= 0 || size < 16) {
        return -1;
    }
    const u32* header = reinterpret_cast<const u32*>(data);
    s32 first = header[1];
    tables->rangeNum = first;
    s32 used = first * 2 + 16;
    s32 second = header[2];
    tables->entryNum = second;
    s32 third = header[3];
    data += 16;
    tables->mapNum = third;
    if (static_cast<s32>(file->size) < used) {
        return -1;
    }
    tables->ranges = reinterpret_cast<const u16*>(data);
    used += second;
    data += first * 2;
    if (static_cast<s32>(file->size) < used) {
        return -1;
    }
    tables->entries = data;
    data += second;
    if (third + used > static_cast<s32>(file->size)) {
        return -1;
    }
    tables->phoneMap = data;
    return 0;
}

namespace {
DECOMP_ALWAYS_INLINE u32 ToLowerCase(u8 language, u32 c)
{
    if (language == 11) {
        if (c - 65 < 26 || c - 176 < 32) {
            c += 32;
        }
        return c == 161 ? 241 : c;
    }
    if (c - 65 < 26 || c - 192 < 31) {
        c += 32;
    }
    return c;
}

// the place of a new candidate in the list, which is sorted with the best first
DECOMP_ALWAYS_INLINE s32 FindCandidatePlace(const RuleCandidate* candidates, s32 num, s32 score)
{
    s32 place = 0;
    s32 last = num - 1;
    if (num > 0 && score <= candidates[0].score) {
        if (score <= candidates[last].score) {
            place = num;
        } else {
            while (place < last) {
                s32 middle = (place + last) / 2;
                if (candidates[middle].score < score) {
                    last = middle;
                } else {
                    place = middle + 1;
                }
            }
        }
    }
    return place;
}

DECOMP_ALWAYS_INLINE void AddCandidate(RuleCandidate* candidates, s32& num, s32 capacity, s32 score, const u32* rule,
                                       u32 choice, u32 parent)
{
    s32 place = FindCandidatePlace(candidates, num, score);
    if (capacity <= place) {
        return;
    }
    s32 end = num < capacity ? num : capacity - 1;
    if (place < end) {
        memmove(&candidates[place + 1], &candidates[place], (end - place) * sizeof(RuleCandidate));
    }
    RuleCandidate& candidate = candidates[place];
    candidate.score = score;
    candidate.parent = parent;
    candidate.choice = choice;
    candidate.rule = rule;
    if (num < capacity) {
        num++;
    }
}

DECOMP_ALWAYS_INLINE u32 GetContextClass(const RuleSet* rules, const u16* text, s32 length, s32 position)
{
    if (position < 0 || U16Length(text) <= position) {
        return rules->boundary;
    }
    u32 letter = text[position];
    if (rules->letterNum < letter) {
        return 0xFF;
    }
    return rules->letterMap[letter] & 0xFF;
}
} // namespace

// 0x007C623C
s32 U16Length(const u16* text)
{
    s32 length = 0;
    if (*text != 0) {
        do {
            length++;
        } while (*++text != 0);
    }
    return length;
}

// 0x007B2C94
s32 U16ListCount(const u16* list)
{
    s32 count = 0;
    while (*list != 0) {
        list += U16Length(list) + 1;
        count++;
    }
    return count;
}

// 0x007B2CD0
s32 U16ListSize(const u16* list)
{
    s32 size = 0;
    while (true) {
        s32 length = U16Length(list);
        if (length == 0) {
            break;
        }
        size += length * 2 + 2;
        list += length + 1;
    }
    return size + 2;
}

// 0x00163F44
s32 RuleConvert(RuleSet* rules, const u16* text, s32 beam, s32 threshold, u16* out, s32* scores, s32* outNum, u32* outSize)
{
    RuleMemory* memory = reinterpret_cast<RuleMemory*>(rules + 1);
    RuleCandidate* candidates = memory->candidates;
    s32 isGood = 1;
    s32 length = U16Length(text);
    s32 stride = (length * rules->maxPhones * 2 + 13) & ~3;
    memset(candidates, 0, sizeof(memory->candidates));
    memset(memory->buffers[0], 0, RULE_BUFFER_SIZE);
    memset(memory->buffers[1], 0, RULE_BUFFER_SIZE);
    u8* current = memory->buffers[0];
    u8* next = memory->buffers[1];
    reinterpret_cast<RuleHypothesis*>(current)->score = RULE_SCORE_BASE;
    reinterpret_cast<RuleHypothesis*>(current)->delta = RULE_SCORE_BASE;
    s32 hypothesisNum = 1;
    for (s32 position = 0; isGood && position < length; position++) {
        s32 candidateNum = 0;
        const u32* rule = NULL;
        for (s32 h = 0; h < hypothesisNum; h++) {
            const RuleHypothesis* hypothesis = reinterpret_cast<const RuleHypothesis*>(current + h * stride);
            if (hypothesis->position > position) {
                AddCandidate(candidates, candidateNum, beam, hypothesis->score + hypothesis->delta - RULE_SCORE_BASE,
                             NULL, 0, h);
                continue;
            }
            if (rule == NULL) {
                u32 letter = text[position];
                if (letter <= rules->letterNum) {
                    s32 index = rules->letterMap[letter];
                    if (index == -1) {
                        return 0;
                    }
                    rule = rules->letters[index].rules;
                    while (*rule & 0x80000000) {
                        u32 word = *rule;
                        u32 kind = (word << 1) >> 29;
                        bool isMatch = true;
                        if (kind <= 1) {
                            s32 offset = (word << 5) >> 29;
                            if (word & 0x8000000) {
                                offset = -offset;
                            }
                            u32 value = (word << 8) >> 24;
                            u32 context = GetContextClass(rules, text, length, position + offset);
                            if (kind == 0) {
                                isMatch = value == context;
                            } else {
                                isMatch = strchr(rules->sets[value], context) != NULL;
                            }
                            if (!isMatch) {
                                rule += static_cast<s16>(word) + 1;
                                continue;
                            }
                        }
                        rule++;
                    }
                }
                if (rule == NULL) {
                    return 0;
                }
            }
            u32 word = *rule;
            u32 type = (word << 4) >> 28;
            if (type == 0) {
                s32 num = (word << 8) >> 24;
                for (s32 i = 0; i < num; i++) {
                    AddCandidate(candidates, candidateNum, beam,
                                 hypothesis->score + static_cast<s16>(rule[i + 1]) - RULE_SCORE_BASE, rule, i, h);
                }
            } else if (type == 1) {
                AddCandidate(candidates, candidateNum, beam, hypothesis->score, rule, 0, h);
            } else {
                isGood = 0;
            }
        }
        if (!isGood) {
            return 0;
        }
        for (s32 i = 0; i < candidateNum; i++) {
            const RuleCandidate& candidate = candidates[i];
            RuleHypothesis* target = reinterpret_cast<RuleHypothesis*>(next + i * stride);
            const RuleHypothesis* parent = reinterpret_cast<const RuleHypothesis*>(current + candidate.parent * stride);
            u32 size = parent->phoneNum * 2 + 8;
            if (i * stride + size > RULE_BUFFER_SIZE) {
                return 0;
            }
            memcpy(target, parent, size);
            target->score = candidate.score;
            const u32* candidateRule = candidate.rule;
            if (candidateRule != NULL) {
                u32 word = *candidateRule;
                target->position += ((word << 1) >> 29) + 1;
                u32 phones;
                if ((word << 4) >> 28 == 1) {
                    target->delta = RULE_SCORE_BASE;
                    phones = word & 0xFFFF;
                } else {
                    u32 choice = candidateRule[candidate.choice + 1];
                    target->delta = choice;
                    phones = choice >> 16;
                }
                const s16* source = rules->phones[phones];
                u16* destination = target->phones + target->phoneNum;
                u16* start = destination;
                while (*source != 0) {
                    u16 phone = *source++;
                    if (phone != rules->skipPhone) {
                        *destination++ = phone;
                    }
                }
                target->phoneNum += destination - start;
            }
        }
        hypothesisNum = candidateNum;
        u8* swap = current;
        current = next;
        next = swap;
    }

    // without duplicates
    s32 uniqueNum = 0;
    for (s32 h = 0; h < hypothesisNum; h++) {
        RuleHypothesis* hypothesis = reinterpret_cast<RuleHypothesis*>(current + h * stride);
        if (hypothesis->phoneNum == 0) {
            continue;
        }
        bool isDuplicate = false;
        for (s32 i = 0; i < uniqueNum; i++) {
            const RuleHypothesis* other = reinterpret_cast<const RuleHypothesis*>(current + i * stride);
            if (hypothesis->phoneNum == other->phoneNum &&
                memcmp(other->phones, hypothesis->phones, other->phoneNum * 2) == 0) {
                isDuplicate = true;
                break;
            }
        }
        if (isDuplicate) {
            continue;
        }
        RuleHypothesis* target = reinterpret_cast<RuleHypothesis*>(current + uniqueNum * stride);
        if (uniqueNum != h) {
            memcpy(target, hypothesis, stride);
        }
        target->phones[target->phoneNum] = 0;
        uniqueNum++;
    }
    if (uniqueNum <= 0) {
        return 0;
    }
    s32 first = reinterpret_cast<const RuleHypothesis*>(current)->score;
    s32 keep = 1;
    for (; keep < uniqueNum; keep++) {
        RuleHypothesis* hypothesis = reinterpret_cast<RuleHypothesis*>(current + keep * stride);
        s32 score = hypothesis->score - first + RULE_SCORE_BASE;
        hypothesis->score = score;
        if (score < threshold) {
            break;
        }
    }
    reinterpret_cast<RuleHypothesis*>(current)->score = RULE_SCORE_BASE;
    u32 total = 1;
    for (s32 i = 0; i < keep; i++) {
        total += reinterpret_cast<const RuleHypothesis*>(current + i * stride)->phoneNum + 1;
    }
    if (*outSize < total * 2) {
        return 0;
    }
    s32 written = 0;
    for (s32 i = 0; i < keep; i++) {
        const RuleHypothesis* hypothesis = reinterpret_cast<const RuleHypothesis*>(current + i * stride);
        memcpy(out + written, hypothesis->phones, hypothesis->phoneNum * 2 + 2);
        written += hypothesis->phoneNum + 1;
        if (scores != NULL) {
            scores[i] = hypothesis->score;
        }
    }
    out[written] = 0;
    if (outNum != NULL) {
        *outNum = keep;
    }
    if (outSize != NULL) {
        *outSize = written * 2 + 2;
    }
    return 1;
}

// 0x0015A724
s32 WesternConvertWide(WesternTables* tables, const u16* text, s32 variants, u16* out, s32* num, s32* size)
{
    const WesternInfo* info = tables->info;
    u8 language = info->language;
    s32 scoreList[10];
    s32 isOk;
    bool isFound = false;
    if (tables->isLexicon != 0) {
        BitWriterClear(&tables->writer);
        s32 result = 0;
        for (s32 i = 0; i < U16Length(text) + 1; i++) {
            result = BitWriterAppend(&tables->writer, ToLowerCase(language, text[i]) & 0xFFFF);
            if (result < 0) {
                break;
            }
        }
        if (result >= 0) {
            const u8* key;
            s32 keyBits;
            BitWriterFinish(&tables->writer, &key, &keyBits);
            u32 keyBytes = (keyBits + 7) >> 3;
            const u32* keys = info->keys;
            const u32* range = keys;
            s32 rangeNum = info->keyNum;
            const u8* strings = info->keyStrings;
            const u32* found = NULL;
            while (rangeNum != 0 && found == NULL) {
                s32 middle = rangeNum / 2;
                const u8* candidate = strings + range[middle];
                u32 i;
                s32 order = 0;
                for (i = 0; i < keyBytes; i++) {
                    if (key[i] < candidate[i]) {
                        order = -1;
                        break;
                    }
                    if (key[i] > candidate[i]) {
                        order = 1;
                        break;
                    }
                }
                if (order < 0) {
                    rangeNum = middle;
                } else if (order > 0) {
                    range += middle + 1;
                    rangeNum -= middle + 1;
                } else {
                    found = &range[middle];
                }
            }
            if (found != NULL) {
                u32 start = (keys[found - keys] + keyBytes) * 8;
                u32 bit = start;
                isOk = 1;
                u32 total = 0;
                u32 maxSize = *size;
                while (true) {
                    s32 count = 0;
                    u16 symbol;
                    BitReaderSeek(&tables->reader, bit);
                    bit += BitReaderRead(&tables->reader, &symbol);
                    if (symbol != 0) {
                        do {
                            count++;
                            bit += BitReaderRead(&tables->reader, &symbol);
                        } while (symbol != 0);
                        if (count != 0) {
                            total += count * 2 + 2;
                            continue;
                        }
                    }
                    break;
                }
                u32 bytes = total + 2;
                if (bytes > maxSize) {
                    isOk = 0;
                } else {
                    BitReaderSeek(&tables->reader, start);
                }
                for (u32 i = 0; i < bytes >> 1; i++) {
                    if (isOk == 0) {
                        return 0;
                    }
                    isOk = 1 + (BitReaderRead(&tables->reader, &out[i]) >> 31);
                }
                if (isOk == 0) {
                    return 0;
                }
                for (s32 i = 0; i < variants; i++) {
                    scoreList[i] = RULE_SCORE_BASE;
                }
                isFound = true;
            }
        }
    }
    if (!isFound) {
        u16 lower[34];
        s32 lowerNum = 0;
        s32 isValid = 1;
        s32 end = U16Length(text) + 1;
        for (s32 i = 0; i < end && isValid; i++) {
            u32 c = ToLowerCase(language, text[i]) & 0xFFFF;
            lower[lowerNum] = c;
            if (RuleSetHasLetter(info->rules, c) != 0) {
                lowerNum++;
            } else if (c != '-' && c != 0) {
                isValid = 0;
            }
        }
        if (!isValid) {
            return 0;
        }
        s32 result = RuleConvert(const_cast<RuleSet*>(info->rules), lower, 4, -206, out, scoreList, num,
                                 reinterpret_cast<u32*>(size));
        if (result == 0) {
            return 0;
        }
        s32 first = scoreList[0];
        s32 listNum = U16ListCount(out);
        for (s32 i = 0; i < listNum; i++) {
            scoreList[i] = scoreList[i] - first + RULE_SCORE_BASE;
        }
        isOk = 1;
    }
    u16* list = out;
    s32 i = 0;
    while (true) {
        s32 length = U16Length(list);
        if ((length == 0 || variants <= i || LEXICON_MIN_SCORE > scoreList[i]) && i != 0) {
            break;
        }
        list += length + 1;
        i++;
    }
    *list = 0;
    *num = i;
    *size = U16ListSize(out);
    return isOk;
}

// 0x0015ABD0
s8 LanguageWesternConvert(void* tables, const char* text, s32 variants, u16* out, s32* num, s32* size)
{
    u16 wide[36];
    s32 i = 0;
    while (text[i] != '\0') {
        wide[i] = static_cast<u8>(text[i]);
        i++;
    }
    wide[i] = 0;
    return WesternConvertWide(static_cast<WesternTables*>(tables), wide, variants, out, num, size);
}

// 0x001592E0
const u8* LanguageWesternGetPhoneMap(void* tables)
{
    if (tables == NULL) {
        return NULL;
    }
    return static_cast<WesternTables*>(tables)->info->phoneMap;
}

// Korean
namespace {
// 0x001649BC
DECOMP_NOINLINE s32 KoreanToWide(const char* text, u16* out)
{
    if (text == NULL) {
        return -1;
    }
    const u8* p = reinterpret_cast<const u8*>(text);
    s32 length = 0;
    u32 first = *p;
    if (first != 0) {
        const u8* q = p;
        do {
            length++;
            q += *q >= 161 ? 1 : 0;
        } while (*++q != 0);
    }
    s32 num = 0;
    if (first != 0) {
        do {
            if (num >= length) {
                out[length] = 0;
                return -1;
            }
            u32 c = *p;
            if (c - 176 <= 71 && p[1] >= 161 && p[1] <= 254) {
                out[num++] = p[1] + (c << 8);
                p++;
            } else if (c - 65 <= 25) {
                out[num++] = c;
            } else if (c == 163 && p[1] >= 193 && p[1] <= 218) {
                out[num++] = p[1] - 128;
                p++;
            } else if (c - 97 <= 25) {
                out[num++] = c;
            } else if (c == 163 && p[1] >= 225 && p[1] <= 250) {
                out[num++] = p[1] - 128;
                p++;
            } else {
                return 0;
            }
        } while (*++p != 0);
    }
    out[num] = 0;
    return num;
}

// the pronunciations of a syllable: pairs of phones without duplicates
// 0x00158AD4
DECOMP_NOINLINE s32 KoreanLookupSyllable(const KoreanTables* tables, u32 syllable, s32 variants, u16* out, s32* size,
                                         s32* num)
{
    if (variants < 0 || out == NULL) {
        return 0;
    }
    const u8* keys = tables->keys;
    s32 count = tables->keyNum;
    const u16* low = reinterpret_cast<const u16*>(keys);
    const u16* high = low + (count - 1) * 2;
    u32 index = MAX_U16;
    while (low <= high && count != 0) {
        s32 half = count / 2;
        if (half == 0) {
            if (*low == syllable) {
                index = low[1];
            }
            break;
        }
        const u16* middle = low + ((count & 1) ? half : half - 1) * 2;
        s32 order = static_cast<s32>(syllable) - *middle;
        if (order == 0) {
            index = middle[1];
            break;
        }
        if (order > 0) {
            count = half;
            low = middle + 2;
        } else {
            count = (count & 1) ? half : half - 1;
            high = middle - 2;
        }
    }
    if (index == MAX_U16) {
        return 0;
    }
    const u16* list = tables->lists + index;
    s32 used = 0;
    while (used / 3 <= variants) {
        u32 pair = (*list & ~0xFC00) - 1;
        u16 first = tables->pairs[pair * 2];
        u16 second = tables->pairs[pair * 2 + 1];
        s32 i;
        for (i = 0; i < used; i += 3) {
            if (out[i] == first && out[i + 1] == second) {
                break;
            }
        }
        if (i >= used) {
            out[used++] = first;
            out[used++] = second;
            out[used++] = 0;
        }
        if (*list & 0x8000) {
            break;
        }
        list++;
    }
    out[used] = 0;
    *size = U16ListSize(out);
    *num = U16ListCount(out);
    return 1;
}

DECOMP_ALWAYS_INLINE s32 GetLatinLetter(u32 c)
{
    if (c - 'a' <= 25) {
        return c - 'a';
    }
    if (c - 'A' <= 25) {
        return c - 'A';
    }
    if (c - 0xA3E1 <= 25) {
        return c - 0xA3E1;
    }
    if (c - 0xA3C1 <= 25) {
        return c - 0xA3C1;
    }
    return -1;
}
} // namespace

// 0x00158E20
u32 LanguageKoreanFindPhone(const void* tables, const char* name)
{
    const KoreanTables* korean = static_cast<const KoreanTables*>(tables);
    u32 low = 0;
    u32 high = (korean->phoneNum - 1) & 0xFFFF;
    do {
        s32 count = high - low + 1;
        u32 half = (((count + (static_cast<u32>(count) >> 31)) << 15) >> 16) & 0xFFFF;
        if (half == 0) {
            if (count != 0 && strcmp(name, korean->phones[low].name) == 0) {
                return korean->phones[low].id;
            }
            return MAX_U16;
        }
        u32 middle = (low + half) & 0xFFFF;
        s32 order = strcmp(name, korean->phones[middle].name);
        if (order == 0) {
            return korean->phones[middle].id;
        }
        if (order < 0) {
            high = (middle - 1) & 0xFFFF;
        } else {
            low = (middle + 1) & 0xFFFF;
        }
    } while (low <= high);
    return MAX_U16;
}

// 0x00157C6C
s8 LanguageKoreanConvert(void* tables, const char* text, s32 variants, u16* out, s32* num, s32* size)
{
    KoreanTables* korean = static_cast<KoreanTables*>(tables);
    u16 wide[66];
    if (KoreanToWide(text, wide) == 0) {
        return 0;
    }
    s32 length = U16Length(wide);
    if (length > 64 || static_cast<u32>(variants) > 4) {
        return 0;
    }
    memset(korean->candidates, 0, sizeof(korean->candidates));
    memset(korean->buffers[0], 0, sizeof(korean->buffers[0]));
    memset(korean->buffers[1], 0, sizeof(korean->buffers[1]));
    KoreanHypothesis* current = korean->buffers[0];
    KoreanHypothesis* next = korean->buffers[1];
    current[0].phoneNum = 0;
    current[0].score = 0;
    current[0].position = 0;
    s32 hypothesisNum = 1;
    s32 isGood = 1;
    for (s32 position = 0; isGood && position < length; position++) {
        u16 words[518];
        s32 listSize = 517;
        s32 listNum = 0;
        s32 advance = 0;
        s32 candidateNum = 0;
        words[0] = 0;
        for (s32 h = 0; h < hypothesisNum; h++) {
            KoreanHypothesis* hypothesis = &current[h];
            if (hypothesis->position > position) {
                s32 place = 0;
                s32 last = candidateNum - 1;
                s32 score = hypothesis->score;
                if (candidateNum > 0 && score <= korean->candidates[0].score) {
                    if (score <= korean->candidates[last].score) {
                        place = candidateNum;
                    } else {
                        while (place < last) {
                            s32 middle = (place + last) / 2;
                            if (korean->candidates[middle].score < score) {
                                last = middle;
                            } else {
                                place = middle + 1;
                            }
                        }
                    }
                }
                if (variants > place) {
                    s32 end = candidateNum < variants ? candidateNum : variants - 1;
                    if (place < end) {
                        memmove(&korean->candidates[place + 1], &korean->candidates[place],
                                (end - place) * sizeof(KoreanCandidate));
                    }
                    korean->candidates[place].score = score;
                    korean->candidates[place].parent = h;
                    korean->candidates[place].phones = NULL;
                    if (candidateNum < variants) {
                        candidateNum++;
                    }
                }
                continue;
            }
            if (words[0] == 0) {
                u32 c = wide[position];
                u32 lowByte = c & 0xFF;
                if ((c >> 8) - 176 <= 71 && lowByte >= 161 && lowByte <= 254) {
                    isGood = KoreanLookupSyllable(korean, c, variants, words, &listSize, &listNum);
                    advance = 1;
                } else if (GetLatinLetter(c) >= 0) {
                    u16 latin[66];
                    s32 latinNum = 0;
                    for (s32 i = position; GetLatinLetter(wide[i]) >= 0; i++) {
                        latin[latinNum++] = wide[i];
                    }
                    advance = latinNum;
                    latin[latinNum] = 0;
                    s32 used = 0;
                    s32 latinLength = U16Length(latin);
                    if (variants >= 0 && latinLength >= 0) {
                        for (s32 i = 0; i < latinLength && used + 1 < listSize / 2; i++) {
                            s32 letter = GetLatinLetter(latin[i]);
                            s32 start = korean->letterRanges[letter];
                            s32 end = letter == korean->letterRangeNum - 1 ? korean->letterPhoneNum
                                                                           : korean->letterRanges[letter + 1];
                            for (; start < end && used + 1 < listSize / 2; start++) {
                                words[used++] = korean->letterPhones[start];
                            }
                        }
                        words[used] = 0;
                        words[used + 1] = 0;
                        listSize = U16ListSize(words);
                        listNum = U16ListCount(words);
                        isGood = 1;
                    } else {
                        return 0;
                    }
                } else {
                    return 0;
                }
                if (!isGood) {
                    return 0;
                }
            } else if (!isGood) {
                return 0;
            }
            const u16* list = words;
            for (s32 i = 0; i < listNum && *list != 0; i++) {
                s32 place = 0;
                s32 last = candidateNum - 1;
                s32 score = hypothesis->score;
                if (candidateNum > 0 && score <= korean->candidates[0].score) {
                    if (score <= korean->candidates[last].score) {
                        place = candidateNum;
                    } else {
                        while (place < last) {
                            s32 middle = (place + last) / 2;
                            if (korean->candidates[middle].score < score) {
                                last = middle;
                            } else {
                                place = middle + 1;
                            }
                        }
                    }
                }
                if (variants > place) {
                    s32 end = candidateNum < variants ? candidateNum : variants - 1;
                    if (place < end) {
                        memmove(&korean->candidates[place + 1], &korean->candidates[place],
                                (end - place) * sizeof(KoreanCandidate));
                    }
                    korean->candidates[place].score = score;
                    korean->candidates[place].parent = h;
                    korean->candidates[place].phones = list;
                    if (candidateNum < variants) {
                        candidateNum++;
                    }
                }
                while (*list != 0) {
                    list++;
                }
                list++;
            }
        }
        if (!isGood) {
            return 0;
        }
        for (s32 i = 0; i < candidateNum; i++) {
            const KoreanCandidate& candidate = korean->candidates[i];
            KoreanHypothesis* target = &next[i];
            const KoreanHypothesis* parent = &current[candidate.parent];
            memcpy(target, parent, parent->phoneNum * 2 + 8);
            target->score = candidate.score;
            if (candidate.phones != NULL) {
                target->position += advance;
                s32 added = 0;
                u16* destination = target->phones + target->phoneNum;
                while (candidate.phones[added] != 0) {
                    destination[added] = candidate.phones[added];
                    added++;
                }
                target->phoneNum += added;
            }
        }
        hypothesisNum = candidateNum;
        KoreanHypothesis* swap = current;
        current = next;
        next = swap;
    }

    s32 uniqueNum = 0;
    for (s32 h = 0; h < hypothesisNum; h++) {
        KoreanHypothesis* hypothesis = &current[h];
        if (hypothesis->phoneNum == 0) {
            continue;
        }
        bool isDuplicate = false;
        for (s32 i = 0; i < uniqueNum; i++) {
            if (hypothesis->phoneNum == current[i].phoneNum &&
                memcmp(current[i].phones, hypothesis->phones, current[i].phoneNum * 2) == 0) {
                isDuplicate = true;
                break;
            }
        }
        if (isDuplicate) {
            continue;
        }
        KoreanHypothesis* target = &current[uniqueNum];
        if (uniqueNum != h) {
            memcpy(target, hypothesis, sizeof(KoreanHypothesis));
        }
        target->phones[target->phoneNum] = 0;
        uniqueNum++;
    }
    if (uniqueNum <= 0) {
        return 0;
    }
    s32 total = 1;
    for (s32 i = 0; i < uniqueNum; i++) {
        total += current[i].phoneNum + 1;
    }
    if (*size < total) {
        return 0;
    }
    s32 written = 0;
    for (s32 i = 0; i < uniqueNum; i++) {
        memcpy(out + written, current[i].phones, current[i].phoneNum * 2 + 2);
        written += current[i].phoneNum + 1;
    }
    out[written] = 0;
    if (num != NULL) {
        *num = uniqueNum;
    }
    if (size != NULL) {
        *size = written * 2 + 2;
    }
    return 1;
}

// 0x00158E14
const u8* LanguageChineseGetPhoneMap(void* tables)
{
    if (tables == NULL) {
        return NULL;
    }
    return static_cast<ChineseTables*>(tables)->phoneMap;
}
} // namespace asr
