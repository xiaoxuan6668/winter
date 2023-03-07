#include <iostream>
#include "../winter/log.h"
#include "../winter/util.h"

int main(int argc, char** argv){
    winter::Logger::ptr logger(new winter::Logger);
    logger->addAppender(winter::LogAppender::ptr(new winter::StdoutLogAppender));
    
    winter::FileLogAppender::ptr file_appender(new winter::FileLogAppender("./test.txt"));
    winter::LogFormatter::ptr fmt(new winter::LogFormatter("%d%T%p%T%m%n"));
    file_appender->setFormatter(fmt);
    file_appender->setLevel(winter::LogLevel::ERROR);
    logger->addAppender(file_appender);

    //winter::LogEvent::ptr event(new winter::LogEvent(__FILE__,__LINE__, 0, 1, 2, time(0)));
    //logger->log(winter::LogLevel::DEBUG,event);

    WINTER_LOG_INFO(logger) << "test macro";
    WINTER_LOG_ERROR(logger) << "test macro error";

    WINTER_LOG_FMT_ERROR(logger, "test macro fmt error %s", "aa");
    
    auto l = winter::LoggerMgr::GetInstance()->getLogger("xx");
    WINTER_LOG_INFO(l) << "xxx";
    return 0;
}