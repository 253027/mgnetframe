#ifndef __MG_HTTP_PACKET_PARSER_H__
#define __MG_HTTP_PACKET_PARSER_H__

#include "singleton.h"
#include "function-callbacks.h"
#include "picohttpparser.h"
#include "http-type.h"
#include "buffer.h"

#include <vector>
#include <tuple>
#include <algorithm>
#include <unordered_map>

namespace mg
{
    namespace http
    {
        class HttpRequest
        {
            friend int parse(mg::Buffer &buf, HttpRequest &request);

        public:
            HttpRequest() : isComplete(false), lastCheckIndex(0) {}

            HttpRequest &operator=(HttpRequest &&other);

            const std::string &getMethod() const;

            const std::string &getPath() const;

            bool hasHeader(const std::string &key) const;

            const std::string &getHeader(const std::string &key) const;

            const std::string &getBody() const;

        private:
            std::string method;
            std::string path;
            std::unordered_map<std::string, std::string> headers;
            std::string body;
            bool isComplete;
            size_t lastCheckIndex;
        };

        class HttpResponse
        {
        public:
            void setStatus(HttpStatus status);

            template <typename T, typename U>
            void setHeader(T &&key, U &&value)
            {
                this->headers.insert(std::make_pair(std::forward<T>(key), std::forward<U>(value)));
            }

            template <typename T>
            void setBody(T &&body)
            {
                this->body = std::forward<T>(body);
            }

            std::string dumpHead() const;

            std::string dump() const;

        private:
            HttpStatus status;
            std::unordered_map<std::string, std::string> headers;
            std::string body;
        };

        int parse(mg::Buffer &buf, HttpRequest &request);

        bool urlDecode(const std::string &str, std::string &result);
    }

    std::vector<std::string> spilt(const std::string &str, const std::string &delimiter);

    std::string urlEncode(const std::string &str);
}

#endif //__MG_HTTP_PACKET_PARSER_H__