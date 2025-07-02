#include "http-connection.h"
#include "event-loop.h"
#include "log.h"

mg::http::HttpConnection::HttpConnection(EventLoop *loop, const std::string &name, int sockfd,
                                         const InternetAddress &localAddress, const InternetAddress &peerAddress)
    : TcpConnection(loop, name, sockfd, localAddress, peerAddress)
{
    ;
}

mg::http::HttpConnection::~HttpConnection()
{
    ;
}

void mg::http::HttpConnection::setConnectionCallback(HttpConnectionCallback callback)
{
    this->_httpConnectionCallback = callback;
}

void mg::http::HttpConnection::connectionEstablished()
{
    this->setConnectionState(CONNECTED);
    this->_channel->tie(shared_from_this());
    this->_channel->enableReading();

    if (this->_httpConnectionCallback)
        this->_httpConnectionCallback(std::static_pointer_cast<HttpConnection>(shared_from_this()));
}

void mg::http::HttpConnection::connectionDestoryed()
{
    LOG_TRACE("{} destroyed", this->_name);
    assert(_loop->isInOwnerThread());
    if (this->_state == CONNECTED)
    {
        this->setConnectionState(DISCONNECTED);
        this->_channel->disableAllEvents();
        if (this->_httpConnectionCallback)
            this->_httpConnectionCallback(std::static_pointer_cast<HttpConnection>(shared_from_this()));
    }
    this->_channel->remove();
}

void mg::http::HttpConnection::setMessageCallback(HttpMessageCallback callback)
{
    this->_httpMessageCallback = std::move(callback);
}

void mg::http::HttpConnection::setWriteCompleteCallback(HttpCompleteCallback callback)
{
    this->_httpWriteCompleteCallback = std::move(callback);
}

void mg::http::HttpConnection::send(mg::http::HttpResponse &response)
{
    this->mg::TcpConnection::send(response.dump());
}

void mg::http::HttpConnection::onRead(TimeStamp time)
{
    while (1)
    {
        int ret = mg::http::parse(this->_readBuffer, this->_request);
        if (ret < 0)
        {
            if (ret == -1)
            {
                LOG_ERROR("[{}] invalid http message", this->name());
                this->forceClose();
            }
            return;
        }

        if (this->_httpMessageCallback)
            this->_httpMessageCallback(std::static_pointer_cast<HttpConnection>(shared_from_this()), &this->_request, time);
        else
            LOG_ERROR("[{}] no message callback", this->name());
    }
}

void mg::http::HttpConnection::handleClose()
{
    this->setConnectionState(DISCONNECTED);
    this->_channel->disableAllEvents();
    this->clearTimer();
    HttpConnectionPointer temp = std::static_pointer_cast<HttpConnection>(shared_from_this());
    if (this->_httpConnectionCallback)
        this->_httpConnectionCallback(temp);
    else
        LOG_WARN("{} _httpConnectionCallback not initial", this->_name);
    if (this->_closeCallback)
        this->_closeCallback(temp);
    else
        LOG_WARN("{} _closeCallback not initial", this->_name);
}

void mg::http::HttpConnection::onWriteComplete()
{
    if (this->_httpWriteCompleteCallback)
        this->_httpWriteCompleteCallback(std::static_pointer_cast<HttpConnection>(shared_from_this()));
    else
        LOG_WARN("{} _httpWriteCompleteCallback not initial", this->_name);
}
