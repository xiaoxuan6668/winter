#include "../winter/tcp_server.h"
#include "../winter/iomanager.h"
#include "../winter/log.h"

winter::Logger::ptr g_logger = WINTER_LOG_ROOT();

void run() {
    auto addr = winter::Address::LookupAny("0.0.0.0:8033");
    //auto addr2 = winter::UnixAddress::ptr(new winter::UnixAddress("/tmp/unix_addr"));
    std::vector<winter::Address::ptr> addrs;
    addrs.push_back(addr);
    //addrs.push_back(addr2);

    winter::TcpServer::ptr tcp_server(new winter::TcpServer);
    std::vector<winter::Address::ptr> fails;
    while(!tcp_server->bind(addrs, fails)) {
        sleep(2);
    }
    tcp_server->start();
    
}
int main(int argc, char** argv) {
    winter::IOManager iom(2);
    iom.schedule(run);
    return 0;
}
