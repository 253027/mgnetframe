#include <sstream>
#include "http-server.h"
#include "../base/log.h"
#include <fstream>

static std::string content;

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
                                  response.setHeader("Content-Type", "text/html");
                                  response.setBody(content);
                                  link->send(response); //
                              });

    server.setConnectionCallback([](const mg::HttpConnectionPointer &link)
                                 {
                                     if (link->connected())
                                         LOG_DEBUG("[{}] connected", link->name());
                                     else
                                         LOG_DEBUG("[{}] disconnected", link->name()); //
                                 });

    std::fstream file("./index.html");
    if (file.is_open())
    {
        content = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();
    }

    server.start();
    loop.loop();
    return 0;
}