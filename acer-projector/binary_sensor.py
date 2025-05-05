import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import (
    CONF_ID,
    CONF_ICON,
    ENTITY_CATEGORY_DIAGNOSTIC,
)
from . import acer_projector_ns, AcerProjector, CONF_ACER_PROJECTOR_ID

DEPENDENCIES = ["acer_projector"]

# Define custom constants
CONF_LAMP_STATUS = "lamp_status"

# Define custom icon values
ICON_LIGHTBULB = "mdi:lightbulb"

AcerProjectorBinarySensor = acer_projector_ns.class_("AcerProjectorBinarySensor", binary_sensor.BinarySensor)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_ACER_PROJECTOR_ID): cv.use_id(AcerProjector),
        cv.Optional(CONF_LAMP_STATUS): binary_sensor.binary_sensor_schema(
            AcerProjectorBinarySensor,
            icon=ICON_LIGHTBULB,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
    }
)

async def to_code(config):
    parent = await cg.get_variable(config[CONF_ACER_PROJECTOR_ID])
    
    # Generate lamp status binary sensor
    if CONF_LAMP_STATUS in config:
        conf = config[CONF_LAMP_STATUS]
        var = await binary_sensor.new_binary_sensor(conf)
        cg.add(parent.register_lamp_status_binary_sensor(var)) 