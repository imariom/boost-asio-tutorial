
namespace chatito {

class ChatServerManager
{
public:
    void createChatServer();
    void stopChatServer();
    void listChatServers();
    void listChatClients();

private:
    void queueOperation();
};
    
}