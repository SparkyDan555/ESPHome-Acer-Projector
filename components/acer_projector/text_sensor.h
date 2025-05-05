#pragma once

#include "esphome/core/component.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "acer_projector.h"

namespace esphome {
namespace acer_projector {

class AcerProjectorTextSensor : public text_sensor::TextSensor, public Component {
 public:
  void setup() override {}
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }
};

}  // namespace acer_projector
}  // namespace esphome 