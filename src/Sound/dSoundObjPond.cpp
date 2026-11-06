#include "Sound/dSoundObjPond.h"
#include "Sound/dSoundLabel.h"

// 0x0020BA00
// 0x0020B9AC (deleting dtor)
SoundObjPond::~SoundObjPond()
{
    // (only the base is destroyed)
}

// 0x0020B968
void SoundObjPond::Start(const nn::math::VEC3* world, const nn::math::VEC3* relative)
{
    m_Type = 6;
    m_IsIgnoreMaster = 1;
    SoundObjHold::Start(world, relative, SE_ENV_POND);
}
