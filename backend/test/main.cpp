#include <gtest/gtest.h>
#include "libmrock/common/src/my_log.h"

log4cplus::Logger logger;

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    logger = getLogger("test");
    return RUN_ALL_TESTS();
}
