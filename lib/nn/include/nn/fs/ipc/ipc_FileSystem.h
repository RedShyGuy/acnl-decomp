#pragma once

#include "decomp.h"

namespace nn {
namespace fs {
namespace ipc {
class FileSystem
{
public:
    void OpenArchive(unsigned long long*, unsigned, unsigned, const unsigned char*, unsigned); // 0x0013044C | nintendogs:bytes [tier A]
    void CloseArchive(unsigned long long); // 0x001304AC | nintendogs:callgraph [tier A]
    void OpenFileDirectly(nn::Handle*, nn::fs::Transaction, unsigned, unsigned, const unsigned char*, unsigned, unsigned, const unsigned char*, unsigned, unsigned, nn::fs::Attributes); // 0x001304DC | nintendogs:bytes [tier A]
    void SetPriority(int); // 0x00136214 | nintendogs:bytes [tier A]
    void GetPriority(int*); // 0x0013AEE0 | nintendogs:bytes [tier A]
    void InitializeWithSdkVersion(unsigned); // 0x0013AF18 | nintendogs:bytes [tier A]
    void CreateFile(nn::fs::Transaction, unsigned long long, unsigned, const unsigned char*, unsigned, nn::fs::Attributes, long long); // 0x00348C30 | nintendogs:bytes [tier A]
    void DeleteFile(nn::fs::Transaction, unsigned long long, unsigned, const unsigned char*, unsigned); // 0x00348C8C | nintendogs:bytes [tier A]
    void RenameFile(nn::fs::Transaction, unsigned long long, unsigned int, const unsigned char*, unsigned int, unsigned long long, unsigned int, const unsigned char*, unsigned int); // 0x00348CDC | fefates:bytes [tier A]
    void OpenDirectory(nn::Handle*, unsigned long long, unsigned, const unsigned char*, unsigned); // 0x00348DA8 | nintendogs:bytes [tier A]
    void ControlArchive(unsigned long long, unsigned, const void*, unsigned, void*, unsigned); // 0x00348E0C | nintendogs:bytes [tier A]
    void FormatSaveData(unsigned, unsigned, const unsigned char*, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned, bool); // 0x00348E78 | nintendogs:bytes [tier A]
    void IsSdmcDetected(bool*); // 0x00348ED0 | nintendogs:bytes [tier A]
    void IsSdmcWritable(bool*); // 0x00348F08 | nintendogs:bytes [tier A]
    void CreateDirectory(nn::fs::Transaction, unsigned long long, unsigned, const unsigned char*, unsigned, nn::fs::Attributes); // 0x00348F40 | nintendogs:bytes [tier A]
    void DeleteDirectory(nn::fs::Transaction, unsigned long long, unsigned, const unsigned char*, unsigned); // 0x00348F94 | nintendogs:bytes [tier A]
    void CreateExtSaveData(const nn::fs::ExtSaveDataSpecifier&, unsigned, unsigned, long long, const void*, unsigned); // 0x00349074 | nintendogs:bytes [tier A]
    void DeleteExtSaveData(const nn::fs::ExtSaveDataSpecifier&); // 0x003490D0 | nintendogs:bytes [tier A]
    void GetArchiveResource(nn::fs::ArchiveResource*, nn::fs::SystemMediaType); // 0x00349148 | nintendogs:bytes [tier A]
    void GetExtDataBlockSize(long long*, long long*, int*, const nn::fs::ExtSaveDataSpecifier&); // 0x003491D0 | nintendogs:bytes [tier B]
    void OpenFile(nn::Handle*, nn::fs::Transaction, unsigned long long, unsigned, const unsigned char*, unsigned, unsigned, nn::fs::Attributes); // 0x00349334 | nintendogs:bytes [tier A]
};
} // namespace ipc
} // namespace fs
} // namespace nn
