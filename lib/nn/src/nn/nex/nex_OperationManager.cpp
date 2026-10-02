#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_OperationManager.h"

namespace nn {
namespace nex {
// 0x003831E8 slot 0x00 | fefates:bytes
nn::nex::OperationManager::~OperationManager()
{
}

// 0x00382F20 | mk7dlp:bytes [tier A]
void nn::nex::OperationManager::OperationEnds(nn::nex::Operation*)
{
}

// 0x00382FB4 | mk7dlp:bytes [tier A]
void nn::nex::OperationManager::OperationBegins(nn::nex::Operation*)
{
}

// 0x003830CC | fefates:bytes [tier B]
nn::nex::OperationManager::OperationManager()
{
}

// 0x0072B75C | mk7dlp:bytes [tier A]
void nn::nex::OperationManager::GetCurrentOperation() const
{
}

} // namespace nex
} // namespace nn
