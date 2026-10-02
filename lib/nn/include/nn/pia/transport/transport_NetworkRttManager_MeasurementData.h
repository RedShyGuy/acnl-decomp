#pragma once

#include "decomp.h"
#include "nn/pia/transport/transport_NetworkRttManager.h"

class nn::pia::transport::NetworkRttManager::MeasurementData
{
public:
    void UpdateTime(unsigned int); // 0x00454B2C | fefates:bytes [tier B]
};
