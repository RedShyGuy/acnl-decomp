#include "Audio/dAudioObjSymbolTree.h"
#include "Sound/sound_Extern.h"

// 0x002D60A8
AudioObjSymbolTree::AudioObjSymbolTree() : m_IsStarted(false)
{
}

// 0x002D6004
void AudioObjSymbolTree::Start(const nn::math::VEC3* world)
{
    nn::math::VEC3 relative;
    Func1E5E50(&relative, world);
    m_Obj.Start(world, &relative);
    m_IsStarted = true;
}

// 0x002D603C
s8 AudioObjSymbolTree::IsNear()
{
    return m_Obj.m_IsNear;
}

// 0x002D6044
void AudioObjSymbolTree::SetIsIgnoreMaster(bool isIgnoreMaster)
{
    m_Obj.SetIsIgnoreMaster(isIgnoreMaster);
}

// 0x002D6048
void AudioObjSymbolTree::SetPosition(const nn::math::VEC3* world)
{
    nn::math::VEC3 relative;
    Func1E5E50(&relative, world);
    m_Obj.SetPosition(world, &relative);
}

// 0x002D6078
void AudioObjSymbolTree::Stop()
{
    if (m_IsStarted) {
        m_Obj.Stop();
        m_IsStarted = false;
    }
}
