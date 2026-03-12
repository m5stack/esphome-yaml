import esphome.codegen as cg
from esphome.components import i2c

DEPENDENCIES = ["i2c"]
CODEOWNERS = ["@Jasionf"]

m5station_axp192_ns = cg.esphome_ns.namespace("m5station_axp192")
M5StationAXP192Component = m5station_axp192_ns.class_(
    "M5StationAXP192Component", cg.PollingComponent, i2c.I2CDevice
)
