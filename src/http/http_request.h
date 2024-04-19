#include <string>
#include <vector>
#include <unordered_map>

class HttpRequest {
public:
    std::string _request_line;
    std::vector<std::string> _request_header;
    std::string _blank;
    std::string _request_body;


    std::string _method;
    std::string _uri;
    std::string _version;
    std::unordered_map<std::string, std::string> _header_kv;
    int _content_length;
    std::string _path;
    std::string _query_string;

    bool _cgi;
public:
    HttpRequest()
        :_content_length(0)
        , _cgi(false)
    {}
    ~HttpRequest()
    {}
};