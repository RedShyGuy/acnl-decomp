#include "script/dIUiRecept.h"

namespace script {
// 0x00600EF0 slot 0x98 (name is ours)
bool IUiRecept::ProcessTag98(const Tag* tag)
{
    (void)tag;
    return true;
}

// 0x00600EF8 slot 0x94 (name is ours)
bool IUiRecept::ProcessTag94(const Tag* tag)
{
    (void)tag;
    return true;
}

// 0x00600F1C slot 0x00 (name after the RTTI)
// 0x00600F14 (deleting dtor)
IUiRecept::~IUiRecept()
{
    // empty (in the original too)
}

// 0x0075DD4C slot 0x8C (name is ours)
s32 IUiRecept::GetSex()
{
    return 4;
}

// 0x0075DD54 slot 0x90 (name is ours)
s32 IUiRecept::GetSpeakerSex()
{
    return 4;
}
} // namespace script
