import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import uart
from esphome.components import sensor
from esphome.const import CONF_ID, CONF_HEIGHT, CONF_TIMEOUT, ICON_GAUGE

DEPENDENCIES = ['uart']
AUTO_LOAD = ['sensor']

timotion = cg.esphome_ns.namespace('timotion')

DeskHeight = timotion.class_('DeskHeight', cg.Component, uart.UARTDevice)

CONFIG_SCHEMA = cv.COMPONENT_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(DeskHeight),
}).extend(uart.UART_DEVICE_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    # await uart.register_uart_device(var, config)