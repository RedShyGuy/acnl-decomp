#pragma once

#include "decomp.h"

namespace nn {
namespace nex {
class Message;

// Instantiations found in the binary:
//   nn::nex::AnyObjectAdapter<nn::nex::Data, nn::nex::String>  typeinfo 0x008CE544
//   nn::nex::AnyObjectAdapter<nn::nex::Gathering, nn::nex::String>  typeinfo 0x008CE54C
//
// Writes and reads an object of a polymorphic DDL type (any<Data,string>, any<Gathering,string>):
// its type name, the size of its data and the data. The registered adapter of a type is in
// s_pInstance (null while no client uses it). The names of the functions and the member are ours.
template <typename T0, typename T1>
class AnyObjectAdapter
{
public:
    virtual ~AnyObjectAdapter() {}
    virtual void StreamIn(Message* pMessage, const T0& object) const = 0;
    // a new object of the type in the message (a clone of the registered prototype)
    virtual void StreamOut(Message* pMessage, T0** ppObject) const = 0;

    static AnyObjectAdapter* s_pInstance;
};

class Data;
class Gathering;
class String;
// (defined in nex_AnyDataAdapter.cpp / nex_AnyGatheringAdapter.cpp)
template <>
AnyObjectAdapter<Data, String>* AnyObjectAdapter<Data, String>::s_pInstance;
template <>
AnyObjectAdapter<Gathering, String>* AnyObjectAdapter<Gathering, String>::s_pInstance;
} // namespace nex
} // namespace nn
