#ifndef THTTPSERVER_H
#define THTTPSERVER_H

#include <string>

class THttpServer
{
public:
    THttpServer(std::string host, uint16_t port, std::string directory);

public:
    void run();
    void operator()(int workerSocket);

protected:
    bool initSocket();
    void runLoop();

private:
    std::string m_host;
    uint16_t    m_port;
    std::string m_directory;

    int         m_socket;
};

#endif // THTTPSERVER_H
