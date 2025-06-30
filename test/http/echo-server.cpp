#include <sstream>
#include "http-server.h"
#include "../base/log.h"

int main()
{
    mg::LogConfig logConfig("debug", "./log", "server.log");
    INITLOG(logConfig);
    mg::EventLoop loop("http-loop");
    mg::HttpServer server(&loop, mg::InternetAddress("0.0.0.0", 18888), "http-server");

    server.setMessageCallback([](const mg::HttpConnectionPointer &link, mg::http::HttpRequest *request, mg::TimeStamp time)
                              {
                                  mg::http::HttpResponse response;
                                  response.setStatus(mg::http::HttpStatus::OK);
                                  response.setBody("hello world");
                                  link->send(response); //
                              });

    server.start();
    loop.loop();
    return 0;
}