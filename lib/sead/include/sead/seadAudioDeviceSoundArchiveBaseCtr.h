#pragma once

#include "decomp.h"
#include "sead/seadNew.h"
#include "sead/seadAudioSoundArchiveBaseCtr.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"

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
    // whether the class is the one of typeInfo or derives from it (name is ours)
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return AudioSoundArchiveBaseCtr::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours); the static object of
    // FsSoundArchive is at 0x0098358C, the one of RomSoundArchive at 0x00983594 (guards 4 bytes before)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        static const RuntimeTypeInfo::Derive<AudioSoundArchiveBaseCtr> s_TypeInfo;
        return &s_TypeInfo;
    }
    // (inline in the creation functions of AudioSoundDataMgrCtr)
    AudioDeviceSoundArchiveBaseCtr(u8 type, Heap* heap)
        : AudioSoundArchiveBaseCtr(type), T(), m_pHeap(heap), m_pHeaderBuffer(nullptr), m_pLabelStringBuffer(nullptr),
          m_IsLoadLabelString(false)
    {
    }
    virtual ~AudioDeviceSoundArchiveBaseCtr();
    // the header and the label strings into buffers of the heap (inline in open of the derived classes)
    bool openDevice_(const void* arg);
    virtual void close()
    {
        if (this->IsAvailable()) {
            T::Close();
        }
    }

    Heap* m_pHeap;               // 0x19C of the buffers
    u8* m_pHeaderBuffer;         // 0x1A0
    u8* m_pLabelStringBuffer;    // 0x1A4
    bool m_IsLoadLabelString;    // 0x1A8
};

template <typename T>
inline bool AudioDeviceSoundArchiveBaseCtr<T>::openDevice_(const void* arg)
{
    if (!T::Open(static_cast<const char*>(arg))) {
        return false;
    }
    u32 headerSize = this->m_ArchiveFileReader.GetInfoBlockSize();
    m_pHeaderBuffer = new (m_pHeap, 32) u8[headerSize];
    if (m_pHeaderBuffer == nullptr) {
        return false;
    }
    if (!T::LoadHeader(m_pHeaderBuffer, headerSize)) {
        return false;
    }
    if (m_IsLoadLabelString) {
        u32 labelSize = this->m_ArchiveFileReader.GetStringBlockSize();
        m_pLabelStringBuffer = new (m_pHeap, 32) u8[labelSize];
        if (m_pLabelStringBuffer == nullptr) {
            return false;
        }
        if (!T::LoadLabelStringData(m_pLabelStringBuffer, labelSize)) {
            return false;
        }
    }
    return true;
}

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
