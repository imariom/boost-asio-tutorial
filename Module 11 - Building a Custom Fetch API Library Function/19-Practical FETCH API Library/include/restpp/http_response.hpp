#pragma once
#ifndef REST_HTTP_RESPONSE_HPP
#define REST_HTTP_RESPONSE_HPP

#include <string>

namespace restpp {

/**
 * @brief Represents an HTTP response.
 *
 * This structure holds the headers and body of an HTTP response.
 */
struct http_response
{
    /**
     * @brief Stores the HTTP response headers.
     *
     * The headers include metadata such as content type, status codes, 
     * and other HTTP-specific information.
     */
    std::string headers;

    /**
     * @brief Stores the body of the HTTP response.
     *
     * The body contains the actual content returned by the server, such as 
     * JSON, HTML, or plain text.
     */
    std::string body;
};
    
} // namespace restpp

#endif // REST_HTTP_RESPONSE_HPP
