#include "util.h"

namespace winter {

    pid_t GetThreadId() {
        return syscall(SYS_gettid);
    }

    uint32_t GetFiberId() {
        return 0;
    }

}