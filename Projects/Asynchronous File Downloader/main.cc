#include <iostream>
#include <thread>
#include <boost/asio.hpp>

class FileDownloader
{
public:
    explicit FileDownloader(std::size_t argc, char* argv[]);
    void download();
private:
};

int main(int argc, char* argv[])
{
    // curler --url=url --output=dir
    FileDownloader downloader(argc, argv);
    downloader.download();
    return 0;
}