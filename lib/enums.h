#ifndef _ENUMS_H_
#define _ENUMS_H_

enum CommandCode
{
    CMD_FORCE_TRIGGER = 0x01,
    CMD_TRIGGER_MODE = 0x02,
    CMD_SET_RELAY = 0x03,
    CMD_CHANNEL_SELECTION = 0x04,
    CMD_TRIGGER_SOURCE = 0x05,
    CMD_CHANNEL_VOLTS_DIV_125 = 0x06,
    CMD_PRE_TRIGGER_LENGTH = 0x07,
    CMD_POST_TRIGGER_LENGTH = 0x08,
    CMD_RAM_START_POSITION = 0x09,
    CMD_RESERVED_DATA_OUTPUT = 0x0a,
    CMD_CH1_PWM = 0x0b,
    CMD_CH2_PWM = 0x0c,
    CMD_TRIGGER_PWM = 0x0d,
    CMD_LOGIC_ANALYZER_1 = 0x0e,
    CMD_LOGIC_ANALYZER_2 = 0x0f,
    CMD_LOGIC_ANALYZER_4 = 0x10,
    CMD_SAMPLE_RATE = 0x11,
    CMD_FREQ_DIV_LOW = 0x12,
    CMD_FREQ_DIV_HIGH = 0x13,
    CMD_SERIAL_BAUD_RATE = 0x14,
    CMD_RAM_CHANNEL_SELECTION = 0x15,
    CMD_READ_RAM_COUNT = 0x16
};

enum TimeBase
{
    HDIV_5nS = 0,
    HDIV_10nS = 1,
    HDIV_20nS = 2,
    HDIV_50nS = 3,
    HDIV_100nS = 4,
    HDIV_200nS = 5,
    HDIV_500nS = 6,
    HDIV_1uS = 7,
    HDIV_2uS = 8,
    HDIV_5uS = 9,
    HDIV_10uS = 10,
    HDIV_20uS = 11,
    HDIV_50uS = 12,
    HDIV_100uS = 13,
    HDIV_200uS = 14,
    HDIV_500uS = 15,
    HDIV_1mS = 16,
    HDIV_2mS = 17,
    HDIV_5mS = 18,
    HDIV_10mS = 19,
    HDIV_20mS = 20,
    HDIV_50mS = 21,
    HDIV_100mS = 22,
    HDIV_200mS = 23,
    HDIV_500mS = 24,
    HDIV_1S = 25,
    HDIV_2S = 26,
    HDIV_5S = 27,
    HDIV_10S = 28,
    HDIV_20S = 29,
    HDIV_50S = 30,
    HDIV_100S = 31,
    HDIV_200S = 32,
    HDIV_500S = 33
};

enum VoltageDiv
{
    VDIV_10mV,
    VDIV_20mV,
    VDIV_50mV,
    VDIV_100mV,
    VDIV_200mV,
    VDIV_500mV,
    VDIV_1V,
    VDIV_2V,
    VDIV_5V
};

enum InputCoupling
{
    COUPLING_AC,
    COUPLING_DC,
    COUPLING_GND
};

enum CaptureMode
{
    CAPMODE_ROLL,
    CAPMODE_SCAN,
    CAPMODE_NORMAL
};

enum ScopeMode
{
    SCOMODE_ANALOG,
    SCOMODE_DIGITAL
};

enum TriggerChannel
{
    TRIGCHAN_CH1,
    TRIGCHAN_CH2,
    TRIGCHAN_EXT,
    TRIGCHAN_EXT10,
    TRIGCHAN_ALT
};

enum TriggerMode
{
    TRIGMODE_AUTO,
    TRIGMODE_NORMAL,
    TRIGMODE_SINGLE
};

enum TriggerSlope
{
    TRIGSLOPE_RISING,
    TRIGSLOPE_FALLING
};

#endif // _ENUMS_H_