#ifndef TAPPLICATION_H
#define TAPPLICATION_H
#include <string>

class THttpServer;
class TApplication
{
public:
    TApplication();
    ~TApplication();

public:

    bool init(int argc, char **argv);
    int run();
protected:
    bool demonization();

private:    
    THttpServer*    m_server;
};

#endif // TAPPLICATION_H
