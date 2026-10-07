#include "nn/pia/util/util_Api.h"

namespace nn {
namespace pia {
namespace util {
// 0x00413978 | fefates:bytes [tier B]
bool IsPiaResult(const nn::Result& result)
{
    return result.GetModule() == RESULT_MODULE_PIA;
}

} // namespace util
} // namespace pia
} // namespace nn
