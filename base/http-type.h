#ifndef __MG_HTTP_TYPE_H__
#define __MG_HTTP_TYPE_H__

#include <string>

namespace mg
{
    namespace http
    {
        #define HTTP_METHOD_DEFINE(FUNC)       \
            FUNC(0, GET, GET)                  \
            FUNC(1, POST, POST)                \
            FUNC(2, PUT, PUT)                  \
            FUNC(3, DELETE, DELETE)            \
            FUNC(4, HEAD, HEAD)                \
            FUNC(5, OPTIONS, OPTIONS)          \
            FUNC(6, PATCH, PATCH)              \
            FUNC(7, TRACE, TRACE)              \
            FUNC(8, CONNECT, CONNECT)          \
            FUNC(9, COPY, COPY)                \
            FUNC(10, LOCK, LOCK)               \
            FUNC(11, MKCOL, MKCOL)             \
            FUNC(12, MOVE, MOVE)               \
            FUNC(13, PROPFIND, PROPFIND)       \
            FUNC(14, PROPPATCH, PROPPATCH)     \
            FUNC(15, SEARCH, SEARCH)           \
            FUNC(16, UNLOCK, UNLOCK)           \
            FUNC(17, BIND, BIND)               \
            FUNC(18, REBIND, REBIND)           \
            FUNC(19, UNBIND, UNBIND)           \
            FUNC(20, ACL, ACL)                 \
            FUNC(21, REPORT, REPORT)           \
            FUNC(22, MKACTIVITY, MKACTIVITY)   \
            FUNC(23, CHECKOUT, CHECKOUT)       \
            FUNC(24, MERGE, MERGE)             \
            FUNC(25, MSEARCH, MSEARCH)         \
            FUNC(26, NOTIFY, NOTIFY)           \
            FUNC(27, SUBSCRIBE, SUBSCRIBE)     \
            FUNC(28, UNSUBSCRIBE, UNSUBSCRIBE) \
            FUNC(29, PURGE, PURGE)             \
            FUNC(30, MKCALENDAR, MKCALENDAR)   \
            FUNC(31, LINK, LINK)               \
            FUNC(32, UNLINK, UNLINK)           \
            FUNC(33, SOURCE, SOURCE)

        #define HTTP_STATUS_DEFINE(FUNC)                                                \
            FUNC(100, CONTINUE, Continue)                                               \
            FUNC(101, SWITCHING_PROTOCOLS, Switching Protocols)                         \
            FUNC(102, PROCESSING, Processing)                                           \
            FUNC(200, OK, OK)                                                           \
            FUNC(201, CREATED, Created)                                                 \
            FUNC(202, ACCEPTED, Accepted)                                               \
            FUNC(203, NON_AUTHORITATIVE_INFORMATION, Non-Authoritative Information)     \
            FUNC(204, NO_CONTENT, No Content)                                           \
            FUNC(205, RESET_CONTENT, Reset Content)                                     \
            FUNC(206, PARTIAL_CONTENT, Partial Content)                                 \
            FUNC(207, MULTI_STATUS, Multi-Status)                                       \
            FUNC(208, ALREADY_REPORTED, Already Reported)                               \
            FUNC(226, IM_USED, IM Used)                                                 \
            FUNC(300, MULTIPLE_CHOICES, Multiple Choices)                               \
            FUNC(301, MOVED_PERMANENTLY, Moved Permanently)                             \
            FUNC(302, FOUND, Found)                                                     \
            FUNC(303, SEE_OTHER, See Other)                                             \
            FUNC(304, NOT_MODIFIED, Not Modified)                                       \
            FUNC(305, USE_PROXY, Use Proxy)                                             \
            FUNC(307, TEMPORARY_REDIRECT, Temporary Redirect)                           \
            FUNC(308, PERMANENT_REDIRECT, Permanent Redirect)                           \
            FUNC(400, BAD_REQUEST, Bad Request)                                         \
            FUNC(401, UNAUTHORIZED, Unauthorized)                                       \
            FUNC(402, PAYMENT_REQUIRED, Payment Required)                               \
            FUNC(403, FORBIDDEN, Forbidden)                                             \
            FUNC(404, NOT_FOUND, Not Found)                                             \
            FUNC(405, METHOD_NOT_ALLOWED, Method Not Allowed)                           \
            FUNC(406, NOT_ACCEPTABLE, Not Acceptable)                                   \
            FUNC(407, PROXY_AUTHENTICATION_REQUIRED, Proxy Authentication Required)     \
            FUNC(408, REQUEST_TIMEOUT, Request Timeout)                                 \
            FUNC(409, CONFLICT, Conflict)                                               \
            FUNC(410, GONE, Gone)                                                       \
            FUNC(411, LENGTH_REQUIRED, Length Required)                                 \
            FUNC(412, PRECONDITION_FAILED, Precondition Failed)                         \
            FUNC(413, PAYLOAD_TOO_LARGE, Payload Too Large)                             \
            FUNC(414, URI_TOO_LONG, URI Too Long)                                       \
            FUNC(415, UNSUPPORTED_MEDIA_TYPE, Unsupported Media Type)                   \
            FUNC(416, RANGE_NOT_SATISFIABLE, Range Not Satisfiable)                     \
            FUNC(417, EXPECTATION_FAILED, Expectation Failed)                           \
            FUNC(421, MISDIRECTED_REQUEST, Misdirected Request)                         \
            FUNC(422, UNPROCESSABLE_ENTITY, Unprocessable Entity)                       \
            FUNC(423, LOCKED, Locked)                                                   \
            FUNC(424, FAILED_DEPENDENCY, Failed Dependency)                             \
            FUNC(426, UPGRADE_REQUIRED, Upgrade Required)                               \
            FUNC(428, PRECONDITION_REQUIRED, Precondition Required)                     \
            FUNC(429, TOO_MANY_REQUESTS, Too Many Requests)                             \
            FUNC(431, REQUEST_HEADER_FIELDS_TOO_LARGE, Request Header Fields Too Large) \
            FUNC(451, UNAVAILABLE_FOR_LEGAL_REASONS, Unavailable For Legal Reasons)     \
            FUNC(500, INTERNAL_SERVER_ERROR, Internal Server Error)                     \
            FUNC(501, NOT_IMPLEMENTED, Not Implemented)                                 \
            FUNC(502, BAD_GATEWAY, Bad Gateway)                                         \
            FUNC(503, SERVICE_UNAVAILABLE, Service Unavailable)                         \
            FUNC(504, GATEWAY_TIMEOUT, Gateway Timeout)                                 \
            FUNC(505, HTTP_VERSION_NOT_SUPPORTED, HTTP Version Not Supported)           \
            FUNC(506, VARIANT_ALSO_NEGOTIATES, Variant Also Negotiates)                 \
            FUNC(507, INSUFFICIENT_STORAGE, Insufficient Storage)                       \
            FUNC(508, LOOP_DETECTED, Loop Detected)                                     \
            FUNC(510, NOT_EXTENDED, Not Extended)                                       \
            FUNC(511, NETWORK_AUTHENTICATION_REQUIRED, Network Authentication Required)

        enum class HttpMethod
        {
            #define FUNC(code, name, desc) name = code,
                    HTTP_METHOD_DEFINE(FUNC)
            #undef FUNC
            INVALID_METHOD
        };

        enum class HttpStatus
        {
            #define FUNC(code, name, desc) name = code,
                    HTTP_STATUS_DEFINE(FUNC)
            #undef FUNC
            INVALID_STATUS
        };

        /**
         * @brief Converts a string representation to an HttpMethod enum value
         * @param method The string representation of the HTTP method
         * @return The corresponding HttpMethod enumeration value
         */
        HttpMethod stringToMethod(const std::string &method);

        /**
         * @brief Converts an HttpMethod enum value to a string representation
         * @param method The HttpMethod enumeration value
         * @return The string representation of the HTTP method
         */
        const char *methodToString(HttpMethod method);

        /**
         * @brief Converts an HttpStatus enum value to a string representation
         * @param status The HttpStatus enumeration value
         * @return The string representation of the HTTP status
         */
        const char * statusToString(HttpStatus status);
    }
}

#endif // __MG_HTTP_TYPE_H__