#ifndef __MG_HTTP_SERVER_H__
#define __MG_HTTP_SERVER_H__

#include "tcp-server.h"
#include "http-connection.h"

namespace mg
{
    class HttpServer : public TcpServer
    {
    public:
        HttpServer(EventLoop *loop, const InternetAddress &listenAddress,
                   const std::string &name, int domain = IPV4_DOMAIN, int type = TCP_SOCKET);

        ~HttpServer();

        void setConnectionCallback(const HttpConnectionCallback &callback);

        void setMessageCallback(const HttpMessageCallback &callback);

        void setWriteCompleteCallback(const HttpCompleteCallback &callback);

        void handleNewConnection(EventLoop *loop, const std::string &name, int fd,
                                 const mg::InternetAddress &local,
                                 const mg::InternetAddress &peer) override;

    private:
        void acceptorCallback(int fd, const InternetAddress &peerAddress);

    private:
        HttpMessageCallback _httpMessageCallback;
        HttpConnectionCallback _httpConnectionCallback;
        HttpCompleteCallback _httpWriteCompleteCallback;
    };
}

#endif // __MG_HTTP_SERVER_H__