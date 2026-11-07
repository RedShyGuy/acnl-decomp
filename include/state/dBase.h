#pragma once

#include "decomp.h"

namespace state {
// the base of the state machines of the game (class, member and slot names after libgarden)
template <typename T>
class Base
{
public:
    typedef void (T::*Func)();

    Base() : m_pName(NULL), m_Enabled(false) {}
    virtual ~Base() {}
    // the name of a state function (debug)
    virtual const char* GetName(Func func) const = 0;

    const char* m_pName;  // 0x4
    bool m_Enabled;       // 0x8
};
} // namespace state
