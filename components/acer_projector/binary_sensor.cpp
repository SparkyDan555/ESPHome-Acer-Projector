#include "binary_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace acer_projector {

static const char *const TAG = "acer_projector.binary_sensor";

void AcerProjectorBinarySensor::dump_config() {
  LOG_BINARY_SENSOR("", "Acer Projector Binary Sensor", this);
}

}  // namespace acer_projector
}  // namespace esphome 