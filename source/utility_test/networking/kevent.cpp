//
//  kevent.cpp
//  utility_test
//
//  Created by volodja on 14.05.2018.
//


#include <gtest/gtest.h>
#include <utility/logging/logger.h>


using utility::logging::update;
using utility::logging::config::createDefaultConsoleLoggerConfig;


TEST(example, kevent) {
    update(createDefaultConsoleLoggerConfig());
    
    LOGT << "begin";
    
    LOGT << "end";
}
