#include "libms/lms_Api.h"
#include "lms_Internal.h"

extern "C" {

// 0x00159FC0 (name is ours)
LMS_MessageBinary* LMS_InitMessage(const void* data)
{
    LMS_MessageBinary* message = static_cast<LMS_MessageBinary*>(LMSi_Allocate(sizeof(LMS_MessageBinary)));
    message->common.data = static_cast<const u8*>(data);
    LMSi_AnalyzeMessageHeader(&message->common, "MsgStdBn");
    message->lbl1 = LMSi_SearchBlockByName(&message->common, "LBL1");
    message->txt2 = LMSi_SearchBlockByName(&message->common, "TXT2");
    message->atr1 = LMSi_SearchBlockByName(&message->common, "ATR1");
    message->ato1 = LMSi_SearchBlockByName(&message->common, "ATO1");
    message->tsy1 = LMSi_SearchBlockByName(&message->common, "TSY1");
    return message;
}

// 0x0013A574 (name is ours)
void LMS_CloseMessage(LMS_MessageBinary* message)
{
    if (message->common.blocks != NULL)
    {
        LMSi_Free(message->common.blocks);
    }
    LMSi_Free(message);
}

// 0x00159F1C (name is ours)
s32 LMS_GetTextNum(LMS_MessageBinary* message)
{
    if (message->txt2 == -1)
    {
        return -1;
    }
    return *reinterpret_cast<const s32*>(message->common.blocks[message->txt2].data);
}

// TXT2: u32 textNum, u32 offset[textNum] (from the start of the block)
// 0x00159D9C | mk7dlp:bytes [tier A]
const void* LMS_GetText(LMS_MessageBinary* message, s32 index)
{
    if (message->txt2 == -1)
    {
        return NULL;
    }
    const u8* data = message->common.blocks[message->txt2].data;
    if (*reinterpret_cast<const s32*>(data) > index)
    {
        return data + reinterpret_cast<const u32*>(data + 4)[index];
    }
    return NULL;
}

// 0x00159E3C (name is ours)
DECOMP_NOINLINE s32 LMS_GetTextIndexByLabel(LMS_MessageBinary* message, const char* label)
{
    if (message->lbl1 == -1)
    {
        return LMS_ERROR_NO_LABELS;
    }
    s32 length = LMSi_GetLength(label);
    return LMSi_FindLabel(&message->common.blocks[message->lbl1], label, length);
}

// 0x00159DE0 (name is ours)
const void* LMS_GetTextByLabel(LMS_MessageBinary* message, const char* label)
{
    s32 index = LMS_GetTextIndexByLabel(message, label);
    if (index < 0)
    {
        return NULL;
    }
    return LMS_GetText(message, index);
}

// the label of a text (copied with a terminator)
// 0x00159CE8 (name is ours)
bool LMS_GetLabelByIndex(LMS_MessageBinary* message, s32 index, char* label)
{
    const LMS_BinaryBlock* block = &message->common.blocks[message->lbl1];
    const u8* data = block->data;
    // the labels follow the bucket table
    u32 offset = 4 + *reinterpret_cast<const u32*>(data) * 8;
    while (block->size > offset)
    {
        u8 size = data[offset];
        if (*reinterpret_cast<const s32*>(data + offset + size + 1) == index)
        {
            LMSi_MemCopy(label, reinterpret_cast<const char*>(data + offset + 1), size);
            label[size] = '\0';
            return true;
        }
        offset += size + 5;
    }
    return false;
}

// ATR1: u32 attributeNum, u32 attributeSize, the attributes
// 0x00159A84 (name is ours)
const void* LMS_GetAttribute(LMS_MessageBinary* message, s32 index)
{
    const u8* data = message->common.blocks[message->atr1].data;
    return data + *reinterpret_cast<const u32*>(data + 4) * index + 8;
}

} // extern "C"
