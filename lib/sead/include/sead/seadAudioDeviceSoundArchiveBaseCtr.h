#pragma once

#include "decomp.h"
#include "sead/seadNew.h"
#include "sead/seadAudioSoundArchiveBaseCtr.h"

namespace sead {
class Heap;

// A sound archive of nw on a device (the file system or the ROM); the header and the label strings are
// loaded into buffers of the heap (the members are ours).
// Instantiations found in the binary:
//   sead::AudioDeviceSoundArchiveBaseCtr<nw::snd::FsSoundArchive>  typeinfo 0x008D1F90  vtable 0x009068FC
//   sead::AudioDeviceSoundArchiveBaseCtr<nw::snd::RomSoundArchive>  typeinfo 0x008D1FB0  vtable 0x0090693C
template <typename T>
class AudioDeviceSoundArchiveBaseCtr : public ::sead::AudioSoundArchiveBaseCtr, public T
{
public:
    virtual ~AudioDeviceSoundArchiveBaseCtr();

    Heap* m_pHeap;               // 0x19C of the buffers
    u8* m_pHeaderBuffer;         // 0x1A0
    u8* m_pLabelStringBuffer;    // 0x1A4
    bool m_IsLoadLabelString;    // 0x1A8
};

template <typename T>
AudioDeviceSoundArchiveBaseCtr<T>::~AudioDeviceSoundArchiveBaseCtr()
{
    if (this->IsAvailable()) {
        this->Close();
    }
    if (m_pHeaderBuffer != nullptr) {
        delete[] m_pHeaderBuffer;
        m_pHeaderBuffer = nullptr;
    }
    if (m_pLabelStringBuffer != nullptr) {
        delete[] m_pLabelStringBuffer;
        m_pLabelStringBuffer = nullptr;
    }
}
} // namespace sead
