#include "cfl/cfl_RecentDB.h"

// removes an entry of the order
// 0x002B7530 | nintendogs:bytes [tier B]
DECOMP_NOINLINE s32 CFLiRecentDBFile::DeleteIndex(int index)
{
    if (index < 0 || num <= index) {
        return CFL_RESULT_INVALID_ARGUMENT;
    }
    num--;
    for (int i = index; i < num; i++) {
        order[i] = order[i + 1];
    }
    order[num] = 0;
    return CFL_RESULT_SUCCESS;
}
