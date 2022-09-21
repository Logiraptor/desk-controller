import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.components import uart, sensor
from esphome.const import CONF_ID, CONF_STATE, DEVICE_CLASS_VOLTAGE, ICON_RULER, UNIT_METER

MULTI_CONF = True
DEPENDENCIES = ['uart']
AUTO_LOAD = ['sensor']

uart_demo_ns = cg.esphome_ns.namespace('uart_demo')

UARTDemo = uart_demo_ns.class_('UARTDemo', cg.Component, uart.UARTDevice)

CONF_DESK_HEIGHT = "desk_height"
CONF_DESK_TARGET_HEIGHT = "desk_target_height"

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(UARTDemo),
    cv.Optional(CONF_DESK_HEIGHT): sensor.sensor_schema(
        unit_of_measurement="in",
        icon=ICON_RULER,
        accuracy_decimals=1,
        # device_class=DEVICE_CLASS_VOLTAGE
    ),
}).extend(uart.UART_DEVICE_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

    height_sens = await sensor.new_sensor(config[CONF_DESK_HEIGHT])
    cg.add(var.set_height_sensor(height_sens))

    target_sens = await sensor.new_sensor(config[CONF_DESK_TARGET_HEIGHT])
    cg.add(var.set_target_height_sensor(target_sens))