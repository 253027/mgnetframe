#include "tcp-connection.h"
#include "http-parser.h"

namespace mg
{
    namespace http
    {
        class HttpConnection : public TcpConnection
        {
        public:
            HttpConnection(EventLoop *loop, const std::string &name, int sockfd,
                           const InternetAddress &localAddress, const InternetAddress &peerAddress);

            ~HttpConnection();

            void setConnectionCallback(HttpConnectionCallback callback);

            void setMessageCallback(HttpMessageCallback callback);

            void setWriteCompleteCallback(HttpCompleteCallback callback);

            void connectionEstablished() override;

            void connectionDestoryed() override;

            void send(http::HttpResponse &response);

        private:
            void onRead(TimeStamp time) override;

            void handleClose() override;

            void onWriteComplete() override;

        private:
            mg::http::HttpRequest _request;
            HttpMessageCallback _httpMessageCallback;
            HttpConnectionCallback _httpConnectionCallback;
            HttpCompleteCallback _httpWriteCompleteCallback;
        };
    }
}