#include "Sound/dSoundHistoryMgr_SingletonDisposer_.h"

// 0x002A3910
// 0x002A38CC (deleting dtor)
SoundHistoryMgr::SingletonDisposer_::~SingletonDisposer_()
{
    if (this == s_pSingletonDisposer) {
        s_pSingletonDisposer = NULL;
        s_pInstance = NULL;
    }
}
