#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_SoundArchiveFile.h"

class nw::snd::internal::SoundArchiveFile::InfoBlockBody
{
public:
    void GetItemPrefetchFileId(unsigned int) const; // 0x0013FCB0 | fefates:bytes [tier B]
    void GetBankInfo(unsigned) const; // 0x00141D58 | nintendogs:bytes [tier A]
    void GetSoundInfo(unsigned) const; // 0x00141D94 | nintendogs:bytes [tier A]
    void GetSoundGroupInfo(unsigned) const; // 0x00141DD0 | nintendogs:bytes [tier A]
    void GetWaveArchiveInfo(unsigned) const; // 0x00141E0C | nintendogs:bytes [tier A]
    void GetFileInfo(unsigned) const; // 0x00143AB8 | nintendogs:bytes [tier A]
    void GetSoundGroupInfoReferenceTable() const; // 0x00143AE4 | nintendogs:callgraph [tier A]
    void GetGroupInfo(unsigned) const; // 0x0073FE84 | nintendogs:bytes [tier A]
    void GetPlayerInfo(unsigned) const; // 0x0073FEC0 | nintendogs:bytes [tier A]
    void GetItemStringId(unsigned) const; // 0x0073FEFC | nintendogs:callgraph [tier A]
};
