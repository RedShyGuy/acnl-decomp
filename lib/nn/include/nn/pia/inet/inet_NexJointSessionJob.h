#pragma once

#include "decomp.h"
#include "nn/pia/session/session_JointSessionJob.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet18NexJointSessionJobE @ 0x008CF8EC
// vtable 0x00900150 (vptr 0x00900158), offset_to_top 0, 25 entries
class NexJointSessionJob : public ::nn::pia::session::JointSessionJob
{
public:
    NexJointSessionJob(); // ctor candidate(s) 0x003F8334 (unverified)
    virtual ~NexJointSessionJob(); // 0x003F8400 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x003F83DC slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x0072F154 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    virtual void vf_0x18(); // 0x003E9EE0 slot 0x18 | virtual slot, introduced by nn::pia::session::JointSessionJob
    virtual void vf_0x24(); // 0x003F6F08 slot 0x24 | virtual slot, introduced by nn::pia::session::JointSessionJob
    virtual void vf_0x28(); // 0x003F5060 slot 0x28 | virtual slot, introduced by nn::pia::session::JointSessionJob
    virtual void vf_0x2C(); // 0x003F3468 slot 0x2C | virtual slot, introduced by nn::pia::session::JointSessionJob
    virtual void vf_0x30(); // 0x003F4588 slot 0x30 | virtual slot, introduced by nn::pia::session::JointSessionJob
    virtual void vf_0x34(); // 0x003F55E8 slot 0x34 | virtual slot, introduced by nn::pia::session::JointSessionJob
    virtual void vf_0x38(); // 0x003F1F24 slot 0x38 | virtual slot, introduced by nn::pia::session::JointSessionJob
    virtual void vf_0x3C(); // 0x003F09CC slot 0x3C | virtual slot, introduced by nn::pia::session::JointSessionJob
    virtual void vf_0x40(); // 0x003F27E8 slot 0x40 | virtual slot, introduced by nn::pia::session::JointSessionJob
    virtual void vf_0x44(); // 0x003F55D0 slot 0x44 | virtual slot, introduced by nn::pia::session::JointSessionJob
    virtual void vf_0x48(); // 0x003F5050 slot 0x48 | virtual slot, introduced by nn::pia::session::JointSessionJob
    virtual void vf_0x4C(); // 0x003F1E18 slot 0x4C | virtual slot, introduced by nn::pia::session::JointSessionJob
    virtual void vf_0x50(); // 0x003F2770 slot 0x50 | virtual slot, introduced by nn::pia::session::JointSessionJob
    virtual void vf_0x54(); // 0x003F1ECC slot 0x54 | virtual slot, introduced by nn::pia::session::JointSessionJob
    virtual void vf_0x58(); // 0x003ED7A4 slot 0x58 | virtual slot, introduced by nn::pia::session::JointSessionJob
    virtual void vf_0x5C(); // 0x003F1E5C slot 0x5C | virtual slot, introduced by nn::pia::session::JointSessionJob
    virtual void vf_0x60(); // 0x003E9248 slot 0x60 | virtual slot, introduced by nn::pia::session::JointSessionJob
};
} // namespace inet
} // namespace pia
} // namespace nn
