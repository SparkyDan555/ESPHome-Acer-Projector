#include "sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace acer_projector {

static const char *const TAG = "acer_projector.sensor";

void AcerProjectorSensor::dump_config() {
  LOG_SENSOR("", "Acer Projector Sensor", this);
}

}  // namespace acer_projector
}  // namespace esphome 