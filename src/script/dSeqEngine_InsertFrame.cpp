#include "script/dRenderTag.h"
#include "script/dSeqEngine.h"

namespace script {
// 0x005FDE44
void SeqEngine::InsertFrame::PushClose()
{
    if (isTag3) {
        const char16* text = s_FrameTag4;
        pEngine->PushText(&text, &MsgEngine::s_pNoStop);
    }
    if (color != COLOR_NONE) {
        const char16* text = s_DefaultColorTag;
        pEngine->PushText(&text, &MsgEngine::s_pNoStop);
    }
}

// 0x005FDEA4
void SeqEngine::InsertFrame::PushOpen()
{
    if (color != COLOR_NONE) {
        const char16* text = color < COLOR_NONE ? s_ColorTags[color] : s_DefaultColorTag;
        pEngine->PushText(&text, &MsgEngine::s_pNoStop);
    }
    if (isTag3) {
        const char16* text = s_FrameTag3;
        pEngine->PushText(&text, &MsgEngine::s_pNoStop);
    }
    if (isTag2) {
        const char16* text = s_FrameTag2;
        pEngine->PushText(&text, &MsgEngine::s_pNoStop);
    }
    isTag2 = 0;
    color = COLOR_NONE;
    isTag3 = 0;
}

// 0x005FDF88
void SeqEngine::InsertFrame::InsertWord(const IWord* word, const Tag* tag)
{
    PushClose();
    pEngine->PushWord(word);
    PushOpen();
    s32 group = tag->group;
    s32 index = tag->index;
    if (HasTagAttribute(group, index, 1)) {
        pWords[0] = word;
    }
    if (HasTagAttribute(group, index, 2)) {
        pWords[1] = word;
    }
    if (HasTagAttribute(group, index, 8)) {
        pWords[2] = word;
    }
}

// 0x005FE03C
void SeqEngine::InsertFrame::InsertWordEx(const IWord* word, const Tag* tag)
{
    PushClose();
    pEngine->PushWord(word);
    PushOpen();
    s32 group = tag->group;
    s32 index = tag->index;
    if (HasTagAttribute(group, index, 1)) {
        pWords[0] = word;
    }
    if (HasTagAttribute(group, index, 2)) {
        pWords[1] = word;
    }
    if (HasTagAttribute(group, index, 8)) {
        pWords[2] = word;
    }
    group = tag->group;
    index = tag->index;
    if (HasTagAttribute(group, index, 4)) {
        pWords[3] = word;
    }
    if (HasTagAttribute(group, index, 5)) {
        pWords[4] = word;
    }
    if (HasTagAttribute(group, index, 6)) {
        pWords[5] = word;
    }
}
} // namespace script
