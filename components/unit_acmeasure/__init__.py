import esphome.codegen as cg
from esphome.components import i2c

CODEOWNERS = ["@Jasionf"]
DEPENDENCIES = ["i2c"]

unit_acmeasure_ns = cg.esphome_ns.namespace("unit_acmeasure")
UnitACMeasureComponent = unit_acmeasure_ns.class_(
    "UnitACMeasureComponent",
    cg.PollingComponent,
    i2c.I2CDevice,
)

# Expose CONFIG_SCHEMA and to_code so this component can be used
# directly as a top-level YAML block (unit_acmeasure:).
from .sensor import CONFIG_SCHEMA, to_code

