#include "decomp.h"

// the constants of PlayerHeadBank (own file, otherwise GCC folds them into the code)

// the scale of the Mii
// 0x00887D30
extern const f32 s_MiiScale = 0.375f;
// the CFL expression by the digit at index 2 of the name of the face texture
// 0x00887D38
extern const s32 s_MiiExpressions[8] = {0, 5, 5, 2, 3, 1, 4, 3};
