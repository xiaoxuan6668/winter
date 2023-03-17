#include "winter/http/http_server.h"
#include "winter/log.h"

winter::Logger::ptr g_logger = WINTER_LOG_ROOT();
winter::IOManager::ptr worker;
void run() {
    g_logger->setLevel(winter::LogLevel::INFO);
    winter::Address::ptr addr = winter::Address::LookupAnyIPAddress("0.0.0.0:8020");
    if(!addr) {
        WINTER_LOG_ERROR(g_logger) << "get address error";
        return;
    }

    winter::http::HttpServer::ptr http_server(new winter::http::HttpServer(true, worker.get()));
    //winter::http::HttpServer::ptr http_server(new winter::http::HttpServer(true));
    bool ssl = false;
    while(!http_server->bind(addr, ssl)) {
        WINTER_LOG_ERROR(g_logger) << "bind " << *addr << " fail";
        sleep(1);
    }
    if(ssl) {
    }

    http_server->start();
}

int main(int argc, char** argv) {
    winter::IOManager iom(1);
    worker.reset(new winter::IOManager(4, false));
    iom.schedule(run);
    return 0;
}
