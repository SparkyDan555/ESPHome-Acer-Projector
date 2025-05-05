import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID

DEPENDENCIES = ["uart"]
CODEOWNERS = ["@danielwhite"]

CONF_ACER_PROJECTOR_ID = "acer_projector_id"

acer_projector_ns = cg.esphome_ns.namespace("acer_projector")
AcerProjector = acer_projector_ns.class_("AcerProjector", cg.Component, uart.UARTDevice)

# Configuration schema
CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(AcerProjector),
}).extend(uart.UART_DEVICE_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config) 