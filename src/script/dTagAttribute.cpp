#include "script/dSeqEngine.h"

namespace script {
namespace {
// the attributes per tag of a group (rows: the tag index, columns: the kinds 0..9; group 7: only the tag 6)
// 0x00891798
extern const bool s_TagAttributes4[21][10] = {
    {false, false, false, false, false, false, false, true, false, false},
    {false, false, false, false, false, false, false, true, false, false},
    {false, false, false, false, false, false, false, true, false, false},
    {false, false, false, false, false, false, false, true, false, false},
    {false, false, false, false, true, false, false, true, false, false},
    {false, false, false, false, false, false, false, true, false, false},
    {false, false, false, false, true, true, true, true, false, false},
    {false, false, false, false, true, true, true, true, false, false},
    {false, false, false, false, true, true, true, true, false, false},
    {false, false, false, false, true, true, true, true, false, false},
    {false, false, false, false, true, true, true, true, false, false},
    {false, false, false, false, true, true, true, true, false, false},
    {false, false, false, false, true, true, true, true, false, false},
    {false, false, false, false, false, false, false, true, false, false},
    {false, false, false, false, false, false, false, true, false, false},
    {false, false, false, false, false, false, false, true, false, false},
    {false, false, false, false, false, false, false, true, false, false},
    {false, false, false, false, true, false, false, true, false, false},
    {false, false, false, false, false, false, false, true, false, false},
    {false, false, false, false, false, false, false, true, false, false},
    {false, false, false, false, false, false, false, true, false, false},
};

// 0x0089186A
extern const bool s_TagAttributes5[38][10] = {
    {false, false, false, false, false, false, false, true, true, false},
    {false, false, false, false, false, false, false, true, false, false},
    {false, false, true, false, false, false, false, true, true, false},
    {false, false, true, false, false, false, false, true, true, false},
    {false, false, true, false, false, false, false, true, true, false},
    {false, false, false, false, false, false, false, true, false, false},
    {false, false, false, false, false, false, false, true, false, false},
    {false, false, false, false, false, false, false, true, false, false},
    {false, false, false, false, false, false, false, true, true, false},
    {true, false, false, false, false, false, false, true, false, false},
    {true, false, false, true, false, false, false, true, false, false},
    {true, false, false, false, false, false, false, true, false, false},
    {true, false, false, false, false, false, false, true, false, false},
    {false, false, false, false, false, false, false, true, false, false},
    {true, false, false, true, false, false, false, true, false, false},
    {true, false, false, true, false, false, false, true, false, false},
    {true, false, false, true, false, false, false, true, false, false},
    {true, false, false, true, false, false, false, true, false, false},
    {true, false, false, true, false, false, false, true, false, false},
    {true, false, false, true, false, false, false, true, false, false},
    {true, false, false, true, false, false, false, true, false, false},
    {true, false, false, true, false, false, false, true, false, false},
    {false, false, false, false, false, false, false, true, false, false},
    {false, false, false, false, false, false, false, true, false, false},
    {false, false, false, false, false, false, false, true, false, false},
    {false, false, false, false, false, false, false, true, false, false},
    {true, false, false, true, false, false, false, true, false, false},
    {false, true, false, false, false, false, false, true, true, false},
    {false, true, false, false, false, false, false, true, true, false},
    {true, false, false, true, false, false, false, true, false, false},
    {true, false, false, false, false, false, false, true, false, false},
    {true, false, false, true, false, false, false, true, false, false},
    {true, false, false, false, false, false, false, true, false, false},
    {true, false, false, true, false, false, false, true, false, false},
    {false, false, true, true, false, false, false, true, true, false},
    {true, false, false, false, false, false, false, true, false, false},
    {false, true, false, false, false, false, false, true, true, false},
    {false, false, false, false, false, false, false, false, false, false},
};

// 0x008919E6
extern const bool s_TagAttributes7[10] = {false, false, false, false, false, false, false, true, false, false};

// 0x008919F0
extern const bool s_TagAttributes17[11][10] = {
    {false, true, false, true, false, false, false, true, true, false},
    {false, false, true, true, false, false, false, true, true, false},
    {false, false, false, true, false, false, false, true, false, false},
    {true, false, false, true, false, false, false, true, false, false},
    {true, false, false, true, false, false, false, true, false, false},
    {false, false, false, false, false, false, false, true, false, false},
    {true, false, false, true, false, false, false, true, false, false},
    {false, false, false, false, false, false, false, true, false, false},
    {false, false, false, false, false, false, false, true, false, false},
    {true, false, false, true, false, false, false, true, false, false},
    {false, true, false, true, false, false, false, true, true, false},
};

// 0x00891A5E
extern const bool s_TagAttributes18[35][10] = {
    {true, false, false, true, false, false, false, true, false, false},
    {true, false, false, false, false, false, false, true, false, false},
    {true, false, false, false, false, false, false, true, false, false},
    {true, false, false, true, false, false, false, true, false, false},
    {true, false, false, true, false, false, false, true, false, false},
    {true, false, false, true, false, false, false, true, false, false},
    {true, false, false, true, false, false, false, true, false, false},
    {false, false, false, false, false, false, false, true, false, false},
    {true, false, false, false, false, false, false, true, false, false},
    {true, false, false, false, false, false, false, true, false, false},
    {true, false, false, true, false, false, false, true, false, false},
    {true, false, false, true, false, false, false, true, false, false},
    {true, false, false, true, false, false, false, true, false, false},
    {true, false, false, false, false, false, false, true, false, false},
    {true, false, false, false, false, false, false, true, false, false},
    {false, false, false, false, false, false, false, true, false, false},
    {false, false, false, false, false, false, false, true, false, false},
    {true, false, false, false, false, false, false, true, false, false},
    {true, false, false, true, false, false, false, true, false, false},
    {true, false, false, true, false, false, false, true, false, false},
    {false, false, false, false, false, false, false, true, false, false},
    {true, false, false, false, false, false, false, true, false, false},
    {true, false, false, true, false, false, false, true, false, false},
    {true, false, false, true, false, false, false, true, false, false},
    {true, false, false, true, false, false, false, true, false, false},
    {false, false, false, false, false, false, false, true, false, false},
    {false, false, false, false, false, false, false, true, false, false},
    {true, false, false, false, false, false, false, true, false, false},
    {false, false, false, false, false, false, false, true, false, false},
    {true, false, false, false, false, false, false, true, false, false},
    {true, false, false, false, false, false, false, true, false, false},
    {true, false, false, false, false, false, false, true, false, false},
    {true, false, false, false, false, false, false, true, false, false},
    {false, false, false, false, false, false, false, true, false, false},
    {false, false, false, false, false, false, false, true, false, false},
};

// 0x00891BBC
extern const bool s_TagAttributes20[5][10] = {
    {true, false, false, true, false, false, false, true, false, false},
    {true, false, false, false, false, false, false, true, false, true},
    {true, false, false, true, false, false, false, true, false, false},
    {true, false, false, false, false, false, false, true, false, false},
    {false, false, false, false, false, false, false, false, false, false},
};
} // namespace

// 0x005D608C (name is ours)
bool HasTagAttribute(s32 group, s32 index, u32 kind)
{
    static const u32 KIND_NUM = 10;

    if (group == 4) {
        if (static_cast<u32>(index) >= 21) {
            return false;
        }
        if (kind >= KIND_NUM) {
            return true;
        }
        return s_TagAttributes4[index][kind];
    } else if (group == 5) {
        if (static_cast<u32>(index) >= 38) {
            return false;
        }
        if (kind >= KIND_NUM) {
            return true;
        }
        return s_TagAttributes5[index][kind];
    } else if (group == 7) {
        if (index != 6) {
            return false;
        }
        if (kind >= KIND_NUM) {
            return true;
        }
        return s_TagAttributes7[kind];
    } else if (group == 17) {
        if (static_cast<u32>(index) >= 11) {
            return false;
        }
        if (kind >= KIND_NUM) {
            return true;
        }
        return s_TagAttributes17[index][kind];
    } else if (group == 18) {
        if (static_cast<u32>(index) >= 35) {
            return false;
        }
        if (kind >= KIND_NUM) {
            return true;
        }
        return s_TagAttributes18[index][kind];
    } else if (group == 20) {
        if (static_cast<u32>(index) >= 5) {
            return false;
        }
        if (kind >= KIND_NUM) {
            return true;
        }
        return s_TagAttributes20[index][kind];
    }
    return false;
}
} // namespace script
