#include "http-type.h"

#include <cstring>
#include <cstdint>

mg::http::HttpMethod mg::http::stringToMethod(const std::string &method)
{
#define FUNC(code, name, desc)                    \
    if (::strcasecmp(method.c_str(), #name) == 0) \
        return mg::http::HttpMethod::name;
    HTTP_METHOD_DEFINE(FUNC)
#undef FUNC
    return mg::http::HttpMethod::INVALID_METHOD;
}

const static char *method_string[] =
{
#define FUNC(code, name, desc) #name,
        HTTP_METHOD_DEFINE(FUNC)
#undef FUNC
};

const char *mg::http::methodToString(HttpMethod method)
{
    uint32_t index = static_cast<uint32_t>(method);
    if (index >= sizeof(method_string) / sizeof(method_string[0]))
        return "Invalid Method";
    return method_string[index];
}

const char *mg::http::statusToString(HttpStatus status)
{
    switch(status)
    {
    #define FUNC(code, name, desc) \
        case mg::http::HttpStatus::name: \
            return #desc;
        HTTP_STATUS_DEFINE(FUNC)
    #undef FUNC
    default:
        return "Unknown Status";
    }
}