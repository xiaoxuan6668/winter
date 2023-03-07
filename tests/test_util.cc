#include "../winter/winter.h"
#include <assert.h>

winter::Logger::ptr g_logger = WINTER_LOG_ROOT();

void test_assert() {
    WINTER_LOG_INFO(g_logger) << winter::BacktraceToString(10);
    WINTER_ASSERT2(0 == 1, "abcdef xx");
}

int main(int argc, char** argv) {
    test_assert();
    return 0;
}