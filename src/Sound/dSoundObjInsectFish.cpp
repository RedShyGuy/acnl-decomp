#include "Sound/dSoundObjInsectFish.h"
#include "Sound/dSoundLabel.h"
#include "nw/snd/internal/snd_SequenceSound.h"
#include "nw/snd/snd_SequenceSoundHandle.h"

namespace {
const s32 VARIABLE_MOVING = 13;
} // namespace

// 0x002E92B4
SoundObjInsectFish::SoundObjInsectFish() : m_Kind(0), _60(1.0f), m_IsMoving(false)
{
    m_Priority = 6;
}

// 0x002E9378
// 0x002E9324 (deleting dtor)
SoundObjInsectFish::~SoundObjInsectFish()
{
    // (only the base is destroyed)
}

// 0x002E8AC4
u32 SoundObjInsectFish::ConvertSound(u32 soundId) const
{
    switch (m_Kind) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 30:
    case 48:
        if (soundId == SE_INSECT_ESCAPE) {
            return SE_INSECT_ESCAPE_BUTTERFLY;
        }
        break;
    case 10:
        if (soundId == SE_INSECT_ESCAPE) {
            return SE_INSECT_ESCAPE_WING_L;
        }
        break;
    case 11:
        if (soundId == SE_INSECT_MOVE_LV) {
            return SE_INSECT_HONEYBEE;
        } else if (soundId == SE_INSECT_ESCAPE) {
            return SE_INSECT_ESCAPE_HONEYBEE;
        }
        break;
    case 12:
        if (soundId == SE_INSECT_MOVE_LV) {
            return SE_INSECT_HORNET;
        } else if (soundId == SE_INSECT_ESCAPE) {
            return SE_INSECT_ESCAPE_FLY_S;
        }
        break;
    case 13:
        if (soundId == SE_INSECT_MOVE_LV) {
            return SE_INSECT_SHORYOBATTA;
        } else if (soundId == SE_INSECT_ESCAPE) {
            return SE_INSECT_ESCAPE_BATTA;
        }
        break;
    case 14:
        if (soundId == SE_INSECT_MOVE_LV) {
            return SE_INSECT_TONOSAMABATTA;
        } else if (soundId == SE_INSECT_ESCAPE) {
            return SE_INSECT_ESCAPE_BATTA;
        }
        break;
    case 15:
        if (soundId == SE_INSECT_VOICE) {
            return SE_INSECT_INAGO;
        } else if (soundId == SE_INSECT_MOVE_LV) {
            return SE_INSECT_BATTA;
        } else if (soundId == SE_INSECT_ESCAPE) {
            return SE_INSECT_ESCAPE_BATTA;
        }
        break;
    case 16:
    case 17:
        if (soundId == SE_INSECT_MOVE_LV) {
            return SE_INSECT_BATTA;
        } else if (soundId == SE_INSECT_ESCAPE) {
            return SE_INSECT_ESCAPE_FLY_SS;
        }
        break;
    case 18:
        if (soundId == SE_INSECT_VOICE) {
            return SE_INSECT_ABURAZEMI;
        } else if (soundId == SE_INSECT_ESCAPE) {
            return SE_INSECT_ESCAPE_FLY_SEMI;
        }
        break;
    case 19:
        if (soundId == SE_INSECT_VOICE) {
            return SE_INSECT_MINMINZEMI;
        } else if (soundId == SE_INSECT_ESCAPE) {
            return SE_INSECT_ESCAPE_FLY_SEMI;
        }
        break;
    case 20:
        if (soundId == SE_INSECT_VOICE) {
            return SE_INSECT_KUMAZEMI;
        } else if (soundId == SE_INSECT_ESCAPE) {
            return SE_INSECT_ESCAPE_FLY_SEMI;
        }
        break;
    case 21:
        if (soundId == SE_INSECT_VOICE) {
            return SE_INSECT_TSUKUTSUKUHOSHI;
        } else if (soundId == SE_INSECT_ESCAPE) {
            return SE_INSECT_ESCAPE_FLY_SEMI;
        }
        break;
    case 22:
        if (soundId == SE_INSECT_VOICE) {
            return SE_INSECT_HIGURASHI;
        } else if (soundId == SE_INSECT_ESCAPE) {
            return SE_INSECT_ESCAPE_FLY_SEMI;
        }
        break;
    case 24:
        if (soundId == SE_INSECT_ESCAPE) {
            return SE_INSECT_ESCAPE_WING_S;
        }
        break;
    case 25:
    case 26:
    case 27:
    case 28:
        if (soundId == SE_INSECT_ESCAPE) {
            return SE_INSECT_ESCAPE_TOMBO;
        }
        break;
    case 29:
    case 63:
        if (soundId == SE_INSECT_ESCAPE) {
            return SE_INSECT_ESCAPE_FALL_S;
        }
        break;
    case 31:
    case 52:
    case 53:
    case 54:
    case 55:
    case 56:
    case 57:
    case 58:
        if (soundId == SE_INSECT_ESCAPE) {
            return SE_INSECT_ESCAPE_FLY_M;
        }
        break;
    case 32:
    case 42:
    case 43:
    case 45:
    case 49:
    case 50:
    case 51:
        if (soundId == SE_INSECT_ESCAPE) {
            return SE_INSECT_ESCAPE_FLY_S;
        }
        break;
    case 33:
    case 39:
    case 40:
    case 69:
        if (soundId == SE_INSECT_ESCAPE) {
            return SE_INSECT_ESCAPE_FALL_M;
        }
        break;
    case 34:
        if (soundId == SE_INSECT_VOICE) {
            return SE_INSECT_KOROGI;
        } else if (soundId == SE_INSECT_MOVE_LV) {
            return SE_INSECT_BATTA;
        } else if (soundId == SE_INSECT_ESCAPE) {
            return SE_INSECT_ESCAPE_BATTA;
        }
        break;
    case 35:
        if (soundId == SE_INSECT_VOICE) {
            return SE_INSECT_SUZUMUSHI;
        } else if (soundId == SE_INSECT_MOVE_LV) {
            return SE_INSECT_BATTA;
        } else if (soundId == SE_INSECT_ESCAPE) {
            return SE_INSECT_ESCAPE_BATTA;
        }
        break;
    case 36:
        if (soundId == SE_INSECT_VOICE) {
            return SE_INSECT_KIRIGIRISU;
        } else if (soundId == SE_INSECT_MOVE_LV) {
            return SE_INSECT_BATTA;
        } else if (soundId == SE_INSECT_ESCAPE) {
            return SE_INSECT_ESCAPE_BATTA;
        }
        break;
    case 37:
    case 64:
        if (soundId == SE_INSECT_ESCAPE) {
            return SE_INSECT_ESCAPE_RUN;
        }
        break;
    case 38:
        if (soundId == SE_INSECT_MOVE_TR) {
            return SE_INSECT_YADOKARI_CHANGE;
        } else if (soundId == SE_INSECT_ESCAPE) {
            return SE_INSECT_ESCAPE_RUN;
        }
        break;
    case 41:
        if (soundId == SE_INSECT_ESCAPE) {
            return SE_INSECT_ESCAPE_FLY_SS;
        }
        break;
    case 44:
        if (soundId == SE_INSECT_ESCAPE) {
            return SE_INSECT_ESCAPE_BATTA;
        }
        break;
    case 46:
        if (soundId == SE_INSECT_MOVE_LV) {
            return SE_INSECT_FUNAMUSHI_MOVE;
        } else if (soundId == SE_INSECT_ESCAPE) {
            return SE_INSECT_FUNAMUSHI_ESCAPE;
        }
        break;
    case 47:
        if (soundId == SE_INSECT_MOVE_TR) {
            return SE_INSECT_YADOKARI_CHANGE;
        } else if (soundId == SE_INSECT_MOVE_LV) {
            return SE_INSECT_YADOKARI_MOVE;
        } else if (soundId == SE_INSECT_ESCAPE) {
            return SE_INSECT_ESCAPE_RUN;
        }
        break;
    case 59:
    case 60:
    case 61:
    case 62:
        if (soundId == SE_INSECT_ESCAPE) {
            return SE_INSECT_ESCAPE_FLY_L;
        }
        break;
    case 65:
        if (soundId == SE_INSECT_MOVE_LV) {
            return SE_INSECT_MOSQUITO;
        } else if (soundId == SE_INSECT_ESCAPE) {
            return SE_INSECT_ESCAPE_MOSQUITO;
        }
        break;
    case 66:
        if (soundId == SE_INSECT_MOVE_LV) {
            return SE_INSECT_FLY;
        } else if (soundId == SE_INSECT_ESCAPE) {
            return SE_INSECT_ESCAPE_FLY;
        }
        break;
    case 67:
    case 68:
        if (soundId == SE_INSECT_MOVE_LV) {
            return SE_INSECT_MUKADE;
        } else if (soundId == SE_INSECT_ESCAPE) {
            return SE_INSECT_ESCAPE_RUN;
        }
        break;
    case 70:
        if (soundId == SE_INSECT_MOVE_LV) {
            return SE_INSECT_TARANTULA;
        } else if (soundId == SE_INSECT_MOVE_TR) {
            return SE_INSECT_TARANTULA_OFFENSIVE;
        } else if (soundId == SE_INSECT_ESCAPE) {
            return SE_INSECT_ESCAPE_RUN;
        }
        break;
    case 71:
        if (soundId == SE_INSECT_MOVE_LV) {
            return SE_INSECT_SCORPION_RUN;
        } else if (soundId == SE_INSECT_MOVE_TR) {
            return SE_INSECT_SCORPION_OFFENSIVE;
        } else if (soundId == SE_INSECT_ESCAPE) {
            return SE_INSECT_ESCAPE_RUN;
        }
        break;
    case 72:
        if (soundId == SE_INSECT_MOVE_LV) {
            return SE_OBJ_SNOW_CRYSTAL;
        }
        break;
    }
    return soundId;
}

// 0x002E91A4
sead::SoundHandle* SoundObjInsectFish::HoldSound(u32 soundId)
{
    return SoundObj<1>::HoldSound(ConvertSound(soundId));
}

// 0x002E91C0
void SoundObjInsectFish::SetPosition(const nn::math::VEC3* world, const nn::math::VEC3* relative)
{
    SoundObj<1>::SetPosition(world, relative);
    m_IsMoving = false;
    if (m_HandleSet.m_Handles[0].m_pSound != NULL) {
        nw::snd::SequenceSoundHandle sequence(&m_HandleSet.m_Handles[0]);
        if (sequence.m_pSound != NULL) {
            s16 value = 0;
            sequence.m_pSound->ReadVariable(VARIABLE_MOVING, &value);
            m_IsMoving = static_cast<u16>(value) == 1;
        }
    }
}

// 0x002E9298
sead::SoundHandle* SoundObjInsectFish::StartSound(u32 soundId)
{
    return SoundObj<1>::StartSound(ConvertSound(soundId));
}
