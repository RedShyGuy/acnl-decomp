#pragma once

#include "decomp.h"

namespace sead {
class ListImpl
{
public:
    void popFront(); // 0x0012CD84 | nintendogs:bytes [tier A]
    void clear(); // 0x0053D8DC | nintendogs:callseq-callee [tier A]
    void indexOf(const sead::ListNode*) const; // 0x0074F944 | nintendogs:bytes [tier A]
};
} // namespace sead
