#include "nn/pia/transport/transport_AnalysisPrinter.h"
#include <stdarg.h>
#include <stdio.h>

namespace nn {
namespace pia {
namespace transport {
// 0x00AF5F90
char nn::pia::transport::AnalysisPrinter::s_Buffer[BUFFER_SIZE];
// 0x0097FA30
nn::pia::transport::AnalysisPrinter::Callback nn::pia::transport::AnalysisPrinter::s_Callback;

void nn::pia::transport::AnalysisPrinter::SetCallback(Callback callback)
{
    s_Callback = callback;
}

// 0x0044FE84 | fefates:bytes [tier B]
void nn::pia::transport::AnalysisPrinter::Write(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    vsprintf(s_Buffer, format, args);
    va_end(args);
    if (s_Callback != nullptr) {
        s_Callback(s_Buffer);
    }
}

} // namespace transport
} // namespace pia
} // namespace nn
