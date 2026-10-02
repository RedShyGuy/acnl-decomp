#include "nn/pia/transport/transport_MissingStationHandler.h"
#include "nn/pia/inet/inet_MissingStationHandler.h"

namespace nn {
namespace pia {
namespace inet {
// ctor address unknown
nn::pia::inet::MissingStationHandler::MissingStationHandler()
{
}

// 0x004015B4 slot 0x00 | virtual slot, introduced by nn::pia::inet::MissingStationHandler
void nn::pia::inet::MissingStationHandler::vf_0x00()
{
}

// 0x004015B0 slot 0x04 | virtual slot, introduced by nn::pia::inet::MissingStationHandler
void nn::pia::inet::MissingStationHandler::vf_0x04()
{
}

// 0x00401570 slot 0x08 | fefates:bytes
void nn::pia::inet::MissingStationHandler::Execute(nn::pia::transport::StationConnectionInfo*)
{
}

} // namespace inet
} // namespace pia
} // namespace nn
