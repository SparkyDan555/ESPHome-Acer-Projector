#include "text_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace acer_projector {

static const char *const TAG = "acer_projector.text_sensor";

void AcerProjectorTextSensor::dump_config() {
  LOG_TEXT_SENSOR("", "Acer Projector Text Sensor", this);
}

}  // namespace acer_projector
}  // namespace esphome 