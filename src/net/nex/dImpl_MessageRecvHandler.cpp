#include "nn/nex/nex_MessagingNotificationHandler.h"
#include "net/nex/dImpl_MessageRecvHandler.h"

// ctor address unknown
net::nex::Impl::MessageRecvHandler::MessageRecvHandler()
{
}

// 0x003BDDF4 slot 0x00
// 0x00513400 (deleting dtor)
net::nex::Impl::MessageRecvHandler::~MessageRecvHandler()
{
    // nothing to do
}

// 0x00513328 slot 0x08
void net::nex::Impl::MessageRecvHandler::DeliverMessage(nn::nex::AnyObjectHolder<nn::nex::Data, nn::nex::String>& oUserMessage)
{
}

