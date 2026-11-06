#include "Sound/dSoundFurnitureMgr_SingletonDisposer_.h"

// 0x002D20D4
// 0x002D2090 (deleting dtor)
SoundFurnitureMgr::SingletonDisposer_::~SingletonDisposer_()
{
    if (this == s_pSingletonDisposer) {
        s_pSingletonDisposer = NULL;
        s_pInstance = NULL;
    }
}
