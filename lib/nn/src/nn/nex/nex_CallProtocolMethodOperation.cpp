#include "nn/nex/nex_CallProtocolMethodOperation.h"

namespace nn {
namespace nex {
// 0x003BC298 | mk7dlp:bytes [tier A]
void nn::nex::CallProtocolMethodOperation::InitializeForNewCallAttempt()
{
    const int RESULT_SUCCESS = qResult::SUCCESS;
    m_AttemptCount++;
    m_Unknown0x14 = false;
    m_Result = qResult(RESULT_SUCCESS);
}

// 0x003BC2E4 slot 0x14
void nn::nex::CallProtocolMethodOperation::vf_0x14()
{
    // nothing to do
}

// 0x003BC2F8 slot 0x00
// 0x003BC2E8 (deleting dtor)
nn::nex::CallProtocolMethodOperation::~CallProtocolMethodOperation()
{
    // nothing to do
}

// 0x0072DE18 slot 0x0C
u32 nn::nex::CallProtocolMethodOperation::GetTypeImpl() const
{
    return TYPE;
}

// 0x0072DDE8 slot 0x10
const wchar_t* nn::nex::CallProtocolMethodOperation::GetName() const
{
    return L"CallProtocolMethod";
}

} // namespace nex
} // namespace nn
