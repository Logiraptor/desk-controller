import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.components import uart, binary_sensor, button, output, sensor, switch, text_sensor
from esphome.const import CONF_ID, CONF_STATE, DEVICE_CLASS_VOLTAGE, ICON_RULER, UNIT_METER

MULTI_CONF = True
DEPENDENCIES = ['uart']
AUTO_LOAD = ['binary_sensor', 'button', 'output', 'sensor', 'switch', 'text_sensor']

uart_demo_ns = cg.esphome_ns.namespace('uart_demo')

UARTDemo = uart_demo_ns.class_('UARTDemo', cg.Component, uart.UARTDevice)
UARTDemoBOutput = uart_demo_ns.class_("UARTDemoBOutput", output.BinaryOutput)
UARTDemoFOutput = uart_demo_ns.class_("UARTDemoFOutput", output.FloatOutput)
UARTDemoSwitch = uart_demo_ns.class_("UARTDemoSwitch", switch.Switch, cg.Component)
UARTDemoButton = uart_demo_ns.class_("UARTDemoButton", button.Button, cg.Component)

CONF_DESK_HEIGHT = "desk_height"

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

    sens = await sensor.new_sensor(id: "desk_height")
    cg.add(var.set_the_sensor(sens))