#include "Sound/dSoundSeqParser_SingletonDisposer_.h"

// 0x00278D60
// 0x00278D1C (deleting dtor)
SoundSeqParser::SingletonDisposer_::~SingletonDisposer_()
{
    if (this == s_pSingletonDisposer) {
        s_pSingletonDisposer = NULL;
        s_pInstance = NULL;
    }
}
