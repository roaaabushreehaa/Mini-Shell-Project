#include "requsetparser.hpp"
#include <string>
#include <cctype>    // std::tolower, std::isdigit, std::isalnum

// Conservative limits (can later be driven by config).
static const size_t MAX_REQUEST_LINE = 8192;
static const size_t MAX_HEADER_BYTES = 16384;
static const size_t MAX_HEADER_COUNT = 200;
static const size_t MAX_TARGET_LEN   = 2048;

static std::string trim(const std::string& s)
{
    size_t start = 0;
    while (start < s.size() && (s[start] == ' ' || s[start] == '\t'))
        ++start;
    size_t end = s.size();
    while (end > start && (s[end - 1] == ' ' || s[end - 1] == '\t'))
        --end;
    return s.substr(start, end - start);
}

static std::string toLowerAscii(const std::string& s)
{
    std::string out;
    out.reserve(s.size());
    for (size_t i = 0; i < s.size(); ++i)
        out.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(s[i]))));
    return out;
}

static bool isTokenChar(unsigned char c)
{
    // RFC7230 tchar: "!#$%&'*+-.^_`|~" / DIGIT / ALPHA
    if (std::isalnum(c))
        return true;
    switch (c)
    {
        case '!': case '#': case '$': case '%': case '&': case '\'':
        case '*': case '+': case '-': case '.': case '^': case '_':
        case '`': case '|': case '~':
            return true;
        default:
            return false;
    }
}

static bool isToken(const std::string& s)
{
    if (s.empty())
        return false;
    for (size_t i = 0; i < s.size(); ++i)
    {
        unsigned char c = static_cast<unsigned char>(s[i]);
        if (!isTokenChar(c))
            return false;
    }
    return true;
}

static bool headerHasTokenValue(const std::string& value, const std::string& tokenLower)
{
    // Best-effort parsing for comma-separated tokens (case-insensitive).
    std::string v = toLowerAscii(value);
    size_t i = 0;
    while (i < v.size())
    {
        while (i < v.size() && (v[i] == ' ' || v[i] == '\t' || v[i] == ','))
            ++i;
        size_t start = i;
        while (i < v.size() && v[i] != ',' && v[i] != ' ' && v[i] != '\t')
            ++i;
        if (start < i)
        {
            if (v.substr(start, i - start) == tokenLower)
                return true;
        }
        while (i < v.size() && v[i] != ',')
            ++i;
    }
    return false;
}

static bool parseUnsignedDecimal(const std::string& s, size_t& out)
{
    if (s.empty())
        return false;
    size_t value = 0;
    for (size_t i = 0; i < s.size(); ++i)
    {
        unsigned char c = static_cast<unsigned char>(s[i]);
        if (!std::isdigit(c))
            return false;
        size_t digit = static_cast<size_t>(c - '0');
        if (value > (static_cast<size_t>(-1) - digit) / 10)
            return false;
        value = value * 10 + digit;
    }
    out = value;
    return true;
}

static bool parseChunkSizeLine(const std::string& line, size_t& chunkSize)
{
    // Accept: 1a;ext=value (ignore extensions)
    std::string s = line;
    size_t semi = s.find(';');
    if (semi != std::string::npos)
        s = s.substr(0, semi);
    s = trim(s);
    if (s.empty())
        return false;

    size_t value = 0;
    for (size_t i = 0; i < s.size(); ++i)
    {
        unsigned char c = static_cast<unsigned char>(s[i]);
        size_t digit;
        if (c >= '0' && c <= '9')
            digit = static_cast<size_t>(c - '0');
        else if (c >= 'a' && c <= 'f')
            digit = static_cast<size_t>(10 + (c - 'a'));
        else if (c >= 'A' && c <= 'F')
            digit = static_cast<size_t>(10 + (c - 'A'));
        else
            return false;

        if (value > (static_cast<size_t>(-1) - digit) / 16)
            return false;
        value = value * 16 + digit;
    }

    chunkSize = value;
    return true;
}

static ParseResult parseChunkedBody(const std::string& buffer, size_t bodyStart, Request& req)
{
    size_t pos = bodyStart;
    req.body.clear();
    while (pos < buffer.size())
    {
        size_t lineEnd = buffer.find('\n', pos);
        if (lineEnd == std::string::npos)
            return PARSE_INCOMPLETE;
        std::string sizeLine = buffer.substr(pos, lineEnd - pos);
        if (!sizeLine.empty() && sizeLine[sizeLine.size() - 1] == '\r')
            sizeLine.erase(sizeLine.size() - 1);
        size_t chunkSize = 0;
        if (!parseChunkSizeLine(sizeLine, chunkSize))
        {
            req.status = HTTP_BAD_REQUEST;
            return PARSE_BAD_REQUEST;
        }
        pos = lineEnd + 1;
        if (chunkSize == 0)
            break;
        if (pos > buffer.size() || chunkSize > buffer.size() - pos ||
            buffer.size() - pos - chunkSize < 2)
            return PARSE_INCOMPLETE;
        req.body += buffer.substr(pos, chunkSize);
        pos += chunkSize + 2;
    }
    return PARSE_OK;
}

static ParseResult parseRequestLine(const std::string& buffer, Request& req, size_t& lineEnd)
{
    // Enforce maximum request line length.
    size_t lineEndPos = buffer.find('\n');
    if (lineEndPos == std::string::npos)
    {
        if (buffer.size() > MAX_REQUEST_LINE)
        {
            req.status = HTTP_BAD_REQUEST;
            return PARSE_BAD_REQUEST;
        }
        return PARSE_INCOMPLETE;
    }
    if (lineEndPos > MAX_REQUEST_LINE)
    {
        req.status = HTTP_BAD_REQUEST;
        return PARSE_BAD_REQUEST;
    }

    // Strip trailing \r from CRLF.
    size_t lineContentEnd = lineEndPos;
    if (lineContentEnd > 0 && buffer[lineContentEnd - 1] == '\r')
        --lineContentEnd;

    lineEnd = lineEndPos;

    std::string line = buffer.substr(0, lineContentEnd);

    // Parse: METHOD SP Request-Target SP HTTP-Version
    size_t sp1 = line.find(' ');
    if (sp1 == std::string::npos)
    {
        req.status = HTTP_BAD_REQUEST;
        return PARSE_BAD_REQUEST;
    }
    req.method = line.substr(0, sp1);
    if (!isToken(req.method))
    {
        req.status = HTTP_BAD_REQUEST;
        return PARSE_BAD_REQUEST;
    }

    size_t sp2 = line.find(' ', sp1 + 1);
    if (sp2 == std::string::npos)
    {
        req.status = HTTP_BAD_REQUEST;
        return PARSE_BAD_REQUEST;
    }

    req.path = line.substr(sp1 + 1, sp2 - sp1 - 1);
    if (req.path.empty())
    {
        req.status = HTTP_BAD_REQUEST;
        return PARSE_BAD_REQUEST;
    }
    if (req.path.size() > MAX_TARGET_LEN)
    {
        req.status = HTTP_URI_TOO_LONG;
        return PARSE_URI_TOO_LONG;
    }

    req.version = line.substr(sp2 + 1);
    if (req.version != "HTTP/1.0" && req.version != "HTTP/1.1")
    {
        req.status = HTTP_VERSION_NOT_SUPPORTED;
        return PARSE_VERSION_UNSUPPORTED;
    }

    return PARSE_OK;
}

static ParseResult parseHeaders(const std::string& buffer, size_t headersStart,
                                size_t headersEnd, Request& req)
{
    if (headersEnd - headersStart > MAX_HEADER_BYTES)
    {
        req.status = HTTP_BAD_REQUEST;
        return PARSE_BAD_REQUEST;
    }

    size_t headerCount = 0;
    size_t i = headersStart;
    while (i < headersEnd)
    {
        // Find end of this header line.
        size_t lineEnd = buffer.find('\n', i);
        if (lineEnd == std::string::npos || lineEnd > headersEnd)
            lineEnd = headersEnd;
        size_t lineContentEnd = lineEnd;
        if (lineContentEnd > i && buffer[lineContentEnd - 1] == '\r')
            --lineContentEnd;

        std::string line = buffer.substr(i, lineContentEnd - i);
        i = lineEnd + 1;

        if (line.empty())
            continue;

        // Find colon separator.
        size_t colon = line.find(':');
        if (colon == std::string::npos)
        {
            req.status = HTTP_BAD_REQUEST;
            return PARSE_BAD_REQUEST;
        }

        std::string key   = line.substr(0, colon);
        std::string value = trim(line.substr(colon + 1));

        if (!isToken(key))
        {
            req.status = HTTP_BAD_REQUEST;
            return PARSE_BAD_REQUEST;
        }

        std::string keyLower = toLowerAscii(key);

        ++headerCount;
        if (headerCount > MAX_HEADER_COUNT)
        {
            req.status = HTTP_BAD_REQUEST;
            return PARSE_BAD_REQUEST;
        }

        // Duplicate Content-Length is always an error.
        std::map<std::string, std::string>::iterator it = req.headers.find(keyLower);
        if (it != req.headers.end())
        {
            if (keyLower == "content-length")
            {
                req.status = HTTP_BAD_REQUEST;
                return PARSE_BAD_REQUEST;
            }
            if (!value.empty())
            {
                if (!it->second.empty())
                    it->second += ",";
                it->second += value;
            }
            continue;
        }

        req.headers[keyLower] = value;
    }

    return PARSE_OK;
}

// Parse body: starts after \r\n\r\n; uses Content-Length or chunked Transfer-Encoding.
static ParseResult parseBody(const std::string& buffer, size_t bodyStart, Request& req)
{
    req.body.clear();

    const bool hasCL = (req.headers.find("content-length") != req.headers.end());
    const bool hasTE = (req.headers.find("transfer-encoding") != req.headers.end());

    if (hasTE)
    {
        if (req.version == "HTTP/1.1" &&
            headerHasTokenValue(req.headers["transfer-encoding"], "chunked"))
            return parseChunkedBody(buffer, bodyStart, req);
        // HTTP/1.0 or unsupported Transfer-Encoding.
        req.status = HTTP_VERSION_NOT_SUPPORTED;
        return PARSE_VERSION_UNSUPPORTED;
    }

    if (!hasCL)
    {
        if (req.method == "POST")
        {
            req.status = HTTP_LENGTH_REQUIRED; // 411
            return PARSE_BAD_REQUEST;
        }
        // No Content-Length and not POST: treat body as empty.
        return PARSE_OK;
    }

    std::string lenStr = trim(req.headers["content-length"]);
    size_t contentLength = 0;
    if (!parseUnsignedDecimal(lenStr, contentLength))
    {
        req.status = HTTP_BAD_REQUEST;
        return PARSE_BAD_REQUEST;
    }

    const size_t need = bodyStart + contentLength;
    if (buffer.size() < need)
        return PARSE_INCOMPLETE;

    req.body = buffer.substr(bodyStart, contentLength);
    return PARSE_OK;
}


// Main parsing function that orchestrates the parsing process
ParseResult parseRequest(const std::string& buffer, Request& req)
{
    // Ensure deterministic values even if parsing fails early.
    req.status = HTTP_OK;
    req.isError = false;
    req.method.clear();
    req.path.clear();
    req.version.clear();
    req.headers.clear();
    req.body.clear();

    size_t lineEnd;
    ParseResult result = parseRequestLine(buffer, req, lineEnd);
    if (result != PARSE_OK)
        return result;

    size_t headersEnd = buffer.find("\r\n\r\n");
    size_t delimLen = 4;
    if (headersEnd == std::string::npos)
    {
        headersEnd = buffer.find("\n\n");
        delimLen = 2;
    }
    if (headersEnd == std::string::npos)
        return PARSE_INCOMPLETE;

    // Move to the start of headers, right after the request line end.
    size_t headersStart = lineEnd;
    if (headersStart < buffer.size() && buffer[headersStart] == '\r')
        headersStart += 2;
    else
        headersStart += 1;

    result = parseHeaders(buffer, headersStart, headersEnd, req);
    if (result != PARSE_OK)
        return result;

    size_t bodyStart = headersEnd + delimLen;

    result = parseBody(buffer, bodyStart, req);
    if (result != PARSE_OK)
        return result;

    return PARSE_OK;
}
