#ifndef TWORKER_H
#define TWORKER_H

#include <string>

class TWorker
{
    static const size_t BUFFER_SIZE = 10*1024;
public:
    TWorker(const std::string directory);
    void work(int socket);

private:
    int setNonBlock(int socket);
    bool readRequest(int socket, std::string &request);
    bool sendAnswer(int socket, std::string &answer);
    bool parseRequest(std::string &request, std::string &fileName);
    void disconnect(int socket);

    std::string createHeader(int code, size_t contextLenth = 0);
    bool getContext(std::string path, std::string& context);
    bool existFile(const std::string &fileName);
    bool readFile(const std::string &fileName, std::string& data);
    ssize_t fileSize(FILE* file);

private:
    std::string     m_directory;
};

#endif // TWORKER_H
