#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/switch/switch.h"
#include "esphome/components/button/button.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include <string>
#include <vector>

namespace esphome {
namespace acer_projector {

/**
 * Command codes from the Acer projector serial protocol.
 * These represent different functions that can be controlled.
 */
enum CommandCode {
  CMD_POWER_STATUS = 0,      // Power On/Off status
  CMD_LAMP_HOURS = 30,       // Lamp hours
  CMD_3D_MODE = 36,          // 3D Mode
  CMD_HIDE = 40,             // Hide
  CMD_FREEZE = 41,           // Freeze
  CMD_SOURCE = 20,           // Source selection
  CMD_KEYSTONE_UP = 42,      // Keystone up
  CMD_KEYSTONE_DOWN = 43,    // Keystone down
  CMD_VOLUME_UP = 22,        // Volume up
  CMD_VOLUME_DOWN = 23,      // Volume down
};

/**
 * Acer Projector Component for ESPHome
 * 
 * Provides control of Acer projectors via RS232 serial interface.
 * Features include:
 * - Power control with state tracking
 * - 3D mode with two-phase activation sequence (8.5 second delay between phases)
 * - Hide, freeze, and keystone adjustment buttons
 * - Lamp hours, lamp status, and model information
 * - Smart interlocks (preventing 3D mode when projector is off)
 * - Efficient polling (only checking lamp hours when projector is on)
 */
class AcerProjector : public Component, public uart::UARTDevice {
 public:
  AcerProjector() = default;

  void setup() override;
  void loop() override;
  void dump_config() override;
  void update();
  float get_setup_priority() const override { return setup_priority::DATA; }

  // Methods to register switches, sensors, etc.
  void register_power_switch(switch_::Switch *power_switch) { this->power_switch_ = power_switch; }
  void register_lamp_hours_sensor(sensor::Sensor *lamp_hours_sensor) { this->lamp_hours_sensor_ = lamp_hours_sensor; }
  void register_lamp_status_binary_sensor(binary_sensor::BinarySensor *lamp_status_sensor) {
    this->lamp_status_sensor_ = lamp_status_sensor;
  }
  void register_3d_mode_switch(switch_::Switch *sync_3d_switch) { this->sync_3d_switch_ = sync_3d_switch; }
  void register_hide_button(button::Button *hide_button) { this->hide_button_ = hide_button; }
  void register_freeze_button(button::Button *freeze_button) { this->freeze_button_ = freeze_button; }
  void register_keystone_up_button(button::Button *keystone_up_button) { this->keystone_up_button_ = keystone_up_button; }
  void register_keystone_down_button(button::Button *keystone_down_button) { this->keystone_down_button_ = keystone_down_button; }
  void register_model_text_sensor(text_sensor::TextSensor *model_text_sensor) {
    this->model_text_sensor_ = model_text_sensor;
  }

  // Command methods
  void queue_command(const std::string &cmd);
  void send_ir_command(const std::string &ir_code); // e.g. "001" for Power On
  void query_lamp_status();
  void query_lamp_hours();
  void query_model_name();
  void query_source();
  void set_power(bool state);
  void set_3d_mode(bool state);
  void send_3d_on_phase1();  // Sends IR code 056
  void send_3d_on_phase2();  // Sends IR code 062 after delay
  void send_3d_off();        // Sends IR code 057
  void send_hide();
  void send_freeze();
  void send_keystone_up();
  void send_keystone_down();
  void send_power_on();
  void send_power_off();
  
  // State variables (public to allow access from switches)
  bool power_on_{false};
  bool sync_3d_on_{false};

 protected:
  void process_incoming_();
  void handle_response_(const std::string &resp);
  void send_next_command_();

  struct Command {
    std::string command;
  };
  std::vector<Command> command_queue_{};
  bool command_pending_{false};
  uint32_t last_command_time_{0};
  static const uint32_t COMMAND_TIMEOUT_MS = 1000;
  uint32_t last_update_time_{0};
  static const uint32_t UPDATE_INTERVAL_MS = 10000;  // Update every 10 seconds
  
  // Timer for delayed 3D mode command
  uint32_t sync_3d_phase2_time_{0};
  bool sync_3d_phase2_pending_{false};
  static const uint32_t SYNC_3D_PHASE2_DELAY_MS = 8500; // 8.5 seconds

  std::string incoming_buffer_;

  // Store sensor/switch pointers
  switch_::Switch *power_switch_{nullptr};
  sensor::Sensor *lamp_hours_sensor_{nullptr};
  binary_sensor::BinarySensor *lamp_status_sensor_{nullptr};
  switch_::Switch *sync_3d_switch_{nullptr};
  button::Button *hide_button_{nullptr};
  button::Button *freeze_button_{nullptr};
  button::Button *keystone_up_button_{nullptr};
  button::Button *keystone_down_button_{nullptr};
  text_sensor::TextSensor *model_text_sensor_{nullptr};

  // State variables
  bool lamp_on_{false};
  uint32_t lamp_hours_{0};
  std::string model_name_{};
  std::string current_source_{};
};

}  // namespace acer_projector
}  // namespace esphome 