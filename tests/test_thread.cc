#include "../winter/winter.h"
#include <unistd.h>

winter::Logger::ptr g_logger = WINTER_LOG_ROOT();

int count = 0;
// winter::RWMutex s_mutex;
winter::Mutex s_mutex;

void fun1() {
    WINTER_LOG_INFO(g_logger) << "name: " << winter::Thread::GetName()
                             << " this.name: " << winter::Thread::GetThis()->getName()
                             << " id: " << winter::GetThreadId()
                             << " this.id: " << winter::Thread::GetThis()->getId();
    for(int i = 0; i < 100000; ++i) {
        // winter::RWMutex::WriteLock lock(s_mutex);
        winter::Mutex::Lock lock(s_mutex);
        ++count;
    }
}

void fun2() {
    while(true) {
        WINTER_LOG_INFO(g_logger) << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
    }
}

void fun3() {
    while(true) {
        WINTER_LOG_INFO(g_logger) << "========================================";
    }
}

int main(int argc, char** argv) {
    WINTER_LOG_INFO(g_logger) << "thread test begin";
    YAML::Node root = YAML::LoadFile("/home/ghx/MyProjects/cpp/winter/bin/conf/log2.yml");
    winter::Config::LoadFromYaml(root);

    std::vector<winter::Thread::ptr> thrs;
    for(int i = 0; i < 2; ++i) {
        winter::Thread::ptr thr(new winter::Thread(&fun2, "name_" + std::to_string(i * 2)));
        winter::Thread::ptr thr2(new winter::Thread(&fun3, "name_" + std::to_string(i * 2 + 1)));
        thrs.push_back(thr);
        thrs.push_back(thr2);
    }

    for(size_t i = 0; i < thrs.size(); ++i) {
        thrs[i]->join();
    }
    
    WINTER_LOG_INFO(g_logger) << "thread test end";
    WINTER_LOG_INFO(g_logger) << "count=" << count;
    return 0;
}