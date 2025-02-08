#pragma once
#ifndef BRO_HTTP_RESPONSE_HPP
#define BRO_HTTP_RESPONSE_HPP

#include <string>

namespace restpp {

struct http_response
{
    std::string headers;
    std::string body;
};
    
} // namespace bro

#endif // BRO_HTTP_RESPONSE_HPP
