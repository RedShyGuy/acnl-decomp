#include "asr/asr_Network.h"
#include "asr/asr_Language.h"
#include "asr/asr_Frontend.h"
#include <cstdint>
#include <cstring>

// Setting up: the files of the recognizer and the search network of a word list.
namespace asr {
// 0x007B8518
TreeNode* ReadTreeNode(Recognizer* rec)
{
    TreeNode* node = rec->treeNodeCursor;
    if (reinterpret_cast<u8*>(node) - reinterpret_cast<u8*>(rec->treeNodes) >= static_cast<s32>(sizeof(rec->treeNodes))) {
        return NULL;
    }
    rec->treeNodeCursor = node + 1;
    if (rec->treeFileSize < ++rec->treeFileUsed) {
        return NULL;
    }
    if (*rec->treeCursor++ == 0) {
        TreeQuestion* question = rec->questionCursor;
        if (question - rec->questions >= 700) {
            return NULL;
        }
        node->question = question;
        rec->treeFileUsed += 2;
        if (static_cast<s32>(rec->treeFileUsed) > static_cast<s32>(rec->treeFileSize)) {
            return NULL;
        }
        rec->questionCursor->kind = *rec->treeCursor++;
        rec->questionCursor->position = *rec->treeCursor++;
        TreeQuestion* current = rec->questionCursor;
        switch (current->kind) {
        case 'M':
        case 'G':
        case 'W':
            break;
        case 'P':
            if (rec->treeFileSize < ++rec->treeFileUsed) {
                return NULL;
            }
            current->value = reinterpret_cast<const void*>(static_cast<uintptr_t>(*rec->treeCursor++));
            break;
        case 'C':
            if (rec->treeFileSize < ++rec->treeFileUsed) {
                return NULL;
            }
            current->value = &rec->phoneSets[*rec->treeCursor++];
            break;
        default:
            return NULL;
        }
        rec->questionCursor++;
        node->yes = ReadTreeNode(rec);
        if (node->yes == NULL) {
            return NULL;
        }
        node->no = ReadTreeNode(rec);
        if (node->no == NULL) {
            return NULL;
        }
    }
    return node;
}

// 0x007B86EC
void NumberLeaves(Recognizer* rec, TreeNode* node)
{
    while (node->yes != NULL) {
        NumberLeaves(rec, node->yes);
        node = node->no;
    }
    node->leaf = rec->leafCount;
    rec->leafCount++;
}

namespace {
enum FileKind
{
    FILE_MODEL = 1,
    FILE_DICTIONARY = 2,
    FILE_TREES = 3,
    FILE_LANGUAGE = 4,
    FILE_ADAPTION = 5,
    FILE_STATS = 6,
};

const char s_SilenceName[] = "sil";

DECOMP_ALWAYS_INLINE const char* SkipSeparators(const char* p, const char* end)
{
    while (p < end && (*p == ' ' || *p == '\t' || *p == '_')) {
        p++;
    }
    return p;
}

DECOMP_ALWAYS_INLINE void LanguageTablesClear(Recognizer* rec, u8 language)
{
    if (language == 9) {
        memset(rec->koreanTables, 0, 0x1594);
    } else if (language == 10) {
        memset(rec->chineseTables, 0, 24);
    } else {
        memset(rec->westernTables, 0, sizeof(WesternTables));
    }
}

DECOMP_ALWAYS_INLINE s32 LanguageTablesLoad(Recognizer* rec, u8 language, const File* file)
{
    if (language == 9) {
        return KoreanLoad(language, file, reinterpret_cast<KoreanTables*>(rec->koreanTables)) == -1 ? -1 : 0;
    }
    if (language == 10) {
        return ChineseLoad(language, file, reinterpret_cast<ChineseTables*>(rec->chineseTables)) == -1 ? -1 : 0;
    }
    if (file->size == 0) {
        return 0;
    }
    if (file->size < 52) {
        return -1;
    }
    const u32* header = reinterpret_cast<const u32*>(file->data);
    if (header[0] != language) {
        return -1;
    }
    WesternTables* tables = reinterpret_cast<WesternTables*>(rec->westernTables);
    WesternData* western = reinterpret_cast<WesternData*>(rec->westernTables + sizeof(WesternTables));
    western->info.language = language;
    western->info.rules = &western->rules;
    western->rules.letters = western->letters;
    western->rules.phones = western->phones;
    western->rules.sets = western->sets;
    western->info.writerTableBits = header[1];
    western->info.readerTreeBits = header[2];
    western->info._1C = header[3];
    s32 keyNum = header[4];
    western->info.keyNum = keyNum;
    s32 stringBytes = header[5];
    western->info.stringBits = stringBytes;
    western->rules.maxPhones = header[6];
    western->rules._C = header[7];
    western->rules.skipPhone = header[8];
    s8 letterKinds = header[9];
    western->rules.boundary = letterKinds;
    western->rules._20 = header[10];
    u16 letterNum = header[11];
    western->rules.letterNum = letterNum;
    s32 setNum = header[12];
    western->rules._14 = setNum;
    if (letterKinds > 45 || setNum > 10) {
        return -1;
    }
    u32 used = keyNum * 4 + 52;
    if (used > file->size) {
        return -1;
    }
    const u8* data = reinterpret_cast<const u8*>(header + 13);
    western->info.keys = reinterpret_cast<const u32*>(data);
    data += keyNum * 4;
    for (s32 i = 0; i < letterKinds; i++) {
        used += 8;
        if (file->size < used) {
            return -1;
        }
        RuleLetter& letter = western->letters[i];
        letter.letter = *reinterpret_cast<const u32*>(data);
        u32 size = reinterpret_cast<const u32*>(data)[1];
        data += 8;
        letter._8 = size;
        used += size;
        if (file->size < used) {
            return -1;
        }
        letter.rules = reinterpret_cast<const u32*>(data);
        data += size;
    }
    used += 2;
    s32 phoneNum = *reinterpret_cast<const s16*>(data);
    if (used > file->size || phoneNum > 300) {
        return -1;
    }
    data += 2;
    s32 p;
    for (p = 0; p < phoneNum; p++) {
        used += 2;
        if (file->size < used) {
            return -1;
        }
        s32 length = *reinterpret_cast<const s16*>(data);
        data += 2;
        used += length * 2;
        if (file->size < used) {
            return -1;
        }
        western->phones[p] = reinterpret_cast<const s16*>(data);
        data += length * 2;
    }
    western->phones[p] = NULL;
    used += western->rules.letterNum * 2 + 2;
    if (used > file->size) {
        return -1;
    }
    western->rules.letterMap = reinterpret_cast<const s16*>(data);
    data += (western->rules.letterNum + 1) * 2;
    for (s32 i = 0; i < western->rules._14; i++) {
        used++;
        if (file->size < used) {
            return -1;
        }
        u32 length = *data++;
        used += length;
        if (file->size < used) {
            return -1;
        }
        western->sets[i] = reinterpret_cast<const char*>(data);
        data += length;
    }
    used += western->info.stringBits + western->info.writerTableBits + western->info.readerTreeBits + 1;
    if (file->size < used) {
        return -1;
    }
    western->info.writerTable = data;
    data += western->info.writerTableBits;
    western->info.readerTree = data;
    data += western->info.readerTreeBits;
    western->info.keyStrings = data;
    const u8* phoneMap = data + western->info.stringBits;
    u32 phoneMapSize = *phoneMap++;
    used += phoneMapSize;
    if (file->size < used) {
        return -1;
    }
    western->info.phoneMap = phoneMap;
    memset(tables, 0, sizeof(WesternTables));
    if (used < file->size) {
        const u8* names = phoneMap + phoneMapSize;
        s32 nameNum = *names++;
        used++;
        tables->phoneNum = nameNum;
        if (nameNum > 65) {
            return -1;
        }
        for (s32 i = 0; i < nameNum; i++) {
            used++;
            if (file->size < used) {
                return -1;
            }
            u32 length = *names++;
            tables->phones[i].length = length - 1;
            used += length + 1;
            if (file->size < used) {
                return -1;
            }
            tables->phones[i].name = reinterpret_cast<const char*>(names);
            names += length;
            tables->phones[i].id = *names++;
        }
        if (file->size != used) {
            return -1;
        }
    }
    tables->isLexicon = 1;
    if (WesternInitialize(&tables->writer, western->info.writerTable, western->info.writerTableBits, &tables->reader,
                          reinterpret_cast<const u16*>(western->info.readerTree), western->info.readerTreeBits,
                          western->info._1C, western->info.keyStrings, western->info.stringBits) == 0) {
        return -1;
    }
    tables->info = &western->info;
    return 0;
}

DECOMP_ALWAYS_INLINE bool LanguageTablesReady(Recognizer* rec, u8 language)
{
    if (reinterpret_cast<WesternTables*>(rec->westernTables)->info != NULL) {
        return true;
    }
    return language == 1 || language == 9 || language == 10;
}

} // namespace

// a debug message of the library, empty in this build (armlink folded it with another empty function)
// 0x0014E1EC (folded)
DECOMP_NOIPA void DebugMessage(s32, u32)
{
    // empty (in the original too)
}

// 0x0015A714
void GlobalInitialize()
{
    DebugMessage(0, 0x4000361);
}

// 0x00152CAC
void DctInitialize(Dct* dct, s32, s32 inputNum)
{
    memset(dct, 0, sizeof(Dct));
    dct->outputNum = 12;
    dct->_8 = 12;
    dct->inputNum = inputNum;
    for (s32 i = 0; i < inputNum; i++) {
        dct->logBand[i] = 1;
    }
}

// 0x00162B4C
void RecognizerInitialize(Recognizer* rec, VadParams* params, s32, s32, s32)
{
    memset(rec, 0, sizeof(Recognizer));
    rec->savedStats = rec->defaultStats;
    rec->params = params;
    DctInitialize(&rec->dct, reinterpret_cast<intptr_t>(params), 13);
    rec->frame.bandNum = 13;
    rec->frame.sampleNum = FRAME_SAMPLES;
    rec->frame.samples = rec->samples;
    rec->params->threshold = 1;
    rec->params->maxLevel = 60;
    rec->params->mode = 3;
    VadInitialize(&rec->vad, 13, params);
    rec->isAdaption = 1;
    rec->peakSample = 9000;
    rec->gain = 256;
    rec->isAgc = 1;
    for (s32 i = 0; i < COEFFICIENT_NUM; i++) {
        rec->savedStats[i] = 0;
    }
    for (s32 i = 0; i < COEFFICIENT_NUM; i++) {
        rec->speechSum[i] = 0;
    }
    rec->speechNum = 0;
}

// 0x00162C90
s32 RecognizerLoadFiles(Recognizer* rec, u8 language, const File* files, s32 fileNum)
{
    rec->language = language;
    memset(&rec->model, 0, sizeof(Model));
    memset(&rec->dict, 0, offsetof(Dictionary, nameTable));
    memset(&rec->tree, 0, sizeof(PhoneTree));
    rec->network = NULL;
    LanguageTablesClear(rec, language);

    for (s32 i = 0; i < fileNum; i++) {
        const File& file = files[i];
        const u32* data = reinterpret_cast<const u32*>(file.data);
        switch (file.kind) {
        case FILE_MODEL: {
            if (file.size < 44 || data[0] != language) {
                return -1;
            }
            Model& model = rec->model;
            s32 streamNum = data[1];
            model.streamNum = streamNum;
            if (streamNum > 3) {
                return -1;
            }
            s32 codewordNum = data[2];
            model.codewordNum = codewordNum;
            if (streamNum * codewordNum > 1536) {
                return -1;
            }
            s32 senoneNum = data[3];
            model.senoneNum = senoneNum;
            if (static_cast<s32>(data[4]) != senoneNum || senoneNum > 2000) {
                return -1;
            }
            s32 meanNum = data[5];
            model.meanNum = meanNum;
            s32 precisionNum = data[6];
            model.precisionNum = precisionNum;
            if (meanNum != precisionNum) {
                return -1;
            }
            s32 senoneListNum = data[7];
            model.senoneListNum = senoneListNum;
            s32 stateNum = data[8];
            model.stateNum = stateNum;
            if (stateNum > 700) {
                return -1;
            }
            s32 weightNum = data[9];
            model.weightNum = weightNum;
            s32 size = stateNum * 4 + meanNum * 2 + precisionNum * 2 + senoneListNum * 2 + weightNum * 2 +
                       (codewordNum > 256 ? 2 : 1) * streamNum * senoneNum + 42;
            if (size != static_cast<s32>(file.size)) {
                return -1;
            }
            const u8* p = reinterpret_cast<const u8*>(data + 10);
            model.stateRanges = reinterpret_cast<const u16*>(p);
            p += stateNum * 4;
            model.means = const_cast<s16*>(reinterpret_cast<const s16*>(p));
            p += meanNum * 2;
            model.precisions = reinterpret_cast<const u16*>(p);
            p += precisionNum * 2;
            model.senones = reinterpret_cast<const u16*>(p);
            p += senoneListNum * 2;
            model.weights = reinterpret_cast<const s16*>(p);
            p += weightNum * 2;
            if (*reinterpret_cast<const u16*>(p) == 0) {
                return -1;
            }
            model.senoneCodewords = p + 2;
            break;
        }
        case FILE_DICTIONARY: {
            if (file.size < 12 || data[0] != language) {
                return -1;
            }
            Dictionary& dict = rec->dict;
            s32 phoneNum = data[1];
            dict.phoneNum = phoneNum;
            s32 stateNum = data[2];
            dict.stateNum = stateNum;
            s32 size = phoneNum * 8 + stateNum * 12 + 12;
            if (static_cast<s32>(file.size) < size) {
                return -1;
            }
            dict.phones = reinterpret_cast<const PhoneInfo*>(data + 3);
            const PhoneState* states = reinterpret_cast<const PhoneState*>(dict.phones + phoneNum);
            dict.states = states;
            if (size < static_cast<s32>(file.size)) {
                if (file.size < static_cast<u32>(size + 4)) {
                    return -1;
                }
                const u32* namesSize = reinterpret_cast<const u32*>(states + stateNum);
                if (file.size != size + *namesSize + 4) {
                    return -1;
                }
                const char* name = reinterpret_cast<const char*>(namesSize + 1);
                const char* end = name + *namesSize;
                dict.names = dict.nameTable;
                for (s32 j = 0; j < phoneNum; j++) {
                    dict.nameTable[j] = name;
                    while (*name != '\0' && name < end) {
                        name++;
                    }
                    if (name == end) {
                        return -1;
                    }
                    name++;
                }
            }
            break;
        }
        case FILE_ADAPTION: {
            s32 codewordNum = rec->model.codewordNum;
            if (file.size != static_cast<u32>(0x1114 + codewordNum * 15 * 2) || data[0] != language) {
                return -1;
            }
            const u8* p = reinterpret_cast<const u8*>(data + 1);
            s32 weights = codewordNum * 5 * 2;
            for (s32 j = 0; j < 3; j++) {
                rec->adaption[j].matrix = reinterpret_cast<const s16*>(p);
                p += 1440;
                rec->adaption[j].weights = reinterpret_cast<const s16*>(p);
                p += weights;
            }
            rec->channelMean = const_cast<s32*>(reinterpret_cast<const s32*>(p));
            break;
        }
        case FILE_TREES: {
            if (file.size == 0) {
                break;
            }
            rec->treeFileSize = file.size;
            rec->treeFileUsed = 44;
            if (file.size < 44 || data[0] != language) {
                return -1;
            }
            PhoneTree& tree = rec->tree;
            tree.silencePhone = data[1];
            tree.silenceClass = data[2];
            tree.leafOffset = data[3];
            tree.leafNum = data[4];
            tree.stateNum = data[5];
            tree.phoneNum = data[6];
            s32 phoneNum = data[7];
            s32 mapSize = data[8];
            s32 treeNum = data[9];
            s32 setNum = data[10];
            rec->treeFileUsed = mapSize + phoneNum * 4 + 44;
            if (rec->treeFileUsed > file.size) {
                return -1;
            }
            const u8* p = reinterpret_cast<const u8*>(data + 11);
            tree.firstLeaves = reinterpret_cast<const u32*>(p);
            p += phoneNum * 4;
            tree.map = p;
            p += mapSize;
            if (setNum > 100 || treeNum > 65) {
                return -1;
            }
            for (s32 j = 0; j < setNum; j++) {
                rec->treeFileUsed++;
                if (rec->treeFileUsed > file.size) {
                    return -1;
                }
                u32 num = *p++;
                rec->phoneSets[j].num = num;
                rec->treeFileUsed += num;
                if (rec->treeFileUsed > file.size) {
                    return -1;
                }
                rec->phoneSets[j].phones = p;
                p += num;
            }
            rec->treeNodeCursor = rec->treeNodes;
            rec->questionCursor = rec->questions;
            rec->leafCount = 0;
            memset(rec->treeNodes, 0, sizeof(rec->treeNodes));
            memset(rec->questions, 0, sizeof(rec->questions));
            rec->treeCursor = p;
            for (s32 j = treeNum - 1; j >= 0; j--) {
                if (tree.silenceClass == j) {
                    rec->trees[j] = NULL;
                    continue;
                }
                TreeNode* root = ReadTreeNode(rec);
                rec->trees[j] = root;
                if (root == NULL) {
                    return -1;
                }
                if (root->yes != NULL) {
                    NumberLeaves(rec, root->yes);
                    NumberLeaves(rec, root->no);
                } else {
                    root->leaf = rec->leafCount;
                    rec->leafCount++;
                }
            }
            if (rec->treeFileSize != rec->treeFileUsed) {
                return -1;
            }
            tree.trees = rec->trees;
            break;
        }
        case FILE_LANGUAGE: {
            s32 result = LanguageTablesLoad(rec, language, &file);
            if (result != 0) {
                return result;
            }
            break;
        }
        case FILE_STATS:
            if (file.size == 0) {
                break;
            }
            if (file.size != 96) {
                return -1;
            }
            memcpy(rec->speechSum, data, 96);
            rec->speechNum = 700;
            for (s32 j = 0; j < COEFFICIENT_NUM; j++) {
                rec->stats[j] = (rec->speechSum[j] - rec->savedStats[j]) / rec->speechNum;
            }
            if (rec->speechNum > 1400) {
                for (s32 j = 0; j < COEFFICIENT_NUM; j++) {
                    rec->speechSum[j] = rec->speechSum[j] / rec->speechNum * 700;
                    rec->savedStats[j] = rec->savedStats[j] / rec->speechNum * 700;
                }
                rec->speechNum = 700;
            }
            break;
        }
    }

    if (rec->model.weights == NULL || rec->dict.states == NULL) {
        return -1;
    }
    if (rec->tree.trees == NULL && rec->dict.names == NULL) {
        return -1;
    }
    if (!LanguageTablesReady(rec, language)) {
        return -1;
    }
    rec->isLoaded = 0;
    return 0;
}

// 0x00163CE8
void NetworkReset(Recognizer* rec)
{
    rec->isReady = 1;
}

// 0x0016152C
s32 PriorGetTotalStates(const PhoneTree* tree)
{
    return tree->leafOffset + tree->leafNum;
}

// 0x0016153C
s32 PriorGetEdgeStates(const PhoneTree* tree, const PhoneInfo* phones, s32* states)
{
    s32 base = tree->silencePhone;
    s32 num = phones[base].stateNum - 2;
    for (s32 i = num - 1; i >= 0; i--) {
        states[i] = tree->firstLeaves[base] + tree->leafOffset + i;
    }
    return num;
}

// the leaf of a tree: the questions look at the phone and its neighbours
// 0x001615E0
s32 PriorGetStates(const PhoneTree* tree, const PhoneInfo* phones, const u8* context, s32* states)
{
    u32 center = context[1];
    if (static_cast<s32>(center) > tree->phoneNum) {
        return -1;
    }
    s32 num = phones[center].stateNum - 2;
    u8 classes[3];
    classes[2] = tree->map[context[2]];
    classes[1] = tree->map[context[1]];
    classes[0] = tree->map[context[0]];
    if (classes[1] == 255 || classes[1] == static_cast<u32>(tree->silenceClass)) {
        for (s32 i = num - 1; i >= 0; i--) {
            states[i] = tree->firstLeaves[center] + tree->leafOffset + i;
        }
        return num;
    }
    for (s32 i = num - 1; i >= 0; i--) {
        const TreeNode* node = tree->trees[classes[1]];
        s32 leaf;
        while (node->yes != NULL) {
            const TreeQuestion* question = node->question;
            bool isYes = false;
            switch (question->kind) {
            case 'C': {
                u32 phone = static_cast<u32>(question->position + 1) < 3 ? classes[question->position + 1]
                                                                         : tree->silenceClass & 0xFF;
                const PhoneSet* set = static_cast<const PhoneSet*>(question->value);
                s32 j;
                for (j = 0; j < set->num; j++) {
                    if (set->phones[j] == phone) {
                        break;
                    }
                }
                isYes = j < set->num;
                break;
            }
            case 'P': {
                u32 value = reinterpret_cast<uintptr_t>(question->value) & 0xFF;
                u32 phone = static_cast<u32>(question->position + 1) < 3 ? classes[question->position + 1]
                                                                         : tree->silenceClass;
                isYes = value == phone;
                break;
            }
            case 'M':
                isYes = question->position > i;
                break;
            case 'G':
                isYes = static_cast<u8>(question->position) == 1;
                break;
            case 'W': {
                s32 isEdge = classes[0] == tree->silenceClass || classes[2] == tree->silenceClass;
                isYes = isEdge == question->position;
                break;
            }
            default:
                leaf = -1;
                goto store;
            }
            node = isYes ? node->yes : node->no;
        }
        leaf = node->leaf;
    store:
        states[i] = leaf;
    }
    return num;
}

// 0x00161884
Network* NetworkBuildWords(NetworkBuilder* builder, u8 language, const char* const* words, const s16* ids, s32 num,
                           void* tables, const Dictionary* dict, const PhoneTree* tree)
{
    if (tree != NULL && tree->phoneNum != dict->phoneNum) {
        return NULL;
    }
    builder->wordNum = 0;
    builder->phoneDataSize = 0;
    if (tree != NULL) {
        builder->stateNum = tree->stateNum;
    } else {
        s32 maxStates = 0;
        for (s32 i = dict->phoneNum - 1; i >= 0; i--) {
            if (dict->phones[i].stateNum > maxStates) {
                maxStates = dict->phones[i].stateNum;
            }
        }
        builder->stateNum = maxStates - 2;
    }
    s32 variantNum = 2;
    const u8* phoneMap;
    switch (language) {
    case 1:
        if (!LanguageJapaneseInitialize(tables)) {
            return NULL;
        }
        // fall through
    case 9:
        phoneMap = NULL;
        break;
    case 10:
        phoneMap = LanguageChineseGetPhoneMap(tables);
        break;
    default:
        phoneMap = LanguageWesternGetPhoneMap(tables);
        break;
    }

    const char* const* names;
    s32 nameNum = 0;
    s32 silence;
    if (tree != NULL) {
        silence = tree->silencePhone;
        if (silence > dict->phoneNum) {
            return NULL;
        }
        names = NULL;
    } else {
        for (silence = dict->phoneNum - 1; silence >= 0; silence--) {
            if (strcmp(dict->names[silence], s_SilenceName) == 0) {
                break;
            }
        }
        if (silence < 0) {
            return NULL;
        }
        nameNum = dict->phoneNum;
        names = dict->names;
    }

    u16** parts = reinterpret_cast<u16**>(builder->groups[0]);
    u16** cursors = reinterpret_cast<u16**>(builder->groups[1]);
    for (s32 w = 0; w < num; w++) {
        strcpy(builder->word, words[w]);
        u16 id = ids[w];
        // the German sharp s
        if (language == 6) {
            char* c = builder->word;
            while (*c != '\0') {
                if (*c == static_cast<char>(0xDF)) {
                    *c = 's';
                    char* end = c + 1;
                    while (*end != '\0') {
                        end++;
                    }
                    end[1] = '\0';
                    while (end > c) {
                        *end = end[-1];
                        end--;
                    }
                    c += 2;
                } else {
                    c++;
                }
            }
        }
        char* part = builder->word;
        char* wordEnd = part + strlen(part);
        s32 partNum = 0;
        s32 used = 0;
        const void* koreanTables = NULL;
        bool isGood = true;
        s32 left = 0;
        while (part < wordEnd) {
            part = const_cast<char*>(SkipSeparators(part, wordEnd));
            if (part == wordEnd) {
                break;
            }
            if (partNum == BUILDER_MAX_PARTS) {
                return NULL;
            }
            char* next = part;
            while (next < wordEnd && *next != ' ' && *next != '\t' && *next != '_') {
                u8 c = *next;
                bool isDouble = false;
                switch (language) {
                case 1:
                    isDouble = (c & 0x80) != 0;
                    break;
                case 9:
                    isDouble = c >= 160;
                    break;
                case 10:
                    isDouble = c >= 129;
                    break;
                }
                next += isDouble ? 2 : 1;
            }
            if (next != wordEnd) {
                *next++ = '\0';
                next = const_cast<char*>(SkipSeparators(next, wordEnd));
            }
            size_t length = strlen(part);
            if (length > 128) {
                return NULL;
            }
            if (language != 1 && language != 9 && length > 32) {
                return NULL;
            }
            u16* out = reinterpret_cast<u16*>(reinterpret_cast<u8*>(&builder->trash) + used);
            parts[partNum] = out;
            left = 64000 - used;
            if (*part == '#') {
                // written with phones
                const char* p = part + 1;
                s32 remaining = strlen(p);
                s32 count = 0;
                u32 koreanIndex = 0xFFFF;
                if (language == 9) {
                    koreanTables = tables;
                    if (!LanguageKoreanHasPhones(koreanTables)) {
                        return NULL;
                    }
                } else if (!LanguageWesternHasPhones(tables)) {
                    return NULL;
                }
                isGood = true;
                while (*p != '\0') {
                    if (left < 6) {
                        return NULL;
                    }
                    if (language == 9) {
                        char* dash = strchr(const_cast<char*>(p), '-');
                        if (dash != NULL) {
                            *dash = '\0';
                        }
                        koreanIndex = LanguageKoreanFindPhone(koreanTables, p);
                        if (koreanIndex == 0xFFFF) {
                            if (*p != '\0') {
                                return NULL;
                            }
                            break;
                        }
                        const u16* entry = LanguageKoreanGetPhones(koreanTables, koreanIndex);
                        out[0] = entry[0];
                        out[1] = entry[1];
                        out += 2;
                        count += 4;
                        left -= 4;
                        p += LanguageKoreanGetPhoneLength(koreanTables, koreanIndex);
                        if (dash != NULL) {
                            p++;
                        }
                    } else {
                        s32 index = LanguageWesternFindPhone(tables, p, remaining);
                        if (index < 0) {
                            isGood = false;
                            if (*p != '\0') {
                                return NULL;
                            }
                            break;
                        }
                        count += 2;
                        *out++ = LanguageWesternGetPhoneId(tables, index);
                        left -= 2;
                        s32 phoneLength = LanguageWesternGetPhoneLength(tables, index);
                        p += phoneLength;
                        remaining -= phoneLength;
                    }
                }
                out[0] = 0;
                out[1] = 0;
                left = count + 4;
            } else if (names != NULL) {
                s32 index;
                for (index = nameNum - 1; index >= 0; index--) {
                    if (strcmp(part, names[index]) == 0) {
                        break;
                    }
                }
                if (index < 0) {
                    return NULL;
                }
                if (left < 6) {
                    return NULL;
                }
                out[0] = index + 1;
                out[1] = 0;
                out[2] = 0;
                left = 6;
            } else {
                s32 unused;
                s8 isOk;
                switch (language) {
                case 1:
                    isOk = LanguageJapaneseConvert(tables, part, variantNum, out, &unused, &left);
                    break;
                case 9:
                    isOk = LanguageKoreanConvert(tables, part, variantNum, out, &unused, &left);
                    break;
                case 10:
                    isOk = LanguageChineseConvert(tables, part, variantNum, out, &unused, &left);
                    break;
                default:
                    isOk = LanguageWesternConvert(tables, part, variantNum, out, &unused, &left);
                    break;
                }
                isGood = isOk != 0;
            }
            if (!isGood) {
                return NULL;
            }
            partNum++;
            part = next;
            used += left;
        }

        // all combinations of the pronunciations of the parts, at most variantNum
        memcpy(cursors, parts, partNum * sizeof(u16*));
        builder->phones[0] = silence;
        s32 variant = 0;
        const s32 lastPart = partNum - 1;
        while (true) {
            u8* phones = builder->phones + 1;
            for (s32 p = 0; p < partNum; p++) {
                const u16* source = cursors[p];
                s32 length = U16Length(source);
                for (s32 i = 0; i < length; i++) {
                    u16 value = source[i];
                    if (phoneMap != NULL && names == NULL) {
                        *phones++ = phoneMap[value & 0xFF];
                    } else {
                        *phones++ = value - 1;
                    }
                }
            }
            *phones++ = silence;
            s32 size = phones - builder->phones;
            s32 wordIndex = builder->wordNum;
            WordEntry* entry = &builder->words[wordIndex];
            s32 dataSize = builder->phoneDataSize;
            if (dataSize + size > BUILDER_STATES) {
                return NULL;
            }
            if (wordIndex == BUILDER_WORDS) {
                return NULL;
            }
            memcpy(&builder->phoneData[dataSize], builder->phones, size);
            entry->phones = &builder->phoneData[builder->phoneDataSize];
            entry->index = builder->wordNum;
            entry->id = id;
            entry->phonePos = -1;
            entry->statePos = -1;
            if ((builder->wordNum + 1) * builder->stateNum > BUILDER_STATES) {
                return NULL;
            }
            entry->states = &builder->stateData[builder->wordNum * builder->stateNum];
            entry->stateNum = 0;
            entry->next = entry + 1;
            entry->phoneNum = size;
            builder->phoneDataSize += size;
            builder->wordNum++;
            if (++variant == variantNum) {
                break;
            }
            // the next combination
            s32 p;
            for (p = 0; p < partNum; p++) {
                const u16* source = cursors[p];
                const u16* following = source + U16Length(source) + 1;
                cursors[p] = const_cast<u16*>(following);
                if (*following != 0) {
                    break;
                }
                if (p == lastPart) {
                    p = -1;
                    break;
                }
                cursors[p] = parts[p];
            }
            if (p < 0) {
                break;
            }
        }
    }

    if (builder->wordNum == 0) {
        return NULL;
    }
    builder->words[builder->wordNum - 1].next = NULL;
    Network* network = &builder->network;
    s32 stateTotal = tree != NULL ? PriorGetTotalStates(tree) : dict->stateNum;
    memset(network, 0, sizeof(Network) + sizeof(builder->_3D14));
    WordGroup* groups = builder->groups[0];
    WordGroup* newGroups = builder->groups[1];
    memset(groups, 0, builder->wordNum * sizeof(WordGroup));
    memset(newGroups, 0, builder->wordNum * sizeof(WordGroup));
    memset(builder->stateChains, 0, stateTotal * sizeof(WordEntry*) + sizeof(WordEntry*));
    WordEntry** chains = builder->stateChains + 1;
    groups[0].first = builder->words;
    groups[0].id = 0xFFFF;
    groups[0].isLast = 1;
    groups[0].state = 0xFFFF;
    groups[0].phone = 255;
    memset(&builder->trash, 0, sizeof(Node));
    s32 nodeCount = 1;
    Node* node = builder->nodes;
    s32 groupNum = 1;
    s32 nodeTotal = 0;
    s32 depth = 0;
    bool isMore;
    do {
        isMore = false;
        s32 newNum = 0;
        s32 previousNew = 0;
        depth++;
        s32 base = nodeTotal + groupNum;
        for (s32 g = 0; g < groupNum; g++) {
            s32 stateCount = 0;
            for (WordEntry* entry = groups[g].first; entry != NULL;) {
                WordEntry* following = entry->next;
                s32 state;
                if (entry->statePos < static_cast<s32>(entry->stateNum) - 1) {
                    entry->statePos++;
                    state = entry->states[entry->statePos];
                } else if (entry->phonePos < static_cast<s32>(entry->phoneNum) - 1) {
                    s16 pos = ++entry->phonePos;
                    s32 count;
                    if (tree == NULL) {
                        u32 phone = entry->phones[pos];
                        count = 0;
                        for (s32 i = 0; i < dict->phones[phone].stateNum - 2; i++) {
                            entry->states[i] = dict->states[dict->phones[phone].firstState + i + 1].state;
                            count++;
                        }
                    } else if (pos == 0 || pos == static_cast<s32>(entry->phoneNum) - 1) {
                        count = PriorGetEdgeStates(tree, dict->phones, entry->states);
                    } else {
                        count = PriorGetStates(tree, dict->phones, &entry->phones[pos - 1], entry->states);
                    }
                    entry->stateNum = count;
                    entry->statePos = 0;
                    state = entry->states[0];
                } else {
                    state = -1;
                }
                WordEntry* chain = chains[state];
                if (chain == NULL) {
                    builder->chainStates[stateCount++] = state;
                }
                chains[state] = entry;
                entry->next = chain;
                entry = following;
            }
            s32 lastState = stateCount - 1;
            for (s32 i = 0; i < stateCount; i++) {
                s32 state = builder->chainStates[i];
                if (state == -1) {
                    groups[g].id = builder->stateChains[0]->id;
                    if (i == lastState && stateCount != 1) {
                        newGroups[newNum - 1].isLast = 1;
                    }
                } else {
                    WordEntry* first = chains[state];
                    isMore = true;
                    WordGroup* group = &newGroups[newNum];
                    group->first = first;
                    group->id = 0xFFFF;
                    group->isLast = i == lastState;
                    group->state = state;
                    group->phone = first->statePos == static_cast<s32>(first->stateNum) - 1
                                       ? first->phones[first->phonePos]
                                       : 255;
                    WordEntry* previous = NULL;
                    s32 ended = 0;
                    for (WordEntry* entry = first; entry != NULL;) {
                        WordEntry* following = entry->next;
                        if (entry->statePos == static_cast<s32>(entry->stateNum) - 1 &&
                            entry->phonePos == static_cast<s32>(entry->phoneNum) - 1) {
                            if (ended != 0) {
                                previous->next = NULL;
                                newGroups[newNum].isLast = 0;
                                newNum++;
                                WordGroup* split = &newGroups[newNum];
                                split->first = entry;
                                split->id = 0xFFFF;
                                split->state = state;
                                // (the phone of the head of the chain)
                                split->phone = first->statePos == static_cast<s32>(first->stateNum) - 1
                                                   ? first->phones[first->phonePos]
                                                   : 255;
                                split->isLast = i == lastState;
                            }
                            ended++;
                        }
                        previous = entry;
                        entry = following;
                    }
                    newNum++;
                }
                chains[state] = NULL;
            }
            if (++nodeCount > 4000) {
                return NULL;
            }
            memset(node, 0, sizeof(Node));
            u16 id = groups[g].id;
            node->wordId = id;
            if (stateCount == 1 && id != 0xFFFF) {
                node->next = 0x7FFF;
            } else {
                node->next = previousNew + base;
            }
            previousNew = newNum;
            node->isLast = groups[g].isLast;
            node->state = groups[g].state;
            node++;
        }
        nodeTotal += groupNum;
        WordGroup* swap = groups;
        groups = newGroups;
        newGroups = swap;
        groupNum = newNum;
    } while (isMore);
    if (++nodeCount > 4000) {
        return NULL;
    }
    memset(node, 0, sizeof(Node));
    node->wordId = 0xFFFF;
    node->next = 0x7FFF;
    node->isLast = 1;
    node->state = 0xFFFF;
    network->nodes = builder->nodes;
    network->trash = &builder->trash;
    network->nodeNum = groupNum + nodeTotal;
    network->depth = depth;
    return network;
}

// 0x00163B20
s32 NetworkBuild(Recognizer* rec, s32, const char* const* words, const s16* ids, s32 num)
{
    void* tables;
    switch (rec->language) {
    case 1:
        tables = rec->japaneseTables;
        break;
    case 9:
        tables = rec->koreanTables;
        break;
    case 10:
        tables = rec->chineseTables;
        break;
    default:
        tables = rec->westernTables;
        break;
    }
    Network* network = NetworkBuildWords(reinterpret_cast<NetworkBuilder*>(rec->builder), rec->language, words, ids, num,
                                         tables, &rec->dict, rec->tree.trees != NULL ? &rec->tree : NULL);
    if (network == NULL) {
        return -1;
    }
    rec->isReady = 1;
    memset(&rec->search, 0, sizeof(Search));
    rec->search.params = rec->params;
    rec->search.model = &rec->model;
    if (rec->originalMeans == NULL) {
        rec->originalMeans = rec->model.means;
        rec->model.means = rec->adaptedMeans[0];
    }
    rec->network = network;
    rec->search.network = network;
    rec->search.kind = 32;
    rec->wordNum = num;
    rec->isReady = 1;
    return 0;
}
} // namespace asr
