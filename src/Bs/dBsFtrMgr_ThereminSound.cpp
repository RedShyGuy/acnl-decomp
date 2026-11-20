#include "Bs/dBsFtrMgr.h"
#include "Sound/sound_Extern.h"

// 0x002AA6D0
BsFtrMgr::ThereminSound::ThereminSound() : m_IsStarted(false)
{
}

// 0x002AA658
void BsFtrMgr::ThereminSound::Start()
{
    SoundObjTheremin::Start();
    m_IsStarted = true;
}

// 0x002AA670
void BsFtrMgr::ThereminSound::Update(const nn::math::VEC3* position, f32 distance)
{
    // (the position relative to the camera)
    nn::math::VEC3 relative;
    Func1E5E50(&relative, position);
    SoundObjTheremin::Update(&relative, distance);
}

// 0x002AA6A8
void BsFtrMgr::ThereminSound::Stop()
{
    if (m_IsStarted) {
        SoundObjTheremin::Stop();
        m_IsStarted = false;
    }
}
