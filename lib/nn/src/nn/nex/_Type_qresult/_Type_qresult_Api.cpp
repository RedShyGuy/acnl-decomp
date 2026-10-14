#include "nn/nex/_Type_qresult/_Type_qresult_Api.h"
#include "nn/nex/nex_Message.h"
#include "nn/nex/nex_qResult.h"

namespace nn {
namespace nex {
// 0x0036FBFC | fefates:bytes [tier B]
void nn::nex::_Type_qresult::Extract(nn::nex::Message* pMessage, nn::nex::qResult* pResult)
{
    u32 code;
    pMessage->ExtractPrimitiveInline(&code);
    *pResult = qResult(static_cast<const int&>(code));
}

} // namespace nex
} // namespace nn
