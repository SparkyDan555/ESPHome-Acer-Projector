#pragma once

#include "esphome/core/component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "acer_projector.h"

namespace esphome {
namespace acer_projector {

class AcerProjectorBinarySensor : public binary_sensor::BinarySensor, public Component {
 public:
  void setup() override {}
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }
};

}  // namespace acer_projector
}  // namespace esphome 