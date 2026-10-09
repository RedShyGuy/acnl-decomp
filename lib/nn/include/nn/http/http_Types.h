#pragma once

#include "decomp.h"

namespace nn {
namespace http {
// the method of a request (type name from the symbols; values after 3dbrew "HTTP:CreateContext",
// names are ours)
enum RequestMethod : u8 {
    REQUEST_METHOD_NONE = 0,
    REQUEST_METHOD_GET = 1,
    REQUEST_METHOD_POST = 2,
    REQUEST_METHOD_HEAD = 3,
    REQUEST_METHOD_PUT = 4,
    REQUEST_METHOD_DELETE = 5,
};

// how the post data is sent (values after 3dbrew "HTTP:SetPostDataType", names are ours)
enum PostDataType : u8 {
    POST_DATA_TYPE_ASCII = 0,
    POST_DATA_TYPE_BINARY = 1,
    POST_DATA_TYPE_RAW = 2,
};
} // namespace http
} // namespace nn
