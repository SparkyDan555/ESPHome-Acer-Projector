#include "button.h"
#include "esphome/core/log.h"

namespace esphome {
namespace acer_projector {

static const char *TAG = "acer_projector.button";

void AcerProjectorHideButton::press_action() {
  this->parent_->send_hide();
  ESP_LOGD(TAG, "Hide button pressed");
}

void AcerProjectorFreezeButton::press_action() {
  this->parent_->send_freeze();
  ESP_LOGD(TAG, "Freeze button pressed");
}

void AcerProjectorKeystoneUpButton::press_action() {
  this->parent_->send_keystone_up();
  ESP_LOGD(TAG, "Keystone up button pressed");
}

void AcerProjectorKeystoneDownButton::press_action() {
  this->parent_->send_keystone_down();
  ESP_LOGD(TAG, "Keystone down button pressed");
}

}  // namespace acer_projector
}  // namespace esphome 