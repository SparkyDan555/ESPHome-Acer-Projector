import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import button
from esphome.const import (
    CONF_ID,
    ENTITY_CATEGORY_CONFIG
)
from . import CONF_ACER_PROJECTOR_ID, AcerProjector, acer_projector_ns

# Define icon constants
ICON_HIDE = "mdi:eye-off"
ICON_FREEZE = "mdi:snowflake"
ICON_KEYSTONE_UP = "mdi:perspective-more"
ICON_KEYSTONE_DOWN = "mdi:perspective-less"

AcerProjectorHideButton = acer_projector_ns.class_("AcerProjectorHideButton", button.Button)
AcerProjectorFreezeButton = acer_projector_ns.class_("AcerProjectorFreezeButton", button.Button)
AcerProjectorKeystoneUpButton = acer_projector_ns.class_("AcerProjectorKeystoneUpButton", button.Button)
AcerProjectorKeystoneDownButton = acer_projector_ns.class_("AcerProjectorKeystoneDownButton", button.Button)

CONF_HIDE_BUTTON = "hide"
CONF_FREEZE_BUTTON = "freeze"
CONF_KEYSTONE_UP_BUTTON = "keystone_up"
CONF_KEYSTONE_DOWN_BUTTON = "keystone_down"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_ACER_PROJECTOR_ID): cv.use_id(AcerProjector),
        cv.Optional(CONF_HIDE_BUTTON): button.BUTTON_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(AcerProjectorHideButton),
                cv.Optional("icon", default=ICON_HIDE): cv.icon,
            }
        ),
        cv.Optional(CONF_FREEZE_BUTTON): button.BUTTON_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(AcerProjectorFreezeButton),
                cv.Optional("icon", default=ICON_FREEZE): cv.icon,
            }
        ),
        cv.Optional(CONF_KEYSTONE_UP_BUTTON): button.BUTTON_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(AcerProjectorKeystoneUpButton),
                cv.Optional("icon", default=ICON_KEYSTONE_UP): cv.icon,
                cv.Optional("entity_category", default=ENTITY_CATEGORY_CONFIG): cv.entity_category,
            }
        ),
        cv.Optional(CONF_KEYSTONE_DOWN_BUTTON): button.BUTTON_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(AcerProjectorKeystoneDownButton),
                cv.Optional("icon", default=ICON_KEYSTONE_DOWN): cv.icon,
                cv.Optional("entity_category", default=ENTITY_CATEGORY_CONFIG): cv.entity_category,
            }
        ),
    }
)

async def to_code(config):
    projector = await cg.get_variable(config[CONF_ACER_PROJECTOR_ID])

    if CONF_HIDE_BUTTON in config:
        conf = config[CONF_HIDE_BUTTON]
        var = cg.new_Pvariable(conf[CONF_ID], projector)
        await button.register_button(var, conf)
        cg.add(projector.register_hide_button(var))

    if CONF_FREEZE_BUTTON in config:
        conf = config[CONF_FREEZE_BUTTON]
        var = cg.new_Pvariable(conf[CONF_ID], projector)
        await button.register_button(var, conf)
        cg.add(projector.register_freeze_button(var))
        
    if CONF_KEYSTONE_UP_BUTTON in config:
        conf = config[CONF_KEYSTONE_UP_BUTTON]
        var = cg.new_Pvariable(conf[CONF_ID], projector)
        await button.register_button(var, conf)
        cg.add(projector.register_keystone_up_button(var))
        
    if CONF_KEYSTONE_DOWN_BUTTON in config:
        conf = config[CONF_KEYSTONE_DOWN_BUTTON]
        var = cg.new_Pvariable(conf[CONF_ID], projector)
        await button.register_button(var, conf)
        cg.add(projector.register_keystone_down_button(var)) 