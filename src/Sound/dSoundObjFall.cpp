#include "Sound/dSoundObjFall.h"
#include "Sound/dSoundLabel.h"

// 0x0020B610
// 0x0020B5BC (deleting dtor)
SoundObjFall::~SoundObjFall()
{
    // (only the base is destroyed)
}

// 0x0020B574
void SoundObjFall::Start(const nn::math::VEC3* world, const nn::math::VEC3* relative)
{
    m_Type = 5;
    m_IsIgnoreMaster = 1;
    m_IsUseHeight = 0;
    SoundObjHold::Start(world, relative, SE_ENV_WATERFALL);
}
