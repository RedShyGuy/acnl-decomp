#pragma once

// The C library of LibMessageStudio: reads the binaries "MsgStdBn" (texts), "MsgFlwBn" (flow charts) and
// "MsgPrjBn" (project settings). The prefix LMS_ is confirmed by LMS_GetText (reference symbols of mk7dlp and
// nintendogs); all other names are ours, internal helpers use LMSi_ like cfl.

#include "decomp.h"

extern "C" {

typedef void* (*LMS_AllocFunc)(u32 size);
typedef void (*LMS_FreeFunc)(void* ptr);

// a block of a binary (the 16 byte block header precedes the data)
typedef struct LMS_BinaryBlock
{
    const u8* data;  // +0x00
    char tag[4];     // +0x04 e.g. "TXT2"
    u32 size;        // +0x08
    u16 _0C;         // +0x0C the first two bytes of the padding of the block header
} LMS_BinaryBlock;
ASSERT_SIZE(LMS_BinaryBlock, 0x10);

// what all three binaries share
typedef struct LMS_Binary
{
    const u8* data;           // +0x00 the file
    u32 fileSize;             // +0x04
    u8 encoding;              // +0x08 of the texts
    u16 blockNum;             // +0x0A
    LMS_BinaryBlock* blocks;  // +0x0C
} LMS_Binary;
ASSERT_SIZE(LMS_Binary, 0x10);

// the blocks are indices into LMS_Binary::blocks, -1 if the file has none
typedef struct LMS_MessageBinary
{
    LMS_Binary common;  // +0x00
    s32 lbl1;           // +0x10 labels of the texts (hash table)
    s32 txt2;           // +0x14 texts
    s32 atr1;           // +0x18 attributes of the texts
    s32 ato1;           // +0x1C
    s32 tsy1;           // +0x20
} LMS_MessageBinary;
ASSERT_SIZE(LMS_MessageBinary, 0x24);

typedef struct LMS_FlowBinary
{
    LMS_Binary common;  // +0x00
    s32 flw3;           // +0x10 nodes and branch tables
    s32 fen1;           // +0x14 labels of the entry points (hash table)
    s32 ref1;           // +0x18
} LMS_FlowBinary;
ASSERT_SIZE(LMS_FlowBinary, 0x1C);

typedef struct LMS_ProjectBinary
{
    LMS_Binary common;  // +0x00
    s32 clb1;           // +0x10 color labels
    s32 clr1;           // +0x14 colors (RGBA)
    s32 alb1;           // +0x18 attribute labels
    s32 ati2;           // +0x1C attribute infos
    s32 ali2;           // +0x20
    s32 tgg2;           // +0x24 tag groups
    s32 tag2;           // +0x28 tags
    s32 tgp2;           // +0x2C tag parameters
    s32 tgl2;           // +0x30
    s32 syl3;           // +0x34 styles
    s32 slb1;           // +0x38 style labels
    s32 cti1;           // +0x3C names (string table)
} LMS_ProjectBinary;
ASSERT_SIZE(LMS_ProjectBinary, 0x40);

// a node of FLW3 (16 bytes, after a 16 byte header with the number of nodes)
typedef struct LMS_FlowNode
{
    u8 type;          // +0x00 2 = branch
    u8 _01[13];       // +0x01
    u16 branchIndex;  // +0x0E first case in the branch table (branch nodes)
} LMS_FlowNode;
ASSERT_SIZE(LMS_FlowNode, 0x10);

enum
{
    LMS_FLOW_NODE_BRANCH = 2,
};

// errors (negative results)
enum
{
    LMS_ERROR_NOT_FOUND = -1,
    LMS_ERROR_NO_LABELS = -2,
    LMS_ERROR_NO_FLOW_NODES = -4,
    LMS_ERROR_NO_COLORS = -5,
    LMS_ERROR_NO_ENTRY_LABELS = -10,
};

// memory
void LMS_SetMemFuncs(LMS_AllocFunc alloc, LMS_FreeFunc free);
void* LMSi_Allocate(u32 size);
void LMSi_Free(void* ptr);

// helpers
void LMSi_AnalyzeMessageHeader(LMS_Binary* binary, const char* magic);
LMS_BinaryBlock* LMSi_GetBlock(LMS_Binary* binary, const char* tag);
LMS_BinaryBlock* LMS_GetBlock(LMS_Binary* binary, const char* tag);
s32 LMSi_SearchBlockByName(LMS_Binary* binary, const char* tag);
u32 LMSi_GetHashTableIndex(const char* label, u32 bucketNum);
s32 LMSi_MemCmp(const char* a, const char* b, s32 size);
void LMSi_MemCopy(char* dst, const char* src, s32 size);

// texts
LMS_MessageBinary* LMS_InitMessage(const void* data);
void LMS_CloseMessage(LMS_MessageBinary* message);
s32 LMS_GetTextNum(LMS_MessageBinary* message);
const void* LMS_GetText(LMS_MessageBinary* message, s32 index);
const void* LMS_GetTextByLabel(LMS_MessageBinary* message, const char* label);
s32 LMS_GetTextIndexByLabel(LMS_MessageBinary* message, const char* label);
bool LMS_GetLabelByIndex(LMS_MessageBinary* message, s32 index, char* label);
const void* LMS_GetAttribute(LMS_MessageBinary* message, s32 index);

// flow charts
LMS_FlowBinary* LMS_InitFlow(const void* data);
void LMS_CloseFlow(LMS_FlowBinary* flow);
const LMS_FlowNode* LMS_GetFlowNode(LMS_FlowBinary* flow, s32 index);
const u16* LMS_GetBranchCases(LMS_FlowBinary* flow, s32 index);
s32 LMS_GetFlowEntryIndex(LMS_FlowBinary* flow, const char* label);
const void* LMS_GetFlowData(LMS_FlowBinary* flow, s32 offset);

// project
LMS_ProjectBinary* LMS_InitProject(const void* data);
void LMS_CloseProject(LMS_ProjectBinary* project);
s32 LMS_GetColor(LMS_ProjectBinary* project, s32 index, u8* rgba);
s32 LMS_GetNameNum(LMS_ProjectBinary* project);
const char* LMS_GetName(LMS_ProjectBinary* project, s32 index);

} // extern "C"
