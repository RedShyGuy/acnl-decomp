#pragma once

#include "decomp.h"

namespace nn {
namespace fs {
namespace ipc {
class File
{
public:
    void SetPriority(int); // 0x0034942C | nintendogs:bytes [tier A]
    void OpenLinkFile(nn::Handle*); // 0x0034945C | nintendogs:bytes [tier A]
    void Read(int*, long long, void*, unsigned); // 0x00349494 | nintendogs:bytes [tier A]
    void Close(); // 0x003494EC | nintendogs:bytes [tier A]
    void Write(int*, long long, const void*, unsigned, nn::fs::WriteOption); // 0x00349518 | nintendogs:bytes [tier A]
    void GetSize(long long*); // 0x003495C4 | nintendogs:bytes [tier A]
};
} // namespace ipc
} // namespace fs
} // namespace nn
