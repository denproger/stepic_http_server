#include "thttpserver.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <thread>
#include <functional>
#include "tworker.h"

THttpServer::THttpServer(std::string host, uint16_t port, std::string directory):
    m_host(host), m_port(port), m_directory(directory)
{

}

void THttpServer::run()
{
    if(!initSocket())
        return;
    runLoop();
}

void THttpServer::operator()(int workerSocket)
{
    TWorker worker(m_directory);
    worker.work(workerSocket);
}

bool THttpServer::initSocket()
{
    m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if( m_socket == -1 ) {
        #if defined(DEBUG)
            std::cout << "Error: unable to create socket" << std::endl;
        #endif
        return false;
    }
    sockaddr_in socketAddress;
    socketAddress.sin_family= AF_INET;
    socketAddress.sin_port = htons(m_port);
    inet_pton(AF_INET, m_host.c_str(), &(socketAddress.sin_addr));

    if( bind(m_socket, (struct sockaddr*)&socketAddress , sizeof(socketAddress)) < 0 ) {
        #if defined(DEBUG)
            std::cout << "Error: unable to bind socket" << std::endl;
        #endif
        return false;
    }
    if( listen(m_socket, SOMAXCONN) < 0 ) {
        #if defined(DEBUG)
            std::cout << "Error: unable to bind socket" << std::endl;
        #endif
        return false;
    }
    #if defined(DEBUG)
        std::cout << "Init Socket" << std::endl;
    #endif
    return true;
}       

void THttpServer::runLoop()
{
    while (true) {
        int workerSocket = accept(m_socket, 0, 0);
        std::thread thread(*this, workerSocket);
        thread.detach();
    }
}




