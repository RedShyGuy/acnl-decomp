#include "Sound/dSoundObjNpc.h"
#include "Sound/dSoundMgr.h"
#include "event/event_Extern.h"

namespace {
// an entry of the table of the characters (34 bytes; only the kind is known)
inline const u8* GetNpcEntry(u16 npcId)
{
    const u8* table = event::g_UnknownA84AEC.table;
    if (table == NULL || npcId >= SoundObjNpc::NPC_NUM) {
        return NULL;
    }
    return table + npcId * 34;
}
} // namespace

// 0x001DA5D8
SoundObjNpc::SoundObjNpc() : m_IsFrozen(false), m_FrozenPos(2000.0f, 2000.0f, 2000.0f), m_NpcId(0xFFFF)
{
    m_Priority = 1;
}

// 0x001DA718
// 0x001DA668 (deleting dtor)
SoundObjNpc::~SoundObjNpc()
{
    // (only the members and the base are destroyed)
}

// 0x001DA258
void SoundObjNpc::Start(const nn::math::VEC3* world, const nn::math::VEC3* relative)
{
    if (!m_IsActive) {
        m_Footstep.Init(this);
        SoundObjBase::Start(world, relative);
        m_Hum.m_pOwner = this;
    }
}

// 0x001DA29C
void SoundObjNpc::SetFootstepSize(u32 size)
{
    if (size <= SoundFootstep::SIZE_LARGE) {
        m_Footstep.m_Size = size;
    }
}

// 0x001DA2AC
void SoundObjNpc::Jump(u32 walkSound, u32 groundValue)
{
    UpdateChanged();
    m_Footstep.Jump(walkSound, groundValue);
}

// 0x001DA2E4
void SoundObjNpc::Land(u32 walkSound, u32 groundValue)
{
    UpdateChanged();
    m_Footstep.Land(walkSound, groundValue);
}

// 0x002A67B4
void SoundObjNpc::Walk(f32 speed, u32 walkSound, u32 groundValue)
{
    UpdateChanged();
    m_Footstep.Walk(speed, walkSound, groundValue);
}

// 0x001DA31C
void SoundObjNpc::SetNpc(u16 npcId)
{
    const u8* entry = GetNpcEntry(npcId);
    if (entry == NULL) {
        return;
    }
    u32 size = SoundFootstep::SIZE_MEDIUM;
    switch (static_cast<s8>(entry[2])) {
        case 5:
        case 7:
        case 10:
        case 16:
        case 25:
        case 33:
            size = SoundFootstep::SIZE_SMALL;
            break;
        case 1:
        case 3:
        case 8:
        case 13:
        case 22:
        case 26:
        case 27:
            size = SoundFootstep::SIZE_LARGE;
            break;
    }
    m_Footstep.m_Size = size;
    m_NpcId = npcId;
}

// 0x001DA3E8
void SoundObjNpc::RequestBgmFadeIn()
{
    SoundMgr::s_pInstance->m_BgmMgr.m_IsFadeInRequested = true;
}

// 0x001DA400
void SoundObjNpc::SetPosition(const nn::math::VEC3* world, const nn::math::VEC3* relative)
{
    if (m_IsFaded) {
        SoundObj<2>::SetPosition(world, relative);
    } else {
        SoundMgr* mgr = SoundMgr::s_pInstance;
        u8 state = mgr->m_FadeState;
        if (state == 0) {
            SoundObj<2>::SetPosition(world, relative);
            if (m_IsFrozen) {
                m_IsFrozen = false;
            }
        } else if (state == 1 || state == 2) {
            if (m_IsFrozen) {
                SoundObj<2>::SetPosition(world, &m_FrozenPos);
            } else {
                m_FrozenPos = *relative;
                m_IsFrozen = true;
                SoundObj<2>::SetPosition(world, relative);
            }
        } else if (state == 3) {
            s32 half = mgr->m_FadeFrames / 2;
            if (mgr->m_FadeTimer < half) {
                nn::math::VEC3 position;
                position.x = SoundMgr::Lerp(half, mgr->m_FadeFrames, 0, m_FrozenPos.x, relative->x);
                position.y = SoundMgr::Lerp(half, SoundMgr::s_pInstance->m_FadeFrames, 0, m_FrozenPos.y, relative->y);
                position.z = SoundMgr::Lerp(half, SoundMgr::s_pInstance->m_FadeFrames, 0, m_FrozenPos.z, relative->z);
                SoundObj<2>::SetPosition(world, &position);
            } else {
                SoundObj<2>::SetPosition(world, &m_FrozenPos);
            }
        }
    }
    m_Hum.Update();
}

// 0x001DA56C
void SoundObjNpc::Stop()
{
    if (m_IsActive) {
        m_Footstep.Stop();
        m_Hum.Detach();
        SoundObj<2>::Stop();
    }
}
