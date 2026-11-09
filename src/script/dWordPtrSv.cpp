#include "script/dWordPtrSv.h"

namespace script {
// 0x003082F4 slot 0x00 | slot vf_0x00 of script::WordPtr
// 0x00605378 (deleting dtor)
WordPtrSv::~WordPtrSv()
{
    // empty (in the original too)
}

// 0x0075E140 slot 0x2C
bool WordPtrSv::IsSv() const
{
    return true;
}
} // namespace script
