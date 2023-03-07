#include "../winter/winter.h"

static winter::Logger::ptr g_logger = WINTER_LOG_ROOT();

void test_fiber() {
    WINTER_LOG_INFO(g_logger) << "test in fiber";
}

int main(int argc, char** argv) {
    WINTER_LOG_INFO(g_logger) << "main";
    winter::Scheduler sc;
    sc.schedule(&test_fiber);
    sc.start();
    WINTER_LOG_INFO(g_logger) << "schedule";
    sc.stop();
    WINTER_LOG_INFO(g_logger) << "over";
    return 0;
}