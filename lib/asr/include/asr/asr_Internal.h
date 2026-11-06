#pragma once

// The speech recognition library behind SoundMicAsr: a fixed point recognizer for 11025 Hz audio in frames of
// 128 samples. It has no symbols and no strings except its version; all names are ours.
//
// The application hands over one work area (WORK_SIZE bytes) and six files from Sound/asr/<language>/, sets a
// word list ("id:word" lines) and feeds samples:
//   frames -> pre-emphasis -> half band filter tree (13 bands) -> voice activity detection
//          -> log + DCT (12 coefficients) + two deltas (36 values) -> codebook distances -> senones
//          -> Viterbi search over a network of the words -> the best words with scores
// A channel adaption moves the means of the codebook by what the frames looked like before the search.

#include "decomp.h"

namespace asr {
// the size of a frame and of the first frame, which also measures the noise
const s32 FRAME_SAMPLES = 128;
const s32 FIRST_FRAME_SAMPLES = 626;
// the frames are kept in a ring
const s32 FRAME_RING = 29;
// a frame of the ring: 12 coefficients and their two deltas
const s32 COEFFICIENT_NUM = 12;
const s32 VECTOR_NUM = 36;
// the "no value" of the scores
const s32 SCORE_NONE = static_cast<s32>(0xFA0A1F00);

// how many bits the fixed point format of a value of a frame vector has after the point
DECOMP_ALWAYS_INLINE s32 GetVectorShift(s32 i)
{
    return (i == 0 ? 4 : 0) + (static_cast<u32>(i - 1) <= 1 ? 7 : 0) + (static_cast<u32>(i - 3) <= 8 ? 8 : 0) +
           (i == 12 ? 5 : 0) + (static_cast<u32>(i - 13) <= 1 ? 8 : 0) + (static_cast<u32>(i - 15) <= 8 ? 9 : 0) +
           (i == 24 ? 6 : 0) + (static_cast<u32>(i - 25) <= 1 ? 9 : 0) + (static_cast<u32>(i - 27) <= 8 ? 10 : 0);
}

// the parameters of the voice activity detection from a file
struct VadParams
{
    u8 mode;       // 0x0 2 or 3: the energies decide too
    u8 _1[3];
    u32 threshold; // 0x4
    s32 maxLevel;  // 0x8 above it a frame is too loud
    s32 _C;        // 0xC
    s32 sampleRate; // 0x10
    s32 _14;       // 0x14
    s32 searchResults; // 0x18
    s32 _1C;       // 0x1C
};

// what the voice activity detection says about a frame
struct VadResult
{
    s32 relativeLevel; // 0x0
    s32 maxLevel;      // 0x4
    s32 state;         // 0x8 STATE_*
};
ASSERT_SIZE(VadResult, 0xC);

enum VadState
{
    VAD_SILENCE = 1,
    VAD_SPEECH = 2,
    VAD_TOO_LOUD = 3,
};

// the energies of the first frames
struct VadEntry
{
    u32 sum;     // 0x00
    u32 band[4]; // 0x04
};
ASSERT_SIZE(VadEntry, 0x14);

// the voice activity detection: four groups of bands against a noise model
struct Vad
{
    s32 bandNum;          // 0x000 of all four groups
    s32 calibrationNum;   // 0x004
    s32 calibrationLeft;  // 0x008
    s32 isSpeech;         // 0x00C
    u32 threshold;        // 0x010
    s32 peak;             // 0x014
    s32 dcOffset;         // 0x018 the sum while calibrating
    u32 sampleNum;        // 0x01C
    s32 averagePeak;      // 0x020
    u32 noiseEnergy;      // 0x024
    u32 energy[4];        // 0x028
    u32 noise[4];         // 0x038
    u32 inverseVariance[4]; // 0x048
    s32 noiseLevel;       // 0x058
    s32 maxLevel;         // 0x05C
    u32 minEnergy;        // 0x060
    s32 relativeLevel;    // 0x064
    u8 isVoiced[5];       // 0x068
    u8 history[15];       // 0x06D
    const VadParams* _7C;    // 0x07C
    const VadParams* params; // 0x080
    s32 inverseShift[4];  // 0x084
    s16 sortedPeak[16];   // 0x094
    VadEntry entries[16]; // 0x0B4
    s16 peakRing[25];     // 0x1F4
    u8 _226[2];           // 0x226
    u32 energyRing[25][4]; // 0x228
    s32 ringPos;          // 0x3B8
    s32 ringNum;          // 0x3BC
};
ASSERT_SIZE(Vad, 0x3C0);

// the bands of a frame
struct FrameInfo
{
    s32 bandNum;      // 0x00
    s32 _4;           // 0x04
    s32 sampleNum;    // 0x08
    s16* samples;     // 0x0C
    s32 _10;          // 0x10
    s32 band[13];     // 0x14
};
ASSERT_SIZE(FrameInfo, 0x48);

// the state of the half band filter tree (global: there is only one recognizer)
struct FilterTreeControl
{
    u8 isOdd[5];      // 0x00 every other sample goes on to the next level
    u8 _5;
    u16 isFilled[5];  // 0x06 the ring of the level has enough samples (only the first frame waits for it)
    s16 head[8];      // 0x10
    void (*filter)(const s32* ring, s32* low, s32* high); // 0x20
};
ASSERT_SIZE(FilterTreeControl, 0x24);

struct FilterTreeRings
{
    s32 input[16];    // 0x000
    s32 low1[16];     // 0x040
    s32 high1[16];    // 0x080
    s32 low2[16];     // 0x0C0
    s32 high2[16];    // 0x100
    s32 lowHigh[16];  // 0x140 from high1
    s32 highHigh[16]; // 0x180 from high1
    s32 low3[16];     // 0x1C0
    s32 high3[16];    // 0x200
    s32 highOfHigh2[16]; // 0x240
    s32 lowOfHigh2[16];  // 0x280
    s32 band9[64];    // 0x2C0
    s32 band10[64];   // 0x3C0
    s32 band11[64];   // 0x4C0
    s32 band12[64];   // 0x5C0
    s32 low4[16];     // 0x6C0
    s32 high4[16];    // 0x700
    s32 band3[32];    // 0x740
    s32 band4[32];    // 0x7C0
    s32 band5[32];    // 0x840
    s32 band6[32];    // 0x8C0
    s32 band7[32];    // 0x940
    s32 band8[32];    // 0x9C0
    s32 low5[16];     // 0xA40
    s32 band0[16];    // 0xA80
    s32 band1[16];    // 0xAC0
    s32 band2[16];    // 0xB00
};
ASSERT_SIZE(FilterTreeRings, 0xB40);

// the log energies of the bands and what the DCT needs
struct Dct
{
    s32 _0;          // 0x00
    s32 outputNum;   // 0x04
    s32 _8;          // 0x08
    s32 inputNum;    // 0x0C
    s32 logBand[17]; // 0x10
};
ASSERT_SIZE(Dct, 0x54);

// a node of the search network (they are linked in lists of active nodes)
struct Node
{
    Node* link;    // 0x00 in a list
    s32 score;     // 0x04
    u16 state;     // 0x08
    u16 word;      // 0x0A
    u32 wordId : 16; // 0x0C of a word that ends here, otherwise 0xFFFF
    u32 next : 15;   // the index of the next group, 0x7FFF: none
    u32 isLast : 1;  // the last of its group
};
ASSERT_SIZE(Node, 0x10);

// the network: the words as chains of states
struct Network
{
    s32 _0;        // 0x00
    s32 nodeNum;   // 0x04 (there is one more node as the end of the lists)
    s32 depth;     // 0x08 of the active lists
    s32 _C;        // 0x0C
    Node* nodes;   // 0x10
    Node* trash;   // 0x14 helps to unlink pruned nodes
};

// the acoustic model
struct Model
{
    s32 stateNum;      // 0x00
    s32 senoneNum;     // 0x04
    s32 senoneListNum; // 0x08
    s32 weightNum;     // 0x0C
    s32 meanNum;       // 0x10
    s32 precisionNum;  // 0x14
    const u16* stateRanges; // 0x18 two u16 per state: senone count, first
    const u16* senones;     // 0x1C
    const s16* weights;     // 0x20
    s16* means;             // 0x24 one row per codeword: 3 streams of 12 values
    const u16* precisions;  // 0x28
    s32 streamNum;          // 0x2C
    s32 codewordNum;        // 0x30 256: the senone table has bytes
    const void* senoneCodewords; // 0x34 three codewords per senone
    u8 _38[0x88 - 0x38];
    s32 stateScores[700];   // 0x88
    s32 senoneScores[2001]; // 0xB78
    s32 distances[1536];    // 0x2ABC stream values per codeword
};
ASSERT_OFFSET(Model, stateScores, 0x88);
ASSERT_OFFSET(Model, senoneScores, 0xB78);
ASSERT_OFFSET(Model, distances, 0x2ABC);
ASSERT_SIZE(Model, 0x42BC);

// the search over the network
struct Search
{
    const VadParams* params; // 0x000
    s32 kind;            // 0x004
    s32 threshold;       // 0x008 of the beam
    Network* network;    // 0x00C
    Model* model;        // 0x010
    s32 listNum;         // 0x014
    Node* lists[582];    // 0x018 of active nodes per group
    s32 frameNum;        // 0x930
    s32 activeSum;       // 0x934
    s32 busyFrames;      // 0x938
};
ASSERT_OFFSET(Search, frameNum, 0x930);

// a phone of the dictionary
struct PhoneInfo
{
    u8 stateNum;   // 0x0 with the two silent ends
    u8 _1[3];
    u32 firstState; // 0x4
};
ASSERT_SIZE(PhoneInfo, 0x8);

// a transition of the dictionary
struct PhoneState
{
    s32 state;     // 0x0
    s32 _4[2];     // 0x4
};
ASSERT_SIZE(PhoneState, 0xC);

// a question of the decision trees (8 bytes)
struct TreeQuestion
{
    const void* value; // 0x0 a value or a phone set
    s8 kind;           // 0x4 'C', 'P', 'M', 'G', 'W'
    s8 position;       // 0x5
    u8 _6[2];
};
ASSERT_SIZE(TreeQuestion, 0x8);

// a node of a decision tree
struct TreeNode
{
    TreeQuestion* question; // 0x0 leaves: unused
    TreeNode* yes;          // 0x4 NULL: this is a leaf
    union
    {
        TreeNode* no;       // 0x8
        s32 leaf;           // 0x8 the number of the leaf
    };
};
ASSERT_SIZE(TreeNode, 0xC);

// a phone set of the questions
struct PhoneSet
{
    const u8* phones; // 0x0
    s32 num;          // 0x4
};
ASSERT_SIZE(PhoneSet, 0x8);

// the phones with their decision trees (the context dependent model)
struct PhoneTree
{
    TreeNode** trees;          // 0x00 per phone
    const u8* map;             // 0x04 phone -> context class
    const u32* firstLeaves;    // 0x08 per phone
    s32 silencePhone;          // 0x0C the phone at both ends of every word
    s32 silenceClass;          // 0x10 its context class (it has no tree)
    s32 leafOffset;            // 0x14
    s32 leafNum;               // 0x18
    s32 stateNum;              // 0x1C
    s32 phoneNum;              // 0x20
};
ASSERT_SIZE(PhoneTree, 0x24);

// the context independent dictionary
struct Dictionary
{
    s32 phoneNum;              // 0x00
    s32 stateNum;              // 0x04
    const PhoneInfo* phones;   // 0x08
    const char* const* names;  // 0x0C
    const PhoneState* states;  // 0x10
    const char* nameTable[100]; // 0x14
};
ASSERT_SIZE(Dictionary, 0x1A4);

// the recognizer proper
struct Recognizer
{
    Dct dct;                    // 0x00000
    Vad vad;                    // 0x00054
    s32 startScore;             // 0x00414
    Model model;                // 0x00418
    u8 _46D4[4];                // 0x046D4
    Dictionary dict;            // 0x046D8
    PhoneTree tree;             // 0x0487C
    Network* network;           // 0x048A0
    Search search;              // 0x048A4
    VadParams* params;          // 0x051E0
    u8 language;                // 0x051E4
    FrameInfo frame;            // 0x051E8
    VadResult result;           // 0x05230
    s16 ring[FRAME_RING][VECTOR_NUM]; // 0x0523C
    s16 previousSample;         // 0x05A64
    s16 samples[FIRST_FRAME_SAMPLES]; // 0x05A66
    s32 emphasized[FIRST_FRAME_SAMPLES]; // 0x05F4C
    s32 frameIndex;             // 0x06914
    s32 frameCount;             // 0x06918 negative for the first frames
    s32 _691C;                  // 0x0691C
    s32 wordNum;                // 0x06920
    s8 adaptStep;               // 0x06924 0: before, 1, 2, 3: the three streams, -2: done
    u8 isLoaded;                // 0x06925 cleared when the files are loaded
    u8 isAdaption;              // 0x06926
    u8 isReady;                 // 0x06927 a word list is set
    s32 decodeStep;             // 0x06928
    s32 decodeFrame;            // 0x0692C DECODE_FRAME_NONE: not decoding
    s16* originalMeans;         // 0x06930
    s16 adaptOffsets[3][5][COEFFICIENT_NUM]; // 0x06934
    s16 adaptedMeans[512][VECTOR_NUM]; // 0x06A9C the means while a word list is set
    s32 silenceSum[COEFFICIENT_NUM]; // 0x0FA9C
    s32 silenceNum;             // 0x0FACC
    s32 isSilenceDone;          // 0x0FAD0
    struct Adaption
    {
        const s16* matrix;      // [5][12][12]
        const s16* weights;     // [codewords * 5]
    } adaption[3];              // 0x0FAD4
    s32* channelMean;           // 0x0FAEC
    s32 bandFloor[13];          // 0x0FAF0
    s32 bandFloorNum;           // 0x0FB24
    s32 speechSum[COEFFICIENT_NUM]; // 0x0FB28
    s32 defaultStats[COEFFICIENT_NUM]; // 0x0FB58
    s32* savedStats;            // 0x0FB88
    s32 stats[COEFFICIENT_NUM]; // 0x0FB8C
    s32 floorSum;               // 0x0FBBC
    s32 speechNum;              // 0x0FBC0
    const u8* treeCursor;       // 0x0FBC4
    TreeNode treeNodes[1400];   // 0x0FBC8
    TreeNode* trees[65];        // 0x13D68
    TreeNode* treeNodeCursor;   // 0x13E6C
    PhoneSet phoneSets[100];    // 0x13E70
    TreeQuestion questions[700]; // 0x14190
    TreeQuestion* questionCursor; // 0x15770
    s32 leafCount;              // 0x15774
    u32 treeFileSize;           // 0x15778
    u32 treeFileUsed;           // 0x1577C
    u8 westernTables[0x1727C - 0x15780]; // 0x15780 WesternTables + WesternData
    u8 japaneseTables[0x17C44 - 0x1727C]; // 0x1727C
    u8 koreanTables[0x191D8 - 0x17C44]; // 0x17C44
    u8 chineseTables[0x191F0 - 0x191D8]; // 0x191D8
    u8 builder[0x2F438 - 0x191F0]; // 0x191F0 NetworkBuilder
    s16 peakSample;             // 0x2F438
    s32 gain;                   // 0x2F43C
    u16 isAgc;                  // 0x2F440
};
ASSERT_SIZE(Recognizer, 0x2F444);
ASSERT_OFFSET(Recognizer, search, 0x48A4);
ASSERT_OFFSET(Recognizer, params, 0x51E0);
ASSERT_OFFSET(Recognizer, frame, 0x51E8);
ASSERT_OFFSET(Recognizer, samples, 0x5A66);
ASSERT_OFFSET(Recognizer, frameIndex, 0x6914);
ASSERT_OFFSET(Recognizer, silenceSum, 0xFA9C);
ASSERT_OFFSET(Recognizer, speechNum, 0xFBC0);
ASSERT_OFFSET(Recognizer, peakSample, 0x2F438);
ASSERT_OFFSET(Recognizer, treeNodes, 0xFBC8);
ASSERT_OFFSET(Recognizer, trees, 0x13D68);
ASSERT_OFFSET(Recognizer, treeFileSize, 0x15778);

// the file parts given to Initialize
struct File
{
    u32 kind;      // 0x0
    const u8* data; // 0x4
    u32 size;      // 0x8
};
ASSERT_SIZE(File, 0xC);

// the settings of the application
struct Config
{
    s16 maxResults;    // 0x0 1..10
    s16 silenceLimit;  // 0x2 hundredths of a second
    s16 speechLimit;   // 0x4
    s16 endSilence;    // 0x6
    s16 maxLevel;      // 0x8
    s16 isAgc;         // 0xA
};
ASSERT_SIZE(Config, 0xC);

const s32 MAX_WORDS = 100;
const s32 MAX_RESULTS = 10;
const u32 WORK_SIZE = 0x2F76C;

// the work area of the application
struct Work
{
    const char* words[MAX_WORDS]; // 0x00000
    s16 ids[MAX_WORDS];        // 0x00190
    s32 wordNum;               // 0x00258
    Recognizer rec;            // 0x0025C
    VadParams params;          // 0x2F6A0
    VadResult* result;         // 0x2F6C0
    s32 resultNum;             // 0x2F6C4
    s32 scores[MAX_RESULTS];   // 0x2F6C8
    s32 outScores[MAX_RESULTS]; // 0x2F6F0
    s16 resultIds[MAX_RESULTS]; // 0x2F718
    u8 language;               // 0x2F72C
    s32 isStarted;             // 0x2F730
    s32 silenceFrames;         // 0x2F734
    s32 _2F738;                // 0x2F738
    s32 speechFrames;          // 0x2F73C
    s32 speechLimit;           // 0x2F740
    s32 silenceLimit;          // 0x2F744
    s32 speechLimitFrames;     // 0x2F748
    s32 silenceLimitFrames;    // 0x2F74C
    s32 _2F750;                // 0x2F750
    s32 frameFill;             // 0x2F754
    s32 frameLeft;             // 0x2F758
    s32 isFirstFrame;          // 0x2F75C
    s32 maxResults;            // 0x2F760
    s32 endSilenceFrames;      // 0x2F764
    u16 endSilence;            // 0x2F768
};
ASSERT_SIZE(Work, WORK_SIZE);
ASSERT_OFFSET(Work, rec, 0x25C);
ASSERT_OFFSET(Work, params, 0x2F6A0);
ASSERT_OFFSET(Work, language, 0x2F72C);
ASSERT_OFFSET(Work, endSilence, 0x2F768);
} // namespace asr
