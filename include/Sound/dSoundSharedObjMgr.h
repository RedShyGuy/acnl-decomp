#pragma once

#include "decomp.h"
#include "sead/seadAudioSubsetBase.h"

// RTTI 17SoundSharedObjMgr @ 0x008CC6DC
// vtable 0x008F3DBC (vptr 0x008F3DC4), offset_to_top 0, 20 entries
class SoundSharedObjMgr : public ::sead::AudioSubsetBase
{
public:
    SoundSharedObjMgr(); // ctor candidate(s) 0x00128CB8 (unverified)
    virtual ~SoundSharedObjMgr();
    virtual void initialize(sead::AudioMgr* mgr, sead::Heap* heap) override; // 0x002D3200 slot 0x14
    virtual void finalize() override; // 0x002D3804 slot 0x18
    virtual void calc() override; // 0x002D3570 slot 0x1C
};
