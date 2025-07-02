#include "http-parser.h"
#include <sstream>
#include <iomanip>

mg::http::HttpRequest &mg::http::HttpRequest::operator=(HttpRequest &&other)
{
    if (this != &other)
    {
        this->method = std::move(other.method);
        this->path = std::move(other.path);
        this->headers = std::move(other.headers);
        this->body = std::move(other.body);
        this->isComplete = other.isComplete;
        this->lastCheckIndex = other.lastCheckIndex;
    }
    return *this;
}

const std::string &mg::http::HttpRequest::getMethod() const
{
    return this->method;
}

const std::string &mg::http::HttpRequest::getPath() const
{
    return this->path;
}

bool mg::http::HttpRequest::hasHeader(const std::string &key) const
{
    return this->headers.count(key);
}

const std::string &mg::http::HttpRequest::getHeader(const std::string &key) const
{
    static const std::string memo;
    auto it = this->headers.find(key);
    if (it == this->headers.end())
        return memo;
    return it->second;
}

const std::string &mg::http::HttpRequest::getBody() const
{
    return this->body;
}

void mg::http::HttpResponse::setStatus(HttpStatus status)
{
    this->status = status;
}

std::string mg::http::HttpResponse::dumpHead() const
{
    std::stringstream response;
    response << "HTTP/1.1 " << static_cast<uint16_t>(status) << " " << statusToString(status) << "\r\n";
    for (const auto &val : headers)
        response << val.first << ": " << val.second << "\r\n";
    return response.str();
}

std::string mg::http::HttpResponse::dump() const
{
    std::stringstream response;
    response << this->dumpHead() << "Content-Length: "
             << std::to_string(body.size()) << "\r\n\r\n"
             << body;
    return response.str();
}

std::vector<std::string> mg::spilt(const std::string &str, const std::string &delimiter)
{
    if (delimiter.empty())
        return {str};
    std::vector<std::string> ret;

    size_t start = 0, end;
    while ((end = str.find(delimiter, start)) != std::string::npos)
    {
        std::string temp = str.substr(start, end - start);
        if (!temp.empty())
            ret.emplace_back(temp);
        start = end + delimiter.size();
    }
    std::string temp = str.substr(start);
    if (!temp.empty())
        ret.push_back(temp);
    return ret;
}

bool mg::http::urlDecode(const std::string &str, std::string &result)
{
    result.clear();
    result.reserve(str.size());

    for (size_t i = 0; i < str.size();)
    {
        if (str[i] == '%')
        {
            if (i + 2 >= str.size())
                return false;

            char hex1 = str[i + 1];
            char hex2 = str[i + 2];
            if (!std::isxdigit(hex1) || !std::isxdigit(hex2))
                return false;

            int value = 0;
            std::istringstream iss(str.substr(i + 1, 2));
            if (!(iss >> std::hex >> value))
                return false;

            result += static_cast<unsigned char>(value);
            i += 3;
        }
        else if (str[i] == '+')
        {
            result += ' ';
            i++;
        }
        else
        {
            result += str[i];
            i++;
        }
    }

    return true;
}

std::string mg::urlEncode(const std::string &str)
{
    std::ostringstream os;
    for (auto &c : str)
    {
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~')
            os << c;
        else
            os << '%' << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << (u_char)c;
    }
    return os.str();
}

static std::string tolower(const std::string &str)
{
    std::string res = str;
    std::transform(res.begin(), res.end(), res.begin(), [](unsigned char c)
                   { return std::tolower(c); });
    return res;
}

int mg::http::parse(mg::Buffer &buf, mg::http::HttpRequest &request)
{
    if (request.isComplete)
    {
        request.isComplete = false;
        request.lastCheckIndex = 0;
    }

    const char *method, *path;
    int minor_version;
    struct phr_header headers[256];
    size_t method_len, path_len, num_headers;
    num_headers = sizeof(headers) / sizeof(headers[0]);
    int ret = ::phr_parse_request(reinterpret_cast<char *>(buf.readPeek()), buf.readableBytes(),
                                  &method, &method_len, &path, &path_len, &minor_version, headers, &num_headers, request.lastCheckIndex);
    if (ret < 0)
    {
        if (ret == -2)
            request.lastCheckIndex = buf.readableBytes(); // for fast countermeasure
        return ret;
    }

    // get path
    std::string temp;
    if (!urlDecode(std::string(path, path_len), temp))
        return -1;
    request.path = std::move(temp);

    request.isComplete = true;

    // get request method
    request.method = std::string(method, method_len);

    // get body size
    int body_size = 0;

    // get all headers
    for (int i = 0; i < num_headers; i++)
    {
        if (!urlDecode(std::string(headers[i].value, headers[i].value_len), temp))
            return -1;
        request.headers[tolower(std::string(headers[i].name, headers[i].name_len))] = std::move(temp);

        if (body_size > 0)
            continue;

        auto it = request.headers.find("content-length");
        if (it != request.headers.end())
        {
            int count_size = std::count_if(it->second.begin(), it->second.end(), [](char c)
                                           { return std::isdigit(c); });
            if (count_size != it->second.size())
                return -1;

            body_size = std::stoi(it->second);
        }
        if (buf.readableBytes() < ret + body_size)
            return -2;
    }

    request.body = buf.retrieveAsString(ret + body_size).substr(ret);
    return ret;
}
