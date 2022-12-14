import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.components import uart
from esphome.const import CONF_ID, CONF_STATE, DEVICE_CLASS_VOLTAGE, ICON_RULER, UNIT_METER

MULTI_CONF = True
DEPENDENCIES = ['uart']
AUTO_LOAD = []

uart_demo_ns = cg.esphome_ns.namespace('uart_demo')

UARTDemo = uart_demo_ns.class_('UARTDemo', cg.Component, uart.UARTDevice)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(UARTDemo),
}).extend(uart.UART_DEVICE_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)