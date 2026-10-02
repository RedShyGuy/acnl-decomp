#pragma once

#include "decomp.h"

namespace sead {
// RTTI N4sead8TreeNodeE @ 0x008D2238
class TreeNode
{
public:
    TreeNode(); // 0x0012CE74 | nintendogs:bytes [tier A]
    void detachSubTree(); // 0x00561590 | nintendogs:bytes [tier A]
    void pushBackChild(sead::TreeNode*); // 0x005615F4 | nintendogs:bytes [tier A]
    void pushFrontChild(sead::TreeNode*); // 0x00561654 | nintendogs:bytes [tier A]
    void clearChildLinksRecursively_(); // 0x00561694 | nintendogs:bytes [tier A]
    void detachAll(); // 0x005616D4 | nintendogs:bytes [tier A]
};
} // namespace sead
