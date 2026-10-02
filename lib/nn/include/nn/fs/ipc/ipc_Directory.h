#pragma once

#include "decomp.h"

namespace nn {
namespace fs {
namespace ipc {
class Directory
{
public:
    void Read(int*, nn::fs::DirectoryEntry*, int); // 0x00349694 | nintendogs:bytes [tier A]
    void Close(); // 0x003496E8 | nintendogs:bytes [tier A]
};
} // namespace ipc
} // namespace fs
} // namespace nn
