#pragma once

#include "decomp.h"
#include "Bank/dBank.h"
#include "g3d/dResourceLoader.h"

struct SvPlayerAppearance;

// A bank for the face textures of the player (class name from the RTTI; the member name is ours): one slot with a file.
// RTTI 14PlayerFaceBank @ 0x008CBD4C
// vtable 0x008F061C (vptr 0x008F0624), offset_to_top 0, 3 entries
class PlayerFaceBank : public ::Bank<40543u, 1u, true, BankVramConfig<0u, 0u, 0u, 0u, false> >
{
public:
    PlayerFaceBank(); // 0x0027140C
    virtual ~PlayerFaceBank(); // 0x002714F0 slot 0x00

    // the loader is reset
    void Unload(); // 0x0027129C
    // the face texture file of appearance ("Player/face_tex/<boy|girl><face><a|b>.bcres") loads (true: loaded already
    // or the loading started)
    bool Load(const SvPlayerAppearance* appearance, bool isMale, bool isB); // 0x002712A4

    g3d::ResourceLoader m_Loader; // 0x14
};
