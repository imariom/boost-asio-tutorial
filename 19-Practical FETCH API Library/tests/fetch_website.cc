#include <gtest/gtest.h>
#include <boost/asio.hpp>

#include <restpp/fetch.hpp>
#include <restpp/http_response.hpp>

using namespace restpp;

class FetchTest : public ::testing::Test
{
protected:
    boost::asio::io_context io_ctx;
    std::string test_host = "www.google.com";
    std::string test_path = "/";
};

TEST_F(FetchTest, SuccessfulFetch)
{
    auto handler = [](const boost::system::error_code& ec,
        restpp::http_response response)
    {
        EXPECT_FALSE(ec);  // Expect no error
        EXPECT_FALSE(response.headers.empty());
        EXPECT_FALSE(response.body.empty());
    };

    restpp::fetch(io_ctx, test_host, test_path, handler);
    io_ctx.run();
}

TEST_F(FetchTest, ConnectionFailure)
{
    auto handler = [](const boost::system::error_code& ec,
        restpp::http_response response)
    {
        EXPECT_TRUE(ec);  // Expect an error
        EXPECT_TRUE(response.headers.empty());
        EXPECT_TRUE(response.body.empty());
    };

    restpp::fetch(io_ctx, "invalid-host", test_path, handler);
    io_ctx.run();
}

TEST_F(FetchTest, InvalidResponseFormat)
{
    auto handler = [](const boost::system::error_code& ec,
        restpp::http_response response)
    {
        EXPECT_TRUE(ec);  // Expect an error due to malformed response
        EXPECT_TRUE(response.headers.empty());
    };

    restpp::fetch(io_ctx, "badserver.com", test_path, handler);
    io_ctx.run();
}