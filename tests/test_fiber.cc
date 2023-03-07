#include "../winter/winter.h"

winter::Logger::ptr g_logger = WINTER_LOG_ROOT();

void run_in_fiber() {
    WINTER_LOG_INFO(g_logger) << "run_in_fiber begin";
    winter::Fiber::YieldToHold();
    WINTER_LOG_INFO(g_logger) << "run_in_fiber end";
    winter::Fiber::YieldToHold();
}

void test_fiber() {
    WINTER_LOG_INFO(g_logger) << "main begin -1";
    {
        winter::Fiber::GetThis();
        WINTER_LOG_INFO(g_logger) << "main begin";
        winter::Fiber::ptr fiber(new winter::Fiber(run_in_fiber));
        fiber->swapIn();
        WINTER_LOG_INFO(g_logger) << "main after swapIn";
        fiber->swapIn();
        WINTER_LOG_INFO(g_logger) << "main after end";
        fiber->swapIn();
    }
    WINTER_LOG_INFO(g_logger) << "main after end2";
}

int main(int argc, char** argv) {
    winter::Thread::SetName("main");

    std::vector<winter::Thread::ptr> thrs;
    for(int i = 0; i < 3; ++i) {
        thrs.push_back(winter::Thread::ptr(
                    new winter::Thread(&test_fiber, "name_" + std::to_string(i))));
    }
    for(auto i : thrs) {
        i->join();
    }
    return 0;
}
