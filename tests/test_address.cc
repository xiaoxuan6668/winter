#include "../winter/address.h"
#include "../winter/log.h"

winter::Logger::ptr g_logger = WINTER_LOG_ROOT();

void test() {
    std::vector<winter::Address::ptr> addrs;

    WINTER_LOG_INFO(g_logger) << "begin";
    bool v = winter::Address::Lookup(addrs, "www.baidu.com", AF_INET);
    WINTER_LOG_INFO(g_logger) << "end";
    if(!v) {
        WINTER_LOG_ERROR(g_logger) << "lookup fail";
        return;
    }

    for(size_t i = 0; i < addrs.size(); ++i) {
        WINTER_LOG_INFO(g_logger) << i << " - " << addrs[i]->toString();
    }
}

void test_iface() {
    std::multimap<std::string, std::pair<winter::Address::ptr, uint32_t> > results;

    bool v = winter::Address::GetInterfaceAddresses(results);
    if(!v) {
        WINTER_LOG_ERROR(g_logger) << "GetInterfaceAddresses fail";
        return;
    }

    for(auto& i: results) {
        WINTER_LOG_INFO(g_logger) << i.first << " - " << i.second.first->toString() << " - "
            << i.second.second;
    }
}

void test_ipv4() {
    //auto addr = WINTER::IPAddress::Create("www.WINTER.top");
    auto addr = winter::IPAddress::Create("127.0.0.8");
    if(addr) {
        WINTER_LOG_INFO(g_logger) << addr->toString();
    }
}

int main(int argc, char** argv) {
    test_ipv4();
    //test_iface();
    //test();
    return 0;
}