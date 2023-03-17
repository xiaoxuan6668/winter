#ifndef __WINTER_APPLICATION_H__
#define __WINTER_APPLICATION_H__

#include "winter/http/http_server.h"

namespace winter {

class Application {
public:
    Application();

    static Application* GetInstance() { return s_instance;}
    bool init(int argc, char** argv);
    bool run();
private:
    int main(int argc, char** argv);
    int run_fiber();
private:
    int m_argc = 0;
    char** m_argv = nullptr;

    std::vector<winter::http::HttpServer::ptr> m_httpservers;
    IOManager::ptr m_mainIOManager;
    static Application* s_instance;
};

}

#endif