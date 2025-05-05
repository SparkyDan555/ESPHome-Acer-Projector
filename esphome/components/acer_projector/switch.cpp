#include "switch.h"
#include "esphome/core/log.h"

namespace esphome {
namespace acer_projector {

static const char *const TAG = "acer_projector.switch";

void AcerProjectorSwitch::setup() {
  // Don't do any setup for base class
}

void AcerProjectorSwitch::dump_config() {
  LOG_SWITCH("", "Acer Projector Switch", this);
}

void AcerProjectorSwitch::write_state(bool state) {
  // Base implementation, should be overridden in derived classes
  ESP_LOGW(TAG, "write_state not implemented");
}

// Power switch implementation
void AcerProjectorPowerSwitch::write_state(bool state) {
  this->parent_->set_power(state);
  this->publish_state(state);
}

// 3D mode switch implementation with projector power interlock
void AcerProjector3DModeSwitch::write_state(bool state) {
  // If trying to enable 3D mode while projector is off, force state to off
  if (state && !this->parent_->power_on_) {
    ESP_LOGW(TAG, "Cannot enable 3D mode when projector is off");
    this->publish_state(false);  // Force switch to show as OFF
    return;
  }
  
  // Otherwise, pass through to the actual method
  this->parent_->set_3d_mode(state);
  
  // State might have been rejected, so make sure UI reflects actual state
  this->publish_state(this->parent_->sync_3d_on_);
}

}  // namespace acer_projector
}  // namespace esphome 