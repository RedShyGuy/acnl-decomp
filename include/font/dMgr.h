#pragma once

#include "decomp.h"

namespace font {
class Mgr
{
public:
    class SingletonDisposer_;
    void GetFont(font::FontID) const; // 0x0052ED74 | libgarden [tier A]
    void GetFontName(font::FontID) const; // 0x00748D88 | libgarden [tier A]
};
} // namespace font
