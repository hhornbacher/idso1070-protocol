# About the Protocol

The protocol itself is based on a simple request-response flow with the packets described here.

## Request (Command) packet

This packet has the fixed size of 4 bytes

Address|Size (byte)|Description
-|-|-
`0x0000`|1|Command type/category
`0x0001`|1|Command code
`0x0002`|2|Parameters (can be one or two bytes, padded with zero)

### Commands

Type|=|Command|=|Parameters
-|-|-|-|-
STATE|`0x57`|Read battery level|`0x03`|`0000`
STATE|`0x57`|Read ARM firmware version|`0x04`|`0000`
CONTROL|`0xaa`|Read FPGA firmware version|`0x02`|`0000`
CONTROL|`0xaa`|Start sampling|`0x04`|`0000`
EEROM|`0xee`|Read EEROM page|`0xaa`|first byte: 8bit address of the page to read; second byte: `00`
FPGA|`0x55`|Force trigger|`0x01`|*TBD*
FPGA|`0x55`|Trigger mode|`0x02`|*TBD*
FPGA|`0x55`|Set relay|`0x03`|*TBD*
FPGA|`0x55`|Channel selection|`0x04`|*TBD*
FPGA|`0x55`|Trigger source|`0x05`|*TBD*
FPGA|`0x55`|Channel volts div 125|`0x06`|*TBD*
FPGA|`0x55`|Pre trigger length|`0x07`|*TBD*
FPGA|`0x55`|Post trigger length|`0x08`|*TBD*
FPGA|`0x55`|RAM start position|`0x09`|*TBD*
FPGA|`0x55`|Reserved data output|`0x0a`|*TBD*
FPGA|`0x55`|Channel 1 PWM|`0x0b`|*TBD*
FPGA|`0x55`|Channel 2 PWM|`0x0c`|*TBD*
FPGA|`0x55`|Trigger PWM|`0x0d`|*TBD*
FPGA|`0x55`|Logic analyzer 1|`0x0e`|*TBD*
FPGA|`0x55`|Logic analyzer 2|`0x0f`|*TBD*
FPGA|`0x55`|Logic analyzer 3|`0x10`|*TBD*
FPGA|`0x55`|Sample rate|`0x11`|*TBD*
FPGA|`0x55`|Frequency divider lower 2 bytes|`0x12`|*TBD*
FPGA|`0x55`|Frequency divider higher 2 bytes|`0x13`|*TBD*
FPGA|`0x55`|Serial baud rate|`0x14`|*TBD*
FPGA|`0x55`|RAM channel selection|`0x15`|*TBD*
FPGA|`0x55`|Read RAM count|`0x16`|*TBD*

## Response packet

This packet has the fixed size of 509 bytes.

Section|Address|Size (byte)|Description
-|-|-|-
HEADER|`0x0000`|2|HEADER: Magic bytes: `0xff 0x01`
||`0x0002`|1|HEADER: 8bit packet counter
||`0x0003`|4|HEADER: Bytes of the command packet, this response is addressed to
PAYLOAD|`0x0007`|501|PAYLOAD
END|`0x01FC`|1|Magic byte: `0xce`

## Connection

There are two options connection to the device:

* USB: Device connects over USB-Serial connection (maximum baud rate not yet known)
* WiFi: When you're connected to the device's access point, there is a TCP server listening on `192.186.1.1:8870`