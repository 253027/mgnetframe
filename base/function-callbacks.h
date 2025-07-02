#ifndef __MG_FUNCTION_CALLBACK_H__
#define __MG_FUNCTION_CALLBACK_H__

#include "time-stamp.h"

#include <functional>
#include <memory>

namespace mg
{
    namespace http
    {
        class HttpConnection;
        class HttpRequest;
    }

    class Buffer;
    class TcpConnection;
    class Connector;
    using TcpConnectionPointer = std::shared_ptr<TcpConnection>;
    using HttpConnectionPointer = std::shared_ptr<http::HttpConnection>;
    using ConnectorPointer = std::shared_ptr<Connector>;

    template <typename... Args>
    using BaseHandler = std::function<void(Args...)>;

    using Handler = BaseHandler<>;
    using TcpConnectionCallback = BaseHandler<const TcpConnectionPointer &>;
    using HttpConnectionCallback = BaseHandler<const HttpConnectionPointer &>;
    using MessageDataCallback = BaseHandler<const TcpConnectionPointer &, Buffer *, TimeStamp>;
    using HttpMessageCallback = BaseHandler<const HttpConnectionPointer &, http::HttpRequest *, TimeStamp>;
    using WriteCompleteCallback = BaseHandler<const TcpConnectionPointer &>;
    using HttpCompleteCallback = BaseHandler<const HttpConnectionPointer &>;
    using ConnectionClosedCallback = BaseHandler<const TcpConnectionPointer &>;
    using HighWaterMarkCallback = BaseHandler<const TcpConnectionPointer &, int>;
}

#endif //__MG_FUNCTION_CALLBACK_H__