#include "Player/dPlayerFaceBank.h"
#include "sead/seadFixedSafeString.h"
#include "sead/seadStringFormatter.h"
#include "Player/player_Extern.h"

// 0x0027140C (name is ours)
PlayerFaceBank::PlayerFaceBank()
{
    // (only the base and the members are constructed)
}

// 0x002714F0 slot 0x00
// 0x00271458 (deleting dtor)
PlayerFaceBank::~PlayerFaceBank()
{
    // (only the base and the members are destroyed)
}

// 0x0027129C (name is ours)
void PlayerFaceBank::Unload()
{
    m_Loader.Unload();
}

// 0x002712A4 (name is ours)
bool PlayerFaceBank::Load(const SvPlayerAppearance* appearance, bool isMale, bool isB)
{
    if (m_Loader.m_SetupResult != g3d::ResourceSet::SETUP_RESULT_NONE && m_Loader.m_GraphicsFile.IsValid()) {
        return true;
    }
    sead::Heap* heap = m_Allocators[0].m_pHeap;
    sead::FixedSafeString<64> path;
    const s32 face = Func713758(appearance);
    const char* sex = isMale ? "boy" : "girl";
    const char* variant = isB ? "b" : "a";
    {
        sead::StringFormatter formatter(&path);
        formatter.append("Player/face_tex/%s%02d%s.bcres").append(sex).appendValue(face).append(variant).flush();
    }
    if (!m_Loader.LoadAsync(path, heap, 128)) {
        return false;
    }
    m_Loader.InitializeCgfx(&m_Allocators[0], NULL, true);
    return true;
}
