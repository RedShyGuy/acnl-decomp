#pragma once

#include "asr/asr_Internal.h"

namespace asr {
// a word of the list while the network is built: walks through its phones and their states
struct WordEntry
{
    WordEntry* next;   // 0x00
    s32* states;       // 0x04 of the current phone
    s32 index;         // 0x08
    const u8* phones;  // 0x0C
    s16 phonePos;      // 0x10
    u16 stateNum;      // 0x12
    s16 statePos;      // 0x14
    u16 id;            // 0x16
    u16 phoneNum;      // 0x18
};
ASSERT_SIZE(WordEntry, 0x1C);

// a group of words that share a state at the current depth
struct WordGroup
{
    WordEntry* first;  // 0x00
    u16 id;            // 0x04 of a word that ends here, otherwise 0xFFFF
    u16 state;         // 0x06
    u8 isLast;         // 0x08 the last group of its parent
    u8 phone;          // 0x09 a phone ends here, otherwise 255
};
ASSERT_SIZE(WordGroup, 0xC);

const s32 BUILDER_WORDS = 200;
const s32 BUILDER_STATES = 2000;
const s32 BUILDER_MAX_PARTS = 600;

// the memory the network is built in; the node area also holds the phone strings while the words are read
struct NetworkBuilder
{
    s32 stateNum;                      // 0x00000 per phone
    s32 wordNum;                       // 0x00004
    s32 phoneDataSize;                 // 0x00008
    u8 phoneData[BUILDER_STATES];      // 0x0000C
    WordEntry words[BUILDER_WORDS];    // 0x007DC
    s32 stateData[BUILDER_STATES];     // 0x01DBC
    Network network;                   // 0x03CFC
    u8 _3D14[8];                       // 0x03D14
    WordGroup groups[2][BUILDER_WORDS]; // 0x03D1C also the parts of a word
    WordEntry* stateChains[701];       // 0x04FDC [state + 1]
    s32 chainStates[700];              // 0x05AD0
    u8 _65C0[4];                       // 0x065C0
    Node trash;                        // 0x065C4
    Node nodes[3999];                  // 0x065D4
    u8 phones[387];                    // 0x15FC4
    char word[257];                    // 0x16147
};
ASSERT_SIZE(NetworkBuilder, 0x16248);
ASSERT_OFFSET(NetworkBuilder, network, 0x3CFC);
ASSERT_OFFSET(NetworkBuilder, groups, 0x3D1C);
ASSERT_OFFSET(NetworkBuilder, stateChains, 0x4FDC);
ASSERT_OFFSET(NetworkBuilder, trash, 0x65C4);

void DebugMessage(s32 level, u32 code);
void GlobalInitialize(); // 0x0015A714
DECOMP_NOINLINE void DctInitialize(Dct* dct, s32, s32 inputNum); // 0x00152CAC
void RecognizerInitialize(Recognizer* rec, VadParams* params, s32, s32, s32); // 0x00162B4C
s32 RecognizerLoadFiles(Recognizer* rec, u8 language, const File* files, s32 fileNum); // 0x00162C90
void NetworkReset(Recognizer* rec); // 0x00163CE8
s32 NetworkBuild(Recognizer* rec, s32, const char* const* words, const s16* ids, s32 num); // 0x00163B20

// the triphones of a word from the decision trees
DECOMP_NOINLINE s32 PriorGetTotalStates(const PhoneTree* tree); // 0x0016152C
DECOMP_NOINLINE s32 PriorGetEdgeStates(const PhoneTree* tree, const PhoneInfo* phones, s32* states); // 0x0016153C
s32 PriorGetStates(const PhoneTree* tree, const PhoneInfo* phones, const u8* context, s32* states); // 0x001615E0
Network* NetworkBuildWords(NetworkBuilder* builder, u8 language, const char* const* words, const s16* ids, s32 num,
                           void* tables, const Dictionary* dict, const PhoneTree* tree); // 0x00161884
} // namespace asr
