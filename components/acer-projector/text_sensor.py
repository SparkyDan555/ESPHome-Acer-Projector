import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import text_sensor
from esphome.const import (
    CONF_ID,
    CONF_ICON,
    ENTITY_CATEGORY_DIAGNOSTIC,
)
from . import acer_projector_ns, AcerProjector, CONF_ACER_PROJECTOR_ID

DEPENDENCIES = ["acer_projector"]

# Define custom constants
CONF_MODEL = "model"

# Define custom icon values
ICON_INFORMATION = "mdi:information"

AcerProjectorTextSensor = acer_projector_ns.class_("AcerProjectorTextSensor", text_sensor.TextSensor)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_ACER_PROJECTOR_ID): cv.use_id(AcerProjector),
        cv.Optional(CONF_MODEL): text_sensor.text_sensor_schema(
            AcerProjectorTextSensor,
            icon=ICON_INFORMATION,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
    }
)

async def to_code(config):
    parent = await cg.get_variable(config[CONF_ACER_PROJECTOR_ID])
    
    # Generate model text sensor
    if CONF_MODEL in config:
        conf = config[CONF_MODEL]
        var = await text_sensor.new_text_sensor(conf)
        cg.add(parent.register_model_text_sensor(var)) 