#include "lyt/dTextAreaOneLineChat.h"

// 0x00507C50 slot 0x00 (falls into ~TextAreaOneLine)
// 0x00507C58 (deleting dtor)
lyt::TextAreaOneLineChat::~TextAreaOneLineChat()
{
    // (only the base is destroyed)
}
