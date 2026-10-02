#include "nn/fs/ipc/ipc_File.h"

namespace nn {
namespace fs {
namespace ipc {
// 0x0034942C | nintendogs:bytes [tier A]
void nn::fs::ipc::File::SetPriority(int)
{
}

// 0x0034945C | nintendogs:bytes [tier A]
void nn::fs::ipc::File::OpenLinkFile(nn::Handle*)
{
}

// 0x00349494 | nintendogs:bytes [tier A]
void nn::fs::ipc::File::Read(int*, long long, void*, unsigned)
{
}

// 0x003494EC | nintendogs:bytes [tier A]
void nn::fs::ipc::File::Close()
{
}

// 0x00349518 | nintendogs:bytes [tier A]
void nn::fs::ipc::File::Write(int*, long long, const void*, unsigned, nn::fs::WriteOption)
{
}

// 0x003495C4 | nintendogs:bytes [tier A]
void nn::fs::ipc::File::GetSize(long long*)
{
}

} // namespace ipc
} // namespace fs
} // namespace nn
