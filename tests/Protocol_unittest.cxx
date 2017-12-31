#include "gtest/gtest.h"

#include "Protocol.h"
#include "TestConnector.h"

namespace
{

TEST(ProtocolTest, parseRamChannelSelection)
{
    TestConnector connection;
    Protocol proto(&connection);
    proto.start();
    uint8_t packetPayload[IDSO1070A_PACKET_SIZE] = {
        0xff, 0x01, 0x02, 0x55, 0x15, 0x00, 0x00, 0x60,
        0x04, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x01,
        0x06, 0x00, 0x05, 0x01, 0x08, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a,
        0x5a, 0x5a, 0x5a, 0x5a, 0xce};

    connection.fakePacket(packetPayload);
    proto.receive();
    EXPECT_EQ(proto.getDevice().channel1.enabled, true);
    EXPECT_EQ(proto.getDevice().channel2.enabled, true);
}

TEST(ProtocolTest, parseEEROMPage05)
{
    TestConnector connection;
    Protocol proto(&connection);
    proto.start();
    uint8_t packetPayload[IDSO1070A_PACKET_SIZE] = {
        0xff, 0x01, 0xfa, 0xee, 0xaa, 0x05, 0x00, 0x49,
        0x44, 0x53, 0x4f, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x49, 0x44, 0x53, 0x4f, 0x31,
        0x30, 0x37, 0x30, 0x41, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36,
        0x31, 0x32, 0x30, 0x30, 0x30, 0x35, 0x30, 0x33,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50,
        0x31, 0x2e, 0x30, 0x37, 0x00, 0x44, 0x31, 0x2e,
        0x30, 0x31, 0x00, 0x4d, 0x30, 0x30, 0x31, 0x43,
        0x30, 0x30, 0x31, 0x32, 0x30, 0x31, 0x37, 0x30,
        0x36, 0x32, 0x37, 0x32, 0x30, 0x31, 0x37, 0x30,
        0x36, 0x32, 0x37, 0x54, 0x30, 0x30, 0x31, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xa5, 0x42, 0xc4, 0x39, 0x7a,
        0x3a, 0xc0, 0x4c, 0x1a, 0xa5, 0x59, 0x21, 0xbc,
        0x71, 0x41, 0x60, 0xad, 0xdd, 0xbc, 0xfe, 0xd6,
        0x82, 0x60, 0x0c, 0x00, 0x1e, 0xef, 0xc7, 0x70,
        0x40, 0x4f, 0x6a, 0x6c, 0xff, 0x91, 0x08, 0x31,
        0x4e, 0x24, 0xcb, 0xf7, 0x32, 0x60, 0xbe, 0x5a,
        0x9e, 0x37, 0x40, 0x80, 0xc8, 0x8d, 0x4e, 0x9e,
        0x2a, 0x86, 0x16, 0x34, 0x41, 0x74, 0x8d, 0xd4,
        0x80, 0xa1, 0x82, 0xbd, 0x73, 0x10, 0x12, 0x41,
        0xf6, 0xc4, 0x03, 0xcc, 0x17, 0x92, 0xc7, 0x1e,
        0x5b, 0x30, 0x61, 0xa0, 0x14, 0x93, 0xcb, 0xac,
        0xb3, 0x46, 0x2a, 0x9c, 0x3f, 0x6c, 0x15, 0xfe,
        0x93, 0xe8, 0x69, 0xa2, 0x52, 0x35, 0xa8, 0x20,
        0xd4, 0xef, 0x66, 0xb4, 0x43, 0x82, 0x7f, 0x28,
        0xce, 0x8e, 0x25, 0x01, 0xd1, 0x2b, 0x9e, 0x7d,
        0x4d, 0x4b, 0xb6, 0xbe, 0xa5, 0xcf, 0x50, 0xec,
        0x2e, 0x93, 0xb3, 0xcb, 0xfe, 0xc6, 0xcc, 0xb8,
        0xe9, 0x73, 0x65, 0xf6, 0x45, 0x93, 0x3e, 0xb2,
        0xc4, 0x0d, 0x71, 0xed, 0x58, 0x3e, 0xe3, 0xfe,
        0x01, 0x9f, 0x12, 0xec, 0x69, 0xc0, 0xec, 0x21,
        0x52, 0xc2, 0x13, 0xa5, 0x59, 0xb3, 0xda, 0xd1,
        0x28, 0x48, 0x94, 0x2c, 0x38, 0xe2, 0x30, 0x75,
        0xf7, 0x20, 0x22, 0x6c, 0x9b, 0x74, 0xc5, 0x75,
        0x83, 0x4d, 0x42, 0xe8, 0x3c, 0xe3, 0x09, 0x21,
        0xe9, 0xe0, 0xb1, 0xa3, 0xba, 0xf9, 0x36, 0x3d,
        0x6d, 0x49, 0x6a, 0x05, 0x3f, 0xd6, 0xf4, 0x7d,
        0xe9, 0x09, 0x26, 0xfd, 0xed, 0x67, 0xa4, 0xa5,
        0x84, 0x25, 0xd7, 0x18, 0x3c, 0x17, 0x8f, 0x7d,
        0x6a, 0x43, 0x23, 0x93, 0xfe, 0x9e, 0x3a, 0x35,
        0x16, 0x3a, 0x9b, 0x79, 0x3e, 0xcb, 0xc8, 0x7b,
        0x16, 0x93, 0x4a, 0x26, 0x0f, 0xe1, 0x01, 0x53,
        0xa5, 0x4b, 0xb2, 0xf6, 0x3d, 0xb5, 0x8a, 0x71,
        0xc6, 0x53, 0x3e, 0x79, 0x7a, 0x23, 0x17, 0x77,
        0x7a, 0xe7, 0x05, 0xe9, 0xd7, 0x6a, 0xd2, 0xe4,
        0x10, 0x88, 0xdb, 0xd0, 0x0f, 0x77, 0x19, 0x79,
        0xc6, 0x3f, 0x11, 0x8c, 0x6f, 0x9f, 0xe6, 0x94,
        0x2a, 0x5a, 0xae, 0x6e, 0x69, 0xab, 0x9e, 0x85,
        0x92, 0x44, 0x03, 0x84, 0x55, 0x7e, 0xed, 0x6b,
        0x79, 0x10, 0x78, 0xc1, 0xab, 0xbe, 0x68, 0x85,
        0xec, 0xa6, 0x3a, 0x1c, 0xd8, 0x25, 0x26, 0x84,
        0xe4, 0xdf, 0xc6, 0xa5, 0x46, 0x8a, 0xce, 0xd1,
        0xb7, 0xee, 0x82, 0xc2, 0xb6, 0x3d, 0x88, 0x22,
        0x13, 0xc6, 0xeb, 0xdf, 0xf7, 0x08, 0x33, 0xd1,
        0xe9, 0x48, 0x24, 0x2c, 0x51, 0xd6, 0x60, 0xb5,
        0x00, 0xfb, 0x17, 0x26, 0xdd, 0x9e, 0x7a, 0x77,
        0x58, 0x8e, 0x03, 0xf7, 0x36, 0xd9, 0xaa, 0xea,
        0x76, 0xf6, 0x1c, 0x4b, 0x30, 0x83, 0x4b, 0xfd,
        0x71, 0x3e, 0xa1, 0x1d, 0x5a, 0xc1, 0x37, 0x4c,
        0x78, 0x70, 0x6f, 0x33, 0xf5, 0xf1, 0xe6, 0xde,
        0x93, 0xfa, 0xc0, 0xfe, 0xbd, 0x0a, 0xbc, 0x42,
        0xac, 0x92, 0x34, 0xd5, 0xce};

    connection.fakePacket(packetPayload);
    proto.receive();
    EXPECT_EQ(strcmp((char *)proto.getEEROMData().productName, "IDSO1070A"), 0);
    EXPECT_EQ(strcmp((char *)proto.getEEROMData().userName, "IDSO"), 0);
}

} // namespace