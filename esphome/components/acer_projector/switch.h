#pragma once

#include "esphome/core/component.h"
#include "esphome/components/switch/switch.h"
#include "acer_projector.h"

namespace esphome {
namespace acer_projector {

/**
 * Base switch class for Acer projector controls
 * 
 * Provides common functionality for all projector switches
 */
class AcerProjectorSwitch : public switch_::Switch, public Component {
 public:
  void setup() override;
  void dump_config() override;
  void set_parent(AcerProjector *parent) { this->parent_ = parent; }

 protected:
  void write_state(bool state) override;
  AcerProjector *parent_;
};

/**
 * Power control switch for Acer projector
 * 
 * Controls the power state of the projector
 */
class AcerProjectorPowerSwitch : public AcerProjectorSwitch {
 protected:
  void write_state(bool state) override;
};

/**
 * 3D mode control switch with power interlock
 * 
 * Controls the 3D mode with a two-phase activation sequence.
 * Includes interlock to prevent enabling when projector is off.
 * If enabled while projector is off, switch will immediately revert to off.
 */
class AcerProjector3DModeSwitch : public AcerProjectorSwitch {
 protected:
  void write_state(bool state) override;
};

}  // namespace acer_projector
}  // namespace esphome 