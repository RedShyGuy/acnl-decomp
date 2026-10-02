#pragma once

#include "decomp.h"

namespace sead {
// RTTI N4sead8ListNodeE @ 0x008D2200
class ListNode
{
public:
    ListNode(); // ctor address unknown
    void insertFront_(sead::ListNode*); // 0x001393A4 | nintendogs:callseq-callee [tier A]
    void erase_(); // 0x0013C908 | nintendogs:callseq-callee [tier A]
    void insertBack_(sead::ListNode*); // 0x00561170 | nintendogs:bytes [tier B]
};
} // namespace sead
