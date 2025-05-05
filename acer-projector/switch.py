import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch
from esphome.const import (
    CONF_ID,
    CONF_ICON,
    CONF_NAME,
    ENTITY_CATEGORY_CONFIG,
    ENTITY_CATEGORY_DIAGNOSTIC,
    ENTITY_CATEGORY_NONE,
    CONF_ENTITY_CATEGORY,
)
from . import acer_projector_ns, AcerProjector, CONF_ACER_PROJECTOR_ID

DEPENDENCIES = ["acer_projector"]

# Define custom constants
CONF_POWER_SWITCH = "power"
CONF_3D_MODE_SWITCH = "sync_3d"  # Keep this as sync_3d for backward compatibility
CONF_TYPE = "type"
DEVICE_CLASS_SWITCH = "switch"

# Define custom icon values
ICON_POWER = "mdi:projector"
ICON_3D = "mdi:video-3d-variant"

TYPES = {
    CONF_POWER_SWITCH: {
        "name": "Power",
        "icon": ICON_POWER,
        "entity_category": ENTITY_CATEGORY_NONE,
    },
    CONF_3D_MODE_SWITCH: {
        "name": "3D Mode",
        "icon": ICON_3D,
        "entity_category": ENTITY_CATEGORY_NONE,
    },
}

# Custom switch classes
AcerProjectorSwitch = acer_projector_ns.class_("AcerProjectorSwitch", switch.Switch)
AcerProjectorPowerSwitch = acer_projector_ns.class_("AcerProjectorPowerSwitch", AcerProjectorSwitch)

# For the 3D Mode switch, we keep the old class name for compatibility but it refers to the new implementation
AcerProjector3DSyncSwitch = acer_projector_ns.class_("AcerProjector3DModeSwitch", AcerProjectorSwitch)

# Validate a switch configuration
def validate_switch(config):
    if CONF_NAME not in config:
        config = config.copy()
        config[CONF_NAME] = TYPES[config[CONF_TYPE]]["name"]
    return config

# Config schema for the switch component
CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(CONF_ACER_PROJECTOR_ID): cv.use_id(AcerProjector),
    cv.Optional(CONF_POWER_SWITCH): switch.switch_schema(
        AcerProjectorPowerSwitch,
        icon=ICON_POWER,
        entity_category=ENTITY_CATEGORY_NONE,
    ),
    cv.Optional(CONF_3D_MODE_SWITCH): switch.switch_schema(
        AcerProjector3DSyncSwitch,
        icon=ICON_3D,
        entity_category=ENTITY_CATEGORY_NONE,
    ),
})

async def to_code(config):
    parent = await cg.get_variable(config[CONF_ACER_PROJECTOR_ID])
    
    # Generate code for power switch if configured
    if CONF_POWER_SWITCH in config:
        conf = config[CONF_POWER_SWITCH]
        var = await switch.new_switch(conf)
        cg.add(var.set_parent(parent))
        cg.add(parent.register_power_switch(var))
    
    # Generate code for 3D mode switch if configured
    if CONF_3D_MODE_SWITCH in config:
        conf = config[CONF_3D_MODE_SWITCH]
        var = await switch.new_switch(conf)
        cg.add(var.set_parent(parent))
        cg.add(parent.register_3d_mode_switch(var)) 