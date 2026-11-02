#include "nonameg3d/dChar.h"
#include <cstring>

namespace nonameg3d {
// 0x002FA8C8 slot 0x00 (name is ours)
s32 nonameg3d::Char::Compare(const Char& other) const
{
    return std::strcmp(m_pName, other.m_pName);
}

} // namespace nonameg3d
