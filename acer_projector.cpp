#include "acer_projector.h"
#include "esphome/core/log.h"
#include "esphome/core/application.h"

namespace esphome {
namespace acer_projector {

static const char *const TAG = "acer_projector";

void AcerProjector::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Acer Projector...");
  // Initial queries for status - only query lamp status at startup
  this->query_lamp_status();
  // We'll query model name when the projector turns on
  ESP_LOGD(TAG, "Will query model name when projector turns on");
}

void AcerProjector::loop() {
  // Process any available serial data
  this->process_incoming_();

  // Check for command timeout
  const uint32_t now = millis();
  if (this->command_pending_ && (now - this->last_command_time_ >= COMMAND_TIMEOUT_MS)) {
    ESP_LOGW(TAG, "Command timeout");
    this->command_pending_ = false;
    // Try next command
    this->send_next_command_();
  }

  // Send next command if none pending
  if (!this->command_pending_ && !this->command_queue_.empty()) {
    this->send_next_command_();
  }
  
  // Check for 3D mode phase 2 timing - implements the 8.5 second delay for the second 3D command
  if (this->sync_3d_phase2_pending_ && (now - this->sync_3d_phase2_time_ >= SYNC_3D_PHASE2_DELAY_MS)) {
    this->sync_3d_phase2_pending_ = false;
    if (this->sync_3d_on_) {
      ESP_LOGD(TAG, "Sending delayed 3D mode phase 2 command");
      this->send_3d_on_phase2();
    }
  }
  
  // Regularly update status (lamp status, hours, etc.)
  if (now - this->last_update_time_ >= UPDATE_INTERVAL_MS) {
    this->update();
    this->last_update_time_ = now;
  }
}

void AcerProjector::dump_config() {
  ESP_LOGCONFIG(TAG, "Acer Projector:");
  this->check_uart_settings(9600);
  ESP_LOGCONFIG(TAG, "  Power: %s", this->power_on_ ? "ON" : "OFF");
  ESP_LOGCONFIG(TAG, "  Lamp Status: %s", this->lamp_on_ ? "ON" : "OFF");
  if (this->lamp_on_) {
    ESP_LOGCONFIG(TAG, "  Lamp Hours: %u", this->lamp_hours_);
  }
  ESP_LOGCONFIG(TAG, "  3D Mode: %s", this->sync_3d_on_ ? "ON" : "OFF");
  if (!this->model_name_.empty()) {
    ESP_LOGCONFIG(TAG, "  Model: %s", this->model_name_.c_str());
  }
  if (!this->current_source_.empty()) {
    ESP_LOGCONFIG(TAG, "  Source: %s", this->current_source_.c_str());
  }
}

void AcerProjector::update() {
  // Query status regularly
  this->query_lamp_status();
  
  // Only query other values if the projector is on - this saves bandwidth and processing
  if (this->lamp_on_) {
    this->query_lamp_hours();
    this->query_source();
  }
}

void AcerProjector::queue_command(const std::string &cmd) {
  this->command_queue_.push_back({cmd});
}

void AcerProjector::send_ir_command(const std::string &ir_code) {
  // Format: * 0 IR XXX\r
  std::string cmd = "* 0 IR " + ir_code + "\r";
  this->queue_command(cmd);
}

void AcerProjector::query_lamp_status() {
  // According to table entry 50: "* 0 Lamp ?\r" = Query the lamp ON/OFF
  this->queue_command("* 0 Lamp ?\r");
}

void AcerProjector::query_lamp_hours() {
  // According to table entry 51: "* 0 Lamp\r" = Query the lamp hours
  // Only poll this when lamp is on
  if (this->lamp_on_) {
    this->queue_command("* 0 Lamp\r");
  }
}

void AcerProjector::query_model_name() {
  // According to table entry 34: "* 0 IR 035\r" = Query Model name
  this->send_ir_command("035");
}

void AcerProjector::query_source() {
  // According to table entry 52: "* 0 Src ?\r" = Query source input type
  this->queue_command("* 0 Src ?\r");
}

void AcerProjector::set_power(bool state) {
  if (state) {
    this->send_power_on();
  } else {
    this->send_power_off();
    
    // When turning off the projector, also disable 3D mode
    if (this->sync_3d_on_) {
      ESP_LOGD(TAG, "Turning off 3D mode because projector is powering off");
      this->sync_3d_on_ = false;
      
      // Cancel any pending phase 2 commands
      this->sync_3d_phase2_pending_ = false;
      
      // Update the 3D mode switch if it exists
      if (this->sync_3d_switch_ != nullptr) {
        this->sync_3d_switch_->publish_state(false);
      }
    }
  }
}

void AcerProjector::set_3d_mode(bool state) {
  // If trying to turn on 3D mode but projector is off, reject the command
  if (state && !this->power_on_) {
    ESP_LOGW(TAG, "Cannot enable 3D mode when projector is off");
    // Update switch state to match actual state (off)
    if (this->sync_3d_switch_ != nullptr) {
      this->sync_3d_switch_->publish_state(false);
    }
    return;
  }

  if (state) {
    // First phase of 3D activation
    this->send_3d_on_phase1();
    
    // Schedule second phase after delay
    this->sync_3d_phase2_time_ = millis();
    this->sync_3d_phase2_pending_ = true;
  } else {
    // Turn off 3D mode
    this->send_3d_off();
    this->sync_3d_phase2_pending_ = false;  // Cancel any pending phase 2
  }
  
  this->sync_3d_on_ = state;
  if (this->sync_3d_switch_ != nullptr) {
    this->sync_3d_switch_->publish_state(this->sync_3d_on_);
  }
}

void AcerProjector::send_3d_on_phase1() {
  // First command: "* 0 IR 056" = 3D On (first phase)
  this->send_ir_command("056");
  ESP_LOGD(TAG, "Sent 3D mode phase 1 command");
}

void AcerProjector::send_3d_on_phase2() {
  // Second command after delay: "* 0 IR 062" = 3D On (second phase)
  this->send_ir_command("062");
  ESP_LOGD(TAG, "Sent 3D mode phase 2 command");
}

void AcerProjector::send_3d_off() {
  // "* 0 IR 057" = 3D Off
  this->send_ir_command("057");
  ESP_LOGD(TAG, "Sent 3D mode off command");
}

void AcerProjector::send_hide() {
  // According to table entry 29: "* 0 IR 030\r" = Hide
  this->send_ir_command("030");
}

void AcerProjector::send_freeze() {
  // According to table entry: "* 0 IR 007\r" = Freeze
  this->send_ir_command("007");
}

void AcerProjector::send_keystone_up() {
  // According to table entry: "* 0 IR 042\r" = Keystone Up
  this->send_ir_command("042");
}

void AcerProjector::send_keystone_down() {
  // According to table entry: "* 0 IR 043\r" = Keystone Down
  this->send_ir_command("043");
}

void AcerProjector::send_power_on() {
  // According to table entry 2: "* 0 IR 001\r" = Power On
  this->send_ir_command("001");
}

void AcerProjector::send_power_off() {
  // According to table entry 3: "* 0 IR 002\r" = Power Off
  this->send_ir_command("002");
}

void AcerProjector::send_next_command_() {
  if (this->command_queue_.empty() || this->command_pending_)
    return;

  Command cmd = this->command_queue_.front();
  this->command_queue_.erase(this->command_queue_.begin());

  // Send the command as ASCII
  for (char c : cmd.command) {
    this->write(c);
  }
  ESP_LOGD(TAG, "Sent command: %s", cmd.command.c_str());
  this->command_pending_ = true;
  this->last_command_time_ = millis();
}

void AcerProjector::process_incoming_() {
  while (this->available()) {
    char c = this->read();
    if (c == '\r') {
      // End of response
      if (!this->incoming_buffer_.empty()) {
        this->handle_response_(this->incoming_buffer_);
        this->incoming_buffer_.clear();
      }
    } else if (c != '\n') {
      // Skip newline characters completely
      this->incoming_buffer_ += c;
    }
  }
}

void AcerProjector::handle_response_(const std::string &resp) {
  // Don't log empty responses
  if (resp.empty()) {
    return;
  }
  
  // Reduce logging for common responses when projector is off
  if (!this->power_on_ && (resp == "*000" || resp == "Lamp 0")) {
    // Only log at verbose level for status responses when off
    ESP_LOGV(TAG, "Received response: %s", resp.c_str());
  } else {
    // Regular debug logging for other responses
    ESP_LOGD(TAG, "Received response: %s", resp.c_str());
  }
  
  this->command_pending_ = false;

  // Lamp status response: "Lamp 0" or "Lamp 1"
  if (resp.find("Lamp ") == 0) {
    bool previous_lamp_state = this->lamp_on_;
    
    if (resp.find("Lamp 1") == 0) {
      this->lamp_on_ = true;
      this->power_on_ = true;
      
      // If lamp just turned on, query model name (always refresh it)
      if (!previous_lamp_state) {
        ESP_LOGD(TAG, "Lamp turned on, querying model name");
        this->query_model_name();
      }
    } else if (resp.find("Lamp 0") == 0) {
      this->lamp_on_ = false;
      this->power_on_ = false;
      
      // If lamp is now off but was previously on, ensure 3D mode is turned off
      if (previous_lamp_state && this->sync_3d_on_) {
        ESP_LOGD(TAG, "Turning off 3D mode because lamp is now off");
        this->sync_3d_on_ = false;
        this->sync_3d_phase2_pending_ = false;  // Cancel any pending phase 2
        
        if (this->sync_3d_switch_ != nullptr) {
          this->sync_3d_switch_->publish_state(false);
        }
      }
    }
    if (this->lamp_status_sensor_ != nullptr) {
      this->lamp_status_sensor_->publish_state(this->lamp_on_);
    }
    if (this->power_switch_ != nullptr) {
      this->power_switch_->publish_state(this->power_on_);
    }
    return;
  }

  // Handle model name response (starts with "Model ")
  if (resp.find("Model ") == 0) {
    this->model_name_ = resp.substr(6); // Skip "Model " prefix
    ESP_LOGI(TAG, "Projector model: %s", this->model_name_.c_str());
    if (this->model_text_sensor_ != nullptr) {
      this->model_text_sensor_->publish_state(this->model_name_);
    }
    return;
  }
  
  // Handle lamp hours response (could be a 4-digit number)
  if (resp.length() >= 4 && resp.length() <= 5 && this->lamp_on_) {
    // Try to parse as lamp hours without using exceptions
    char* end;
    long hours = strtol(resp.c_str(), &end, 10);
    if (*end == '\0') { // Successful conversion (entire string was converted)
      this->lamp_hours_ = static_cast<uint32_t>(hours);
      if (this->lamp_hours_sensor_ != nullptr) {
        this->lamp_hours_sensor_->publish_state(this->lamp_hours_);
      }
      ESP_LOGD(TAG, "Parsed lamp hours: %u", this->lamp_hours_);
      return;
    }
  }
  
  // Handle source response
  if (resp.find("Src ") == 0) {
    this->current_source_ = resp.substr(4); // Skip "Src " prefix
    ESP_LOGD(TAG, "Current source: %s", this->current_source_.c_str());
    return;
  }

  // Send next command in queue
  this->send_next_command_();
}

}  // namespace acer_projector
}  // namespace esphome
 
 