#ifndef REQUSETPARSER_HPP
#define REQUSETPARSER_HPP

#include <string>
#include <map>

enum HttpStatus
{
    HTTP_OK                    = 200,
    HTTP_BAD_REQUEST           = 400,
    HTTP_LENGTH_REQUIRED       = 411,
    HTTP_URI_TOO_LONG          = 414,
    HTTP_NOT_IMPLEMENTED       = 501,
    HTTP_VERSION_NOT_SUPPORTED = 505,
};

enum ParseResult
{
    PARSE_OK,
    PARSE_INCOMPLETE,
    PARSE_BAD_REQUEST,
    PARSE_NOT_IMPLEMENTED,
    PARSE_VERSION_UNSUPPORTED,
    PARSE_URI_TOO_LONG,
};

struct Request
{
    std::string                        method;
    std::string                        path;
    std::string                        version;
    std::map<std::string, std::string> headers;
    std::string                        body;
    int                                status;
    bool                               isError;
};

ParseResult parseRequest(const std::string& buffer, Request& req);

#endif
