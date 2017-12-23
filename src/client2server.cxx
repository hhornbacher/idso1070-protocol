// RAM Channel Selection
char peer0_2[] = {/* Packet 4 */
                  0x55, 0x15, 0x00, 0x00};
char peer0_3[] = {/* Packet 7 */
                  0x55, 0x15, 0x00, 0x00};

// getReadEEROMCommands
char peer0_4[] = {/* Packet 9 */
                  0xee, 0xaa, 0x00, 0x00};
char peer0_5[] = {/* Packet 12 */
                  0xee, 0xaa, 0x01, 0x00};
char peer0_6[] = {/* Packet 16 */
                  0xee, 0xaa, 0x04, 0x00};
char peer0_7[] = {/* Packet 21 */
                  0xee, 0xaa, 0x05, 0x00};
char peer0_8[] = {/* Packet 23 */
                  0xee, 0xaa, 0x06, 0x00};
char peer0_9[] = {/* Packet 27 */
                  0xee, 0xaa, 0x07, 0x00};
char peer0_10[] = {/* Packet 31 */
                   0xee, 0xaa, 0x08, 0x00};
char peer0_11[] = {/* Packet 36 */
                   0xee, 0xaa, 0x09, 0x00};
char peer0_12[] = {/* Packet 40 */
                   0xee, 0xaa, 0x0a, 0x00};
char peer0_13[] = {/* Packet 44 */
                   0xee, 0xaa, 0x0b, 0x00};
char peer0_14[] = {/* Packet 48 */
                   0xee, 0xaa, 0x0c, 0x00};
char peer0_15[] = {/* Packet 53 */
                   0xee, 0xaa, 0x0f, 0x00};

// 0x17 Unknown CommandCode
char peer0_16[] = {/* Packet 57 */
                   0x55, 0x17, 0x00, 0x00};
char peer0_17[] = {/* Packet 61 */
                   0x55, 0x17, 0x00, 0x00};
char peer0_18[] = {/* Packet 64 */
                   0x55, 0x17, 0x00, 0x00};
char peer0_19[] = {/* Packet 68 */
                   0x55, 0x17, 0x00, 0x00};
char peer0_20[] = {/* Packet 71 */
                   0x55, 0x17, 0x00, 0x00};
char peer0_21[] = {/* Packet 74 */
                   0x55, 0x17, 0x00, 0x00};
char peer0_22[] = {/* Packet 78 */
                   0x55, 0x17, 0x00, 0x00};
char peer0_23[] = {/* Packet 81 */
                   0x55, 0x17, 0x00, 0x00};
char peer0_24[] = {/* Packet 86 */
                   0x55, 0x17, 0x00, 0x00};
char peer0_25[] = {/* Packet 89 */
                   0x55, 0x17, 0x00, 0x00};

// getReadFpgaVersionCommand
char peer0_26[] = {/* Packet 92 */
                   0xaa, 0x02, 0x00, 0x00};

// Probably getDate --> response = (hex) 32 30 31 36 30 36 30 38 | (ascii) 20160608
char peer0_27[] = {/* Packet 94 */
                   0x57, 0x04, 0x00, 0x00};

// Current working position
// maybe keep alive..?
char peer0_28[] = {/* Packet 96 */
                   0x57, 0x03, 0x00, 0x00};

// getTimebaseCommands
char peer0_29[] = {/* Packet 98 */
                   0x55, 0x11, 0x00, 0x00};
char peer0_30[] = {/* Packet 100 */
                   0x55, 0x12, 0x04, 0x00};
char peer0_31[] = {/* Packet 102 */
                   0x55, 0x13, 0x00, 0x00};

// getTriggerSourceCommands
char peer0_32[] = {/* Packet 104 */
                   0x55, 0x04, 0x02, 0x00};
char peer0_33[] = {/* Packet 106 */
                   0x55, 0x05, 0x91, 0x00};

// TRIGGER_PWM
char peer0_34[] = {/* Packet 108 */
                   0x55, 0x0d, 0xb5, 0x0a};
// PRE_TRIGGER_LENGTH
char peer0_35[] = {/* Packet 110 */
                   0x55, 0x07, 0xed, 0x03};
// POST_TRIGGER_LENGTH
char peer0_36[] = {/* Packet 112 */
                   0x55, 0x08, 0xe8, 0x03};

// READ_RAM_COUNT
char peer0_37[] = {/* Packet 114 */
                   0x55, 0x16, 0xd0, 0x77};
// LOGIC_ANALYZER_1
char peer0_38[] = {/* Packet 116 */
                   0x55, 0x0e, 0x92, 0x24};
// LOGIC_ANALYZER_2
char peer0_39[] = {/* Packet 118 */
                   0x55, 0x0f, 0x49, 0x92};
// LOGIC_ANALYZER_3
char peer0_40[] = {/* Packet 122 */
                   0x55, 0x10, 0x24, 0x49};
// RAM_CHANNEL_SELECTION
char peer0_41[] = {/* Packet 124 */
                   0x55, 0x15, 0x00, 0x00};
// SET_RELAY
char peer0_42[] = {/* Packet 126 */
                   0x55, 0x03, 0x80, 0x00};
// SET_RELAY
char peer0_43[] = {/* Packet 128 */
                   0x55, 0x03, 0x02, 0x00};
// CHANNEL_VOLTS_DIV_125
char peer0_44[] = {/* Packet 130 */
                   0x55, 0x06, 0x00, 0x00};
// CH1_PWM
char peer0_45[] = {/* Packet 132 */
                   0x55, 0x0b, 0x3e, 0x08};

// SET_RELAY
char peer0_46[] = {/* Packet 134 */
                   0x55, 0x03, 0xef, 0x00};
// CHANNEL_VOLTS_DIV_125
char peer0_47[] = {/* Packet 136 */
                   0x55, 0x06, 0x00, 0x00};
// SET_RELAY
char peer0_48[] = {/* Packet 138 */
                   0x55, 0x03, 0x40, 0x00};
// SET_RELAY
char peer0_49[] = {/* Packet 140 */
                   0x55, 0x03, 0x04, 0x00};
// CHANNEL_VOLTS_DIV_125
char peer0_50[] = {/* Packet 142 */
                   0x55, 0x06, 0x00, 0x00};

// CH2_PWM
char peer0_51[] = {/* Packet 144 */
                   0x55, 0x0c, 0x7d, 0x05};

// SET_RELAY
char peer0_52[] = {/* Packet 147 */
                   0x55, 0x03, 0xfe, 0x00};

// CHANNEL_VOLTS_DIV_125
char peer0_53[] = {/* Packet 149 */
                   0x55, 0x06, 0x00, 0x00};

// TRIGGER_MODE
char peer0_54[] = {/* Packet 151 */
                   0x55, 0x02, 0x02, 0x00};

char peer0_55[] = {/* Packet 153 */
                   0xaa, 0x04, 0x00, 0x00};

// LOGIC_ANALYZER_2
char peer0_56[] = {/* Packet 899 */
                   0x55, 0x15, 0x00, 0x00};
char peer0_57[] = {/* Packet 903 */
                   0x55, 0x15, 0x00, 0x00};