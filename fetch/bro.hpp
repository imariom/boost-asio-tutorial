#pragma once
#ifndef BRO_FETCH_AGENT_HPP
#define BRO_FETCH_AGENT_HPP

#include <iostream>
#include <string>
#include <utility>

#include <boost/asio.hpp>

#include "http_response.hpp"

namespace bro {
namespace detail {

class initiate_async_fetch_first : public std::enable_shared_from_this<initiate_async_fetch_first>
{
public:
    using executor_type = boost::asio::io_context::executor_type;

    initiate_async_fetch_first(
        const executor_type& executor,
        const std::string& url)
        : resolver_(executor), socket_(executor), url_(url)
    {
    }

    template<typename FetchHandler>
    void operator()(FetchHandler&& handler)
    {
        auto self = shared_from_this();

        // perform DNS resolution
        resolver_.async_resolve(self->url_, "http",
            [self, handler = std::move(handler)]
                (const boost::system::error_code& ec,
                boost::asio::ip::tcp::resolver::results_type results)
            {
                if (ec) {
                    std::cerr << "DNS resolution" << std::endl;
                    handler(ec, {});
                    return; 
                }

                // connect to remote host
                boost::asio::async_connect(self->socket_, results,
                    [self, handler = std::move(handler)]
                        (const boost::system::error_code& ec,
                        const boost::asio::ip::tcp::endpoint& /*endpoint*/)
                    {
                        if (ec) {
                            std::cerr << "connecting to host" << std::endl;
                            handler(ec, {});
                            return ;
                        }

                        std::ostringstream out; 
                            out << "GET / HTTP/1.1\r\n"
                            << "Host: " << self->url_ << "\r\n"
                            << "Connection: close\r\n"
                            << "\r\n";
                        
                        // send request
                        boost::asio::async_write(self->socket_,
                            boost::asio::buffer(out.str()),
                            [self, handler = std::move(handler)]
                                (const boost::system::error_code& ec,
                                std::size_t /*bytes_transferred*/)
                            {
                                if (ec) {
                                    std::cerr << "sending request" << std::endl;
                                    handler(ec, {});
                                    return ;
                                }

                                // receive response
                                boost::asio::async_read_until(self->socket_,
                                    boost::asio::dynamic_buffer(self->response_.headers), "\r\n\r\n", // HTTP response headers
                                    [self, handler = std::move(handler)]
                                        (const boost::system::error_code& ec, std::size_t)
                                    {
                                        if (ec) {
                                            handler(ec, {});
                                            return ;
                                        }

                                        // HTTP response body
                                        boost::asio::async_read(self->socket_,
                                            boost::asio::dynamic_buffer(self->response_.body),
                                            boost::asio::transfer_at_least(1), // ensure at least some data is read
                                            [self, handler = std::move(handler)]
                                            (const boost::system::error_code& ec, std::size_t)
                                            {
                                                if (ec) {
                                                    if (ec == boost::asio::error::eof) {
                                                        // Done reading or server closed the connection
                                                        handler(ec, self->response_);
                                                        return;
                                                    } else {
                                                        handler(ec, {});
                                                        return;
                                                    }
                                                }

                                                handler(ec, self->response_);
                                            }
                                        );
                                    });
                            });

                    });

            });
    }

private:
    boost::asio::ip::tcp::resolver resolver_;
    boost::asio::ip::tcp::socket socket_;
    std::string url_;
    http_response response_;
};

class initiate_async_fetch
   : public std::enable_shared_from_this<initiate_async_fetch>
{
public:
    using executor_type = boost::asio::io_context::executor_type;

    initiate_async_fetch(
        const executor_type& executor,
        const std::string& url)
        : resolver_(executor), socket_(executor), url_(url)
    {
    }

    template<typename CompletionToken>
    auto operator()(CompletionToken&& token)
    {
        // Use async_compose to handle the composed operation
        return boost::asio::async_compose<CompletionToken,
            void(boost::system::error_code, http_response)>(
                [self = shared_from_this()](auto& self_) mutable
                {
                    // Start the composed operation
                    self->start_resolve(std::move(self_));
                }, token, resolver_, socket_);
    }

private:
    template<typename Self>
    void start_resolve(Self& self_)
    {
        // Perform DNS resolution
        resolver_.async_resolve(url_, "http",
            [this, self_ = std::move(self_)]
                (const boost::system::error_code& ec,
                boost::asio::ip::tcp::resolver::results_type results) mutable
            {
                if (ec)
                {
                    self_.complete(ec, {});
                    return;
                }

                // Move to the next step: connect to the remote host
                start_connect(std::move(self_), results);
            });
    }
    
    template<typename Self>
    void start_connect(Self& self_, boost::asio::ip::tcp::resolver::results_type results)
    {
        boost::asio::async_connect(socket_, results,
        [this, self_ = std::move(self_)]
            (const boost::system::error_code& ec,
            const boost::asio::ip::tcp::endpoint& /*endpoint*/) mutable
        {
            if (ec)
            {
                self_.complete(ec, {});
                return;
            }

            // Move to the next step: send the HTTP request
            start_send_request(std::move(self_));
        });
    }

    template<typename Self>
    void start_send_request(Self& self_)
    {
        std::ostringstream out;
        out << "GET / HTTP/1.1/\r\n"
            << "Host: " << url_ << "\r\n"
            << "Connectin: close\r\n"
            << "\r\n";

        // Send the HTTP request
        boost::asio::async_write(socket_, boost::asio::buffer(out.str()),
            [this, self_ = std::move(self_)]
            (const boost::system::error_code& ec, std::size_t /*bytes_transferred*/) mutable
            {
                if (ec)
                {
                    self_.complete(ec, {});
                    return;
                }

                // Move to the next step: receive the HTTP response headers
                start_receive_headers(std::move(self_));
            });
    }

    template<typename Self>
    void start_receive_headers(Self& self_)
    {
        // Receive the HTTP response headers
        boost::asio::async_read_until(socket_,
            boost::asio::dynamic_buffer(response_.headers), "\r\n",
            [this, self_ = std::move(self_)]
                (const boost::system::error_code& ec, std::size_t) mutable
                {
                    if (ec)
                    {
                        self_.complete(ec, {});
                        return;
                    }

                    // Move to the next step: receive the HTTP response body
                    start_receive_body(std::move(self_));
                });
    }

    template<typename Self>
    void start_receive_body(Self& self_)
    {
        boost::asio::async_read(socket_,
            boost::asio::dynamic_buffer(response_.body),
            boost::asio::transfer_at_least(1),
            [this, self_ = std::move(self_)]
                (const boost::system::error_code& ec, std::size_t) mutable
            {
                if (ec)
                {
                    if (ec == boost::asio::error::eof)
                    {
                        // Done reading or server closed the connection
                        self_.complete(ec, response_);
                        return;
                    }
                    else
                    {
                        self_.complete(ec, {});
                        return;
                    }
                }

                // Continue reading until EOF
                start_receive_body(std::move(self_));
            });
    }

private:
    boost::asio::ip::tcp::resolver resolver_;
    boost::asio::ip::tcp::socket socket_;
    std::string url_;
    http_response response_;
};

} // namespace detail
    
template <typename CompletionToken>
auto fetch(boost::asio::io_context& ioCtx,
    const std::string& url, CompletionToken&& token)
    -> decltype(boost::asio::async_initiate<CompletionToken,
        void(boost::system::error_code, http_response)>(
            std::declval<detail::initiate_async_fetch>(), token))
{
    auto fetcher = std::make_shared<detail::initiate_async_fetch>(ioCtx.get_executor(), url);

    return boost::asio::async_initiate<CompletionToken,
        void(boost::system::error_code, http_response)>(
            [fetcher](auto&& handler) mutable {
                (*fetcher)(std::forward<decltype(handler)>(handler));
            },
            token);
}

} // namespace bro

#endif // BRO_FETCH_AGENT_HPP