import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_ID,
    CONF_ICON,
    ENTITY_CATEGORY_DIAGNOSTIC,
    STATE_CLASS_MEASUREMENT,
    UNIT_HOUR,
)
from . import acer_projector_ns, AcerProjector, CONF_ACER_PROJECTOR_ID

DEPENDENCIES = ["acer_projector"]

# Define custom constants
CONF_LAMP_HOURS = "lamp_hours"
DEVICE_CLASS_DURATION = "duration"

# Define custom icon values
ICON_LAMP_HOURS = "mdi:timer-sand"

AcerProjectorSensor = acer_projector_ns.class_("AcerProjectorSensor", sensor.Sensor)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_ACER_PROJECTOR_ID): cv.use_id(AcerProjector),
        cv.Optional(CONF_LAMP_HOURS): sensor.sensor_schema(
            AcerProjectorSensor,
            unit_of_measurement=UNIT_HOUR,
            icon=ICON_LAMP_HOURS,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_DURATION,
            state_class=STATE_CLASS_MEASUREMENT,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
    }
)

async def to_code(config):
    parent = await cg.get_variable(config[CONF_ACER_PROJECTOR_ID])
    
    # Generate lamp hours sensor
    if CONF_LAMP_HOURS in config:
        conf = config[CONF_LAMP_HOURS]
        var = await sensor.new_sensor(conf)
        cg.add(parent.register_lamp_hours_sensor(var)) 