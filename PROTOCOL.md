# Acer Projector Serial Protocol Documentation

This document outlines the RS232 protocol used for controlling Acer H7550BD projectors based on the command spreadsheet.

## Serial Port Configuration

- Baud Rate: 9600
- Data Bits: 8
- Parity: None
- Stop Bits: 1
- Flow Control: None

## Command Format

Commands sent to the projector follow this format:

```
<CR>*<addr><data><CR>
```

Where:
- `<CR>` is the carriage return character (0x0D)
- `*` is the command marker
- `<addr>` is the 2-digit command code in hexadecimal (e.g., `00` for power status)
- `<data>` is the 2-digit data value in hexadecimal

Example: To turn the projector on, send: `<CR>*0001<CR>`

## Response Format

Responses from the projector follow this format:

```
<CR>P<command><length><data...><checksum><CR>
```

Where:
- `<CR>` is the carriage return character (0x0D)
- `P` indicates a successful response
- `<command>` is the command code that was sent
- `<length>` is the length of the data field
- `<data...>` is the response data (varies based on command)
- `<checksum>` is a checksum byte

## Supported Commands

The Acer H7550BD projector supports many commands as shown in the spreadsheet. Here are the primary ones implemented in this component:

### Power Control (Command Code: 0)

- Power On: `<CR>*0001<CR>`
- Power Off: `<CR>*0000<CR>`
- Query Power Status: `<CR>*0000<CR>`
  - Response: Returns 1 for on, 0 for off

### Lamp Hours (Command Code: 30)

- Query Lamp Hours: `<CR>*3000<CR>`
  - Response: Returns a 32-bit integer representing the lamp hours

### 3D Sync (Command Code: 36)

- Enable 3D Sync: `<CR>*3601<CR>`
- Disable 3D Sync: `<CR>*3600<CR>`
- Query 3D Sync Status: `<CR>*3600<CR>`
  - Response: Returns 1 for on, 0 for off

### Hide (Command Code: 40)

- Enable Hide: `<CR>*4001<CR>`
- Disable Hide: `<CR>*4000<CR>`
- Query Hide Status: `<CR>*4000<CR>`
  - Response: Returns 1 for on, 0 for off

## Additional Commands

Based on the spreadsheet, many other commands are available but not implemented in the basic version of this component:

- Source selection
- Volume control
- Keystone adjustment
- Menu control
- Color settings
- Zoom functions
- And many more

## Response Data Parsing

The response data format varies based on the command. This component implements parsers for:

1. Boolean status values (0 or 1)
2. Multi-byte integer values (like lamp hours)
3. Text data (like model name)

## Command Rate Limiting

To avoid overwhelming the projector, this component implements:

- Command queuing
- Rate limiting (minimum interval between commands)
- Timeout handling for non-responsive commands

## References

For a full list of commands, refer to the original Acer H7550BD protocol documentation or the provided spreadsheet. 