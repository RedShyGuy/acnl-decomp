#pragma once

#include "decomp.h"

namespace nn {
namespace fs {
namespace CTR {
namespace MPCore {
namespace detail {
class UserFileSystem
{
public:
    void TryDeleteFile(const wchar_t*); // 0x0012925C | fefates:bytes [tier B]
    void Initialize(nn::Handle); // 0x0013616C | nintendogs:bytes [tier A]
    void TryCreateFile(const wchar_t*, long long); // 0x00136198 | nintendogs:bytes-fuzzy [tier A]
    void TryOpenFile(void**, const wchar_t*, unsigned int); // 0x0013AE4C | fefates:bytes [tier B]
    void TryReadFile(int*, void*, long long, void*, unsigned); // 0x00140544 | nintendogs:bytes [tier A]
    void TryWriteFile(int*, void*, long long, const void*, unsigned, bool); // 0x001405A4 | nintendogs:bytes [tier A]
    void TryGetFileSize(long long*, const void*); // 0x00140608 | nintendogs:callgraph [tier A]
    void CloseDirectory(void*); // 0x00347A40 | nintendogs:callgraph [tier A]
    void TryOpenDirectory(void**, const wchar_t*); // 0x00347A78 | nintendogs:callgraph [tier A]
    void TryReadDirectory(int*, void*, nn::fs::DirectoryEntry*, int); // 0x00347B04 | nintendogs:bytes [tier A]
    void TrySetPriorityForFile(void*, int); // 0x00347C2C | fefates:bytes [tier B]
    void CloseFile(void*); // 0x00373B68 | nintendogs:bytes [tier B]
};
} // namespace detail
} // namespace MPCore
} // namespace CTR
} // namespace fs
} // namespace nn
