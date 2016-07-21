#include "tapplication.h"
#include <getopt.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include "thttpserver.h"

TApplication::TApplication():
    m_server(NULL)
{

}

TApplication::~TApplication()
{
    if(m_server)
        delete m_server;
}

bool TApplication::init(int argc, char **argv)
{
    std::string host;
    uint16_t    port = 0;
    std::string directory;
    int option = -1;

    while( (option = getopt( argc, argv, "h:p:d:"))!=-1 ) {
        switch( option ) {
            case 'h':
                host = std::string(optarg);
                break;
            case 'p':
                port = (uint16_t)atoi(optarg);
                break;
            case 'd':
                directory = std::string(optarg);
                break;
            default:
                #if defined(DEBUG)
                    std::cout << "Error: unable to find options\n";
                #endif
                return false;
        }
    }
    #if defined(DEBUG)
        std::cout << "Options:\n\t host: \"" << host << "\"\n"
                  << "\t port: " << port << "\n"
                  << "\t directory: \"" << directory << "\"\n";
    #endif
    m_server = new THttpServer(host, port, directory);
    return true;
}

int TApplication::run()
{
    #if !defined(WITHOUT_DEMONIZATION)
        if(!demonization())
            return EXIT_SUCCESS;
    #endif
    if(m_server)
        m_server->run();

    return EXIT_SUCCESS;
}

bool TApplication::demonization()
{
    int pid = fork();
    if( pid == -1 ) {
        #if defined(DEBUG)
            std::cout << "Can not start Server Process ";
        #endif
        exit(1);
    }
    return ( pid > 0 ) ? false : true;
}



