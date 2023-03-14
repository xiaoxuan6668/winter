#include "../winter/daemon.h"
#include "../winter/iomanager.h"
#include "../winter/log.h"

static winter::Logger::ptr g_logger = WINTER_LOG_ROOT();

winter::Timer::ptr timer;
int server_main(int argc, char** argv) {
    WINTER_LOG_INFO(g_logger) << winter::ProcessInfoMgr::GetInstance()->toString();
    winter::IOManager iom(1);
    timer = iom.addTimer(1000, [](){
            WINTER_LOG_INFO(g_logger) << "onTimer";
            static int count = 0;
            if(++count > 10) {
                exit(1);
            }
    }, true);
    return 0;
}

int main(int argc, char** argv) {
    return winter::start_daemon(argc, argv, server_main, argc != 1);
}