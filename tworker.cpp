#include "tworker.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <iostream>
#include <unistd.h>

TWorker::TWorker(std::string directory ):
    m_directory(directory)
{

}

void TWorker::work(int socket)
{
    std::string request;
    std::string fileName;
   // setNonBlock(socket);

    if( readRequest(socket, request) ) {
        #if defined(DEBUG)
            std::cout << "\n\nReqest:\n" << request;
        #endif
        std::string answer;
        if( parseRequest(request, fileName) ) {
            std::string context;
            if( getContext(fileName, context) ) {
                answer = createHeader(200, context.size() ) + context;
            }
            else {
                answer = createHeader(404);
            }
        }
        else {
            answer = createHeader(500);
        }
        #if defined(DEBUG)
            std::cout << "\n\nAnswer:\n" << answer;
        #endif
        sendAnswer(socket, answer);
    }
    disconnect(socket);
}

int TWorker::setNonBlock(int socket) {
    int flags = 1;
    return ioctl(socket, FIONBIO, &flags);
}

bool TWorker::readRequest(int socket, std::string& request)
{
//    char buffer[1024];
//    int size;
//    errno = 0;
//    while((size = recv(socket, buffer, sizeof(buffer), MSG_NOSIGNAL)) > 0) || errno == EINTR)
//    {
//        if(size > 0) {
//            request.append(buffer, size);
//        }
//        if(size < 0){
//            return false; /* handle error - for example throw an exception*/
//        }
//    }

    char buffer[BUFFER_SIZE];
    if( recv(socket, buffer, BUFFER_SIZE, MSG_NOSIGNAL) > 0 ) {
        request = std::string(buffer);
    }
    return true;
}

bool TWorker::sendAnswer(int socket, std::string &answer)
{
    send(socket, answer.c_str(), answer.size(), 0);
}

bool TWorker::parseRequest(std::string &request, std::string &fileName)
{
    fileName = request.substr(4, request.find("HTTP/1.")-5);
    size_t pos = fileName.find("?");
    if( pos != std::string::npos )
        fileName = fileName.substr(0,pos);
    #if defined(DEBUG)
        std::cout << "\n\nFileName: \"" << fileName << "\"\n";
    #endif
    return true;
}

void TWorker::disconnect(int socket)
{
    shutdown(socket, SHUT_RDWR);
    close(socket);
}

std::string TWorker::createHeader(int code, size_t contextLenth)
{
    std::string header = "HTTP/1.0 ";
    switch(code) {
        case 200:
            header += "200 OK\r\nContent-length: " + std::to_string(contextLenth);
            break;
        case 404:
            header += "404 File Not Found\r\nContent-length: 0";
            break;
        default:
            header += "500 Internal Server Error";
            break;
    }
    header += "\r\nContent-Type: text/html\r\n\r\n";
    return header;
}

bool TWorker::getContext(std::string path, std::string &context)
{
    if(path.empty())
        return false;
    std::string fileName = (path[0] == '/') ? m_directory + path : m_directory + "/" + path;
    return readFile(fileName, context);
}

bool TWorker::readFile(const std::string &fileName, std::string &data)
{
    #if defined(DEBUG)
        std::cout << "\n\nRead File: \"" << fileName << "\"\n";
    #endif
    FILE *file = fopen(fileName.c_str(), "r");
    if(file == NULL )
        return false;
    ssize_t size = fileSize(file);
    char* buffer = new char[size];
    fread(buffer, sizeof(char), size, file);
    data = std::string(buffer);
    delete buffer;
    return true;
}

ssize_t TWorker::fileSize(FILE *file)
{
    ssize_t lastSize = ftell(file);
    fseek(file, 0, SEEK_END);
    ssize_t size = ftell(file);
    fseek(file, lastSize, SEEK_SET);
    return size;
}

