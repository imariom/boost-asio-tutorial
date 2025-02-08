#pragma once
#ifndef RESTPP_FETCH_HPP
#define RESTPP_FETCH_HPP

#include <string>
#include <utility>
#include <boost/asio.hpp>
#include "http_response.hpp"

namespace restpp {
namespace detail {

template<typename FetchHandler>
class receive_completion_handler
{
public:
    using fetch_handler_t = FetchHandler;

    receive_completion_handler(FetchHandler&& handler,
        std::shared_ptr<boost::asio::ip::tcp::socket> socket,
        std::shared_ptr<http_response> response,
        bool extracted = false)
        : handler_(std::move(handler)), socket_(socket),
            response_(response), headers_extracted_(extracted)
    {
    }

    void operator()(const boost::system::error_code& ec,
        std::size_t header_length)
    {
        if (ec)
        {
            if (ec == boost::asio::error::eof)
            {
                // Done reading or the server closed connection
                handler_(boost::system::error_code{}, *response_);
                return;
            }
            else
            {
                handler_(ec, http_response{});
                return;
            }
        }

        // Extract the headers and move any remaining data to the body
        if (!headers_extracted_)
        {
            auto headers_end = response_->headers.find("\r\n\r\n");
            if (headers_end != std::string::npos)
            {
                // Extract headers excluding the delimiters "\r\n\r\n"
                std::string headers =
                    response_->headers.substr(0, headers_end);

                // Move the remaining data (if any) to the body
                // beyond the delimiters
                if (response_->headers.size() > headers_end + 4)
                {
                    response_->body =
                        response_->headers.substr(headers_end + 4);
                }

                response_->headers = headers;
                response_->headers.resize(headers_end); // Trim headers
            }

            // Mark headers as processed
            headers_extracted_ = true;
        }
            
        // Receive HTTP response body
        boost::asio::async_read(*socket_,
            boost::asio::dynamic_buffer(response_->body),
            boost::asio::transfer_at_least(1),
            receive_completion_handler<fetch_handler_t>(
                std::move(handler_), socket_, response_, headers_extracted_));
    }

private:
    FetchHandler handler_;
    std::shared_ptr<boost::asio::ip::tcp::socket> socket_;
    std::shared_ptr<http_response> response_;
    bool headers_extracted_;
};

template<typename FetchHandler>
class send_completion_handler
{
public:
    using fetch_handler_t = FetchHandler;

    send_completion_handler(FetchHandler&& handler,
        std::shared_ptr<boost::asio::ip::tcp::socket> socket)
        : handler_(std::move(handler)), socket_(socket),
            response_(std::make_shared<http_response>())
    {
    }

    void operator()(const boost::system::error_code& ec,
        std::size_t bytes_transferred)
    {
        if (ec)
        {
            handler_(ec, http_response{});
            return;
        }

        // Receive HTTP response headers
        boost::asio::async_read_until(*socket_,
            boost::asio::dynamic_buffer(response_->headers), "\r\n\r\n",
            receive_completion_handler<fetch_handler_t>(
                std::move(handler_), socket_, response_));
    }

private:
    FetchHandler handler_;
    std::shared_ptr<boost::asio::ip::tcp::socket> socket_;
    std::shared_ptr<http_response> response_;
};

template<typename FetchHandler>
class connect_completion_handler
{
public:
    using fetch_handler_t = FetchHandler;

    connect_completion_handler(FetchHandler&& handler,
        std::shared_ptr<boost::asio::ip::tcp::socket> socket,
        std::shared_ptr<std::string> host,
        std::shared_ptr<std::string> path)
        : handler_(std::move(handler)), socket_(socket), host_(host),
            path_(path)
    {
    }

    void operator()(const boost::system::error_code& ec,
        const boost::asio::ip::tcp::endpoint& endpoint)
    {
        if (ec)
        {
            handler_(ec, http_response{});
            return;
        }

        std::ostringstream request;
        request << "GET " << *path_ << " HTTP/1.1\r\n"
                << "Host: " << *host_ << "\r\n"
                << "Connection: close\r\n"
                << "User-Agent: restpp/1.0.0\r\n"
                << "Accept: text/html,text/json\r\n"
                << "\r\n";

        // Send the HTTP request
        boost::asio::async_write(*socket_,
            boost::asio::buffer(request.str()),
            send_completion_handler<fetch_handler_t>(
                std::move(handler_), socket_));
    }

private:
    FetchHandler handler_;
    std::shared_ptr<boost::asio::ip::tcp::socket> socket_;
    std::shared_ptr<std::string> host_;
    std::shared_ptr<std::string> path_;
};

template<typename FetchHandler>
class resolve_completion_handler
{
public:
    using fetch_handler_t = typename std::decay<FetchHandler>::type;

    resolve_completion_handler(
        typename std::decay<FetchHandler>::type handler,
        std::shared_ptr<boost::asio::ip::tcp::resolver> resolver,
        std::shared_ptr<std::string> host,
        std::shared_ptr<std::string> path)
        : handler_(std::move(handler)), resolver_(resolver), host_(host),
            path_(path)
    {
    }

    void operator()(const boost::system::error_code& ec,
        boost::asio::ip::tcp::resolver::results_type results)
    {
        if (ec)
        {
            handler_(ec, http_response{});
            return;
        }
        
        // Connect to the host
        auto socket = std::make_shared<boost::asio::ip::tcp::socket>(
            resolver_->get_executor());

        boost::asio::async_connect(*socket, results,
            connect_completion_handler<fetch_handler_t>(
                std::move(handler_), socket, host_, path_));
    }

private:
    typename std::decay<FetchHandler>::type handler_;
    std::shared_ptr<boost::asio::ip::tcp::resolver> resolver_;
    std::shared_ptr<std::string> host_;
    std::shared_ptr<std::string> path_;
};

class initiate_async_fetch
{
public:
    using executor_type = boost::asio::io_context::executor_type;
    
    explicit initiate_async_fetch(const executor_type& executor)
        : resolver_(std::make_shared<boost::asio::ip::tcp::resolver>(executor))
    {
    }

    template<typename FetchHandler>
    void operator()(FetchHandler&& fetch_handler,
        std::shared_ptr<std::string> host,
        std::shared_ptr<std::string> path)
    {
        // Resolve DNS name
        resolver_->async_resolve(*host, "http",
            resolve_completion_handler<FetchHandler>(
                std::forward<FetchHandler>(fetch_handler), resolver_, host,
                    path));
    }

private:
    std::shared_ptr<boost::asio::ip::tcp::resolver> resolver_;
};

} // namespace detail
    
template <typename CompletionToken>
auto fetch(boost::asio::io_context& io_context,
    std::string host, std::string path, CompletionToken&& token)
    -> decltype(boost::asio::async_initiate<CompletionToken,
        void(boost::system::error_code, http_response)>(
            std::declval<detail::initiate_async_fetch>(), token,
                std::make_shared<std::string>(host),
                std::make_shared<std::string>(path)))
{
    return boost::asio::async_initiate<CompletionToken,
        void(boost::system::error_code, http_response)>(
            detail::initiate_async_fetch(io_context.get_executor()),
                token, std::make_shared<std::string>(host),
                std::make_shared<std::string>(path));
}

} // namespace restpp

#endif // RESTPP_FETCH_HPP
