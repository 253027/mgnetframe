#include "http-server.h"
#include "log.h"

mg::HttpServer::HttpServer(EventLoop *loop, const InternetAddress &listenAddress,
                           const std::string &name, int domain, int type)
    : TcpServer(loop, listenAddress, name, domain, type)
{
    ;
}

mg::HttpServer::~HttpServer()
{
    ;
}

void mg::HttpServer::setConnectionCallback(const HttpConnectionCallback &callback)
{
    this->_httpConnectionCallback = std::move(callback);
}

void mg::HttpServer::setMessageCallback(const HttpMessageCallback &callback)
{
    this->_httpMessageCallback = std::move(callback);
}

void mg::HttpServer::setWriteCompleteCallback(const HttpCompleteCallback &callback)
{
    this->_httpWriteCompleteCallback = std::move(callback);
}

void mg::HttpServer::handleNewConnection(EventLoop *loop, const std::string &name, int fd,
                                         const mg::InternetAddress &local,
                                         const mg::InternetAddress &peer)
{
    HttpConnectionPointer connection = std::make_shared<http::HttpConnection>(loop, name, fd, local, peer);
    connection->setConnectionCallback(this->_httpConnectionCallback);
    connection->setMessageCallback(this->_httpMessageCallback);
    connection->setWriteCompleteCallback(this->_httpWriteCompleteCallback);
    connection->setCloseCallback(std::bind(&HttpServer::removeConnection, this, std::placeholders::_1));
    this->_connectionMemo[name] = connection;

    loop->run(std::bind(&http::HttpConnection::connectionEstablished, connection.get()));
}