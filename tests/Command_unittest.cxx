#include "gtest/gtest.h"
#include "Command.h"

namespace
{

TEST(CommandTest, UserDefinedValid)
{
    uint8_t data[4] = {};
    Command c(data);
    EXPECT_EQ(memcmp(data, c.getPayload(), 4), 0);
}

} // namespace