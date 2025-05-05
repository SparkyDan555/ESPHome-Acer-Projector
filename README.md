# ESPHome Acer Projector Component

A custom ESPHome component to control Acer H7550BD projectors (and potentially other compatible Acer models) via RS232 serial communication.

## Features

- Power on/off control
- Lamp hours monitoring with efficient polling (only when projector is on)
- Lamp status monitoring
- 3D mode control with advanced two-phase activation sequence
- Hide and freeze display functions via button entities
- Keystone adjustment controls
- Model name detection (automatically refreshed each time projector turns on)
- Smart power state handling with interlocks

## Installation

### Method 1: Local Components

1. Create a `custom_components` directory in your ESPHome configuration directory if it doesn't already exist.
2. Copy the entire `acer_projector` directory into the `custom_components` directory.
3. Add the following to your YAML configuration:

```yaml
external_components:
  - source: 
      type: local
      path: custom_components
```

### Method 2: Direct Git Reference

Add this to your YAML configuration:

```yaml
external_components:
  - source: github://your-username/your-repo@main
    components: [acer_projector]
```

Replace `your-username/your-repo` with the actual GitHub repository where you've uploaded this component.

## Hardware Setup

Connect the ESP8266/ESP32 to the projector's RS232 port using an appropriate level shifter/converter (RS232 to TTL converter) as RS232 operates at different voltage levels than the ESP.

Typical connections:
- ESP TX → RS232 converter RX
- ESP RX → RS232 converter TX
- ESP GND → RS232 converter GND

Make sure to use a power supply that can handle the requirements of both the ESP and the RS232 converter.

## YAML Configuration

```yaml
# Configure a UART component for the RS232 connection
uart:
  tx_pin: GPIO1  # Adjust according to your ESP model
  rx_pin: GPIO3  # Adjust according to your ESP model
  baud_rate: 9600
  data_bits: 8
  parity: NONE
  stop_bits: 1

# Configure the acer_projector component
acer_projector:
  id: acer_projector_device

# Configure switches
switch:
  - platform: acer_projector
    acer_projector_id: acer_projector_device
    power:
      name: "Acer Projector Power"
    sync_3d:
      name: "Acer Projector 3D Mode"

# Configure buttons
button:
  - platform: acer_projector
    acer_projector_id: acer_projector_device
    hide:
      name: "Acer Projector Hide Display"
      icon: "mdi:eye-off"
    freeze:
      name: "Acer Projector Freeze"
      icon: "mdi:snowflake"
    keystone_up:
      name: "Acer Projector Keystone Up"
      icon: "mdi:perspective-more"
      entity_category: config
    keystone_down:
      name: "Acer Projector Keystone Down"
      icon: "mdi:perspective-less"
      entity_category: config

# Configure sensors
sensor:
  - platform: acer_projector
    acer_projector_id: acer_projector_device
    lamp_hours:
      name: "Acer Projector Lamp Hours"
      icon: "mdi:timer-sand"

# Configure binary sensors
binary_sensor:
  - platform: acer_projector
    acer_projector_id: acer_projector_device
    lamp_status:
      name: "Acer Projector Lamp Status"

# Configure text sensors
text_sensor:
  - platform: acer_projector
    acer_projector_id: acer_projector_device
    model:
      name: "Acer Projector Model"
```

See `example_config.yaml` for a complete configuration example.

## Home Assistant Integration

After setting up the component and connecting to Home Assistant, you'll get these entities:

- **Switch**: Control power and 3D mode features
- **Button**: Control hide display, freeze, and keystone adjustment functions
- **Sensor**: Monitor lamp hours (only polls when projector is on)
- **Binary Sensor**: Monitor lamp status (on/off)
- **Text Sensor**: Show projector model (updated each time projector is powered on)

## Special Features

### Advanced 3D Mode Control
This component implements a specialized 3D mode activation sequence:
1. When 3D mode is turned on, it first sends IR code 056
2. After 8.5 seconds, it sends the follow-up IR code 062 (only if 3D mode is still enabled)
3. When 3D mode is turned off, it sends IR code 057

### Smart Interlocks
- 3D mode cannot be enabled when the projector is off
- If 3D mode is on and projector is turned off, 3D mode is automatically disabled
- 3D mode switch will immediately revert to off position if attempted when projector is off

### Efficient Polling
- Lamp hours are only queried when the projector is on
- Model name is automatically queried each time the projector turns on
- Reduced log verbosity for common status messages when projector is off

## Command Protocol Information

This component implements the Acer serial protocol using IR command codes over the RS232 interface:

- Format: `* 0 IR XXX\r` where XXX is the IR code
- Lamp status query: `* 0 Lamp ?\r` 
- Lamp hours query: `* 0 Lamp\r`

### Key IR Codes Used

- Power on: `001`
- Power off: `002`
- 3D mode phase 1: `056`
- 3D mode phase 2: `062`
- 3D mode off: `057`
- Hide display: `030`
- Freeze display: `007`
- Keystone up: `042`
- Keystone down: `043`
- Query model: `035`

## Troubleshooting

- **No response from projector**: Check your wiring, especially TX/RX connections (they might need to be swapped).
- **Incorrect values**: Make sure baud rate and serial settings match the projector requirements.
- **Connection issues**: Verify the RS232 converter is powered correctly and the voltage levels are appropriate.
- **3D mode not activating**: Remember the 3D mode uses a two-phase activation with an 8.5 second delay between commands.
- **Switch state mismatch**: Power-state interlocks might be preventing certain functions; check if the projector is on.

## License

This component is licensed under the MIT License.

## Acknowledgments

- Based on the Acer H7550BD serial command protocol.
- Developed for integration with the ESPHome ecosystem. 